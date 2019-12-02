//*****************************************************************************
//
//     �C���X�^���V���O�𗘗p�����ϋq�̏���[audience.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "audience.h"
#include "manager.h"
#include "library.h"
#include "shader.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define AUDIENCE_DRAW_UNDER												// �̂̉��̖ʂ�`�悷�邩���Ȃ���
#define AUDIENCE_FILENAME         "data/TEXT/audienceData.txt"			// �ϋq�̃X�N���v�g�t�@�C����
#define AUDIENCE_SHADER_FILENAME  "data/audience.hlsl"					// �ϋq�̃V�F�[�_�[�t�@�C����
#define AUDIENCE_PARTS_FILENAME   "data/MODEL/audience_parts.x"			// �ϋq�̃p�[�c���f���t�@�C����
#define AUDIECNE_SCALE            (D3DXVECTOR3(0.4f,0.4f,0.4f))			// �ϋq�̑傫��
#define AUDIECNE_COLOR_RIVISION   (1.0f)								// �F�̔{��

#define AUDIENCE_BODY_OFFSET      (D3DXVECTOR3(0.0f,0.0f,0.0f))			// �̂̃I�t�Z�b�g
#define AUDIENCE_BODY_HEIGHT      (40.0f)								// �̂̍���
#define AUDIENCE_BODY_SIZE        (12.0f)								// �̂̃T�C�Y

#define AUDIENCE_HEAD_OFFSET      (D3DXVECTOR3(0.0f,50.0f,0.0f))		// ���̃I�t�Z�b�g
#define AUDIENCE_HEAD_SCALE       (D3DXVECTOR3(0.45f,0.45f,0.45f))		// ���̑傫��
#define AUDIENCE_HEAD_COLOR       (D3DXCOLOR(0.98f, 0.6f, 0.3f, 1.0f))	// ���̐F

#define AUDIENCE_LEFTHAND_OFFSET  (D3DXVECTOR3(-15.0f,26.0f,0.0f))		// ����̃I�t�Z�b�g
#define AUDIENCE_LEFTHAND_SCALE   (D3DXVECTOR3(0.20f,0.20f,0.20f))		// ����̑傫��
#define AUDIENCE_LEFTHAND_COLOR   (D3DXCOLOR(0.98f, 0.6f, 0.3f, 1.0f))	// ����̐F

#define AUDIENCE_RIGHTHAND_OFFSET (D3DXVECTOR3(15.0f,26.0f,0.0f))		// �E��̃I�t�Z�b�g
#define AUDIENCE_RIGHTHAND_SCALE  (D3DXVECTOR3(0.20f,0.20f,0.20f))		// �E��̑傫��
#define AUDIENCE_RIGHTHAND_COLOR  (D3DXCOLOR(0.98f, 0.6f, 0.3f, 1.0f))	// �E��̐F

// �t�@�C���ǂݍ��ݗp�̃p�X
#define AUDIENCESET "AUDIENCESET"
#define END_AUDIENCESET "END_AUDIENCESET"
#define POS "POS = "
#define ROT "ROT = "
#define SCALE "SCALE = "
#define COL "COL = "

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CAudience::CAudience(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pWorldBuff = NULL;
	m_pDecl = NULL;
	m_pInsShader = NULL;
	m_nNumInstance = 0;
	for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
	{
		m_pVtxBuff[nCnt] = NULL;
		m_pColorBuff[nCnt] = NULL;
		m_pIdxBuff[nCnt] = NULL;
		m_nNumVertex[nCnt] = 0;
		m_nNumIndex[nCnt] = 0;
		m_nNumPolygon[nCnt] = 0;
		D3DXMatrixIdentity(&m_MtxLocal[nCnt]);
	}
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CAudience::~CAudience()
{

}

//=============================================================================
//    ��������
//=============================================================================
CAudience *CAudience::Create(void)
{
	// ���������m��
	CAudience *pAudience = NULL;
	pAudience = new CAudience;
	if (pAudience == NULL) { return NULL; }

	// ����������
	if (FAILED(pAudience->Init()))
	{
		pAudience->Uninit();
		return NULL;
	}

	return pAudience;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CAudience::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

	// ���_��,�C���f�b�N�X��,�|���S�����̐ݒ�
	// ��
#ifdef AUDIENCE_DRAW_UNDER
	m_nNumVertex[PARTS_BODY] = 9;
	m_nNumIndex[PARTS_BODY] = 30;
	m_nNumPolygon[PARTS_BODY] = 10;
#else
	m_nNumVertex[PARTS_BODY] = 9;
	m_nNumIndex[PARTS_BODY] = 21;
	m_nNumPolygon[PARTS_BODY] = 7;
#endif

	// �C���X�^���V���O����ǂݍ���
	char aStr[256];
	LoadInstanceDataNum(aStr);
	if (m_nNumInstance == 0)
	{
		return E_FAIL;
	}

	// �C���X�^���V���O���ɂ���ă��������m��
	D3DXVECTOR3 *pPos;
	pPos = new D3DXVECTOR3[m_nNumInstance];

	D3DXVECTOR3 *pRot;
	pRot = new D3DXVECTOR3[m_nNumInstance];

	D3DXVECTOR3 *pScale;
	pScale = new D3DXVECTOR3[m_nNumInstance];

	D3DXCOLOR *pCol;
	pCol = new D3DXCOLOR[m_nNumInstance];
	for (int nCnt = 0; nCnt < m_nNumInstance; nCnt++)
	{
		pPos[nCnt] = INITIALIZE_VECTOR3;
		pScale[nCnt] = INITIALIZE_VECTOR3;
		pRot[nCnt] = INITIALIZE_VECTOR3;
		pCol[nCnt] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// �l�̓ǂݍ���
	LoadInstanceData(aStr, pPos, pRot, pScale, pCol);

	// ���[���h�}�g���b�N�X�o�b�t�@�̍쐬
	MakeWorldBuff(pDevice, pPos, pRot, pScale);

	// ���_�錾�f�[�^�̍쐬
	MakeDeclaration(pDevice);

	// �V�F�[�_�[�t�@�C����ǂݍ��ޏ���
	LoadShader();

	// �̂��쐬���鏈��
	MakeBody(pDevice, pCol);

	// �����쐬���鏈��
	MakeHead(pDevice);

	// ������쐬���鏈��
	MakeLeftHand(pDevice);

	// �E����쐬���鏈��
	MakeRightHand(pDevice);

	// �������̊J��
	if (pPos != NULL)
	{
		delete[] pPos;
		pPos = NULL;
	}
	if (pRot != NULL)
	{
		delete[] pRot;
		pRot = NULL;
	}
	if (pScale != NULL)
	{
		delete[] pScale;
		pScale = NULL;
	}
	if (pCol != NULL)
	{
		delete[] pCol;
		pCol = NULL;
	}

	return S_OK;
}


//=============================================================================
//    �C���X�^���V���O�����t�@�C������ǂݍ��ޏ���
//=============================================================================
void CAudience::LoadInstanceDataNum(char *pStr)
{
	// �t�@�C���I�[�v��
	CFileLoader *pFileLoader = CFileLoader::Create(AUDIENCE_FILENAME);
	if (pFileLoader == NULL) { return; }

	// �X�N���v�g�ǂݍ��݊J�n
	pFileLoader->GetString(pStr);
	if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0)
	{
		while (1)
		{
			pFileLoader->GetString(pStr);
			if (CFunctionLib::Memcmp(pStr, AUDIENCESET) == 0)
			{
				m_nNumInstance++;
			}
			else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
			{
				break;
			}
		}
	}


	// �������̊J��
	if (pFileLoader != NULL)
	{
		pFileLoader->Uninit();
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
//    �C���X�^���V���O�f�[�^���t�@�C������ǂݍ��ޏ���
//=============================================================================
void CAudience::LoadInstanceData(char *pStr, D3DXVECTOR3 *pPos, D3DXVECTOR3 *pRot, D3DXVECTOR3 *pScale, D3DXCOLOR *pCol)
{
	// �t�@�C���I�[�v��
	CFileLoader *pFileLoader = CFileLoader::Create(AUDIENCE_FILENAME);
	if (pFileLoader == NULL) { return; }

	// �X�N���v�g�ǂݍ��݊J�n
	int nCntData = 0;
	pFileLoader->GetString(pStr);
	if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0)
	{
		while (1)
		{
			pFileLoader->GetString(pStr);
			if (CFunctionLib::Memcmp(pStr, AUDIENCESET) == 0)
			{
				pRot[nCntData] = INITIALIZE_VECTOR3;
				pScale[nCntData] = AUDIECNE_SCALE;
				while (1)
				{
					pFileLoader->GetString(pStr);
					if (CFunctionLib::Memcmp(pStr, POS) == 0)
					{
						pPos[nCntData] = CFunctionLib::ReadVector3(pStr, POS);
					}
					else if (CFunctionLib::Memcmp(pStr, ROT) == 0)
					{
						pRot[nCntData] = CFunctionLib::ReadVector3(pStr, ROT);
					}
					else if (CFunctionLib::Memcmp(pStr, SCALE) == 0)
					{
						pScale[nCntData] = CFunctionLib::ReadVector3(pStr, SCALE);
					}
					else if (CFunctionLib::Memcmp(pStr, COL) == 0)
					{
						pCol[nCntData] = CFunctionLib::ReadVector4(pStr, COL);
					}
					else if (CFunctionLib::Memcmp(pStr, END_AUDIENCESET) == 0)
					{
						nCntData++;
						break;
					}
				}
			}
			else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
			{
				break;
			}
		}
	}

	// �������̊J��
	if (pFileLoader != NULL)
	{
		pFileLoader->Uninit();
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
//    ���[���h�}�g���b�N�X�o�b�t�@���쐬���鏈��
//=============================================================================
void CAudience::MakeWorldBuff(const LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 *pPos, D3DXVECTOR3 *pRot, D3DXVECTOR3 *pScale)
{
	// ���[���h�}�g���b�N�X�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(WORLDMATRIX) * m_nNumInstance,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&m_pWorldBuff,
		NULL);

	// ���[���h�}�g���b�N�X�o�b�t�@�����b�N��,�f�[�^�ւ̃|�C���^���擾
	WORLDMATRIX *pWorld;
	m_pWorldBuff->Lock(0, 0, (void**)&pWorld, 0);

	// ���[���h�}�g���b�N�X�̐ݒ�
	float fSinPitch;
	float fCosPitch;
	float fSinYaw;
	float fCosYaw;
	float fSinRoll;
	float fCosRoll;
	D3DXMATRIX mtxWorld;
	D3DXMatrixIdentity(&mtxWorld);
	for (int nCnt = 0; nCnt < m_nNumInstance; nCnt++, pPos++, pRot++, pScale++)
	{
		// ��]�s����쐬
		fSinPitch = sinf(pRot->x);
		fCosPitch = cosf(pRot->x);
		fSinYaw = sinf(pRot->y);
		fCosYaw = cosf(pRot->y);
		fSinRoll = sinf(pRot->z);
		fCosRoll = cosf(pRot->z);
		mtxWorld._11 = fSinRoll * fSinPitch * fSinYaw + fCosRoll * fCosYaw;
		mtxWorld._12 = fSinRoll * fCosPitch;
		mtxWorld._13 = fSinRoll * fSinPitch * fCosYaw - fCosRoll * fSinYaw;
		mtxWorld._21 = fCosRoll * fSinPitch * fSinYaw - fSinRoll * fCosYaw;
		mtxWorld._22 = fCosRoll * fCosPitch;
		mtxWorld._23 = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fSinYaw;
		mtxWorld._31 = fCosPitch * fSinYaw;
		mtxWorld._32 = -fSinPitch;
		mtxWorld._33 = fCosPitch * fCosYaw;

		// �s���ݒ�
		pWorld[nCnt].m1[0] = mtxWorld._11 * pScale->x;
		pWorld[nCnt].m1[1] = mtxWorld._12 * pScale->x;
		pWorld[nCnt].m1[2] = mtxWorld._13 * pScale->x;
		pWorld[nCnt].m1[3] = mtxWorld._14;
		pWorld[nCnt].m2[0] = mtxWorld._21 * pScale->y;
		pWorld[nCnt].m2[1] = mtxWorld._22 * pScale->y;
		pWorld[nCnt].m2[2] = mtxWorld._23 * pScale->y;
		pWorld[nCnt].m2[3] = mtxWorld._24;
		pWorld[nCnt].m3[0] = mtxWorld._31 * pScale->z;
		pWorld[nCnt].m3[1] = mtxWorld._32 * pScale->z;
		pWorld[nCnt].m3[2] = mtxWorld._33 * pScale->z;
		pWorld[nCnt].m3[3] = mtxWorld._34;
		pWorld[nCnt].m4[0] = pPos->x;
		pWorld[nCnt].m4[1] = pPos->y;
		pWorld[nCnt].m4[2] = pPos->z;
		pWorld[nCnt].m4[3] = mtxWorld._44;
	}

	// ���[���h�}�g���b�N�X�o�b�t�@���A�����b�N����
	m_pWorldBuff->Unlock();
}

//=============================================================================
//    ���_�錾�f�[�^���쐬���鏈��
//=============================================================================
void CAudience::MakeDeclaration(const LPDIRECT3DDEVICE9 pDevice)
{
	// �K�v�ȃf�[�^�𗅗�
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

		// Stream2(�̂̐F)
		{ 2,  0, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },	// ���_�J���[
		D3DDECL_END()
	};

	// ���_�錾�̍쐬
	pDevice->CreateVertexDeclaration(declElems, &m_pDecl);
}

//=============================================================================
//    �V�F�[�_�[��ǂݍ��ޏ���
//=============================================================================
void CAudience::LoadShader(void)
{
	if (m_pInsShader != NULL) { return; }
	m_pInsShader = CInstancingShader::Create(AUDIENCE_SHADER_FILENAME);
}

//=============================================================================
//    �I������
//=============================================================================
void CAudience::Uninit(void)
{
	for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
	{
		// ���_�o�b�t�@�̊J��
		DIRECT_RELEASE(m_pVtxBuff[nCnt]);

		// ���_�J���[�o�b�t�@���J��
		DIRECT_RELEASE(m_pColorBuff[nCnt]);

		// �C���f�b�N�X�o�b�t�@���J��
		DIRECT_RELEASE(m_pIdxBuff[nCnt]);
	}

	// ���[���h�}�g���b�N�X�o�b�t�@���J��
	DIRECT_RELEASE(m_pWorldBuff);

	// ���_�錾�f�[�^���J��
	DIRECT_RELEASE(m_pDecl);

	// �V�F�[�_�[�̊J��
	if (m_pInsShader != NULL)
	{
		m_pInsShader->Uninit();
		delete m_pInsShader;
		m_pInsShader = NULL;
	}

	// ���S�t���O�𗧂Ă�
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CAudience::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CAudience::Draw(void)
{
	if (m_pInsShader == NULL) { return; }

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return; }

	// �C���X�^���V���O�錾
	pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | m_nNumInstance);
	pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);
	pDevice->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	// ���_�錾�f�[�^��ݒ�
	pDevice->SetVertexDeclaration(m_pDecl);

	// ���[���h�}�g���b�N�X�o�b�t�@���X�g���[����1�Ԃɐݒ�
	pDevice->SetStreamSource(1, m_pWorldBuff, 0, sizeof(WORLDMATRIX));

	// �v���W�F�N�V�����}�g���b�N�X�ƃr���[�}�g���b�N�X�̐ݒ�
	D3DXMATRIX mtxView, mtxProj;
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	m_pInsShader->SetMtxProjection(mtxProj);
	m_pInsShader->SetMtxView(mtxView);

	// �e�N�X�`���̐ݒ�
	m_pInsShader->BindTexture(NULL);

	// ���C�g�̐ݒ�
	D3DLIGHT9 Light;
	pDevice->GetLight(0, &Light);
	m_pInsShader->SetLightDir(Light.Direction);
	m_pInsShader->SetLightDiffuse(Light.Diffuse);
	m_pInsShader->SetLightAmbient(Light.Ambient);

	// �V�F�[�_�[�ɒl���Z�b�g
	m_pInsShader->SetParamToEffect();

	// �J�����O���O��
	DWORD CullMode;
	pDevice->GetRenderState(D3DRS_CULLMODE, &CullMode);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// �`��J�n
	m_pInsShader->Begin();

	// �����_�����O�p�X�̔ԍ���ݒ�
	int nPassIdx[PARTS_MAX] = {1,2,2,2};

	for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
	{
		// ���[�J�����W�ɂ����郏�[���h�}�g���b�N�X��ݒ�
		m_pInsShader->SetMtxLocal(m_MtxLocal[nCnt]);

		// ���_�o�b�t�@���X�g���[����0�Ԃɐݒ�
		pDevice->SetStreamSource(0, m_pVtxBuff[nCnt], 0, sizeof(VERTEXDATA));

		// ���_�J���[�o�b�t�@���X�g���[����2�Ԃɐݒ�
		pDevice->SetStreamSource(2, m_pColorBuff[nCnt], 0, sizeof(COLORDATA));

		// �C���f�b�N�X�o�b�t�@��ݒ�
		pDevice->SetIndices(m_pIdxBuff[nCnt]);

		// �V�F�[�_�[�ɒl���Z�b�g
		m_pInsShader->SetParamToEffect();

		// �����_�����O�p�X�J�n
		m_pInsShader->BeginPass(nPassIdx[nCnt]);

		// �|���S���̕`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_nNumIndex[nCnt], 0, m_nNumPolygon[nCnt]);

		// �����_�����O�p�X�I��
		m_pInsShader->EndPass();
	}

	// �`��I��
	m_pInsShader->End();

	// ��n��
	pDevice->SetVertexDeclaration(NULL);
	pDevice->SetStreamSourceFreq(0, 1);
	pDevice->SetStreamSourceFreq(1, 1);
	pDevice->SetStreamSourceFreq(2, 1);
	pDevice->SetRenderState(D3DRS_CULLMODE, CullMode);
	pDevice->SetStreamSource(1, NULL, 0, 0);
	pDevice->SetStreamSource(2, NULL, 0, 0);
}


//*****************************************************************************
//     �̊֘A
//*****************************************************************************
//=============================================================================
//    �̂��쐬���鏈��
//=============================================================================
void CAudience::MakeBody(const LPDIRECT3DDEVICE9 pDevice, D3DXCOLOR *pCol)
{
	// ���_�o�b�t�@�̍쐬
	MakeVertexBody(pDevice);

	// ���_�J���[�o�b�t�@�̍쐬
	MakeColorBuffBody(pDevice, pCol);

	// �C���f�b�N�X�o�b�t�@�̍쐬
	MakeIndexBody(pDevice);

	// ���[�J�����W�ɂ����郏�[���h�}�g���b�N�X���쐬����
	MakeMtxLocalBody();
}

//=============================================================================
//    ���_�o�b�t�@���쐬���鏈��
//=============================================================================
void CAudience::MakeVertexBody(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEXDATA) * 9,
		D3DUSAGE_WRITEONLY,
		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1),
		D3DPOOL_MANAGED,
		&m_pVtxBuff[PARTS_BODY],
		NULL);

	// ���_���̐ݒ�
	VERTEXDATA *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff[PARTS_BODY]->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(0.0f, AUDIENCE_BODY_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(-AUDIENCE_BODY_SIZE, 0.0f, AUDIENCE_BODY_SIZE);
	pVtx[2].pos = D3DXVECTOR3(AUDIENCE_BODY_SIZE, 0.0f, AUDIENCE_BODY_SIZE);
	pVtx[3].pos = D3DXVECTOR3(-AUDIENCE_BODY_SIZE, 0.0f, -AUDIENCE_BODY_SIZE);
	pVtx[4].pos = D3DXVECTOR3(AUDIENCE_BODY_SIZE, 0.0f, -AUDIENCE_BODY_SIZE);
	pVtx[5].pos = D3DXVECTOR3(-AUDIENCE_BODY_SIZE, 0.0f, AUDIENCE_BODY_SIZE);
	pVtx[6].pos = D3DXVECTOR3(AUDIENCE_BODY_SIZE, 0.0f, AUDIENCE_BODY_SIZE);
	pVtx[7].pos = D3DXVECTOR3(-AUDIENCE_BODY_SIZE, 0.0f, -AUDIENCE_BODY_SIZE);
	pVtx[8].pos = D3DXVECTOR3(AUDIENCE_BODY_SIZE, 0.0f, -AUDIENCE_BODY_SIZE);

	// �@���v�Z�p�x�N�g��
	D3DXVECTOR3 vecVer[12];

	// �O�ϗp�Ƀx�N�g�������
	// ���̖�
	vecVer[0] = pVtx[1].pos - pVtx[0].pos;
	vecVer[1] = pVtx[2].pos - pVtx[0].pos;

	// �E�̖�
	vecVer[2] = pVtx[2].pos - pVtx[0].pos;
	vecVer[3] = pVtx[4].pos - pVtx[0].pos;

	// ��O�̖�
	vecVer[4] = pVtx[4].pos - pVtx[0].pos;
	vecVer[5] = pVtx[3].pos - pVtx[0].pos;

	// ���̖�
	vecVer[6] = pVtx[3].pos - pVtx[0].pos;
	vecVer[7] = pVtx[1].pos - pVtx[0].pos;

	// ���̖�(����)
	vecVer[8] = pVtx[6].pos - pVtx[5].pos;
	vecVer[9] = pVtx[7].pos - pVtx[5].pos;

	// ���̖�(�E��)
	vecVer[10] = pVtx[7].pos - pVtx[8].pos;
	vecVer[11] = pVtx[6].pos - pVtx[8].pos;

	// ���_�̖@��
	D3DXVECTOR3 nor1, nor2, nor3, nor4, nor5, nor6;
	D3DXVec3Cross(&nor1, &vecVer[1], &vecVer[0]);
	D3DXVec3Cross(&nor2, &vecVer[3], &vecVer[2]);
	D3DXVec3Cross(&nor3, &vecVer[5], &vecVer[4]);
	D3DXVec3Cross(&nor4, &vecVer[7], &vecVer[6]);
	D3DXVec3Cross(&nor5, &vecVer[8], &vecVer[9]);
	D3DXVec3Cross(&nor6, &vecVer[10], &vecVer[11]);
	D3DXVec3Normalize(&nor1, &nor1);
	D3DXVec3Normalize(&nor2, &nor2);
	D3DXVec3Normalize(&nor3, &nor3);
	D3DXVec3Normalize(&nor4, &nor4);
	D3DXVec3Normalize(&nor5, &nor5);
	D3DXVec3Normalize(&nor6, &nor6);

	// �@���x�N�g��
	pVtx[0].nor = (nor1 + nor2 + nor3 + nor4) / 4;
	pVtx[1].nor = (nor1 + nor4) / 2;
	pVtx[2].nor = (nor1 + nor2) / 2;
	pVtx[3].nor = (nor3 + nor4) / 2;
	pVtx[4].nor = (nor2 + nor3) / 2;
	pVtx[5].nor = nor5;
	pVtx[6].nor = (nor5 + nor6) / 2;
	pVtx[7].nor = (nor5 + nor6) / 2;
	pVtx[8].nor = nor6;

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[7].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff[PARTS_BODY]->Unlock();
}

//=============================================================================
//    ���_�J���[�o�b�t�@���쐬���鏈��
//=============================================================================
void CAudience::MakeColorBuffBody(const LPDIRECT3DDEVICE9 pDevice, D3DXCOLOR *pCol)
{
	// ���_�J���[�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(COLORDATA) * m_nNumInstance,
		D3DUSAGE_WRITEONLY,
		(D3DFVF_DIFFUSE),
		D3DPOOL_MANAGED,
		&m_pColorBuff[PARTS_BODY],
		NULL);

	// ���_�J���[���̐ݒ�
	COLORDATA *pColor;

	// ���_�J���[�o�b�t�@�����b�N��,���_�J���[�f�[�^�ւ̃|�C���^���擾
	m_pColorBuff[PARTS_BODY]->Lock(0, 0, (void**)&pColor, 0);

	// ���_�J���[�̐ݒ�
	for (int nCnt = 0; nCnt < m_nNumInstance; nCnt++, pCol++)
	{
		pColor[nCnt].col = D3DXCOLOR(pCol->r, pCol->g, pCol->b, pCol->a);
	}

	// ���_�J���[�o�b�t�@���A�����b�N����
	m_pColorBuff[PARTS_BODY]->Unlock();
}

//=============================================================================
//    �C���f�b�N�X�o�b�t�@���쐬���鏈��
//=============================================================================
void CAudience::MakeIndexBody(const LPDIRECT3DDEVICE9 pDevice)
{
	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIndex[PARTS_BODY],
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff[PARTS_BODY],
		NULL);

	WORD *pIdx;       // �C���f�b�N�X�f�[�^�ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N��,�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff[PARTS_BODY]->Lock(0, 0, (void**)&pIdx, 0);

	// ���̖�
	pIdx[0] = 0;
	pIdx[1] = 1;
	pIdx[2] = 2;

	// �k��
	pIdx[3] = 2;
	pIdx[4] = 2;
	pIdx[5] = 0;

	// �E�̖�
	pIdx[6] = 0;
	pIdx[7] = 2;
	pIdx[8] = 4;

	// �k��
	pIdx[9] = 4;
	pIdx[10] = 4;
	pIdx[11] = 0;

	// ��O�̖�
	pIdx[12] = 0;
	pIdx[13] = 4;
	pIdx[14] = 3;

	// �k��
	pIdx[15] = 3;
	pIdx[16] = 3;
	pIdx[17] = 0;

	// ���̖�
	pIdx[18] = 0;
	pIdx[19] = 3;
	pIdx[20] = 1;

#ifdef AUDIENCE_DRAW_UNDER
	// �k��
	pIdx[21] = 5;
	pIdx[22] = 5;
	pIdx[23] = 5;

	// ���̖�(����)
	pIdx[24] = 5;
	pIdx[25] = 6;
	pIdx[26] = 7;

	// ���̖�(�E��)
	pIdx[27] = 7;
	pIdx[28] = 6;
	pIdx[29] = 8;
#endif

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff[PARTS_BODY]->Unlock();
}

//=============================================================================
//    ���[�J�����W�ɂ����郏�[���h�}�g���b�N�X���쐬���鏈��
//=============================================================================
void CAudience::MakeMtxLocalBody(void)
{
	// �I�t�Z�b�g�ʒu�𔽉f
	m_MtxLocal[PARTS_BODY]._41 = AUDIENCE_BODY_OFFSET.x;
	m_MtxLocal[PARTS_BODY]._42 = AUDIENCE_BODY_OFFSET.y;
	m_MtxLocal[PARTS_BODY]._43 = AUDIENCE_BODY_OFFSET.z;
}


//*****************************************************************************
//     ���֘A
//*****************************************************************************
//=============================================================================
//    �����쐬���鏈��
//=============================================================================
void CAudience::MakeHead(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̍쐬
	MakeVertexHead(pDevice);

	// ���_�J���[�o�b�t�@�̍쐬
	MakeColorBuffHead(pDevice);

	// ���[�J�����W�ɂ����郏�[���h�}�g���b�N�X���쐬����
	MakeMtxLocalHead();
}

//=============================================================================
//    ���_�o�b�t�@���쐬���鏈��
//=============================================================================
void CAudience::MakeVertexHead(const LPDIRECT3DDEVICE9 pDevice)
{
	// �����f����ǂݍ���
	LPD3DXMESH pMesh = NULL;
	LPD3DXBUFFER pBuffMat = NULL;
	DWORD nNumMat;
	D3DXLoadMeshFromX(AUDIENCE_PARTS_FILENAME, D3DXMESH_SYSTEMMEM, pDevice, NULL, &pBuffMat, NULL, &nNumMat, &pMesh);
	if (pMesh == NULL || pBuffMat == NULL) { return; }

	// ���_���ƃ|���S�������l��
	m_nNumVertex[PARTS_HEAD] = pMesh->GetNumVertices();
	m_nNumPolygon[PARTS_HEAD] = pMesh->GetNumFaces();

	// �C���f�b�N�X�o�b�t�@���擾
	pMesh->GetIndexBuffer(&m_pIdxBuff[PARTS_HEAD]);

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEXDATA) * m_nNumVertex[PARTS_HEAD],
		D3DUSAGE_WRITEONLY,
		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1),
		D3DPOOL_MANAGED,
		&m_pVtxBuff[PARTS_HEAD],
		NULL);

	// ���_���̐ݒ�
	VERTEXDATA *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff[PARTS_HEAD]->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�o�b�t�@�����b�N
	BYTE *pVtxBuff;  // ���_�o�b�t�@�ւ̃|�C���^
	pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < m_nNumVertex[PARTS_HEAD]; nCntVtx++)
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
	m_pVtxBuff[PARTS_HEAD]->Unlock();

	// �f�[�^���J��
	pMesh->Release();
	pBuffMat->Release();
}

//=============================================================================
//    ���_�J���[�o�b�t�@���쐬���鏈��
//=============================================================================
void CAudience::MakeColorBuffHead(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�J���[�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(COLORDATA) * m_nNumInstance,
		D3DUSAGE_WRITEONLY,
		(D3DFVF_DIFFUSE),
		D3DPOOL_MANAGED,
		&m_pColorBuff[PARTS_HEAD],
		NULL);

	// ���_�J���[���̐ݒ�
	COLORDATA *pColor;

	// ���_�J���[�o�b�t�@�����b�N��,���_�J���[�f�[�^�ւ̃|�C���^���擾
	m_pColorBuff[PARTS_HEAD]->Lock(0, 0, (void**)&pColor, 0);

	// ���_�J���[�̐ݒ�
	for (int nCnt = 0; nCnt < m_nNumInstance; nCnt++)
	{
		pColor[nCnt].col = AUDIENCE_HEAD_COLOR / AUDIECNE_COLOR_RIVISION;
	}

	// ���_�J���[�o�b�t�@���A�����b�N����
	m_pColorBuff[PARTS_HEAD]->Unlock();
}

//=============================================================================
//    ���[�J�����W�ɂ����郏�[���h�}�g���b�N�X���쐬���鏈��
//=============================================================================
void CAudience::MakeMtxLocalHead(void)
{
	// �傫���𔽉f
	m_MtxLocal[PARTS_HEAD]._11 = m_MtxLocal[PARTS_HEAD]._11 * AUDIENCE_HEAD_SCALE.x;
	m_MtxLocal[PARTS_HEAD]._12 = m_MtxLocal[PARTS_HEAD]._12 * AUDIENCE_HEAD_SCALE.x;
	m_MtxLocal[PARTS_HEAD]._13 = m_MtxLocal[PARTS_HEAD]._13 * AUDIENCE_HEAD_SCALE.x;
	m_MtxLocal[PARTS_HEAD]._21 = m_MtxLocal[PARTS_HEAD]._21 * AUDIENCE_HEAD_SCALE.y;
	m_MtxLocal[PARTS_HEAD]._22 = m_MtxLocal[PARTS_HEAD]._22 * AUDIENCE_HEAD_SCALE.y;
	m_MtxLocal[PARTS_HEAD]._23 = m_MtxLocal[PARTS_HEAD]._23 * AUDIENCE_HEAD_SCALE.y;
	m_MtxLocal[PARTS_HEAD]._31 = m_MtxLocal[PARTS_HEAD]._31 * AUDIENCE_HEAD_SCALE.z;
	m_MtxLocal[PARTS_HEAD]._32 = m_MtxLocal[PARTS_HEAD]._32 * AUDIENCE_HEAD_SCALE.z;
	m_MtxLocal[PARTS_HEAD]._33 = m_MtxLocal[PARTS_HEAD]._33 * AUDIENCE_HEAD_SCALE.z;

	// �I�t�Z�b�g�ʒu�𔽉f
	m_MtxLocal[PARTS_HEAD]._41 = AUDIENCE_HEAD_OFFSET.x;
	m_MtxLocal[PARTS_HEAD]._42 = AUDIENCE_HEAD_OFFSET.y;
	m_MtxLocal[PARTS_HEAD]._43 = AUDIENCE_HEAD_OFFSET.z;
}


//*****************************************************************************
//     ���֘A
//*****************************************************************************
//=============================================================================
//    �����쐬���鏈��
//=============================================================================
void CAudience::MakeLeftHand(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̍쐬
	MakeVertexLeftHand(pDevice);

	// ���_�J���[�o�b�t�@�̍쐬
	MakeColorBuffLeftHand(pDevice);

	// ���[�J�����W�ɂ����郏�[���h�}�g���b�N�X���쐬����
	MakeMtxLocalLeftHand();
}

//=============================================================================
//    ���_�o�b�t�@���쐬���鏈��
//=============================================================================
void CAudience::MakeVertexLeftHand(const LPDIRECT3DDEVICE9 pDevice)
{
	// �����f����ǂݍ���
	LPD3DXMESH pMesh = NULL;
	LPD3DXBUFFER pBuffMat = NULL;
	DWORD nNumMat;
	D3DXLoadMeshFromX(AUDIENCE_PARTS_FILENAME, D3DXMESH_SYSTEMMEM, pDevice, NULL, &pBuffMat, NULL, &nNumMat, &pMesh);
	if (pMesh == NULL || pBuffMat == NULL) { return; }

	// ���_���ƃ|���S�������l��
	m_nNumVertex[PARTS_LEFT_HAND] = pMesh->GetNumVertices();
	m_nNumPolygon[PARTS_LEFT_HAND] = pMesh->GetNumFaces();

	// �C���f�b�N�X�o�b�t�@���擾
	pMesh->GetIndexBuffer(&m_pIdxBuff[PARTS_LEFT_HAND]);

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEXDATA) * m_nNumVertex[PARTS_LEFT_HAND],
		D3DUSAGE_WRITEONLY,
		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1),
		D3DPOOL_MANAGED,
		&m_pVtxBuff[PARTS_LEFT_HAND],
		NULL);

	// ���_���̐ݒ�
	VERTEXDATA *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff[PARTS_LEFT_HAND]->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�o�b�t�@�����b�N
	BYTE *pVtxBuff;  // ���_�o�b�t�@�ւ̃|�C���^
	pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < m_nNumVertex[PARTS_LEFT_HAND]; nCntVtx++)
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
	m_pVtxBuff[PARTS_LEFT_HAND]->Unlock();

	// �f�[�^���J��
	pMesh->Release();
	pBuffMat->Release();
}

//=============================================================================
//    ���_�J���[�o�b�t�@���쐬���鏈��
//=============================================================================
void CAudience::MakeColorBuffLeftHand(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�J���[�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(COLORDATA) * m_nNumInstance,
		D3DUSAGE_WRITEONLY,
		(D3DFVF_DIFFUSE),
		D3DPOOL_MANAGED,
		&m_pColorBuff[PARTS_LEFT_HAND],
		NULL);

	// ���_�J���[���̐ݒ�
	COLORDATA *pColor;

	// ���_�J���[�o�b�t�@�����b�N��,���_�J���[�f�[�^�ւ̃|�C���^���擾
	m_pColorBuff[PARTS_LEFT_HAND]->Lock(0, 0, (void**)&pColor, 0);

	// ���_�J���[�̐ݒ�
	for (int nCnt = 0; nCnt < m_nNumInstance; nCnt++)
	{
		pColor[nCnt].col = AUDIENCE_LEFTHAND_COLOR / AUDIECNE_COLOR_RIVISION;
	}

	// ���_�J���[�o�b�t�@���A�����b�N����
	m_pColorBuff[PARTS_LEFT_HAND]->Unlock();
}

//=============================================================================
//    ���[�J�����W�ɂ����郏�[���h�}�g���b�N�X���쐬���鏈��
//=============================================================================
void CAudience::MakeMtxLocalLeftHand(void)
{
	// �傫���𔽉f
	m_MtxLocal[PARTS_LEFT_HAND]._11 = m_MtxLocal[PARTS_LEFT_HAND]._11 * AUDIENCE_LEFTHAND_SCALE.x;
	m_MtxLocal[PARTS_LEFT_HAND]._12 = m_MtxLocal[PARTS_LEFT_HAND]._12 * AUDIENCE_LEFTHAND_SCALE.x;
	m_MtxLocal[PARTS_LEFT_HAND]._13 = m_MtxLocal[PARTS_LEFT_HAND]._13 * AUDIENCE_LEFTHAND_SCALE.x;
	m_MtxLocal[PARTS_LEFT_HAND]._21 = m_MtxLocal[PARTS_LEFT_HAND]._21 * AUDIENCE_LEFTHAND_SCALE.y;
	m_MtxLocal[PARTS_LEFT_HAND]._22 = m_MtxLocal[PARTS_LEFT_HAND]._22 * AUDIENCE_LEFTHAND_SCALE.y;
	m_MtxLocal[PARTS_LEFT_HAND]._23 = m_MtxLocal[PARTS_LEFT_HAND]._23 * AUDIENCE_LEFTHAND_SCALE.y;
	m_MtxLocal[PARTS_LEFT_HAND]._31 = m_MtxLocal[PARTS_LEFT_HAND]._31 * AUDIENCE_LEFTHAND_SCALE.z;
	m_MtxLocal[PARTS_LEFT_HAND]._32 = m_MtxLocal[PARTS_LEFT_HAND]._32 * AUDIENCE_LEFTHAND_SCALE.z;
	m_MtxLocal[PARTS_LEFT_HAND]._33 = m_MtxLocal[PARTS_LEFT_HAND]._33 * AUDIENCE_LEFTHAND_SCALE.z;

	// �I�t�Z�b�g�ʒu�𔽉f
	m_MtxLocal[PARTS_LEFT_HAND]._41 = AUDIENCE_LEFTHAND_OFFSET.x;
	m_MtxLocal[PARTS_LEFT_HAND]._42 = AUDIENCE_LEFTHAND_OFFSET.y;
	m_MtxLocal[PARTS_LEFT_HAND]._43 = AUDIENCE_LEFTHAND_OFFSET.z;
}


//*****************************************************************************
//     �E��֘A
//*****************************************************************************
//=============================================================================
//    �E����쐬���鏈��
//=============================================================================
void CAudience::MakeRightHand(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̍쐬
	MakeVertexRightHand(pDevice);

	// ���_�J���[�o�b�t�@�̍쐬
	MakeColorBuffRightHand(pDevice);

	// ���[�J�����W�ɂ����郏�[���h�}�g���b�N�X���쐬����
	MakeMtxLocalRightHand();
}

//=============================================================================
//    ���_�o�b�t�@���쐬���鏈��
//=============================================================================
void CAudience::MakeVertexRightHand(const LPDIRECT3DDEVICE9 pDevice)
{
	// �����f����ǂݍ���
	LPD3DXMESH pMesh = NULL;
	LPD3DXBUFFER pBuffMat = NULL;
	DWORD nNumMat;
	D3DXLoadMeshFromX(AUDIENCE_PARTS_FILENAME, D3DXMESH_SYSTEMMEM, pDevice, NULL, &pBuffMat, NULL, &nNumMat, &pMesh);
	if (pMesh == NULL || pBuffMat == NULL) { return; }

	// ���_���ƃ|���S�������l��
	m_nNumVertex[PARTS_RIGHT_HAND] = pMesh->GetNumVertices();
	m_nNumPolygon[PARTS_RIGHT_HAND] = pMesh->GetNumFaces();

	// �C���f�b�N�X�o�b�t�@���擾
	pMesh->GetIndexBuffer(&m_pIdxBuff[PARTS_RIGHT_HAND]);

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEXDATA) * m_nNumVertex[PARTS_RIGHT_HAND],
		D3DUSAGE_WRITEONLY,
		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1),
		D3DPOOL_MANAGED,
		&m_pVtxBuff[PARTS_RIGHT_HAND],
		NULL);

	// ���_���̐ݒ�
	VERTEXDATA *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff[PARTS_RIGHT_HAND]->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�o�b�t�@�����b�N
	BYTE *pVtxBuff;  // ���_�o�b�t�@�ւ̃|�C���^
	pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < m_nNumVertex[PARTS_RIGHT_HAND]; nCntVtx++)
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
	m_pVtxBuff[PARTS_RIGHT_HAND]->Unlock();

	// �f�[�^���J��
	pMesh->Release();
	pBuffMat->Release();
}

//=============================================================================
//    ���_�J���[�o�b�t�@���쐬���鏈��
//=============================================================================
void CAudience::MakeColorBuffRightHand(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�J���[�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(COLORDATA) * m_nNumInstance,
		D3DUSAGE_WRITEONLY,
		(D3DFVF_DIFFUSE),
		D3DPOOL_MANAGED,
		&m_pColorBuff[PARTS_RIGHT_HAND],
		NULL);

	// ���_�J���[���̐ݒ�
	COLORDATA *pColor;

	// ���_�J���[�o�b�t�@�����b�N��,���_�J���[�f�[�^�ւ̃|�C���^���擾
	m_pColorBuff[PARTS_RIGHT_HAND]->Lock(0, 0, (void**)&pColor, 0);

	// ���_�J���[�̐ݒ�
	for (int nCnt = 0; nCnt < m_nNumInstance; nCnt++)
	{
		pColor[nCnt].col = AUDIENCE_RIGHTHAND_COLOR / AUDIECNE_COLOR_RIVISION;
	}

	// ���_�J���[�o�b�t�@���A�����b�N����
	m_pColorBuff[PARTS_RIGHT_HAND]->Unlock();
}

//=============================================================================
//    ���[�J�����W�ɂ����郏�[���h�}�g���b�N�X���쐬���鏈��
//=============================================================================
void CAudience::MakeMtxLocalRightHand(void)
{
	// �傫���𔽉f
	m_MtxLocal[PARTS_RIGHT_HAND]._11 = m_MtxLocal[PARTS_RIGHT_HAND]._11 * AUDIENCE_RIGHTHAND_SCALE.x;
	m_MtxLocal[PARTS_RIGHT_HAND]._12 = m_MtxLocal[PARTS_RIGHT_HAND]._12 * AUDIENCE_RIGHTHAND_SCALE.x;
	m_MtxLocal[PARTS_RIGHT_HAND]._13 = m_MtxLocal[PARTS_RIGHT_HAND]._13 * AUDIENCE_RIGHTHAND_SCALE.x;
	m_MtxLocal[PARTS_RIGHT_HAND]._21 = m_MtxLocal[PARTS_RIGHT_HAND]._21 * AUDIENCE_RIGHTHAND_SCALE.y;
	m_MtxLocal[PARTS_RIGHT_HAND]._22 = m_MtxLocal[PARTS_RIGHT_HAND]._22 * AUDIENCE_RIGHTHAND_SCALE.y;
	m_MtxLocal[PARTS_RIGHT_HAND]._23 = m_MtxLocal[PARTS_RIGHT_HAND]._23 * AUDIENCE_RIGHTHAND_SCALE.y;
	m_MtxLocal[PARTS_RIGHT_HAND]._31 = m_MtxLocal[PARTS_RIGHT_HAND]._31 * AUDIENCE_RIGHTHAND_SCALE.z;
	m_MtxLocal[PARTS_RIGHT_HAND]._32 = m_MtxLocal[PARTS_RIGHT_HAND]._32 * AUDIENCE_RIGHTHAND_SCALE.z;
	m_MtxLocal[PARTS_RIGHT_HAND]._33 = m_MtxLocal[PARTS_RIGHT_HAND]._33 * AUDIENCE_RIGHTHAND_SCALE.z;

	// �I�t�Z�b�g�ʒu�𔽉f
	m_MtxLocal[PARTS_RIGHT_HAND]._41 = AUDIENCE_RIGHTHAND_OFFSET.x;
	m_MtxLocal[PARTS_RIGHT_HAND]._42 = AUDIENCE_RIGHTHAND_OFFSET.y;
	m_MtxLocal[PARTS_RIGHT_HAND]._43 = AUDIENCE_RIGHTHAND_OFFSET.z;
}

//=============================================================================
//
//=============================================================================
void  CAudience::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{

}