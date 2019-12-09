//=============================================================================
//
// �v���C���[�̃A�C�R���̏��� [playericon.cpp]
// Author : ���������@SATOU ASUMI
//
//=============================================================================
#include "playericon.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
#include "logo.h"
#include "scene2D.h"
#include "select.h"
#include "moveUI.h"
#include "fade.h"
#include "playericon.h"
#include "RawMouse.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define VECTOR_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	//�x�N�g���̏�����
#define SIZE			(D3DXVECTOR3(130.0f, 75.0f, 0.0f))
#define WIDTH_DIVISION	(4)		//���̕�����
#define HIGHT_DIVISION	(2)		//�c�̕�����
#define MAX_CUNTORY		(8)		//���̍ő吔
#define ADDPOS_X		(25.0f)	//X�̒ǉ���
#define CUNTRY_SIZE		(D3DXVECTOR3(115.0f,  70.0f, 0.0f))	//�����̃T�C�Y
#define CUNTRY_POS		(D3DXVECTOR3(170.0f, 630.0f, 0.0f))
#define ADD_X			(310.0f)
#define MOUSE_MOVE		(15.0f)

CPlayerIcon::STATE CPlayerIcon::m_state = CPlayerIcon::STATE_NORMAL;

//=============================================================================
// ��������
//=============================================================================
CPlayerIcon * CPlayerIcon::Create(D3DXVECTOR3 Pos, int nNumPlayer)
{
	CPlayerIcon * pPlayerIcon = new CPlayerIcon;
	pPlayerIcon->Init();
	pPlayerIcon->Set(Pos, SIZE);
	pPlayerIcon->SetNumPlayer(nNumPlayer);

	return pPlayerIcon;
}

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CPlayerIcon::CPlayerIcon() : CScene2D(6, CScene::OBJTYPE_UI) {}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayerIcon::CPlayerIcon(int nPriority, CScene::OBJTYPE type) : CScene2D(nPriority, type){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPlayerIcon::~CPlayerIcon(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayerIcon::Init(void)
{
	CScene2D::Init();
	m_nNumCuntry = -1;
	m_nOldNumCuntry = 0;
	m_pNationalFlag = NULL;
	m_state = STATE_NORMAL;
	m_nNumInput = -1;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPlayerIcon::Uninit()
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPlayerIcon::Update()
{
	if (GetDraw() == false) { return; }

	if (m_state == STATE_NORMAL)
	{
		UpdateInputXpad();		//�A�C�R���I������
		UpdateInputKeyboard();
		UpdateInputMouse();
		UpdatePos();			//�ʒu�̍X�V����
	}
	else
	{
		UpdateMove();	//�ړ�����
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlayerIcon::Draw()
{
	CScene2D::Draw();
}

//=============================================================================
// ���̑I������(XPad)
//=============================================================================
void CPlayerIcon::UpdateInputXpad(void)
{
	if (m_nNumInput == -1 || m_InputType != CSelect::INPUTTYPE_GAMEPAD) { return; }		//�R���g���[���[���o�^����Ă��Ȃ��Ȃ�X�V���Ȃ�

	//�T�E���h�̎擾
	CSound *pSound = CManager::GetSound();
	CGamePad * pGamePad = CManager::GetInputGamePad();				//�Q�[���p�b�h�̎擾
	CInputXPad * pXPad = CManager::GetXPad();

	if (pXPad->GetStick(0, m_nNumInput) == true)
	{
		m_nOldNumCuntry = m_nNumCuntry;									//�O��̏���ۑ�����

		if (m_pNationalFlag != NULL)
		{
			m_pNationalFlag->Uninit();
			m_pNationalFlag = NULL;
		}
	}

	if (pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_UP, m_nNumInput) == true
		|| pXPad->GetRepeat(CInputXPad::XPADOTHER_STICK_L_UP, m_nNumInput) == true)
	{//��ړ�
	 //�Z���N�g��
		pSound->PlaySound(CSound::SOUND_LABEL_SE009);

		if (m_nNumCuntry < WIDTH_DIVISION)
		{
			m_nNumCuntry += WIDTH_DIVISION;
		}
		else
		{
			m_nNumCuntry -= WIDTH_DIVISION;
		}
	}
	else if (pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_RIGHT, m_nNumInput) == true
		|| pXPad->GetRepeat(CInputXPad::XPADOTHER_STICK_L_RIGHT, m_nNumInput) == true)
	{//�E�ړ�
	 //�Z���N�g��
		pSound->PlaySound(CSound::SOUND_LABEL_SE009);

		m_nNumCuntry += 1;

		if (m_nNumCuntry <= WIDTH_DIVISION)
		{
			if ((m_nNumCuntry % WIDTH_DIVISION) == 0) { m_nNumCuntry = 0; }
		}
		else
		{
			if ((m_nNumCuntry % MAX_CUNTORY) == 0) { m_nNumCuntry = MAX_CUNTORY / 2; }
		}
	}
	else if (pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_DOWN, m_nNumInput) == true
		|| pXPad->GetRepeat(CInputXPad::XPADOTHER_STICK_L_DOWN, m_nNumInput) == true)
	{//���ړ�
	 //�Z���N�g��
		pSound->PlaySound(CSound::SOUND_LABEL_SE009);

		if (m_nNumCuntry < WIDTH_DIVISION)
		{
			m_nNumCuntry += WIDTH_DIVISION;
		}
		else
		{
			m_nNumCuntry -= WIDTH_DIVISION;
		}
	}
	else if (pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_LEFT, m_nNumInput) == true
		|| pXPad->GetRepeat(CInputXPad::XPADOTHER_STICK_L_LEFT, m_nNumInput) == true)
	{//���ړ�
	 //�Z���N�g��
		pSound->PlaySound(CSound::SOUND_LABEL_SE009);
		m_nNumCuntry -= 1;

		if (m_nNumCuntry < 0)
		{
			m_nNumCuntry = WIDTH_DIVISION - 1;
		}
		else if (m_nNumCuntry == WIDTH_DIVISION - 1)
		{
			m_nNumCuntry = MAX_CUNTORY - 1;
		}
	}

	//����{�^������������
	if (pXPad->GetTrigger(XINPUT_GAMEPAD_B, m_nNumInput) == true)
	{
		if (CFade::FADE_OUT != CFade::GetFadeMode())
		{//�t�F�[�h�A�E�g�̎�����Ȃ������特��炷
		 //�Z���N�g��
			pSound->PlaySound(CSound::SOUND_LABEL_SE007);
		}

		//�����̐���
		if (m_pNationalFlag == NULL)
		{
			//�e�N�X�`���̊�����
			LPDIRECT3DTEXTURE9 pTexture = CManager::GetSelect()->GetCuntryTex(m_nNumCuntry);

			//�����̐���
			m_pNationalFlag = CMoveUI::Create(D3DXVECTOR3(CUNTRY_POS.x + (ADD_X * m_nNumPlayer), CUNTRY_POS.y, 0.0f), CUNTRY_SIZE, pTexture);
		}
		else
		{
			CManager::GetSelect()->SetState(CSelect::STATE_CHANGE);
			m_state = STATE_MOVE;	//�ړ���Ԃɂ���
		}
	}
}

//=============================================================================
// ���̑I������(�L�[�{�[�h)
//=============================================================================
void CPlayerIcon::UpdateInputKeyboard(void)
{
	int nNumControl = CManager::GetSelect()->GetNumControler();
	if (nNumControl != m_nNumPlayer) { return; }

	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();	//�L�[�{�[�h�̎擾

	if (pKeyboard->GetKeyboardTrigger(DIK_W) == true
		|| pKeyboard->GetKeyboardRepeat(DIK_W) == true)
	{//��ړ�
		m_nOldNumCuntry = m_nNumCuntry;									//�O��̏���ۑ�����

		if (m_nNumCuntry < WIDTH_DIVISION)
		{
			m_nNumCuntry += WIDTH_DIVISION;
		}
		else
		{
			m_nNumCuntry -= WIDTH_DIVISION;
		}

		//�����̔j��
		if (m_pNationalFlag != NULL)
		{
			m_pNationalFlag->Uninit();
			m_pNationalFlag = NULL;
		}
	}
	else if (pKeyboard->GetKeyboardTrigger(DIK_D) == true
		|| pKeyboard->GetKeyboardRepeat(DIK_D) == true)
	{//�E�ړ�
		m_nOldNumCuntry = m_nNumCuntry;									//�O��̏���ۑ�����
		m_nNumCuntry += 1;

		if (m_nNumCuntry <= WIDTH_DIVISION)
		{
			if ((m_nNumCuntry % WIDTH_DIVISION) == 0) { m_nNumCuntry = 0; }
		}
		else
		{
			if ((m_nNumCuntry % MAX_CUNTORY) == 0) { m_nNumCuntry = MAX_CUNTORY / 2; }
		}

		//�����̔j��
		if (m_pNationalFlag != NULL)
		{
			m_pNationalFlag->Uninit();
			m_pNationalFlag = NULL;
		}
	}
	else if (pKeyboard->GetKeyboardTrigger(DIK_S) == true
		|| pKeyboard->GetKeyboardRepeat(DIK_S) == true)
	{//���ړ�
		m_nOldNumCuntry = m_nNumCuntry;									//�O��̏���ۑ�����

		if (m_nNumCuntry < WIDTH_DIVISION)
		{
			m_nNumCuntry += WIDTH_DIVISION;
		}
		else
		{
			m_nNumCuntry -= WIDTH_DIVISION;
		}

		//�����̔j��
		if (m_pNationalFlag != NULL)
		{
			m_pNationalFlag->Uninit();
			m_pNationalFlag = NULL;
		}
	}
	else if (pKeyboard->GetKeyboardTrigger(DIK_A) == true
		|| pKeyboard->GetKeyboardRepeat(DIK_A) == true)
	{//���ړ�
		m_nOldNumCuntry = m_nNumCuntry;									//�O��̏���ۑ�����
		m_nNumCuntry -= 1;

		if (m_nNumCuntry < 0)
		{
			m_nNumCuntry = WIDTH_DIVISION - 1;
		}
		else if (m_nNumCuntry == WIDTH_DIVISION - 1)
		{
			m_nNumCuntry = MAX_CUNTORY - 1;
		}

		//�����̔j��
		if (m_pNationalFlag != NULL)
		{
			m_pNationalFlag->Uninit();
			m_pNationalFlag = NULL;
		}
	}

	//����{�^������������
	if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (m_pNationalFlag == NULL)
		{
			//�e�N�X�`���̊�����
			LPDIRECT3DTEXTURE9 pTexture = CManager::GetSelect()->GetCuntryTex(m_nNumCuntry);

			//�����̐���
			m_pNationalFlag = CMoveUI::Create(D3DXVECTOR3(CUNTRY_POS.x + (ADD_X * m_nNumPlayer), CUNTRY_POS.y, 0.0f), CUNTRY_SIZE, pTexture);
		}
		else
		{
			CManager::GetSelect()->SetState(CSelect::STATE_CHANGE);
			m_state = STATE_MOVE;	//�ړ���Ԃɂ���
		}
	}
}

//=============================================================================
// ���̑I������(Mouse)
//=============================================================================
void CPlayerIcon::UpdateInputMouse(void)
{
	if (m_nNumInput == -1 || m_InputType != CSelect::INPUTTYPE_MOUSE) { return; }	//�R���g���[���[���o�^����Ă��Ȃ��Ȃ�X�V���Ȃ�

	//�T�E���h�̎擾
	CSound *pSound = CManager::GetSound();
	CRawMouse * pRawMouse = CManager::GetRawMouse();

	if (pRawMouse->GetTrigger(m_nNumInput, CRawMouse::RIMS_SLIDE_UP) == true)
	{//��ړ�
	 //�Z���N�g��
		pSound->PlaySound(CSound::SOUND_LABEL_SE009);
		m_nOldNumCuntry = m_nNumCuntry;									//�O��̏���ۑ�����

		if (m_nNumCuntry < WIDTH_DIVISION)
		{
			m_nNumCuntry += WIDTH_DIVISION;
		}
		else
		{
			m_nNumCuntry -= WIDTH_DIVISION;
		}

		if (m_pNationalFlag != NULL)
		{
			m_pNationalFlag->Uninit();
			m_pNationalFlag = NULL;
		}
	}
	else if (pRawMouse->GetTrigger(m_nNumInput, CRawMouse::RIMS_SLIDE_RIGHT) == true)
	{//�E�ړ�
	 //�Z���N�g��
		pSound->PlaySound(CSound::SOUND_LABEL_SE009);
		m_nOldNumCuntry = m_nNumCuntry;									//�O��̏���ۑ�����

		m_nNumCuntry += 1;

		if (m_nNumCuntry <= WIDTH_DIVISION)
		{
			if ((m_nNumCuntry % WIDTH_DIVISION) == 0) { m_nNumCuntry = 0; }
		}
		else
		{
			if ((m_nNumCuntry % MAX_CUNTORY) == 0) { m_nNumCuntry = MAX_CUNTORY / 2; }
		}

		if (m_pNationalFlag != NULL)
		{
			m_pNationalFlag->Uninit();
			m_pNationalFlag = NULL;
		}
	}
	else if (pRawMouse->GetTrigger(m_nNumInput, CRawMouse::RIMS_SLIDE_DOWN) == true)
	{//���ړ�
	 //�Z���N�g��
		pSound->PlaySound(CSound::SOUND_LABEL_SE009);
		m_nOldNumCuntry = m_nNumCuntry;									//�O��̏���ۑ�����

		if (m_nNumCuntry < WIDTH_DIVISION)
		{
			m_nNumCuntry += WIDTH_DIVISION;
		}
		else
		{
			m_nNumCuntry -= WIDTH_DIVISION;
		}

		if (m_pNationalFlag != NULL)
		{
			m_pNationalFlag->Uninit();
			m_pNationalFlag = NULL;
		}
	}
	else if (pRawMouse->GetTrigger(m_nNumInput, CRawMouse::RIMS_SLIDE_LEFT) == true)
	{//���ړ�
	 //�Z���N�g��
		pSound->PlaySound(CSound::SOUND_LABEL_SE009);
		m_nOldNumCuntry = m_nNumCuntry;									//�O��̏���ۑ�����
		m_nNumCuntry -= 1;

		if (m_nNumCuntry < 0)
		{
			m_nNumCuntry = WIDTH_DIVISION - 1;
		}
		else if (m_nNumCuntry == WIDTH_DIVISION - 1)
		{
			m_nNumCuntry = MAX_CUNTORY - 1;
		}

		if (m_pNationalFlag != NULL)
		{
			m_pNationalFlag->Uninit();
			m_pNationalFlag = NULL;
		}
	}

	//����{�^������������
	if (pRawMouse->GetTrigger(m_nNumInput, CRawMouse::RIMS_BUTTON_RIGHT) == true)
	{
		if (CFade::FADE_OUT != CFade::GetFadeMode())
		{//�t�F�[�h�A�E�g�̎�����Ȃ������特��炷
		 //�Z���N�g��
			pSound->PlaySound(CSound::SOUND_LABEL_SE007);
		}

		//�����̐���
		if (m_pNationalFlag == NULL)
		{
			//�e�N�X�`���̊�����
			LPDIRECT3DTEXTURE9 pTexture = CManager::GetSelect()->GetCuntryTex(m_nNumCuntry);

			//�����̐���
			m_pNationalFlag = CMoveUI::Create(D3DXVECTOR3(CUNTRY_POS.x + (ADD_X * m_nNumPlayer), CUNTRY_POS.y, 0.0f), CUNTRY_SIZE, pTexture);
		}
		else
		{
			CManager::GetSelect()->SetState(CSelect::STATE_CHANGE);
			m_state = STATE_MOVE;	//�ړ���Ԃɂ���
		}
	}
}

//=============================================================================
// �ʒu�̐ݒ�
//=============================================================================
void CPlayerIcon::UpdatePos(void)
{
	if (m_nOldNumCuntry == m_nNumCuntry) { return; }

	//�ʒu���擾����
	D3DXVECTOR3 CuntryPos = CSelect::GetNatonalFlag(m_nNumCuntry)->GetPos();
	CuntryPos.x += ADDPOS_X;

	D3DXVECTOR3 OtherPos = CScene2D::GetOtherPos();
	CScene2D::SetPos(CuntryPos, OtherPos);
}

//=============================================================================
// �ړ�����
//=============================================================================
void CPlayerIcon::UpdateMove(void)
{
	//�ʒu�̎擾
	D3DXVECTOR3 Pos = CScene2D::GetPos();

	//�ړ��ʂ̉��Z
	if (Pos.x < (SCREEN_WIDTH / 2.0f))
	{
		Pos.x -= MOVE_RIGHT_LEFT;
	}
	else
	{
		Pos.x += MOVE_RIGHT_LEFT;
	}

	//�ʒu�ݒ�
	CScene2D::SetPos(Pos, CScene2D::GetOtherPos());

	//�����̈ړ�
	if (m_pNationalFlag == NULL) { return; }
	if (m_nNumPlayer < (NUMPLAYER / 2))
	{
		m_pNationalFlag->UpdateMoveLeft(MOVE_RIGHT_LEFT);
	}
	else
	{
		m_pNationalFlag->UpdateMoveRight(MOVE_RIGHT_LEFT);
	}
}

//=============================================================================
// ���Ɉړ�����
//=============================================================================
void CPlayerIcon::UpdateMoveLeft(float fMove)
{
	//�ʒu�̎擾
	D3DXVECTOR3 Pos = CScene2D::GetPos();

	//�ړ�����
	Pos.x -= fMove;

	//�ʒu�ݒ�
	CScene2D::SetPos(Pos, CScene2D::GetOtherPos());
}

//=============================================================================
// �E�Ɉړ�����
//=============================================================================
void CPlayerIcon::UpdateMoveRight(float fMove)
{
	//�ʒu�̎擾
	D3DXVECTOR3 Pos = CScene2D::GetPos();

	//�ړ�����
	Pos.x += fMove;

	//�ʒu�ݒ�
	CScene2D::SetPos(Pos, CScene2D::GetOtherPos());
}