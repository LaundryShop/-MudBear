#pragma once

//警告についてのｺｰﾄﾞ分析を無効にする.4005:再定義.
#pragma warning(disable:4005)
#include <Windows.h>
#include <crtdbg.h>//_ASSERT_EXPR()で必要.

#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>	//「D3DX～」の定義使用時に必要.
#include <D3D10.h>

#include "MyMacro.h"

//ﾗｲﾌﾞﾗﾘ読み込み.
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx10.lib")	//「D3DX～」の定義使用時に必要.

//=================================================
//	定数.
//=================================================
//#define WND_W 1280	//←定数宣言で#defineは使わない.
const int WND_W = 1280;	//ｳｨﾝﾄﾞｳの幅.1280デフォルト.フルスクリーン1600
const int WND_H = 720;	//ｳｨﾝﾄﾞｳの高さ.720デフォルト.フルスクリーン900

const int	WND_W_HALF = WND_W / 2;	//ウィンドウの幅の半分.
const int	WND_H_HALF = WND_H / 2; //ウィンドウの高さの半分.
