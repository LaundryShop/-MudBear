#ifndef CFADE_H
#define CFADE_H

#include "MyMacro.h"
#include "CSpriteUI.h"



class CFade
{
public:
	CFade();
	~CFade();

	void Fade_in();

	void FadeIn_Disp();

	void FadeOut_Disp();

	int GetFadeFlag() { return m_FadeFlag; }

	void Load(D3DXMATRIX mView, D3DXMATRIX mProj, D3DXVECTOR3 vCamera);
	void Render();
	void Release();


	void Attach(CSpriteUI* SpriteUI) { m_pFade = SpriteUI; }

//protected:



private:

	D3DXMATRIX	m_mView;
	D3DXMATRIX	m_mProj;
	D3DXVECTOR3 m_vCamera;



	CSpriteUI* m_pFade;
	D3DXVECTOR3 m_vPos;
	float m_fAlpha;
	int m_FadeCount;
	int m_FadeFlag;
};





#endif //#ifdef CFADE_H