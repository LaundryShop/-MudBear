#include "CResource.h"
#include "CDX9Mesh.h"

CResource::CResource()
	: m_hWnd(nullptr)
	, m_pDevice11(nullptr)
	, m_pContext11(nullptr)
	, m_ppStaticMesh(nullptr)
{
}

CResource::~CResource()
{
	if (m_ppStaticMesh != nullptr)
	{
		for (int i = static_cast<int>(enStaticMesh::Max) - 1; i >= 0; i--)
		{
			if (m_ppStaticMesh[i] != nullptr) {
				ReleaseStaticMesh(static_cast<enStaticMesh>(i));
			}
		}

		delete[] m_ppStaticMesh;
		m_ppStaticMesh = nullptr;
	}
	m_pContext11 = nullptr;
	m_pDevice11 = nullptr;
	m_hWnd = nullptr;

}


HRESULT CResource::Init(
	HWND hWnd,
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11)
{
	m_hWnd = hWnd;
	m_pDevice11 = pDevice11;
	m_pContext11 = pContext11;

	//��è��ү�����߲�����߲���̗̈�m��.
	//���߲���^�̔z��̈�̊m��.
	m_ppStaticMesh = new CDX9Mesh*[static_cast<int>(enStaticMesh::Max)];
	for (int i = 0; i < static_cast<int>(enStaticMesh::Max); i++)
	{
		//������:nullptr�Ŗ��߂�.
		m_ppStaticMesh[i] = nullptr;
	}
	return S_OK;
}

HRESULT CResource::CreateStaticMesh(
	enStaticMesh enMesh,
	const char* fileName, bool Flag)
{
	//�͈͊O�Ȃ�I��.
	if (enMesh < enStaticMesh::Start || enStaticMesh::Max <= enMesh) {
		return E_FAIL;
	}

	//���ق����ɑ��݂��Ă���I��.
	if (m_ppStaticMesh[static_cast<int>(enMesh)] != nullptr) {
		return E_FAIL;
	}

	//���ٓǂݍ���.
	m_ppStaticMesh[static_cast<int>(enMesh)] = new CDX9Mesh();
	if (FAILED(
		m_ppStaticMesh[static_cast<int>(enMesh)]->Init(
			m_hWnd, m_pDevice11, m_pContext11,
			fileName, Flag)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CResource::ReleaseStaticMesh(enStaticMesh enMesh)
{
	//�͈͊O�Ȃ�I��.
	if (enMesh < enStaticMesh::Start || enStaticMesh::Max <= enMesh) {
		return E_FAIL;
	}
	const int MeshNo = static_cast<int>(enMesh);

	//���ق����݂��ĂȂ���ΏI��.
	if (m_ppStaticMesh[MeshNo] == nullptr) {
		return S_OK;
	}

	//�������.
	m_ppStaticMesh[MeshNo]->Release();
	delete m_ppStaticMesh[MeshNo];//�폜.
	m_ppStaticMesh[MeshNo] = nullptr;

	return S_OK;
}

CDX9Mesh* CResource::GetStaticMesh(enStaticMesh enMesh)
{
	//�͈͊O�Ȃ�I��.
	if (enMesh < enStaticMesh::Start || enStaticMesh::Max <= enMesh) {
		return nullptr;
	}
	const int MeshNo = static_cast<int>(enMesh);
	//���ق����݂��Ȃ��̂ŏI��.
	if (m_ppStaticMesh[MeshNo] == nullptr) {
		return nullptr;
	}

	return m_ppStaticMesh[MeshNo];
}

HRESULT CResource::LoadStaticMesh()
{
	const char fileName[][256] =
	{
		"Data\\Model\\Way\\miti.x",
		"Data\\Model\\Way\\ground_2.x",
		"Data\\Model\\Background\\yama.x",
		"Data\\Model\\WaterGun\\mizudeppou_1.x",
		"Data\\Model\\WaterGun\\mizudeppou_2.x",
		"Data\\Model\\Stone\\isi.x",
		"Data\\Model\\Grass\\kusa.x",
	};

	for (int i = 0; i < static_cast<int>(enStaticMesh::Max); i++)
	{
		CreateStaticMesh(static_cast<enStaticMesh>(i), fileName[i], true);
	}

	return S_OK;
}

HRESULT CResource::ReleaseStaticMesh()
{
	for (int i = static_cast<int>(enStaticMesh::Max) - 1; i >= 0; i--)
	{
		ReleaseStaticMesh(static_cast<enStaticMesh>(i));
	}

	return S_OK;
}
