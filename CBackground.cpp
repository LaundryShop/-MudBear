#include "CBackground.h"

CBackground::CBackground()
{
}

CBackground::~CBackground()
{
}

//�����X�V.
//���g�k���]�A���W�Ȃǂ̒l�������ް��ɔ��f.
void CBackground::Update()
{
	m_pStaticMesh->SetPosition(m_Pos);
	m_pStaticMesh->SetRotation(m_Rot);
	m_pStaticMesh->SetScale(m_fScale);
}

//���f���f�[�^�̏����Ǘ�.
void CBackground::MeshCreate()
{
	AttachModel(m_pResource->GetStaticMesh(enStaticMesh::Background));

	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 40.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fScale = 1.0f;
}

//������.
void CBackground::Init()
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 40.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f,  0.0f);
	m_fScale = 1.0f;
}

//�v���C���[�ƈꏏ�Ɉړ�.
void CBackground::Move(bool m_flag)
{
	if (m_Pos.z <= BACKGROUND_MOVING_MAX) {
		if (m_flag == true) {
			m_Pos.z += 0.1f;
		}
	}
	else{
		m_flag = false;
	}
}
