#include "CGrassGround.h"

CGrassGround::CGrassGround()
{
}

CGrassGround::~CGrassGround()
{
}

//�����X�V.
void CGrassGround::Update()
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
void CGrassGround::MeshCreate()
{
	AttachModel(m_pResource->GetStaticMesh(enStaticMesh::GrassGround));
	m_Pos = D3DXVECTOR3(0.0f, 0.19f, 14.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fScale = 1.0f;

	//���̒n�ʂ̍��W�ʒu��ݒ�.
	for (int i = GRASS_GROUND_MIDDLE; i < GRASS_GROUND_MAX; i++) {
		for (int j = 0; j < GRASS_GROUND_MIDDLE; j++) {
			m_GrassGround[j] = D3DXVECTOR3(31.0f, -2.19f, 14.0f + (23 * j));
		}
		m_GrassGround[i] = D3DXVECTOR3(-31.0f, -2.19f, 14.0f + (23 * (i - GRASS_GROUND_MIDDLE)));
	}
}
