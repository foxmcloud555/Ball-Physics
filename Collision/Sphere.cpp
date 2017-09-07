#include "Sphere.h"
#include "HeightMap.h"


CommonMesh* Sphere::m_pSphereMesh = NULL;

Sphere::Sphere()
{
}


Sphere::Sphere(int id)
{
	m_SphereID = id;
	m_SpherePos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mSphereVel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_GravityAcc = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mSphereMass = 2.0;
	spherePlaneCollisionCheck = false;
	mSphereCollided = false;
	isActive = false;
}



Sphere::~Sphere()
{}

void Sphere::Draw()
{
	if (isActive) {
		if (m_pSphereMesh)
			m_pSphereMesh->Draw();
	}
}

void Sphere::UpdateMatrices(XMMATRIX& worldMtx)
{
	worldMtx = XMMatrixTranslation(m_SpherePos.x, m_SpherePos.y, m_SpherePos.z);
}


void Sphere::SetMesh(CommonMesh* mesh)
{
	m_pSphereMesh = mesh;
}

void  Sphere::DeleteMesh()
{
	if( m_pSphereMesh )
		delete m_pSphereMesh;
}


void Sphere::DropSphere(XMFLOAT3 pos)
{
	m_SpherePos = pos;
	mSphereVel = XMFLOAT3(0.0f, 0.2f, 0.0f); // sphere is thrown up in the air slightly 
	m_GravityAcc = XMFLOAT3(0.0f, -0.05f, 0.0f); //
	spherePlaneCollisionCheck = false;
	mSphereCollided = false;
}


//*******************************************************************************************
void Sphere::Update(HeightMap* m_pHeightMap, std::vector<Sphere*> spheres)
{
	if (isActive) {
		XMVECTOR vSColPos, vSColNorm;
		XMVECTOR vSPos = XMLoadFloat3(&m_SpherePos);
		XMVECTOR vSVel = XMLoadFloat3(&mSphereVel);
		XMVECTOR vSAcc = XMLoadFloat3(&m_GravityAcc);

		vSPos += vSVel; // Really important that we add LAST FRAME'S velocity as this was how fast the collision is expecting the sphere to move
		vSVel += vSAcc; // The new velocity gets passed through to the collision so it can base its predictions on our speed NEXT FRAME

		int checkCount = 0;

		XMStoreFloat3(&mSphereVel, vSVel);
		XMStoreFloat3(&m_SpherePos, vSPos);

		mSphereSpeed = XMVectorGetX(XMVector3Length(vSVel));

		if (mSphereCollided == false) {
			
			for (int i = 0; i < spheres.size(); i++) {// go through each sphere
			
				if (GetSphereId() != spheres[i]->GetSphereId()) {// check that the sphere we are checking against isn't this sphere
					
					{
						
						float t; 
						if (TestMovingSphereSphere(this, spheres[i], t)) { //if spheres collide 
							if (t <= 1)
							{
								// calculating new movement vectors - gamasutra 
								
								XMFLOAT3 newVec;
								XMStoreFloat3(&newVec, XMLoadFloat3(&m_SpherePos) + XMLoadFloat3(&mSphereVel) * t);
								SetPosition(newVec);
								SetHasCollided(true);

								XMFLOAT3 newVec2;
								XMStoreFloat3(&newVec2, XMLoadFloat3(&spheres[i]->GetPosition()) + XMLoadFloat3(&spheres[i]->GetVelocity()) * t);
								spheres[i]->SetPosition(newVec2);
								spheres[i]->SetHasCollided(true);


								// First, find the normalized vector n from the center of 
								// circle1 to the center of circle2
								//Vector n = circle1.center - circle2.center;
								//n.normalize();
								XMVECTOR nVec = XMLoadFloat3(&this->GetPosition()) - XMLoadFloat3(&spheres[i]->GetPosition()); 
								nVec = XMVector3Normalize(nVec);
								
								XMVECTOR vel1 = XMLoadFloat3(&this->GetVelocity());
								XMVECTOR vel2 = XMLoadFloat3(&spheres[i]->GetVelocity());
								// Find the length of the component of each of the movement
								// vectors along n. 
								// a1 = v1 . n
								// a2 = v2 . n
								float a1, a2;
								a1 = XMVectorGetX(XMVector3Dot(vel1, nVec));
								a2 = XMVectorGetX(XMVector3Dot(vel2, nVec));


								// Using the optimized version, 
								// optimizedP =  2(a1 - a2)
								//              -----------
								//                m1 + m2
								//float optimizedP = (2.0 * (a1 - a2)) / (circle1.mass + circle2.mass);
								float optimizedP = (2.0 * (a1 - a2)) / (this->GetMass() + spheres[i]->GetMass());

								//change velocity of the other sphere
								XMVECTOR v1 = vel1 - optimizedP * spheres[i]->GetMass() * nVec;
								XMFLOAT3 sphereVel;
								XMStoreFloat3(&sphereVel, v1);
								this->SetVelocity(sphereVel);

								// Change the velocity of this sphere
								XMFLOAT3 newVel;
								XMStoreFloat3(&newVel, vel2 + optimizedP * this->GetMass() * nVec);
								spheres[i]->SetVelocity(newVel);

								
							}
						}
					}
				}
			}
		}

		// Just to check how many checks this frame did
		

		if (spherePlaneCollisionCheck == false)
		{
			// Collision Check and Move with HeightMap
			spherePlaneCollisionCheck = m_pHeightMap->RayCollision(vSPos, vSVel, mSphereSpeed, vSColPos, vSColNorm, faceNorm);
			if (spherePlaneCollisionCheck)
			{
				mSphereVel = BounceAndDampen();// sphere loses energy as it bounces
				XMStoreFloat3(&m_SpherePos, vSColPos);
			}
			spherePlaneCollisionCheck = false;
		}
	}
}


XMFLOAT3 Sphere::BounceAndDampen()
{
	XMFLOAT3 newVel;
//	XMVECTOR newVel;
	XMFLOAT3 dampeningCoefficient = XMFLOAT3(0.8, 0.8, 0.8);
	// The reflected Velocity is = 2 * Face normal * (sphereVel . N)
	XMStoreFloat3(&newVel, XMLoadFloat3(&mSphereVel) - (2 * XMLoadFloat3(&faceNorm)) * XMVector3Dot(XMLoadFloat3(&mSphereVel), XMLoadFloat3(&faceNorm)));
	
	// Multiply by the coefficient
	newVel = XMFLOAT3(newVel.x * dampeningCoefficient.x, newVel.y * dampeningCoefficient.y, newVel.z * dampeningCoefficient.z);
	
    
	return newVel;
}

bool Sphere::TestMovingSphereSphere(Sphere * sphere, Sphere * otherSphere, float & t) // check if sphere actually colides 
{
	

	//Vector v = v1 - v0;
	//relative motion of first sphere w/r/t stationary 2nd sphere
	XMVECTOR vectorV = XMLoadFloat3(&otherSphere->GetVelocity()) - XMLoadFloat3(&sphere->GetVelocity());

	//Vector s = s1.c - s0.c;
	// vector between the center of the spheres involved 
	XMVECTOR vectorS = XMLoadFloat3(&otherSphere->GetPosition()) - XMLoadFloat3(&sphere->GetPosition());

	//float r = s1.r + s0.r;
	// Sum of sphere radii
	float sumRadii = sphere->GetRadius() + otherSphere->GetRadius(); // sum of sphere radii

	//float c = Dot(s, s) – r * r;
	float c = XMVectorGetX(XMVector3Dot(vectorS, vectorS)) - (sumRadii * sumRadii);

	if (c < 0.0) {
		//// Spheres initially overlapping so exit directly
		t = 0.0f;
		return true;
	}

	
	//float a = Dot(v, v);
	float a = XMVectorGetX(XMVector3Dot(vectorV, vectorV));
	
	if (a < XMVectorGetX(g_XMEpsilon))// Spheres not moving relative to each other
		return false;

	//float b = Dot(v, s);
	float b = XMVectorGetX(XMVector3Dot(vectorV, vectorS));//


	if (b >= 0.0)// spheres not moving towards each other
		return false; 

	//float d = b * b – a * c;				  
	float d = (b * b) - (a * c);

	
	if (d < 0.0) //// No real-valued root, spheres do not intersect
		return false;

	
	t = (-b - sqrt(d)) / a;

	return true;
}


