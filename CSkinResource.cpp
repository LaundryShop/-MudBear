#include "CSkinResource.h"

CSkinResource::CSkinResource()
{

}

CSkinResource::~CSkinResource()
{

}

HRESULT CSkinResource::Init(
	HWND hWnd,
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11)
{
	m_hWnd = hWnd;
	m_pDevice11 = pDevice11;
	m_pContext11 = pContext11;

	//スタティックスキンメッシュのポインタのポインタの領域確保.
	//※ポインタ型の配列領域の確保.
	m_ppStaticSkinMesh = new CDX9SkinMesh*[static_cast<int>(enStaticSkinMesh::Max)];
	for (int i = 0; i < static_cast<int>(enStaticSkinMesh::Max); i++) 
	{
		//初期化:nullptrで埋める.
		m_ppStaticSkinMesh[i] = nullptr;
	}
	return S_OK;
}

HRESULT CSkinResource::CreateStaticSkinMesh(enStaticSkinMesh enSkinMesh, const char * fileName, bool Flag)
{
	//範囲外なら終了.
	if (enSkinMesh < enStaticSkinMesh::Start || enStaticSkinMesh::Max <= enSkinMesh) {
		return E_FAIL;
	}

	//モデルがすでに存在していたら終了.
	if (m_ppStaticSkinMesh[static_cast<int>(enSkinMesh)] != nullptr) {
		return E_FAIL;
	}

	//モデル読み込み.
	m_ppStaticSkinMesh[static_cast<int>(enSkinMesh)] = new CDX9SkinMesh();
	if (FAILED(
		m_ppStaticSkinMesh[static_cast<int>(enSkinMesh)]->Init(
			&m_SkinMesh_Init,fileName)))
	{
		return E_FAIL;
	}


	return S_OK;
}

HRESULT CSkinResource::ReleaseStaticSkinMesh(enStaticSkinMesh enSkinMesh)
{
	//範囲外なら終了.
	if (enSkinMesh < enStaticSkinMesh::Start || enStaticSkinMesh::Max <= enSkinMesh) {
		return E_FAIL;
	}
	const int MeshNo = static_cast<int>(enSkinMesh);

	//モデルが存在していなければ終了.
	if (m_ppStaticSkinMesh[MeshNo] == nullptr) {
		return S_OK;
	}

	//解放処理.
	m_ppStaticSkinMesh[MeshNo]->Release();
	delete m_ppStaticSkinMesh[MeshNo];//削除.
	m_ppStaticSkinMesh[MeshNo] = nullptr;

	return S_OK;
}

CDX9SkinMesh* CSkinResource::GetStaticSkinMeesh(enStaticSkinMesh enSkinMesh)
{
	//範囲外なら終了.
	if (enSkinMesh < enStaticSkinMesh::Start || enStaticSkinMesh::Max <= enSkinMesh) {
		return nullptr;
	}
	const int MeshNo = static_cast<int>(enSkinMesh);
	//モデルが存在しないので終了.
	if (m_ppStaticSkinMesh[MeshNo] == nullptr) {
		return nullptr;
	}

	return m_ppStaticSkinMesh[MeshNo];

}

HRESULT CSkinResource::LoadStaticSkinMesh()
{
	const char fileName[][256] =
	{
		"Data\\ModelSkin\\Kuma\\Kuma.X",
		"Data\\ModelSkin\\BossKuma\\BossKuma.X"
	};

	for (int i = 0; i < static_cast<int>(enStaticSkinMesh::Max); i++)
	{
		CreateStaticSkinMesh(static_cast<enStaticSkinMesh>(i), fileName[i], true);
	}

	return S_OK;
}

HRESULT CSkinResource::ReleaseStaticSkinMesh()
{
	for (int i = static_cast<int>(enStaticSkinMesh::Max) - 1; i >= 0; i--)
	{
		ReleaseStaticSkinMesh(static_cast<enStaticSkinMesh>(i));
	}

	return S_OK;
}

