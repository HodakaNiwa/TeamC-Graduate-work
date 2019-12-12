//=============================================================================
//
// �����L���O�̏��� [ranking.cpp]
// Author : Shoji Takahiro
//=============================================================================
#include "ranking.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "loadText.h"
#include "number2D.h"
#include "camera.h"
#include "fieldmanager.h"
#include "score.h"
#include "RawMouse.h"
#include "sky.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FAREM_COUNT	(60 * 10)	// �t�F�[�h�̃t���[����
#define LOAD_UI		("data\\TEXT\\UI\\Ranking.txt")
#define LOAD_OBJ			("data/TEXT/Stage.txt")
#define LOAD_TERRITORY		("data/TEXT/Territory.txt")

// �^�C�g���̎��ԗp
#define RANKING_BG_TEXIDX     (3)
#define RANKING_BG_PRIORITY   (5)

#define RANKING_BG_POS0       (D3DXVECTOR3(640.0f, 100.0f, 0.0f))
#define RANKING_BG_POS1       (D3DXVECTOR3(640.0f, 180.0f, 0.0f))
#define RANKING_BG_POS2       (D3DXVECTOR3(640.0f, 240.0f, 0.0f))
#define RANKING_BG_POS3       (D3DXVECTOR3(640.0f, 300.0f, 0.0f))
#define RANKING_BG_POS4       (D3DXVECTOR3(640.0f, 360.0f, 0.0f))
#define RANKING_BG_POS5       (D3DXVECTOR3(640.0f, 420.0f, 0.0f))
#define RANKING_BG_POS6       (D3DXVECTOR3(640.0f, 480.0f, 0.0f))
#define RANKING_BG_POS7       (D3DXVECTOR3(640.0f, 540.0f, 0.0f))
#define RANKING_BG_POS8       (D3DXVECTOR3(640.0f, 600.0f, 0.0f))

#define RANKING_BG_SIZE0      (D3DXVECTOR3(560.0f, 50.0f, 0.0f))
#define RANKING_BG_SIZE1      (D3DXVECTOR3(560.0f, 30.0f, 0.0f))
#define RANKING_BG_SIZE2      (D3DXVECTOR3(560.0f, 30.0f, 0.0f))
#define RANKING_BG_SIZE3      (D3DXVECTOR3(560.0f, 30.0f, 0.0f))
#define RANKING_BG_SIZE4      (D3DXVECTOR3(560.0f, 30.0f, 0.0f))
#define RANKING_BG_SIZE5      (D3DXVECTOR3(560.0f, 30.0f, 0.0f))
#define RANKING_BG_SIZE6      (D3DXVECTOR3(560.0f, 30.0f, 0.0f))
#define RANKING_BG_SIZE7      (D3DXVECTOR3(560.0f, 30.0f, 0.0f))
#define RANKING_BG_SIZE8      (D3DXVECTOR3(560.0f, 30.0f, 0.0f))

#define RANKING_BG_ROTSPEED0  (D3DX_PI * 0.000f)
#define RANKING_BG_ROTSPEED1  (D3DX_PI * 0.000f)
#define RANKING_BG_ROTSPEED2  (D3DX_PI * 0.000f)
#define RANKING_BG_ROTSPEED3  (D3DX_PI * 0.000f)
#define RANKING_BG_ROTSPEED4  (D3DX_PI * 0.000f)
#define RANKING_BG_ROTSPEED5  (D3DX_PI * 0.000f)
#define RANKING_BG_ROTSPEED6  (D3DX_PI * 0.000f)
#define RANKING_BG_ROTSPEED7  (D3DX_PI * 0.000f)
#define RANKING_BG_ROTSPEED8  (D3DX_PI * 0.000f)

#define RANKING_BG_TEXHEIGHT0 (1.0f)
#define RANKING_BG_TEXHEIGHT1 (1.0f)
#define RANKING_BG_TEXHEIGHT2 (1.0f)
#define RANKING_BG_TEXHEIGHT3 (1.0f)
#define RANKING_BG_TEXHEIGHT4 (1.0f)
#define RANKING_BG_TEXHEIGHT5 (1.0f)
#define RANKING_BG_TEXHEIGHT6 (1.0f)
#define RANKING_BG_TEXHEIGHT7 (1.0f)
#define RANKING_BG_TEXHEIGHT8 (1.0f)

#define RANKING_BG_MOVE0 (0.0f)
#define RANKING_BG_MOVE1 (60.0f)
#define RANKING_BG_MOVE2 (60.0f)
#define RANKING_BG_MOVE3 (60.0f)
#define RANKING_BG_MOVE4 (60.0f)
#define RANKING_BG_MOVE5 (60.0f)
#define RANKING_BG_MOVE6 (60.0f)
#define RANKING_BG_MOVE7 (60.0f)
#define RANKING_BG_MOVE8 (60.0f)

#define RANKING_ADDPOS (1200.0f)

#define RANKING_TITLE		("data/TEXTURE/Ranking/RANKING_BG_000.png")
#define RANKING_001			("data/TEXTURE/Ranking/RANKING_BG_001.png")
#define RANKING_002			("data/TEXTURE/Ranking/RANKING_BG_002.png")
#define RANKING_003			("data/TEXTURE/Ranking/RANKING_BG_003.png")
#define RANKING_004			("data/TEXTURE/Ranking/RANKING_BG_004.png")
#define RANKING_005			("data/TEXTURE/Ranking/RANKING_BG_005.png")
#define RANKING_006			("data/TEXTURE/Ranking/RANKING_BG_006.png")
#define RANKING_007			("data/TEXTURE/Ranking/RANKING_BG_007.png")
#define RANKING_008			("data/TEXTURE/Ranking/RANKING_BG_008.png")

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************


//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CRanking::CRanking()
{
	for (int nCntTex = 0; nCntTex < 9; nCntTex++)
	{
		m_apTextureRanking[nCntTex] = NULL;
		m_apRankingBG[nCntTex] = NULL;
	}

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CRanking::~CRanking(){}

//=============================================================================
// ����������
//=============================================================================
void CRanking::Init(void)
{
	//�x�[�X�̏�����
	CModeBase::Init();

	// �ϐ���������
	m_pCamera = NULL;
	m_pFieldManager = NULL;
	m_pLoadTextObj = NULL;
	m_pLoadTerritory = NULL;
	m_nNumRanking = 0;
	m_nCounterRanking = 0;

	//UI���̓ǂݍ���
	LoadUI(LOAD_UI);

	//�e�N�X�`���̓ǂݍ���
	CNumber2D::Load();
	CScoreRanking::LoadTex();
	LoadTexture();

	// �����L���OBG�𐶐�
	CreateRankingBG(0);

	//�J�����̐���
	if (m_pCamera == NULL)
	{
		D3DVIEWPORT9 viewport;
		viewport.X = 0;		//�r���[�|�[�g�̍���X���W
		viewport.Y = 0;		//�r���[�|�[�g�̍���Y���W
		viewport.Width = SCREEN_WIDTH;	//��
		viewport.Height = SCREEN_HEIGHT;//����
		viewport.MaxZ = 1.0f;
		viewport.MinZ = 0.0f;

		m_pCamera = new CCamera;
		m_pCamera->Init();
		m_pCamera->SetViewport(viewport);
		m_pCamera->SetPosV(D3DXVECTOR3(14.0f, 1000.0f, -600.0f));
		m_pCamera->SetPosR(D3DXVECTOR3(900.0f, 255.0f, 600.0f));
	}

	//�n�ʂ̐���
	if (m_pFieldManager == NULL)
	{
		m_pFieldManager = new CFieldManager;
		m_pFieldManager->Init();
	}

	if (m_pLoadTextObj == NULL) { m_pLoadTextObj = CLoadTextObject::Create(LOAD_OBJ); }					//�I�u�W�F�N�g�̐���
	if (m_pLoadTerritory == NULL) { m_pLoadTerritory = CLoadTextTerritory::Create(LOAD_TERRITORY); }	//�e���g���[�̓ǂݍ���

	// ��̐���
	CSky::Load();
	CSky::Create();

}
//=============================================================================
// �I������
//=============================================================================
void CRanking::Uninit(void)
{
	//�J�����̔j��
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	//�n�ʃ}�l�[�W���[�̔j��
	if (m_pFieldManager != NULL)
	{
		m_pFieldManager->Uninit();
		delete m_pFieldManager;
		m_pFieldManager = NULL;
	}

	//�I�u�W�F�N�g�̔j��
	if (m_pLoadTextObj != NULL)
	{
		m_pLoadTextObj->Uninit();
		delete m_pLoadTextObj;
		m_pLoadTextObj = NULL;
	}

	//�e���g���[�̔j��
	if (m_pLoadTerritory != NULL)
	{
		m_pLoadTerritory->Uninit();
		delete m_pLoadTerritory;
		m_pLoadTerritory = NULL;
	}

	// �e�N�X�`���̊J��
	CNumber2D::Unload();
	UnloadTexture();
	CScoreRanking::UnloadTex();
	CSky::UnLoad();

	for (int nCntTex = 0; nCntTex < 9; nCntTex++)
	{
		if (m_apRankingBG[nCntTex] != NULL)
		{
			m_apRankingBG[nCntTex]->Uninit();
			m_apRankingBG[nCntTex] = NULL;
		}
	}

	//�x�[�X�̔j��
	CModeBase::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CRanking::Update(void)
{
	//�T�E���h�̎擾
	CSound *pSound = CManager::GetSound();

	//�C���v�b�g�̎擾�@
	CGamePad	*pGamePad = CManager::GetInputGamePad();
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();
	CInputXPad * pXPad = CManager::GetXPad();
	CRawMouse *pRawMouse = CManager::GetRawMouse();					//RawMouse�̎擾

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || pXPad->GetTrigger(XINPUT_GAMEPAD_B, nCnt) == true ||
			pRawMouse->GetTrigger(nCnt, CRawMouse::RIMS_BUTTON_RIGHT) == true)
		{
			if (CFade::GetFadeMode() == CFade::FADE_NONE)
			{
				//���艹
				pSound->PlaySound(CSound::SOUND_LABEL_SE007);
				CFade::SetFade(CManager::MODE_TITLE);
			}
		}
	}

	// �J�E���^�[�𑝉�
	m_nCounterRanking++;
	if (m_nCounterRanking == 10)
	{// �J�E���^�[�����̒l�ɒB����
		if (m_nNumRanking == 0)
		{
			// �����L���O�𐶐�
			CScoreRanking::Create(CScore::TYPE_RANKING, 6, D3DXVECTOR3(1007.5f + 80.0f + RANKING_ADDPOS, 180.0f, 0.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f));
		}
		if (m_nNumRanking < 8)
		{// �܂������ł���
			// �����L���OBG�𐶐�
			CreateRankingBG((8 - m_nNumRanking));
			m_nNumRanking++;
			m_nCounterRanking = 0;
		}
	}

	// �J�E���^�[�𑝉�
	m_nCounter++;
	if ((m_nCounter > FAREM_COUNT))
	{// �J�E���^�[�����̒l�ɒB����
		if (CFade::GetFadeMode() != CFade::FADE_OUT)
		{//�^�C�g���ɑJ��
			CFade::SetFade(CManager::MODE_TITLE);
			m_nCounter = 0;

		}
	}

	// �n�ʂ̍X�V����
	if (m_pFieldManager != NULL) { m_pFieldManager->Update(); }

	//���ʂ̒���
	//pSound->SetVolume(CSound::SOUND_LABEL_SE007, 10.0f);

	//�f�o�b�N�L�[
	if (pKeyboard->GetKeyboardTrigger(DIK_1) == true) { CFade::SetFade(CManager::MODE_RANKING); }
}
//=============================================================================
// �`�揈��
//=============================================================================
void CRanking::Draw(void)
{
	//�J�����̐ݒ�
	if (m_pCamera != NULL) { m_pCamera->Set(); }

	CScene::DrawAll();
}

//=============================================================================
// �e�N�X�`���̓ǂݍ��ݏ���
//=============================================================================
void CRanking::LoadTexture(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, RANKING_TITLE, &m_apTextureRanking[0]);		//�����L���O�^�C�g��
	D3DXCreateTextureFromFile(pDevice, RANKING_001, &m_apTextureRanking[1]);		//1��
	D3DXCreateTextureFromFile(pDevice, RANKING_002, &m_apTextureRanking[2]);		//2��
	D3DXCreateTextureFromFile(pDevice, RANKING_003, &m_apTextureRanking[3]);		//3��
	D3DXCreateTextureFromFile(pDevice, RANKING_004, &m_apTextureRanking[4]);		//4��
	D3DXCreateTextureFromFile(pDevice, RANKING_005, &m_apTextureRanking[5]);		//5��
	D3DXCreateTextureFromFile(pDevice, RANKING_006, &m_apTextureRanking[6]);		//6��
	D3DXCreateTextureFromFile(pDevice, RANKING_007, &m_apTextureRanking[7]);		//7��
	D3DXCreateTextureFromFile(pDevice, RANKING_008, &m_apTextureRanking[8]);		//8��
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CRanking::UnloadTexture(void)
{
	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < RANKING_BG_MAX; nCnt++)
	{
		if (m_apTextureRanking[nCnt] != NULL)
		{
			m_apTextureRanking[nCnt]->Release();
			m_apTextureRanking[nCnt] = NULL;
		}
	}
}

//=============================================================================
// �����L���OBG�𐶐����鏈��
//=============================================================================
void CRanking::CreateRankingBG(int nIdx)
{
	D3DXVECTOR3 pos[RANKING_BG_MAX] =
	{
		RANKING_BG_POS0,
		RANKING_BG_POS1,
		RANKING_BG_POS2,
		RANKING_BG_POS3,
		RANKING_BG_POS4,
		RANKING_BG_POS5,
		RANKING_BG_POS6,
		RANKING_BG_POS7,
		RANKING_BG_POS8,
	};
	D3DXVECTOR3 Size[RANKING_BG_MAX] =
	{
		RANKING_BG_SIZE0,
		RANKING_BG_SIZE1,
		RANKING_BG_SIZE2,
		RANKING_BG_SIZE3,
		RANKING_BG_SIZE4,
		RANKING_BG_SIZE5,
		RANKING_BG_SIZE6,
		RANKING_BG_SIZE7,
		RANKING_BG_SIZE8,
	};
	float fRotSpeed[RANKING_BG_MAX] =
	{
		RANKING_BG_ROTSPEED0,
		RANKING_BG_ROTSPEED1,
		RANKING_BG_ROTSPEED2,
		RANKING_BG_ROTSPEED3,
		RANKING_BG_ROTSPEED4,
		RANKING_BG_ROTSPEED5,
		RANKING_BG_ROTSPEED6,
		RANKING_BG_ROTSPEED7,
		RANKING_BG_ROTSPEED8,
	};
	float fTexHeight[RANKING_BG_MAX] =
	{
		RANKING_BG_TEXHEIGHT0,
		RANKING_BG_TEXHEIGHT1,
		RANKING_BG_TEXHEIGHT2,
		RANKING_BG_TEXHEIGHT3,
		RANKING_BG_TEXHEIGHT4,
		RANKING_BG_TEXHEIGHT5,
		RANKING_BG_TEXHEIGHT6,
		RANKING_BG_TEXHEIGHT7,
		RANKING_BG_TEXHEIGHT8,
	};
	float fMove[RANKING_BG_MAX] =
	{
		RANKING_BG_MOVE0,
		RANKING_BG_MOVE1,
		RANKING_BG_MOVE2,
		RANKING_BG_MOVE3,
		RANKING_BG_MOVE4,
		RANKING_BG_MOVE5,
		RANKING_BG_MOVE6,
		RANKING_BG_MOVE7,
		RANKING_BG_MOVE8,
	};

	//�e�N�X�`���̕\��
	float fAddPos = 0.0f;
	if (nIdx != 0)
	{
		fAddPos = RANKING_ADDPOS;
	}
	m_apRankingBG[nIdx] = CRankingBG::Create(pos[nIdx] + D3DXVECTOR3(fAddPos, 0.0f, 0.0f), pos[nIdx], Size[nIdx], fMove[nIdx], fRotSpeed[nIdx], fTexHeight[nIdx], 5);
	if (m_apRankingBG[nIdx] == NULL) { return; }
	m_apRankingBG[nIdx]->BindTexture(m_apTextureRanking[nIdx]);
}


//*****************************************************************************
// �����L���OBG�N���X�̏���(CRankingBG)
//*****************************************************************************
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CRankingBG::CRankingBG(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fMove = 0.1f;
	m_fRot = 0.0f;
	m_fRotSpeed = 0.0f;
	m_fAngle = 0.0f;
	m_fLength = 0.0f;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CRankingBG::~CRankingBG(){}

//=============================================================================
// ��������
//=============================================================================
CRankingBG *CRankingBG::Create(D3DXVECTOR3 pos, D3DXVECTOR3 destPos, D3DXVECTOR3 Size, float fMove, float fRotSpeed, float fTexHeight, int nPriority, CScene2D::TYPE type)
{
	// �������̊m��
	CRankingBG *m_pRankingBG2D = new CRankingBG(nPriority);
	if (m_pRankingBG2D == NULL) { return NULL; }

	// �e��l�̐ݒ�
	m_pRankingBG2D->SetRotSpeed(fRotSpeed);
	m_pRankingBG2D->SetMove(fMove);
	m_pRankingBG2D->SetPos(pos);
	m_pRankingBG2D->SetDestPos(destPos);

	// ����������
	if (FAILED(m_pRankingBG2D->Init()))
	{
		m_pRankingBG2D->Uninit();
		m_pRankingBG2D = NULL;
		return NULL;
	}

	// �傫���̐ݒ�
	m_pRankingBG2D->Set(pos, Size);

	// �e�N�X�`�����W�̐ݒ�
	m_pRankingBG2D->SetTexUV(0.0f, 1.0f, 0.0f, fTexHeight);

	// �|���S���̑傫�����v�Z���Ă���
	m_pRankingBG2D->CalcPolygonScale();

	return m_pRankingBG2D;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CRankingBG::Init(void)
{
	// ���ʂ̏���������
	if (FAILED(CScene2D::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CRankingBG::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CRankingBG::Update(void)
{
	// �ʒu����ɓ�����
	m_pos.x -= m_fMove;

	// �ړI�̈ʒu����ɓ������Ă�����߂�
	if (m_pos.x < m_DestPos.x)
	{
		m_pos.x = m_DestPos.x;
	}

	// �|���S���̈ʒu��ݒ�
	CScene2D::SetPos(m_pos, GetOtherPos());
}

//=============================================================================
// �`�揈��
//=============================================================================
void CRankingBG::Draw(void)
{
	// ���ʂ̕`�揈��
	CScene2D::Draw();
}

//=============================================================================
// �|���S���̑傫�����v�Z���鏈��
//=============================================================================
void CRankingBG::CalcPolygonScale(void)
{
	// �傫�����擾
	D3DXVECTOR3 Size = GetSize();

	// �|���S���̑Ίp���̊p�x���߂�
	m_fAngle = (float)atan2(Size.x, Size.y);

	// �Ίp���̒��������߂�
	m_fLength = sqrtf((Size.x * Size.x) + (Size.y * Size.y));
}

//=============================================================================
// �|���S������]�����鏈��
//=============================================================================
void CRankingBG::RotationPolygon(void)
{
	// ���_���
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff == NULL) { return; }
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̉�]���l�����č��W���v�Z
	float XPos[4];
	float YPos[4];

	// ���_���W
	for (int nCntVer = 0; nCntVer < 4; nCntVer++)
	{// ���_�̐������J��Ԃ�
		pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f) + GetPos();
	}

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();
}