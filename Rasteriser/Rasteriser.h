#pragma once
#include <vector>
#include "Framework.h"
#include "Matrix.h"
#include "Vertex.h"
#include "Model.h"
#include "Camera.h"

class Rasteriser : public Framework
{
public:
	bool Initialise();
	void Update(Bitmap &bitmap);
	void Render(Bitmap &bitmap);

	void GeneratePerspectiveMatrix(float d, float aspectRatio);
	void GenerateViewMatrix(float d, int width, int height);

	void UpdateRenderMode();
	// selects which render mode to use at a given time
	void Draw(Bitmap &bitmap);

	// render modes
	void DrawWireFrame(Bitmap &bitmap);
	void DrawCulledWireFrame(Bitmap &bitmap);
	void DrawDepthSorted(Bitmap &bitmap);
	void DrawSolidFlat(Bitmap &bitmap);
	void DrawSolidFaceted(Bitmap &bitmap);
	void DrawGouraud(Bitmap &bitmap);

private:
	std::string _file = ".\\teapot.md2";

	COLORREF _background = RGB(0, 0, 0);
	COLORREF _wireframe = RGB(0, 255, 0);
	// used for flat view
	COLORREF _flat = RGB(255, 150, 140);
	// used for depth sorted view
	COLORREF _front = RGB(255, 190, 240);
	COLORREF _back = RGB(40, 0, 0);
	
	// debug option
	bool _cycleModes = true;
	// how many render modes are there?
	int _renderModes = 6;
	// current render mode
	int _renderMode = 0;
	// used to count how many frames the current render mode has been used for
	int _modeFrames;
	int _framesPerMode = 90;

	Model _model;
	Camera _camera;
	Vertex _cameraPos = Vertex(0, 7, -7);

	Matrix _modelTransformation = Matrix::IdentityMatrix();
	Matrix _perspective;
	Matrix _view;

	AmbientLight _ambient;
	std::vector<DirectionalLight> _directionalLights;
	std::vector<PointLight> _pointLights;

	void MoveToPoint(HDC hdc, POINT &p, bool draw);
	void SelectDCObjects(HDC hdc);
	void SetColour(HDC hdc, COLORREF colour);

	void DrawWireframePolygon(HDC hdc, Polygon3D &poly, COLORREF colour);
	void DrawFilledPolygon(HDC hdc, Polygon3D &poly, COLORREF colour);

	void PolygonGouraud(HDC hdc, Polygon3D &poly, COLORREF colour);
	void RasterLine(HDC hdc, float start, float end, int y, COLORREF startColour, COLORREF endColour);
	void FillTopFlatTriangle(HDC hdc, Vertex &tip, Vertex &botLeft, Vertex &botRight);
	void FillBottomFlatTriangle(HDC hdc, Vertex &tip, Vertex &botLeft, Vertex &botRight);

	void DrawString(Bitmap &bitmap, LPCTSTR text);
};

