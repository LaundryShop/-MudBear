#include "CGround.h"

CGround::CGround()
{
}

CGround::~CGround()
{
}
//�����X�V.
//���g�k���]�A���W�Ȃǂ̒l�������ް��ɔ��f.
void CGround::Update()
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
void CGround::MeshCreate()
{
	AttachModel(m_pResource->GetStaticMesh(enStaticMesh::Ground));
	m_Pos = D3DXVECTOR3(0.0f, -2.19f, 14.0f);
	m_Rot = D3DXVECTOR3(0.0f,	0.0f,  0.0f);
	m_fScale = 1.0f;

	//�n�ʂ̍��W�ʒu��ݒ�.
	for (int i = 0; i < GROUND_MAX; i++) {
		m_GroundPos[i] = D3DXVECTOR3(0.0f, -2.19f, 0.0f + (23 * i));
	}
}
