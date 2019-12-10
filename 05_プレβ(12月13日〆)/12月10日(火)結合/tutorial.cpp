//=============================================================================
//
// �`���[�g���A���̏��� [tutorial.cpp]
// Author : ���� ���� & �R����
//
//=============================================================================
#include "tutorial.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "loadText.h"
#include "camera.h"
#include "fieldmanager.h"
#include "loadText.h"
#include "territory.h"
#include "RawMouse.h"
#include "moveUI.h"
#include "loadEffect.h"
#include "tutorialplayer.h"
#include "line.h"
#include "territoryRender.h"
#include "ringRender.h"
#include "mine.h"
#include "effectManager.h"
#include "ring.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BG_POS	(D3DXVECTOR3(640.0f, 350.0f, 0.0f))
#define BG_SIZE	(D3DXVECTOR3(640.0f, 370.0f, 0.0f))
#define FIRST_POS	(D3DXVECTOR3(1800.0f, 360.0f, 0.0f))
#define MIDDLE_POS	(D3DXVECTOR3(640.0f, 360.0f, 0.0f))
#define SLIDEOUT_POS	(D3DXVECTOR3(-600.0f, 360.0f, 0.0f))
#define MOVE		(50.0f) //50

#define LOGO_SIZE	(D3DXVECTOR3(400.0f, 60.0f, 0.0f))
#define TEXT00_POS	(D3DXVECTOR3(640.0f, 180.0f, 0.0f))
#define TEXT00_SIZE	(D3DXVECTOR3(400.0f, 60.0f, 0.0f))
#define TEXT01_POS	(D3DXVECTOR3(650.0f, 450.0f, 0.0f))
#define TEXT01_SIZE	(D3DXVECTOR3(500.0f, 120.0f, 0.0f))
#define TEXTEND_SIZE (D3DXVECTOR3(500.0f, 160.0f, 0.0f))
#define WINDOW_POS	 (D3DXVECTOR3(640.0f, 350.0f, 0.0f))
#define WINDOW_POS1	 (D3DXVECTOR3(850.0f, 340.0f, 0.0f))
#define WINDOW_SIZE	(D3DXVECTOR3(300.0f, 170.0f, 0.0f))
#define CHANG_COL	(0.025f)			//0.025
#define FADEOUT_TIME (240)				//240
#define GETTERRITORY00_TIME (60 * 7)	//7
#define GETTERRITORY01_TIME (60 * 16)	//16
#define GETTERRITORY02_TIME (60 * 20)	//20
#define DEFFOLT_TIME		(60 * 5)	//5
#define CHAR_TIME		(60 * 6)		//6
#define CHAR_TIME1		(60 * 11)		//11
#define CHAR_TIME2		(60 * 18)		//18


//�w�i�e�N�X�`���̃p�X
#define TEX_BG_001	("data/TEXTURE/Tutorial/BG/0.png")
#define TEX_BG_002	("data/TEXTURE/Tutorial/BG/1_mine.png")
#define TEX_BG_003	("data/TEXTURE/Tutorial/BG/1_pawer.png")
#define TEX_BG_004	("data/TEXTURE/Tutorial/BG/1_speed.png")
#define TEX_BG_005	("data/TEXTURE/Tutorial/BG/2.png")
#define TEX_BG_006	("data/TEXTURE/Tutorial/BG/3.png")
#define TEX_BG_007	("data/TEXTURE/Tutorial/BG/4.png")
#define TEX_BG_008	("data/TEXTURE/Tutorial/BG/5.png")
#define TEX_BG_009	("data/TEXTURE/Tutorial/BG/6.png")
#define TEX_BG_010	("data/TEXTURE/Tutorial/BG/7.png")

//�e�L�X�g�̃e�N�X�`���p�X
#define TEX_TURORIAL		("data/TEXTURE/Tutorial/Logo.png")
#define TEX_WIN				("data/TEXTURE/Tutorial/��������.png")
#define TEX_WIN_TEXT1		("data/TEXTURE/Tutorial/�e�L�X�g.png")
#define TEX_WIN_TEXT2		("data/TEXTURE/Tutorial/�e�L�X�g1.png")
#define TEX_GETTERRITORIY	("data/TEXTURE/Tutorial/�̒n�̎���.png")
#define TEX_CONTROL			("data/TEXTURE/Tutorial/������@.png")
#define TEX_CHARACTER		("data/TEXTURE/Tutorial/�I��Љ�.png")
#define TEX_END				("data/TEXTURE/Tutorial/�I��.png")

//�ǂݍ��ރe�L�X�g�t�@�C����
#define LOAD_TERRITORY	("data/TEXT/Territory.txt")
#define LOAD_EFFECT	("data/TEXT/EFFECT/Particle.txt")
#define EFFECTMANAGER_FILENAME ("data/TEXT/EFFECT/ringEffect.txt")

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CTutorial::CTutorial(){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTutorial::~CTutorial(){}

//=============================================================================
// ����������
//=============================================================================
void CTutorial::Init(void)
{
	//�x�[�X�̏�����
	CModeBase::Init();

	CSound *pSound = CManager::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_BGM005);

	//�ϐ��̏�����
	m_pBG = NULL;
	m_pTitle = NULL;
	m_pText00 = NULL;
	m_pText01 = NULL;
	m_pWindow = NULL;
	m_pLoadTerritory = NULL;
	m_pCamera = NULL;
	m_pFieldManager = NULL;
	m_pLoadEffect = NULL;
	m_pCharTutorial = NULL;
	m_pTerritoryRender = NULL;
	m_pEffectManager = NULL;
	m_pCharDamege = NULL;
	m_state = STATE_FIRST;
	m_nCountTime = 0;
	m_bMoveSlideTitle = false;
	m_bSlideOut = false;

	//�e�N�X�`���̏�����
	for (int nCnt = 0; nCnt < TEX_TUTORIAL_BG; nCnt++)
	{
		m_pTextureBG[nCnt] = NULL;
	}

	//�e�N�X�`���̏�����
	for (int nCnt = 0; nCnt < TEX_TUTORIAL; nCnt++)
	{
		m_pTextureText[nCnt] = NULL;
	}

	
	LoadTex();						//�e�N�X�`���̓ǂݍ���
	CreateMultRenderingTex();		//�}���`�����_�����O�p�e�N�X�`���̐���
	SetObjTypeList();				//�I�u�W�F�N�g�^�C�v�̃��X�g�ݒ�
	CMine::Load();					//�n���̓ǂݍ���

	//�G�t�F�N�g�̓ǂݍ���
	if (m_pLoadEffect == NULL)
	{
		m_pLoadEffect = CLoadTextEffect::Create(LOAD_EFFECT);
		CParticle::LoadTexture();
		CEffectTool::LoadEffect();
	}

#if _DEBUG
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
		m_pCamera->SetPosV(D3DXVECTOR3(0.0f, 500.0f, -180.0f));
	}
#endif

	//�t�B�[���h�̐���
	if (m_pFieldManager == NULL)
	{
		m_pFieldManager = new CFieldManager;
		m_pFieldManager->Init();
	}

	//�����O�̑������������@
	CRing::ResetMaxRing();

	//�e���g���[�̐���
	if (m_pLoadTerritory == NULL) { m_pLoadTerritory = CLoadTextTerritory::Create(LOAD_TERRITORY); }	//�e���g���[

	//�L�����N�^�[�̐���
	if (m_pCharTutorial == NULL)
	{
		m_pCharTutorial = CTutorialPlayer::Create(D3DXVECTOR3(10.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt", 0, CCharacter::TYPE_JAPAN, CCharacter::CHARCTERTYPE_SPEED);
	}

	// �����O�`��N���X�𐶐�
	m_pRingRender = CRingRender::Create();

	// �e���g���[�`��N���X�𐶐�
	m_pTerritoryRender = CTerritoryRender::Create();

	// �G�t�F�N�g�Ǌ��N���X�𐶐�
	m_pEffectManager = CEffectManager::Create(EFFECTMANAGER_FILENAME);
}

//=============================================================================
// �I������
//=============================================================================
void CTutorial::Uninit(void)
{
	//�x�[�X�̔j��
	CModeBase::Uninit();

	//�e�N�X�`���̔j��
	if (m_pTextureMT != NULL)
	{
		m_pTextureMT->Release();
		m_pTextureMT = NULL;
	}

	//�����_���[�̔j��
	if (m_pRenderMT != NULL)
	{
		m_pRenderMT->Release();
		m_pRenderMT = NULL;
	}

	//�o�b�t�@�̔j��
	if (m_pBuffMT != NULL)
	{
		m_pBuffMT->Release();
		m_pBuffMT = NULL;
	}

	//�e���g���[�̔j��
	if (m_pLoadTerritory != NULL)
	{
		m_pLoadTerritory->Uninit();
		delete m_pLoadTerritory;
		m_pLoadTerritory = NULL;
	}

	//�G�t�F�N�g�̔j��
	if (m_pLoadEffect != NULL)
	{
		CEffectTool::UnloadEffect();
		CParticle::UnloadTexture();

		m_pLoadEffect->Uninit();
		delete m_pLoadEffect;
		m_pLoadEffect = NULL;
	}

	//�t�B�[���h�}�l�[�W���[�̔j��
	if (m_pFieldManager != NULL)
	{
		m_pFieldManager->Uninit();
		delete m_pFieldManager;
		m_pFieldManager = NULL;
	}

	if (m_pCamera != NULL)
	{//�J�����̔j��
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	//�e�N�X�`���̔j��
	UnloadTex();
	CMine::UnLoad();

	if (m_pTerritoryRender != NULL)
	{//�e���g���[�����_���[�̔j��
		m_pTerritoryRender = NULL;
	}

	if (m_pRingRender != NULL)
	{//�����O�����_���[�̔j��
		m_pRingRender = NULL;
	}

	// �G�t�F�N�g�Ǌ��N���X�̔j��
	if (m_pEffectManager != NULL)
	{
		m_pEffectManager->Uninit();
		delete m_pEffectManager;
		m_pEffectManager = NULL;
	}

	//�t�F�[�h�ȊO�폜
	CScene::ReleaseFade();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTutorial::Update(void)
{
	//�C���v�b�g�̎擾�@
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();
	CInputXPad * pXPad = CManager::GetXPad();
	CRawMouse *pRawMouse = CManager::GetRawMouse();					//RawMouse�̎擾
	
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || pXPad->GetTrigger(XINPUT_GAMEPAD_A, nCnt) == true ||
			pRawMouse->GetTrigger(nCnt, CRawMouse::RIMS_BUTTON_LEFT) == true)
		{
			if (CFade::GetFadeMode() == CFade::FADE_NONE) { CFade::SetFade(CManager::MODE_TITLE); }
		}
	}

	//��Ԃ��Ƃ̍X�V����
	switch (m_state)
	{
	case STATE_FIRST: UpdateFirst(); break;					 //�`��
	case STATE_WINCONDITIONS: UpdateWin(); break;			 //��������
	case STATE_GETTERRITORY1: 								 //�e���g���[�̎擾�P
	case STATE_GETTERRITORY2: UpdateGetTerritory(); break;	 //�e���g���[�̎擾2
	case STATE_CUTLINE: UpdateCutLine(); break;				 //���C����؂�
	case STATE_POINT: UpdateGetPoint(); break;				 //�|�C���g
	case STATE_CONTROLER: UpdateControler(); break;			 //�R���g���[���[
	case STATE_CHARACTER: UpdateCharacter(); break;			 //�L�����N�^�[
	case STATE_END: UpdateEnd(); break;
	}

	//�t�B�[���h�}�l�[�W���[�̍X�V
	if (m_pFieldManager != NULL) { m_pFieldManager->Update(); }

	//�J�����̍X�V����
	if (m_pCamera != NULL) { m_pCamera->Update(); }

	//�e���g���[�����_���[�̍X�V
	if (m_pTerritoryRender != NULL) { m_pTerritoryRender->Update(); }

	//�����O�����_���[�̍X�V
	if (m_pRingRender != NULL) { m_pRingRender->Update(); }
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTutorial::Draw(void)
{
	if (m_pCamera != NULL) { m_pCamera->Set(); }
	DrawMultRendering();
	CScene::DrawAll();
}

//=============================================================================
// �\����Ԃ̍X�V����
//=============================================================================
void CTutorial::UpdateFirst(void)
{
	if (m_pBG == NULL)
	{//�w�i�̐���
		m_pBG = CMoveUI::Create(BG_POS, BG_SIZE, m_pTextureBG[0]);
	}

	if (m_pTitle == NULL)
	{//�`���[�g���A�����S�̐���
		m_pTitle = CMoveUI::Create(FIRST_POS, LOGO_SIZE, m_pTextureText[0]);
	}
	else
	{//���S�܂ňړ�
		MoveTitle(STATE_WINCONDITIONS);
	}
}

//=============================================================================
// ���������̍X�V����
//=============================================================================
void CTutorial::UpdateWin(void)
{
	//�ϐ��錾
	bool bFadeOut = false;

	if (m_pTitle == NULL)
	{//�`���[�g���A�����S�̐���
		m_pTitle = CMoveUI::Create(FIRST_POS, LOGO_SIZE, m_pTextureText[1]);
	}
	else
	{
		m_bSlideOut = MoveSlideTitle();
	}

	if (m_bSlideOut)
	{
		//�^�C�g�����S�̔j��
		DeleteTitle();

		//���Ԃ̉��Z
		m_nCountTime++;

		if (m_pText00 == NULL)
		{//�e�L�X�g�̐���
			m_pText00 = CMoveUI::Create(TEXT00_POS, TEXT00_SIZE, m_pTextureText[3]);
			m_pText00->SetColoer(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
		else
		{
			bFadeOut = TextFadeInOut(m_pText00, FADEOUT_TIME, m_nCountTime);
		}

		if (m_nCountTime > 80)
		{//�e�L�X�g����
			if (m_pText01 == NULL)
			{
				m_pText01 = CMoveUI::Create(TEXT01_POS, TEXT01_SIZE, m_pTextureText[2]);
				m_pText01->SetColoer(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			}
			else
			{
				bFadeOut = TextFadeInOut(m_pText01, FADEOUT_TIME, m_nCountTime);
			}
		}

		if (bFadeOut)
		{//�t�F�[�h�A�E�g��ԂɂȂ�����
			m_nCountTime = 0;
			m_bSlideOut = false;

			if (m_pText00 != NULL)
			{//�e�L�X�g�̔j��
				m_pText00->Uninit();
				m_pText00 = NULL;
			}

			if (m_pText01 != NULL)
			{//�e�L�X�g�̔j��
				m_pText01->Uninit();
				m_pText01 = NULL;
			}

			//�e���g���[�̎擾��Ԃɂ���
			m_state = STATE_GETTERRITORY1;
		}
	}
}

//=============================================================================
// �e���g���[�̎擾�����̍X�V����
//=============================================================================
void CTutorial::UpdateGetTerritory(void)
{
	if (m_pTitle == NULL)
	{
		m_pTitle = CMoveUI::Create(FIRST_POS, LOGO_SIZE, m_pTextureText[4]);
	}
	else
	{
		m_bSlideOut = MoveSlideTitle();
	}

	if (m_bSlideOut)
	{
		//�^�C�g�����S�̔j��
		DeleteTitle();

		//���Ԃ̉��Z
		m_nCountTime++;

		if (m_nCountTime >= GETTERRITORY01_TIME)
		{//���C����؂��Ԃɂ���
			if (m_nCountTime == GETTERRITORY01_TIME)
			{//�ړ����Ă��Ȃ���Ԃɂ���
				if (m_pCharTutorial != NULL)
				{//�v���C���[��j��
					CTutorialPlayer * pTutorialPlayer = (CTutorialPlayer *)m_pCharTutorial;
					pTutorialPlayer->SetMoveEnd(false);
					pTutorialPlayer->ResetList();
					pTutorialPlayer->SetPos(D3DXVECTOR3(-360.0f, 0.0f, 20.0f));
					pTutorialPlayer->SetRot(INITIALIZE_VECTOR3);
					CTerritory::ResetColorTerritory();		//�e���g���[������������

					//���C���̐���
					CLine::Create(D3DXVECTOR3(-220.0f, 0.0f, -20.0f), D3DXVECTOR3(-250.0f, 0.0f, 150.0f), 1, 0);
					CLine::Create(D3DXVECTOR3(-250.0f, 0.0f, 150.0f), D3DXVECTOR3(-220.0f, 0.0f, 320.0f), 1, 0);
					CLine::Create(D3DXVECTOR3(-220.0f, 0.0f, 320.0f), D3DXVECTOR3(100.0f, 0.0f, 320.0f), 1, 0);
				}

				if (m_pCharTutorial != NULL)
				{//�L�����N�^�[���ړ���Ԃɂ���
					CTutorialPlayer * pTutorialPlayer = (CTutorialPlayer *)m_pCharTutorial;
					if (pTutorialPlayer->GetMoveEnd() == false) { pTutorialPlayer->SetMovePlayer(true, 2); }
				}
			}

			m_pBG->BindTexture(m_pTextureBG[6]);
			m_state = STATE_CUTLINE;
			m_nCountTime = 0;
			m_bSlideOut = false;
		}
		else if (m_nCountTime >= GETTERRITORY00_TIME)
		{//BG�e�N�X�`���̓��ւ�

			if (m_nCountTime == GETTERRITORY00_TIME)
			{//�ړ����Ă��Ȃ���Ԃɂ���
				if (m_pCharTutorial != NULL)
				{//�v���C���[��j��
					CTutorialPlayer * pTutorialPlayer = (CTutorialPlayer *)m_pCharTutorial;
					pTutorialPlayer->SetMoveEnd(false);
					pTutorialPlayer->SetPos(D3DXVECTOR3(0.0f, 0.0f, -30.0f));
					pTutorialPlayer->SetRot(INITIALIZE_VECTOR3);
					pTutorialPlayer->ResetList();
					CTerritory::ResetColorTerritory();		//�e���g���[������������
				}
			}

			if (m_pCharTutorial != NULL)
			{//�L�����N�^�[���ړ���Ԃɂ���
				CTutorialPlayer * pTutorialPlayer = (CTutorialPlayer *)m_pCharTutorial;
				if (pTutorialPlayer->GetMoveEnd() == false) { pTutorialPlayer->SetMovePlayer(true, 1); }
			}

			m_pBG->BindTexture(m_pTextureBG[5]);
		}
		else if (m_nCountTime > 0)
		{//BG�e�N�X�`���̓��ւ�
			CreateMultiWindow(WINDOW_POS, WINDOW_SIZE);

			if (m_pCharTutorial != NULL)
			{//�L�����N�^�[���ړ���Ԃɂ���
				CTutorialPlayer * pTutorialPlayer = (CTutorialPlayer *)m_pCharTutorial;
				if (pTutorialPlayer->GetMoveEnd() == false){ pTutorialPlayer->SetMovePlayer(true, 0); }
			}

			m_pBG->BindTexture(m_pTextureBG[4]);
		}
	}
}

//=============================================================================
// ���C����؂��Ԃ̍X�V����
//=============================================================================
void CTutorial::UpdateCutLine(void)
{
	//���Ԃ̉��Z
	m_nCountTime++;

	if (m_nCountTime > /*DEFFOLT_TIME*/ 300)
	{
		m_pBG->BindTexture(m_pTextureBG[7]);
		m_state = STATE_POINT;
		m_nCountTime = 0;
		DeleteMultiWindow();
		DeleteCharacter();
	}
}

//=============================================================================
// �|�C���g�̐����̍X�V����
//=============================================================================
void CTutorial::UpdateGetPoint(void)
{
	//���Ԃ̉��Z
	m_nCountTime++;

	if (m_nCountTime > DEFFOLT_TIME)
	{
		m_pBG->BindTexture(m_pTextureBG[0]);
		m_state = STATE_CONTROLER;
		m_nCountTime = 0;
	}
}

//=============================================================================
// �R���g���[���[�̍X�V����
//=============================================================================
void CTutorial::UpdateControler(void)
{
	if (m_pTitle == NULL)
	{
		m_pTitle = CMoveUI::Create(FIRST_POS, LOGO_SIZE, m_pTextureText[5]);
	}
	else
	{
		m_bSlideOut = MoveSlideTitle();
	}

	if (m_bSlideOut)
	{
		//�^�C�g���̔j��
		DeleteTitle();

		//���Ԃ̉��Z
		m_nCountTime++;

		if (m_nCountTime > DEFFOLT_TIME)
		{
			m_pBG->BindTexture(m_pTextureBG[0]);
			m_nCountTime = 0;
			m_bSlideOut = false;
			m_state = STATE_CHARACTER;
		}
		else if (m_nCountTime > 0)
		{
			m_pBG->BindTexture(m_pTextureBG[8]);
		}
	}
}

//=============================================================================
// �L�����N�^�[���̍X�V����
//=============================================================================
void CTutorial::UpdateCharacter(void)
{
	if (m_pTitle == NULL)
	{
		m_pTitle = CMoveUI::Create(FIRST_POS, LOGO_SIZE, m_pTextureText[6]);
	}
	else
	{
		m_bSlideOut = MoveSlideTitle();
	}

	if (m_bSlideOut)
	{
		//�^�C�g�����S�̔j��
		DeleteTitle();

		//���Ԃ̉��Z
		m_nCountTime++;

		if (m_nCountTime > CHAR_TIME2)
		{
			m_pBG->BindTexture(m_pTextureBG[0]);
			m_nCountTime = 0;
			m_bSlideOut = false;
			m_state = STATE_END;
			DeleteMultiWindow();
		}
		else if (m_nCountTime >= CHAR_TIME1)
		{
			if (m_nCountTime == CHAR_TIME1) { DeleteCharacter(); }

			if (m_pCharTutorial == NULL)
			{//�X�s�[�h�^�̂̐���
				m_pCharTutorial = CTutorialSpeedPlayer::Create(INITIALIZE_VECTOR3, 0, 0);
			}

			m_pBG->BindTexture(m_pTextureBG[3]);
		}
		else if (m_nCountTime >= CHAR_TIME)
		{
			if (m_nCountTime == CHAR_TIME) { DeleteCharacter(); }

			if (m_pCharTutorial == NULL)
			{//�p���[�^�̂̐���
				m_pCharTutorial = CTutorialPowerPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 500.0f), 0, 0);
			}

			if (m_pCharDamege == NULL)
			{//�_���[�W�L�����̐���
				m_pCharDamege = CTutorialSpeedPlayer::Create(D3DXVECTOR3(600.0f, 0.0f, 150.0f), 1, 2);
				CTutorialPlayer * pPlayer = (CTutorialPlayer *)m_pCharDamege;
				pPlayer->SetMovePlayer(true, 7);	//�ړ����[�g�̐ݒ�
			}
			m_pBG->BindTexture(m_pTextureBG[2]);
		}
		else if (m_nCountTime > 0)
		{
			if (m_pCharTutorial == NULL)
			{//�n���^�̐���
				m_pCharTutorial = CTutorialTechniquePlayer::Create(D3DXVECTOR3(-40.0f, 0.0f, 0.0f), 0, 0);
			}

			if (m_pCharDamege == NULL)
			{//�_���[�W�L�����̐���
				m_pCharDamege = CTutorialSpeedPlayer::Create(D3DXVECTOR3(250.0f, 0.0f, 600.0f), 1, 2);
				CTutorialPlayer * pPlayer = (CTutorialPlayer *)m_pCharDamege;
				pPlayer->SetMovePlayer(true, 5);	//�ړ����[�g�̐ݒ�
			}

			CreateMultiWindow(WINDOW_POS1, WINDOW_SIZE);
			m_pBG->BindTexture(m_pTextureBG[1]);
		}
	}
}

//=============================================================================
// �I����Ԃ̍X�V����
//=============================================================================
void CTutorial::UpdateEnd(void)
{
	//�ϐ��錾
	bool bFadeOut = false;

	if (m_pTitle == NULL)
	{
		m_pTitle = CMoveUI::Create(MIDDLE_POS, TEXTEND_SIZE, m_pTextureText[7]);
		m_pTitle->SetColoer(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	}
	else
	{
		//���Ԃ̉��Z
		m_nCountTime++;

		bFadeOut = TextFadeInOut(m_pTitle, FADEOUT_TIME, m_nCountTime);

		if (bFadeOut)
		{//�t�F�[�h�A�E�g����
			if (CFade::GetFadeMode() != CFade::FADE_OUT) { CFade::SetFade(CManager::MODE_TITLE); }
		}
	}
}

//=============================================================================
// �^�C�g�����S�̔j��
//=============================================================================
void CTutorial::DeleteTitle(void)
{
	if (m_pTitle != NULL)
	{//�^�C�g�����S�̔j��
		m_pTitle->Uninit();
		m_pTitle = NULL;
	}
}

//=============================================================================
//	�^�C�g�����S�̈ړ�
//=============================================================================
void CTutorial::MoveTitle(STATE NextState)
{
	//�ϐ��錾
	D3DXCOLOR col;

	m_pTitle->UpdateMoveDest(MIDDLE_POS, MOVE);

	if (m_pTitle->GetPos().x == MIDDLE_POS.x)
	{//�����ʒu�ɂȂ����甼�����ɂ���
		m_nCountTime++;

		if (m_nCountTime > 60)
		{
			col = m_pTitle->GetColor();
			col.a -= CHANG_COL;

			if (col.a < 0.0f)
			{//�F�̏C��
				col.a = 0.0f;
				m_nCountTime = 0;		//�^�C�}�[�̏�����
				m_state = NextState;	//����������Ԃɂ���
			}

			//�F��ݒ肷��
			m_pTitle->SetColoer(col);
		}
	}

	//�^�C�g�����S�̔j��
	if (m_state == NextState) { DeleteTitle(); }
}

//=============================================================================
// �^�C�g�����S�@�X���C�h�p
//=============================================================================
bool CTutorial::MoveSlideTitle(void)
{
	//�ϐ��錾
	bool bSetPotision = false;

	if (!m_bMoveSlideTitle)
	{
		//���S�܂ňړ�
		m_pTitle->UpdateMoveDest(MIDDLE_POS, MOVE);

		if (m_pTitle->GetPos().x == MIDDLE_POS.x)
		{//�ړ��t���O��L���ɂ���
			m_bMoveSlideTitle = true;
		}
	}
	else
	{
		m_nCountTime++;

		if (m_nCountTime > 60)
		{//��ʊO�ֈړ�
			m_pTitle->UpdateMoveDest(SLIDEOUT_POS, MOVE);

			if (m_pTitle->GetPos().x == SLIDEOUT_POS.x)
			{
				m_nCountTime = 0;
				m_bMoveSlideTitle = false;
				bSetPotision = true;
			}
		}
	}

	return bSetPotision;
}

//=============================================================================
// �e�L�X�g�̃t�F�[�h�C���A�E�g�̏���
//=============================================================================
bool CTutorial::TextFadeInOut(CMoveUI * pText, int nFadeOutTime, int nCountTime)
{
	//�ϐ��錾
	D3DXCOLOR col = INITIALIZE_VECTOR4;
	bool bFadeOut = false;

	if (pText != NULL)
	{
		//�F�̎擾
		col = pText->GetColor();

		if (nFadeOutTime > nCountTime)
		{//�t�F�[�h�C������
			col.a += CHANG_COL;

			if (col.a > 1.0f) { col.a = 1.0f; }
		}
		else
		{//�t�F�[�h�A�E�g����
			col.a -= CHANG_COL;

			if (col.a < 0.0f)
			{
				col.a = 0.0f;
				bFadeOut = true;
			}
		}

		//�F�̐ݒ�
		pText->SetColoer(col);
	}

	return bFadeOut;
}

//=============================================================================
// �E�C���h�E�̐���
//=============================================================================
void CTutorial::CreateMultiWindow(D3DXVECTOR3 Pos, D3DXVECTOR3 Size)
{
	if (m_pWindow == NULL)
	{
		m_pWindow = CMoveUI::Create(Pos, Size, m_pTextureMT);
	}
}

//=============================================================================
// �E�C���h�E�̔j��
//=============================================================================
void  CTutorial::DeleteMultiWindow(void)
{
	if (m_pWindow != NULL)
	{
		m_pWindow->Uninit();
		m_pWindow = NULL;
	}
}

//=============================================================================
//	�L�����N�^�[�̔j��
//=============================================================================
void CTutorial::DeleteCharacter(void)
{
	//�`���[�g���A���L�����̔j��
	if (m_pCharTutorial != NULL)
	{
		m_pCharTutorial->ResetList();	//�e���g���[�̏�����
		m_pCharTutorial->UninitLine();	//���C���̏�����
		m_pCharTutorial->Uninit();
		m_pCharTutorial = NULL;
	}

	//�_���[�W�L�����̔j��
	if (m_pCharDamege != NULL)
	{
		m_pCharDamege->ResetList();	//�e���g���[�̏�����
		m_pCharDamege->UninitLine();	//���C���̏�����
		m_pCharDamege->Uninit();
		m_pCharDamege = NULL;
	}
}

//=============================================================================
// �}���`�����_�����O�p�̃e�N�X�`���𐶐�����
//=============================================================================
void CTutorial::CreateMultRenderingTex(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	LPDIRECT3DSURFACE9 pRenderOrg = NULL;
	LPDIRECT3DSURFACE9 pBuffOrg = NULL;
	m_pTextureMT = NULL;
	m_pBuffMT = NULL;
	m_pRenderMT = NULL;

	// Texture�̍쐬
	pDevice->CreateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTextureMT, NULL);

	// �����_���[�̎擾
	m_pTextureMT->GetSurfaceLevel(0, &m_pRenderMT);

	// �o�b�t�@�̐���
	pDevice->CreateDepthStencilSurface(SCREEN_WIDTH, SCREEN_HEIGHT, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pBuffMT, NULL);

	pDevice->GetRenderTarget(0, &pRenderOrg);		// ���݂̃����_���[�^�[�Q�b�g��ۑ�
	pDevice->GetDepthStencilSurface(&pBuffOrg);		// ���݂̃o�b�t�@��ۑ�
	pDevice->SetRenderTarget(0, m_pRenderMT);		// �����_���[�̐ݒ�
	pDevice->SetDepthStencilSurface(m_pBuffMT);		// �o�b�t�@�̐ݒ�
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// ���ɖ߂�
	pDevice->SetRenderTarget(0, pRenderOrg);		// �����_���[
	pDevice->SetDepthStencilSurface(pBuffOrg);		// �o�b�t�@

	// �r���[�|�[�g�̏�����
	m_viewportMT.X = 0;						// X�̐ݒ�
	m_viewportMT.Y = 0;						// Y�̐ݒ�
	m_viewportMT.Width = SCREEN_WIDTH;		// ���̐ݒ�
	m_viewportMT.Height = SCREEN_HEIGHT;	// �����̐ݒ�
	m_viewportMT.MinZ = 0.0f;				// ��O
	m_viewportMT.MaxZ = 1.0f;				// ��

	//���_�A�����_�̐ݒ�
	m_PosV = D3DXVECTOR3(-50.0f, 575.0f, 75.0f);
	m_PosR = D3DXVECTOR3(-50.0f, 0.0f, 140.0f);
}

//=============================================================================
// �}���`�����_�����O�p�̕`�揈��
//=============================================================================
void CTutorial::DrawMultRendering(void)
{
	//�ϐ��錾
	PDIRECT3DSURFACE9 pRenderOrg = NULL;	//�����_���[�̕ۑ��p�ϐ�
	LPDIRECT3DSURFACE9 pBuffOrg = NULL;	//�o�b�t�@�̕ۑ��p�ϐ�
	D3DVIEWPORT9 viewportOrg;
	D3DXMATRIX  mtxProjectionOrg;	//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX  mtxViewOrg;			//�r���[�}�g���N�X
	D3DXMATRIX  mtxProjection;		//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX  mtxView;			//�r���[�}�g���N�X

	//�f�o�C�X�̎擾
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//���݂̏�Ԃ�ۑ�����
	pDevice->GetViewport(&viewportOrg);							//�r���[�|�[�g�̎擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxViewOrg);				//�r���[�}�g���b�N�X�̎擾
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProjectionOrg);	//�v���W�F�N�V�����}�g���b�N�X�̎擾
	pDevice->GetRenderTarget(0, &pRenderOrg);
	pDevice->GetDepthStencilSurface(&pBuffOrg);

	//�ݒ菈��
	pDevice->SetRenderTarget(0, m_pRenderMT);
	pDevice->SetDepthStencilSurface(m_pBuffMT);

	//�^�[�Q�b�g�EZ�o�b�t�@�̃N���A
	pDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	pDevice->SetViewport(&m_viewportMT);	//�r���[�|�[�g�̐ݒ�

	D3DXMatrixIdentity(&mtxView);			//�r���[�}�g���b�N�X�̏�����

											// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&mtxView, &m_PosV, &m_PosR, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	// �r���[�}�g���b�N�X��ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		2500.0f);										//�J�����̕`��͈�

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);

	//�����ŕ`�悷��
	for (int nCntLayer = 0; nCntLayer < MAX_LAYER; nCntLayer++)
	{
		CScene * pSceneTop = CScene::GetTop(nCntLayer); //�I�u�W�F�N�g�|�C���^�̐擪���擾

		if (pSceneTop != NULL)
		{
			//�g�b�v�̈ʒu��ۑ�����
			CScene * pScene = pSceneTop;

			while (pScene != NULL)
			{
				CScene * pSceneNext = pScene->GetpNext();	//���̃I�u�W�F�N�g�̃|�C���^��ۑ�����
				CScene::OBJTYPE ObjType = pScene->GetObjType();

				for (int nCntObjType = 0; nCntObjType < MAX_OBJTYPE; nCntObjType++)
				{
					if (ObjType == m_ObjTypeList[nCntObjType])
					{//���X�g���ɓ���̃I�u�W�F�N�g����������`�悷��
						pScene->Draw();
						break;
					}
				}
				pScene = pSceneNext;					//�|�C���^��i�߂�
			}
		}
	}

	//�f�t�H���g�ɖ߂�
	pDevice->SetViewport(&viewportOrg);							//�r���[�|�[�g�̎擾
	pDevice->SetTransform(D3DTS_VIEW, &mtxViewOrg);				//�r���[�}�g���b�N�X�̎擾
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjectionOrg);	//�v���W�F�N�V�����}�g���b�N�X�̎擾
	pDevice->SetRenderTarget(0, pRenderOrg);
	pDevice->SetDepthStencilSurface(pBuffOrg);
}

//=============================================================================
// �I�u�W�F�N�g�^�C�v�̃��X�g
//=============================================================================
void CTutorial::SetObjTypeList(void)
{
	m_ObjTypeList[0] = CScene::OBJTYPE_TERRITORY;
	m_ObjTypeList[1] = CScene::OBJTYPE_FIELDRENDER;
	m_ObjTypeList[2] = CScene::OBJTYPE_FLOR;
	m_ObjTypeList[3] = CScene::OBJTYPE_RING;
	m_ObjTypeList[4] = CScene::OBJTYPE_PARTICLE;
	m_ObjTypeList[5] = CScene::OBJTYPE_TRIANGLE;
	m_ObjTypeList[6] = CScene::OBJTYPE_LINE;
	m_ObjTypeList[7] = CScene::OBJTYPE_PLAYER;
	m_ObjTypeList[8] = CScene::OBJTYPE_TERRITORYRENDER;
	m_ObjTypeList[9] = CScene::OBJTYPE_RINGRENDER;
	m_ObjTypeList[10] = CScene::OBJTYPE_MINE;
	m_ObjTypeList[11] = CScene::OBJTYPE_RINGEMITTER;
	m_ObjTypeList[11] = CScene::OBJTYPE_RINGEFFECT;
}

//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
void CTutorial::LoadTex(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�w�i�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEX_BG_001, &m_pTextureBG[0]);
	D3DXCreateTextureFromFile(pDevice, TEX_BG_002, &m_pTextureBG[1]);
	D3DXCreateTextureFromFile(pDevice, TEX_BG_003, &m_pTextureBG[2]);
	D3DXCreateTextureFromFile(pDevice, TEX_BG_004, &m_pTextureBG[3]);
	D3DXCreateTextureFromFile(pDevice, TEX_BG_005, &m_pTextureBG[4]);
	D3DXCreateTextureFromFile(pDevice, TEX_BG_006, &m_pTextureBG[5]);
	D3DXCreateTextureFromFile(pDevice, TEX_BG_007, &m_pTextureBG[6]);
	D3DXCreateTextureFromFile(pDevice, TEX_BG_008, &m_pTextureBG[7]);
	D3DXCreateTextureFromFile(pDevice, TEX_BG_009, &m_pTextureBG[8]);
	D3DXCreateTextureFromFile(pDevice, TEX_BG_010, &m_pTextureBG[9]);

	//�e�L�X�g�e�N�X�`�����̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEX_TURORIAL, &m_pTextureText[0]);
	D3DXCreateTextureFromFile(pDevice, TEX_WIN, &m_pTextureText[1]);
	D3DXCreateTextureFromFile(pDevice, TEX_WIN_TEXT1, &m_pTextureText[2]);
	D3DXCreateTextureFromFile(pDevice, TEX_WIN_TEXT2, &m_pTextureText[3]);
	D3DXCreateTextureFromFile(pDevice, TEX_GETTERRITORIY, &m_pTextureText[4]);
	D3DXCreateTextureFromFile(pDevice, TEX_CONTROL, &m_pTextureText[5]);
	D3DXCreateTextureFromFile(pDevice, TEX_CHARACTER, &m_pTextureText[6]);
	D3DXCreateTextureFromFile(pDevice, TEX_END, &m_pTextureText[7]);
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CTutorial::UnloadTex(void)
{
	for (int nCnt = 0; nCnt < TEX_TUTORIAL_BG; nCnt++)
	{//�w�i�̃e�N�X�`���̔j��
		if (m_pTextureBG[nCnt] != NULL)
		{
			m_pTextureBG[nCnt]->Release();
			m_pTextureBG[nCnt] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < TEX_TUTORIAL; nCnt++)
	{//�e�L�X�g�̃e�N�X�`���̔j��
		if (m_pTextureText[nCnt] != NULL)
		{
			m_pTextureText[nCnt]->Release();
			m_pTextureText[nCnt] = NULL;
		}
	}
}