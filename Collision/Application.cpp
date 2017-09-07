#include "Application.h"
#include <algorithm>
#include "HeightMap.h"
#include "Sphere.h"


#include <vector>

Application* Application::s_pApp = NULL;

const int CAMERA_TOP = 0;
const int CAMERA_ROTATE = 1;
const int CAMERA_MAX = 2;

using namespace std;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

bool Application::HandleStart()
{
	s_pApp = this;

	m_frameCount = 0.0f;

	m_bWireframe = true;
	m_pHeightMap = new HeightMap("Resources/heightmap.bmp", 2.0f, 0.75f);


	
	Sphere::SetMesh(CommonMesh::NewSphereMesh(this, 1.0f, 16, 16));
	

	m_pSpheres.push_back( new Sphere (0));


	m_cameraZ = 50.0f;
	m_rotationAngle = 0.f;

	m_reload = false;

	ReloadShaders();

	if (!this->CommonApp::HandleStart())
		return false;

	this->SetRasterizerState( false, m_bWireframe );

	m_cameraState = CAMERA_ROTATE;

	



	return true;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void Application::HandleStop()
{
	delete m_pHeightMap;

	Sphere::DeleteMesh();
	

	this->CommonApp::HandleStop();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void Application::ReloadShaders()
{
	if( m_pHeightMap->ReloadShader() == false )
		this->SetWindowTitle("Reload Failed - see Visual Studio output window. Press F5 to try again.");
	else
		this->SetWindowTitle("Your Shader Here. Press F5 to reload shader file.");
}

void Application::HandleUpdate()
{
	if( m_cameraState == CAMERA_ROTATE )
	{
		if (this->IsKeyPressed('Q') && m_cameraZ > 38.0f )
			m_cameraZ -= 1.0f;
		
		if (this->IsKeyPressed('A'))
			m_cameraZ += 1.0f;

		if (this->IsKeyPressed('O'))
			m_rotationAngle -= .01f;
		
		if (this->IsKeyPressed('P'))
			m_rotationAngle += .01f;
	}
	
	if (this->IsKeyPressed(VK_SPACE)) {
		
		m_isDebugOn = true;
	} else {
		m_isDebugOn = false;
	}
	
	static bool dbC = false;

	if (this->IsKeyPressed('C') )	
	{
		if( !dbC )
		{
			if( ++m_cameraState == CAMERA_MAX )
				m_cameraState = CAMERA_TOP;

			dbC = true;
		}
	}
	else
	{
		dbC = false;
	}


	static bool dbW = false;
	if (this->IsKeyPressed('W') )	
	{
		if( !dbW )
		{
			m_bWireframe = !m_bWireframe;
			this->SetRasterizerState( false, m_bWireframe );
			dbW = true;
		}
	}
	else
	{
		dbW = false;
	}


	if (this->IsKeyPressed(VK_F5))
	{
		if (!m_reload)
		{
			ReloadShaders();
			m_reload = true;
		}
	}
	else
		m_reload = false;

	static bool dbR = false;
	if (this->IsKeyPressed('R') )
	{
		if( dbR == false )
		{
			static int dx = 0;
			static int dy = 0;

			for (int i = 0; i < m_pSpheres.size(); i++)
			{
				m_pSpheres[i]->SetActive(true);
				m_pSpheres[i]->DropSphere(XMFLOAT3((float)((rand() % 14 - 7.0f) - 0.5), 20.0f, (float)((rand() % 14 - 7.0f) - 0.5)));
			}
			dbR = true;
		}
	}
	else
	{
		dbR = false;
	}

	static bool dbF = false;
	if (this->IsKeyPressed('F'))
	{
		if (dbF == false)
		{
			static int dx = 0;
			static int dy = 0;

			if (m_pSpheres.size() < 2)
				m_pSpheres.push_back(new Sphere(1));
			else if (m_pSpheres.size() > 2) {
				for (int i = 0; i < m_pSpheres.size() - 2; i++) {
					m_pSpheres.pop_back();
				}
			}

			m_numberOfSpheres = 2;

			m_pSpheres[0]->SetActive(true);
			m_pSpheres[0]->DropSphere(XMFLOAT3(10, 20.0f, 0));

			m_pSpheres[1]->SetActive(true);
			m_pSpheres[1]->DropSphere(XMFLOAT3(-10, 20.0f, 0));

			m_numberOfSpheres++;

			dbF = true;
		}
	}
	else
	{
		dbF = false;
	}

	static bool dbUp = false;
	if (this->IsKeyPressed(VK_UP))
	{
		if (dbUp == false)
		{
			
				m_pSpheres.push_back(new Sphere(m_numberOfSpheres));
				m_numberOfSpheres++;
		
			dbUp = true;
		}
	}
	else
	{
		dbUp = false;
	}

	static bool dbDown = false;
	if (this->IsKeyPressed(VK_DOWN))
	{
		if (dbDown == false)
		{
			if (m_pSpheres.size() > 1) {
				m_pSpheres.pop_back();
				m_numberOfSpheres--;
			}
			dbDown = true;
		}
	}
	else
	{
		dbDown = false;
	}

	static bool dbT = false;
	if (this->IsKeyPressed('T'))
	{
		if (dbT == false)
		{
			static int dx = 0;
			static int dy = 0;
			for (int i = 0; i < m_pSpheres.size(); i++) {
				m_pSpheres[i]->SetActive(true);
				
			}
			dbT = true;
		}
	}
	else
	{
		dbT = false;
	}

	static int dx = 0;
	static int dy = 0;
	static int seg = 0;
	static bool dbN = false;

	if (this->IsKeyPressed('N') )
	{
		m_numberOfSpheres = 0;
		if( dbN == false )
		{
			if( ++seg == 2 )
			{
				seg=0;
				if( ++dx==15 ) 
				{
					if( ++dy ==15 ) dy=0;
					dx=0;
				}
			}
			
			if (seg == 0) {
				XMFLOAT3 m_SpherePos = XMFLOAT3(((dx - 7.0f) * 2) - 0.5f, 20.0f, ((dy - 7.0f) * 2) - 0.5f);
				m_pSpheres[0]->SetActive(true);
				m_pSpheres[0]->DropSphere(m_SpherePos);

			} else {
				XMFLOAT3 m_SpherePos = XMFLOAT3(((dx - 7.0f) * 2) + 0.5f, 20.0f, ((dy - 7.0f) * 2) + 0.5f);
				m_pSpheres[0]->SetActive(true);
				m_pSpheres[0]->DropSphere(m_SpherePos);
			}
			dbN = true;
		}
	}
	else
	{
		dbN = false;
	}

	static bool dbH = false;
	if (this->IsKeyPressed('H'))
	{
		if (dbH == false)
		{
			if (m_pHeightMap->GetIsDisabled()) {
				m_pHeightMap->EnableAll();
			} else {
				m_pHeightMap->DisableBelowLevel(4);
			}
			m_pHeightMap->RebuildVertexData();
			dbH = true;
		}
	}
	else
	{
		dbH = false;
	}


	static bool db1 = false;

	if (this->IsKeyPressed('1'))
	{
		if (!db1)
		{
			m_pHeightMap = new HeightMap("Resources/heightmap.bmp", 2.0f, 0.75f);

			db1 = true;
		}
	}
	else
	{
		db1 = false;
	}

	static bool db2 = false;
	if (this->IsKeyPressed('2'))
	{
		if (!db2)
		{
			m_pHeightMap = new HeightMap("Resources/otherHeightmap.bmp", 2.0f, 0.75f);

			db2 = true;
		}
	}
	else
	{
		db2 = false;
	}
	

	

	

	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);

	LARGE_INTEGER liTicksPerSecond;
	QueryPerformanceFrequency(&liTicksPerSecond);

	if (m_isDebugOn == false ||  (m_isDebugOn == true && (li.QuadPart > m_timer.QuadPart)) ) {
		

		m_timer.QuadPart = li.QuadPart + liTicksPerSecond.QuadPart;
	
		for (int i = 0; i < m_pSpheres.size(); i++) {
			m_pSpheres[i]->Update(m_pHeightMap, m_pSpheres/*, MAX_SPHERE_COUNT*/);
		}

		
		for (int i = 0; i < m_pSpheres.size(); i++) {
			m_pSpheres[i]->SetHasCollided(false); 
		}

		
	}
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void Application::HandleRender()
{
	XMVECTOR vCamera, vLookat;
	XMVECTOR vUpVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX matProj, matView;

	switch( m_cameraState )
	{
		case CAMERA_TOP:
			vCamera = XMVectorSet(0.0f, 100.0f, 0.1f, 0.0f);
			vLookat = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			matView = XMMatrixLookAtLH(vCamera, vLookat, vUpVector);
			matProj = XMMatrixOrthographicLH(64, 36, 1.5f, 5000.0f);
			break;
		case CAMERA_ROTATE:
			vCamera = XMVectorSet(sin(m_rotationAngle)*m_cameraZ, (m_cameraZ*m_cameraZ) / 50, cos(m_rotationAngle)*m_cameraZ, 0.0f);
			vLookat = XMVectorSet(0.0f, 10.0f, 0.0f, 0.0f);
			matView = XMMatrixLookAtLH(vCamera, vLookat, vUpVector);
			matProj = XMMatrixPerspectiveFovLH(float(D3DX_PI / 7), 2, 1.5f, 5000.0f);
			break;
	}

	this->EnableDirectionalLight(1, XMFLOAT3(-1.f, -1.f, -1.f), XMFLOAT3(0.55f, 0.55f, 0.65f));
	this->EnableDirectionalLight(2, XMFLOAT3(1.f, -1.f, 1.f), XMFLOAT3(0.15f, 0.15f, 0.15f));

	this->SetViewMatrix(matView);
	this->SetProjectionMatrix(matProj);

	this->Clear(XMFLOAT4(0.05f, 0.05f, 0.5f, 1.f));


	
	for (int i = 0; i < m_pSpheres.size(); i++) {
		XMMATRIX worldMtx;
		m_pSpheres[i]->UpdateMatrices(worldMtx);
	
		this->SetWorldMatrix(worldMtx);
		SetDepthStencilState( false, false );
		m_pSpheres[i]->Draw();

		this->SetWorldMatrix(worldMtx);
		SetDepthStencilState( true, true );
		m_pSpheres[i]->Draw();
	}
	SetDepthStencilState( false, true );
	m_pHeightMap->Draw( m_frameCount );

	m_frameCount++;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////



int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
{
	Application application;

	Run(&application);

	return 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
