//=============================================================================
//
// �`���[�g���A���̏��� [tutorial.cpp]
// Author : ���� ����
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
#include "movie.h"
#include "RawMouse.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define LOAD_OBJ			("data/TEXT/Stage.txt")
#define LOAD_TERRITORY		("data/TEXT/Territory.txt")
#define MOVIE_NAME			(L"data/MOVIE/Tutorial.avi")

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

	//�ϐ��̏�����
	m_pMovie = NULL;

	//����̐���
	if (m_pMovie == NULL)
	{
		m_pMovie = CMovie::Create(MOVIE_NAME);
	}
}

//=============================================================================
// �I������
//=============================================================================
void CTutorial::Uninit(void)
{
	//�x�[�X�̔j��
	CModeBase::Uninit();

	if (m_pMovie != NULL)
	{
		m_pMovie->Uninit();
		delete m_pMovie;
		m_pMovie = NULL;
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
	CGamePad	*pGamePad = CManager::GetInputGamePad();
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();
	CInputXPad * pXPad = CManager::GetXPad();
	CRawMouse *pRawMouse = CManager::GetRawMouse();					//RawMouse�̎擾
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//�L�[�{�[�h�̎擾

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || pXPad->GetTrigger(XINPUT_GAMEPAD_B, nCnt) == true ||
			pRawMouse->GetTrigger(nCnt, CRawMouse::RIMS_BUTTON_RIGHT) == true)
		{
			if (CFade::GetFadeMode() != CFade::FADE_OUT) { CFade::SetFade(CManager::MODE_TITLE); }
		}
	}

	//����̍X�V����
	if (m_pMovie != NULL)
	{
		m_pMovie->Update();
		bool bEndMovie = m_pMovie->GetEndMovie();

		if (bEndMovie)
		{
			//����̔j��
			m_pMovie->Uninit();
			delete m_pMovie;
			m_pMovie = NULL;

			//�^�C�g����
			if (CFade::GetFadeMode() != CFade::FADE_OUT) { CFade::SetFade(CManager::MODE_TITLE); }
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTutorial::Draw(void)
{
	CScene::DrawAll();
}