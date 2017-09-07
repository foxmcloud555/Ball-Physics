#ifndef SPHERE_H
#define SPHERE_H


#include <assert.h>
#include <vector>
#include <stdio.h>
#include <windows.h>
#include <d3d11.h>

#include "CommonApp.h"
#include "CommonMesh.h"
class HeightMap;

class Sphere
{
public:
	Sphere();
	Sphere(int id);
	~Sphere();

	
	void Update(HeightMap* m_pHeightMap, std::vector<Sphere*> spheres );
	void Draw();
	void UpdateMatrices(XMMATRIX& worldMtx);

	static void SetMesh(CommonMesh* mesh);
	static void DeleteMesh();

	// Sets
	void SetId(int id)							{ m_SphereID = id; }
	void SetVelocity(XMFLOAT3 newVel)			{ mSphereVel = newVel; }
	void SetPosition(XMFLOAT3 newPos)			{ m_SpherePos = newPos; }
	void SetActive(bool active)					{ isActive = active; }
	void SetHasCollided(bool newCol)			{ mSphereCollided = newCol; }

	// Gets
	int GetSphereId()				{ return m_SphereID; }
	XMFLOAT3 GetPosition()	{ return m_SpherePos; }
	XMFLOAT3 GetVelocity()	{ return mSphereVel; }
	float GetRadius()		{ return mSphereRadius; }
	float GetMass()			{ return mSphereMass; }
	bool GetActive()		{ return isActive; }

	
	void DropSphere(XMFLOAT3 pos);
	//void MoveSphereToKiss();
	static bool TestMovingSphereSphere(Sphere* sphere, Sphere* otherSphere, float& t);
	


protected:
	XMFLOAT3 BounceAndDampen();
private:
	XMFLOAT3 faceNorm;

	static CommonMesh *m_pSphereMesh;
	int m_SphereID;
	XMFLOAT3 m_SpherePos;
	XMFLOAT3 mSphereVel;
	float mSphereSpeed;
	bool spherePlaneCollisionCheck;
	bool mSphereCollided;
	XMFLOAT3 m_GravityAcc;
	float mSphereRadius = 1.0;
	float mSphereMass = 1.0;

	bool isActive;
};

#endif //SPHERE_H