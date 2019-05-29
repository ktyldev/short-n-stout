#pragma once
#include <vector>
#include "Polygon3D.h"
#include "Vertex.h"
#include "Matrix.h"
#include "AmbientLight.h"
#include "PointLight.h"
#include "DirectionalLight.h"

#include <Windows.h>

class Model
{
public:
	Model();
	~Model();

	// accessors
	std::vector<Polygon3D>& GetPolygons(); 
	std::vector<Vertex>& GetLocalVertices();
	std::vector<Vertex>& GetTransformedVertices();
	Vertex& GetTransformedVertex(Polygon3D &poly, int index);
	void GetTransformedVertices(Polygon3D &poly, Vertex* vertices);
	size_t GetPolygonCount() const;
	size_t GetVertexCount() const;

	// vertex methods
	void AddVertex(float x, float y, float z);
	void ApplyTransformToLocalVertices(const Matrix &transform);
	void ApplyTransformToTransformedVertices(const Matrix &transform);
	void DehomogeniseTransformedVertices();
	void CalculateVertexNormals();

	// polygon methods
	void AddPolygon(int i0, int i1, int i2);
	void CalculateBackfaces(Vertex &cameraPos);
	void ClearBackfaces();
	void Sort();

	// lighting
	void CalculateLightingAmbient(AmbientLight& light);
	// flat
	void CalculateFlatLightingDirectional(std::vector<DirectionalLight>& lights);
	void CalculateFlatLightingPoint(std::vector<PointLight>& lights);
	// vertex
	void CalculateVertexLightingDirectional(std::vector<DirectionalLight>& lights);
	void CalculateVertexLightingPoint(std::vector<PointLight>& lights);

private:
	std::vector<Polygon3D> _polygons;
	std::vector<Vertex> _vertices;
	std::vector<Vertex> _transformedVertices;

	// reflection coeefficients ordered r, g, b
	float _ka[3] = { 1.0, 1.0, 1.0 };
	float _kd[3] = { 0.5f, 0.8f, 1.0f };
	float _ks[3] = { 1.0, 1.0, 1.0 };
};

