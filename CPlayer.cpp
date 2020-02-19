#include "CPlayer.h"

CPlayer::CPlayer()
  : m_DamageCnt(0)
{
}

CPlayer::~CPlayer()
{
}

//初期化
void CPlayer::Init()
{
	m_Pos = D3DXVECTOR3(0.0f, 0.9f, 4.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	DamageInit();
}

void CPlayer::DamageInit()
{
	m_DamageCnt = 0;//ダメージを初期化.
}

//プレイヤーの動き.
void CPlayer::Move(bool m_flag)
{
	if (m_flag == true) {
		m_Pos.z += 0.1f;
	}
}
