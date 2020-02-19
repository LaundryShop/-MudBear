#ifndef CMOUSE_H
#define CMOUSE_H

#include "Global.h"
const float CURSOR_SIZE = 128.0f;	//カーソルの画像サイズ.

class CMouse
{
public:
	CMouse();
	~CMouse();
	//マウスをセット.
	void SetMouse(HWND	m_hWnd);
	
	//矩形と点(マウスカーソル)の判定をとる.
	bool RecAndPos(
		float iDx, float iDy,
		float iDw, float iDh);

	//点(マウスカーソル)と円の判定をとる.
	bool PointAndCircle(float iDx, float iDy, float m_Radius);

	// 吉田変更
	// スクリーン座標をワールド座標に変換.
	D3DXVECTOR3* CalcScreenToWorld(D3DXVECTOR3* Pos, float Sx,  // スクリーンX座標.
		float Sy,  // スクリーンY座標.
		float fZ,// 射影空間でのZ値（0～1）.
		D3DXMATRIX* View, D3DXMATRIX* Prj);

	//カーソルの位置情報Xを取得.
	float	GetCursorPosX() {
		return	m_CursorPos.x;
	}
	//カーソルの位置情報Yを取得.
	float	GetCursorPosY() {
		return	m_CursorPos.y;
	}

	//マウスの位置情報Xを取得.
	long	GetMousePosX() {
		return	m_MousePos.x;
	}
	//マウスの位置情報Yを取得.
	long	GetMousePosY() {
		return	m_MousePos.y;
	}
private:
	D3DXVECTOR3	m_CursorPos;	//カーソル位置.
	POINT		m_MousePos;		//マウスの位置情報.
};
#endif //#ifndef CMOUSE_H

