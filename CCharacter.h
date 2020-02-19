#ifndef CCHARACTER_SKIN_H
#define CCHARACTER_SKIN_H

#include "CGameObject.h"
#include "CDX9SkinMesh.h"
#include "CSkinResource.h"	//リリースクラス.

/********************************************************
*	キャラクタクラス(スキンメッシュ用).吉田変更.
**/
class CCharacter
	:public	CGameObject
{
public:

	CCharacter();
	virtual ~CCharacter();

	//レンダリング.
	void Render(D3DXMATRIX& mView, D3DXMATRIX& mProj,
		D3DXVECTOR3& vLight, D3DXVECTOR3& vCamPos);


	//アニメーション速度設定関数.
	void SetAnimSpeed(double Speed) {
		m_AnimSpeed = Speed;
	}
	double GetAnimSpeed() { return m_AnimSpeed; }

	//アニメーション最大数を取得する.
	int GetAnimSetMax() {
		if (m_pAnimeCtrl != nullptr) {
			return static_cast<int>(
				m_pAnimeCtrl->GetMaxNumAnimationSets());
		}
		return -1;
	}

	//アニメーション切り替え.
	void ChangeAnimSet(int index, double StartPos = 0.0) {
		//していアニメーションが有効範囲かチェックする.
		if (0 <= index && index < GetAnimSetMax()) {
			m_pSkinMesh->ChangeAnimSet_StartPos(
				index, StartPos, m_pAnimeCtrl);
		}
	}
	// アニメーション停止時間を取得.
	double GetAnimPeriod(int index);

	//複数テクスチャ切り替えするのに一つ一つにセットする必要あり.
	void SetTexChangeSystem(int SkinNo) { m_pSkinMesh->SetEnabelChangeTexttureSystem(SkinNo); }


	//ｽﾌｨｱ構造体.
	struct SPHERE
	{
		D3DXVECTOR3	vCenter;	//中心.
		float		fRadius;	//半径. 
	};

	SPHERE m_Sphere;


	//ここでは、外部でﾛｰﾄﾞしたﾓﾃﾞﾙを受け取るためのﾎﾟｲﾝﾀ.
	CDX9SkinMesh*	m_pStaticSkinMesh;



protected:

	//モデルデータ関連付け関数.
	void AttachModel(CDX9SkinMesh* pModel);

	//モデルデータ関連付け解除関数.
	void DetachModel() {
		if (m_pSkinMesh != nullptr) {
			m_pSkinMesh = nullptr;
		}
		if (m_pAnimeCtrl != nullptr) {
			m_pAnimeCtrl->Release();
			m_pAnimeCtrl = nullptr;
		}
	}


	//情報更新.
	//※拡縮や回転、座標などの値をモデルデータに変換.
	//virtual void Update() = 0;	//今後これにする.
	//void Update();
	//情報を更新.
//※拡縮や回転、座標などの値をﾓﾃﾞﾙﾃﾞｰﾀに反映.
	virtual void Update() = 0;

	//モデルデータの情報を管理.
	virtual void MeshCreate() = 0;



	//ここでは、外部でロードしたメモリを受け散るためのポインタ.
	CDX9SkinMesh*			m_pSkinMesh;	//スキンメッシュモデルデータ.
	LPD3DXANIMATIONCONTROLLER	m_pAnimeCtrl;	//アニメーションコントローラ.
	
	CSkinResource*			m_pSkinResource;

private:
	bool	m_ShotFlag;		//ｼｮｯﾄﾌﾗｸﾞ.
	bool	m_DeadFlag;		//死亡ﾌﾗｸﾞ.
	bool	m_DispFlag;		//表示ﾌﾗｸﾞ.
	int		m_DispCount;	//表示カウント.
	float	m_MoveSpeed;	//速度.
	double						m_AnimSpeed;	//アニメーション速度.

};

#endif // #ifndef CCHARACTER_SKIN_H
