#include "CMeshObject.h"

CMeshObject::CMeshObject()
	: m_pStaticMesh(nullptr)
	, m_pResource(nullptr)

	, m_DispFlag(false)
	, m_DispCount(0)
{
}

CMeshObject::~CMeshObject()
{
	SAFE_DELETE(m_pResource);
	m_pStaticMesh = nullptr;
}

//�����ݸ�.
void CMeshObject::Render(
	D3DXMATRIX& mView, D3DXMATRIX& mProj,
	D3DXVECTOR3& vLight, D3DXVECTOR3& vCamPos)
{
	//ү������Ă���ĂȂ��ꍇ�ͽٰ����.
	if (m_pStaticMesh == nullptr) {
		return;
	}

	//���X�V.
	Update();

	//ү���������ݸފ֐����Ă�.
	m_pStaticMesh->Render(mView, mProj, vLight, vCamPos);
}

//ү���̓ǂݍ��݊֐�.
HRESULT CMeshObject::LoadMesh(
	HWND m_hWnd, ID3D11Device * m_pDevice11,
	ID3D11DeviceContext * m_pContext11)
{
	//ؿ���׽�̍쐬.
	m_pResource = new CResource();
	m_pResource->Init(m_hWnd, m_pDevice11, m_pContext11);
	m_pResource->LoadStaticMesh();

	//���f���f�[�^�̏����Ǘ�.
	MeshCreate();

	return S_OK;
}