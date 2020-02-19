#include "CDX9Mesh.h"
#include <crtdbg.h>	//_ASSERTϸۂŕK�v.


//�ݽ�׸�.
CDX9Mesh::CDX9Mesh()
	: m_hWnd(nullptr)
	, m_pD3d9(nullptr)
	, m_pDevice9(nullptr)
	, m_pDevice11(nullptr)
	, m_pContext11(nullptr)

	, m_pVertexShader(nullptr)
	, m_pVertexLayout(nullptr)
	, m_pPixelShader(nullptr)
	, m_pCBufferPerMesh(nullptr)
	, m_pCBufferPerMaterial(nullptr)
	, m_pVertexBuffer(nullptr)
	, m_ppIndexBuffer(nullptr)
	, m_pSampleLinear(nullptr)

	, m_pMesh(nullptr)
	, m_NumMaterials(0)
	, m_pMaterials(nullptr)
	, m_NumAttr(0)
	, m_AttrID()
	, m_EnableTexture(false)
	, m_pAlphaBlend(nullptr)
	, m_pNoAlphaBlend(nullptr)

	, m_fAlpha(1.0f)

	, m_vScale( 1.0f)
	, m_vRot(0.0f, 0.0f, 0.0f)
	, m_vPos(0.0f, 0.0f, 0.0f)
{
}

//�޽�׸�.
CDX9Mesh::~CDX9Mesh()
{
	//�������.
	Release();

}


HRESULT CDX9Mesh::Init(
	HWND hWnd, ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11, const char* fileName, bool Flag)
{
	m_hWnd = hWnd;
	m_pDevice11 = pDevice11;
	m_pContext11 = pContext11;

	if (FAILED(InitDx9(m_hWnd))) {
		return E_FAIL;
	}
	if (FAILED(LoadXMesh(fileName))) {
		return E_FAIL;
	}
	if (FAILED(InitShader())){
		return E_FAIL;
	}
	//�����޽ðč쐬.
	if (FAILED(InitBlend(Flag))) {
		return E_FAIL;
	}
	return S_OK;
}


//Dx9������.
HRESULT CDX9Mesh::InitDx9(HWND hWnd)
{
	m_hWnd = hWnd;

	//�uDirect3D�v��޼ު�Ă̍쐬.
	m_pD3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3d9 == nullptr) {
		_ASSERT_EXPR(false, L"Dx9��޼ު�č쐬���s");
		return E_FAIL;
	}

	//Diret3D���޲���޼ު�Ă̍쐬.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;	//�ޯ��ޯ̧��̫�ϯ�(��̫��).
	d3dpp.BackBufferCount = 1;					//�ޯ��ޯ̧�̐�.
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//�ܯ�ߴ̪��(��̫��).
	d3dpp.Windowed = true;						//����޳Ӱ��.
	d3dpp.EnableAutoDepthStencil = true;		//��ݼٗL��.
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	//��ݼق�̫�ϯ�(16bit).

	//���޲��쐬(HALӰ��:�`��ƒ��_������GPU�ōs��).
	if (FAILED(m_pD3d9->CreateDevice(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pDevice9)))
	{
		//���޲��쐬(HALӰ��:�`���GPU�A���_������CPU�ōs��).
		if (FAILED(m_pD3d9->CreateDevice(
			D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pDevice9)))
		{
			MessageBox(nullptr,
				"HALӰ�ނ����޲��쐬�ł��܂���\nREFӰ�ނōĎ��s���܂�",
				"�x��", MB_OK);

			//���޲��쐬(REFӰ��:�`���CPU�A���_������GPU�ōs��).
			if (FAILED(m_pD3d9->CreateDevice(
				D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, m_hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp, &m_pDevice9)))
			{
				//���޲��쐬(REFӰ��:�`��ƒ��_������CPU�ōs��).
				if (FAILED(m_pD3d9->CreateDevice(
					D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, m_hWnd,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&d3dpp, &m_pDevice9)))
				{
					MessageBox(nullptr,
						"Direct3D���޲��쐬���s", "�x��", MB_OK);
					return E_FAIL;
				}
			}
		}
	}

	return S_OK;
}


//ү���ǂݍ���.
HRESULT CDX9Mesh::LoadXMesh(const char* fileName)
{
	//��ر��ޯ̧.
	LPD3DXBUFFER pD3DXMtrlBuffer = nullptr;

	//X̧�ق�۰��.
	if (FAILED(D3DXLoadMeshFromXA(
		fileName,	//̧�ٖ�.
		D3DXMESH_SYSTEMMEM	//������؂ɓǂݍ���.
			| D3DXMESH_32BIT,	//32bit.
		m_pDevice9, nullptr,
		&pD3DXMtrlBuffer,	//(out)��رُ��.
		nullptr,
		&m_NumMaterials,	//(out)��رِ�.
		&m_pMesh)))			//(out)ү����޼ު��.
	{
		_ASSERT_EXPR(false, L"X̧�ٓǍ����s");
		return E_FAIL;
	}

	D3D11_BUFFER_DESC		bd;	//Dx11�ޯ̧�\����.
	D3D11_SUBRESOURCE_DATA	InitData;//�������ް�.
	//�ǂݍ��񂾏�񂩂�K�v�ȏ��𔲂��o��.
	D3DXMATERIAL* d3dxMaterials
		= static_cast<D3DXMATERIAL*>(pD3DXMtrlBuffer->GetBufferPointer());
	//��رِ����̗̈���m��.
	m_pMaterials = new MY_MATERIAL[m_NumMaterials]();
	m_ppIndexBuffer = new ID3D11Buffer*[m_NumMaterials]();
	//��رِ����J��Ԃ�.
	for (DWORD No = 0; No < m_NumMaterials; No++) 
	{
		//���ޯ���ޯ̧�̏�����.
		m_ppIndexBuffer[No] = nullptr;

		//��رُ��̺�߰.
		//���޴��.
		m_pMaterials[No].Ambient.x = d3dxMaterials[No].MatD3D.Ambient.r;
		m_pMaterials[No].Ambient.y = d3dxMaterials[No].MatD3D.Ambient.g;
		m_pMaterials[No].Ambient.z = d3dxMaterials[No].MatD3D.Ambient.b;
		m_pMaterials[No].Ambient.w = d3dxMaterials[No].MatD3D.Ambient.a;
		//�ި̭���.
		m_pMaterials[No].Diffuse.x = d3dxMaterials[No].MatD3D.Diffuse.r;
		m_pMaterials[No].Diffuse.y = d3dxMaterials[No].MatD3D.Diffuse.g;
		m_pMaterials[No].Diffuse.z = d3dxMaterials[No].MatD3D.Diffuse.b;
		m_pMaterials[No].Diffuse.w = d3dxMaterials[No].MatD3D.Diffuse.a;
		//��߷��.
		m_pMaterials[No].Specular.x = d3dxMaterials[No].MatD3D.Specular.r;
		m_pMaterials[No].Specular.y = d3dxMaterials[No].MatD3D.Specular.g;
		m_pMaterials[No].Specular.z = d3dxMaterials[No].MatD3D.Specular.b;
		m_pMaterials[No].Specular.w = d3dxMaterials[No].MatD3D.Specular.a;

		//(���̖ʂ�)ø������\���Ă��邩�H.
		if( d3dxMaterials[No].pTextureFilename != nullptr
			&& lstrlen(d3dxMaterials[No].pTextureFilename) > 0)
		{
			//ø���������׸ނ𗧂Ă�.
			m_EnableTexture = true;

			char path[128] = "";
			int path_count = lstrlen(fileName);
			for (int k = path_count; k >= 0; k--) {
				if (fileName[k] == '\\') {
					for (int j = 0; j <= k; j++) {
						path[j] = fileName[j];
					}
					path[k + 1] = '\0';
					break;
				}
			}
			//�߽��ø���̧�ٖ���A��.
			strcat_s(path, sizeof(path), d3dxMaterials[No].pTextureFilename);

			//ø���̧�ٖ����߰.
			strcpy_s(m_pMaterials[No].szTextureName,
				sizeof(m_pMaterials[No].szTextureName),	path);

			// ø����쐬.
			if (FAILED(D3DX11CreateShaderResourceViewFromFileA(
				m_pDevice11, m_pMaterials[No].szTextureName,//ø���̧�ٖ�.
				nullptr, nullptr,
				&m_pMaterials[No].pTexture,//(out)ø�����޼ު��.
				nullptr)))
			{
				_ASSERT_EXPR(false, L"ø����쐬���s");
				return E_FAIL;
			}
		}
	}

	//------------------------------------------------
	//	���ޯ���ޯ̧�쐬.
	//------------------------------------------------
	//ү���̑������𓾂�.
	//�������Ų��ޯ���ޯ̧����ׂ�����رق��Ƃ̲��ޯ���ޯ̧�𕪗��ł���.
	D3DXATTRIBUTERANGE* pAttrTable = nullptr;

	//ү���̖ʂ���ђ��_�̏��ԕύX�𐧌䂵�A��̫��ݽ���œK������.
	//D3DXMESHOPT_COMPACT : �ʂ̏��Ԃ�ύX���A�g�p����Ă��Ȃ����_�Ɩʂ��폜����.
	//D3DXMESHOPT_ATTRSORT : ��̫��ݽ���グ��ׁA�ʂ̏��Ԃ�ύX���čœK�����s��.
	m_pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT,
		nullptr, nullptr, nullptr, nullptr);
	//����ð��ق̎擾.
	m_pMesh->GetAttributeTable(nullptr, &m_NumAttr);
	pAttrTable = new D3DXATTRIBUTERANGE[m_NumAttr];
	if (FAILED(m_pMesh->GetAttributeTable(pAttrTable, &m_NumAttr)))
	{
		_ASSERT_EXPR(false, L"����ð��َ擾���s");
		return E_FAIL;
	}

	//������Lock���Ȃ��Ǝ��o���Ȃ�.
	int* pIndex = nullptr;
	m_pMesh->LockIndexBuffer(
		D3DLOCK_READONLY, (void**)&pIndex);
	//�������Ƃ̲��ޯ���ޯ̧���쐬.
	for (DWORD No = 0; No < m_NumAttr; No++)
	{
		m_AttrID[No] = pAttrTable[No].AttribId;
		//Dx9�̲��ޯ���ޯ̧����̏��ŁADx11�̲��ޯ���ޯ̧���쐬.
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth
			= sizeof(int)*pAttrTable[No].FaceCount * 3;//�ʐ��~3�Œ��_��.
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		//�傫�����ޯ���ޯ̧���̵̾��(�~3����).
		InitData.pSysMem = &pIndex[pAttrTable[No].FaceStart*3];

		if (FAILED(m_pDevice11->CreateBuffer(
			&bd, &InitData, &m_ppIndexBuffer[No])))
		{
			_ASSERT_EXPR(false, L"���ޯ���ޯ̧�쐬���s");
			return E_FAIL;
		}
		//�ʂ̐����߰.
		m_pMaterials[m_AttrID[No]].dwNumFace = pAttrTable[No].FaceCount;
	}
	//����ð��ق̍폜.
	delete[] pAttrTable;
	//�g�p�ς݂̲��ޯ���ޯ̧�̉��.
	m_pMesh->UnlockIndexBuffer();
	//�s�v�ɂȂ�����ر��ޯ̧�����.
	SAFE_RELEASE(pD3DXMtrlBuffer);

	//------------------------------------------------
	//	���_�ޯ̧�̍쐬.
	//------------------------------------------------
	//Dx9�̏ꍇ�Amap�ł͂Ȃ�Lock�Œ��_�ޯ̧�����ް������o��.
	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;
	m_pMesh->GetVertexBuffer(&pVB);
	DWORD dwStride = m_pMesh->GetNumBytesPerVertex();
	BYTE* pVertices = nullptr;
	VERTEX* pVertex = nullptr;
	if (SUCCEEDED(
		pVB->Lock(0, 0, (VOID**)&pVertices, 0)))
	{
		pVertex = (VERTEX*)pVertices;
		//Dx9�̒��_�ޯ̧����̏��ŁADx11���_�ޯ̧���쐬.
		bd.Usage = D3D11_USAGE_DEFAULT;
		//���_���i�[����̂ɕK�v���޲Đ�.
		bd.ByteWidth = m_pMesh->GetNumBytesPerVertex()*m_pMesh->GetNumVertices();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		InitData.pSysMem = pVertex;
		if (FAILED(m_pDevice11->CreateBuffer(
			&bd, &InitData, &m_pVertexBuffer)))
		{
			_ASSERT_EXPR(false, L"���_�ޯ̧�쐬���s");
			return E_FAIL;
		}
		pVB->Unlock();
	}
	SAFE_RELEASE(pVB);	//���_�ޯ̧���.




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
	return S_OK;
}

//����֐�.
void CDX9Mesh::Release()
{

	SAFE_RELEASE(m_pNoAlphaBlend);
	SAFE_RELEASE(m_pAlphaBlend);

	//���ޯ���ޯ̧���.	���o�O��̂Œ����K�v.
	if (m_ppIndexBuffer != nullptr) {
		for (int No = m_NumMaterials-1; No >= 0; No--) {
			if (m_ppIndexBuffer[No] != nullptr) {
				SAFE_RELEASE(m_ppIndexBuffer[No]);
			}
		}
		delete[] m_ppIndexBuffer;
		m_ppIndexBuffer = nullptr;
	}
	//��رى��.
	if (m_pMaterials != nullptr) {
		delete[] m_pMaterials;
		m_pMaterials = nullptr;
	}
	//ү���ް��̉��.
	SAFE_RELEASE(m_pMesh);

	SAFE_RELEASE(m_pSampleLinear);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pCBufferPerMaterial);
	SAFE_RELEASE(m_pCBufferPerMesh);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pVertexShader);

	SAFE_RELEASE(m_pDevice9);
	SAFE_RELEASE(m_pD3d9);

	m_pContext11 = nullptr;
	m_pDevice11 = nullptr;
	m_hWnd = nullptr;
}

//===========================================================
//	HLSĻ�ق�ǂݍ��ݼ���ނ��쐬����.
//	HLSL: High Level Shading Language �̗�.
//===========================================================
HRESULT CDX9Mesh::InitShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif//#ifdef _DEBUG

	//HLSL�����ްï������ނ�����ނ��쐬.
	if (m_EnableTexture == true){
		if (FAILED(
			D3DX11CompileFromFile(
				"Mesh.hlsl", nullptr, nullptr, "VS_Main", "vs_5_0",
				uCompileFlag, 0, nullptr, &pCompiledShader, &pErrors, nullptr)))
		{
			_ASSERT_EXPR(false, L"hlsl�ǂݍ��ݎ��s");
			return E_FAIL;
		}
	}
	else {
		if (FAILED(
			D3DX11CompileFromFile(
				"Mesh.hlsl", nullptr, nullptr, "VS_NoTex", "vs_5_0",
				uCompileFlag, 0, nullptr, &pCompiledShader, &pErrors, nullptr)))
		{
			_ASSERT_EXPR(false, L"hlsl�ǂݍ��ݎ��s");
			return E_FAIL;
		}
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
	D3D11_INPUT_ELEMENT_DESC layout[3];
	//���_���߯�ڲ��Ă̔z��v�f�����Z�o.
	UINT numElements = 0;
	if (m_EnableTexture == true)
	{
		D3D11_INPUT_ELEMENT_DESC tmp[] =
		{
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0, 0,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"NORMAL",  0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,   0,24,D3D11_INPUT_PER_VERTEX_DATA,0}
		};
		numElements = sizeof(tmp) / sizeof(tmp[0]);	//�v�f���Z�o.
		memcpy_s( layout, sizeof(layout),
			tmp, sizeof(D3D11_INPUT_ELEMENT_DESC)*numElements);
	}
	else
	{
		D3D11_INPUT_ELEMENT_DESC tmp[] =
		{
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0, 0,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"NORMAL",  0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0}
		};
		numElements = sizeof(tmp) / sizeof(tmp[0]);	//�v�f���Z�o.
		memcpy_s(layout, sizeof(layout),
			tmp, sizeof(D3D11_INPUT_ELEMENT_DESC)*numElements);
	}

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
	if (m_EnableTexture == true){
		if (FAILED(
			D3DX11CompileFromFile(
				"Mesh.hlsl", nullptr, nullptr, "PS_Main", "ps_5_0",
				uCompileFlag, 0, nullptr, &pCompiledShader, &pErrors, nullptr)))
		{
			_ASSERT_EXPR(false, L"hlsl�ǂݍ��ݎ��s");
			return E_FAIL;
		}
	}
	else {
		if (FAILED(
			D3DX11CompileFromFile(
				"Mesh.hlsl", nullptr, nullptr, "PS_NoTex", "ps_5_0",
				uCompileFlag, 0, nullptr, &pCompiledShader, &pErrors, nullptr)))
		{
			_ASSERT_EXPR(false, L"hlsl�ǂݍ��ݎ��s");
			return E_FAIL;
		}
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
	cb.ByteWidth = sizeof(CBUFFER_PER_MESH);	//�ݽ����ޯ̧�̻���.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//�������݂ű���.
	cb.MiscFlags = 0;	//���̑����׸�(���g�p).
	cb.StructureByteStride = 0;	//�\���̻̂���(���g�p).
	cb.Usage = D3D11_USAGE_DYNAMIC;	//�g�p���@:���ڏ�������.

	//�ݽ����ޯ̧�̍쐬.
	if (FAILED(
		m_pDevice11->CreateBuffer(&cb,nullptr,&m_pCBufferPerMesh)))
	{
		_ASSERT_EXPR(false, L"�ݽ����ޯ̧�쐬���s");
		return E_FAIL;
	}

	//�ݽ����ޯ̧ ��رٗp.
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//�ݽ����ޯ̧���w��.
	cb.ByteWidth = sizeof(CBUFFER_PER_MATERIAL);//�ݽ����ޯ̧�̻���.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//�������݂ű���.
	cb.MiscFlags = 0;	//���̑����׸�(���g�p).
	cb.StructureByteStride = 0;	//�\���̻̂���(���g�p).
	cb.Usage = D3D11_USAGE_DYNAMIC;	//�g�p���@:���ڏ�������.

	//�ݽ����ޯ̧�̍쐬.
	if (FAILED(
		m_pDevice11->CreateBuffer(&cb,nullptr,&m_pCBufferPerMaterial)))
	{
		_ASSERT_EXPR(false, L"�ݽ����ޯ̧�쐬���s");
		return E_FAIL;
	}

	return S_OK;
}


//�����ݸޗp.
//��DirectX���������ݸފ֐�.
//  �ŏI�I�ɉ�ʂɏo�͂���̂�Main�׽�������ݸފ֐������.
void CDX9Mesh::Render(
	D3DXMATRIX& mView, D3DXMATRIX& mProj,
	D3DXVECTOR3& vLight, D3DXVECTOR3& vCamPos)
{
	
	//ܰ��ލs��A���ٍs��A��]�s��A���s�ړ��s��.
	D3DXMATRIX mWorld, mScale, mRot, mTran;
	D3DXMATRIX mYaw, mPitch, mRoll;

	//�g��k���s��쐬.
	D3DXMatrixScaling(
		&mScale,	//(out)�v�Z����.
		m_vScale, m_vScale, m_vScale);	//x,y,z���ꂼ��̊g�k�l.
	//Y����]�s��쐬.
	D3DXMatrixRotationY( &mYaw, m_vRot.y);
	//X����]�s��쐬.
	D3DXMatrixRotationX( &mPitch, m_vRot.x);
	//Z����]�s��쐬.
	D3DXMatrixRotationZ( &mRoll, m_vRot.z);

	//���s�ړ��s��쐬.
	D3DXMatrixTranslation(
		&mTran,	//(out)�v�Z����.
		m_vPos.x, m_vPos.y, m_vPos.z);	//x,y,z���W.

	//��]�s����쐬.
	mRot = mYaw * mPitch * mRoll;

	//ܰ��ލs��쐬.
	//�g�k�~��]�~�ړ� �����Ԃ��ƂĂ���؁I�I.
	mWorld = mScale * mRot * mTran;


	//�g�p���鼪��ނ̾��.
	m_pContext11->VSSetShader(m_pVertexShader, nullptr, 0);//���_�����.
	m_pContext11->PSSetShader(m_pPixelShader, nullptr, 0);//�߸�ټ����.

	//����ނ̺ݽ����ޯ̧�Ɋe���ް���n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	//�ޯ̧�����ް��̏��������J�n����Map.
	if (SUCCEEDED(m_pContext11->Map(
		m_pCBufferPerMesh, 0,
		D3D11_MAP_WRITE_DISCARD,
		0, &pData)))
	{
		CBUFFER_PER_MESH cb;	//�ݽ����ޯ̧.

		//ܰ��ލs���n��.
		cb.mW = mWorld;
		D3DXMatrixTranspose(&cb.mW, &cb.mW);

		//ܰ���,�ޭ�,��ۼު���ݍs���n��.
		D3DXMATRIX mWVP = mWorld * mView * mProj;
		D3DXMatrixTranspose(&mWVP, &mWVP);//�s���]�u����.
		//���s��̌v�Z���@��DirectX��GPU�ňقȂ邽�ߓ]�u���K�v.
		cb.mWVP = mWVP;

		//ײĕ�����n��.
		cb.vLightDir = static_cast<D3DXVECTOR4>(vLight);

		//��ׂ̈ʒu(���_)�𼪰�ނɓn��.
		cb.vCamPos = D3DXVECTOR4(
			vCamPos.x, vCamPos.y, vCamPos.z, 0.0f);
		
		cb.fAlpha = m_fAlpha;

		memcpy_s(
			pData.pData,	//��߰����ޯ̧.
			pData.RowPitch,	//��߰����ޯ̧����.
			(void*)(&cb),	//��߰�����ޯ̧.
			sizeof(cb));	//��߰�����ޯ̧����.

		//�ޯ̧�����ް��̏��������I������Unmap.
		m_pContext11->Unmap(m_pCBufferPerMesh, 0);
	}

	//���̺ݽ����ޯ̧���ǂ̼���ނŎg�p���邩�H.
	m_pContext11->VSSetConstantBuffers(
		0, 1, &m_pCBufferPerMesh);	//���_�����.
	m_pContext11->PSSetConstantBuffers(
		0, 1, &m_pCBufferPerMesh);	//�߸�ټ����.

	//���_���߯�ڲ��Ă��.
	m_pContext11->IASetInputLayout(m_pVertexLayout);

	//����è�ށE���ۼް���.
	m_pContext11->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//���_�ޯ̧���.
	UINT stride = m_pMesh->GetNumBytesPerVertex();
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers(
		0, 1, &m_pVertexBuffer, &stride, &offset);

	
	//�����̐������A���ꂼ��̑����̲��ޯ���ޯ̧��`��.
	for (DWORD No = 0; No < m_NumAttr; No++)
	{
		//�g�p����Ă��Ȃ���رّ΍�.
		if (m_pMaterials[m_AttrID[No]].dwNumFace == 0) {
			continue;
		}
		//���ޯ���ޯ̧���.
		m_pContext11->IASetIndexBuffer(
			m_ppIndexBuffer[No], DXGI_FORMAT_R32_UINT, 0);
		//��رق̊e�v�f�𼪰�ނɓn��.
		D3D11_MAPPED_SUBRESOURCE pDataMat;
		if (SUCCEEDED(
			m_pContext11->Map(m_pCBufferPerMaterial,
				0, D3D11_MAP_WRITE_DISCARD, 0, &pDataMat)))
		{
			CBUFFER_PER_MATERIAL cb;
			//���޴��,�ި̭���,��߷�ׂ𼪰�ނɓn��.
			cb.vAmbient = m_pMaterials[m_AttrID[No]].Ambient;
			cb.vDiffuse = m_pMaterials[m_AttrID[No]].Diffuse;
			cb.vSpecular= m_pMaterials[m_AttrID[No]].Specular;

			memcpy_s(pDataMat.pData, pDataMat.RowPitch,
				(void*)&cb, sizeof(cb));

			m_pContext11->Unmap(m_pCBufferPerMaterial, 0);
		}

		//���̺ݽ����ޯ̧���ǂ̼���ނŎg�����H.
		m_pContext11->VSSetConstantBuffers(1, 1, &m_pCBufferPerMaterial);
		m_pContext11->PSSetConstantBuffers(1, 1, &m_pCBufferPerMaterial);

		//ø����𼪰�ނɓn��.
		if (m_pMaterials[m_AttrID[No]].pTexture != nullptr) {
			//ø���������Ƃ�.
			m_pContext11->PSSetSamplers(0, 1, &m_pSampleLinear);
			m_pContext11->PSSetShaderResources(
				0, 1, &m_pMaterials[m_AttrID[No]].pTexture);
		}
		else {
			//ø������Ȃ��Ƃ�.
			ID3D11ShaderResourceView* pNothing[1] = { 0 };
			m_pContext11->PSSetShaderResources(0, 1, pNothing);
		}
		
		SetBlend(true);
		//����è��(��غ��)�������ݸ�.
		m_pContext11->DrawIndexed(
			m_pMaterials[m_AttrID[No]].dwNumFace * 3, 0, 0);
		SetBlend(false);
	}
	
}

HRESULT CDX9Mesh::InitBlend(bool Flag)
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
		= Flag;	//true:��̧�����ނ��g�p����.
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

void CDX9Mesh::SetBlend(bool EnableAlpha)
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
