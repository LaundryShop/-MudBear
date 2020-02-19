#include "CHideTex.h"

CHideTex::CHideTex()
	: m_pHideTex(nullptr)
	, m_Pos(0.0f, 0.0f, 0.0f)
	, m_HideTexFlag(false)
	, m_AnimCount(0)
	, m_UV(0.0f, 0.0f)
{

}

CHideTex::~CHideTex()
{
	Release();
}




//================================================.
//		�ǂݍ��ݏ����֐�.
//================================================.
void CHideTex::Load(D3DXMATRIX mView, D3DXMATRIX mProj, D3DXVECTOR3 vCamera)
{
	m_mView = mView;
	m_mProj = mProj;
	m_vCamera = vCamera;
}

//=================================================.
//		�`�揈���֐�.
//=================================================.
void CHideTex::Render()
{
	m_pHideTex->SetPosition(m_Pos);
	m_pHideTex->SetUVPosition(m_UV);
	m_pHideTex->Render(m_mView, m_mProj, m_vCamera);
	
}

//=================================================.
//		�B���e�N�X�`�������֐�.
//=================================================.
void CHideTex::HideTex_Disp(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
	m_InitPos = m_Pos;
	m_UV = D3DXVECTOR2(0.0f, 0.0f);
	m_HideTexFlag = true;
}

//=================================================.
//		�A�j���[�V�����J�E���g.
//=================================================.
void CHideTex::Move()
{
	
	//�e�N�X�`�����W.
	//�A�j���[�V����.
	if (m_HideTexFlag == true) {
		if (m_AnimCount % 8 == 0) {
			m_UV.x += 1.0f / 3.0f;
			if (m_UV.x >= (1.0f / 3.0f * 3.0f)) {
				m_UV.y += 1.0f / 2.0f;
				m_UV.x = 0.0f;
			}
			if (m_UV.y >= (1.0f / 2.0f * 2.0f)) {
				m_HideTexFlag = false;
			}
			m_AnimCount = 0;
		}
		m_AnimCount++;
	}
}

//��������֐�.

void CHideTex::Release()
{
	//SAFE_DELETE(m_pHideTex);
}

void CHideTex::Init()
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_HideTexFlag = false;
	m_AnimCount = 0;
	m_UV = D3DXVECTOR2(0.0f, 0.0f);

}