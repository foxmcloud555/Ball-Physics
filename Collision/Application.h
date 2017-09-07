#ifndef APPLICATION_H
#define APPLICATION_H

#define WIN32_LEAN_AND_MEAN

#include <assert.h>

#include <stdio.h>
#include <windows.h>
#include <d3d11.h>

#include "CommonApp.h"
#include "CommonMesh.h"
#include <vector>

#include "Sphere.h"

class HeightMap;


using namespace std;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//
// An example of creating a shader of your own, that fits in
// relatively neatly with the CommonApp functionality.
//
// Edit the shader as the program runs, then Alt+Tab back to it and
// press F5 to reload the shader. Instant feedback!
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//struct SortSpheres
//{
//	bool operator()(Sphere* s1, Sphere* s2)
//	{
//		// Sort the spheres in the x position. So each sphere is of a higher x value.
//		return s1->GetPosition().x < s2->GetPosition().x;
//	}
//};


class Application:
public CommonApp
{
public:
	static Application* s_pApp;
protected:
	bool HandleStart();
	void HandleStop();
	void HandleUpdate();
	void HandleRender();
	//bool SortSpheres(Sphere* s1, Sphere* s2);
private:
	float m_frameCount;

	bool m_reload;
	bool m_isDebugOn = false;
	LARGE_INTEGER m_timer;

	float m_rotationAngle;
	float m_cameraZ;
	bool m_bWireframe;

	int m_cameraState;

	HeightMap* m_pHeightMap;
	
	vector<Sphere*> m_pSpheres;

	
	int m_numberOfSpheres = 1;

	void ReloadShaders();

	
	
};

#endif