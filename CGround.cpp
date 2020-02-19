#include "CGround.h"

CGround::CGround()
{
}

CGround::~CGround()
{
}
//情報を更新.
//※拡縮や回転、座標などの値をﾓﾃﾞﾙﾃﾞｰﾀに反映.
void CGround::Update()
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
void CGround::MeshCreate()
{
	AttachModel(m_pResource->GetStaticMesh(enStaticMesh::Ground));
	m_Pos = D3DXVECTOR3(0.0f, -2.19f, 14.0f);
	m_Rot = D3DXVECTOR3(0.0f,	0.0f,  0.0f);
	m_fScale = 1.0f;

	//地面の座標位置を設定.
	for (int i = 0; i < GROUND_MAX; i++) {
		m_GroundPos[i] = D3DXVECTOR3(0.0f, -2.19f, 0.0f + (23 * i));
	}
}
