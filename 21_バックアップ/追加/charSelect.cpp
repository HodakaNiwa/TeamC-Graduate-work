//=============================================================================
//
// �L�����I���̏��� [charselect.cpp]
// Author : ���� �����@[ SATO ASUMI ]
//
//=============================================================================
#include "charSelect.h"
#include "moveUI.h"
#include "charMultRendering.h"
#include "Player.h"
#include "manager.h"
#include "input.h"
#include "gamepad.h"
#include "RawMouse.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEX_TYPENAME	("data/TEXTURE/Select/TypeName.png")
#define TEX_ENTER		("data/TEXTURE/Select/CharEnter.png")
#define MOUSE_MOVE		(15.0f)

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CSelectCharacter::m_pTexture[SELECTCHAR_TEX] = {};

//=============================================================================
// ��������
//=============================================================================
CSelectCharacter * CSelectCharacter::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nCuntry, int nNumPlayer, int nNumControler)
{
	CSelectCharacter *pSelectChar = new CSelectCharacter;
	pSelectChar->Init(pos, size, nCuntry, nNumPlayer, nNumControler);

	return pSelectChar;
}

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CSelectCharacter::CSelectCharacter() {}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CSelectCharacter::~CSelectCharacter() {}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSelectCharacter::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nCuntry, int nNumPlayer, int nNumControler)
{
	//�ϐ��̏�����
	m_pMoveUI = NULL;
	m_pTypeName = NULL;
	m_pStatus = NULL;
	m_pStatusTex = NULL;				//�X�e�[�^�X�̃e�N�X�`��
	m_pEnter = NULL;					//�G���^�[�L�[�����������ǂ���
	m_nCuntry = nCuntry;				//���ԍ�
	m_nType = 0;						//�^�C�v�ԍ�
	m_nNumPlayer = nNumPlayer;			//�v���C���[�ԍ�
	m_nNumControler = nNumControler;	//�R���g���[���[�ԍ�
	m_state = STATE_SPEED;				//���݂̏��
	m_nCountTime = 0;					//���Ԃ̉��Z
	m_nOldType = 0;						//�O��̎���
	m_bEnter = false;					//�L���������肵�����ǂ���

	//�}���`�����_�����O�p�e�N�X�`���̐���
	LoadMultRendering();

	//�L�����\��
	if (m_pMoveUI == NULL) { m_pMoveUI = CMoveUI::Create(pos, size, m_pCharMultRender[m_nType]->GetTexture()); }

	//�^�C�v���̕\��
	if (m_pTypeName == NULL)
	{
		m_pTypeName = CMoveUI::Create(D3DXVECTOR3(170.0f + (nNumPlayer * 315.0f), 530.0f, 0.0f), D3DXVECTOR3(10.0f, 1.0f, 0.0f), m_pTexture[0]);
		m_pTypeName->SetTexUV(0.0f, 1.0f, 0.0f, 0.25f);
	}

	//�X�e�[�^�X�\�̐���
	if (m_pStatus == NULL)
	{
		m_pStatus = CMoveUI::Create(D3DXVECTOR3(170.0f + (nNumPlayer * 315.0f), 628.0f, 0.0f), D3DXVECTOR3(90.0f, 13.0f, 0.0f), m_pStatusTex);
		m_pStatus->SetTexUV(0.0f, 1.0f, 0.0f, 0.25f);
	}
	
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void  CSelectCharacter::Uninit(void)
{
	//�������̏�����
	for (int nCnt = 0; nCnt < MAX_TYPE; nCnt++)
	{
		m_pCharMultRender[nCnt] = NULL;
	}

	m_pMoveUI = NULL;
	m_pTypeName = NULL;
	m_pStatus = NULL;
	m_pEnter = NULL;

	//�X�e�[�^�X�e�N�X�`���̔j��
	if (m_pStatusTex != NULL)
	{
		m_pStatusTex->Release();
		m_pStatusTex = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void  CSelectCharacter::Update(void)
{
	//�O��̃^�C�v��ۑ�
	m_nOldType = m_nType;

	//���͏���
	UpdateInput();

	//�G���^�[�L�[�̐��� / �j��
	if (m_bEnter)
	{// ����
		D3DXVECTOR3 Size = m_pMoveUI->GetSize();
		Size.y += 5.0f;
		if (m_pEnter == NULL) { m_pEnter = CMoveUI::Create(m_pMoveUI->GetPos(), Size, m_pTexture[1]); }
	}
	else
	{//	�j��
		if (m_pEnter != NULL)
		{
			m_pEnter->Uninit();
			m_pEnter = NULL;
		}
	}

	switch (m_state)
	{
	case STATE_SPEED:
		m_nType = 0;

		if ((m_pMoveUI != NULL) && (m_pTypeName != NULL) && (m_pStatus != NULL))
		{//�e�N�X�`����UV�̐ݒ�
			m_pMoveUI->BindTexture(m_pCharMultRender[m_nType]->GetTexture()); 
			m_pTypeName->SetTexUV(0.0f, 1.0f, 0.0f, 0.25f);
			m_pStatus->SetTexUV(0.0f, 1.0f, 0.0f, 0.25f);
		}
		break;

	case STATE_PAWER:
		m_nType = 1;

		if ((m_pMoveUI != NULL) && (m_pTypeName != NULL) && (m_pStatus != NULL))
		{//�e�N�X�`����UV�̐ݒ�
			m_pMoveUI->BindTexture(m_pCharMultRender[m_nType]->GetTexture());
			m_pTypeName->SetTexUV(0.0f, 1.0f, 0.25f, 0.5f);
			m_pStatus->SetTexUV(0.0f, 1.0f, 0.25f, 0.5f);
		}
		break;

	case STATE_MINE:
		m_nType = 2;

		if ((m_pMoveUI != NULL) && (m_pTypeName != NULL) && (m_pStatus != NULL))
		{//�e�N�X�`����UV�̐ݒ�
			m_pMoveUI->BindTexture(m_pCharMultRender[m_nType]->GetTexture());
			m_pTypeName->SetTexUV(0.0f, 1.0f, 0.5f, 0.75f);
			m_pStatus->SetTexUV(0.0f, 1.0f, 0.5f, 0.75f);
		}
		break;

	case STATE_RANDUM:	//�����_���̎�
		m_nCountTime++;

		if ((m_nCountTime % 5) == 0)
		{
			m_nType += 1;
			m_nType = m_nType % MAX_TYPE;
		}

		//�e�N�X�`�������蓖�Ă�
		if ((m_pMoveUI != NULL) && (m_pTypeName != NULL) && (m_pStatus != NULL))
		{
			m_pMoveUI->BindTexture(m_pCharMultRender[m_nType]->GetTexture());
			m_pTypeName->SetTexUV(0.0f, 1.0f, 0.75f, 1.0f);
			m_pStatus->SetTexUV(0.0f, 1.0f, 0.75f, 1.0f);
		}
	}
}

//=============================================================================
// X�p�b�h�̓��͏���
//=============================================================================
void CSelectCharacter::UpdateInput(void)
{
	//���̓f�o�C�X�̎擾
	CInputXPad * pXPad = CManager::GetXPad();
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();
	CRawMouse * pRawMouse = CManager::GetRawMouse();

	if (m_InputType == CSelect::INPUTTYPE_GAMEPAD)
	{
		if ((pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_RIGHT, m_nNumControler) == true) || (pKeyboard->GetKeyboardTrigger(DIK_RIGHT) == true))
		{//�E�ړ�
			MoveRight();
		}
		else if ((pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_LEFT, m_nNumControler) == true) || (pKeyboard->GetKeyboardTrigger(DIK_LEFT) == true))
		{//���ړ�
			MoveLeft();
		}

		//���菈��
		if ((pXPad->GetTrigger(XINPUT_GAMEPAD_A, m_nNumControler) == true) || (pKeyboard->GetKeyboardTrigger(DIK_SPACE)) == true)
		{
			PressEnter();
		}
	}
	else
	{
		//�}�E�X�̊p�x���擾
		int MouseX = pRawMouse->GetMouse(m_nNumControler).lLastX;
		int MouseY = pRawMouse->GetMouse(m_nNumControler).lLastY;

		if ((MouseX > MOUSE_MOVE) || (pKeyboard->GetKeyboardTrigger(DIK_RIGHT) == true))
		{//�E�ړ�
			MoveRight();
		}
		else if ((MouseX < -MOUSE_MOVE) || (pKeyboard->GetKeyboardTrigger(DIK_LEFT) == true))
		{//���ړ�
			MoveLeft();
		}

		//���菈��
		if ((pRawMouse->GetTrigger(m_nNumControler, CRawMouse::RIMS_BUTTON_LEFT) == true) || (pKeyboard->GetKeyboardTrigger(DIK_SPACE)) == true)
		{
			PressEnter();
		}
	}
}

//=============================================================================
// �g�又��
//=============================================================================
void CSelectCharacter::UpdateScaleUp(D3DXVECTOR3 AimSize, D3DXVECTOR2 Move)
{
	if (m_pMoveUI != NULL) { m_pMoveUI->UpdateScaleUp(AimSize, Move); }
	if (m_pTypeName != NULL) { m_pTypeName->UpdateScaleUp(D3DXVECTOR3(70.0f, 30.0f, 0.0f), Move); }
	if(m_pStatus != NULL){ m_pStatus->UpdateScaleUp(D3DXVECTOR3(150.0f, 75.0f, 0.0f), Move); }
}

//=============================================================================
// �k������
//=============================================================================
void CSelectCharacter::UpdateScaleDown(D3DXVECTOR3 AimSize, D3DXVECTOR2 Move)
{
	if (m_pMoveUI != NULL) { m_pMoveUI->UpdateScaleDown(AimSize, Move); }
	if(m_pTypeName != NULL){ m_pTypeName->UpdateScaleDown(D3DXVECTOR3(10.0f, 1.0f, 0.0f), Move); }
	if (m_pStatus != NULL) { m_pStatus->UpdateScaleDown(D3DXVECTOR3(25.0f, 25.0f, 0.0f), Move); }
}

//=============================================================================
// ���ړ�
//=============================================================================
void CSelectCharacter::MoveLeft(void)
{
	int nState = m_state - 1;

	if (nState < 0) { nState = (int)STATE_RANDUM; }
	if (m_state == STATE_MAX) { nState = (int)STATE_MINE; }

	m_state = (STATE)nState;
	m_bEnter = false;
}

//=============================================================================
// �E�ړ�
//=============================================================================
void CSelectCharacter::MoveRight(void)
{
	int nState = m_state + 1;

	if ((nState % (int)STATE_MAX) == 0) { nState = 0; }
	if (m_state == STATE_MAX) { nState = 0; }

	m_state = (STATE)nState;
	m_bEnter = false;
}

//=============================================================================
// ���菈��
//=============================================================================
void CSelectCharacter::PressEnter(void)
{
	if (m_state == STATE_RANDUM)
	{//�����_�����~�߂�
		m_state = (STATE)m_nType;
	}
	else
	{
		m_bEnter = true;	//�����Ԃɂ���
	}
}

//=============================================================================
// UI�̏���
//=============================================================================
void CSelectCharacter::UninitUI(void)
{
	//�L�����N�^�[�̃}���`�����_�����O������
	for (int nCnt = 0; nCnt < MAX_TYPE; nCnt++)
	{
		if (m_pCharMultRender[nCnt] != NULL)
		{
			m_pCharMultRender[nCnt]->Uninit();
			m_pCharMultRender[nCnt] = NULL;
		}
	}

	//�|���S��������
	if (m_pMoveUI != NULL)
	{
		m_pMoveUI->Uninit();
		m_pMoveUI = NULL;
	}

	//�^�C�v�̖��O�̔j��
	if (m_pTypeName != NULL)
	{
		m_pTypeName->Uninit();
		m_pTypeName = NULL;
	}

	//�X�e�[�^�X�̔j��
	if (m_pStatus != NULL)
	{
		m_pStatus->Uninit();
		m_pStatus = NULL;
	}

	//����e�N�X�`���̔j��
	if (m_pEnter != NULL)
	{
		m_pEnter->Uninit();
		m_pEnter = NULL;
	}
}

//=============================================================================
// �}���`�����_�����O�p�e�N�X�`���̓ǂݍ���
//=============================================================================
void CSelectCharacter::LoadMultRendering(void)
{
	//�ϐ��̏�����
	for (int nCnt = 0; nCnt < MAX_TYPE; nCnt++)
	{
		m_pCharMultRender[nCnt] = NULL;
	}

	switch (m_nCuntry)
	{
	case CPlayer::TYPE_JAPAN:	//���{
		m_pCharMultRender[0] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		m_pCharMultRender[1] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		m_pCharMultRender[2] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\TEQPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\TEQPLAYER_MOTION.txt");
		break;

	case CPlayer::TYPE_RUSSIA:	//���V�A
		m_pCharMultRender[0] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		m_pCharMultRender[1] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		m_pCharMultRender[2] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\TEQPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\TEQPLAYER_MOTION.txt");
		break;

	case CPlayer::TYPE_AMERICA:	//�A�����J
		m_pCharMultRender[0] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		m_pCharMultRender[1] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		m_pCharMultRender[2] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\TEQPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\TEQPLAYER_MOTION.txt");
		break;

	case CPlayer::TYPE_BRAZIL:	//�u���W��
		m_pCharMultRender[0] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		m_pCharMultRender[1] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		m_pCharMultRender[2] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\TEQPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\TEQPLAYER_MOTION.txt");
		break;

	case CPlayer::TYPE_ENGLAND:	//�C�M���X
		m_pCharMultRender[0] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		m_pCharMultRender[1] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		m_pCharMultRender[2] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\TEQPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\TEQPLAYER_MOTION.txt");
		break;

	case CPlayer::TYPE_ITALY:	//�C�^���A
		m_pCharMultRender[0] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		m_pCharMultRender[1] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		m_pCharMultRender[2] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\TEQPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\TEQPLAYER_MOTION.txt");
		break;

	case CPlayer::TYPE_NEWZEALAND:	//�j���[�W�[�����h
		m_pCharMultRender[0] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		m_pCharMultRender[1] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		m_pCharMultRender[2] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\TEQPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\TEQPLAYER_MOTION.txt");
		break;

	case CPlayer::TYPE_SOUTHAFRICA:	//��A�t���J
		m_pCharMultRender[0] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		m_pCharMultRender[1] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		m_pCharMultRender[2] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\TEQPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\TEQPLAYER_MOTION.txt");
		break;
	}
}


//=============================================================================
// �e�N�X�`���̓ǂݍ��ݏ���
//=============================================================================
void CSelectCharacter::LoadTex(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�����̃e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEX_TYPENAME, &m_pTexture[0]);	//�^�C�v��
	D3DXCreateTextureFromFile(pDevice, TEX_ENTER, &m_pTexture[1]);	//�L��������
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CSelectCharacter::UnloadTex(void)
{
	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < SELECTCHAR_TEX; nCntTex++)
	{
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}