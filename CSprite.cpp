#include "CSprite.h"
#include <crtdbg.h>

//�����̧�ٖ�(�ިڸ�؂��܂�).
const char SHADER_NAME[] = "Mesh.hlsl";



//�ݽ�׸�.
CSprite::CSprite()
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
	, m_fAlpha(0.5f)
	, m_pAlphaBlend(nullptr)
	, m_pNoAlphaBlend(nullptr)
	, m_vPos()
	//, m_fAlpha(1.0f)
{

}

//�޽�׸�.
CSprite::~CSprite()
{
	Release();
}

//������.
HRESULT CSprite::Init(
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11, const char* fileName, SPRITE_STATE* pSs  )
{
	m_pDevice11 = pDevice11;
	m_pContext11 = pContext11;

	m_SState = *pSs;

	//�����޽ðč쐬.
	if (FAILED(InitBlend()))
	{
		return E_FAIL;
	}
	//����ލ쐬.
	if (FAILED(InitShader()))
	{
		return E_FAIL;
	}
	//����غ�ݍ쐬.
	if (FAILED(InitModel(fileName)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//���.
void CSprite::Release()
{
	SAFE_RELEASE(m_pNoAlphaBlend);
	SAFE_RELEASE(m_pAlphaBlend);
	
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
//	HLSĻ�ق�ǂݍ��ݼ���ނ��쐬����.
//	HLSL: High Level Shading Language �̗�.
//===========================================================
HRESULT CSprite::InitShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif//#ifdef _DEBUG

	//HLSL�����ްï������ނ�����ނ��쐬.
	if (FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME,	//�����̧�ٖ�(HLSĻ��).
			nullptr,		//ϸے�`�̔z��ւ��߲��(���g�p).
			nullptr,		//�ݸٰ��̧�ق���������̪���ւ��߲��(���g�p).
			"VS_Ita",		//����޴��ذ�߲�Ċ֐��̖��O.
			"vs_5_0",		//����ނ����ق��w�肷�镶����(���̧��).
			uCompileFlag,	//����޺��߲��׸�.
			0,				//�̪�ĺ��߲��׸�(���g�p).
			nullptr,		//�گ�� ����� ����̪���ւ��߲��(���g�p).
			&pCompiledShader,//����ނ��i�[������؂ւ��߲��.
			&pErrors,		//�װ�ƌx���ꗗ���i�[������؂ւ��߲��.
			nullptr)))		//�߂�l�ւ��߲��(���g�p).
	{
		_ASSERT_EXPR(false, L"hlsl�ǂݍ��ݎ��s");
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//��L�ō쐬��������ނ���u�ްï������ށv���쐬.
	if (FAILED(
		m_pDevice11->CreateVertexShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pVertexShader)))	//(out)�ްï�������.
	{
		_ASSERT_EXPR(false, L"�ްï������ލ쐬���s");
		return E_FAIL;
	}

	//���_���߯�ڲ��Ă��`.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{
			"POSITION",						//�ʒu.
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,	//DXGI��̫�ϯ�(32bit float�^*3).
			0,
			0,								//�ް��̊J�n�ʒu.
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD",						//ø����ʒu.
			0,
			DXGI_FORMAT_R32G32_FLOAT,		//DXGI��̫�ϯ�(32bit float�^*2).
			0,
			12,								//�ް��̊J�n�ʒu.
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};
	//���_���߯�ڲ��Ă̔z��v�f�����Z�o.
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//���_���߯�ڲ��Ă��쐬.
	if (FAILED(
		m_pDevice11->CreateInputLayout(
			layout,
			numElements,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			&m_pVertexLayout)))	//(out)���_���߯�ڲ���.
	{
		_ASSERT_EXPR(false, L"���_���߯�ڲ��č쐬���s");
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//HLSL�����߸�ټ���ނ�����ނ��쐬.
	if (FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME,	//�����̧�ٖ�(HLSĻ��).
			nullptr,		//ϸے�`�̔z��ւ��߲��(���g�p).
			nullptr,		//�ݸٰ��̧�ق���������̪���ւ��߲��(���g�p).
			"PS_Ita",		//����޴��ذ�߲�Ċ֐��̖��O.
			"ps_5_0",		//����ނ����ق��w�肷�镶����(���̧��).
			uCompileFlag,	//����޺��߲��׸�.
			0,				//�̪�ĺ��߲��׸�(���g�p).
			nullptr,		//�گ�� ����� ����̪���ւ��߲��(���g�p).
			&pCompiledShader,//����ނ��i�[������؂ւ��߲��.
			&pErrors,		//�װ�ƌx���ꗗ���i�[������؂ւ��߲��.
			nullptr)))		//�߂�l�ւ��߲��(���g�p).
	{
		_ASSERT_EXPR(false, L"hlsl�ǂݍ��ݎ��s");
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//��L�ō쐬��������ނ���u�߸�ټ���ށv���쐬.
	if (FAILED(
		m_pDevice11->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pPixelShader)))	//(out)�߸�ټ����.
	{
		_ASSERT_EXPR(false, L"�߸�ټ���ލ쐬���s");
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//�ݽ���(�萔)�ޯ̧�쐬.
	//����ނɓ���̐��l�𑗂��ޯ̧.
	//�����ł͕ϊ��s��n���p.
	//����ނ� World, View, Projection �s���n��.
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//�ݽ����ޯ̧���w��.
	cb.ByteWidth = sizeof(SHADER_CONSTANT_BUFFER);//�ݽ����ޯ̧�̻���.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//�������݂ű���.
	cb.MiscFlags = 0;	//���̑����׸�(���g�p).
	cb.StructureByteStride = 0;	//�\���̻̂���(���g�p).
	cb.Usage = D3D11_USAGE_DYNAMIC;	//�g�p���@:���ڏ�������.

	//�ݽ����ޯ̧�̍쐬.
	if (FAILED(
		m_pDevice11->CreateBuffer(
			&cb,
			nullptr,
			&m_pConstantBuffer)))
	{
		_ASSERT_EXPR(false, L"�ݽ����ޯ̧�쐬���s");
		return E_FAIL;
	}


	return S_OK;
}


//���ٍ쐬.
HRESULT CSprite::InitModel(const char* fileName)
{

	//float w, h;
	//w = h = (1.0f / 8.0f);
	float w = m_SState.Disp.w;//�\���X�v���C�g��.
	float h = m_SState.Disp.h;//�\���X�v���C�g����.
	float u = m_SState.Stride.w / m_SState.Base.w;//1�R�}������̕�.
	float v = m_SState.Stride.h / m_SState.Base.h;//1�R�}������̕�.


	//�����(�l�p�`)�̒��_���쐬.
	VERTEX vertices[] =
	{
		////���_���W(x,y,z)				 UV���W(u,v)
		//D3DXVECTOR3(-Scale, -Scale, 0.0f), D3DXVECTOR2(0.0f,    h),	//���_�P(����).
		//D3DXVECTOR3(-Scale,  Scale, 0.0f), D3DXVECTOR2(0.0f, 0.0f),	//���_�Q(����).
		//D3DXVECTOR3(Scale,   -Scale, 0.0f), D3DXVECTOR2(w,    h),	//���_�R(�E��).
		//D3DXVECTOR3(Scale,    Scale, 0.0f), D3DXVECTOR2(w, 0.0f)	//���_�S(�E��).

				//���_���W(x,y,z)				 UV���W(u,v)
		D3DXVECTOR3(0.0f,    h, 0.0f), D3DXVECTOR2(0.0f,    v),	//���_�P(����).
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f),	//���_�Q(����).
		D3DXVECTOR3(w,    h, 0.0f), D3DXVECTOR2(u,    v),	//���_�R(�E��).
		D3DXVECTOR3(w, 0.0f, 0.0f), D3DXVECTOR2(u, 0.0f)	//���_�S(�E��).

	};
	//�ő�v�f�����Z�o����.
	UINT uVerMax = sizeof(vertices) / sizeof(vertices[0]);

	//�ޯ̧�\����.
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;	//�g�p���@(��̫��).
	bd.ByteWidth = sizeof(VERTEX) * uVerMax;//���_�̻���.
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//���_�ޯ̧�Ƃ��Ĉ���.
	bd.CPUAccessFlags = 0;	//CPU����ͱ������Ȃ�.
	bd.MiscFlags = 0;		//���̑����׸�(���g�p).
	bd.StructureByteStride = 0;	//�\���̻̂���(���g�p).

	//���ؿ���ް��\����.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;	//����؂̒��_���.

	//���_�ޯ̧�̍쐬.
	if (FAILED(m_pDevice11->CreateBuffer(
		&bd, &InitData, &m_pVertexBuffer)))
	{
		_ASSERT_EXPR(false, L"���_�ޯ̧�쐬���s");
		return E_FAIL;
	}

	//���_�ޯ̧���.
	UINT stride = sizeof(VERTEX);	//�ް��Ԋu.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers(
		0, 1,
		&m_pVertexBuffer, &stride, &offset);

	//ø����p�̻���׍\����.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory(&samDesc, sizeof(samDesc));
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;//�Ʊ̨��(���`���).
						//POINT:���������e��.
	samDesc.AddressU
		= D3D11_TEXTURE_ADDRESS_WRAP;//ׯ��ݸ�Ӱ��(WRAP:�J��Ԃ�).
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//MIRROR: ���]�J��Ԃ�.
	//CLAMP : �[�̖͗l�������L�΂�.
	//BORDER: �ʓr���E�F�����߂�.
//����׍쐬.
	if (FAILED(m_pDevice11->CreateSamplerState(
		&samDesc, &m_pSampleLinear)))//(out)�����.
	{
		_ASSERT_EXPR(false, L"����׍쐬���s");
		return E_FAIL;
	}

	//ø����쐬.
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(
		m_pDevice11,	//ؿ�����g�p�������޲����߲��.
		fileName,	//̧�ٖ�.
		nullptr, nullptr,
		&m_pTexture,	//(out)ø���.
		nullptr)))
	{
		_ASSERT_EXPR(false, L"ø����쐬���s");
		return E_FAIL;
	}

	return S_OK;
}


//�����ݸޗp.
void CSprite::Render(
	D3DXMATRIX& mView, D3DXMATRIX& mProj, D3DXVECTOR3& vCamPos)
{

	//ܰ��ލs��.
	D3DXMATRIX	mWorld;

	//���s�ړ�.
	D3DXMatrixTranslation(&mWorld,
		m_vPos.x, m_vPos.y, m_vPos.z);

	//�g�p���鼪��ނ̓o�^.
	m_pContext11->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pContext11->PSSetShader(m_pPixelShader, nullptr, 0);

	//����ް�ޗp.
	D3DXMATRIX CancelRotation = mView;//�ޭ��s��.
	CancelRotation._41
		= CancelRotation._42 = CancelRotation._43 = 0.0f;//xyz��0�ɂ���.
	//CancelRotation�̋t�s������߂܂�.
	D3DXMatrixInverse(&CancelRotation, nullptr, &CancelRotation);
	mWorld = CancelRotation * mWorld;

	////����ނ̺ݽ����ޯ̧�Ɋe���ް���n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	SHADER_CONSTANT_BUFFER		cb;		//�R���X�^���g�o�b�t�@.

	//�ޯ̧�����ް��̏��������J�n����map.
	if (SUCCEEDED(
		m_pContext11->Map(m_pConstantBuffer,
			0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{

		//ܰ���,�ޭ�,��ۼު���ݍs���n��.
		D3DXMATRIX m = mWorld * mView * mProj;
		D3DXMatrixTranspose(&m, &m);//�s���]�u����.
		cb.mWVP = m;
		//�װ.
		cb.vColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

		cb.fAlpha = m_fAlpha;

		cb.vUV = D3DXVECTOR4(m_UV.x, m_UV.y, 0.0f, 0.0f);

		memcpy_s(pData.pData, pData.RowPitch,
			(void*)(&cb), sizeof(cb));

		m_pContext11->Unmap(m_pConstantBuffer, 0);
	}

	//���̺ݽ����ޯ̧���ǂ̼���ނŎg�����H.
	m_pContext11->VSSetConstantBuffers(2, 1, &m_pConstantBuffer);
	m_pContext11->PSSetConstantBuffers(2, 1, &m_pConstantBuffer);

	//���_�ޯ̧���.
	UINT stride = sizeof(VERTEX);//�ް��̊Ԋu.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers(0, 1,
		&m_pVertexBuffer, &stride, &offset);

	//���_���߯�ڲ��Ă��.
	m_pContext11->IASetInputLayout(m_pVertexLayout);
	//����è�ށE���ۼް���.
	m_pContext11->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ø����𼪰�ނɓn��.
	m_pContext11->PSSetSamplers(0, 1, &m_pSampleLinear);
	m_pContext11->PSSetShaderResources(0, 1, &m_pTexture);

	//��̧�����ޗL���ɂ���.
	SetBlend(true);

	//����è�ނ������ݸ�.
	m_pContext11->Draw(4, 0);//�����(���_4��).

	//��̧�����ޖ����ɂ���.
	SetBlend(false);
}

HRESULT CSprite::InitBlend( )
{
	//��̧�����ޗp�����޽ðč\����.
	//pnģ�ٓ��ɱ�̧��񂪂���̂ŁA���߂���悤�������޽ðĂŐݒ肷��.
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));	//������.

	BlendDesc.IndependentBlendEnable
		= false;	//false:RenderTarget[0]�����ް�̂ݎg�p����.
					//true:RenderTarget[0�`7]���g�p�ł���
					// (���ް���ޯĖ��ɓƗ����������ޏ���).
	BlendDesc.AlphaToCoverageEnable
		= false;	//true:��̧ĩ���ڰ�ނ��g�p����.
	BlendDesc.RenderTarget[0].BlendEnable
		= true;	//true:��̧�����ނ��g�p����.
	BlendDesc.RenderTarget[0].SrcBlend	//���f�ނɑ΂���ݒ�.
		= D3D11_BLEND_SRC_ALPHA;			//��̧�����ނ��w��.
	BlendDesc.RenderTarget[0].DestBlend	//�d�˂�f�ނɑ΂���ݒ�.
		= D3D11_BLEND_INV_SRC_ALPHA;		//��̧�����ނ̔��]���w��.
	BlendDesc.RenderTarget[0].BlendOp	//�����޵�߼��.
		= D3D11_BLEND_OP_ADD;				//ADD:���Z����.
	BlendDesc.RenderTarget[0].SrcBlendAlpha	//���f�ނ̱�̧�ɑ΂���w��.
		= D3D11_BLEND_ONE;						//���̂܂܎g�p.
	BlendDesc.RenderTarget[0].DestBlendAlpha	//�d�˂�f�ނ̱�̧�ɑ΂���ݒ�.
		= D3D11_BLEND_ZERO;							//�������Ȃ��B
	BlendDesc.RenderTarget[0].BlendOpAlpha	//��̧�������޵�߼��.
		= D3D11_BLEND_OP_ADD;					//ADD:���Z����.
	BlendDesc.RenderTarget[0].RenderTargetWriteMask	//�߸�ٖ��̏�������Ͻ�.
		= D3D11_COLOR_WRITE_ENABLE_ALL;			//�S�Ă̐���(RGBA)�ւ��ް��̊i�[��������.

	//�����޽ðč쐬.
	if (FAILED(
		m_pDevice11->CreateBlendState(
			&BlendDesc, &m_pAlphaBlend)))
	{
		_ASSERT_EXPR(false, L"�����޽ðč쐬���s");
		return E_FAIL;
	}

	//�����޽ðč쐬.
	BlendDesc.RenderTarget[0].BlendEnable
		= false;	//false:��̧�����ނ��g�p���Ȃ�.
	if (FAILED(
		m_pDevice11->CreateBlendState(
			&BlendDesc, &m_pNoAlphaBlend)))
	{
		_ASSERT_EXPR(false, L"�����޽ðč쐬���s");
		return E_FAIL;
	}

	return S_OK;
}

//���ߐݒ�̐؂�ւ�.
void CSprite::SetBlend(bool EnableAlpha)
{
	//�����޽ðĂ̐ݒ�.
	UINT mask = 0xffffffff;	//Ͻ��l.
	if (EnableAlpha == true) {
		m_pContext11->OMSetBlendState(
			m_pAlphaBlend, nullptr, mask);
	}
	else {
		m_pContext11->OMSetBlendState(
			m_pNoAlphaBlend, nullptr, mask);
	}
}