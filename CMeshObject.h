#ifndef CCHARACTER_H
#define CCHARACTER_H

#include "CGameObject.h" //ゲームオブジェクトクラス.
#include "CDX9Mesh.h"
#include "CResource.h"	//リリースクラス.

/************************************************************
*	ｷｬﾗｸﾀｰｸﾗｽ.
**/
class CMeshObject
	:public CGameObject
{
public:
	CMeshObject();
	virtual ~CMeshObject();

	//ﾚﾝﾀﾞﾘﾝｸﾞ.
	void Render(D3DXMATRIX& mView, D3DXMATRIX& mProj,
		D3DXVECTOR3& vLight, D3DXVECTOR3& vCamPos);

	//ﾒｯｼｭの読み込み関数.
	HRESULT LoadMesh(HWND m_hWnd, ID3D11Device* m_pDevice11, ID3D11DeviceContext*	m_pContext11);

protected:
	//ﾓﾃﾞﾙﾃﾞｰﾀ関連付け関数.
	void AttachModel(CDX9Mesh* pModel) {
		if (pModel != nullptr) {
			m_pStaticMesh = pModel;
		}
	}
	//ﾓﾃﾞﾙﾃﾞｰﾀ関連付け解除関数.
	void DetachModel() {
		if (m_pStaticMesh != nullptr) {
			m_pStaticMesh = nullptr;
		}
	}

	//情報を更新.
	//※拡縮や回転、座標などの値をﾓﾃﾞﾙﾃﾞｰﾀに反映.
	virtual void Update() = 0;

	//モデルデータの情報を管理.
	virtual void MeshCreate() = 0;

	//ここでは、外部でﾛｰﾄﾞしたﾓﾃﾞﾙを受け取るためのﾎﾟｲﾝﾀ.
	CDX9Mesh*	m_pStaticMesh;
	CResource*	m_pResource;
private:

	bool	m_DispFlag;		//表示ﾌﾗｸﾞ.
	int		m_DispCount;	//表示ｶｳﾝﾄ.

};

#endif//#ifndef CCHARACTER_H