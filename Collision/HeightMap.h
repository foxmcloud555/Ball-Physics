#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

//**********************************************************************
// File:			HeightMap.h
// Description:		A simple class to represent a heightmap
// Module:			Real-Time 3D Techniques for Games
// Created:			Jake - 2010-2015
// Notes:			
//**********************************************************************

#include "Application.h"

static const char *const g_aTextureFileNames[] = {
	"Resources/Intersection.dds",
	"Resources/Intersection.dds",
	"Resources/Collision.dds",
	"Resources/MaterialMap.dds",
};

static const size_t NUM_TEXTURE_FILES = sizeof g_aTextureFileNames / sizeof g_aTextureFileNames[0];

struct FaceCollisionData
{
	XMFLOAT3 m_v0;
	XMFLOAT3 m_v1;
	XMFLOAT3 m_v2;
	XMFLOAT3 m_vNormal;
	bool m_bCollided; // Debug colouring
	bool m_bDisabled;
	XMFLOAT3 m_D;
	bool m_bSelected;
};

class HeightMap
{
public:
	HeightMap(char* filename, float gridSize, float heightRange);
	~HeightMap();

	void Draw(float frameCount);
	bool ReloadShader();
	void DeleteShader();
	bool RayCollision(XMVECTOR& rayPos, XMVECTOR rayDir, float speed, XMVECTOR& colPos, XMVECTOR& colNormN, XMFLOAT3& faceNorm);
	int DisableBelowLevel(float fY);
	int EnableAll(void);

	bool GetIsDisabled() { return m_isHeightmapDisabled; }

	void RebuildVertexData(void);
	
private:


	bool LoadHeightMap(char* filename, float gridSize, float heightRange);
	bool RayTriangle(int nFaceIndex, const XMVECTOR& rayPos, const XMVECTOR& rayDir, XMVECTOR& colPos, XMVECTOR& colNormN, float& colDist);
	bool PointPlane(const XMVECTOR& vert0, const XMVECTOR& vert1, const XMVECTOR& vert2, const XMVECTOR& pointPos);
	bool PointOverQuad(XMVECTOR& vPos, XMVECTOR& v0, XMVECTOR& v1, XMVECTOR& v2);
	void BuildCollisionData(void);

	bool m_isHeightmapDisabled = false;

	

	ID3D11Buffer *m_pHeightMapBuffer;

	int m_HeightMapWidth;
	int m_HeightMapLength;
	int m_HeightMapVtxCount;
	int m_HeightMapFaceCount;
	XMFLOAT4* m_pHeightMap;
	FaceCollisionData* m_pFaceData;
	Vertex_Pos3fColour4ubNormal3fTex2f* m_pMapVtxs;

	Application::Shader m_shader;

	ID3D11Buffer *m_pPSCBuffer;
	ID3D11Buffer *m_pVSCBuffer;

	int m_psCBufferSlot;
	int m_psFrameCount;

	int m_psTexture0;
	int m_psTexture1;
	int m_psTexture2;
	int m_psMaterialMap;
	int m_vsMaterialMap;

	int m_vsCBufferSlot;
	int m_vsFrameCount;

	ID3D11Texture2D *m_pTextures[NUM_TEXTURE_FILES];
	ID3D11ShaderResourceView *m_pTextureViews[NUM_TEXTURE_FILES];
	ID3D11SamplerState *m_pSamplerState;

};

#endif