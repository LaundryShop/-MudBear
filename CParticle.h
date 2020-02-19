#ifndef CPARTICLE_H
#define CPARTICLE_H
//警告についてのｺｰﾄﾞ分析を無効にする.4005:再定義.
#pragma warning(disable:4005)
#include <Windows.h>

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


/**************************************************
*	ﾊﾟｰﾃｨｸﾙｸﾗｽ.
**/
class CParticle
{
public:
	//関数のｵｰﾊﾞｰﾛｰﾄﾞ.
	//同じ関数名で、引数が異なる数や異なる型.
	CParticle(const int MaxParticle,
		const D3DXVECTOR3& vEmitPos);
	CParticle();	//ｺﾝｽﾄﾗｸﾀ(引数なしｺﾝｽﾄﾗｸﾀまたはﾃﾞﾌｫﾙﾄｺﾝｽﾄﾗｸﾀとも言う).
	~CParticle();	//ﾃﾞｽﾄﾗｸﾀ.

	//初期化.
	HRESULT Init(ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11);

	//解放.
	void Release();

	//ｼｪｰﾀﾞ作成.
	HRESULT InitShader();
	//ﾓﾃﾞﾙ作成.
	HRESULT InitModel();
	//ﾃｸｽﾁｬ読み込み＆作成.
	HRESULT InitTexture(const char* fileName);

	//ﾚﾝﾀﾞﾘﾝｸﾞ用.
	void Render(D3DXMATRIX& mView, D3DXMATRIX& mProj,
		D3DXVECTOR3& vCamPos);

	HRESULT InitBlend();
	void SetBlend(bool EnableAlpha);

	//ｼｪｰﾀﾞを外す関数.
	void DetachShader();

	//ﾊﾟｰﾃｨｸﾙの移動.
	void Run();

	//全てのﾎﾟｲﾝﾄｽﾌﾟﾗｲﾄをﾚﾝﾀﾞﾘﾝｸﾞ.
	void RenderAll(D3DXMATRIX& mView, D3DXMATRIX& mProj,
		D3DXVECTOR3& vCamPos);

	//位置情報を設定.
	void SetPosition(float x, float y, float z) {
		m_vPos.x = x;
		m_vPos.y = y;
		m_vPos.z = z;
	}
	void SetPosition(const D3DXVECTOR3& vPos) {
		m_vPos = vPos;
	}

private:
	//======================================
	//	構造体.
	//======================================
	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｱﾌﾟﾘ側の定義.
	//※ｼｪｰﾀﾞ内のｺﾝｽﾀﾝﾄﾊﾞｯﾌｧと一致している必要あり.
	struct SHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX	mWVP;		//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,ﾌﾟﾛｼﾞｪｸｼｮﾝの合成変換行列.	
//		D3DXVECTOR4	vColor;		//ｶﾗｰ(RGBAの型に合わせる).
//		D3DXVECTOR4	vUV;		//UV座標(x,yのみ使用).
	};
	//頂点の構造体.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	//頂点座標.
//		D3DXVECTOR2	Tex;	//ﾃｸｽﾁｬ座標.
	};

	//ﾊﾟｰﾃｨｸﾙ用構造体.
	struct PART
	{
		D3DXVECTOR3	vPos;	//位置.
		D3DXVECTOR3	vDir;	//方向.
		float		Speed;	//速度.
		int			BirthFrame;//開始ﾌﾚｰﾑ.
		//構造体の初期化(ｺﾝｽﾄﾗｸﾀ).
		PART()
			: vPos(0.0f, 0.0f, 0.0f)
			, vDir(0.0f, 0.0f, 0.0f)
			, Speed(0.0f)
			, BirthFrame(0)
		{};
	};

	//↓ｱﾌﾟﾘにひとつ.
	ID3D11Device*			m_pDevice11;	//ﾃﾞﾊﾞｲｽｵﾌﾞｼﾞｪｸﾄ.
	ID3D11DeviceContext*	m_pContext11;	//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.

	//↓ﾓﾃﾞﾙの種類ごとに用意.
	ID3D11VertexShader*		m_pVertexShader;	//頂点ｼｪｰﾀﾞ.
	ID3D11InputLayout*		m_pVertexLayout;	//頂点ﾚｲｱｳﾄ.
	ID3D11GeometryShader*	m_pGeometryShader;	//ｼﾞｵﾒﾄﾘｼｪｰﾀﾞ.
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
	float			m_Scale;//拡縮値.

	const int		m_MaxParticle;	//ﾊﾟｰﾃｨｸﾙ最大数.
	D3DXVECTOR3		m_vEmitPos;		//放射位置.
	PART*			m_pParticleArray;//ﾊﾟｰﾃｨｸﾙ配列.
	int				m_Frame;		//ﾌﾚｰﾑ数.
};

#endif//#ifndef CPARTICLE_H