//*****************************************************************************
//
//     UI�̏���[UI.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "UI.h"
#include "score.h"


// �f�o�b�O�p
#include "input.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define UI_PRIORITY (6)		// UI�̕`��D�揇��

// �Q�[���X�R�A�p
#define UI_GAMESCORE_POS_0 (D3DXVECTOR3(20.0f,20.0f,0.0f))
#define UI_GAMESCORE_POS_1 (D3DXVECTOR3(1180.0f,20.0f,0.0f))
#define UI_GAMESCORE_POS_2 (D3DXVECTOR3(20.0f,695.0f,0.0f))
#define UI_GAMESCORE_POS_3 (D3DXVECTOR3(1180.0f,695.0f,0.0f))
#define UI_GAMESCORE_SIZE  (D3DXVECTOR3(25.0f,25.0f,0.0f))

//*****************************************************************************
//     �ÓI�����o�ϐ��錾
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CUI::CUI()
{
	m_nNumPlayer = 0;
	for (int nCntScore = 0; nCntScore < MAX_PLAYERNUM; nCntScore++)
	{
		m_pScoreGame[nCntScore] = NULL;
	}
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CUI::~CUI()
{

}

//=============================================================================
//    ��������
//=============================================================================
CUI *CUI::Create(int nNumPlayer)
{
	CUI *pUI = NULL;
	if (pUI != NULL) { return NULL; }

	// ���������m��
	pUI = new CUI;
	if (pUI == NULL) { return NULL; }

	// ����������
	if (FAILED(pUI->Init(nNumPlayer)))
	{
		pUI->Uninit();
		delete pUI;
		return NULL;
	}

	return pUI;	// �C���X�^���X�̃A�h���X��Ԃ�
}


//=============================================================================
//    ����������
//=============================================================================
HRESULT CUI::Init(int nNumPlayer)
{
	// �v���C���[�̐l�����L��
	m_nNumPlayer = nNumPlayer;

	// �Q�[���X�R�A�𐶐�����
	CreateScoreGame();

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CUI::Uninit(void)
{
	// �Q�[���X�R�A�N���X���J������
	ReleaseScoreGame();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CUI::Update(void)
{
	if (CManager::GetInputkeyboard()->GetKeyboardPress(DIK_9) == true)
	{
		m_pScoreGame[0]->AddScore(1);
	}
	if (CManager::GetInputkeyboard()->GetKeyboardPress(DIK_8) == true)
	{
		m_pScoreGame[0]->AddScore(-1);
	}
}

//=============================================================================
//    �Q�[���X�R�A�N���X�𐶐����鏈��
//=============================================================================
void CUI::CreateScoreGame(void)
{
	// �K�v�ȕϐ���錾
	D3DXVECTOR3 pos[MAX_PLAYERNUM] =
	{
		UI_GAMESCORE_POS_0,
		UI_GAMESCORE_POS_1,
		UI_GAMESCORE_POS_2,
		UI_GAMESCORE_POS_3
	};
	D3DXVECTOR3 size = UI_GAMESCORE_SIZE;

	// �N���X����
	for (int nCntPlayer = 0; nCntPlayer < m_nNumPlayer; nCntPlayer++)
	{// �Q�[���ɎQ������v���C���[�̐������J��Ԃ�
		if (m_pScoreGame[nCntPlayer] != NULL) { continue; }// �N���A����Ă��Ȃ��ꍇ�͏����������̃��[�v��

		// �C���X�^���X����
		m_pScoreGame[nCntPlayer] = (CScoreGame*)CScore::Create(CScore::TYPE_GAME, UI_PRIORITY, pos[nCntPlayer], size);
	}
}

//=============================================================================
//    �Q�[���X�R�A�N���X���J�����鏈��
//=============================================================================
void CUI::ReleaseScoreGame(void)
{
	for (int nCntScore = 0; nCntScore < MAX_PLAYERNUM; nCntScore++)
	{
		if (m_pScoreGame[nCntScore] != NULL)
		{
			m_pScoreGame[nCntScore]->Uninit();
			m_pScoreGame[nCntScore] = NULL;
		}
	}
}