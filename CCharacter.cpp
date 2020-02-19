#include "CCharacter.h"

CCharacter::CCharacter()
	: m_pSkinMesh(nullptr)
	, m_pAnimeCtrl(nullptr)
	, m_AnimSpeed(0.06f)
	, m_ShotFlag(false)
	, m_DeadFlag(false)
	, m_DispCount(0)
	, m_MoveSpeed(0.0f)
{

}


CCharacter::~CCharacter()
{
	m_pAnimeCtrl = nullptr;

	m_pSkinMesh = nullptr;
}
void CCharacter::AttachModel(CDX9SkinMesh* pModel) {
	if (pModel != nullptr) {
		//モデルをセット.
		m_pSkinMesh = pModel;
		//			//アニメーション速度.
		m_AnimSpeed = pModel->GetAnimSpeed();
		//アニメーションコントローラのクローン作成.
		LPD3DXANIMATIONCONTROLLER pAC = m_pSkinMesh->GetAnimationController();

		if (FAILED(
			pAC->CloneAnimationController(
				pAC->GetMaxNumAnimationOutputs(),
				pAC->GetMaxNumAnimationSets(),
				pAC->GetMaxNumTracks(),
				pAC->GetMaxNumEvents(),
				&m_pAnimeCtrl)))	//(out)アニメーションコントローラ.
		{
			_ASSERT_EXPR(false, "アニメーションコントローラのクローン作成失敗");
		}
	}
}

void CCharacter::Render(D3DXMATRIX & mView, D3DXMATRIX & mProj, D3DXVECTOR3 & vLight, D3DXVECTOR3 & vCamPos)
{
	if (m_pSkinMesh == nullptr)
	{
		return;
	}

	//情報更新.
	Update();


	//メッシュのレンダリング関数を呼ぶ.
	m_pSkinMesh->Render(mView, mProj, vLight, vCamPos, m_pAnimeCtrl);
}
//情報更新.
//※拡縮や回転、座標などの値をモデルデータに変換.
void CCharacter::Update()
{
	if (m_pSkinMesh == nullptr) {
		return;
	}
	m_pSkinMesh->SetPosition(m_Pos);
	m_pSkinMesh->SetRotation(m_Rot);
	m_pSkinMesh->SetScale(m_fScale);
	m_pSkinMesh->SetAnimSpeed(m_AnimSpeed);

}

// アニメーション停止時間を取得.
double CCharacter::GetAnimPeriod(int index)
{
	if (m_pSkinMesh == NULL) {
		return 0.0f;
	}
	return m_pSkinMesh->GetAnimPeriod(index);
}
