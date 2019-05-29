#include "Model.h"
#include <algorithm>
#include "Math.h"

Model::Model()
{
}

std::vector<Polygon3D>& Model::GetPolygons()
{
	return _polygons;
}

std::vector<Vertex>& Model::GetLocalVertices()
{
	return _vertices;
}

std::vector<Vertex>& Model::GetTransformedVertices()
{
	return _transformedVertices;
}

Vertex& Model::GetTransformedVertex(Polygon3D &poly, int index)
{
	return _transformedVertices[poly.GetIndex(index)];
}

void Model::GetTransformedVertices(Polygon3D &poly, Vertex* vertices)
{
	for (int i = 0; i < 3; i++)
	{
		vertices[i] = GetTransformedVertex(poly, i);
	}
}

size_t Model::GetPolygonCount() const
{
	return _polygons.size();
}

size_t Model::GetVertexCount() const
{
	return _vertices.size();
}

void Model::AddPolygon(int i0, int i1, int i2)
{
	_polygons.push_back(Polygon3D(i0, i1, i2));
}

void Model::AddVertex(float x, float y, float z)
{
	_vertices.push_back(Vertex(x, y, z));
}

void Model::ApplyTransformToLocalVertices(const Matrix &transform)
{
	_transformedVertices.clear();
	for (int i = 0; i < _vertices.size(); i++)
	{
		_transformedVertices.push_back(transform * _vertices[i]);
	}
}

void Model::ApplyTransformToTransformedVertices(const Matrix &transform)
{
	for (int i = 0; i < _transformedVertices.size(); i++)
	{
		_transformedVertices[i] = transform * _transformedVertices[i];
	}
}

void Model::DehomogeniseTransformedVertices()
{
	for (int i = 0; i < _transformedVertices.size(); i++)
	{
		_transformedVertices[i].Dehomogenise();
	}
}

void Model::CalculateVertexNormals()
{
	// for each vertex reset its normal to (0, 0, 0) and set the contributing count
	// to 0
	Vector3D zero = Vector3D();
	for (auto&& v : _transformedVertices)
	{
		v.SetNormal(zero);
		v.ResetContributions();
	}

	Vertex* v;
	Vector3D tempVector;
	for (auto&& poly : _polygons)
	{
		tempVector = poly.GetNormal();
		for (int i = 0; i < 3; i++)
		{
			v = &_transformedVertices[poly.GetIndex(i)];

			// add the polygon normal to the vertex normal for each of the 3 vertices
			v->AddNormal(tempVector);
		}
	}

	// once all the polygons are processed, for each vertex...
	for (auto&& v : _transformedVertices)
	{
		// divide the summed vertex normals by the contribution count
		tempVector = v.GetNormal() / (float)v.NormalContributions();

		// normalise the resulting vector
		tempVector = tempVector.ToUnitVector();

		v.SetNormal(tempVector);
	}
}

void Model::CalculateBackfaces(Vertex &cameraPos)
{
	size_t polyCount = _polygons.size();
	Polygon3D* poly;
	Vertex polyVerts[3];
	Vector3D a, b, normal, eyeVector;

	for (int i = 0; i < polyCount; i++)
	{
		poly = &_polygons[i];
		for (int j = 0; j < 3; j++)
		{
			polyVerts[j] = _transformedVertices[poly->GetIndex(j)];
		}

		a = polyVerts[0] - polyVerts[1];
		b = polyVerts[0] - polyVerts[2];

		normal = Vector3D::CrossProduct(a, b);
		normal = normal.ToUnitVector();

		poly->SetNormal(normal);

		eyeVector = (polyVerts[0] - cameraPos)
			.ToUnitVector();

		float dot = Vector3D::DotProduct(normal, eyeVector);
		poly->cull = dot < 0;
	}
}

void Model::ClearBackfaces()
{
	size_t polyCount = GetPolygonCount();

	for (int i = 0; i < polyCount; i++)
	{
		_polygons[i].cull = false;
	}
}

void Model::Sort()
{
	size_t polyCount = GetPolygonCount();
	Polygon3D* poly;
	Vertex vertices[3];

	for (size_t i = 0; i < polyCount; i++)
	{
		poly = &(_polygons[i]);
		for (int j = 0; j < 3; j++)
		{
			vertices[j] = _transformedVertices[poly->GetIndex(j)];
		}

		float avgZ =
			(vertices[0].GetZ()
				+ vertices[1].GetZ()
				+ vertices[2].GetZ()) / 3.0f;

		poly->SetZDepth(avgZ);
	}

	std::sort(_polygons.begin(), _polygons.end(),
		[](Polygon3D& a, Polygon3D& b)
	{
		return a.GetZDepth() > b.GetZDepth();
	});
}

void Model::CalculateLightingAmbient(AmbientLight& light)
{
	Colour colour;
	Colour lightColour = light.GetColour();

	for (int i = 0; i < 3; i++)
	{
		colour.Set(i, (int)(lightColour.Get(i) * _ka[i]));
	}

	for (auto&& poly : _polygons)
	{
		if (poly.cull)
			continue;

		poly.SetColour(colour);
	}
}

void Model::CalculateFlatLightingDirectional(std::vector<DirectionalLight>& lights)
{
	Colour colour;
	float temp[3]{ 0 };

	// for each polygon
	for (auto&& poly : _polygons)
	{
		if (poly.cull)
			continue;

		// get colour from ambiently lit polygon
		colour = poly.GetColour();

		// for each directional light in the collection
		for (auto&& light : lights)
		{
			Colour lightColour = light.GetColour();

			// calculate dot product of the normal vector to the light
			// source and the polygon's normal vector
			Vector3D lightDir = light.GetDirection();
			Vector3D polyNormal = poly.GetNormal();

			float dot = Vector3D::DotProduct(
				light.GetDirection(),
				poly.GetNormal());

			for (int i = 0; i < 3; i++)
			{
				// set temp rgb to light intensity,  
				temp[i] = (float)lightColour.Get(i);

				// modulate by corresponding diffuse reflectance coefficients,
				temp[i] *= _kd[i];

				// multiply temp by dot product of light and poly normals	
				temp[i] *= dot;

				// add temp to total
				colour.Set(i, colour.Get(i) + (int)temp[i]);
			}
		}

		// store colour in polygon
		poly.SetColour(colour);
	}
}

void Model::CalculateFlatLightingPoint(std::vector<PointLight>& lights)
{
	Colour colour;
	float temp[3]{ 0 };

	for (auto&& poly : _polygons)
	{
		if (poly.cull)
			continue;

		colour = poly.GetColour();

		for (auto&& light : lights)
		{
			Colour lightColour = light.GetColour();

			Vertex polyPos = _transformedVertices[poly.GetIndex(0)];
			Vector3D dir = polyPos - light.GetPosition();
			dir = dir.ToUnitVector();

			float dot = Vector3D::DotProduct(
				dir,
				poly.GetNormal());

			for (int i = 0; i < 3; i++)
			{
				temp[i] = (float)lightColour.Get(i);
				temp[i] *= _kd[i];
				temp[i] *= dot;
				temp[i] *= light.GetAttenuation();

				// add temp to total
				colour.Set(i, colour.Get(i) + (int)temp[i]);
			}
		}

		poly.SetColour(colour);
	}
}

void Model::CalculateVertexLightingDirectional(std::vector<DirectionalLight>& lights)
{
	Colour colour;
	float temp[3]{ 0 };
	Vertex* v;

	for (auto&& poly : _polygons)
	{
		// still respect poly culling
		if (poly.cull)
			continue;

		for (int i = 0; i < 3; i++)
		{
			v = &_transformedVertices[poly.GetIndex(i)];

			// get colour from ambiently lit polygon
			colour = poly.GetColour();

			for (auto&& light : lights)
			{
				Colour lightColour = light.GetColour();

				// calculate dot product of the direction of the light source
				// and the vertex's normal vector
				Vector3D lightDir = light.GetDirection();
				Vector3D vNormal = v->GetNormal();

				float dot = Vector3D::DotProduct(lightDir, vNormal);

				for (int i = 0; i < 3; i++)
				{
					temp[i] = (float)lightColour.Get(i);
					temp[i] *= _kd[i];
					temp[i] *= dot;

					colour.Set(i, colour.Get(i) + (int)temp[i]);
				}
			}

			// store colour in vertex
			v->SetColour(colour);
		}
	}
}

void Model::CalculateVertexLightingPoint(std::vector<PointLight>& lights)
{
	Colour colour;
	float temp[3]{ 0 };
	Vertex* v;

	for (auto&& poly : _polygons)
	{
		if (poly.cull)
			continue;

		for (int i = 0; i < 3; i++)
		{
			v = &(_transformedVertices[poly.GetIndex(i)]);

			colour = v->GetColour();

			for (auto&& light : lights)
			{
				Colour lightColour = light.GetColour();

				Vector3D dir = *v - light.GetPosition();
				dir = dir.ToUnitVector();

				float dot = Vector3D::DotProduct(dir, v->GetNormal());

				for (int i = 0; i < 3; i++)
				{
					temp[i] = (float)lightColour.Get(i);
					temp[i] *= _kd[i];
					temp[i] *= dot;
					temp[i] *= light.GetAttenuation();

					colour.Set(i, colour.Get(i) + (int)temp[i]);
				}
			}

			v->SetColour(colour);
		}
	}
}

Model::~Model()
{
}
