//*****************************************************************************
//
//     �e�`��̏���[shadowRender.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "shadowRender.h"
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "shader.h"
#include "camera.h"
#include "game.h"
#include "debuglog.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define SHADOWRENDER_SHADER_FILANAME "data/ringShader.hlsl"
#define SHADOWRENDER_PRIORITY (8)
#define SHADOWRENDER_SHADOW_PRIORITY (3)

//*****************************************************************************
//     �ÓI�����o�ϐ��錾
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CShadowRender::CShadowRender(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pVtxBuff = NULL;
	m_pWorldBuff = NULL;
	m_pColorBuff = NULL;
	m_pSizeBuff = NULL;
	m_pIdxBuff = NULL;
	m_pDecl = NULL;
	m_pShadowShader = NULL;
	m_pTexture = NULL;
	m_nNumInstance = 0;
	m_bDisp = false;
	m_bLighting = false;
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CShadowRender::~CShadowRender()
{

}

//=============================================================================
//    ��������
//=============================================================================
CShadowRender *CShadowRender::Create(void)
{
	CShadowRender *pShadowRender = NULL;
	if (pShadowRender != NULL) { return NULL; }

	// ���������m��
	pShadowRender = new CShadowRender(SHADOWRENDER_SHADOW_PRIORITY + 1);
	if (pShadowRender == NULL) { return NULL; }

	// �C���X�^���X����ݒ�
	int nNumInstance = 0;

	// �S�Ẵ����O������
	CScene *pScene = GetTop(SHADOWRENDER_SHADOW_PRIORITY);
	CScene *pSceneNext = NULL;
	while (pScene != NULL)
	{
		pSceneNext = pScene->GetpNext();
		if (pScene->GetObjType() == OBJTYPE_SHADOW)
		{// �e��������
			nNumInstance++;
		}
		pScene = pSceneNext;
	}

	// �l�̐ݒ�
	pShadowRender->SetNumInstance(nNumInstance);

	// ����������
	if (FAILED(pShadowRender->Init()))
	{
		pShadowRender->Uninit();
		return NULL;
	}

	// �e�N�X�`����ݒ肷��
	pShadowRender->BindTexture(CShadow::GetTexture());

	return pShadowRender;	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CShadowRender::Init(void)
{
	// �C���X�^���X����������Ώ������Ȃ�
	if (m_nNumInstance <= 0) { return E_FAIL; }

	// �`��f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

	// ���_�o�b�t�@�̐���
	if (FAILED(MakeVertexBuff(pDevice)))
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

	// �T�C�Y�o�b�t�@�̐���
	if (FAILED(MakeSizeBuff(pDevice)))
	{
		return E_FAIL;
	}

	// �C���f�b�N�X�o�b�t�@�̐���
	if (FAILED(MakeIdxBuff(pDevice)))
	{
		return E_FAIL;
	}

	// ���_�錾�f�[�^�̐���
	if (FAILED(MakeDeclaration(pDevice)))
	{
		return E_FAIL;
	}

	// �V�F�[�_�[�̓ǂݍ��ݏ���
	if (FAILED(LoadShader()))
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
HRESULT CShadowRender::MakeVertexBuff(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEXDATA) * 4,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);
	if (m_pVtxBuff == NULL) { return E_FAIL; }

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	VERTEXDATA *pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-1.0f, 0.0f, 1.0f);
	pVtx[1].pos = D3DXVECTOR3(1.0f, 0.0f, 1.0f);
	pVtx[2].pos = D3DXVECTOR3(-1.0f, 0.0f, -1.0f);
	pVtx[3].pos = D3DXVECTOR3(1.0f, 0.0f, -1.0f);

	// �@���x�N�g��
	pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	return S_OK;
}

//=============================================================================
//    ���[���h�}�g���b�N�X�o�b�t�@�̐�������
//=============================================================================
HRESULT CShadowRender::MakeWorldBuff(const LPDIRECT3DDEVICE9 pDevice)
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
HRESULT CShadowRender::MakeColorBuff(const LPDIRECT3DDEVICE9 pDevice)
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
//    �T�C�Y�o�b�t�@�̐�������
//=============================================================================
HRESULT CShadowRender::MakeSizeBuff(const LPDIRECT3DDEVICE9 pDevice)
{
	// �T�C�Y�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(SIZEDATA) * m_nNumInstance,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&m_pSizeBuff,
		NULL);
	if (m_pSizeBuff == NULL) { return E_FAIL; }

	return S_OK;
}

//=============================================================================
//    �C���f�b�N�X�o�b�t�@�̐�������
//=============================================================================
HRESULT CShadowRender::MakeIdxBuff(const LPDIRECT3DDEVICE9 pDevice)
{
	if (m_pIdxBuff != NULL) { return E_FAIL; }

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * 6,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);
	if (m_pIdxBuff == NULL) { return E_FAIL; }

	WORD *pIdx;  // �C���f�b�N�X�f�[�^�ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N��,�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// ����
	pIdx[0] = 0;
	pIdx[1] = 1;
	pIdx[2] = 2;

	// �E��
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
HRESULT CShadowRender::MakeDeclaration(const LPDIRECT3DDEVICE9 pDevice)
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

		// Stream3(�T�C�Y)
		{ 3, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5 },	// �T�C�Y
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
//    �V�F�[�_�[��ǂݍ��ޏ���
//=============================================================================
HRESULT CShadowRender::LoadShader(void)
{
	if (m_pShadowShader != NULL) { return E_FAIL; }
	m_pShadowShader = CShadowShader::Create(SHADOWRENDER_SHADER_FILANAME);
	if (m_pShadowShader == NULL) { return E_FAIL; }

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CShadowRender::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	DIRECT_RELEASE(m_pVtxBuff);

	// ���[���h�}�g���b�N�X�o�b�t�@�̊J��
	DIRECT_RELEASE(m_pWorldBuff);

	// ���_�J���[�o�b�t�@�̊J��
	DIRECT_RELEASE(m_pColorBuff);

	// �T�C�Y�o�b�t�@�̊J��
	DIRECT_RELEASE(m_pSizeBuff);

	// �C���f�b�N�X�o�b�t�@�̊J��
	DIRECT_RELEASE(m_pIdxBuff);

	// ���_�錾�f�[�^�̊J��
	DIRECT_RELEASE(m_pDecl);

	// �V�F�[�_�[�̊J��
	if (m_pShadowShader != NULL)
	{
		m_pShadowShader->Uninit();
		delete m_pShadowShader;
		m_pShadowShader = NULL;
	}

	// ���S�t���O�𗧂Ă�
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CShadowRender::Update(void)
{
	// �C���X�^���X����������
	m_nNumInstance = 0;

	// ���[���h�}�g���b�N�X�o�b�t�@�̊J��
	DIRECT_RELEASE(m_pWorldBuff);

	// ���_�J���[�o�b�t�@�̊J��
	DIRECT_RELEASE(m_pColorBuff);

	// �T�C�Y�o�b�t�@�̊J��
	DIRECT_RELEASE(m_pSizeBuff);

	// �S�Ẵ����O������
	CScene *pScene = GetTop(SHADOWRENDER_SHADOW_PRIORITY);
	CScene *pSceneNext = NULL;
	while (pScene != NULL)
	{
		pSceneNext = pScene->GetpNext();
		if (pScene->GetObjType() == OBJTYPE_SHADOW)
		{// �e��������
			m_nNumInstance++;
		}
		pScene = pSceneNext;
	}

	// �`��f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return; }

	// ���[���h�}�g���b�N�X�o�b�t�@�̐���
	if (FAILED(MakeWorldBuff(pDevice)))
	{
		return;
	}

	// ���_�J���[�o�b�t�@�̐���
	if (FAILED(MakeColorBuff(pDevice)))
	{
		return;
	}

	// �T�C�Y�o�b�t�@�̐���
	if (FAILED(MakeSizeBuff(pDevice)))
	{
		return;
	}

	// ���[���h�}�g���b�N�X�o�b�t�@�����b�N���A���[���h�}�g���b�N�X�f�[�^�ւ̃|�C���^���擾
	CShadowRender::WORLDMATRIX *pWorld = NULL;
	m_pWorldBuff->Lock(0, 0, (void**)&pWorld, 0);

	// ���_�J���[�o�b�t�@�����b�N���A���_�J���[�f�[�^�ւ̃|�C���^���擾
	CShadowRender::COLORDATA *pColor = NULL;
	m_pColorBuff->Lock(0, 0, (void**)&pColor, 0);

	// �T�C�Y�o�b�t�@�����b�N���A�T�C�Y�f�[�^�ւ̃|�C���^���擾
	CShadowRender::SIZEDATA *pSize = NULL;
	m_pSizeBuff->Lock(0, 0, (void**)&pSize, 0);

	// �S�Ẵ����O��T���ă��X�g�ɒǉ�����
	pScene = GetTop(SHADOWRENDER_SHADOW_PRIORITY);
	pSceneNext = NULL;
	while (pScene != NULL)
	{
		pSceneNext = pScene->GetpNext();
		if (pScene->GetObjType() == OBJTYPE_SHADOW)
		{// �e��������
			AddList((CShadow*)pScene, pWorld, pColor, pSize);
			pWorld++;
			pColor++;
			pSize++;
		}
		pScene = pSceneNext;
	}

	// ���[���h�}�g���b�N�X�o�b�t�@���A�����b�N����
	m_pWorldBuff->Unlock();

	// ���_�J���[�o�b�t�@���A�����b�N����
	m_pColorBuff->Unlock();

	// �T�C�Y�o�b�t�@���A�����b�N����
	m_pSizeBuff->Unlock();
}

//=============================================================================
//    �o�b�t�@�ɏ������ޏ�������
//=============================================================================
void CShadowRender::AddList(CShadow *pShadow, WORLDMATRIX *pWorld, COLORDATA *pCol, SIZEDATA *pSize)
{
	// ���[���h�}�g���b�N�X������������
	AddWorld(pShadow, pWorld);

	// ���_�J���[������������
	AddColor(pShadow, pCol);

	// �T�C�Y������������
	AddSize(pShadow, pSize);
}

//=============================================================================
//    ���[���h�}�g���b�N�X�����������ޏ���
//=============================================================================
void CShadowRender::AddWorld(CShadow *pShadow, WORLDMATRIX *pWorld)
{
	D3DXVECTOR3 pos = INITIALIZE_VECTOR3;
	D3DXVECTOR3 rot = INITIALIZE_VECTOR3;
	D3DXVECTOR3 Scale = pShadow->GetSize();
	Scale.y = 1.0f;
	pos = pShadow->GetPos();
	rot = pShadow->GetRot();
	float fSinPitch = sinf(rot.x);
	float fCosPitch = cosf(rot.x);
	float fSinYaw = sinf(rot.y);
	float fCosYaw = cosf(rot.y);
	float fSinRoll = sinf(rot.z);
	float fCosRoll = cosf(rot.z);
	pWorld[0].m1[0] = fSinRoll * fSinPitch * fSinYaw + fCosRoll * fCosYaw;
	pWorld[0].m1[1] = fSinRoll * fCosPitch;
	pWorld[0].m1[2] = fSinRoll * fSinPitch * fCosYaw - fCosRoll * fSinYaw;
	pWorld[0].m1[3] = 0.0f;
	pWorld[0].m2[0] = fCosRoll * fSinPitch * fSinYaw - fSinRoll * fCosYaw;
	pWorld[0].m2[1] = fCosRoll * fCosPitch;
	pWorld[0].m2[2] = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fSinYaw;
	pWorld[0].m2[3] = 0.0f;
	pWorld[0].m3[0] = fCosPitch * fSinYaw;
	pWorld[0].m3[1] = -fSinPitch;
	pWorld[0].m3[2] = fCosPitch * fCosYaw;
	pWorld[0].m3[3] = 0.0f;
	pWorld[0].m4[0] = pos.x;
	pWorld[0].m4[1] = pos.y;
	pWorld[0].m4[2] = pos.z;
	pWorld[0].m4[3] = 1.0f;

	// �傫���𔽉f������
	pWorld[0].m1[0] *= Scale.x;
	pWorld[0].m1[1] *= Scale.x;
	pWorld[0].m1[2] *= Scale.x;
	pWorld[0].m2[0] *= Scale.y;
	pWorld[0].m2[1] *= Scale.y;
	pWorld[0].m2[2] *= Scale.y;
	pWorld[0].m3[0] *= Scale.z;
	pWorld[0].m3[1] *= Scale.z;
	pWorld[0].m3[2] *= Scale.z;
}

//=============================================================================
//    �F�����������ޏ���
//=============================================================================
void CShadowRender::AddColor(CShadow *pShadow, COLORDATA *pCol)
{
	pCol[0].col = pShadow->GetCol();
}

//=============================================================================
//    �T�C�Y�����������ޏ���
//=============================================================================
void CShadowRender::AddSize(CShadow *pShadow, SIZEDATA *pSize)
{
	pSize[0].fWidth = pShadow->GetSize().x;
	pSize[0].fDepth = pShadow->GetSize().z;
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CShadowRender::Draw(void)
{
	if (m_pShadowShader == NULL || m_bDisp == false) { return; }
	CGame *pGame = CManager::GetGame();				// �Q�[���̎擾
	CEventCamera *pEveCam = pGame->GetEveCam();		// �C�x���g�J�����̎擾
	if (pEveCam != NULL)
	{// �C�x���g���͕`�悵�Ȃ�
		return;
	}

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return; }

	// �C���X�^���V���O�錾
	pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | m_nNumInstance);
	pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);
	pDevice->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA | 1);
	pDevice->SetStreamSourceFreq(3, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	// ���_�錾�f�[�^��ݒ�
	pDevice->SetVertexDeclaration(m_pDecl);

	// ���_�o�b�t�@���X�g���[����0�Ԃɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEXDATA));

	// ���[���h�}�g���b�N�X�o�b�t�@���X�g���[����1�Ԃɐݒ�
	pDevice->SetStreamSource(1, m_pWorldBuff, 0, sizeof(WORLDMATRIX));

	// ���_�J���[�o�b�t�@���X�g���[����2�Ԃɐݒ�
	pDevice->SetStreamSource(2, m_pColorBuff, 0, sizeof(COLORDATA));

	// �T�C�Y�o�b�t�@���X�g���[����3�Ԃɐݒ�
	pDevice->SetStreamSource(3, m_pSizeBuff, 0, sizeof(SIZEDATA));

	// �C���f�b�N�X�o�b�t�@��ݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// �v���W�F�N�V�����}�g���b�N�X�ƃr���[�}�g���b�N�X�̐ݒ�
	D3DXMATRIX mtxView, mtxProj;
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	m_pShadowShader->SetMtxProjection(mtxProj);
	m_pShadowShader->SetMtxView(mtxView);

	// �e�N�X�`���̐ݒ�
	m_pShadowShader->BindTexture(m_pTexture);

	// ���C�g�̐ݒ�
	D3DLIGHT9 Light;
	for (int nCntLight = 0; nCntLight < MAXLIGHT; nCntLight++)
	{
		pDevice->GetLight(nCntLight, &Light);
		m_pShadowShader->SetLightDir(Light.Direction, nCntLight);
		m_pShadowShader->SetLightDiffuse(Light.Diffuse, nCntLight);
	}

	// �V�F�[�_�[�ɒl���Z�b�g
	m_pShadowShader->SetParamToEffect();

	// �`��J�n
	m_pShadowShader->Begin();

	// �����_�����O�p�X�J�n
	int nPassIdx = 0;
	if (m_pTexture == NULL) { nPassIdx = 1; }
	m_pShadowShader->BeginPass(nPassIdx);

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �����_�����O�p�X�I��
	m_pShadowShader->EndPass();

	// �`��I��
	m_pShadowShader->End();

	// ��n��
	pDevice->SetVertexDeclaration(NULL);
	pDevice->SetStreamSourceFreq(0, 1);
	pDevice->SetStreamSourceFreq(1, 1);
	pDevice->SetStreamSourceFreq(2, 1);
	pDevice->SetStreamSourceFreq(3, 1);
	pDevice->SetStreamSource(1, NULL, 0, 0);
	pDevice->SetStreamSource(2, NULL, 0, 0);
	pDevice->SetStreamSource(3, NULL, 0, 0);
}