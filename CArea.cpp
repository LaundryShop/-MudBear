#include "CArea.h"

CArea::CArea()
	: m_pArea(nullptr)
	, m_vPos(0.0f,0.0f,0.0f)
	, m_FadeFlag(false)
	, m_fAlpha(1.0f)
	, m_FadeInCount(0)
	, m_FadeOutCount(0)
	, m_FadeInEndFlag(false)
	, m_FadeOutEndFlag(false)
{
}

CArea::~CArea()
{
	Release();
}


//in‚Æout•ª‚¯‚È‚­‚Ä‚¢‚¢‚±‚±‚Å“_–Å‚³‚¹‚é.
void CArea::AreaFade()
{

	if (m_FadeFlag == true) {
		if (m_FadeOutEndFlag == true) {
			if (m_FadeInCount % 2 == 0) {
				m_fAlpha -= 0.01f;
				if (m_fAlpha <= 0.0f) {
					m_FadeOutEndFlag = false;
					m_FadeInEndFlag = true;
				}
				m_FadeInCount = 0;
			}
			m_FadeInCount++;
		}
		if (m_FadeInEndFlag == true) {
			if (m_FadeOutCount % 2 == 0) {
				m_fAlpha += 0.01f;
				if (m_fAlpha >= 0.5f) {
					m_FadeInEndFlag = false;
					m_FadeOutEndFlag = true;
				}
				m_FadeOutCount = 0;
			}
			m_FadeOutCount++;
		}
	}
	
}




void CArea::AreaFade_Disp()
{
	m_FadeFlag = true;
	m_fAlpha = 1.0f;
	m_FadeInEndFlag = false;
	m_FadeOutEndFlag = true;

}


void CArea::Load(D3DXMATRIX mView, D3DXMATRIX mProj, D3DXVECTOR3 vLight, D3DXVECTOR3 vCamera)
{
	m_mView = mView;
	m_mProj = mProj;
	m_vLight = vLight;
	m_vCamera = vCamera;
}

void CArea::Render( D3DXVECTOR3 vPos)
{
	m_pArea->SetBlend(true);
	m_pArea->SetPosition(vPos);
	m_pArea->SetAlpha(m_fAlpha);
	
	m_pArea->Render(m_mView,m_mProj,m_vLight, m_vCamera);
	m_pArea->SetBlend(false);
}

void CArea::Release()
{
	m_pArea = nullptr;
}

