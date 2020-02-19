#include "CStone.h"

CStone::CStone()
	: m_StonePos()
	, m_pExcelData()
{
}

CStone::~CStone()
{
	SAFE_DELETE(m_pExcelData);
}
//�����X�V.
//���g�k���]�A���W�Ȃǂ̒l�������ް��ɔ��f.
void CStone::Update()
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
void CStone::MeshCreate()
{
	AttachModel(m_pResource->GetStaticMesh(enStaticMesh::Stone));
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 3.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fScale = 1.0f;

	m_pExcelData = new CExcelData();
	m_pExcelData->MeshObjectData("Data\\ExcelData\\Stone.csv", m_StonePos);
}
