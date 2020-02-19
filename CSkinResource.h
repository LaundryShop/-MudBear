#ifndef CSKINRESOURCE_H
#define CSKINRESOURCE_H
#include "Global.h"
#include "CDX9SkinMesh.h"

class CDX9SkinMesh;

enum class enStaticSkinMesh
{
	Kuma = 0,
	BossKuma,
	Max,
	Start = Kuma
};



/**********************************
*	スキン用リソースクラス.
**/

class CSkinResource
{
public:
	CSkinResource();
	~CSkinResource();

	HRESULT Init(HWND hWnd,
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11);

	CDX9SkinMesh*	GetStaticSkinMeesh(enStaticSkinMesh emSkinMesh);

	HRESULT LoadStaticSkinMesh();
	HRESULT ReleaseStaticSkinMesh();

private:
	HRESULT CreateStaticSkinMesh(
		enStaticSkinMesh enSkinMesh, const char* fileName, bool Flag);

	HRESULT ReleaseStaticSkinMesh(enStaticSkinMesh enSkinMesh);

	HWND					m_hWnd;
	ID3D11Device*			m_pDevice11;
	ID3D11DeviceContext*	m_pContext11;

	CDX9SkinMesh**			m_ppStaticSkinMesh;
	CD3DXSKINMESH_INIT		m_SkinMesh_Init;
};


#endif //#ifndef CSKINRESOURCE_H.