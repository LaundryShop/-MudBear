#include "CGunParts.h"

CGunParts::CGunParts()
{
}

CGunParts::~CGunParts()
{
}

//初期化.
void CGunParts::Init()
{
	m_Pos = D3DXVECTOR3(0.5f, 0.6f, 0.5f);
	m_Rot = D3DXVECTOR3(0.0f, 1.5f, 0.0f);
	m_fScale = 0.4f;
}

//移動.
void CGunParts::Move(bool m_Moveflag)
{
	if (m_Moveflag == true) {
		m_Pos.z += 0.1f;
	}
}

//情報を更新.
void CGunParts::Update()
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
void CGunParts::MeshCreate()
{
	AttachModel(m_pResource->GetStaticMesh(enStaticMesh::GunParts));
	m_Pos = D3DXVECTOR3(0.5f, 0.6f, 0.5f);
	m_Rot = D3DXVECTOR3(0.0f, 1.5f, 0.0f);
	m_fScale = 0.4f;
}
