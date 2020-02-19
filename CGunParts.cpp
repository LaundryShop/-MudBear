#include "CGunParts.h"

CGunParts::CGunParts()
{
}

CGunParts::~CGunParts()
{
}

//������.
void CGunParts::Init()
{
	m_Pos = D3DXVECTOR3(0.5f, 0.6f, 0.5f);
	m_Rot = D3DXVECTOR3(0.0f, 1.5f, 0.0f);
	m_fScale = 0.4f;
}

//�ړ�.
void CGunParts::Move(bool m_Moveflag)
{
	if (m_Moveflag == true) {
		m_Pos.z += 0.1f;
	}
}

//�����X�V.
void CGunParts::Update()
{
	//ү������Ă���ĂȂ��ꍇ�ͽٰ����.
	if (m_pStaticMesh == nullptr) {
		return;
	}
	m_pStaticMesh->SetPosition(m_Pos);
	m_pStaticMesh->SetRotation(m_Rot);
	m_pStaticMesh->SetScale(m_fScale);
}

//���f���f�[�^�̏����Ǘ�.
void CGunParts::MeshCreate()
{
	AttachModel(m_pResource->GetStaticMesh(enStaticMesh::GunParts));
	m_Pos = D3DXVECTOR3(0.5f, 0.6f, 0.5f);
	m_Rot = D3DXVECTOR3(0.0f, 1.5f, 0.0f);
	m_fScale = 0.4f;
}
