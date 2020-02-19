#ifndef CHANI_H
#define CHANI_H

#include "MyMacro.h"
#include "CDX9Mesh.h"

class CArea
{
public:
	CArea();
	~CArea();

	void AreaFade();



	void AreaFade_Disp();

	void Load(D3DXMATRIX mView, D3DXMATRIX mProj, D3DXVECTOR3 vLight, D3DXVECTOR3 vCamera);

	void Render(D3DXVECTOR3 vPos);
	void Release();

	bool GetFadeFlag() { return m_FadeFlag; }

	void Attach(CDX9Mesh* CDX9Mesh) { m_pArea = CDX9Mesh; }

private:

	D3DXMATRIX	m_mView;
	D3DXMATRIX	m_mProj;
	D3DXVECTOR3 m_vLight;
	D3DXVECTOR3 m_vCamera;

	CDX9Mesh*   m_pArea;

	D3DXVECTOR3 m_vPos;
	float       m_fAlpha;

	bool m_FadeFlag;
	bool m_FadeInEndFlag;
	bool m_FadeOutEndFlag;
	int m_FadeInCount;
	int m_FadeOutCount;


};



#endif // !CHANI_H