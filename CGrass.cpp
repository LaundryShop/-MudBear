#include "CGrass.h"

CGrass::CGrass()
	: m_GrassPos()
	, m_pExcelData()
{
}

CGrass::~CGrass()
{
	SAFE_DELETE(m_pExcelData);
}
//情報を更新.
//※拡縮や回転、座標などの値をﾓﾃﾞﾙﾃﾞｰﾀに反映.
void CGrass::Update()
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
void CGrass::MeshCreate()
{
	AttachModel(m_pResource->GetStaticMesh(enStaticMesh::Grass));
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 3.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fScale = 1.0f;

	m_pExcelData = new CExcelData();
	m_pExcelData->MeshObjectData("Data\\ExcelData\\Grass.csv", m_GrassPos);
}
