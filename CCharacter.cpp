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
		//���f�����Z�b�g.
		m_pSkinMesh = pModel;
		//			//�A�j���[�V�������x.
		m_AnimSpeed = pModel->GetAnimSpeed();
		//�A�j���[�V�����R���g���[���̃N���[���쐬.
		LPD3DXANIMATIONCONTROLLER pAC = m_pSkinMesh->GetAnimationController();

		if (FAILED(
			pAC->CloneAnimationController(
				pAC->GetMaxNumAnimationOutputs(),
				pAC->GetMaxNumAnimationSets(),
				pAC->GetMaxNumTracks(),
				pAC->GetMaxNumEvents(),
				&m_pAnimeCtrl)))	//(out)�A�j���[�V�����R���g���[��.
		{
			_ASSERT_EXPR(false, "�A�j���[�V�����R���g���[���̃N���[���쐬���s");
		}
	}
}

void CCharacter::Render(D3DXMATRIX & mView, D3DXMATRIX & mProj, D3DXVECTOR3 & vLight, D3DXVECTOR3 & vCamPos)
{
	if (m_pSkinMesh == nullptr)
	{
		return;
	}

	//���X�V.
	Update();


	//���b�V���̃����_�����O�֐����Ă�.
	m_pSkinMesh->Render(mView, mProj, vLight, vCamPos, m_pAnimeCtrl);
}
//���X�V.
//���g�k���]�A���W�Ȃǂ̒l�����f���f�[�^�ɕϊ�.
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

// �A�j���[�V������~���Ԃ��擾.
double CCharacter::GetAnimPeriod(int index)
{
	if (m_pSkinMesh == NULL) {
		return 0.0f;
	}
	return m_pSkinMesh->GetAnimPeriod(index);
}
