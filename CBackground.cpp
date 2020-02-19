#include "CBackground.h"

CBackground::CBackground()
{
}

CBackground::~CBackground()
{
}

//情報を更新.
//※拡縮や回転、座標などの値をﾓﾃﾞﾙﾃﾞｰﾀに反映.
void CBackground::Update()
{
	m_pStaticMesh->SetPosition(m_Pos);
	m_pStaticMesh->SetRotation(m_Rot);
	m_pStaticMesh->SetScale(m_fScale);
}

//モデルデータの情報を管理.
void CBackground::MeshCreate()
{
	AttachModel(m_pResource->GetStaticMesh(enStaticMesh::Background));

	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 40.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fScale = 1.0f;
}

//初期化.
void CBackground::Init()
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 40.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f,  0.0f);
	m_fScale = 1.0f;
}

//プレイヤーと一緒に移動.
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
