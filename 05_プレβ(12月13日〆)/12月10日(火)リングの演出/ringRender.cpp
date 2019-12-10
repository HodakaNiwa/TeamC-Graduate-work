//*****************************************************************************
//
//     �����O�`��̏���[ringRender.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "ringRender.h"
#include "ring.h"
#include "manager.h"
#include "renderer.h"
#include "shader.h"
#include "territory.h"
#include "debuglog.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
//#define RINGRENDER_UPDATE_EVERYTIME

#define RINGRENDER_SHADER_FILANAME "data/ringShader.hlsl"
#define RINGRENDER_PRIORITY (8)
#define RINGRENDER_RING_PRIORITY (3)
#define RINGRENDER_RING_MODELIDX (1)

//*****************************************************************************
//     �ÓI�����o�ϐ��錾
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CRingRender::CRingRender(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pVtxBuff = NULL;
	m_pWorldBuff = NULL;
	m_pColorBuff = NULL;
	m_pIdxBuff = NULL;
	m_pDecl = NULL;
	m_pRingShader = NULL;
	m_pTexture = NULL;
	m_nNumInstance = 0;
	m_bDisp = false;
	m_bLighting = false;
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CRingRender::~CRingRender()
{

}

//=============================================================================
//    ��������
//=============================================================================
CRingRender *CRingRender::Create(void)
{
	CRingRender *pRingRender = NULL;
	if (pRingRender != NULL) { return NULL; }

	// ���������m��
	pRingRender = new CRingRender(RINGRENDER_RING_PRIORITY + 1);
	if (pRingRender == NULL) { return NULL; }

	// �C���X�^���X����ݒ�
	int nNumInstance = 0;

	// �S�Ẵ����O������
	CScene *pScene = GetTop(RINGRENDER_RING_PRIORITY);
	CScene *pSceneNext = NULL;
	while (pScene != NULL)
	{
		pSceneNext = pScene->GetpNext();
		if (pScene->GetObjType() == OBJTYPE_RING)
		{// �����O��������
			nNumInstance++;
		}
		pScene = pSceneNext;
	}

	// �l�̐ݒ�
	pRingRender->SetNumInstance(nNumInstance);

	// ����������
	if (FAILED(pRingRender->Init()))
	{
		pRingRender->Uninit();
		return NULL;
	}

	// ���f����ݒ肷��
	pRingRender->BindModel(CTerritory::GetMesh()[RINGRENDER_RING_MODELIDX], NULL);

	return pRingRender;	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CRingRender::Init(void)
{
	// �C���X�^���X����������Ώ������Ȃ�
	if (m_nNumInstance <= 0) { return E_FAIL; }

	// �`��f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

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

#ifndef RINGRENDER_UPDATE_EVERYTIME
	// ���[���h�}�g���b�N�X�o�b�t�@�����b�N���A���[���h�}�g���b�N�X�f�[�^�ւ̃|�C���^���擾
	CRingRender::WORLDMATRIX *pWorld = NULL;
	m_pWorldBuff->Lock(0, 0, (void**)&pWorld, 0);

	// ���_�J���[�o�b�t�@�����b�N���A���_�J���[�f�[�^�ւ̃|�C���^���擾
	CRingRender::COLORDATA *pColor = NULL;
	m_pColorBuff->Lock(0, 0, (void**)&pColor, 0);

	// �S�Ẵ����O��T���ă��X�g�ɒǉ�����
	CScene *pScene = GetTop(RINGRENDER_RING_PRIORITY);
	CScene *pSceneNext = NULL;
	while (pScene != NULL)
	{
		pSceneNext = pScene->GetpNext();
		if (pScene->GetObjType() == OBJTYPE_RING)
		{// �����O��������
			pColor->col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
			AddList((CRing*)pScene, pWorld, pColor);
			pWorld++;
			pColor++;
		}
		pScene = pSceneNext;
	}
#endif

	return S_OK;
}

//=============================================================================
//    ���[���h�}�g���b�N�X�o�b�t�@�̐�������
//=============================================================================
HRESULT CRingRender::MakeWorldBuff(const LPDIRECT3DDEVICE9 pDevice)
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
HRESULT CRingRender::MakeColorBuff(const LPDIRECT3DDEVICE9 pDevice)
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
//    ���_�錾�f�[�^�̍쐬����
//=============================================================================
HRESULT CRingRender::MakeDeclaration(const LPDIRECT3DDEVICE9 pDevice)
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
//    �V�F�[�_�[��ǂݍ��ޏ���
//=============================================================================
HRESULT CRingRender::LoadShader(void)
{
	if (m_pRingShader != NULL) { return E_FAIL; }
	m_pRingShader = CRingShader::Create(RINGRENDER_SHADER_FILANAME);
	if (m_pRingShader == NULL) { return E_FAIL; }

	return S_OK;
}

//=============================================================================
//    �\�����郂�f����ݒ肷�鏈��
//=============================================================================
void CRingRender::BindModel(const LPD3DXMESH pMesh, const LPDIRECT3DTEXTURE9 pTexture)
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return; }

	// ���_���ƃ|���S�������l��
	m_nNumVertex = pMesh->GetNumVertices();
	m_nNumPolygon = pMesh->GetNumFaces();

	// �C���f�b�N�X�o�b�t�@���擾
	pMesh->GetIndexBuffer(&m_pIdxBuff);

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEXDATA) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1),
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEXDATA *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�o�b�t�@�����b�N
	BYTE *pVtxBuff;  // ���_�o�b�t�@�ւ̃|�C���^
	pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < m_nNumVertex; nCntVtx++)
	{// ���ׂĂ̒��_���𔲂��o��
	    // ���_���W
		pVtx[nCntVtx].pos = *(D3DXVECTOR3*)pVtxBuff;
		pVtxBuff += sizeof(D3DXVECTOR3);

		// �@���x�N�g��
		pVtx[nCntVtx].nor = pVtx[nCntVtx].pos;
		D3DXVec3Normalize(&pVtx[nCntVtx].nor, &-pVtx[nCntVtx].nor);
		pVtxBuff += sizeof(D3DXVECTOR3);

		// �e�N�X�`�����W
		pVtx[nCntVtx].tex = *(D3DXVECTOR2*)pVtxBuff;
		pVtxBuff += sizeof(D3DXVECTOR2);
	}

	// ���_�o�b�t�@���A�����b�N
	pMesh->UnlockVertexBuffer();

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// �e�N�X�`�������蓖�Ă�
	BindTexture(pTexture);
}

//=============================================================================
//    �I������
//=============================================================================
void CRingRender::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	DIRECT_RELEASE(m_pVtxBuff);

	// ���[���h�}�g���b�N�X�o�b�t�@�̊J��
	DIRECT_RELEASE(m_pWorldBuff);

	// ���_�J���[�o�b�t�@�̊J��
	DIRECT_RELEASE(m_pColorBuff);

	// ���_�錾�f�[�^�̊J��
	DIRECT_RELEASE(m_pDecl);

	// �V�F�[�_�[�̊J��
	if (m_pRingShader != NULL)
	{
		m_pRingShader->Uninit();
		delete m_pRingShader;
		m_pRingShader = NULL;
	}

	// ���S�t���O�𗧂Ă�
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CRingRender::Update(void)
{
#ifdef RINGRENDER_UPDATE_EVERYTIME
	// ���[���h�}�g���b�N�X�o�b�t�@�����b�N���A���[���h�}�g���b�N�X�f�[�^�ւ̃|�C���^���擾
	CRingRender::WORLDMATRIX *pWorld = NULL;
	m_pWorldBuff->Lock(0, 0, (void**)&pWorld, 0);

	// ���_�J���[�o�b�t�@�����b�N���A���_�J���[�f�[�^�ւ̃|�C���^���擾
	CRingRender::COLORDATA *pColor = NULL;
	m_pColorBuff->Lock(0, 0, (void**)&pColor, 0);

	// �S�Ẵ����O��T���ă��X�g�ɒǉ�����
	CScene *pScene = GetTop(RINGRENDER_RING_PRIORITY);
	CScene *pSceneNext = NULL;
	while (pScene != NULL)
	{
		pSceneNext = pScene->GetpNext();
		if (pScene->GetObjType() == OBJTYPE_RING)
		{// �����O��������
			AddList((CRing*)pScene, pWorld, pColor);
			pWorld++;
			pColor++;
		}
		pScene = pSceneNext;
	}
#endif
}

//=============================================================================
//    �o�b�t�@�ɏ������ޏ�������
//=============================================================================
void CRingRender::AddList(CRing *pRing, WORLDMATRIX *pWorld, COLORDATA *pCol)
{
	// ���[���h�}�g���b�N�X������������
	AddWorld(pRing, pWorld);

	// ���_�J���[������������
	AddColor(pRing, pCol);
}

//=============================================================================
//    ���[���h�}�g���b�N�X�����������ޏ���
//=============================================================================
void CRingRender::AddWorld(CRing *pRing, WORLDMATRIX *pWorld)
{
	D3DXVECTOR3 pos = INITIALIZE_VECTOR3;
	D3DXVECTOR3 rot = INITIALIZE_VECTOR3;
	D3DXVECTOR3 Scale = INITIALIZE_VECTOR3;
	pos = pRing->GetPos();
	rot = pRing->GetRot();
	Scale = pRing->GetScale();
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
void CRingRender::AddColor(CRing *pRing, COLORDATA *pCol)
{
	//pCol[0].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	pCol[0].col = pCol[0].col; //��
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CRingRender::Draw(void)
{
	if (m_pRingShader == NULL || m_bDisp == false) { return; }

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
	m_pRingShader->SetMtxProjection(mtxProj);
	m_pRingShader->SetMtxView(mtxView);

	// �e�N�X�`���̐ݒ�
	m_pRingShader->BindTexture(m_pTexture);

	// ���C�g�̐ݒ�
	D3DLIGHT9 Light;
	for (int nCntLight = 0; nCntLight < MAXLIGHT; nCntLight++)
	{
		pDevice->GetLight(nCntLight, &Light);
		m_pRingShader->SetLightDir(Light.Direction, nCntLight);
		m_pRingShader->SetLightDiffuse(Light.Diffuse, nCntLight);
	}

	// �@���𐳋K������(�傫���ɂ���Ė@�����ς��̂�)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	// �V�F�[�_�[�ɒl���Z�b�g
	m_pRingShader->SetParamToEffect();

	// �`��J�n
	m_pRingShader->Begin();

	// �����_�����O�p�X�J�n
	int nPassIdx = 0;
	if (m_pTexture == NULL) { nPassIdx = 1; }
	m_pRingShader->BeginPass(nPassIdx);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_nNumVertex, 0, m_nNumPolygon);

	// �����_�����O�p�X�I��
	m_pRingShader->EndPass();

	// �`��I��
	m_pRingShader->End();

	// ��n��
	pDevice->SetVertexDeclaration(NULL);
	pDevice->SetStreamSourceFreq(0, 1);
	pDevice->SetStreamSourceFreq(1, 1);
	pDevice->SetStreamSourceFreq(2, 1);
	pDevice->SetStreamSource(1, NULL, 0, 0);
	pDevice->SetStreamSource(2, NULL, 0, 0);

	// �@���𐳋K�����Ȃ�
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, false);
}