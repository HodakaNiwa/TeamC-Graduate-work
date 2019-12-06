//=============================================================================
//
// �Z���N�g�̏��� [select.cpp]
// Author : ���� ����
//
//=============================================================================
#include "select.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "loadText.h"
#include "camera.h"
#include "loadText.h"
#include "fieldmanager.h"
#include "playericon.h"
#include "moveUI.h"
#include "charSelect.h"
#include "RawMouse.h"
#include "collision.h"
#include "sky.h"
#include "audience.h"
#include "ringRender.h"
#include "territoryRender.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define LOAD_UI				("data\\TEXT\\UI\\Select.txt")
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
#define SELECT_1P			("data/TEXTURE/Select/NumPlayer/1P.png")
#define SELECT_2P			("data/TEXTURE/Select/NumPlayer/2P.png")
#define SELECT_3P			("data/TEXTURE/Select/NumPlayer/3P.png")
#define SELECT_4P			("data/TEXTURE/Select/NumPlayer/4P.png")
#define NAME_JAPAN			("data/TEXTURE/Select/CountryName/JPN.png")
#define NAME_RUSSIA			("data/TEXTURE/Select/CountryName/RUS.png")
#define NAME_AMERICA		("data/TEXTURE/Select/CountryName/USA.png")
#define NAME_BRAZIL			("data/TEXTURE/Select/CountryName/BRA.png")
#define NAME_ENGLAND		("data/TEXTURE/Select/CountryName/GBR.png")
#define NAME_ITALY			("data/TEXTURE/Select/CountryName/ITA.png")
#define NAME_NEWZEALAND		("data/TEXTURE/Select/CountryName/NGL.png")
#define NAME_SOUTHAFRICA	("data/TEXTURE/Select/CountryName/ZAF.png")
#define NO_ENTRY			("data/TEXTURE/Select/NoEntry.png")
#define LOGO				("data/TEXTURE/Select/CountrySelect_Title.png")
#define FLAM				("data/TEXTURE/Select/select_window.png")
#define LOGO2				("data/TEXTURE/Select/PlayerSelect_title.png")
#define NO_ENTRY_CHR		("data/TEXTURE/Select/NoEntryChar.png")
#define BACK_CUNTRY			("data/TEXTURE/Select/BackArrow.png")
#define RADY_TO_FIGHT		("data/TEXTURE/Select/READY_TO_FIGHT.png")
#define FLAG_SIZE			(D3DXVECTOR3(100.0f, 70.0f, 0.0f))
#define PLAYER_SIZE			(D3DXVECTOR3(130.0f, 75.0f, 0.0f))
#define NAME_SIZE			(D3DXVECTOR3(80.0f, 20.0f, 0.0f))
#define NAME_POS			(D3DXVECTOR3(150.0f, 527.0f, 0.0f))
#define NAME_MOVE			(320.0f)
#define MOVE_X				(280.0f)
#define MOVE_Y				(200.0f)
#define NOENTRY_SIZE		(D3DXVECTOR3(150.0f, 100.0f, 0.0f))
#define NOENTRY_POS			(D3DXVECTOR3(170.0f, 605.0f, 0.0f))
#define NOENTRY_POS_REMAKE	(D3DXVECTOR3(-475.0f, 605.0f, 0.0f))
#define NOENTRY_MOVE		(315.0f)
#define MOVE_UP				(2.5f)
#define MOVE_DOWN			(3.4f)
#define MOVE_TIME			(70)
#define CHAR_BACK_SIZE		(D3DXVECTOR3(150.0f, 170.0f, 0.0f))
#define CHAR_BACK_POS		(D3DXVECTOR3(170.0f, 340.0f, 0.0f))
#define CHAR_BACK_MOVE		(313.0f)
#define BACKBUTTON_SIZE		(D3DXVECTOR3(30.0f, 30.0f, 0.0f))
#define TIME_BACKBOTTUN		(30)
#define CHANGE_CHARBACK		(D3DXVECTOR2(2.5f, 3.0f))
#define CHANGE_CHARBACK2	(D3DXVECTOR2(2.3f, 2.5f))
#define CHANGE_FLAM			(D3DXVECTOR2(5.0f, 4.0f))
#define CHANGE_BACKBUTTON	(D3DXVECTOR2(0.3f, 0.3f))
#define FIRST_SIZE			(D3DXVECTOR3(40.0f, 40.0f, 0.0f))


//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CMoveUI * CSelect::m_pNationalFlag[MAX_COUNTRY] = {};
int CSelect::m_nEntryPlayer = 0;
int CSelect::m_anPlayeCuntry[NUMPLAYER] = { -1, -1, -1, -1 };
int CSelect::m_anPlayerType[NUMPLAYER] = { -1, -1, -1, -1 };
int CSelect::m_anControler[NUMPLAYER] = { -1, -1, -1, -1 };
CSelect::INPUTTYPE CSelect::m_anInputType[NUMPLAYER] = { INPUTTYPE_MAX, INPUTTYPE_MAX, INPUTTYPE_MAX, INPUTTYPE_MAX };

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CSelect::CSelect(){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSelect::~CSelect(){}

//=============================================================================
// ����������
//=============================================================================
void CSelect::Init(void)
{
	//�x�[�X�̏�����
	CModeBase::Init();

	//�ϐ��̏�����
	m_pCamera = NULL;
	m_pFieldManager = NULL;
	m_pLoadTextObj = NULL;
	m_pLoadTerritory = NULL;
	m_pTexNoEntry = NULL;
	m_pLogo = NULL;
	m_pBackButton = NULL;
	m_pRedyFight = NULL;
	m_nEntryPlayer = 0;
	m_state = STATE_CUNTRY_SELECT;
	m_nCountTime = 0;
	m_nNumControler = 0;
	m_nCounter = 0;

	for (int nCnt = 0; nCnt < MAX_COUNTRY; nCnt++)
	{
		m_apTextureCountry[nCnt] = NULL;	//�����e�N�X�`��
		m_apTexCuntoryName[nCnt] = NULL;	//�����e�N�X�`��
		m_pNationalFlag[nCnt] = NULL;		//�����|�C���^
	}

	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		m_apTexPlayer[nCnt] = NULL;				//�v���C���[�A�C�R���e�N�X�`��
		m_pNumPlayer[nCnt] = NULL;				//�v���C���[�A�C�R���|�C���^
		m_pNoEntry[nCnt] = NULL;				//�G���g���[���Ă��邩�ǂ���
		m_pFram[nCnt] = NULL;					//�t���[��
		m_pCharBack[nCnt] = NULL;				//�L�����w�i
		m_pCuntoryName[nCnt] = NULL;			//����
		m_nCntPressTime[nCnt] = 0;				//�{�^��������������
		m_nCntPressTime2[nCnt] = 0;				//�{�^��������������
		m_anPlayeCuntry[nCnt] = -1;				//��
		m_anPlayerType[nCnt] = -1;				//�^�C�v
		m_pSelectChar[nCnt] = NULL;				//�L�����I��UI
		m_bPlayerEnter[nCnt] = false;			//�G���g���[���Ă��邩�ǂ���
		m_bEntryGamePad[nCnt] = false;			//�R���g���[���[���G���g���[���Ă��邩�ǂ���
		m_bEntryMouse[nCnt] = false;			//�}�E�X���G���g���[���Ă��邩�ǂ���
		m_anControler[nCnt] = -1;				//�R���g���[���[�̔ԍ�
		m_anInputType[nCnt] = INPUTTYPE_MAX;	//���̓f�o�C�X�̃^�C�v
	}
	m_bEntryKeyboard = false;					//�L�[�{�[�h���G���g���[���Ă邩�ǂ���

	for (int nCnt = 0; nCnt < SELECT_TEX; nCnt++)
	{
		m_pTex[nCnt] = NULL;
	}

	//�e�N�X�`���̓ǂݍ��ݏ���
	LoadTexture();
	CSelectCharacter::LoadTex();

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

		m_pCamera = new CRotationCamera;
		m_pCamera->Init(800.0f, D3DXVECTOR3(0.0f, 400.0f, 0.0f), 0.001f, -600.0f);
		m_pCamera->SetViewport(viewport);
	}

	//�n�ʂ̐���
	if (m_pFieldManager == NULL)
	{
		m_pFieldManager = new CFieldManager;
		m_pFieldManager->Init();
	}

	if (m_pLoadTextObj == NULL) { m_pLoadTextObj = CLoadTextObject::Create(LOAD_OBJ); }					//�I�u�W�F�N�g�̐���
	if (m_pLoadTerritory == NULL) { m_pLoadTerritory = CLoadTextTerritory::Create(LOAD_TERRITORY); }	//�e���g���[�̓ǂݍ���

	//���I�����[�h��UI��������
	InitCuntrySelect();

	// �ϋq�̐���
	CAudience::Create();

	// ��̐���
	CSky::Load();
	CSky::Create();

	// �����O�`��N���X�𐶐�
	CRingRender::Create();

	// �e���g���[�`��N���X�𐶐�
	CTerritoryRender::Create();
}

//=============================================================================
// �I������
//=============================================================================
void CSelect::Uninit(void)
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

	for (int nCntUI = 0; nCntUI < NUMPLAYER; nCntUI++)
	{
		if (m_pSelectChar[nCntUI] != NULL)
		{
			m_pSelectChar[nCntUI]->Uninit();
			delete m_pSelectChar[nCntUI];
			m_pSelectChar[nCntUI] = NULL;
		}
	}

	//�e�N�X�`���̔j��
	UnloadTexture();
	CSelectCharacter::UnloadTex();

	// ��e�N�X�`���̔j��
	CSky::UnLoad();

	//�t�F�[�h�ȊO�폜
	CScene::ReleaseFade();

	//���S�t���O
	CScene::DeathCheck();
}

//=============================================================================
// �X�V����
//=============================================================================
void CSelect::Update(void)
{
	//�T�E���h�̎擾
	CSound *pSound = CManager::GetSound();

	//�C���v�b�g�̎擾�@
	CGamePad	*pGamePad = CManager::GetInputGamePad();
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();

	//�����̃e�N�X�`���̐؂�ւ�
	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		if (m_pCuntoryName[nCnt] != NULL)
		{
			m_pCuntoryName[nCnt]->BindTexture(m_apTexCuntoryName[m_pNumPlayer[nCnt]->GetNumCuntry()]);
		}
	}

	//�J�����̍X�V����
	if (m_pCamera != NULL) { m_pCamera->Update(); }

	//��Ԃ��Ƃ̍X�V����
	switch (m_state)
	{
	case STATE_CUNTRY_SELECT: UpdateCuntrySelect(); break;			//���I��
	case STATE_CHARACTER_SELECT: UpdateCharacterSelect(); break;	//�L�����I��
	case STATE_CHANGE: UpdateChange(); break;						//���[�h�ؑ�
	case STATE_BACKCHANGE: UpdateBackChange(); break;				//���I����ʂɖ߂�
	case STATE_DICISION: UpdateDicision(); break;					//����
	}

	// �n�ʂ̍X�V����
	if (m_pFieldManager != NULL) { m_pFieldManager->Update(); }

	CScene::DeathCheck();
	CCollider::DeathCheck();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CSelect::Draw(void)
{
	//�J�����̐ݒ�
	if (m_pCamera != NULL) { m_pCamera->Set(); }

	//���ׂĕ`�悷��
	CScene::DrawAll();
}

//=============================================================================
// ���I���̏���������
//=============================================================================
void CSelect::InitCuntrySelect(void)
{
	//���S�̐ݒ�
	if (m_pLogo == NULL) { m_pLogo = CMoveUI::Create(D3DXVECTOR3(640.0f, 50.0f, 0.0f), D3DXVECTOR3(640.0f, 50.0f, 0.0f), m_pTex[0]); }

	//�v���C���[�̑I��\������
	for (int nCnt = NUMPLAYER - 1; nCnt >= 0; nCnt--)
	{
		m_pNumPlayer[nCnt] = CPlayerIcon::Create(D3DXVECTOR3(255.0f, 200.0f, 0.0f), nCnt);
		m_pNumPlayer[nCnt]->BindTexture(m_apTexPlayer[nCnt]);
		m_pNumPlayer[nCnt]->SetDraw(false);
	}

	//�����̕\��
	int nCountFlag = 0;;
	for (int nCntY = 0; nCntY < 2; nCntY++)
	{
		for (int nCntX = 0; nCntX < 4; nCntX++)
		{
			m_pNationalFlag[nCountFlag] = CMoveUI::Create(D3DXVECTOR3(230.0f + (nCntX * MOVE_X), 200.0f + (nCntY * MOVE_Y), 0.0f), FLAG_SIZE, m_apTextureCountry[nCountFlag]);
			nCountFlag++;
		}
	}

	//�t���[���E�G���g���[���Ă��Ȃ��e�N�X�`���̕\��
	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		if (m_pFram[nCnt] == NULL)
		{
			m_pFram[nCnt] = CMoveUI::Create(D3DXVECTOR3(NOENTRY_POS.x + (nCnt * NOENTRY_MOVE), NOENTRY_POS.y, 0.0f), NOENTRY_SIZE, m_pTex[1]);
		}

		if (m_pNoEntry[nCnt] == NULL)
		{
			m_pNoEntry[nCnt] = CMoveUI::Create(D3DXVECTOR3(NOENTRY_POS.x + (nCnt * NOENTRY_MOVE), NOENTRY_POS.y, 0.0f), NOENTRY_SIZE, m_pTexNoEntry);
		}
	}
}

//=============================================================================
// ���I���̍X�V����
//=============================================================================
void CSelect::UpdateCuntrySelect(void)
{
	//���̓f�o�C�X�̎擾
	CGamePad * pGamePad = CManager::GetInputGamePad();
	CInputXPad * pXPad = CManager::GetXPad();
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();
	CRawMouse * pRawMouse = CManager::GetRawMouse();

	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		if (pXPad->GetTrigger(XINPUT_GAMEPAD_B, nCnt) == true)
		{
			PlayerEnter(nCnt, INPUTTYPE_GAMEPAD);
		}

		//�}�E�X�̓��͏���
		if (pRawMouse->GetTrigger(nCnt, CRawMouse::RIMS_BUTTON_RIGHT) == true)
		{
			PlayerEnter(nCnt, INPUTTYPE_MOUSE);
		}
	}

	//�L�[�{�[�h����
	if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
	{
		PlayerEnter(0, INPUTTYPE_KEYBOARD);
		m_nNumControler = 0;
	}
	else if (pKeyboard->GetKeyboardTrigger(DIK_2) == true)
	{
		PlayerEnter(1, INPUTTYPE_GAMEPAD);
		m_nNumControler = 1;
	}
	else if (pKeyboard->GetKeyboardTrigger(DIK_3) == true)
	{
		PlayerEnter(2, INPUTTYPE_GAMEPAD);
		m_nNumControler = 2;
	}
	else if (pKeyboard->GetKeyboardTrigger(DIK_4) == true)
	{
		PlayerEnter(3, INPUTTYPE_GAMEPAD);
		m_nNumControler = 3;
	}

	//�����̕\������
	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		//�����̐���
		if ((m_pCuntoryName[nCnt] == NULL) && (m_pNumPlayer[nCnt] != NULL))
		{
			if (m_pNumPlayer[nCnt]->GetDraw() == false) { continue; }

			//�e�N�X�`���ԍ��̐ݒ�
			int nTexNum = m_pNumPlayer[nCnt]->GetNumCuntry();
			if (nTexNum == -1) { nTexNum = 0; }
			m_pNumPlayer[nCnt]->SetNumCuntry(nTexNum);

			m_pCuntoryName[nCnt] = CMoveUI::Create(D3DXVECTOR3(NAME_POS.x + (nCnt * NAME_MOVE), NAME_POS.y, 0.0f), NAME_SIZE, m_apTexCuntoryName[nTexNum]);

			//NO ENTRY�̔j��
			if (m_pNoEntry[nCnt] != NULL)
			{
				m_pNoEntry[nCnt]->Uninit();
				m_pNoEntry[nCnt] = NULL;
			}
		}
	}
}

//=============================================================================
// ���I���̔j��
//===========================================================================
void CSelect::UninitCuntrySelect(void)
{
	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		//�t���[��
		if(m_pFram[nCnt] != NULL)
		{
			m_pFram[nCnt]->Uninit();
			m_pFram[nCnt] = NULL;
		}

		//NO ENTRY
		if (m_pNoEntry[nCnt] != NULL)
		{
			m_pNoEntry[nCnt]->Uninit();
			m_pNoEntry[nCnt] = NULL;
		}

		//����
		if (m_pCuntoryName[nCnt] != NULL)
		{
			m_pCuntoryName[nCnt]->Uninit();
			m_pCuntoryName[nCnt] = NULL;
		}

		//�v���C���[�̃A�C�R��
		if (m_pNumPlayer[nCnt] != NULL)
		{
			m_pNumPlayer[nCnt]->Uninit();
			m_pNumPlayer[nCnt] = NULL;
		}
	}

	//�����̔j��
	for (int nCnt = 0; nCnt < MAX_COUNTRY; nCnt++)
	{
		if (m_pNationalFlag[nCnt] != NULL)
		{
			m_pNationalFlag[nCnt]->Uninit();
			m_pNationalFlag[nCnt] = NULL;
		}
	}
}

//=============================================================================
// �L�����I���̏���������
//=============================================================================
void CSelect::InitCharacterSelect(void)
{
	//�v���C���[�^�C�v�I�����S�ɐݒ�
	if (m_pLogo != NULL) { m_pLogo->BindTexture(m_pTex[2]); }


	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		//�t���[���E�G���g���[���Ă��Ȃ��e�N�X�`���̕\��
		if (m_pFram[nCnt] == NULL)
		{
			m_pFram[nCnt] = CMoveUI::Create(D3DXVECTOR3(NOENTRY_POS.x + (nCnt * NOENTRY_MOVE), NOENTRY_POS.y, 0.0f), D3DXVECTOR3(40.0f, 40.0f, 0.0f), m_pTex[1]);
		}

		//�L�����w�i�̐���
		if (m_pCharBack[nCnt] == NULL)
		{
			D3DXVECTOR3 Pos = D3DXVECTOR3(CHAR_BACK_POS.x + (nCnt * CHAR_BACK_MOVE), CHAR_BACK_POS.y, 0.0f);		//�ʒu�v�Z

			if (m_bPlayerEnter[nCnt] == true)
			{//�G���g���[���Ă���ꍇ
				m_pCharBack[nCnt] = CMoveUI::Create(Pos, D3DXVECTOR3(40.0f, 40.0f, 0.0f), m_apTextureCountry[m_anPlayeCuntry[nCnt]]);
				m_pCharBack[nCnt]->SetColoer(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f));	//�F�̐ݒ�

				if (m_pSelectChar[nCnt] == NULL)
				{//�L�����I��UI�̐���
					Pos.y -= 5.0f;
					m_pSelectChar[nCnt] = CSelectCharacter::Create(Pos, D3DXVECTOR3(40.0f, 40.0f, 0.0f), m_anPlayeCuntry[nCnt], nCnt, m_anControler[nCnt]);
					m_pSelectChar[nCnt]->SetInputType(m_anInputType[nCnt]);
				}
			}
			else
			{//�G���g���[���Ă��Ȃ��ꍇ
				Pos.y -= 5.0f;
				m_pCharBack[nCnt] = CMoveUI::Create(Pos, D3DXVECTOR3(40.0f, 40.0f, 0.0f), m_pTex[3]);
			}
		}
	}

	//�o�b�N�{�^���̐���
	if (m_pBackButton == NULL) { m_pBackButton = CMoveUI::Create(D3DXVECTOR3(50.0f, 140.0f, 0.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), m_pTex[4]); }
}

//=============================================================================
// �L�����I���̍X�V����
//=============================================================================
void CSelect::UpdateCharacterSelect(void)
{
	//���̓f�o�C�X�̎擾
	CGamePad * pGamePad = CManager::GetInputGamePad();
	CInputXPad * pXPad = CManager::GetXPad();
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();
	CRawMouse * pRawMouse = CManager::GetRawMouse();

	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		//�Q�[���p�b�h���͏���
		if (pXPad->GetPress(XINPUT_GAMEPAD_B, nCnt) == true)
		{
			m_nCntPressTime[nCnt]++;

			if ((m_nCntPressTime[nCnt] % TIME_BACKBOTTUN) == 0)
			{
				SetState(STATE_BACKCHANGE);	//�O��̉�ʂɖ߂�
			}
		}
		else
		{
			m_nCntPressTime[nCnt] = 0;
		}

		//�}�E�X�̓��͏���
		if (pRawMouse->GetPress(nCnt, CRawMouse::RIMS_BUTTON_LEFT) == true)
		{
			m_nCntPressTime2[nCnt]++;

			if ((m_nCntPressTime2[nCnt] % TIME_BACKBOTTUN) == 0)
			{
				SetState(STATE_BACKCHANGE);	//�O��̉�ʂɖ߂�
			}
		}
		else
		{
			m_nCntPressTime2[nCnt] = 0;
		}
	}
	

	//�L�[�{�[�h�̓��͏���
	if (pKeyboard->GetKeyboardTrigger(DIK_B) == true)
	{ SetState(STATE_BACKCHANGE); }	//�O��̉�ʂɖ߂�

	for (int nCntEntry = 0; nCntEntry < m_nEntryPlayer; nCntEntry++)
	{
		if (pXPad->GetTrigger(XINPUT_GAMEPAD_START, nCntEntry) == true)
		{
			SetState(STATE_BACKCHANGE);
		}

		//�}�E�X�̓��͏���
		if (pRawMouse->GetPress(nCntEntry, CRawMouse::RIMS_BUTTON_LEFT) == true)
		{
			SetState(STATE_BACKCHANGE);

		}
	}

	int nCntEnter = 0;;
	for (int nCntNumPlayer = 0; nCntNumPlayer < NUMPLAYER; nCntNumPlayer++)
	{
		if (m_pSelectChar[nCntNumPlayer] != NULL)
		{
			m_pSelectChar[nCntNumPlayer]->Update();

			bool bEnter = m_pSelectChar[nCntNumPlayer]->GetbEnter();
			if (bEnter == true) { nCntEnter++; }	//���肵���l�������Z
		}
	}

	//���萔�������ɂȂ����猈���Ԃɂ���
	if (nCntEnter == m_nEntryPlayer) { SetState(STATE_DICISION); }
}

//=============================================================================
// �L�����I���̔j������
//=============================================================================
void CSelect::UninitCharacterSelect(void)
{
	//�g�又��
	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		//�L�����t���[��
		if (m_pCharBack[nCnt] != NULL)
		{
			m_pCharBack[nCnt]->Uninit();
			m_pCharBack[nCnt] = NULL;
		}

		//�L�����I��UI
		if (m_pSelectChar[nCnt] != NULL)
		{
			m_pSelectChar[nCnt]->UninitUI();	//UI������
			m_pSelectChar[nCnt]->Uninit();
			delete m_pSelectChar[nCnt];
			m_pSelectChar[nCnt] = NULL;
		}

		//�t���[��
		if (m_pFram[nCnt] != NULL)
		{
			m_pFram[nCnt]->Uninit();
			m_pFram[nCnt] = NULL;
		}
	}

	//�{�^��
	if (m_pBackButton != NULL)
	{
		m_pBackButton->Uninit();
		m_pBackButton = NULL;
	}

	//���S�̔j��
	if (m_pLogo != NULL)
	{
		m_pLogo->Uninit();
		m_pLogo = NULL;
	}
}

//=============================================================================
// ���[�h�؂�ւ�����
//=============================================================================
void CSelect::UpdateChange(void)
{
	m_nCountTime++;

	if (m_nCountTime < MOVE_TIME)
	{
		//��ړ�
		if (m_pLogo != NULL) { m_pLogo->UpdateMoveUp(MOVE_UP); }

		//���E�ړ�
		for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
		{
			//�t���[��
			if (m_pFram[nCnt] != NULL)
			{
				if (nCnt < NUMPLAYER / 2)
				{//��
					m_pFram[nCnt]->UpdateMoveLeft(MOVE_RIGHT_LEFT);
				}
				else
				{//�E
					m_pFram[nCnt]->UpdateMoveRight(MOVE_RIGHT_LEFT);
				}
			}

			//NO ENTRY
			if (m_pNoEntry[nCnt] != NULL)
			{
				if (nCnt < NUMPLAYER / 2)
				{//��
					m_pNoEntry[nCnt]->UpdateMoveLeft(MOVE_RIGHT_LEFT);
				}
				else
				{//�E
					m_pNoEntry[nCnt]->UpdateMoveRight(MOVE_RIGHT_LEFT);
				}
			}

			//����
			if (m_pCuntoryName[nCnt] != NULL)
			{
				if (nCnt < NUMPLAYER / 2)
				{//��
					m_pCuntoryName[nCnt]->UpdateMoveLeft(MOVE_RIGHT_LEFT);
				}
				else
				{//�E
					m_pCuntoryName[nCnt]->UpdateMoveRight(MOVE_RIGHT_LEFT);
				}
			}

			//�I�����ꂽ����ۑ�
			if (m_pNumPlayer[nCnt] != NULL) { m_anPlayeCuntry[nCnt] = m_pNumPlayer[nCnt]->GetNumCuntry(); }
		}

		//�������E�ړ�
		for (int nCnt = 0; nCnt < MAX_COUNTRY; nCnt++)
		{
			if (m_pNationalFlag[nCnt] != NULL)
			{
				if (nCnt == 0 || nCnt == 1 || nCnt == 4 || nCnt == 5)
				{//��
					m_pNationalFlag[nCnt]->UpdateMoveLeft(MOVE_RIGHT_LEFT);
				}
				else
				{//�E
					m_pNationalFlag[nCnt]->UpdateMoveRight(MOVE_RIGHT_LEFT);
				}
			}
		}
	}
	else if(m_nCountTime ==  MOVE_TIME)
	{
		UninitCuntrySelect();				//���I���̔j��
	}

	if ((m_nCountTime > MOVE_TIME) && (m_nCountTime < 120))
	{
		InitCharacterSelect();				//�L�����N�^�[�̑I������

		//���ړ�
		if (m_pLogo != NULL) { m_pLogo->UpdateMoveDown(MOVE_DOWN); }

		//�g�又��
		for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
		{
			//�L�����t���[��
			if (m_bPlayerEnter[nCnt] == true)
			{
				m_pCharBack[nCnt]->UpdateScaleUp(CHAR_BACK_SIZE, CHANGE_CHARBACK);
			}
			else
			{
				m_pCharBack[nCnt]->UpdateScaleUp(D3DXVECTOR3(145.0f, 150.0f, 0.0f), CHANGE_CHARBACK2);
			}

			//�t���[��
			if (m_pFram[nCnt] != NULL) { m_pFram[nCnt]->UpdateScaleUp(NOENTRY_SIZE, CHANGE_FLAM); }

			//�L�����I��UI
			if (m_pSelectChar[nCnt] != NULL) { m_pSelectChar[nCnt]->UpdateScaleUp(D3DXVECTOR3(145.0f, 150.0f, 0.0f), CHANGE_CHARBACK2); }
		}

		if (m_pBackButton != NULL) { m_pBackButton->UpdateScaleUp(BACKBUTTON_SIZE, CHANGE_BACKBUTTON); }
	}
	else if (m_nCountTime > 120)
	{//�L�����I�����[�h�ɂ���
		SetState(STATE_CHARACTER_SELECT);
		m_nCountTime = 0;
	}
}

//=============================================================================
// ���̑I�������ɖ߂�
//=============================================================================
void CSelect::UpdateBackChange(void)
{
	//���Ԃ̉Z
	m_nCountTime++;

	if (m_nCountTime < 30)
	{
		//���S�̏�ړ�
		if (m_pLogo != NULL) { m_pLogo->UpdateMoveUp(4.0f); }

		//�k������
		for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
		{
			//�L�����t���[��
			if (m_bPlayerEnter[nCnt] == true)
			{
				m_pCharBack[nCnt]->UpdateScaleDown(FIRST_SIZE, CHANGE_CHARBACK);
			}
			else
			{
				m_pCharBack[nCnt]->UpdateScaleDown(FIRST_SIZE, CHANGE_CHARBACK2);
			}

			//�L�����I��UI
			if (m_pSelectChar[nCnt] != NULL) { m_pSelectChar[nCnt]->UpdateScaleDown(FIRST_SIZE, CHANGE_CHARBACK2); }

			//�t���[��
			if (m_pFram[nCnt] != NULL) { m_pFram[nCnt]->UpdateScaleDown(FIRST_SIZE, CHANGE_FLAM); }
		}

		//�{�^���̏k������
		if (m_pBackButton != NULL) { m_pBackButton->UpdateScaleDown(D3DXVECTOR3(10.0f, 10.0f, 0.0f), CHANGE_BACKBUTTON); }
	}
	else if (m_nCountTime == 30)
	{
		UninitCharacterSelect();	//�L�����N�^�[�I����ʂ̔j��
		BackCuntryInit();			//���I���̃|���S���𐶐�
	}
	else if (m_nCountTime > 30 && m_nCountTime < 70)
	{
		//���ړ�
		if (m_pLogo != NULL) { m_pLogo->UpdateMoveDown(MOVE_UP); }

		//���E�ړ�
		for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
		{
			//�t���[��
			if (m_pFram[nCnt] != NULL)
			{
				if (nCnt < NUMPLAYER / 2)
				{//��
					m_pFram[nCnt]->UpdateMoveRight(MOVE_RIGHT_LEFT_REMAKE);
				}
				else
				{//�E
					m_pFram[nCnt]->UpdateMoveLeft(MOVE_RIGHT_LEFT_REMAKE);
				}
			}

			//NO ENTRY
			if (m_pNoEntry[nCnt] != NULL)
			{
				if (nCnt < NUMPLAYER / 2)
				{//��
					m_pNoEntry[nCnt]->UpdateMoveRight(MOVE_RIGHT_LEFT_REMAKE);
				}
				else
				{//�E
					m_pNoEntry[nCnt]->UpdateMoveLeft(MOVE_RIGHT_LEFT_REMAKE);
				}
			}

			//�v���C���[�̃A�C�R���̈ړ�
			if ((m_pNumPlayer[nCnt] != NULL) && (true == m_pNumPlayer[nCnt]->GetDraw()))
			{
				D3DXVECTOR3 Pos = m_pNumPlayer[nCnt]->GetPos();

				if (Pos.x < SCREEN_WIDTH / 2.0f)
				{
					m_pNumPlayer[nCnt]->UpdateMoveRight(17.0f);
				}
				else
				{
					m_pNumPlayer[nCnt]->UpdateMoveLeft(15.7f);
				}
			}
		}

		//�����̍��E�ړ�
		for (int nCnt = 0; nCnt < MAX_COUNTRY; nCnt++)
		{
			if (m_pNationalFlag[nCnt] != NULL)
			{
				if (nCnt == 0 || nCnt == 1 || nCnt == 4 || nCnt == 5)
				{//�E
					m_pNationalFlag[nCnt]->UpdateMoveRight(MOVE_RIGHT_LEFT_REMAKE);
				}
				else
				{//��
					m_pNationalFlag[nCnt]->UpdateMoveLeft(MOVE_RIGHT_LEFT_REMAKE);
				}
			}
		}
	}
	else if (m_nCountTime == 70)
	{
		SetState(STATE_CUNTRY_SELECT);	//���̑I����Ԃɂ���
		m_nCountTime = 0;				//���Ԃ̏�����
	}
}

//=============================================================================
// ���̑I����ʂ̐���
//=============================================================================
void CSelect::BackCuntryInit(void)
{
	//���S�̐ݒ�
	if (m_pLogo == NULL) { m_pLogo = CMoveUI::Create(D3DXVECTOR3(640.0f, -55.0f, 0.0f), D3DXVECTOR3(640.0f, 50.0f, 0.0f), m_pTex[0]); }

	//�v���C���[�̑I��\������
	for (int nCnt = NUMPLAYER - 1; nCnt >= 0; nCnt--)
	{
		m_pNumPlayer[nCnt] = CPlayerIcon::Create(D3DXVECTOR3(255.0f, 200.0f, 0.0f), nCnt);
		m_pNumPlayer[nCnt]->BindTexture(m_apTexPlayer[nCnt]);		//�e�N�X�`���̊�����
		m_pNumPlayer[nCnt]->SetNumCuntry(m_anPlayeCuntry[nCnt]);	//�I������Ă��鍑��ݒ�
		m_pNumPlayer[nCnt]->SetDraw(false);

		//�`���Ԃɂ���
		if (m_bPlayerEnter[nCnt] == true)
		{
			m_pNumPlayer[nCnt]->SetNumInput(m_anControler[nCnt]);		//�R���g���[���[�̎擾
			m_pNumPlayer[nCnt]->SetInputType(m_anInputType[nCnt]);		//�R���g���[���[�̃^�C�v�擾
			m_pNumPlayer[nCnt]->SetDraw(true);
		}
	}

	//�����̕\��
	int nCountFlag = 0;;
	for (int nCntY = 0; nCntY < 2; nCntY++)
	{
		for (int nCntX = 0; nCntX < 4; nCntX++)
		{
			//�ʒu�ݒ�
			D3DXVECTOR3 Pos = D3DXVECTOR3(-410.0f + (nCntX * MOVE_X), 200.0f + (nCntY * MOVE_Y), 0.0f);
			if (nCntX >= (NUMPLAYER / 2.0f)) { Pos.x += SCREEN_WIDTH; }

			m_pNationalFlag[nCountFlag] = CMoveUI::Create(Pos, FLAG_SIZE, m_apTextureCountry[nCountFlag]);

			//�v���C���[�̃A�C�R���ʒu�����킹��
			for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
			{
				if (m_anPlayeCuntry[nCnt] != nCountFlag) { continue; }
				if (m_pNumPlayer[nCnt] != NULL)
				{
					m_pNumPlayer[nCnt]->SetPos(Pos, m_pNumPlayer[nCnt]->GetOtherPos());
				}
			}

			nCountFlag++;
		}
	}

	//�t���[���E�G���g���[���Ă��Ȃ��e�N�X�`���̕\��
	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		D3DXVECTOR3 Pos = D3DXVECTOR3(NOENTRY_POS_REMAKE.x + (nCnt * NOENTRY_MOVE), NOENTRY_POS_REMAKE.y, 0.0f);

		if (nCnt >= (NUMPLAYER / 2.0f)) { Pos.x += SCREEN_WIDTH; }

		if (m_pFram[nCnt] == NULL)
		{
			m_pFram[nCnt] = CMoveUI::Create(Pos, NOENTRY_SIZE, m_pTex[1]);
		}

		if ((m_pNoEntry[nCnt] == NULL) && (m_bPlayerEnter[nCnt] == false))
		{//�G���g���[��Ԃ���Ȃ�������
			m_pNoEntry[nCnt] = CMoveUI::Create(Pos, NOENTRY_SIZE, m_pTexNoEntry);
		}
	}
}

//=============================================================================
// ���菈��
//=============================================================================
void CSelect::UpdateDicision(void)
{
	if (m_pRedyFight == NULL)
	{ //���f�B�[�g�D�t�@�C�g�𐶐�
		m_pRedyFight = CMoveUI::Create(D3DXVECTOR3(0.0f, 340.0f, 0.0f), D3DXVECTOR3(640.0f, 150.0f, 0.0f), m_pTex[5]);
	}
	else
	{
		m_pRedyFight->UpdateMoveRight(50.0f);
		D3DXVECTOR3 Pos = m_pRedyFight->GetPos();

		if (Pos.x > 680.0f)
		{
			//�ʒu�̍Đݒ�
			Pos.x = 640.0f;
			m_pRedyFight->SetPos(Pos, m_pRedyFight->GetOtherPos());

			//���̓f�o�C�X�̎擾
			CInputXPad * pXPad = CManager::GetXPad();
			CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();
			CRawMouse * pRawMouse = CManager::GetRawMouse();

			for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
			{
				if (pXPad->GetTrigger(XINPUT_GAMEPAD_B, nCnt) == true)
				{
					SetPlayerInfo();	//�v���C���[���̕ۑ�
					if (CFade::GetFadeMode() != CFade::FADE_OUT) { CFade::SetFade(CManager::MODE_GAME); }
				}

				if (pRawMouse->GetTrigger(nCnt, CRawMouse::RIMS_BUTTON_RIGHT) == true)
				{
					SetPlayerInfo();	//�v���C���[���̕ۑ�
					if (CFade::GetFadeMode() != CFade::FADE_OUT) { CFade::SetFade(CManager::MODE_GAME); }
				}
			}

			if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
			{
				if (CFade::GetFadeMode() != CFade::FADE_OUT)
				{
					SetPlayerInfo();	//�v���C���[���̕ۑ�
					CFade::SetFade(CManager::MODE_GAME);
				}
			}
		}
	}
}

//=============================================================================
// �G���g���[����
//=============================================================================
void CSelect::PlayerEnter(int nCntPlayer, INPUTTYPE InputType)
{
	//�f�o�C�X���G���g���[���Ă��邩�ǂ���
	if (InputType == INPUTTYPE_GAMEPAD)
	{
		if (m_bEntryGamePad[nCntPlayer]) { return; }
		m_bEntryGamePad[nCntPlayer] = true;
	}
	else if (InputType == INPUTTYPE_MOUSE)
	{
		if (m_bEntryMouse[nCntPlayer]) { return; }
		m_bEntryMouse[nCntPlayer] = true;
	}
	else if (InputType == INPUTTYPE_KEYBOARD)
	{
		if (m_bEntryKeyboard) { return; }
		m_bEntryKeyboard = true;
	}

	//�G���g���[���̉��Z
	if (false == m_pNumPlayer[m_nEntryPlayer]->GetDraw())
	{
		//�G���g���[���Ă����Ԃɂ���
		m_bPlayerEnter[m_nEntryPlayer] = true;

		//�R���g���[���[�̔ԍ���ۑ�
		m_anControler[m_nEntryPlayer] = nCntPlayer;

		//���̓f�o�C�X�̃^�C�v�ݒ�
		m_anInputType[m_nEntryPlayer] = InputType;

		//�v���C���[�A�C�R���E�����̕\��
		m_pNumPlayer[m_nEntryPlayer]->SetDraw(true);
		m_pNumPlayer[m_nEntryPlayer]->SetNumInput(nCntPlayer);
		m_pNumPlayer[m_nEntryPlayer]->SetInputType(InputType);

		//�����̉��Z
		m_nEntryPlayer++;
	}
}

//=============================================================================
// �v���C���[���̕ۑ�
//=============================================================================
void CSelect::SetPlayerInfo(void)
{
	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		if (m_pSelectChar[nCnt] != NULL)
		{
			m_anPlayeCuntry[nCnt] = m_pSelectChar[nCnt]->GetCuntry();
			m_anPlayerType[nCnt] = m_pSelectChar[nCnt]->GetType();
		}
	}
}

//=============================================================================
// �e�N�X�`���̓ǂݍ��ݏ���
//=============================================================================
void CSelect::LoadTexture(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�����̃e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, FLAG_JAPAN, &m_apTextureCountry[0]);			//���{
	D3DXCreateTextureFromFile(pDevice, FLAG_RUSSIA, &m_apTextureCountry[1]);		//���V�A
	D3DXCreateTextureFromFile(pDevice, FLAG_AMERICA, &m_apTextureCountry[2]);		//�A�����J
	D3DXCreateTextureFromFile(pDevice, FLAG_BRAZIL, &m_apTextureCountry[3]);		//�u���W��
	D3DXCreateTextureFromFile(pDevice, FLAG_ENGLAND, &m_apTextureCountry[4]);		//�C�M���X
	D3DXCreateTextureFromFile(pDevice, FLAG_ITALY, &m_apTextureCountry[5]);			//�C�^���A
	D3DXCreateTextureFromFile(pDevice, FLAG_NEWZEALAND, &m_apTextureCountry[6]);	//�j���[�W�[�����h
	D3DXCreateTextureFromFile(pDevice, FLAG_SOUTHAFRICA, &m_apTextureCountry[7]);	//��A�t���J

	//�v���C���[�̑I���摜
	D3DXCreateTextureFromFile(pDevice, SELECT_1P, &m_apTexPlayer[0]);	//1P
	D3DXCreateTextureFromFile(pDevice, SELECT_2P, &m_apTexPlayer[1]);	//2P
	D3DXCreateTextureFromFile(pDevice, SELECT_3P, &m_apTexPlayer[2]);	//3P
	D3DXCreateTextureFromFile(pDevice, SELECT_4P, &m_apTexPlayer[3]);	//4P

	//�����̓ǂݍ��ݏ���
	D3DXCreateTextureFromFile(pDevice, NAME_JAPAN, &m_apTexCuntoryName[0]);			//���{
	D3DXCreateTextureFromFile(pDevice, NAME_RUSSIA, &m_apTexCuntoryName[1]);		//���V�A
	D3DXCreateTextureFromFile(pDevice, NAME_AMERICA, &m_apTexCuntoryName[2]);		//�A�����J
	D3DXCreateTextureFromFile(pDevice, NAME_BRAZIL, &m_apTexCuntoryName[3]);		//�u���W��
	D3DXCreateTextureFromFile(pDevice, NAME_ENGLAND, &m_apTexCuntoryName[4]);		//�C�M���X
	D3DXCreateTextureFromFile(pDevice, NAME_ITALY, &m_apTexCuntoryName[5]);			//�C�^���A
	D3DXCreateTextureFromFile(pDevice, NAME_NEWZEALAND, &m_apTexCuntoryName[6]);	//�j���[�W�[�����h
	D3DXCreateTextureFromFile(pDevice, NAME_SOUTHAFRICA, &m_apTexCuntoryName[7]);	//��A�t���J

	//�G���g���[���Ă��Ȃ��e�N�X�`���̓ǂݍ��ݏ���
	D3DXCreateTextureFromFile(pDevice, NO_ENTRY, &m_pTexNoEntry);			//���{

	//���̑��̃e�N�X�`��
	D3DXCreateTextureFromFile(pDevice, LOGO, &m_pTex[0]);			//���S
	D3DXCreateTextureFromFile(pDevice, FLAM, &m_pTex[1]);			//�t���[��
	D3DXCreateTextureFromFile(pDevice, LOGO2, &m_pTex[2]);			//�v���C���[�^�C�v�I�����S
	D3DXCreateTextureFromFile(pDevice, NO_ENTRY_CHR, &m_pTex[3]);	//�L�����N�^�[��NOENTRY
	D3DXCreateTextureFromFile(pDevice, BACK_CUNTRY, &m_pTex[4]);	//�L�����N�^�[��NOENTRY
	D3DXCreateTextureFromFile(pDevice, RADY_TO_FIGHT, &m_pTex[5]);	//���f�B�[�g�D�t�@�C�g
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CSelect::UnloadTexture(void)
{
	//�����e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < MAX_COUNTRY; nCnt++)
	{
		if (m_apTextureCountry[nCnt] != NULL)
		{
			m_apTextureCountry[nCnt]->Release();
			m_apTextureCountry[nCnt] = NULL;
		}
	}

	//�v���C���[�̃A�C�R��
	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		if (m_apTexPlayer[nCnt] != NULL)
		{
			m_apTexPlayer[nCnt]->Release();
			m_apTexPlayer[nCnt] = NULL;
		}
	}

	//�����e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < MAX_COUNTRY; nCnt++)
	{
		if (m_apTexCuntoryName[nCnt] != NULL)
		{
			m_apTexCuntoryName[nCnt]->Release();
			m_apTexCuntoryName[nCnt] = NULL;
		}
	}

	//�G���g���[���Ă��Ă��Ȃ�
	if (m_pTexNoEntry != NULL)
	{
		m_pTexNoEntry->Release();
		m_pTexNoEntry = NULL;
	}

	//���S�e�N�X�`��
	for (int nCnt = 0; nCnt < SELECT_TEX; nCnt++)
	{
		if (m_pTex[nCnt] != NULL)
		{
			m_pTex[nCnt]->Release();
			m_pTex[nCnt] = NULL;
		}
	}
}