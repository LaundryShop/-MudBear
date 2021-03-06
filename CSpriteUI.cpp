#include "CSpriteUI.h"
#include <crtdbg.h>

//ｼｪｰﾀﾞﾌｧｲﾙ名(ﾃﾞｨﾚｸﾄﾘも含む).
const char SHADER_NAME[] = "SpriteUI.hlsl";

//ｺﾝｽﾄﾗｸﾀ.
CSpriteUI::CSpriteUI()
	: m_pDevice11(nullptr)
	, m_pContext11(nullptr)
	, m_pVertexShader(nullptr)
	, m_pVertexLayout(nullptr)
	, m_pPixelShader(nullptr)
	, m_pConstantBuffer(nullptr)
	, m_pVertexBuffer(nullptr)
	, m_pSampleLinear(nullptr)
	, m_pTexture(nullptr)
	, m_AnimCount(0)
	, m_UV(0.0f, 0.0f)
	, m_pAlphaBlend(nullptr)
	, m_pNoAlphaBlend(nullptr)
	, m_vPos()
	, m_fAlpha(1.0f) //0:透明,1:完全不透明.
	, m_SState()
	, m_PatternNo()
{
}

//ﾃﾞｽﾄﾗｸﾀ.
CSpriteUI::~CSpriteUI()
{
	Release();
}

//初期化.
HRESULT CSpriteUI::Init(
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11,
	const char* fileName, SPRITE_STATE* pSs)
{
	m_pDevice11 = pDevice11;
	m_pContext11 = pContext11;

	m_SState = *pSs;

	//ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ作成.
	if (FAILED(InitBlend()))
	{
		return E_FAIL;
	}
	//ｼｪｰﾀﾞ作成.
	if (FAILED(InitShader()))
	{
		return E_FAIL;
	}
	//板ﾎﾟﾘｺﾞﾝ作成.
	if (FAILED(InitModel()))
	{
		return E_FAIL;
	}
	//ﾃｸｽﾁｬ作成.
	if (FAILED(CreateTexture(fileName, &m_pTexture)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//解放.
void CSpriteUI::Release()
{
	SAFE_RELEASE(m_pAlphaBlend);
	SAFE_RELEASE(m_pNoAlphaBlend);

	SAFE_RELEASE(m_pSampleLinear);
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pVertexShader);

	m_pContext11 = nullptr;
	m_pDevice11 = nullptr;
}

//===========================================================
//	HLSLﾌｧｲﾙを読み込みｼｪｰﾀﾞを作成する.
//	HLSL: High Level Shading Language の略.
//===========================================================
HRESULT CSpriteUI::InitShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif//#ifdef _DEBUG

	//HLSLからﾊﾞｰﾃｯｸｽｼｪｰﾀﾞのﾌﾞﾛﾌﾞを作成.
	if (FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME,	//ｼｪｰﾀﾞﾌｧｲﾙ名(HLSLﾌｧｲﾙ).
			nullptr,		//ﾏｸﾛ定義の配列へのﾎﾟｲﾝﾀ(未使用).
			nullptr,		//ｲﾝｸﾙｰﾄﾞﾌｧｲﾙを扱うｲﾝﾀｰﾌｪｲｽへのﾎﾟｲﾝﾀ(未使用).
			"VS_Main",		//ｼｪｰﾀﾞｴﾝﾄﾘｰﾎﾟｲﾝﾄ関数の名前.
			"vs_5_0",		//ｼｪｰﾀﾞのﾓﾃﾞﾙを指定する文字列(ﾌﾟﾛﾌｧｲﾙ).
			uCompileFlag,	//ｼｪｰﾀﾞｺﾝﾊﾟｲﾙﾌﾗｸﾞ.
			0,				//ｴﾌｪｸﾄｺﾝﾊﾟｲﾙﾌﾗｸﾞ(未使用).
			nullptr,		//ｽﾚｯﾄﾞ ﾎﾟﾝﾌﾟ ｲﾝﾀｰﾌｪｲｽへのﾎﾟｲﾝﾀ(未使用).
			&pCompiledShader,//ﾌﾞﾛﾌﾞを格納するﾒﾓﾘへのﾎﾟｲﾝﾀ.
			&pErrors,		//ｴﾗｰと警告一覧を格納するﾒﾓﾘへのﾎﾟｲﾝﾀ.
			nullptr)))		//戻り値へのﾎﾟｲﾝﾀ(未使用).
	{
		_ASSERT_EXPR(false, L"hlsl読み込み失敗");
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//上記で作成したﾌﾞﾛﾌﾞから「ﾊﾞｰﾃｯｸｽｼｪｰﾀﾞ」を作成.
	if (FAILED(
		m_pDevice11->CreateVertexShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pVertexShader)))	//(out)ﾊﾞｰﾃｯｸｽｼｪｰﾀﾞ.
	{
		_ASSERT_EXPR(false, L"ﾊﾞｰﾃｯｸｽｼｪｰﾀﾞ作成失敗");
		return E_FAIL;
	}

	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄを定義.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{
			"POSITION",						//位置.
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,	//DXGIのﾌｫｰﾏｯﾄ(32bit float型*3).
			0,
			0,								//ﾃﾞｰﾀの開始位置.
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD",						//ﾃｸｽﾁｬ位置.
			0,
			DXGI_FORMAT_R32G32_FLOAT,		//DXGIのﾌｫｰﾏｯﾄ(32bit float型*2).
			0,
			12,								//ﾃﾞｰﾀの開始位置.
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};
	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄの配列要素数を算出.
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄを作成.
	if (FAILED(
		m_pDevice11->CreateInputLayout(
			layout,
			numElements,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			&m_pVertexLayout)))	//(out)頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄ.
	{
		_ASSERT_EXPR(false, L"頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄ作成失敗");
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//HLSLからﾋﾟｸｾﾙｼｪｰﾀﾞのﾌﾞﾛﾌﾞを作成.
	if (FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME,	//ｼｪｰﾀﾞﾌｧｲﾙ名(HLSLﾌｧｲﾙ).
			nullptr,		//ﾏｸﾛ定義の配列へのﾎﾟｲﾝﾀ(未使用).
			nullptr,		//ｲﾝｸﾙｰﾄﾞﾌｧｲﾙを扱うｲﾝﾀｰﾌｪｲｽへのﾎﾟｲﾝﾀ(未使用).
			"PS_Main",		//ｼｪｰﾀﾞｴﾝﾄﾘｰﾎﾟｲﾝﾄ関数の名前.
			"ps_5_0",		//ｼｪｰﾀﾞのﾓﾃﾞﾙを指定する文字列(ﾌﾟﾛﾌｧｲﾙ).
			uCompileFlag,	//ｼｪｰﾀﾞｺﾝﾊﾟｲﾙﾌﾗｸﾞ.
			0,				//ｴﾌｪｸﾄｺﾝﾊﾟｲﾙﾌﾗｸﾞ(未使用).
			nullptr,		//ｽﾚｯﾄﾞ ﾎﾟﾝﾌﾟ ｲﾝﾀｰﾌｪｲｽへのﾎﾟｲﾝﾀ(未使用).
			&pCompiledShader,//ﾌﾞﾛﾌﾞを格納するﾒﾓﾘへのﾎﾟｲﾝﾀ.
			&pErrors,		//ｴﾗｰと警告一覧を格納するﾒﾓﾘへのﾎﾟｲﾝﾀ.
			nullptr)))		//戻り値へのﾎﾟｲﾝﾀ(未使用).
	{
		_ASSERT_EXPR(false, L"hlsl読み込み失敗");
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//上記で作成したﾌﾞﾛﾌﾞから「ﾋﾟｸｾﾙｼｪｰﾀﾞ」を作成.
	if (FAILED(
		m_pDevice11->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pPixelShader)))	//(out)ﾋﾟｸｾﾙｼｪｰﾀﾞ.
	{
		_ASSERT_EXPR(false, L"ﾋﾟｸｾﾙｼｪｰﾀﾞ作成失敗");
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//ｺﾝｽﾀﾝﾄ(定数)ﾊﾞｯﾌｧ作成.
	//ｼｪｰﾀﾞに特定の数値を送るﾊﾞｯﾌｧ.
	//ここでは変換行列渡し用.
	//ｼｪｰﾀﾞに World, View, Projection 行列を渡す.
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧを指定.
	cb.ByteWidth = sizeof(SHADER_CONSTANT_BUFFER);//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｻｲｽﾞ.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//書き込みでｱｸｾｽ.
	cb.MiscFlags = 0;	//その他のﾌﾗｸﾞ(未使用).
	cb.StructureByteStride = 0;	//構造体のｻｲｽﾞ(未使用).
	cb.Usage = D3D11_USAGE_DYNAMIC;	//使用方法:直接書き込み.

	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧの作成.
	if (FAILED(
		m_pDevice11->CreateBuffer(
			&cb,
			nullptr,
			&m_pConstantBuffer)))
	{
		_ASSERT_EXPR(false, L"ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ作成失敗");
		return E_FAIL;
	}


	return S_OK;
}


//ﾓﾃﾞﾙ作成.
HRESULT CSpriteUI::InitModel()
{
	float w = m_SState.Disp.w;//表示スプライト幅.
	float h = m_SState.Disp.h;//表示スプライト高さ.
	float u = m_SState.Stride.w / m_SState.Base.w;//1コマ当たりの幅.
	float v = m_SState.Stride.h / m_SState.Base.h;//1コマあたりの幅.

	//板ﾎﾟﾘ(四角形)の頂点を作成.
	VERTEX vertices[] =
	{
		//頂点座標(x,y,z)				 UV座標(u,v)
		D3DXVECTOR3(0.0f,    h, 0.0f), D3DXVECTOR2(0.0f,    v),	//頂点１(左下).
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f),	//頂点２(左上).
		D3DXVECTOR3(   w,    h, 0.0f), D3DXVECTOR2(   u,    v),	//頂点３(右下).
		D3DXVECTOR3(   w, 0.0f, 0.0f), D3DXVECTOR2(   u, 0.0f)	//頂点４(右上).
	};
	//最大要素数を算出する.
	UINT uVerMax = sizeof(vertices) / sizeof(vertices[0]);

	//ﾊﾞｯﾌｧ構造体.
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;	//使用方法(ﾃﾞﾌｫﾙﾄ).
	bd.ByteWidth = sizeof(VERTEX) * uVerMax;//頂点のｻｲｽﾞ.
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//頂点ﾊﾞｯﾌｧとして扱う.
	bd.CPUAccessFlags = 0;	//CPUからはｱｸｾｽしない.
	bd.MiscFlags = 0;		//その他のﾌﾗｸﾞ(未使用).
	bd.StructureByteStride = 0;	//構造体のｻｲｽﾞ(未使用).

	//ｻﾌﾞﾘｿｰｽﾃﾞｰﾀ構造体.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;	//板ﾎﾟﾘの頂点をｾｯﾄ.

	//頂点ﾊﾞｯﾌｧの作成.
	if (FAILED(m_pDevice11->CreateBuffer(
		&bd, &InitData, &m_pVertexBuffer)))
	{
		_ASSERT_EXPR(false, L"頂点ﾊﾞｯﾌｧ作成失敗");
		return E_FAIL;
	}

	//頂点ﾊﾞｯﾌｧをｾｯﾄ.
	UINT stride = sizeof(VERTEX);	//ﾃﾞｰﾀ間隔.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers(
		0, 1,
		&m_pVertexBuffer, &stride, &offset);

	//ﾃｸｽﾁｬ用のｻﾝﾌﾟﾗ構造体.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory(&samDesc, sizeof(samDesc));
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;//ﾘﾆｱﾌｨﾙﾀ(線形補間).
						//POINT:高速だが粗い.
	
	samDesc.AddressU=  D3D11_TEXTURE_ADDRESS_WRAP;//ﾗｯﾋﾟﾝｸﾞﾓｰﾄﾞ(WRAP:繰り返し).
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//MIRROR: 反転繰り返し.
	//CLAMP : 端の模様を引き伸ばす.
	//BORDER: 別途境界色を決める.
//ｻﾝﾌﾟﾗ作成.
	if (FAILED(m_pDevice11->CreateSamplerState(
		&samDesc, &m_pSampleLinear)))//(out)ｻﾝﾌﾟﾗ.
	{
		_ASSERT_EXPR(false, L"ｻﾝﾌﾟﾗ作成失敗");
		return E_FAIL;
	}

	return S_OK;
}

//ﾃｸｽﾁｬ作成.
HRESULT CSpriteUI::CreateTexture(
	const char* fileName,
	ID3D11ShaderResourceView** pTexture)
{
	//ﾃｸｽﾁｬ作成.
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(
		m_pDevice11,	//ﾘｿｰｽを使用するﾃﾞﾊﾞｲｽのﾎﾟｲﾝﾀ.
		fileName,		//ﾌｧｲﾙ名.
		nullptr, nullptr,
		pTexture,		//(out)ﾃｸｽﾁｬ.
		nullptr)))
	{
		_ASSERT_EXPR(false, L"ﾃｸｽﾁｬ作成失敗");
		return E_FAIL;
	}

	return S_OK;
}


//ﾚﾝﾀﾞﾘﾝｸﾞ用.
void CSpriteUI::Render()
{
	//ﾜｰﾙﾄﾞ行列.
	D3DXMATRIX	mWorld;

	//平行移動.
	D3DXMatrixTranslation(&mWorld,
		m_vPos.x, m_vPos.y, m_vPos.z);

	//使用するｼｪｰﾀﾞの登録.
	m_pContext11->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pContext11->PSSetShader(m_pPixelShader, nullptr, 0);


	//ｼｪｰﾀﾞのｺﾝｽﾀﾝﾄﾊﾞｯﾌｧに各種ﾃﾞｰﾀを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	SHADER_CONSTANT_BUFFER cb;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
								//ﾊﾞｯﾌｧ内のﾃﾞｰﾀの書き換え開始時にmap.
	if (SUCCEEDED(
		m_pContext11->Map(m_pConstantBuffer,
			0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ﾜｰﾙﾄﾞ行列を渡す.
		D3DXMATRIX m = mWorld;
		D3DXMatrixTranspose(&m, &m);//行列を転置する.
		cb.mW = m;

		//ﾋﾞｭｰﾎﾟｰﾄの幅、高さを渡す.
		cb.fViewPortWidth = WND_W;
		cb.fViewPortHeight = WND_H;

		//ｱﾙﾌｧ値を渡す.
		cb.fAlpha = m_fAlpha;

		//UV座標.
		//１コマ当たりの割合にﾊﾟﾀｰﾝ番号をかけて位置を設定する.
		cb.vUV.x
			= (m_SState.Stride.w / m_SState.Base.w) *
			static_cast<float>(m_PatternNo.x);
		cb.vUV.y
			= (m_SState.Stride.h / m_SState.Base.h) *
			static_cast<float>(m_PatternNo.y);

		memcpy_s(pData.pData, pData.RowPitch,
			(void*)(&cb), sizeof(cb));

		m_pContext11->Unmap(m_pConstantBuffer, 0);
	}

	//このｺﾝｽﾀﾝﾄﾊﾞｯﾌｧをどのｼｪｰﾀﾞで使うか？.
	m_pContext11->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pContext11->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	//頂点ﾊﾞｯﾌｧをｾｯﾄ.
	UINT stride = sizeof(VERTEX);//ﾃﾞｰﾀの間隔.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers(0, 1,
		&m_pVertexBuffer, &stride, &offset);

	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄをｾｯﾄ.
	m_pContext11->IASetInputLayout(m_pVertexLayout);
	//ﾌﾟﾘﾐﾃｨﾌﾞ・ﾄﾎﾟﾛｼﾞｰをｾｯﾄ.
	m_pContext11->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ﾃｸｽﾁｬをｼｪｰﾀﾞに渡す.
	m_pContext11->PSSetSamplers(0, 1, &m_pSampleLinear);
	m_pContext11->PSSetShaderResources(0, 1, &m_pTexture);

	//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ有効にする.
	SetBlend(true);

	//ﾌﾟﾘﾐﾃｨﾌﾞをﾚﾝﾀﾞﾘﾝｸﾞ.
	m_pContext11->Draw(4, 0);//板ﾎﾟﾘ(頂点4つ分).

							 //ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ無効にする.
	SetBlend(false);
}

//ゲージのﾚﾝﾀﾞﾘﾝｸﾞ用
void CSpriteUI::GaugeRender(float fScale_x)
{
	//ﾜｰﾙﾄﾞ行列、ｽｹｰﾙ行列、平行移動行列.
	D3DXMATRIX mWorld, mScale, mTran;

	//拡大縮小行列作成.
	D3DXMatrixScaling(
		&mScale,	//(out)計算結果.
		fScale_x, 1.0f, 1.0f);	//x,y,zそれぞれの拡縮値.

	//平行移動行列作成.
	D3DXMatrixTranslation(
		&mTran,	//(out)計算結果.
		m_vPos.x, m_vPos.y, m_vPos.z);	//x,y,z座標.

	//ﾜｰﾙﾄﾞ行列作成.
	//拡縮×移動 ※順番がとても大切！！.
	mWorld = mScale * mTran;

	//使用するｼｪｰﾀﾞの登録.
	m_pContext11->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pContext11->PSSetShader(m_pPixelShader, nullptr, 0);


	//ｼｪｰﾀﾞのｺﾝｽﾀﾝﾄﾊﾞｯﾌｧに各種ﾃﾞｰﾀを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	SHADER_CONSTANT_BUFFER cb;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
								//ﾊﾞｯﾌｧ内のﾃﾞｰﾀの書き換え開始時にmap.
	if (SUCCEEDED(
		m_pContext11->Map(m_pConstantBuffer,
			0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ﾜｰﾙﾄﾞ行列を渡す.
		D3DXMATRIX m = mWorld;
		D3DXMatrixTranspose(&m, &m);//行列を転置する.
		cb.mW = m;

		//ﾋﾞｭｰﾎﾟｰﾄの幅、高さを渡す.
		cb.fViewPortWidth = WND_W;
		cb.fViewPortHeight = WND_H;

		//アルファ値を渡す.
		cb.fAlpha = m_fAlpha;//ここの値をいじるとフェイドアウトできる.

		//UV座標.
		//1コマ当たりの割合にパターン番号をかけて位置を設定する.
		cb.vUV.x = (m_SState.Stride.w / m_SState.Base.w) *
			static_cast<float>(m_PatternNo.x);

		cb.vUV.y = (m_SState.Stride.h / m_SState.Base.h) *
			static_cast<float>(m_PatternNo.y);

		memcpy_s(pData.pData, pData.RowPitch,
			(void*)(&cb), sizeof(cb));

		m_pContext11->Unmap(m_pConstantBuffer, 0);
	}

	//このｺﾝｽﾀﾝﾄﾊﾞｯﾌｧをどのｼｪｰﾀﾞで使うか？.
	m_pContext11->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pContext11->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	//頂点ﾊﾞｯﾌｧをｾｯﾄ.
	UINT stride = sizeof(VERTEX);//ﾃﾞｰﾀの間隔.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers(0, 1,
		&m_pVertexBuffer, &stride, &offset);

	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄをｾｯﾄ.
	m_pContext11->IASetInputLayout(m_pVertexLayout);
	//ﾌﾟﾘﾐﾃｨﾌﾞ・ﾄﾎﾟﾛｼﾞｰをｾｯﾄ.
	m_pContext11->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ﾃｸｽﾁｬをｼｪｰﾀﾞに渡す.
	m_pContext11->PSSetSamplers(0, 1, &m_pSampleLinear);
	m_pContext11->PSSetShaderResources(0, 1, &m_pTexture);

	//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ有効にする.
	SetBlend(true);

	//ﾌﾟﾘﾐﾃｨﾌﾞをﾚﾝﾀﾞﾘﾝｸﾞ.
	m_pContext11->Draw(4, 0);//板ﾎﾟﾘ(頂点4つ分).

							 //ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ無効にする.
	SetBlend(false);
}

HRESULT CSpriteUI::InitBlend()
{
	//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ用ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ構造体.
	//pngﾌｧｲﾙ内にｱﾙﾌｧ情報があるので、透過するようにﾌﾞﾚﾝﾄﾞｽﾃｰﾄで設定する.
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));	//初期化.

	BlendDesc.IndependentBlendEnable
		= false;	//false:RenderTarget[0]のﾒﾝﾊﾞｰのみ使用する.
					//true:RenderTarget[0〜7]が使用できる
					// (ﾚﾝﾀﾞｰﾀｰｹﾞｯﾄ毎に独立したﾌﾞﾚﾝﾄﾞ処理).
	BlendDesc.AlphaToCoverageEnable
		= true;	//true:ｱﾙﾌｧﾄｩｶﾊﾞﾚｰｼﾞを使用する.
	BlendDesc.RenderTarget[0].BlendEnable
		= true;	//true:ｱﾙﾌｧﾌﾞﾚﾝﾄﾞを使用する.
	BlendDesc.RenderTarget[0].SrcBlend	//元素材に対する設定.
		= D3D11_BLEND_SRC_ALPHA;			//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞを指定.
	BlendDesc.RenderTarget[0].DestBlend	//重ねる素材に対する設定.
		= D3D11_BLEND_INV_SRC_ALPHA;		//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞの反転を指定.
	BlendDesc.RenderTarget[0].BlendOp	//ﾌﾞﾚﾝﾄﾞｵﾌﾟｼｮﾝ.
		= D3D11_BLEND_OP_ADD;				//ADD:加算合成.
	BlendDesc.RenderTarget[0].SrcBlendAlpha	//元素材のｱﾙﾌｧに対する指定.
		= D3D11_BLEND_ONE;						//そのまま使用.
	BlendDesc.RenderTarget[0].DestBlendAlpha	//重ねる素材のｱﾙﾌｧに対する設定.
		= D3D11_BLEND_ZERO;							//何もしない。
	BlendDesc.RenderTarget[0].BlendOpAlpha	//ｱﾙﾌｧのﾌﾞﾚﾝﾄﾞｵﾌﾟｼｮﾝ.
		= D3D11_BLEND_OP_ADD;					//ADD:加算合成.
	BlendDesc.RenderTarget[0].RenderTargetWriteMask	//ﾋﾟｸｾﾙ毎の書き込みﾏｽｸ.
		= D3D11_COLOR_WRITE_ENABLE_ALL;			//全ての成分(RGBA)へのﾃﾞｰﾀの格納を許可する.

	//ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ作成.
	if (FAILED(
		m_pDevice11->CreateBlendState(
			&BlendDesc, &m_pAlphaBlend)))
	{
		_ASSERT_EXPR(false, L"ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ作成失敗");
		return E_FAIL;
	}

	//ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ作成.
	BlendDesc.RenderTarget[0].BlendEnable
		= false;	//false:ｱﾙﾌｧﾌﾞﾚﾝﾄﾞを使用しない.
	if (FAILED(
		m_pDevice11->CreateBlendState(
			&BlendDesc, &m_pNoAlphaBlend)))
	{
		_ASSERT_EXPR(false, L"ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ作成失敗");
		return E_FAIL;
	}

	return S_OK;
}

//透過設定の切り替え.
void CSpriteUI::SetBlend(bool EnableAlpha)
{
	//ﾌﾞﾚﾝﾄﾞｽﾃｰﾄの設定.
	UINT mask = 0xffffffff;	//ﾏｽｸ値.
	if (EnableAlpha == true) {
		m_pContext11->OMSetBlendState(
			m_pAlphaBlend, nullptr, mask);
	}
	else {
		m_pContext11->OMSetBlendState(
			m_pNoAlphaBlend, nullptr, mask);
	}
}