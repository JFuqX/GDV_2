/**
 * @file mandelbrot_shader.fx
 * @author John Reinecke
 * Contact: j.reinecke@skyridge.eu
 */

#include "yoshix.h"
#include <iostream>

using namespace gfx;

// #############################################################################

struct VSPerFrameConstants
{
	float m_VSViewProjectionMatrix[16];
	float m_VSWorldMatrix[16];
};

struct PSPerObjectConstants
{
	float m_Resolution[2];
	float m_Time;
	float m_Padding;
};

// #############################################################################

class CApplication : public IApplication
{
public:

	CApplication();
	virtual ~CApplication();

public:
	//own function to increase time
	void increaseTime(float TimeAmount);

private:

	float m_FieldOfViewY;             // Vertical view angle of the camera
	float m_Time;					  // Time variable for shader momvement

	float m_Worldmatrix[16];
	float m_ViewMatrix[16];
	float m_ProjectionMatrix[16];

	BHandle m_pVSPerFrameConstants;
	BHandle m_pPSPerObjectConstants;
	BHandle m_pVertexShader;
	BHandle m_pPixelShader;
	BHandle m_pMaterial;
	BHandle m_pRectangleMesh;

private:

	virtual bool InternOnCreateConstantBuffers();
	virtual bool InternOnReleaseConstantBuffers();
	virtual bool InternOnCreateShader();
	virtual bool InternOnReleaseShader();
	virtual bool InternOnCreateMaterials();
	virtual bool InternOnReleaseMaterials();
	virtual bool InternOnCreateMeshes();
	virtual bool InternOnReleaseMeshes();
	virtual bool InternOnResize(int _Width, int _Height);
	virtual bool InternOnUpdate();
	virtual bool InternOnFrame();
};

// #############################################################################

CApplication::CApplication()
	: m_FieldOfViewY(60.0f)        // Set the vertical view angle of the camera to 60 degrees.
	, m_Time(1.0f)
	, m_pVSPerFrameConstants(nullptr)
	, m_pPSPerObjectConstants(nullptr)
	, m_pVertexShader(nullptr)
	, m_pPixelShader(nullptr)
	, m_pMaterial(nullptr)
	, m_pRectangleMesh(nullptr)
{
	std::cout << '\n';
	std::cout << "______________________CApplication______________________";
	std::cout << '\n';
}

// #############################################################################

CApplication::~CApplication()
{
}

// #############################################################################

// Extra function to increase time/travel
void CApplication::increaseTime(float TimeAmount)
{
	std::cout << '\n';
	std::cout << "_________________________TIME__________________________";
	std::cout << '\n';
	std::cout << "__________________________" << m_Time << "___________________________";

	m_Time = m_Time + TimeAmount;
	//m_Time of 25.00f is the maximum before the shader shows single pixels
	if (m_Time >= 25.00f) {
		m_Time = 0.0f;
		std::cout << '\n';
		std::cout << "_____________________________RESETTING_______________________________";
		std::cout << '\n';
	}

}

// #############################################################################

bool CApplication::InternOnCreateConstantBuffers()
{
	std::cout << '\n';
	std::cout << "______________________InternOnCreateConstantBuffers______________________";
	std::cout << '\n';

	CreateConstantBuffer(sizeof(VSPerFrameConstants), &m_pVSPerFrameConstants);
	CreateConstantBuffer(sizeof(PSPerObjectConstants), &m_pPSPerObjectConstants);

	return true;
}

// #############################################################################

bool CApplication::InternOnReleaseConstantBuffers()
{
	ReleaseConstantBuffer(m_pVSPerFrameConstants);
	ReleaseConstantBuffer(m_pPSPerObjectConstants);

	return true;
}

// #############################################################################

bool CApplication::InternOnCreateShader()
{
	std::cout << '\n';
	std::cout << "______________________InternOnCreateShader______________________";
	std::cout << '\n';

	CreateVertexShader("..\\data\\shader\\mandelbrot_julia.fx", "VSShader", &m_pVertexShader);
	CreatePixelShader("..\\data\\shader\\mandelbrot_julia.fx", "PSShader", &m_pPixelShader);

	return true;
}

// #############################################################################

bool CApplication::InternOnReleaseShader()
{
	std::cout << '\n';
	std::cout << "______________________InternOnReleaseShader______________________";
	std::cout << '\n';

	ReleaseVertexShader(m_pVertexShader);
	ReleasePixelShader(m_pPixelShader);

	return true;
}

// #############################################################################

bool CApplication::InternOnCreateMaterials()
{
	std::cout << '\n';
	std::cout << "______________________InternOnCreateMaterials______________________";
	std::cout << '\n';

	SMaterialInfo Materialinformation;

	Materialinformation.m_NumberOfTextures = 0;
	Materialinformation.m_NumberOfVertexConstantBuffers = 1;
	Materialinformation.m_pVertexConstantBuffers[0] = m_pVSPerFrameConstants;
	Materialinformation.m_NumberOfPixelConstantBuffers = 1;
	Materialinformation.m_pPixelConstantBuffers[0] = m_pPSPerObjectConstants;
	Materialinformation.m_pVertexShader = m_pVertexShader;
	Materialinformation.m_pPixelShader = m_pPixelShader;
	Materialinformation.m_NumberOfInputElements = 1;
	Materialinformation.m_InputElements[0].m_Type = SInputElement::Float3;
	Materialinformation.m_InputElements[0].m_pName = "POSITION";


	CreateMaterial(Materialinformation, &m_pMaterial);

	return true;
}

// #############################################################################

bool CApplication::InternOnReleaseMaterials()
{
	std::cout << '\n';
	std::cout << "______________________InternOnReleaseMaterials______________________";
	std::cout << '\n';

	ReleaseMaterial(m_pMaterial);
	return true;
}

// #############################################################################

bool CApplication::InternOnCreateMeshes()
{
	std::cout << '\n';
	std::cout << "______________________InternOnCreateMeshes______________________";
	std::cout << '\n';

	//Simple Rectangle to project the Shader on, cause mandelbrot doesnt need 3 dimensional space in order to be displayed correctly

	float RectangleVertices[][3] =
	{

		{ -2.0f, -2.0f, 0.0f },
		{ -2.0f, 2.0f, 0.0f },
		{ 2.0f, 2.0f, 0.0f },
		{ 2.0f, -2.0f, 0.0f },
	};

	int RectangleIndices[][3] =
	{
		{ 0, 3, 2, },
		{ 0, 2, 1, },
	};

	SMeshInfo Meshinformation;

	Meshinformation.m_pVertices = &RectangleVertices[0][0];
	Meshinformation.m_NumberOfVertices = 4;
	Meshinformation.m_pIndices = &RectangleIndices[0][0];
	Meshinformation.m_NumberOfIndices = 6;
	Meshinformation.m_pMaterial = m_pMaterial;

	CreateMesh(Meshinformation, &m_pRectangleMesh);

	return true;
}

// #############################################################################

bool CApplication::InternOnReleaseMeshes()
{
	std::cout << '\n';
	std::cout << "______________________InternOnReleaseMeshes______________________";
	std::cout << '\n';

	ReleaseMesh(m_pRectangleMesh);
	return true;
}

// #############################################################################

bool CApplication::InternOnResize(int _Width, int _Height)
{
	std::cout << '\n';
	std::cout << "______________________InternOnResize______________________";
	std::cout << '\n';

	GetProjectionMatrix(m_FieldOfViewY, static_cast<float>(_Width) / static_cast<float>(_Height), 0.01f, 1000.0f, m_ProjectionMatrix);
	return true;
}

// #############################################################################

bool CApplication::InternOnUpdate()
{
	std::cout << '\n';
	std::cout << "______________________InternOnUpdate______________________";
	std::cout << '\n';
	//                  Z
	//                  ^
	//                  |
	//                  |
	//                  |
	//                  |
	//                  | Quad
	//      ----------TTTTT--------------> X
	//                  ^
	//                  |
	//                  |
	//                  |
	//                  |
	//                  x Eye

	float Eye[] = { 0.0f, 0.0f, -1.0f };
	//doesnt need to be moved
	float At[] = { 0.0f, 0.0f,  0.0f };
	float Up[] = { 0.0f, 1.0f,  0.0f };

	GetViewMatrix(Eye, At, Up, m_ViewMatrix);
	return true;
}

// #############################################################################

bool CApplication::InternOnFrame()
{
	// Once per frame
	//Initializing PerFrameConstantsVS + PerObjectConstantsPS
	VSPerFrameConstants PerFrameConstantsVS;
	PSPerObjectConstants PerObjectConstantsPS;
	//Matching window/screen resolution
	PerObjectConstantsPS.m_Resolution[0] = 800;
	PerObjectConstantsPS.m_Resolution[1] = 600;

	//Worldmatrix 
	MulMatrix(m_ViewMatrix, m_ProjectionMatrix, PerFrameConstantsVS.m_VSViewProjectionMatrix);
	GetTranslationMatrix(0.0f, 0.0f, 0.0f, m_Worldmatrix);
	GetRotationYMatrix(0, PerFrameConstantsVS.m_VSWorldMatrix);
	MulMatrix(m_Worldmatrix, PerFrameConstantsVS.m_VSWorldMatrix, PerFrameConstantsVS.m_VSWorldMatrix);

	// #############################################################################
	// resolution change for the Zoom effect (PSPerObjectConstants -> Zooming in the object)
	// #############################################################################

	increaseTime(0.03f); //Increase time for the zoom effect
	PerObjectConstantsPS.m_Time = m_Time; //Set the time value to the object

	// #############################################################################
	// Upload
	// #############################################################################
	UploadConstantBuffer(&PerFrameConstantsVS, m_pVSPerFrameConstants);
	UploadConstantBuffer(&PerObjectConstantsPS, m_pPSPerObjectConstants);

	DrawMesh(m_pRectangleMesh);

	return true;
}

// #############################################################################

void main()
{
	std::cout << '\n';
	std::cout << "______________________main______________________";
	std::cout << '\n';

	CApplication Application;
	//Matching window/screen resolution
	//PerObjectConstantsPS.m_Resolution[0] = 800;
	//PerObjectConstantsPS.m_Resolution[1] = 600;
	RunApplication(800, 600, "Mandelbrot Fractals + Julia Fractals ", &Application);
}