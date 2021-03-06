#ifndef CGAME_OBJECT_H
#define CGAME_OBJECT_H

#include "Global.h"

/**************************************************
*	ｹﾞｰﾑｵﾌﾞｼﾞｪｸﾄｸﾗｽ.
**/
class CGameObject
{
public:
	CGameObject();
	//virtual 仮想関数.
	virtual ~CGameObject();

	void SetPosition(const D3DXVECTOR3& Pos) {	m_Pos = Pos;	}	//位置情報を設定.
	void SetRotation(const D3DXVECTOR3& Rot) {	m_Rot = Rot; 	}	//回転情報を設定.
	void SetScale(const float Scale){	m_fScale = Scale;		}	//拡縮情報を設定.
	D3DXVECTOR3 GetPosition() { return m_Pos; }		//位置情報を取得.
	D3DXVECTOR3 GetRotation() { return m_Rot; }		//回転情報を取得.
	float	GetScale() {	return m_fScale;	  }		//拡縮情報を取得.

protected:
	D3DXVECTOR3	m_Pos;		//位置情報.
	D3DXVECTOR3	m_Rot;		//回転情報.
	float	m_fScale;	//拡縮情報.
};

#endif//#ifndef CGAME_OBJECT_H