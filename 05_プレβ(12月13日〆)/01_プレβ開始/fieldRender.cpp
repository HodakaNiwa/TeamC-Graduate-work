//*****************************************************************************
//
//     �n�ʕ`��̏���[fieldRender.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "fieldRender.h"
#include "manager.h"
#include "renderer.h"
#include "shader.h"
#include "debuglog.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************


//*****************************************************************************
//     �ÓI�����o�ϐ��錾
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CFieldRender::CFieldRender(int nPriority, OBJTYPE objType)
{
	m_pVtxBuff = NULL;
	m_pWorldBuff = NULL;
	m_pColorBuff = NULL;
	m_pIdxBuff = NULL;
	m_pDecl = NULL;
	m_pFieldShader = NULL;
	m_pTexture = NULL;
	m_nNumInstance = 0;
	m_bDisp = false;
	m_bLighting = false;
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CFieldRender::~CFieldRender()
{

}

//=============================================================================
//    ��������
//=============================================================================
CFieldRender *CFieldRender::Create(int nNumInstance, int nPriority)
{
	CFieldRender *pFieldRender = NULL;
	if (pFieldRender != NULL) { return NULL; }

	// ���������m��
	pFieldRender = new CFieldRender(nPriority);
	if (pFieldRender == NULL) { return NULL; }

	// �l�̐ݒ�
	pFieldRender->SetNumInstance(nNumInstance);

	// ����������
	if (FAILED(pFieldRender->Init()))
	{
		pFieldRender->Uninit();
		return NULL;
	}

	return pFieldRender;	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CFieldRender::Init(void)
{
	// �`��f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

	// ���_�o�b�t�@�̐���
	if (FAILED(MakeVertex(pDevice)))
	{
		return E_FAIL;
	}

	// ���[���h�}�g���b�N�X�o�b�t�@�̐���
	if (FAILED(MakeWorldBuff(pDevice)))
	{
		return E_FAIL;
	}

	// ���_�J���[�o�b�t�@�̐���
	if (FAILED(MakeColorBuff(pDevice)))
	{
		return E_FAIL;
	}

	// �C���f�b�N�X�o�b�t�@�̐���
	if (FAILED(MakeIndex(pDevice)))
	{
		return E_FAIL;
	}

	// ���_�錾�f�[�^�̐���
	if (FAILED(MakeDeclaration(pDevice)))
	{
		return E_FAIL;
	}

	// ���C�e�B���O�ƕ`�������ݒ��
	m_bDisp = true;
	m_bLighting = true;

	return S_OK;
}

//=============================================================================
//    ���_�o�b�t�@�̐�������
//=============================================================================
HRESULT CFieldRender::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEXDATA) * 4,
		D3DUSAGE_WRITEONLY,
		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1),
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);
	if (m_pVtxBuff == NULL) { return E_FAIL; }

	return S_OK;
}

//=============================================================================
//    ���[���h�}�g���b�N�X�o�b�t�@�̐�������
//=============================================================================
HRESULT CFieldRender::MakeWorldBuff(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���[���h�}�g���b�N�X�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(WORLDMATRIX) * m_nNumInstance,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&m_pWorldBuff,
		NULL);
	if (m_pWorldBuff == NULL) { return E_FAIL; }

	return S_OK;
}

//=============================================================================
//    ���_�J���[�o�b�t�@�̐�������
//=============================================================================
HRESULT CFieldRender::MakeColorBuff(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�J���[�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(COLORDATA) * m_nNumInstance,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&m_pColorBuff,
		NULL);
	if (m_pColorBuff == NULL) { return E_FAIL; }

	return S_OK;
}

//=============================================================================
//    �C���f�b�N�X�o�b�t�@�̐�������
//=============================================================================
HRESULT CFieldRender::MakeIndex(const LPDIRECT3DDEVICE9 pDevice)
{
	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * 6,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);
	if (m_pIdxBuff == NULL) { return E_FAIL; }

	WORD *pIdx;       // �C���f�b�N�X�f�[�^�ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N��,�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0] = 0;
	pIdx[1] = 1;
	pIdx[2] = 2;
	pIdx[3] = 2;
	pIdx[4] = 1;
	pIdx[5] = 3;

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();

	return S_OK;
}

//=============================================================================
//    ���_�錾�f�[�^�̍쐬����
//=============================================================================
HRESULT CFieldRender::MakeDeclaration(const LPDIRECT3DDEVICE9 pDevice)
{	// �K�v�ȃf�[�^�𗅗�
	D3DVERTEXELEMENT9 declElems[] =
	{
		// Stream0(�J��Ԃ��f�[�^�̑�)
		{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },	// ���_���W
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 }, 	// �@���x�N�g��
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },	// UV���W

		// Stream1(���[���h��ԏ�̈ʒu)
		{ 1,  0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },	// WORLD 1�s��
		{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },	// WORLD 2�s��
		{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },	// WORLD 3�s��
		{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },	// WORLD 4�s��

		// Stream2(���_�J���[)
		{ 2, 0, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },	// ���_�J���[
		D3DDECL_END()
	};

	// ���_�錾�̍쐬
	if (FAILED(pDevice->CreateVertexDeclaration(declElems, &m_pDecl)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CFieldRender::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	DIRECT_RELEASE(m_pVtxBuff);

	// ���[���h�}�g���b�N�X�o�b�t�@�̊J��
	DIRECT_RELEASE(m_pWorldBuff);

	// ���_�J���[�o�b�t�@�̊J��
	DIRECT_RELEASE(m_pColorBuff);

	// �C���f�b�N�X�o�b�t�@�̊J��
	DIRECT_RELEASE(m_pIdxBuff);

	// ���_�錾�f�[�^�̊J��
	DIRECT_RELEASE(m_pDecl);

	// ���S�t���O�𗧂Ă�
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CFieldRender::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CFieldRender::Draw(void)
{
	if (m_pFieldShader == NULL || m_bDisp == false) { return; }

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return; }

	// �C���X�^���V���O�錾
	pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | m_nNumInstance);
	pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);
	pDevice->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	// ���_�錾�f�[�^��ݒ�
	pDevice->SetVertexDeclaration(m_pDecl);

	// ���_�o�b�t�@���X�g���[����0�Ԃɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEXDATA));

	// ���[���h�}�g���b�N�X�o�b�t�@���X�g���[����1�Ԃɐݒ�
	pDevice->SetStreamSource(1, m_pWorldBuff, 0, sizeof(WORLDMATRIX));

	// ���_�J���[�o�b�t�@���X�g���[����2�Ԃɐݒ�
	pDevice->SetStreamSource(2, m_pColorBuff, 0, sizeof(COLORDATA));

	// �C���f�b�N�X�o�b�t�@��ݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// �v���W�F�N�V�����}�g���b�N�X�ƃr���[�}�g���b�N�X�̐ݒ�
	D3DXMATRIX mtxView, mtxProj;
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	m_pFieldShader->SetMtxProjection(mtxProj);
	m_pFieldShader->SetMtxView(mtxView);

	// �e�N�X�`���̐ݒ�
	m_pFieldShader->BindTexture(m_pTexture);

	// ���C�g�̐ݒ�
	D3DLIGHT9 Light;
	for (int nCntLight = 0; nCntLight < MAXLIGHT; nCntLight++)
	{
		pDevice->GetLight(nCntLight, &Light);
		m_pFieldShader->SetLightDir(Light.Direction, nCntLight);
		m_pFieldShader->SetLightDiffuse(Light.Diffuse, nCntLight);
	}

	// �V�F�[�_�[�ɒl���Z�b�g
	m_pFieldShader->SetParamToEffect();

	// �`��J�n
	m_pFieldShader->Begin();

	// �����_�����O�p�X�J�n
	int nPassIdx = 0;
	if (m_pTexture == NULL) { nPassIdx = 1; }
	if (m_bLighting == false) { nPassIdx += 2; }
	m_pFieldShader->BeginPass(nPassIdx);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	// �����_�����O�p�X�I��
	m_pFieldShader->EndPass();

	// �`��I��
	m_pFieldShader->End();

	// ��n��
	pDevice->SetVertexDeclaration(NULL);
	pDevice->SetStreamSourceFreq(0, 1);
	pDevice->SetStreamSourceFreq(1, 1);
	pDevice->SetStreamSourceFreq(2, 1);
	pDevice->SetStreamSource(1, NULL, 0, 0);
	pDevice->SetStreamSource(2, NULL, 0, 0);
}