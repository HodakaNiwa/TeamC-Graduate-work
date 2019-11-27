//*****************************************************************************
//
//     �G�t�F�N�g�Ǌ��̏���[effectManager.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "effectManager.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"
#include "emitter.h"
#include "particle.h"
#include "ringEffect.h"
#include "textureManager.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
//#define EFFECT_AUTO_SAVE                                    // �錾�� : �I�[�g�Z�[�u
#define EFFECT_SAVE_FILENAME "data/TEXT/save_ringEffect.txt"  // �G�t�F�N�g�f�[�^��ۑ�����O���t�@�C���p�X��

// �l�ǂݍ��݂�����ۂ̖ڈ�ƂȂ镶����
// �e�N�X�`��
#define NUM_TEXTURE "NUM_TEXTURE = "
#define TEXTURE_FILENAME "TEXTURE_FILENAME = "

// �G�~�b�^�f�[�^
#define NUM_EMITTER       "NUM_EMITTER = "         // �G�~�b�^�f�[�^�̌�
#define EMITTERSET        "EMITTERSET"             // �G�~�b�^�f�[�^�ǂݍ��݊J�n�̍��}
#define END_EMITTERSET    "END_EMITTERSET"         // �G�~�b�^�f�[�^�ǂݍ��ݏI���̍��}
#define TYPE              "TYPE = "                // �g�p����G�t�F�N�g�̎��
#define TEX_TYPE          "TEX_TYPE = "            // �g�p����e�N�X�`���̔ԍ�
#define EFFECT_TYPE       "EFFECT_TYPE = "         // �g�p����G�t�F�N�g�̔ԍ�
#define RANGE_MIN         "RANGE_MIN = "           // �G�t�F�N�g�𐶐�����͈͂̍ŏ��l
#define RANGE_MAX         "RANGE_MAX = "           // �G�t�F�N�g�𐶐�����͈͂̍ő�l
#define LIFE              "LIFE = "                // �G�~�b�^�̎���
#define LAP               "LAP = "                 // �P��̕��o�ŏo���G�t�F�N�g�̌�
#define APPEAR            "APPEAR = "              // �G�t�F�N�g���o���Ԋu
#define LOOP              "LOOP = "                // ���[�v���邩���Ȃ���

// �p�[�e�B�N���f�[�^
#define NUM_PARTICLE      "NUM_PARTICLE = "        // �p�[�e�B�N���f�[�^�̌�
#define PARTICLESET       "PARTICLESET"            // �p�[�e�B�N���f�[�^�ǂݍ��݊J�n�̍��}
#define END_PARTICLESET   "END_PARTICLESET"        // �p�[�e�B�N���f�[�^�ǂݍ��ݏI���̍��}
#define ROT               "ROT = "                 // ����
#define MAX_MOVE          "MAX_MOVE = "            // �ړ��ʂ̍ő�l
#define MIN_MOVE          "MIN_MOVE = "            // �ړ��ʂ̍ŏ��l
#define CHANGE_MOVE       "CHANGE_MOVE = "         // �ړ��ʂ̕ω���
#define INIT_COL          "INIT_COL = "            // �������̐F
#define CHANGE_COL        "CHANGE_COL = "          // ���t���[���̐F�̕ω���
#define MAX_SIZE          "MAX_SIZE = "            // �傫���̍ő�l
#define MIN_SIZE          "MIN_SIZE = "            // �傫���̍ŏ��l
#define SPREAD            "SPREAD = "              // �傫���̕ω���
#define MAX_LIFE          "MAX_LIFE = "            // �����̍ő�l
#define MIN_LIFE          "MIN_LIFE = "            // �����̍ŏ��l
#define GRAVITY           "GRAVITY = "             // ���t���[��������d��
#define BOUNCING          "BOUNCING = "            // �o�E���h��
#define ROT_PATTERN       "ROT_PATTERN = "         // ��]�̎��[ 0:���v��� 1:�����v��� 2:�����_�� ]
#define ROT_SPEED         "ROT_SPEED = "           // ��]����X�s�[�h
#define COLLISION         "COLLISION = "           // �����蔻�肷�邩���Ȃ���[ 0:�Ȃ� 1:���� ]
#define DRAW_ADDTIVE      "DRAW_ADDTIVE = "        // ���Z�������邩���Ȃ���[ 0:�Ȃ� 1:���� ]
#define LIGHTING          "LIGHTING = "            // ���C�e�B���O���邩���Ȃ���[ 0:�Ȃ� 1:���� ]

// �����O�G�t�F�N�g�f�[�^
#define NUM_RING          "NUM_RING = "            // �����O�G�t�F�N�g�f�[�^�̌�
#define RINGSET           "RINGSET"                // �����O�G�t�F�N�g�f�[�^�ǂݍ��݊J�n�̍��}
#define MAX_ROT           "MAX_ROT = "             // �����̍ő�l
#define MIN_ROT           "MIN_ROT = "             // �����̍ő�l
#define HEIGHT            "HEIGHT = "              // ����
#define RADIUS            "RADIUS = "              // ���a
#define DIFFUSION         "DIFFUSION = "           // ��
#define CHANGE_HEIGHT     "CHANGE_HEIGHT = "       // �����𖈃t���[���ǂꂭ�炢�ω������邩
#define CHANGE_RADIUS     "CHANGE_RADIUS = "       // ���a�𖈃t���[���ǂꂭ�炢�ω������邩
#define CHANGE_DIFFUSION  "CHANGE_DIFFUSION = "    // ���𖈃t���[���ǂꂭ�炢�ω������邩
#define XBLOCK            "XBLOCK = "              // ���̕�����
#define YBLOCK            "YBLOCK = "              // �c�̕�����
#define CULLING           "CULLING = "             // �J�����O���邩���Ȃ���[ 0:�Ȃ� 1:���� ]
#define END_RINGSET       "END_RINGSET"            // �����O�G�t�F�N�g�f�[�^�ǂݍ��ݏI���̍��}

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CEffectManager::CEffectManager()
{
	// �e��l�̃N���A
	strcpy(m_aFileName, "\0");				// �ǂݍ��ރX�N���v�g�t�@�C����
	strcpy(m_aTextureListFileName, "\0");	// �e�N�X�`�����X�g�t�@�C����
	m_apEmitterData = NULL;					// �G�~�b�^�f�[�^�N���X�ւ̃|�C���^
	m_apRingData = NULL;					// �����O�G�t�F�N�g�f�[�^�N���X�ւ̃|�C���^
	m_nNumEmitterData = 0;					// �G�~�b�^�f�[�^�̌�
	m_nNumRingEffectData = 0;				// �����O�G�t�F�N�g�f�[�^�̌�
	m_pTextureManager = NULL;				// �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CEffectManager::~CEffectManager()
{

}

//=============================================================================
//    ��������
//=============================================================================
CEffectManager *CEffectManager::Create(char *pFileName)
{
	CEffectManager *pEffectManager = NULL;   // �G�t�F�N�g�Ǌ��N���X�^�̃|�C���^
	if (pEffectManager == NULL)
	{// ����������ɂȂ��Ă���
		pEffectManager = new CEffectManager;
		if (pEffectManager != NULL)
		{// �C���X�^���X�𐶐��ł���
			pEffectManager->SetFileName(pFileName);
			if (FAILED(pEffectManager->Init()))
			{// �������Ɏ��s����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}

	return pEffectManager;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CEffectManager::Init(void)
{
	// �X�N���v�g�t�@�C�����ǂݍ��߂邩�`�F�b�N
	CFileLoader *pFileLoader = CFileLoader::Create(m_aFileName);
	if (pFileLoader == NULL) { return E_FAIL; }

	// �X�N���v�g�t�@�C����ǂݍ���
	char aStr[256] = "\0";
	strcpy(aStr, pFileLoader->GetString(aStr));
	if (CFunctionLib::Memcmp(aStr, SCRIPT) == 0)
	{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
		if (FAILED(LoadScript(pFileLoader, aStr)))
		{
			return E_FAIL;
		}
	}

	// �������̊J��
	if (pFileLoader != NULL)
	{
		pFileLoader->Uninit();
		delete pFileLoader;
		pFileLoader = NULL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CEffectManager::Uninit(void)
{
#ifdef EFFECT_AUTO_SAVE
	// �G�t�F�N�g�f�[�^��ۑ�����
	Save();
#endif

	// �e�N�X�`���Ǌ��N���X�̊J��
	ReleaseTextureManager();

	// �G�~�b�^�f�[�^�N���X�̊J��
	ReleaseEmitterData();

	// �����O�G�t�F�N�g�f�[�^�N���X�̊J��
	ReleaseRingEffectData();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CEffectManager::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CEffectManager::Draw(void)
{

}

//=============================================================================
//    �G�t�F�N�g��ݒ肷�鏈��
//=============================================================================
CEmitter *CEffectManager::SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority)
{
	// �G�~�b�^�f�[�^�N���X����f�[�^���擾
	CEmitter *pEmitter = NULL;
	if (m_apEmitterData[nType] != NULL)
	{// �f�[�^�N���X���쐬����Ă���
		int nEffectType = m_apEmitterData[nType]->GetType();
		int nTexIdx = m_apEmitterData[nType]->GetTexIdx();
		int nEffectIdx = m_apEmitterData[nType]->GetEffectIdx();
		int nLife = m_apEmitterData[nType]->GetLife();
		D3DXVECTOR3 RangeMax = m_apEmitterData[nType]->GetRangeMax();
		D3DXVECTOR3 RangeMin = m_apEmitterData[nType]->GetRangeMin();
		int nLap = m_apEmitterData[nType]->GetLap();
		int nAppear = m_apEmitterData[nType]->GetAppear();
		bool bLoop = m_apEmitterData[nType]->GetLoop();

		// �G�~�b�^�N���X���쐬����
		pEmitter = CRingEmitter::Create(pos, rot, nTexIdx, nEffectIdx, RangeMax, RangeMin, nLife, nLap, nAppear, bLoop, m_apRingData[nEffectIdx], m_pTextureManager->GetTexture(nTexIdx), nType, nPriority);
	}

	m_pEmitter = pEmitter;
	return pEmitter;
}

//=============================================================================
//    �X�N���v�g�t�@�C����ǂݍ���
//=============================================================================
HRESULT CEffectManager::LoadScript(CFileLoader *pFileLoader, char *pStr)
{
	int nCntTex = 0;
	int nCntEmitter = 0;
	int nCntParticle = 0;
	int nCntRing = 0;

	while (1)
	{// ���[�v�J�n
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_TEXTURE) == 0)
		{// �e�N�X�`�����X�g�t�@�C����������
			CreateTextureMemory(CFunctionLib::ReadInt(pStr, NUM_TEXTURE));
		}
		else if (CFunctionLib::Memcmp(pStr, TEXTURE_FILENAME) == 0)
		{// �e�N�X�`�����X�g�t�@�C����������
			LoadTexture(CFunctionLib::ReadString(pStr, pStr, TEXTURE_FILENAME), nCntTex);
			nCntTex++;
		}
		else if (CFunctionLib::Memcmp(pStr, NUM_EMITTER) == 0)
		{// �G�~�b�^�f�[�^�̌���񂪂���
			CreateEmitterDataMemory(CFunctionLib::ReadInt(pStr, NUM_EMITTER));
		}
		else if (CFunctionLib::Memcmp(pStr, EMITTERSET) == 0)
		{// �G�~�b�^�f�[�^�ǂݍ��݊J�n�̍��}������
			m_apEmitterData[nCntEmitter] = LoadEmitterData(pFileLoader, pStr, nCntEmitter);
			nCntEmitter++;
		}
		else if (CFunctionLib::Memcmp(pStr, NUM_RING) == 0)
		{// �����O�G�t�F�N�g�f�[�^�̌���񂪂���
			CreateRingEffectDataMemory(CFunctionLib::ReadInt(pStr, NUM_RING));
		}
		else if (CFunctionLib::Memcmp(pStr, RINGSET) == 0)
		{// �����O�G�t�F�N�g�f�[�^�ǂݍ��݊J�n�̍��}������
			m_apRingData[nCntRing] = LoadRingEffectData(pFileLoader, pStr, nCntRing);
			nCntRing++;
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}

	return S_OK;
}

//=============================================================================
//    �e�N�X�`����ǂݍ��ޏ���
//=============================================================================
HRESULT CEffectManager::LoadTexture(char *pTextureFileName, int nCntTex)
{
	if (m_pTextureManager == NULL) { return E_FAIL; }

	// �e�N�X�`���̓ǂݍ���
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	if (FAILED(D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), pTextureFileName, &pTexture)))
	{
		return E_FAIL;
	}

	// �e�N�X�`���Ǌ��N���X�ɃZ�b�g
	m_pTextureManager->BindTexture(pTexture, nCntTex);

	return S_OK;
}

//=============================================================================
//    �G�~�b�^�f�[�^��ǂݍ��ޏ���
//=============================================================================
CEmitterData *CEffectManager::LoadEmitterData(CFileLoader *pFileLoader, char *pStr, const int nCntEmitter)
{
	// �G�~�b�^�f�[�^�p�̃��������m�ۂ���
	CEmitterData *pEmitterData = NULL;  // �G�~�b�^�f�[�^�N���X�ւ̃|�C���^
	pEmitterData = new CEmitterData;
	if (pEmitterData == NULL) { return NULL; }

	while (1)
	{// ���[�v�J�n
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TYPE) == 0)
		{// �G�t�F�N�g�̎�ޏ�񂪂���
			pEmitterData->SetType(CFunctionLib::ReadInt(pStr, TYPE));
		}
		else if (CFunctionLib::Memcmp(pStr, TEX_TYPE) == 0)
		{// �e�N�X�`���ԍ���񂪂���
			pEmitterData->SetTexIdx(CFunctionLib::ReadInt(pStr, TEX_TYPE));
		}
		else if (CFunctionLib::Memcmp(pStr, EFFECT_TYPE) == 0)
		{// ��ޏ�񂪂���
			pEmitterData->SetEffectIdx(CFunctionLib::ReadInt(pStr, EFFECT_TYPE));
		}
		else if (CFunctionLib::Memcmp(pStr, RANGE_MIN) == 0)
		{// �G�t�F�N�g�𐶐�����͈͂̍ŏ��l��񂪂���
			pEmitterData->SetRangeMin(CFunctionLib::ReadVector3(pStr, RANGE_MIN));
		}
		else if (CFunctionLib::Memcmp(pStr, RANGE_MAX) == 0)
		{// �G�t�F�N�g�𐶐�����͈͂̍ő�l��񂪂���
			pEmitterData->SetRangeMax(CFunctionLib::ReadVector3(pStr, RANGE_MAX));
		}
		else if (CFunctionLib::Memcmp(pStr, LIFE) == 0)
		{// ������񂪂���
			pEmitterData->SetLife(CFunctionLib::ReadInt(pStr, LIFE));
		}
		else if (CFunctionLib::Memcmp(pStr, LAP) == 0)
		{// 1��̕��o�Ő����������񂪂���
			pEmitterData->SetLap(CFunctionLib::ReadInt(pStr, LAP));
		}
		else if (CFunctionLib::Memcmp(pStr, APPEAR) == 0)
		{// ���o����Ԋu��񂪂���
			pEmitterData->SetAppear(CFunctionLib::ReadInt(pStr, APPEAR));
		}
		else if (CFunctionLib::Memcmp(pStr, LOOP) == 0)
		{// ���[�v���邩���Ȃ�����񂪂���
			pEmitterData->SetLoop(CFunctionLib::ReadBool(pStr, LOOP));
		}
		else if (CFunctionLib::Memcmp(pStr, END_EMITTERSET) == 0)
		{// �G�~�b�^�f�[�^�ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}

	return pEmitterData;
}

//=============================================================================
//    �����O�G�t�F�N�g�f�[�^��ǂݍ��ޏ���
//=============================================================================
CRingData *CEffectManager::LoadRingEffectData(CFileLoader *pFileLoader, char *pStr, const int nCntRingEffectData)
{
	// �����O�G�t�F�N�g�f�[�^�p�̃��������m�ۂ���
	CRingData *pRingData = NULL;
	pRingData = new CRingData;
	if (pRingData == NULL) { return NULL; }

	while (1)
	{// ���[�v�J�n
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, MAX_ROT) == 0)
		{// �����̍ő�l��񂪂���
			pRingData->SetMaxRot(CFunctionLib::ReadVector3(pStr, MAX_ROT));
		}
		else if (CFunctionLib::Memcmp(pStr, MIN_ROT) == 0)
		{// �����̍ŏ��l��񂪂���
			pRingData->SetMinRot(CFunctionLib::ReadVector3(pStr, MIN_ROT));
		}
		else if (CFunctionLib::Memcmp(pStr, MAX_MOVE) == 0)
		{// �ړ��ʂ̍ő�l��񂪂���
			pRingData->SetMaxMove(CFunctionLib::ReadVector3(pStr, MAX_MOVE));
		}
		else if (CFunctionLib::Memcmp(pStr, MIN_MOVE) == 0)
		{// �ړ��ʂ̍ŏ��l��񂪂���
			pRingData->SetMinMove(CFunctionLib::ReadVector3(pStr, MIN_MOVE));
		}
		else if (CFunctionLib::Memcmp(pStr, CHANGE_MOVE) == 0)
		{// �ړ��ʂ̕ω��ʏ�񂪂���
			pRingData->SetChangeMove(CFunctionLib::ReadVector3(pStr, CHANGE_MOVE));
		}
		else if (CFunctionLib::Memcmp(pStr, INIT_COL) == 0)
		{// �������̐F��񂪂���
			pRingData->SetInitCol(CFunctionLib::ReadVector4(pStr, INIT_COL));
		}
		else if (CFunctionLib::Memcmp(pStr, CHANGE_COL) == 0)
		{// �t���[�����̐F�̕ω��ʏ�񂪂���
			pRingData->SetChangeCol(CFunctionLib::ReadVector4(pStr, CHANGE_COL));
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ������񂪂���
			pRingData->SetHeight(CFunctionLib::ReadFloat(pStr, HEIGHT));
		}
		else if (CFunctionLib::Memcmp(pStr, RADIUS) == 0)
		{// ���a��񂪂���
			pRingData->SetRadius(CFunctionLib::ReadFloat(pStr, RADIUS));
		}
		else if (CFunctionLib::Memcmp(pStr, DIFFUSION) == 0)
		{// ����񂪂���
			pRingData->SetDiffusion(CFunctionLib::ReadFloat(pStr, DIFFUSION));
		}
		else if (CFunctionLib::Memcmp(pStr, CHANGE_HEIGHT) == 0)
		{// �����𖈃t���[���ǂꂭ�炢�ω��������񂪂���
			pRingData->SetChangeHeight(CFunctionLib::ReadFloat(pStr, CHANGE_HEIGHT));
		}
		else if (CFunctionLib::Memcmp(pStr, CHANGE_RADIUS) == 0)
		{// ���a�𖈃t���[���ǂꂭ�炢�ω��������񂪂���
			pRingData->SetChangeRadius(CFunctionLib::ReadFloat(pStr, CHANGE_RADIUS));
		}
		else if (CFunctionLib::Memcmp(pStr, CHANGE_DIFFUSION) == 0)
		{// ���𖈃t���[���ǂꂭ�炢�ω��������񂪂���
			pRingData->SetChangeDiffusion(CFunctionLib::ReadFloat(pStr, CHANGE_DIFFUSION));
		}
		else if (CFunctionLib::Memcmp(pStr, MAX_LIFE) == 0)
		{// �����̍ő�l��񂪂���
			pRingData->SetMaxLife(CFunctionLib::ReadInt(pStr, MAX_LIFE));
		}
		else if (CFunctionLib::Memcmp(pStr, MIN_LIFE) == 0)
		{// �����̍ŏ��l��񂪂���
			pRingData->SetMinLife(CFunctionLib::ReadInt(pStr, MIN_LIFE));
		}
		else if (CFunctionLib::Memcmp(pStr, XBLOCK) == 0)
		{// ���̕�������񂪂���
			pRingData->SetXBlock(CFunctionLib::ReadInt(pStr, XBLOCK));
		}
		else if (CFunctionLib::Memcmp(pStr, YBLOCK) == 0)
		{// �c�̕�������񂪂���
			pRingData->SetYBlock(CFunctionLib::ReadInt(pStr, YBLOCK));
		}
		else if (CFunctionLib::Memcmp(pStr, ROT_PATTERN) == 0)
		{// ��]�̎�ޏ�񂪂���
			pRingData->SetRotPattern(CFunctionLib::ReadInt(pStr, ROT_PATTERN));
		}
		else if (CFunctionLib::Memcmp(pStr, ROT_SPEED) == 0)
		{// ��]�̃X�s�[�h��񂪂���
			pRingData->SetRotSpeed(CFunctionLib::ReadVector3(pStr, ROT_SPEED));
		}
		else if (CFunctionLib::Memcmp(pStr, CULLING) == 0)
		{// �J�����O�����邩���Ȃ�����񂪂���
			pRingData->SetCulling(CFunctionLib::ReadBool(pStr, CULLING));
		}
		else if (CFunctionLib::Memcmp(pStr, DRAW_ADDTIVE) == 0)
		{// ���Z�������邩���Ȃ�����񂪂���
			pRingData->SetDrawAddtive(CFunctionLib::ReadBool(pStr, DRAW_ADDTIVE));
		}
		else if (CFunctionLib::Memcmp(pStr, END_RINGSET) == 0)
		{// �����O�G�t�F�N�g�f�[�^�ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}

	return pRingData;
}

//=============================================================================
//    �G�t�F�N�g�f�[�^��ۑ����鏈��
//=============================================================================
void CEffectManager::Save(void)
{
	CFileSaver *pFileSaver = CFileSaver::Create(EFFECT_SAVE_FILENAME);
	if (pFileSaver != NULL)
	{// �t�@�C���|�C���^���m�ۂł���
	    // �t�@�C���̖`���������쐬
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("# �G�t�F�N�g�f�[�^ [%s]\n", EFFECT_SAVE_FILENAME);
		pFileSaver->Print("# Author : HODAKA NIWA\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("%s			# ���̍s�͐�΂ɏ����Ȃ����ƁI\n\n", SCRIPT);

		// �e�N�X�`���f�[�^�̕ۑ�
		SaveTextureData(pFileSaver);

		// �G�~�b�^�f�[�^�̕ۑ�
		SaveEmitterData(pFileSaver);

		// �����O�G�t�F�N�g�f�[�^�̕ۑ�
		SaveRingEffectData(pFileSaver);

		// �R�����g�������쐬
		pFileSaver->Print("%s			# ���̍s�͐�΂ɏ����Ȃ����ƁI\n\n", END_SCRIPT);

		// �������̊J��
		if (pFileSaver != NULL)
		{
			pFileSaver->Uninit();
			delete pFileSaver;
			pFileSaver = NULL;
		}
	}
}

//=============================================================================
//    �e�N�X�`�����X�g�t�@�C������ۑ����鏈��
//=============================================================================
void CEffectManager::SaveTextureData(CFileSaver *pFileSaver)
{
	// �R�����g�������쐬
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  �g�p����e�N�X�`���̐�\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// �g�p����e�N�X�`���̐���ۑ�
	pFileSaver->Print("%s%d\n\n", NUM_TEXTURE, m_aTextureListFileName);

	// �R�����g�������쐬
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  �g�p����e�N�X�`���̃t�@�C����\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// �t�@�C������ۑ�
	for (int nCntTex = 0; nCntTex < m_pTextureManager->GetNumTexture(); nCntTex++)
	{
		pFileSaver->Print("%s%s\n", TEXTURE_FILENAME, m_pTextureManager->GetFileName(nCntTex));
	}
	pFileSaver->Print("\n");
}

//=============================================================================
//    �G�~�b�^�f�[�^��ۑ����鏈��
//=============================================================================
void CEffectManager::SaveEmitterData(CFileSaver *pFileSaver)
{
	// �R�����g�������쐬
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  �G�~�b�^��\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// �G�~�b�^����ۑ�
	pFileSaver->Print("%s%d\n\n", NUM_EMITTER, m_nNumEmitterData);

	// �R�����g�������쐬
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  �G�~�b�^�̃f�[�^\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// �G�~�b�^�̃f�[�^���쐬�����������ۑ�
	if (m_apEmitterData == NULL) { return; }
	for (int nCntEmitter = 0; nCntEmitter < m_nNumEmitterData; nCntEmitter++)
	{// �쐬�����G�~�b�^�̐������J��Ԃ�
		if (m_apEmitterData[nCntEmitter] != NULL)
		{// �G�~�b�^�f�[�^���쐬����Ă���
			D3DXVECTOR3 RangeMin = m_apEmitterData[nCntEmitter]->GetRangeMin();
			D3DXVECTOR3 RangeMax = m_apEmitterData[nCntEmitter]->GetRangeMax();

			pFileSaver->Print("%s\n", EMITTERSET);
			pFileSaver->Print("	%s%d					# �g�p����G�t�F�N�g�̎��\n", TYPE, m_apEmitterData[nCntEmitter]->GetType());
			pFileSaver->Print("	%s%d				# �e�N�X�`���̔ԍ�\n", TEX_TYPE, m_apEmitterData[nCntEmitter]->GetTexIdx());
			pFileSaver->Print("	%s%d				# �g�p����G�t�F�N�g�̔ԍ�\n", EFFECT_TYPE, m_apEmitterData[nCntEmitter]->GetEffectIdx());
			pFileSaver->Print("	%s%.1f %.1f %.1f		# �G�t�F�N�g����o����͈͂̍ŏ��l\n", RANGE_MIN, RangeMin.x, RangeMin.y, RangeMin.z);
			pFileSaver->Print("	%s%.1f %.1f %.1f		# �G�t�F�N�g����o����͈͂̍ő�l\n", RANGE_MAX, RangeMax.x, RangeMax.y, RangeMax.z);
			pFileSaver->Print("	%s%d					# �G�~�b�^�̎���\n", LIFE, m_apEmitterData[nCntEmitter]->GetLife());
			pFileSaver->Print("	%s%d						# �P��̕��o�ŏo���G�t�F�N�g�̌�\n", LAP, m_apEmitterData[nCntEmitter]->GetLap());
			pFileSaver->Print("	%s%d					# �G�t�F�N�g���o���Ԋu\n", APPEAR, m_apEmitterData[nCntEmitter]->GetAppear());
			pFileSaver->Print("	%s%d					# ���[�v���邩���Ȃ���[ 0:�Ȃ� 1:���� ]\n", LOOP, (int)m_apEmitterData[nCntEmitter]->GetLoop());
			pFileSaver->Print("%s\n\n", END_EMITTERSET);
		}
	}
}

//=============================================================================
//    �����O�G�t�F�N�g�f�[�^��ۑ����鏈��
//=============================================================================
void CEffectManager::SaveRingEffectData(CFileSaver *pFileSaver)
{
	// �R�����g�������쐬
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  �����O�G�t�F�N�g�f�[�^�̐�\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// �����O�G�t�F�N�g�f�[�^����ۑ�
	pFileSaver->Print("%s%d\n\n", NUM_RING, m_nNumRingEffectData);

	// �R�����g�������쐬
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  �����O�G�t�F�N�g�̃f�[�^\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// �����O�G�t�F�N�g�̃f�[�^���쐬�����������ۑ�
	if (m_apRingData == NULL) { return; }
	for (int nCntRing = 0; nCntRing < m_nNumRingEffectData; nCntRing++)
	{// �쐬���������O�G�t�F�N�g�̃f�[�^�̐������J��Ԃ�
		pFileSaver->Print("%s\n", RINGSET);
		pFileSaver->Print("	%s%.4f %.4f %.4f			# �����̍ő�l\n", MAX_ROT, m_apRingData[nCntRing]->GetMaxRot().x, m_apRingData[nCntRing]->GetMaxRot().y, m_apRingData[nCntRing]->GetMaxRot().z);
		pFileSaver->Print("	%s%.4f %.4f %.4f			# �����̍ŏ��l\n", MIN_ROT, m_apRingData[nCntRing]->GetMinRot().x, m_apRingData[nCntRing]->GetMinRot().y, m_apRingData[nCntRing]->GetMinRot().z);
		pFileSaver->Print("	%s%.4f %.4f %.4f			# �ړ��ʂ̍ő�l\n", MAX_MOVE, m_apRingData[nCntRing]->GetMaxMove().x, m_apRingData[nCntRing]->GetMaxMove().y, m_apRingData[nCntRing]->GetMaxMove().z);
		pFileSaver->Print("	%s%.4f %.4f %.4f				# �ړ��ʂ̍ŏ��l\n", MIN_MOVE, m_apRingData[nCntRing]->GetMinMove().x, m_apRingData[nCntRing]->GetMinMove().y, m_apRingData[nCntRing]->GetMinMove().z);
		pFileSaver->Print("	%s%.4f %.4f %.4f			# �ړ��ʂ̕ω���\n", CHANGE_MOVE, m_apRingData[nCntRing]->GetChangeMove().x, m_apRingData[nCntRing]->GetChangeMove().y, m_apRingData[nCntRing]->GetChangeMove().z);
		pFileSaver->Print("	%s%.4f %.4f %.4f %.4f			# �������̐F\n", INIT_COL, m_apRingData[nCntRing]->GetInitCol().r, m_apRingData[nCntRing]->GetInitCol().g, m_apRingData[nCntRing]->GetInitCol().b, m_apRingData[nCntRing]->GetInitCol().a);
		pFileSaver->Print("	%s%.4f %.4f %.4f %.4f		# �t���[�����̐F�̕ω���\n", CHANGE_COL, m_apRingData[nCntRing]->GetChangeCol().r, m_apRingData[nCntRing]->GetChangeCol().g, m_apRingData[nCntRing]->GetChangeCol().b, m_apRingData[nCntRing]->GetChangeCol().a);
		pFileSaver->Print("	%s%f						# ����\n", HEIGHT, m_apRingData[nCntRing]->GetHeight());
		pFileSaver->Print("	%s%f						# ���a\n", RADIUS, m_apRingData[nCntRing]->GetRadius());
		pFileSaver->Print("	%s%f						# ��\n", DIFFUSION, m_apRingData[nCntRing]->GetDiffusion());
		pFileSaver->Print("	%s%f						# �����𖈃t���[���ǂꂭ�炢�ω�������\n", CHANGE_HEIGHT, m_apRingData[nCntRing]->GetChangeHeight());
		pFileSaver->Print("	%s%f						# ���a�𖈃t���[���ǂꂭ�炢�ω�������\n", CHANGE_RADIUS, m_apRingData[nCntRing]->GetChangeRadius());
		pFileSaver->Print("	%s%f						# ���𖈃t���[���ǂꂭ�炢�ω�������\n", CHANGE_DIFFUSION, m_apRingData[nCntRing]->GetChangeDiffusion());
		pFileSaver->Print("	%s%d						# �����̍ő�l\n", MAX_LIFE, m_apRingData[nCntRing]->GetMaxLife());
		pFileSaver->Print("	%s%d						# �����̍ŏ��l\n", MIN_LIFE, m_apRingData[nCntRing]->GetMinLife());
		pFileSaver->Print("	%s%d						# ���̕�����\n", XBLOCK, m_apRingData[nCntRing]->GetXBlock());
		pFileSaver->Print("	%s%d						# �c�̕�����\n", YBLOCK, m_apRingData[nCntRing]->GetYBlock());
		pFileSaver->Print("	%s%d						# ��]�̃p�^�[��[ 0:���v��� 1:�����v��� 2:�����_�� ]\n", ROT_PATTERN, m_apRingData[nCntRing]->GetRotPattern());
		pFileSaver->Print("	%s%.4f %.4f %.4f			# ��]�̃X�s�[�h\n", ROT_SPEED, m_apRingData[nCntRing]->GetRotSpeed().x, m_apRingData[nCntRing]->GetRotSpeed().y, m_apRingData[nCntRing]->GetRotSpeed().z);
		pFileSaver->Print("	%s%d						# �J�����O���邩���Ȃ���[ 0:�Ȃ� 1:���� ]\n", CULLING, (int)m_apRingData[nCntRing]->GetCulling());
		pFileSaver->Print("	%s%d					# ���Z�������邩���Ȃ���[ 0:�Ȃ� 1:���� ]\n", DRAW_ADDTIVE, (int)m_apRingData[nCntRing]->GetDrawAddtive());
		pFileSaver->Print("%s\n\n", END_RINGSET);
	}
}

//=============================================================================
//    �e�N�X�`���p�̃������𐶐����鏈��
//=============================================================================
void CEffectManager::CreateTextureMemory(const int nNumTexture)
{
	if (nNumTexture <= 0 || m_pTextureManager != NULL) { return; }
	m_pTextureManager = CTextureManager::Create(nNumTexture);
}

//=============================================================================
//    �G�~�b�^�f�[�^�p�̃������𐶐����鏈��
//=============================================================================
void CEffectManager::CreateEmitterDataMemory(const int nNumEmitterData)
{
	// ��������ۑ�
	m_nNumEmitterData = nNumEmitterData;

	// ���������m�ۂ���
	if (nNumEmitterData == 0 || m_apEmitterData != NULL) { return; }
	m_apEmitterData = new CEmitterData*[nNumEmitterData];

	// �m�ۂ��������������������Ă���
	if (m_apEmitterData == NULL) { return; }
	for (int nCntEmitter = 0; nCntEmitter < nNumEmitterData; nCntEmitter++)
	{// �G�~�b�^�f�[�^�������J��Ԃ�
		m_apEmitterData[nCntEmitter] = NULL;
	}
}

//=============================================================================
//    �����O�G�t�F�N�g�f�[�^�p�̃������𐶐����鏈��
//=============================================================================
void CEffectManager::CreateRingEffectDataMemory(const int nNumRingEffectData)
{
	// ��������ۑ�
	m_nNumRingEffectData = nNumRingEffectData;

	// ���������m�ۂ���
	if (nNumRingEffectData == 0 || m_apRingData != NULL) { return; }
	m_apRingData = new CRingData*[nNumRingEffectData];

	// �m�ۂ��������������������Ă���
	if (m_apRingData == NULL) { return; }
	for (int nCntRingData = 0; nCntRingData < nNumRingEffectData; nCntRingData++)
	{// �����O�G�t�F�N�g�f�[�^�������J��Ԃ�
		m_apRingData[nCntRingData] = NULL;
	}
}

//=============================================================================
//    �e�N�X�`���Ǌ��N���X���J�����鏈��
//=============================================================================
void CEffectManager::ReleaseTextureManager(void)
{
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}
}

//=============================================================================
//    �G�~�b�^�f�[�^���J�����鏈��
//=============================================================================
void CEffectManager::ReleaseEmitterData(void)
{
	// ���������m�ۂ���Ă��Ȃ���Ώ������Ȃ�
	if (m_apEmitterData == NULL) { return; }

	for (int nCntEmitter = 0; nCntEmitter < m_nNumEmitterData; nCntEmitter++)
	{// �G�~�b�^�f�[�^�������J��Ԃ�
		if (m_apEmitterData[nCntEmitter] != NULL)
		{// ���������m�ۂ���Ă���
			delete m_apEmitterData[nCntEmitter];
			m_apEmitterData[nCntEmitter] = NULL;
		}
	}
	delete[] m_apEmitterData;
	m_apEmitterData = NULL;
}

//=============================================================================
//    �����O�G�t�F�N�g�f�[�^���J�����鏈��
//=============================================================================
void CEffectManager::ReleaseRingEffectData(void)
{
	// ���������m�ۂ���Ă��Ȃ���Ώ������Ȃ�
	if (m_apRingData == NULL) { return; }

	for (int nCntRingData = 0; nCntRingData < m_nNumRingEffectData; nCntRingData++)
	{// �����O�G�t�F�N�g�f�[�^�������J��Ԃ�
		if (m_apRingData[nCntRingData] != NULL)
		{// ���������m�ۂ���Ă���
			delete m_apRingData[nCntRingData];
			m_apRingData[nCntRingData] = NULL;
		}
	}
	delete[] m_apRingData;
	m_apRingData = NULL;
}

//=============================================================================
//    �X�N���v�g�t�@�C������ݒ肷�鏈��
//=============================================================================
void CEffectManager::SetFileName(char *pFileName)
{
	strcpy(m_aFileName, pFileName);
}

//=============================================================================
//    �e�N�X�`�����X�g�t�@�C������ݒ肷�鏈��
//=============================================================================
void CEffectManager::SetTextureListFileName(char *pFileName)
{
	strcpy(m_aTextureListFileName, pFileName);
}

//=============================================================================
//    �Ō�ɐ��������G�~�b�^�N���X�ւ̃|�C���^��ݒ肷�鏈��
//=============================================================================
void CEffectManager::SetEmitter(CEmitter *pEmitter)
{
	m_pEmitter = pEmitter;
}

//=============================================================================
//    �G�~�b�^�f�[�^��ݒ肷�鏈��
//=============================================================================
void CEffectManager::SetEmitterData(CEmitterData *pEmitterData, const int nIdx)
{
	m_apEmitterData[nIdx] = pEmitterData;
}

//=============================================================================
//    �����O�G�t�F�N�g�f�[�^��ݒ肷�鏈��
//=============================================================================
void CEffectManager::SetRingEffectData(CRingData *pRingData, const int nIdx)
{
	m_apRingData[nIdx] = pRingData;
}

//=============================================================================
//    �G�~�b�^�f�[�^�̐���ݒ肷�鏈��
//=============================================================================
void CEffectManager::SetNumEmitterData(const int nNumEmitterData)
{
	m_nNumEmitterData = nNumEmitterData;
}

//=============================================================================
//    �����O�G�t�F�N�g�f�[�^�̐���ݒ肷�鏈��
//=============================================================================
void CEffectManager::SetNumRingEffectData(const int nNumRingEffectData)
{
	m_nNumRingEffectData = nNumRingEffectData;
}

//=============================================================================
//    �X�N���v�g�t�@�C�������擾���鏈��
//=============================================================================
char *CEffectManager::GetFileName(void)
{
	return m_aFileName;
}

//=============================================================================
//    �e�N�X�`�����X�g�t�@�C�������擾���鏈��
//=============================================================================
char *CEffectManager::GetTextureListFileName(void)
{
	return m_aTextureListFileName;
}

//=============================================================================
//    �Ō�ɐ��������G�~�b�^�N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CEmitter *CEffectManager::GetEmitter(void)
{
	return m_pEmitter;
}

//=============================================================================
//    �G�~�b�^�f�[�^���擾���鏈��
//=============================================================================
CEmitterData **CEffectManager::GetEmitterData(void)
{
	return m_apEmitterData;
}
CEmitterData *CEffectManager::GetEmitterData(const int nIdx)
{
	return m_apEmitterData[nIdx];
}

//=============================================================================
//    �����O�G�t�F�N�g�f�[�^���擾���鏈��
//=============================================================================
CRingData **CEffectManager::GetRingEffectData(void)
{
	return m_apRingData;
}
CRingData *CEffectManager::GetRingEffectData(const int nIdx)
{
	return m_apRingData[nIdx];
}

//=============================================================================
//    �G�~�b�^�f�[�^�̐����擾���鏈��
//=============================================================================
int CEffectManager::GetNumEmitterData(void)
{
	return m_nNumEmitterData;
}

//=============================================================================
//    �����O�G�t�F�N�g�f�[�^�̐����擾���鏈��
//=============================================================================
int CEffectManager::GetNumRingEffectData(void)
{
	return m_nNumRingEffectData;
}