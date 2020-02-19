#include "CGrassGround.h"

CGrassGround::CGrassGround()
{
}

CGrassGround::~CGrassGround()
{
}

//情報を更新.
void CGrassGround::Update()
{
	//ﾒｯｼｭがｾｯﾄされてない場合はｽﾙｰする.
	if (m_pStaticMesh == nullptr) {
		return;
	}
	m_pStaticMesh->SetPosition(m_Pos);
	m_pStaticMesh->SetRotation(m_Rot);
	m_pStaticMesh->SetScale(m_fScale);
}

//モデルデータの情報を管理.
void CGrassGround::MeshCreate()
{
	AttachModel(m_pResource->GetStaticMesh(enStaticMesh::GrassGround));
	m_Pos = D3DXVECTOR3(0.0f, 0.19f, 14.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fScale = 1.0f;

	//草の地面の座標位置を設定.
	for (int i = GRASS_GROUND_MIDDLE; i < GRASS_GROUND_MAX; i++) {
		for (int j = 0; j < GRASS_GROUND_MIDDLE; j++) {
			m_GrassGround[j] = D3DXVECTOR3(31.0f, -2.19f, 14.0f + (23 * j));
		}
		m_GrassGround[i] = D3DXVECTOR3(-31.0f, -2.19f, 14.0f + (23 * (i - GRASS_GROUND_MIDDLE)));
	}
}
