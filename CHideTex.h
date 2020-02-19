#ifndef CHIDETEX_H
#define CHIDETEX_H

#include "CSprite.h"
#include "MyMacro.h"



class CHideTex
{
public:

	CHideTex();
	~CHideTex();

	//読み込み処理関数.
	void Load(D3DXMATRIX mView, D3DXMATRIX mProj, D3DXVECTOR3 vCamera);	

	//描画処理関数.
	void Render();		

	//隠すテクスチャ処理関数関数.
	void HideTex_Disp(D3DXVECTOR3 Pos);		

	//アニメーションカウント関数.
	void Move();											
	
	//解放処理関数.
	void Release();													

	//獲得関数.
	bool GetHideTexFlag() { return m_HideTexFlag; }
	D3DXVECTOR2 GetUVPos() { return m_UV; }

	//置換関数
	void Attach(CSprite* Sprite) { m_pHideTex = Sprite; }

	void Init();

private:
	D3DXMATRIX	m_mView;
	D3DXMATRIX	m_mProj;
	D3DXVECTOR3 m_vCamera;
	CSprite*	m_pHideTex;		//隠すテクスチャ.
	D3DXVECTOR3 m_InitPos;				//位置.
	D3DXVECTOR3 m_Distance;
	D3DXVECTOR3	m_Pos;
	bool		m_HideTexFlag;	//隠すフラグ.
	int						m_AnimCount;			//カウンタ.
	D3DXVECTOR2				m_UV;					//テクスチャUV座標.




};



#endif	//CHIDETEX_H
