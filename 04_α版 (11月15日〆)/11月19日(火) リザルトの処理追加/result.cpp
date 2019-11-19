//=============================================================================
//
// ���U���g���� [result.cpp]
// Author : �����@����
//
//=============================================================================
#include "result.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "loadText.h"
#include "camera.h"
#include "fieldmanager.h"
#include "charResult.h"
#include "scene2D.h"
#include "moveUI.h"
#include "resultCharUI.h"
#include "select.h"
#include "RawMouse.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FADE_OUT_TIME		(60  * 5)		// �t�F�[�h����܂ł̎���
#define UI_FILENAME			("data\\TEXT\\UI\\result.txt")
#define LOAD_OBJ			("data/TEXT/Stage.txt")
#define LOAD_TERRITORY		("data/TEXT/Territory.txt")
#define FLAG_JAPAN			("data/TEXTURE/Select/NationalFlag/Japan.png")
#define FLAG_RUSSIA			("data/TEXTURE/Select/NationalFlag/Russia.png")
#define FLAG_AMERICA		("data/TEXTURE/Select/NationalFlag/America.png")
#define FLAG_BRAZIL			("data/TEXTURE/Select/NationalFlag/Brazil.png")
#define FLAG_ENGLAND		("data/TEXTURE/Select/NationalFlag/Engrand.png")
#define FLAG_ITALY			("data/TEXTURE/Select/NationalFlag/Italy.png")
#define FLAG_NEWZEALAND		("data/TEXTURE/Select/NationalFlag/Newzealand.png")
#define FLAG_SOUTHAFRICA	("data/TEXTURE/Select/NationalFlag/SouthAfrica.png")
#define PLAYERNAME			("data/TEXTURE/result/PLAYER_ALL.png")
#define BACK_LINE			("data/TEXTURE/result/BackLine.png")
#define RANK_FLAM			("data/TEXTURE/result/ResultCharFlam.png")
#define RANK_ALLNUM			("data/TEXTURE/result/Rank_ALL.png")
#define COM_NAME			("data/TEXTURE/result/COMPUTER_ALL.png")
#define RANKING_FLAM		("data/TEXTURE/result/Result_Shape.png")
#define CPU_TEX				("data/TEXTURE/result/CPU.png")
#define P_TEX				("data/TEXTURE/result/P.png")
#define CHR_JAPAN			("data/TEXTURE/result/Char.png")

//��ʂ�������Ԏ��̃}�N��
#define FLAG_WHIGHTSIZE		(D3DXVECTOR3(130.0f, 100.0f, 0.0f))
#define FLAG_AIMSIZE		(D3DXVECTOR3(90.0f, 70.0f, 0.0f))

//�Y�[����Ԃ̃}�N��
#define CHANGE_TIME	(90)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CResult::TYPE CResult::m_type = TYPE_MAX;

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CResult::CResult(){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CResult::~CResult(){}

//=============================================================================
// ����������
//=============================================================================
void CResult::Init(void)
{
	//�x�[�X�̏�����
	CModeBase::Init();

	//�ϐ��̏�����
	m_pCamera = NULL;
	m_pLoadTerritory = NULL;
	m_pLoadTextObj = NULL;
	m_pFieldManager = NULL;
	m_pWinChar = NULL;
	m_pWhiteBack = NULL;
	m_pNatinalFlag = NULL;
	m_pWinerName = NULL;
	m_pBackLine = NULL;
	m_pRankNo1 = NULL;
	m_state = STATE_WHITE;
	m_bWhiteIn = false;
	m_nCharFlagDown = false;
	m_nMaxPlayer = 0;
	m_nCountID = 0;
	m_nCounter = 0;
	m_nTime = 0;

	for (int nCnt = 0; nCnt < CUNTRY; nCnt++)
	{
		m_pCuntryTex[nCnt] = NULL;
	}
	for (int nCnt = 0; nCnt < RESULT_TEX; nCnt++)
	{
		m_pTexture[nCnt] = NULL;
	}
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		m_pResultCharUI[nCnt] = NULL;
	}

	//�L�������
	for (int nCnt = 0; nCnt < MAX_CHARACTER; nCnt++)
	{
		m_CharInfo[nCnt] = { 0, 0, 0, 7 };
		m_CharResultScore[nCnt] = { 0, 0, 0, 7 };
		m_pCharImage[nCnt] = NULL;
		m_pCharaTex[nCnt] = NULL;
	}

	//�e�N�X�`���̓ǂݍ���
	LoadTex();

	//�n�ʂ̐���
	if (m_pFieldManager == NULL)
	{
		m_pFieldManager = new CFieldManager;
		m_pFieldManager->Init();
	}

	if (m_pLoadTextObj == NULL) { m_pLoadTextObj = CLoadTextObject::Create(LOAD_OBJ); }					//�I�u�W�F�N�g�̐���
	if (m_pLoadTerritory == NULL) { m_pLoadTerritory = CLoadTextTerritory::Create(LOAD_TERRITORY); }	//�e���g���[�̓ǂݍ���

	//�X�R�A���Ƀ\�[�g����
	SortScore();

	//�L�����e�N�X�`����ǂݍ���
	InitCharImage();

	//���҃L�����̐���
	InitWinerChar(m_CharInfo[0].nCuntry, m_CharInfo[0].nType);

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

		m_pCamera = new CResultCamera;
		if (m_pWinChar != NULL) { m_pCamera->SetTargetPos(m_pWinChar->GetPos()); }
		m_pCamera->Init();
		m_pCamera->SetViewport(viewport);
		m_pCamera->SetPosV(D3DXVECTOR3(-40.0f, 60.0f, -100.0f));
		m_pCamera->SetPosR(D3DXVECTOR3(-70.0f, 90.0f, 50.0f));
	}

	//���w�i�̐���
	if (m_pWhiteBack == NULL) { m_pWhiteBack = CScene2D::Create(D3DXVECTOR3(640.0f, 360.0f, 0.0f), D3DXVECTOR3(640.0f, 360.0f, 0.0f), CScene2D::TYPE_LOGO); }

	//�����̐���
	if (m_pNatinalFlag == NULL) { m_pNatinalFlag = CMoveUI::Create(D3DXVECTOR3(640.0f, 360.0f, 0.0f), D3DXVECTOR3(140.0f, 110.0f, 0.0f), m_pCuntryTex[m_CharInfo[0].nCuntry]); }

	//�L�����\���̏�����
	InitCharUI();
}

//=============================================================================
// �I������
//=============================================================================
void CResult::Uninit(void)
{
	//�x�[�X�̔j��
	CModeBase::Uninit();

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

	//�L�����\��UI�̔j��
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (m_pResultCharUI[nCnt] != NULL)
		{
			m_pResultCharUI[nCnt]->Uninit();
			delete m_pResultCharUI[nCnt];
			m_pResultCharUI[nCnt] = NULL;
		}
	}

	m_pWinChar = NULL;
	m_pNatinalFlag = NULL;
	m_pBackLine = NULL;

	//�L�����e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < MAX_CHARACTER; nCnt++)
	{
		if (m_pCharaTex[nCnt] != NULL)
		{
			m_pCharaTex[nCnt]->Release();
			m_pCharaTex[nCnt] = NULL;
		}
	}

	//�e�N�X�`���̔j��
	UnloadTex();

	//�t�F�[�h�ȊO���폜
	CScene::ReleaseFade();
}

//=============================================================================
// �X�V����
//=============================================================================
void CResult::Update(void)
{
	//�C���v�b�g�̎擾�@
	CGamePad	*pGamePad = CManager::GetInputGamePad();
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();
	
	if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (CFade::FADE_OUT != CFade::GetFadeMode())
		{
			//���艹
			CSound::PlaySound(CSound::SOUND_LABEL_SE007);
			CFade::SetFade(CManager::MODE_TITLE);
		}
	}

	//�J�����̍X�V����
	if (m_pCamera != NULL) { m_pCamera->Update(); }

	//��Ԃ��Ƃ̍X�V����
	switch (m_state)
	{
	case STATE_WHITE: UpdateWhite(); break;
	case STATE_ZOOM: UpdateZoom();	break;
	case STATE_RANKING: UpdateRanking(); break;
	case STATE_SCORERESULT: UpdateScoreResult(); break;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CResult::Draw(void)
{
	//�J�����̐ݒ�
	if (m_pCamera != NULL) { m_pCamera->Set(); }

	CScene::DrawAll();
}

//=============================================================================
// ��ʂ�������Ԃ̍X�V����
//=============================================================================
void CResult::UpdateWhite(void)
{
	if (m_pNatinalFlag == NULL) { return; }

	if (!m_bWhiteIn)
	{
		D3DXVECTOR3 Size = m_pNatinalFlag->GetSize();

		if (Size.x == FLAG_WHIGHTSIZE.x)
		{
			m_bWhiteIn = true;
		}
		else
		{
			m_pNatinalFlag->UpdateScaleDown(FLAG_WHIGHTSIZE, D3DXVECTOR2(0.1f, 0.1f));
		}
	}

	if (!m_bWhiteIn) { return; }

	//����������Ɉړ�����
	m_pNatinalFlag->UpdateScaleDown(FLAG_AIMSIZE, D3DXVECTOR2(1.0f, 0.7f));
	m_pNatinalFlag->UpdateMoveDest(D3DXVECTOR3(140.0f, 95.0f, 0.0f), 8.0f);

	if (m_pWhiteBack != NULL)
	{//�w�i�𓧖��ɂ���
		//���S�t���O
		bool bDeth = false;

		//�F���擾
		D3DXCOLOR col = m_pWhiteBack->GetColor();
		col.a -= 0.05f;

		if (col.a < 0.0f)
		{
			col.a = 0.0f;
			bDeth = true;
		}

		//�F��ݒ肷��
		m_pWhiteBack->SetColoer(col);

		if (bDeth)
		{//���w�i��j������
			m_pWhiteBack->Uninit();
			m_pWhiteBack = NULL;
		}
	}

	//�J�������L�����N�^�[���ڂ���Ԃɂ���
	if (m_pCamera != NULL) 
	{ 
		if (m_pCamera->GetState() != CResultCamera::STATE_CHAR_ZOOM)
		{
			m_pCamera->SetState(CResultCamera::STATE_CHAR_IN);
		}
		else
		{
			m_state = STATE_ZOOM;	//�Y�[����Ԃɂ���
		}
	}
}

//=============================================================================
// �L�����N�^�[�Y�[�����̏���
//=============================================================================
void CResult::UpdateZoom(void)
{
	//���Җ��̐����E�ړ�����
	if (m_pWinerName == NULL)
	{
		if (m_CharInfo[0].nCharNo < m_nMaxPlayer)
		{
			m_pWinerName = CMoveUI::Create(D3DXVECTOR3(350.0f, 600.0f, 0.0f), D3DXVECTOR3(300.0f, 100.0f, 0.0f), m_pTexture[0]);
			m_pWinerName->SetTexUV(0.0f, 1.0f, 0.25f * m_CharInfo[0].nCharNo, (0.25f * m_CharInfo[0].nCharNo) + 0.25f);
		}
		else
		{
			m_pWinerName = CMoveUI::Create(D3DXVECTOR3(350.0f, 600.0f, 0.0f), D3DXVECTOR3(300.0f, 100.0f, 0.0f), m_pTexture[4]);
			m_pWinerName->SetTexUV(0.0f, 1.0f, 0.125f * (m_CharInfo[0].nCharNo - m_nMaxPlayer), (0.125f * (m_CharInfo[0].nCharNo - m_nMaxPlayer)) + 0.125f);
		}
	}
	else
	{
		m_pWinerName->UpdateMoveDest(D3DXVECTOR3(300.0f, 600.0f, 0.0f), 1.0f);
	}

	//���Ԃ̉��Z
	m_nCounter++;

	if (CHANGE_TIME < m_nCounter) { m_state = STATE_RANKING; }
}

//=============================================================================
// �����L���O��\������
//=============================================================================
void CResult::UpdateRanking(void)
{
	if (m_pWinerName != NULL) { m_pWinerName->UpdateMoveDest(D3DXVECTOR3(300.0f, 350.0f, 0.0f), 25.0f); }	//���O

	//�����L���O�̕\��
	if (m_pRankNo1 == NULL)
	{
		m_pRankNo1 = CMoveUI::Create(D3DXVECTOR3(350.0f, -150.0f, 0.0f), D3DXVECTOR3(100.0f, 100.0f, 0.0f), m_pTexture[3]);
		m_pRankNo1->SetTex(0.125f, 0);
	}
	else
	{
		m_pRankNo1->UpdateMoveDest(D3DXVECTOR3(350.0f, 100.0f, 0.0f), 20.0f);
	}

	//�w�i�̃��C��
	if (m_pBackLine != NULL) { m_pBackLine->UpdateMoveDest(D3DXVECTOR3(640.0f, 640.0f, 0.0f), 25.0f); }

	//�L����UI�̕\��
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (m_pResultCharUI[nCnt] != NULL)
		{
			if (nCnt < 2)
			{
				m_pResultCharUI[nCnt]->UpdateMoveDest(D3DXVECTOR3(170.0f + (nCnt * 290.0f), 600.0f + (nCnt * 8.0f), 0.0f), 25.0f);
			}
			else if (nCnt == 2)
			{
				m_pResultCharUI[nCnt]->UpdateMoveDest(D3DXVECTOR3(720.0f, 600.0f + (nCnt * 8.0f), 0.0f), 25.0f);
			}
			else
			{
				int nCounter = nCnt;
				nCounter -= 3;
				m_pResultCharUI[nCnt]->UpdateMoveDest(D3DXVECTOR3(895.0f + (nCounter * 108.0f), 620.0f, 0.0f), 25.0f);
			}
		}
	}

	//���͏���
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();
	CInputXPad * pInputXPad = CManager::GetXPad();
	CRawMouse * pRawMouse = CManager::GetRawMouse();

	if (pKeyboard->GetKeyboardTrigger(DIK_SPACE) == true)
	{
		m_state = STATE_SCORERESULT;	//�X�R�A�\����Ԃɂ���
		InitScoreResult();				//�X�R�A�\�̏�����
	}

	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		if (pInputXPad->GetTrigger(XINPUT_GAMEPAD_A, nCnt) == true)
		{
			m_state = STATE_SCORERESULT;	//�X�R�A�\����Ԃɂ���
			InitScoreResult();				//�X�R�A�\�̏�����
		}

		if (pRawMouse->GetTrigger(nCnt, CRawMouse::RIMS_BUTTON_LEFT) == true)
		{
			m_state = STATE_SCORERESULT;	//�X�R�A�\����Ԃɂ���
			InitScoreResult();				//�X�R�A�\�̏�����
		}
	}
}

//=============================================================================
// �X�R�A�\���̏���������
//=============================================================================
void CResult::InitScoreResult(void)
{
	for (int nCnt = 0; nCnt < MAX_CHARACTER; nCnt++)
	{
		if (m_pCharImage[nCnt] == NULL)
		{
			m_pCharImage[nCnt] = CMoveUI::Create(D3DXVECTOR3(80.0f + (nCnt * 160.0f), -550.0f, 0.0f), D3DXVECTOR3(80.0f, 275.0f, 0.0f), m_pCharaTex[nCnt]);
		}
	}
}

//=============================================================================
// �X�R�A�\��\������
//=============================================================================
void CResult::UpdateScoreResult(void)
{
	m_nTime++;

	if ((m_nTime % 10) == 0)
	{
		m_nCountID++;
		
		//�l�̏C��
		if (m_nCountID > MAX_CHARACTER) { m_nCountID = MAX_CHARACTER; }
	}

	for (int nCnt = 0; nCnt < m_nCountID; nCnt++)
	{
		if (m_pCharImage[nCnt] != NULL)
		{
			m_pCharImage[nCnt]->UpdateMoveDest(D3DXVECTOR3(80.0f + (nCnt * 160.0f), 400.0f, 0.0f), 30.0f);
		}
	}
}

//=============================================================================
// �L����UI�̏���������
//=============================================================================
void CResult::InitCharUI(void)
{
	//�w�i�̃��C��
	if (m_pBackLine == NULL)
	{
		m_pBackLine = CMoveUI::Create(D3DXVECTOR3(640.0f, 890.0f, 0.0f), D3DXVECTOR3(640.0f, 83.0f, 0.0f), m_pTexture[1]);
	}

	//�L�����\��
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (m_pResultCharUI[nCnt] == NULL)
		{
			if (nCnt < 2)
			{
				m_pResultCharUI[nCnt] = CResultCharUI::Create(D3DXVECTOR3(170.0f + (nCnt * 290.0f), 850.0f + (nCnt * 8.0f), 0.0f), D3DXVECTOR3(150.0f + (nCnt * -15.0f), 200.0f + (nCnt * -15.0f), 0.0f), m_CharInfo[nCnt + 1].nCharNo, nCnt, m_CharInfo[nCnt + 1].nCuntry, m_CharInfo[nCnt + 1].nType);
			}
			else if (nCnt == 2)
			{
				m_pResultCharUI[nCnt] = CResultCharUI::Create(D3DXVECTOR3(720.0f, 850.0f + (nCnt * 8.0f), 0.0f), D3DXVECTOR3(150.0f + (nCnt * -15.0f), 200.0f + (nCnt * -15.0f), 0.0f), m_CharInfo[nCnt + 1].nCharNo, nCnt, m_CharInfo[nCnt + 1].nCuntry, m_CharInfo[nCnt + 1].nType);
			}
			else
			{
				int nCounter = nCnt;
				nCounter -= 3;
				m_pResultCharUI[nCnt] = CResultCharUI::Create(D3DXVECTOR3(895.0f + (nCounter * 108.0f), 870.0f, 0.0f), D3DXVECTOR3(52.0f, 160.0f, 0.0f), m_CharInfo[nCnt + 1].nCharNo, nCnt, m_CharInfo[nCnt + 1].nCuntry, m_CharInfo[nCnt + 1].nType);
			}
		}
	}
}

//=============================================================================
// �X�R�A���Ƀ\�[�g����
//=============================================================================
void CResult::SortScore(void)
{
	CGame *pGame = CManager::GetGame();			//�Q�[���̎擾
	m_nMaxPlayer = CSelect::GetEntryPlayer();	//�v���C���[�̑������擾
	int nMaxEnemy = pGame->GetEnemyNum();		//�G�l�~�[�̑������擾

	//�v���C���[�ƃG�l�~�[�̑������܂킷
	for (int nCnt = 0; nCnt < MAX_CHARACTER; nCnt++)
	{
		//�L�����N�^�[�̎擾
		CCharacter *pCharacter = pGame->GetChara(nCnt);

		//�L�����^�C�v�̎擾
		m_CharInfo[nCnt].nType = pCharacter->GetCharcterTypeResult(nCnt);
		m_CharResultScore[nCnt].nType = m_CharInfo[nCnt].nType;

		//�L�����̍����擾
		m_CharInfo[nCnt].nCuntry = pCharacter->GetCuntryResult(nCnt);
		m_CharResultScore[nCnt].nCuntry = m_CharInfo[nCnt].nCuntry;

		//�X�R�A�̎擾
		m_CharInfo[nCnt].nScore = pGame->GetScore(nCnt);
		m_CharResultScore[nCnt].nScore = m_CharInfo[nCnt].nScore;

		//�L�����ԍ��̕ۑ�
		m_CharInfo[nCnt].nCharNo = nCnt;
		m_CharResultScore[nCnt].nCharNo = nCnt;
	}

	for (int nCnt = m_nMaxPlayer; nCnt < MAX_CHARACTER; nCnt++)
	{
		//�L�����^�C�v�̎擾
		m_CharInfo[nCnt].nType = 0;
		m_CharResultScore[nCnt].nType = 0;

		//�L�����̍����擾
		m_CharInfo[nCnt].nCuntry = 0;
		m_CharResultScore[nCnt].nCuntry = 0;

		//�X�R�A�̎擾
		m_CharInfo[nCnt].nScore = 0;
		m_CharResultScore[nCnt].nScore = 0;
	}

	//�X�R�A���\�[�g����
	for (int nCnt = 0; nCnt < MAX_CHARACTER; nCnt++)
	{
		for (int nCnt2 = nCnt; nCnt2 < MAX_CHARACTER - 1; nCnt2++)
		{
			if (m_CharInfo[nCnt].nScore < m_CharInfo[nCnt2].nScore)
			{
				//�X�R�A�̓��ւ�
				int nSubScore = m_CharInfo[nCnt].nScore;
				m_CharInfo[nCnt].nScore = m_CharInfo[nCnt2].nScore;
				m_CharInfo[nCnt2].nScore = nSubScore;

				//�L�����^�C�v�̓���ւ�
				int nSubType = m_CharInfo[nCnt].nType;
				m_CharInfo[nCnt].nType = m_CharInfo[nCnt2].nType;
				m_CharInfo[nCnt2].nType = nSubType;

				//���^�C�v�̓��ւ�
				int nSubCuntry = m_CharInfo[nCnt].nCuntry;
				m_CharInfo[nCnt].nCuntry = m_CharInfo[nCnt2].nCuntry;
				m_CharInfo[nCnt2].nCuntry = nSubCuntry;

				//�L�����ԍ��̓���ւ�
				int nSubCharNo = m_CharInfo[nCnt].nCharNo;
				m_CharInfo[nCnt].nCharNo = m_CharInfo[nCnt2].nCharNo;
				m_CharInfo[nCnt2].nCharNo = nSubCharNo;
			}
		}
	}
}

//=============================================================================
// �����L�����̐���
//=============================================================================
void CResult::InitWinerChar(int nCuntry, int nType)
{
	switch (nCuntry)
	{
	case CCharacter::TYPE_JAPAN:
		if (nType == 0)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		}
		else if (nType == 1)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		}
		else
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\PLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\PLAYER_MOTION.txt");
		}
		break;

	case CCharacter::TYPE_RUSSIA:
		if (nType == 0)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		}
		else if (nType == 1)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		}
		else
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\PLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\PLAYER_MOTION.txt");
		}
		break;

	case CCharacter::TYPE_AMERICA:
		if (nType == 0)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		}
		else if (nType == 1)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		}
		else
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\PLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\PLAYER_MOTION.txt");
		}
		break;

	case CCharacter::TYPE_BRAZIL:
		if (nType == 0)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		}
		else if (nType == 1)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		}
		else
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\PLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\PLAYER_MOTION.txt");
		}
		break;

	case CCharacter::TYPE_ENGLAND:
		if (nType == 0)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		}
		else if (nType == 1)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		}
		else
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\PLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\PLAYER_MOTION.txt");
		}
		break;

	case CCharacter::TYPE_ITALY:
		if (nType == 0)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		}
		else if (nType == 1)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		}
		else
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\PLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\PLAYER_MOTION.txt");
		}
		break;

	case CCharacter::TYPE_NEWZEALAND:
		if (nType == 0)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		}
		else if (nType == 1)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		}
		else
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\PLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\PLAYER_MOTION.txt");
		}
		break;

	case CCharacter::TYPE_SOUTHAFRICA:
		if (nType == 0)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		}
		else if (nType == 1)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		}
		else
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\PLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\PLAYER_MOTION.txt");
		}
		break;
	}
}

//=============================================================================
// �L�����C���[�W�摜�̓ǂݍ��ݏ���
//=============================================================================
void CResult::InitCharImage(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCnt = 0; nCnt < MAX_CHARACTER; nCnt++)
	{
		switch (m_CharResultScore[nCnt].nCuntry)
		{
		case CCharacter::TYPE_JAPAN:	//���{
			if (m_CharResultScore[nCnt].nType == 0)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else if (m_CharResultScore[nCnt].nType == 1)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			break;

		case CCharacter::TYPE_RUSSIA:	//���V�A
			if (m_CharResultScore[nCnt].nType == 0)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else if (m_CharResultScore[nCnt].nType == 1)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			break;

		case CCharacter::TYPE_AMERICA:	//�A�����J
			if (m_CharResultScore[nCnt].nType == 0)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else if (m_CharResultScore[nCnt].nType == 1)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			break;

		case CCharacter::TYPE_BRAZIL:	//�u���W��
			if (m_CharResultScore[nCnt].nType == 0)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else if (m_CharResultScore[nCnt].nType == 1)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			break;

		case CCharacter::TYPE_ENGLAND:	//�C�M���X
			if (m_CharResultScore[nCnt].nType == 0)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else if (m_CharResultScore[nCnt].nType == 1)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			break;

		case CCharacter::TYPE_ITALY:	//�C�^���A
			if (m_CharResultScore[nCnt].nType == 0)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else if (m_CharResultScore[nCnt].nType == 1)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			break;

		case CCharacter::TYPE_NEWZEALAND:	//�j���[�W�[�����h
			if (m_CharResultScore[nCnt].nType == 0)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else if (m_CharResultScore[nCnt].nType == 1)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			break;

		case CCharacter::TYPE_SOUTHAFRICA:	//��A�t���J
			if (m_CharResultScore[nCnt].nType == 0)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else if (m_CharResultScore[nCnt].nType == 1)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			break;
		}
	}
}

//=============================================================================
// �e�N�X�`���̓ǂݍ��ݏ���
//=============================================================================
void CResult::LoadTex(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�����e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, FLAG_JAPAN, &m_pCuntryTex[0]);		//���{
	D3DXCreateTextureFromFile(pDevice, FLAG_RUSSIA, &m_pCuntryTex[1]);		//���V�A
	D3DXCreateTextureFromFile(pDevice, FLAG_AMERICA, &m_pCuntryTex[2]);		//�A�����J
	D3DXCreateTextureFromFile(pDevice, FLAG_BRAZIL, &m_pCuntryTex[3]);		//�u���W��
	D3DXCreateTextureFromFile(pDevice, FLAG_ENGLAND, &m_pCuntryTex[4]);		//�C�M���X
	D3DXCreateTextureFromFile(pDevice, FLAG_ITALY, &m_pCuntryTex[5]);		//�C�^���A
	D3DXCreateTextureFromFile(pDevice, FLAG_NEWZEALAND, &m_pCuntryTex[6]);	//�j���[�W�[�����h
	D3DXCreateTextureFromFile(pDevice, FLAG_SOUTHAFRICA, &m_pCuntryTex[7]);	//��A�t���J

	//���U���g�e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, PLAYERNAME, &m_pTexture[0]);		//�v���C���[�l�[��
	D3DXCreateTextureFromFile(pDevice, BACK_LINE, &m_pTexture[1]);		//�o�b�N���C��
	D3DXCreateTextureFromFile(pDevice, RANK_FLAM, &m_pTexture[2]);		//�����L���O�t���[��
	D3DXCreateTextureFromFile(pDevice, RANK_ALLNUM, &m_pTexture[3]);	//�ԍ��e�N�X�`��
	D3DXCreateTextureFromFile(pDevice, COM_NAME, &m_pTexture[4]);		//�R���s���[�^�[�̖��O
	D3DXCreateTextureFromFile(pDevice, RANKING_FLAM, &m_pTexture[5]);	//�����L���O�t���[��
	D3DXCreateTextureFromFile(pDevice, CPU_TEX, &m_pTexture[6]);		//CPU���S
	D3DXCreateTextureFromFile(pDevice, P_TEX, &m_pTexture[7]);			//P���S
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CResult::UnloadTex(void)
{
	//�����̃e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < CUNTRY; nCnt++)
	{
		if (m_pCuntryTex[nCnt] != NULL)
		{
			m_pCuntryTex[nCnt]->Release();
			m_pCuntryTex[nCnt] = NULL;
		}
	}

	//���U���g�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < RESULT_TEX; nCnt++)
	{
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}