#ifndef CSPRITE_UI_H
#define CSPRITE_UI_H
//警告についてのｺｰﾄﾞ分析を無効にする.4005:再定義.
#pragma warning(disable:4005)
#include <Windows.h>

#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>	//「D3DX～」の定義使用時に必要.
#include <D3D10.h>

#include "MyMacro.h"
#include "Global.h"

//ﾗｲﾌﾞﾗﾘ読み込み.
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx10.lib")	//「D3DX～」の定義使用時に必要.

#define ALIGN16	_declspec(align(16))


/**************************************************
*	ｽﾌﾟﾗｲﾄUIｸﾗｽ.
**/
class CSpriteUI
{
public:
	//幅高さ構造体(float型)
	struct WHSIZE_FLOAT
	{
		float w;
		float h;
	};
	//ｽﾌﾟﾗｲﾄ構造体.
	struct SPRITE_STATE
	{
		WHSIZE_FLOAT	Disp;	//表示幅高さ.
		WHSIZE_FLOAT    Base;	//元画像幅高さ.
		WHSIZE_FLOAT	Stride;	//1コマ当たりの幅高さ.
	};

	CSpriteUI();	//ｺﾝｽﾄﾗｸﾀ.
	~CSpriteUI();	//ﾃﾞｽﾄﾗｸﾀ.

	//初期化.
	HRESULT Init(ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		const char* fileName, SPRITE_STATE* pSs);

	//解放.
	void Release();

	//ｼｪｰﾀﾞ作成.
	HRESULT InitShader();
	//ﾓﾃﾞﾙ作成.
	HRESULT InitModel();
	//ﾃｸｽﾁｬ作成.
	HRESULT CreateTexture(const char* fileName,
		ID3D11ShaderResourceView** pTexture);

	//ﾚﾝﾀﾞﾘﾝｸﾞ用.
	void Render();

	//ゲージのﾚﾝﾀﾞﾘﾝｸﾞ用.
	void GaugeRender(float fScale_x);

	HRESULT InitBlend();
	void SetBlend(bool EnableAlpha);

	//位置情報を設定.
	void SetPosition(float x, float y, float z) {
		m_vPos.x = x;
		m_vPos.y = y;
		m_vPos.z = z;
	}
	void SetPosition(const D3DXVECTOR3& vPos) {
		m_vPos = vPos;
	}

	void SetPattern(LONG x, LONG y) {
		m_PatternNo.x = x;
		m_PatternNo.y = y;
	}

	void SetAlpha(const float Alpha) {
		m_fAlpha = Alpha;
	}

private:
	//======================================
	//	構造体.
	//======================================
	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｱﾌﾟﾘ側の定義.
	//※ｼｪｰﾀﾞ内のｺﾝｽﾀﾝﾄﾊﾞｯﾌｧと一致している必要あり.
	struct SHADER_CONSTANT_BUFFER
	{

		ALIGN16 D3DMATRIX	mW;				//ﾜｰﾙﾄﾞ行列.
		ALIGN16	float		fViewPortWidth;	//ﾋﾞｭｰﾎﾟｰﾄ幅.
		ALIGN16	float		fViewPortHeight;//ﾋﾞｭｰﾎﾟｰﾄ高さ.
		ALIGN16 float       fAlpha;			//アルファ値(透過で使用する).
		ALIGN16 D3DXVECTOR2	vUV;			//UV座標.
	};
	//頂点の構造体.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	//頂点座標.
		D3DXVECTOR2	Tex;	//ﾃｸｽﾁｬ座標.
	};

	//↓ｱﾌﾟﾘにひとつ.
	ID3D11Device*			m_pDevice11;	//ﾃﾞﾊﾞｲｽｵﾌﾞｼﾞｪｸﾄ.
	ID3D11DeviceContext*	m_pContext11;	//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.

	//↓ﾓﾃﾞﾙの種類ごとに用意.
	ID3D11VertexShader*		m_pVertexShader;	//頂点ｼｪｰﾀﾞ.
	ID3D11InputLayout*		m_pVertexLayout;	//頂点ﾚｲｱｳﾄ.
	ID3D11PixelShader*		m_pPixelShader;		//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
	ID3D11Buffer*			m_pConstantBuffer;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.

	//↓ﾓﾃﾞﾙごとに用意.
	ID3D11Buffer*			m_pVertexBuffer;	//頂点ﾊﾞｯﾌｧ.

	ID3D11SamplerState*		m_pSampleLinear;	//ｻﾝﾌﾟﾗ:ﾃｸｽﾁｬに各種ﾌｨﾙﾀをかける.
	ID3D11ShaderResourceView*	m_pTexture;		//ﾃｸｽﾁｬ.

	int		m_AnimCount;	//ｶｳﾝﾀ.
	D3DXVECTOR2	m_UV;		//ﾃｸｽﾁｬUV座標.

	ID3D11BlendState*		m_pAlphaBlend;		//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ有効.
	ID3D11BlendState*		m_pNoAlphaBlend;	//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ無効.


	D3DXVECTOR3		m_vPos;	//位置.

	float			m_fAlpha;	//アルファ値.

	SPRITE_STATE	m_SState;	//スプライト情報.
	POINT			m_PatternNo;//パターン番号.
};

#endif//#ifndef CSPRITE_UI_H