#ifndef CRESOURCE_H
#define CRESOURCE_H
#include "Global.h"

//∏◊ΩÇÃëOï˚êÈåæ.
class CDX9Mesh;

enum class enStaticMesh
{
	Ground = 0,
	GrassGround,
	Background,
	GunParts,
	Pump,
	Stone,
	Grass,

	Max,
	Start = Ground
};
/**************************************************
*	ÿø∞Ω∏◊Ω.
**/
class CResource
{
public:
	CResource();
	~CResource();

	HRESULT Init(HWND hWnd,
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11);

	CDX9Mesh* GetStaticMesh(enStaticMesh enMesh);

	HRESULT LoadStaticMesh();
	HRESULT ReleaseStaticMesh();

private:
	HRESULT CreateStaticMesh(
		enStaticMesh enMesh, const char* fileName, bool Flag);
	HRESULT ReleaseStaticMesh(enStaticMesh enMesh);

	HWND					m_hWnd;
	ID3D11Device*			m_pDevice11;
	ID3D11DeviceContext*	m_pContext11;

	CDX9Mesh**				m_ppStaticMesh;
};


#endif//#ifndef CRESOURCE_H