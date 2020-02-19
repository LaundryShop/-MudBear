#include "CFade.h"

CFade::CFade()
	: m_pFade(nullptr)
	, m_vPos(0.0f,0.0f,0.0f)
	, m_fAlpha(0.0f)
	, m_FadeCount(0)
	, m_FadeFlag(0)
{
}

CFade::~CFade()
{
	Release();
}

void CFade::Fade_in()
{
	switch (m_FadeFlag)
	{
	case 1:
	{

		if (m_FadeCount % 2 == 0) {
			m_fAlpha += 0.01f;
			if (m_fAlpha >= 1.0f) {
				m_FadeFlag = 2;

			}
			m_FadeCount = 0;
		}
		m_FadeCount++;
		break;
	}
	case 2:
	{
		//フェイドインが終わったとき用.
		FadeOut_Disp();
		break;
	}
	case 3:
	{
		if (m_FadeCount % 2 == 0) {
			m_fAlpha -= 0.01f;
			if (m_fAlpha <= 0.0f) {
				m_FadeFlag = 0;

			}
			m_FadeCount = 0;
		}

		m_FadeCount++;
		break;
	}
	default:
		break;
	}
}

void CFade::FadeIn_Disp()
{
	m_FadeFlag = 1;
	m_fAlpha = 0.0f;
}
void CFade::FadeOut_Disp()
{
	m_FadeFlag = 3;
	m_fAlpha = 1.0f;
}

void CFade::Load(D3DXMATRIX mView, D3DXMATRIX mProj, D3DXVECTOR3 vCamera)
{
	m_mView = mView;
	m_mProj = mProj;
	m_vCamera = vCamera;
}

void CFade::Render() 
{
	m_pFade->SetPosition(m_vPos);
	m_pFade->SetAlpha(m_fAlpha);
	m_pFade->Render();
}

void CFade::Release()
{
	m_pFade = nullptr;
}


