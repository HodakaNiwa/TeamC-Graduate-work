//*****************************************************************************
//
//     UI�̏���[UI.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "UI.h"
#include "score.h"
#include "minimap.h"

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
	m_pMiniMap = NULL;
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

	// �~�j�}�b�v�N���X�𐶐�����
	CreateMiniMap();

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CUI::Uninit(void)
{
	// �Q�[���X�R�A�N���X���J������
	ReleaseScoreGame();

	// �~�j�}�b�v�N���X���J������
	ReleaseMiniMap();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CUI::Update(void)
{
	// �~�j�}�b�v�̍X�V����
	if (m_pMiniMap != NULL)
	{
		m_pMiniMap->Update();
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
//    �~�j�}�b�v�N���X�𐶐����鏈��
//=============================================================================
void CUI::CreateMiniMap(void)
{
	if (m_pMiniMap != NULL) { return; }
	m_pMiniMap = CMiniMap::Create();
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

//=============================================================================
//    �~�j�}�b�v�N���X���J�����鏈��
//=============================================================================
void CUI::ReleaseMiniMap(void)
{
	if (m_pMiniMap != NULL)
	{
		m_pMiniMap->Uninit();
		delete m_pMiniMap;
		m_pMiniMap = NULL;
	}
}