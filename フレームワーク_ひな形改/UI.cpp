//*****************************************************************************
//
//     UI�̏���[UI.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "UI.h"
#include "score.h"
#include "minimap.h"
#include "skilicon.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define UI_PRIORITY (6)		// UI�̕`��D�揇��

// �Q�[���X�R�A�p
#define UI_GAMESCORE_POS_0 (D3DXVECTOR3(280.0f,23.0f,0.0f))
#define UI_GAMESCORE_POS_1 (D3DXVECTOR3(920.0f,23.0f,0.0f))
#define UI_GAMESCORE_POS_2 (D3DXVECTOR3(280.0f,690.0f,0.0f))
#define UI_GAMESCORE_POS_3 (D3DXVECTOR3(920.0f,690.0f,0.0f))
#define UI_GAMESCORE_POS_4 (D3DXVECTOR3(10000.0f,690.0f,0.0f))
#define UI_GAMESCORE_SIZE  (D3DXVECTOR3(20.0f,40.0f,0.0f))

//*****************************************************************************
//     �ÓI�����o�ϐ��錾
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CUI::CUI()
{
	m_nNumPlayer = 0;
	m_nNumAllCharacter = 0;
	m_pMiniMap = NULL;
	for (int nCntScore = 0; nCntScore < MAX_PLAYERNUM * 2; nCntScore++)
	{
		m_pScoreGame[nCntScore] = NULL;
	}
	for (int nCntSkil = 0; nCntSkil < MAX_PLAYERNUM; nCntSkil++)
	{
		m_pSkilicon[nCntSkil] = NULL;
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
CUI *CUI::Create(int nNumPlayer, int nAllCharacter)
{
	CUI *pUI = NULL;
	if (pUI != NULL) { return NULL; }

	// ���������m��
	pUI = new CUI;
	if (pUI == NULL) { return NULL; }

	// ����������
	if (FAILED(pUI->Init(nNumPlayer, nAllCharacter)))
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
HRESULT CUI::Init(int nNumPlayer, int nAllCharacter)
{
	// �v���C���[�̐l�����L��
	m_nNumPlayer = nNumPlayer;

	// �S�Ă̎Q���l�����L��
	m_nNumAllCharacter = nAllCharacter;

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

	// �X�L���A�C�R���N���X���J������
	ReleaseSkilicon();
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

	// �X�L���A�C�R���̍X�V����
	UpdateSkilicon();
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CUI::Draw(void)
{
	// �X�L���A�C�R���̕`�揈��
	DrawSkilicon();
}

//=============================================================================
//    �Q�[���X�R�A�N���X�𐶐����鏈��
//=============================================================================
void CUI::CreateScoreGame(void)
{
	// �K�v�ȕϐ���錾
	D3DXVECTOR3 pos[MAX_PLAYERNUM * 2] =
	{
		UI_GAMESCORE_POS_0,
		UI_GAMESCORE_POS_1,
		UI_GAMESCORE_POS_2,
		UI_GAMESCORE_POS_3,
		UI_GAMESCORE_POS_4,
		UI_GAMESCORE_POS_4,
		UI_GAMESCORE_POS_4,
		UI_GAMESCORE_POS_4,
	};
	D3DXVECTOR3 size = UI_GAMESCORE_SIZE;

	if (m_nNumPlayer == 1)
	{// �v���C�l������l��������
		float fValue = UI_GAMESCORE_SIZE.x * 1.5f;
		pos[0].x = SCREEN_WIDTH / 2 - fValue - (fValue / 2.0f);
		pos[0].y += UI_GAMESCORE_SIZE.y / 3.0f;
		size.x *= 1.5f;
		size.y *= 1.5f;
	}

	// �N���X����
	for (int nCntPlayer = 0; nCntPlayer < m_nNumAllCharacter; nCntPlayer++)
	{// �Q�[���ɎQ������v���C���[�̐������J��Ԃ�
		if (m_pScoreGame[nCntPlayer] != NULL) { continue; }// �N���A����Ă��Ȃ��ꍇ�͏����������̃��[�v��
		if (nCntPlayer >= m_nNumPlayer) { pos[nCntPlayer] = UI_GAMESCORE_POS_4; }

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
//    �X�L���A�C�R���N���X�𐶐����鏈��
//=============================================================================
void CUI::CreateSkilicon(CCharacter *pChara, int nIdx, int nRecastTime)
{
	if (m_pSkilicon[nIdx] != NULL) { return; }
	m_pSkilicon[nIdx] = CSkilicon::Create(pChara, nIdx, nRecastTime);
}

//=============================================================================
//    �Q�[���X�R�A�N���X���J�����鏈��
//=============================================================================
void CUI::ReleaseScoreGame(void)
{
	for (int nCntScore = 0; nCntScore < MAX_PLAYERNUM * 2; nCntScore++)
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

//=============================================================================
//    �X�L���A�C�R���N���X���J�����鏈��
//=============================================================================
void CUI::ReleaseSkilicon(void)
{
	for (int nCntSkil = 0; nCntSkil < MAX_PLAYERNUM; nCntSkil++)
	{
		if (m_pSkilicon[nCntSkil] != NULL)
		{
			m_pSkilicon[nCntSkil]->Uninit();
			delete m_pSkilicon[nCntSkil];
			m_pSkilicon[nCntSkil] = NULL;
		}
	}
}

//=============================================================================
//    �X�L���A�C�R���N���X�̍X�V����
//=============================================================================
void CUI::UpdateSkilicon(void)
{
	for (int nCntSkil = 0; nCntSkil < m_nNumPlayer; nCntSkil++)
	{
		if (m_pSkilicon[nCntSkil] != NULL)
		{
			m_pSkilicon[nCntSkil]->Update();
		}
	}
}

//=============================================================================
//    �X�L���A�C�R����`�悷�鏈��
//=============================================================================
void CUI::DrawSkilicon(void)
{
	for (int nCntSkil = 0; nCntSkil < m_nNumPlayer; nCntSkil++)
	{
		if (m_pSkilicon[nCntSkil] != NULL)
		{
			m_pSkilicon[nCntSkil]->Draw();
		}
	}
}