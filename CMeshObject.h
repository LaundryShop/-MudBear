#ifndef CCHARACTER_H
#define CCHARACTER_H

#include "CGameObject.h" //�Q�[���I�u�W�F�N�g�N���X.
#include "CDX9Mesh.h"
#include "CResource.h"	//�����[�X�N���X.

/************************************************************
*	��׸���׽.
**/
class CMeshObject
	:public CGameObject
{
public:
	CMeshObject();
	virtual ~CMeshObject();

	//�����ݸ�.
	void Render(D3DXMATRIX& mView, D3DXMATRIX& mProj,
		D3DXVECTOR3& vLight, D3DXVECTOR3& vCamPos);

	//ү���̓ǂݍ��݊֐�.
	HRESULT LoadMesh(HWND m_hWnd, ID3D11Device* m_pDevice11, ID3D11DeviceContext*	m_pContext11);

protected:
	//�����ް��֘A�t���֐�.
	void AttachModel(CDX9Mesh* pModel) {
		if (pModel != nullptr) {
			m_pStaticMesh = pModel;
		}
	}
	//�����ް��֘A�t�������֐�.
	void DetachModel() {
		if (m_pStaticMesh != nullptr) {
			m_pStaticMesh = nullptr;
		}
	}

	//�����X�V.
	//���g�k���]�A���W�Ȃǂ̒l�������ް��ɔ��f.
	virtual void Update() = 0;

	//���f���f�[�^�̏����Ǘ�.
	virtual void MeshCreate() = 0;

	//�����ł́A�O����۰�ނ������ق��󂯎�邽�߂��߲��.
	CDX9Mesh*	m_pStaticMesh;
	CResource*	m_pResource;
private:

	bool	m_DispFlag;		//�\���׸�.
	int		m_DispCount;	//�\������.

};

#endif//#ifndef CCHARACTER_H