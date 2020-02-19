#include "CKuma.h"

CKuma::CKuma()
{

}

CKuma::~CKuma()
{

}

void CKuma::Update()
{
	m_pStaticSkinMesh->SetPosition(m_Pos);
	m_pStaticSkinMesh->SetRotation(m_Rot);
	m_pStaticSkinMesh->SetScale(m_fScale);
}

void CKuma::MeshCreate()
{
	AttachModel(m_pSkinResource->GetStaticSkinMeesh(enStaticSkinMesh::Kuma));
	
	ChangeAnimSet(1);
	SetAnimSpeed(1.0);
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fScale = 0.02f;
}

void CKuma::Init()
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fScale = 0.02f;

}