#include "CParticle.h"
#include <crtdbg.h>

//ｼｪｰﾀﾞﾈｰﾑ.
const char SHADER_NAME[] = "Particle.hlsl";

//関数のｵｰﾊﾞｰﾛｰﾄﾞ.
//同じ関数名で、引数が異なる数や異なる型.
CParticle::CParticle(
	const int MaxParticle,
	const D3DXVECTOR3& vEmitPos)
	: m_MaxParticle(MaxParticle)
	, m_vEmitPos(vEmitPos)

	, m_pDevice11(nullptr)
	, m_pContext11(nullptr)
	, m_pVertexShader(nullptr)
	, m_pVertexLayout(nullptr)
	, m_pGeometryShader(nullptr)
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
	, m_Scale(1.0f)
	, m_pParticleArray(nullptr)
	, m_Frame(0)
{
	//ﾊﾟｰﾃｨｸﾙ数分の配列を作成.
	m_pParticleArray = new PART[m_MaxParticle];

	//ﾊﾟｰﾃｨｸﾙ数分繰り返し(ﾊﾟｰﾃｨｸﾙの初期化)
	for (int i = 0; i < m_MaxParticle; i++)
	{
		m_pParticleArray[i].vPos = vEmitPos;//放射位置(初期位置).
		m_pParticleArray[i].vDir			//方向.
			= D3DXVECTOR3(
				2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f,
				(float)rand() / (float)RAND_MAX,
				2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f);

		D3DXVec3Normalize(
			&m_pParticleArray[i].vDir, &m_pParticleArray[i].vDir);//正規化.

		m_pParticleArray[i].Speed
			= (5.0f + (float)rand() / (float)RAND_MAX)*0.001f;

		m_pParticleArray[i].BirthFrame = rand();
	}
}

//ﾃﾞﾌｫﾙﾄｺﾝｽﾄﾗｸﾀ.
CParticle::CParticle()
	: CParticle(0,D3DXVECTOR3(0.0f,0.0f,0.0f))
{
}

//ﾃﾞｽﾄﾗｸﾀ.
CParticle::~CParticle()
{
	Release();
}

//初期化.
HRESULT CParticle::Init(
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11)
{
	m_pDevice11 = pDevice11;
	m_pContext11 = pContext11;

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
	if (FAILED(InitTexture("Data\\Texture\\particle.png")))
	{
		return E_FAIL;
	}

	return S_OK;
}

//解放.
void CParticle::Release()
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

	SAFE_DELETE_ARRAY(m_pParticleArray);
}

//===========================================================
//	HLSLﾌｧｲﾙを読み込みｼｪｰﾀﾞを作成する.
//	HLSL: High Level Shading Language の略.
//===========================================================
HRESULT CParticle::InitShader()
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
//		{
//			"TEXCOORD",						//ﾃｸｽﾁｬ位置.
//			0,
//			DXGI_FORMAT_R32G32_FLOAT,		//DXGIのﾌｫｰﾏｯﾄ(32bit float型*2).
//			0,
//			12,								//ﾃﾞｰﾀの開始位置.
//			D3D11_INPUT_PER_VERTEX_DATA, 0
//		}
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

	//HLSLからｼﾞｵﾒﾄﾘｼｪｰﾀﾞのﾌﾞﾛﾌﾞを作成.
	if (FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME,	//ｼｪｰﾀﾞﾌｧｲﾙ名(HLSLﾌｧｲﾙ).
			nullptr,		//ﾏｸﾛ定義の配列へのﾎﾟｲﾝﾀ(未使用).
			nullptr,		//ｲﾝｸﾙｰﾄﾞﾌｧｲﾙを扱うｲﾝﾀｰﾌｪｲｽへのﾎﾟｲﾝﾀ(未使用).
			"GS_Point",		//ｼｪｰﾀﾞｴﾝﾄﾘｰﾎﾟｲﾝﾄ関数の名前.
			"gs_5_0",		//ｼｪｰﾀﾞのﾓﾃﾞﾙを指定する文字列(ﾌﾟﾛﾌｧｲﾙ).
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

	//上記で作成したﾌﾞﾛﾌﾞから「ｼﾞｵﾒﾄﾘｼｪｰﾀﾞ」を作成.
	if (FAILED(
		m_pDevice11->CreateGeometryShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pGeometryShader)))	//(out)ｼﾞｵﾒﾄﾘｼｪｰﾀﾞ.
	{
		_ASSERT_EXPR(false, L"ｼﾞｵﾒﾄﾘｼｪｰﾀﾞ作成失敗");
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
HRESULT CParticle::InitModel()
{
	//頂点を作成.
	VERTEX vertices[]=
	{
		//頂点座標(x,y,z)
		D3DXVECTOR3(-0.5f, 0.5f, 0.0f ),
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



	return S_OK;
}


//ﾃｸｽﾁｬ読み込み＆作成.
HRESULT CParticle::InitTexture(const char* fileName)
{
	//ﾃｸｽﾁｬ用のｻﾝﾌﾟﾗ構造体.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory(&samDesc, sizeof(samDesc));
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;//ﾘﾆｱﾌｨﾙﾀ(線形補間).
						//POINT:高速だが粗い.
	samDesc.AddressU
		= D3D11_TEXTURE_ADDRESS_WRAP;//ﾗｯﾋﾟﾝｸﾞﾓｰﾄﾞ(WRAP:繰り返し).
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

	//ﾃｸｽﾁｬ作成.
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(
		m_pDevice11,	//ﾘｿｰｽを使用するﾃﾞﾊﾞｲｽのﾎﾟｲﾝﾀ.
		fileName,		//ﾌｧｲﾙ名.
		nullptr, nullptr,
		&m_pTexture,	//(out)ﾃｸｽﾁｬ.
		nullptr)))
	{
		_ASSERT_EXPR(false, L"ﾃｸｽﾁｬ作成失敗");
		return E_FAIL;
	}

	return S_OK;
}


//ﾚﾝﾀﾞﾘﾝｸﾞ用.
void CParticle::Render(
	D3DXMATRIX& mView,D3DXMATRIX& mProj,D3DXVECTOR3& vCamPos)
{
	//ﾜｰﾙﾄﾞ行列.
	D3DXMATRIX	mWorld, mScale, mTrans;

	m_vPos.x += 0.01f;//少しずつ右へ.

	//拡大・縮小.
	D3DXMatrixScaling(&mScale,
		m_Scale, m_Scale, m_Scale);

	//平行移動.
	D3DXMatrixTranslation(&mTrans,
		m_vPos.x, m_vPos.y, m_vPos.z);

	//ﾜｰﾙﾄﾞ行列.
	mWorld = mScale * mTrans;

	//使用するｼｪｰﾀﾞの登録.
	m_pContext11->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pContext11->GSSetShader(m_pGeometryShader, nullptr, 0);
	m_pContext11->PSSetShader(m_pPixelShader, nullptr, 0);

	//ｼｪｰﾀﾞのｺﾝｽﾀﾝﾄﾊﾞｯﾌｧに各種ﾃﾞｰﾀを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	SHADER_CONSTANT_BUFFER cb;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
	//ﾊﾞｯﾌｧ内のﾃﾞｰﾀの書き換え開始時にmap.
	if (SUCCEEDED(
		m_pContext11->Map(m_pConstantBuffer,
			0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,ﾌﾟﾛｼﾞｪｸｼｮﾝ行列を渡す.
		D3DXMATRIX m = mWorld * mView * mProj;
		D3DXMatrixTranspose(&m, &m);//行列を転置する.
		cb.mWVP = m;

		memcpy_s(pData.pData, pData.RowPitch,
			(void*)(&cb), sizeof(cb));

		m_pContext11->Unmap(m_pConstantBuffer, 0);
	}

	//このｺﾝｽﾀﾝﾄﾊﾞｯﾌｧをどのｼｪｰﾀﾞで使うか？.
	m_pContext11->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pContext11->GSSetConstantBuffers(0, 1, &m_pConstantBuffer);
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
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	//ﾃｸｽﾁｬをｼｪｰﾀﾞに渡す.
	m_pContext11->PSSetSamplers(0, 1, &m_pSampleLinear);
	m_pContext11->PSSetShaderResources(0, 1, &m_pTexture);

	//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ有効にする.
	SetBlend(true);

	//ﾌﾟﾘﾐﾃｨﾌﾞをﾚﾝﾀﾞﾘﾝｸﾞ.
	m_pContext11->Draw(1, 0);//ﾎﾟｲﾝﾄｽﾌﾟﾗｲﾄなので頂点は１つ.

	//ｼﾞｵﾒﾄﾘｼｪｰﾀﾞを無効にする.
	m_pContext11->GSSetShader(nullptr, nullptr, 0);

	//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ無効にする.
	SetBlend(false);
}

HRESULT CParticle::InitBlend()
{
	//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ用ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ構造体.
	//pngﾌｧｲﾙ内にｱﾙﾌｧ情報があるので、透過するようにﾌﾞﾚﾝﾄﾞｽﾃｰﾄで設定する.
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));	//初期化.

	BlendDesc.IndependentBlendEnable
		= false;	//false:RenderTarget[0]のﾒﾝﾊﾞｰのみ使用する.
					//true:RenderTarget[0～7]が使用できる
					// (ﾚﾝﾀﾞｰﾀｰｹﾞｯﾄ毎に独立したﾌﾞﾚﾝﾄﾞ処理).
	BlendDesc.AlphaToCoverageEnable
		= false;	//true:ｱﾙﾌｧﾄｩｶﾊﾞﾚｰｼﾞを使用する.
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
void CParticle::SetBlend(bool EnableAlpha)
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

//ｼｪｰﾀﾞを外す関数.
void CParticle::DetachShader()
{
	//各ｼｪｰﾀﾞをﾃﾞﾀｯﾁする.

	m_pContext11->VSSetShader(nullptr, nullptr, 0);//頂点ｼｪｰﾀﾞ.
	m_pContext11->HSSetShader(nullptr, nullptr, 0);//ﾊﾙｼｪｰﾀﾞ.

	//～～～～ﾃｯｾﾚｰﾀ～～～～(上下の間処理).

	m_pContext11->DSSetShader(nullptr, nullptr, 0);//ﾄﾞﾒｲﾝｼｪｰﾀﾞ.
	m_pContext11->GSSetShader(nullptr, nullptr, 0);//ｼﾞｵﾒﾄﾘｼｪｰﾀﾞ.

	//～～～～ﾗｽﾀﾗｲｻﾞ～～～～(上下の間処理).

	m_pContext11->PSSetShader(nullptr, nullptr, 0);//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
}

//ﾊﾟｰﾃｨｸﾙの移動.
void CParticle::Run()
{
	//ﾌﾚｰﾑのｶｳﾝﾄUP.
	m_Frame++;

	//ﾊﾟｰﾃｨｸﾙ数分繰り返し.
	for (int i = 0; i < m_MaxParticle; i++) 
	{
		//ﾌﾚｰﾑｶｳﾝﾄが開始ﾌﾚｰﾑ(ﾗﾝﾀﾞﾑで設定)より大きくなった.
		if (m_Frame > m_pParticleArray[i].BirthFrame)
		{
			//方向×速度で移動する値を算出.
			m_pParticleArray[i].vPos
				+= m_pParticleArray[i].vDir * m_pParticleArray[i].Speed;
			//重力.
			m_pParticleArray[i].vDir.y -= 0.00098f;
			//地面でﾊﾞｳﾝﾄﾞ.
			if (m_pParticleArray[i].vPos.y < 0.0f) {
				m_pParticleArray[i].vDir.y
					= -m_pParticleArray[i].vDir.y;
			}
		}
	}
}


//全てのﾎﾟｲﾝﾄｽﾌﾟﾗｲﾄをﾚﾝﾀﾞﾘﾝｸﾞ.
void CParticle::RenderAll(
	D3DXMATRIX& mView, D3DXMATRIX& mProj, D3DXVECTOR3& vCamPos)
{
	for (int i = 0; i < m_MaxParticle; i++)
	{
		m_vPos = m_pParticleArray[i].vPos;
		Render(mView, mProj, vCamPos);
	}
	Run();
}