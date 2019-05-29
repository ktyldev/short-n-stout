#define _USE_MATH_DEFINES

#include "Rasteriser.h"
#include "Vertex.h"
#include "Matrix.h"
#include "MD2Loader.h"
#include <cmath>
#include "Math.h"

Rasteriser _app;

bool Rasteriser::Initialise()
{
	// load _model
	if (!MD2Loader::LoadModel(_file.c_str(), _model, &Model::AddPolygon, &Model::AddVertex))
		return false;

	// set up lights
	_ambient = AmbientLight(45, 105, 115);
	_directionalLights.push_back(DirectionalLight(165, 95, 250, Vector3D(.5f, .4f, -.3f)));
	_pointLights.push_back(PointLight(Colour(200, 0, 0), Vertex(-5, 3, -1), 0.3f));

	_camera = Camera(-0.8f, 0, 0, _cameraPos);

	return true;
}

void Rasteriser::Update(Bitmap &bitmap)
{
	_modelTransformation = _modelTransformation * Matrix::XYZRotationMatrix(0.0f, 0.01f, 0.0f);

	float d = 90.0f / _camera.GetFov();
	int w = bitmap.GetWidth();
	int h = bitmap.GetHeight();

	GeneratePerspectiveMatrix(d, (float)w / (float)h);
	GenerateViewMatrix(d, w, h);
}

void Rasteriser::Render(Bitmap &bitmap)
{
	bitmap.Clear(_background);

	size_t vCount = _model.GetVertexCount();
	if (vCount == 0)
		return;

	// apply animation transformation e.g a slow rotation
	_model.ApplyTransformToLocalVertices(_modelTransformation);

	// mark polygons facing away from the camera for culling
	_model.CalculateBackfaces(_camera.GetPosition());

	// calculate vertex normals
	_model.CalculateVertexNormals();

	// lighting
	_model.CalculateLightingAmbient(_ambient);
	switch (_renderMode)
	{
	case 4: // faceted shading
		_model.CalculateFlatLightingDirectional(_directionalLights);
		_model.CalculateFlatLightingPoint(_pointLights);
		break;

	case 5: // smooth shading
		_model.CalculateVertexLightingDirectional(_directionalLights);
		_model.CalculateVertexLightingPoint(_pointLights);
		break;

	default:
		break;
	}

	// transform the vertices with respect to the camera, giving us a definite
	// viewpoint
	_model.ApplyTransformToTransformedVertices(_camera.GetViewingMatrix());

	// order the model's polygons from furthest away to closest
	_model.Sort();

	_model.ApplyTransformToTransformedVertices(_perspective);

	// transform from 3d to 2d in preparation for drawing vertices on the
	// screen
	_model.DehomogeniseTransformedVertices();

	// transform the now 2d vertices such that they are centred on the screen
	// rather than stuck in the corner
	_model.ApplyTransformToTransformedVertices(_view);

	// ＥＮＴＥＲ　　　ＴＨＥ　　　ＤＲＡＷ　　　ＰＨＡＳＥ
	Draw(bitmap);

	_model.ClearBackfaces();

	UpdateRenderMode();
}

void Rasteriser::GeneratePerspectiveMatrix(float d, float aspectRatio)
{
	_perspective = Matrix(
		{
			d / aspectRatio, 0, 0, 0,
			0, d, 0, 0,
			0, 0, d, 0,
			0, 0, 1, 0
		});
}

void Rasteriser::GenerateViewMatrix(float d, int width, int height)
{
	// adjust for screen width and height
	float w = (float)width / 2.0f;
	float h = (float)height / 2.0f;

	_view = Matrix(
		{
			w, 0, 0, w,
			0,-h, 0, h,
			0, 0, d / 2.0f, d / 2.0f,
			0, 0, 0, 1
		});
}

void Rasteriser::UpdateRenderMode()
{
	if (!_cycleModes)
		return;

	// update frames
	_modeFrames++;

	// update the render mode if enough frames have passed
	if (_modeFrames % _framesPerMode != 0)
		return;

	// reset mode frames since we're switching render mode
	_modeFrames = 0;

	if (_renderMode == _renderModes - 1)
	{
		_renderMode = 0;
		return;
	}

	_renderMode++;
}

void Rasteriser::Draw(Bitmap &bitmap)
{
	switch (_renderMode)
	{
	case 0:
		DrawWireFrame(bitmap);
		break;
	case 1:
		DrawCulledWireFrame(bitmap);
		break;
	case 2:
		DrawSolidFlat(bitmap);
		break;
	case 3:
		DrawDepthSorted(bitmap);
		break;
	case 4:
		DrawSolidFaceted(bitmap);
		break;
	case 5:
		DrawGouraud(bitmap);
		break;
	default:
		throw "invalid render mode";
	}
}

void Rasteriser::DrawWireFrame(Bitmap &bitmap)
{
	HDC hdc = bitmap.GetDC();

	SelectDCObjects(hdc);

	for (auto&& poly : _model.GetPolygons())
	{
		DrawWireframePolygon(hdc, poly, _wireframe);
	}

	DrawString(bitmap, L"Wireframe");
}

void Rasteriser::DrawCulledWireFrame(Bitmap &bitmap)
{
	HDC hdc = bitmap.GetDC();

	SelectDCObjects(hdc);

	for (auto&& poly : _model.GetPolygons())
	{
		if (poly.cull)
			continue;

		DrawWireframePolygon(hdc, poly, _wireframe);
	}

	DrawString(bitmap, L"Culled Wireframe");
}

void Rasteriser::DrawDepthSorted(Bitmap &bitmap)
{
	HDC hdc = bitmap.GetDC();
	std::vector<Polygon3D> polygons = _model.GetPolygons();
	int polyCount = (int)_model.GetPolygonCount();
	Polygon3D* poly;
	COLORREF colour;

	SelectDCObjects(hdc);

	// can't use a range-based for since we need the index to lerp
	for (int i = 0; i < polyCount; i++)
	{
		poly = &polygons[i];
		if (poly->cull)
			continue;

		// get a colour between the front and back based on the z-order
		// of the polygon
		colour = Math::LerpRGB(_back, _front, (float)i / polyCount);

		DrawFilledPolygon(hdc, *poly, colour);
	}

	DrawString(bitmap, L"Depth Sorted");
}

void Rasteriser::DrawSolidFlat(Bitmap &bitmap)
{
	HDC hdc = bitmap.GetDC();

	SelectDCObjects(hdc);

	for (auto&& poly : _model.GetPolygons())
	{
		if (poly.cull)
			continue;

		DrawFilledPolygon(hdc, poly, _flat);
	}

	DrawString(bitmap, L"Flat");
}

void Rasteriser::DrawSolidFaceted(Bitmap &bitmap)
{
	HDC hdc = bitmap.GetDC();

	SelectDCObjects(hdc);

	for (auto&& poly : _model.GetPolygons())
	{
		if (poly.cull)
			continue;

		DrawFilledPolygon(hdc, poly, poly.GetColour().ToColorRef());
	}

	DrawString(bitmap, L"Faceted");
}

void Rasteriser::DrawGouraud(Bitmap &bitmap)
{
	HDC hdc = bitmap.GetDC();
	SelectDCObjects(hdc);

	for (auto&& poly : _model.GetPolygons())
	{
		if (poly.cull)
			continue;

		PolygonGouraud(hdc, poly, poly.GetColour().ToColorRef());
	}

	DrawString(bitmap, L"Smooth");
}

void Rasteriser::MoveToPoint(HDC hdc, POINT &p, bool draw)
{
	if (draw)
	{
		LineTo(hdc, p.x, p.y);
	}
	else
	{
		MoveToEx(hdc, p.x, p.y, NULL);
	}
}

void Rasteriser::SelectDCObjects(HDC hDc)
{
	SelectObject(hDc, GetStockObject(DC_PEN));
	SelectObject(hDc, GetStockObject(DC_BRUSH));
}

void Rasteriser::SetColour(HDC hDc, COLORREF colour)
{
	SetDCPenColor(hDc, colour);
	SetDCBrushColor(hDc, colour);
}

void Rasteriser::DrawWireframePolygon(HDC hdc, Polygon3D &poly, COLORREF colour)
{
	POINT points[3];

	for (int i = 0; i < 3; i++)
	{
		points[i] = _model
			.GetTransformedVertex(poly, i)
			.ToPoint();
	}

	SetColour(hdc, colour);

	MoveToPoint(hdc, points[0], false);
	MoveToPoint(hdc, points[1], true);
	MoveToPoint(hdc, points[2], true);
	MoveToPoint(hdc, points[0], true);
}

void Rasteriser::DrawFilledPolygon(HDC hdc, Polygon3D &poly, COLORREF colour)
{
	Vertex vertices[3];
	POINT points[3];

	_model.GetTransformedVertices(poly, vertices);
	for (int i = 0; i < 3; i++)
	{
		points[i] = vertices[i].ToPoint();
	}

	SetColour(hdc, colour);
	Polygon(hdc, points, 3);
}

void Rasteriser::PolygonGouraud(HDC hdc, Polygon3D &poly, COLORREF colour)
{
	Vertex v[3];

	_model.GetTransformedVertices(poly, v);
	Vertex::OrderPolyVerticesByAscendingY(v);

	SetColour(hdc, colour);

	Vector3D side = v[2] - v[0];
	// gradient of the side between 0 and 2
	float slope = side.GetY() / side.GetX();

	// relative y-coord of the middle point
	float midpointY = v[1].GetY() - v[0].GetY();
	float ratio = abs(midpointY / side.GetY());

	Vertex midPoint = Vertex(
		v[0].GetX() + (midpointY * side.GetX()) / side.GetY(),
		v[1].GetY());

	// set the colour of the temporary vertex
	midPoint.SetColour(Colour::Lerp(v[0].GetColour(), v[2].GetColour(), ratio));
	midPoint.SetNormal(Math::LerpVector3D(v[0].GetNormal(), v[2].GetNormal(), ratio));
	
	bool midpointLeft = midPoint.GetX() < v[1].GetX();
	Vertex left = midpointLeft ? midPoint : v[1];
	Vertex right = midpointLeft ? v[1] : midPoint;

	FillBottomFlatTriangle(hdc, v[0], left, right);
	FillTopFlatTriangle(hdc, v[2], left, right);
}

void Rasteriser::RasterLine(HDC hdc, float left, float right, int y,
	COLORREF colourLeft, COLORREF colourRight)
{
	// overestimate a little bit to get pixel coords for left and right points
	int xLeft = (int)floor(left - 0.5f);
	int xRight = (int)ceil(right + 0.5f);

	// set up some values ahead of time to avoid recalculating them in the loop
	float j = 0;
	int r = xRight - xLeft;
	int i = xLeft;

	// colour at current position
	COLORREF colour;
	// for each pixel in line
	for (int x = xLeft; x < xRight; x++)
	{
		colour = Math::LerpRGB(colourLeft, colourRight, j / r);
		SetPixelV(hdc, x, y, colour);

		j++;
	}
}

void Rasteriser::FillBottomFlatTriangle(HDC hdc, Vertex &tip, Vertex &botLeft, Vertex &botRight)
{
	float slopeLeft =
		(float)((int)botLeft.GetX() - (int)tip.GetX()) /
		(float)((int)botLeft.GetY() - (int)tip.GetY());
	float slopeRight =
		(float)((int)botRight.GetX() - (int)tip.GetX()) /
		(float)((int)botRight.GetY() - (int)tip.GetY());

	COLORREF colourTip = tip.GetColour().ToColorRef();
	COLORREF colourLeft = botLeft.GetColour().ToColorRef();
	COLORREF colourRight = botRight.GetColour().ToColorRef();
	COLORREF gradientLeft, gradientRight;

	int yTip = (int)tip.GetY();
	int yBot = (int)botLeft.GetY(); // same as bot right, could use either
	// height of triangle we're about to draw
	int h = yBot - yTip;
	// interpolant for side gradients
	float t = 0;
	// iterator to get the interpolant
	float j = 0;
	// iterators for the current y coord being rasterized
	float xLeft = tip.GetX();
	float xRight = tip.GetX();

	// for each horizontal coordinate in the triangle
	for (int y = yTip; y <= yBot; y++)
	{
		// set gradients to value at current distance down the side
		gradientLeft = Math::LerpRGB(colourTip, colourLeft, t);
		gradientRight = Math::LerpRGB(colourTip, colourRight, t);

		RasterLine(hdc, xLeft, xRight, y, gradientLeft, gradientRight);

		// move along the triangle's sides
		xLeft += slopeLeft;
		xRight += slopeRight;

		// update for the next round
		j++;
		t = j / h;
	}
}

// 1 and 2 are the flat side
void Rasteriser::FillTopFlatTriangle(HDC hdc, Vertex &tip, Vertex &botLeft, Vertex &botRight)
{
	float slopeLeft =
		(float)((int)tip.GetX() - (int)botLeft.GetX()) /
		(float)((int)tip.GetY() - (int)botLeft.GetY());
	float slopeRight =
		(float)((int)tip.GetX() - (int)botRight.GetX()) /
		(float)((int)tip.GetY() - (int)botRight.GetY());

	COLORREF colourTip = tip.GetColour().ToColorRef();
	COLORREF colourLeft = botLeft.GetColour().ToColorRef();
	COLORREF colourRight = botRight.GetColour().ToColorRef();
	COLORREF gradientLeft, gradientRight;

	int yTip = (int)tip.GetY();
	int yBot = (int)botLeft.GetY(); // same as bot right, could use either
	// height of triangle we're about to draw
	int h = yTip - yBot;
	// interpolant for side gradients
	float t = 0;
	// iterator to get the interpolant
	float j = 0;
	// iterators for the current y coord being rasterized
	float xLeft = tip.GetX();
	float xRight = tip.GetX();
	// start at the tip, move upwards towards the flat side	
	for (int y = yTip; y > yBot; y--)
	{

		// set gradients to value at current distance down the side
		gradientLeft = Math::LerpRGB(colourTip, colourLeft, t);
		gradientRight = Math::LerpRGB(colourTip, colourRight, t);

		RasterLine(hdc, xLeft, xRight, y, gradientLeft, gradientRight);

		// move along the triangle's sides
		xLeft -= slopeLeft;
		xRight -= slopeRight;

		// update for the next round
		j++;
		t = j / h;
	}
}

void Rasteriser::DrawString(Bitmap &bitmap, LPCTSTR text)
{
	HDC hdc = bitmap.GetDC();
	HFONT hFont, hOldFont;

	// Retrieve a handle to the variable stock font.  
	hFont = hFont = CreateFont(48, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial"));

	// Select the variable stock font into the specified device context. 
	if (hOldFont = (HFONT)SelectObject(hdc, hFont))
	{
		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkColor(hdc, RGB(0, 0, 0));

		// Display the text string.  
		TextOut(hdc, 10, 10, text, lstrlen(text));

		// Restore the original font.        
		SelectObject(hdc, hOldFont);
	}
	DeleteObject(hFont);
}