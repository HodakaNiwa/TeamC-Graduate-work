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
#define PLAYERNAME			("data/TEXTURE/result/PlayerNumber.png")

//��ʂ�������Ԏ��̃}�N��
#define FLAG_WHIGHTSIZE		(D3DXVECTOR3(130.0f, 100.0f, 0.0f))
#define FLAG_AIMSIZE		(D3DXVECTOR3(75.0f, 60.0f, 0.0f))

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
	m_state = STATE_WHITE;
	m_bWhiteIn = false;

	for (int nCnt = 0; nCnt < CUNTRY; nCnt++)
	{
		m_pCuntryTex[nCnt] = NULL;
	}
	for (int nCnt = 0; nCnt < RESULT_TEX; nCnt++)
	{
		m_pTexture[nCnt] = NULL;
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

	//�L�����̐���
	m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");

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
	if (m_pNatinalFlag == NULL) { m_pNatinalFlag = CMoveUI::Create(D3DXVECTOR3(640.0f, 360.0f, 0.0f), D3DXVECTOR3(140.0f, 110.0f, 0.0f), m_pCuntryTex[0]); }

	//������
	m_nCounter = 0;
	m_nTime = 0;
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

	m_pWinChar = NULL;
	m_pNatinalFlag = NULL;

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
	m_pNatinalFlag->UpdateMoveDest(D3DXVECTOR3(140.0f, 90.0f, 0.0f), 8.0f);

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
		m_pWinerName = CMoveUI::Create(D3DXVECTOR3(350.0f, 600.0f, 0.0f), D3DXVECTOR3(300.0f, 100.0f, 0.0f), m_pTexture[0]);
		m_pWinerName->SetTexUV(0.0f, 1.0f, 0.0f, 0.0833f);
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
	if (m_pWinerName != NULL) { m_pWinerName->UpdateMoveDest(D3DXVECTOR3(300.0f, 350.0f, 0.0f), 20.0f); }
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
	D3DXCreateTextureFromFile(pDevice, PLAYERNAME, &m_pTexture[0]);		//�v���C���[�l�[���̓ǂݍ���
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