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

//ﾚﾝﾀﾞﾘﾝｸﾞ.
void CMeshObject::Render(
	D3DXMATRIX& mView, D3DXMATRIX& mProj,
	D3DXVECTOR3& vLight, D3DXVECTOR3& vCamPos)
{
	//ﾒｯｼｭがｾｯﾄされてない場合はｽﾙｰする.
	if (m_pStaticMesh == nullptr) {
		return;
	}

	//情報更新.
	Update();

	//ﾒｯｼｭのﾚﾝﾀﾞﾘﾝｸﾞ関数を呼ぶ.
	m_pStaticMesh->Render(mView, mProj, vLight, vCamPos);
}

//ﾒｯｼｭの読み込み関数.
HRESULT CMeshObject::LoadMesh(
	HWND m_hWnd, ID3D11Device * m_pDevice11,
	ID3D11DeviceContext * m_pContext11)
{
	//ﾘｿｰｽｸﾗｽの作成.
	m_pResource = new CResource();
	m_pResource->Init(m_hWnd, m_pDevice11, m_pContext11);
	m_pResource->LoadStaticMesh();

	//モデルデータの情報を管理.
	MeshCreate();

	return S_OK;
}