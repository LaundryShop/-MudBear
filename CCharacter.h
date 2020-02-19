#ifndef CCHARACTER_SKIN_H
#define CCHARACTER_SKIN_H

#include "CGameObject.h"
#include "CDX9SkinMesh.h"
#include "CSkinResource.h"	//�����[�X�N���X.

/********************************************************
*	�L�����N�^�N���X(�X�L�����b�V���p).�g�c�ύX.
**/
class CCharacter
	:public	CGameObject
{
public:

	CCharacter();
	virtual ~CCharacter();

	//�����_�����O.
	void Render(D3DXMATRIX& mView, D3DXMATRIX& mProj,
		D3DXVECTOR3& vLight, D3DXVECTOR3& vCamPos);


	//�A�j���[�V�������x�ݒ�֐�.
	void SetAnimSpeed(double Speed) {
		m_AnimSpeed = Speed;
	}
	double GetAnimSpeed() { return m_AnimSpeed; }

	//�A�j���[�V�����ő吔���擾����.
	int GetAnimSetMax() {
		if (m_pAnimeCtrl != nullptr) {
			return static_cast<int>(
				m_pAnimeCtrl->GetMaxNumAnimationSets());
		}
		return -1;
	}

	//�A�j���[�V�����؂�ւ�.
	void ChangeAnimSet(int index, double StartPos = 0.0) {
		//���Ă��A�j���[�V�������L���͈͂��`�F�b�N����.
		if (0 <= index && index < GetAnimSetMax()) {
			m_pSkinMesh->ChangeAnimSet_StartPos(
				index, StartPos, m_pAnimeCtrl);
		}
	}
	// �A�j���[�V������~���Ԃ��擾.
	double GetAnimPeriod(int index);

	//�����e�N�X�`���؂�ւ�����̂Ɉ��ɃZ�b�g����K�v����.
	void SetTexChangeSystem(int SkinNo) { m_pSkinMesh->SetEnabelChangeTexttureSystem(SkinNo); }


	//�̨��\����.
	struct SPHERE
	{
		D3DXVECTOR3	vCenter;	//���S.
		float		fRadius;	//���a. 
	};

	SPHERE m_Sphere;


	//�����ł́A�O����۰�ނ������ق��󂯎�邽�߂��߲��.
	CDX9SkinMesh*	m_pStaticSkinMesh;



protected:

	//���f���f�[�^�֘A�t���֐�.
	void AttachModel(CDX9SkinMesh* pModel);

	//���f���f�[�^�֘A�t�������֐�.
	void DetachModel() {
		if (m_pSkinMesh != nullptr) {
			m_pSkinMesh = nullptr;
		}
		if (m_pAnimeCtrl != nullptr) {
			m_pAnimeCtrl->Release();
			m_pAnimeCtrl = nullptr;
		}
	}


	//���X�V.
	//���g�k���]�A���W�Ȃǂ̒l�����f���f�[�^�ɕϊ�.
	//virtual void Update() = 0;	//���ケ��ɂ���.
	//void Update();
	//�����X�V.
//���g�k���]�A���W�Ȃǂ̒l�������ް��ɔ��f.
	virtual void Update() = 0;

	//���f���f�[�^�̏����Ǘ�.
	virtual void MeshCreate() = 0;



	//�����ł́A�O���Ń��[�h�������������󂯎U�邽�߂̃|�C���^.
	CDX9SkinMesh*			m_pSkinMesh;	//�X�L�����b�V�����f���f�[�^.
	LPD3DXANIMATIONCONTROLLER	m_pAnimeCtrl;	//�A�j���[�V�����R���g���[��.
	
	CSkinResource*			m_pSkinResource;

private:
	bool	m_ShotFlag;		//�����׸�.
	bool	m_DeadFlag;		//���S�׸�.
	bool	m_DispFlag;		//�\���׸�.
	int		m_DispCount;	//�\���J�E���g.
	float	m_MoveSpeed;	//���x.
	double						m_AnimSpeed;	//�A�j���[�V�������x.

};

#endif // #ifndef CCHARACTER_SKIN_H
