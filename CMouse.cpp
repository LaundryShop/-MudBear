#include "CMouse.h"

CMouse::CMouse()
{
}

CMouse::~CMouse()
{
}

void CMouse::SetMouse(HWND	m_hWnd)
{
	//マウスカーソル位置取得.
	GetCursorPos(&m_MousePos);
	//スクリーン座標→クライアント座標に変換.
	ScreenToClient(m_hWnd, //ウィンドウハンドル.
		&m_MousePos);//(out)POINT構造体.
	//マウス座標→マップ位置.
	//マウス座標がクライアント領域に入っている.
	(0 <= m_MousePos.x) && (m_MousePos.x <= WND_W);
	(0 <= m_MousePos.y) && (m_MousePos.y <= WND_H);
	//---------------------------------------------------

	//---------------------------------------------------
	//	2019_7_19更新(吉田).
	//カーソルの調整(真ん中にする).
	m_CursorPos.x = m_MousePos.x - CURSOR_SIZE / 2 + 8;
	m_CursorPos.y = m_MousePos.y - CURSOR_SIZE / 2 + 22;
}

//矩形と点(マウスカーソル)の判定をとる
bool CMouse::RecAndPos(
	float iDx, float iDy,	//矩形配置してる左端X | 矩形配置してるY.
	float iDw, float iDh)	//矩形の幅,高さ.
{
	if ((m_MousePos.x >= iDx && m_MousePos.x <= iDx + iDw) &&
		(m_MousePos.y >= iDy && m_MousePos.y <= iDy + iDh))
	{
		return true;
	}
	return false;
}

//点(マウスカーソル)と円の判定をとる.
bool CMouse::PointAndCircle(float iDx, float iDy, float m_Radius)
{
	float A = m_MousePos.x - (iDx - 10.0f);		//-10.0fはマウスの位置がずれているために調節している.
	float B = m_MousePos.y - (iDy - 40.0f);		//-40.0fはマウスの位置がずれているために調節している.
	float C = sqrtf(A * A + B * B);

	if (C <= m_Radius) {
		return true;
	}
	return false;
}

// スクリーン座標をワールド座標に変換.
D3DXVECTOR3 * CMouse::CalcScreenToWorld(
	D3DXVECTOR3 * Pos,
	float Sx, float Sy, float fZ,
	D3DXMATRIX * View, D3DXMATRIX * Prj)
{
	// 各行列の逆行列を算出.
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, View);
	D3DXMatrixInverse(&InvPrj, NULL, Prj);
	D3DXMatrixIdentity(&VP);
	VP._11 = WND_W / 2.0f; VP._22 = -WND_H / 2.0f;
	VP._41 = WND_W / 2.0f; VP._42 = WND_H / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// 逆変換.
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVec3TransformCoord(Pos, &D3DXVECTOR3(Sx, Sy, fZ), &tmp);

	return Pos;
}
