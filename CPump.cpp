#include "CPump.h"

CPump::CPump()
	: m_ReloadFlag(false)
	, m_ReturnFlag(false)
	, m_GaugeBarCnt(MAXGAUGE)
{
}

CPump::~CPump()
{
}

//初期化.
void CPump::Init()
{
	m_Pos = D3DXVECTOR3(0.5f, 0.6f, 0.5f);
	m_Rot = D3DXVECTOR3(0.0f, 1.5f, 0.0f);
	m_fScale = 0.4f;

	m_GaugeBarCnt = 1.0f;
}

//移動.
void CPump::Move(bool m_Moveflag)
{
	if (m_Moveflag == true) {
		m_Pos.z += 0.1f;
	}
}

//チャージ(ゲージを回復する).
void CPump::Charge(D3DXVECTOR3 m_GunPos)
{
	if (m_ReloadFlag == true) {
		if (m_ReturnFlag == false) {
			m_Pos.z -= PUMP_SPD;
			if (m_Pos.z <= m_GunPos.z - 0.3f) {
				m_ReturnFlag = true;
			}
		}
		else if (m_ReturnFlag == true) {
			m_Pos.z += PUMP_SPD;
			m_GaugeBarCnt = m_GaugeBarCnt + 0.01f;
			if (m_Pos.z >= m_GunPos.z) {
				m_ReturnFlag = false;
				m_ReloadFlag = false;
			}
		}
	}
}

//情報を更新.
void CPump::Update()
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
void CPump::MeshCreate()
{
	AttachModel(m_pResource->GetStaticMesh(enStaticMesh::Pump));
	m_Pos = D3DXVECTOR3(0.5f, 0.6f, 0.5f);
	m_Rot = D3DXVECTOR3(0.0f, 1.5f, 0.0f);
	m_fScale = 0.4f;
}
