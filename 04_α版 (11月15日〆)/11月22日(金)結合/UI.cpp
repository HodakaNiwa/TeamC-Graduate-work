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
#include "charaicon.h"
#include "game.h"
#include "character.h"


#include "debuglog.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define UI_PRIORITY                (6)									// UI�̕`��D�揇��

// �Q�[���X�R�A�p
#define UI_GAMESCORE_POS_0         (D3DXVECTOR3(280.0f,23.0f,0.0f))		// 1P�̃X�R�A
#define UI_GAMESCORE_POS_1         (D3DXVECTOR3(920.0f,23.0f,0.0f))		// 2P�̃X�R�A
#define UI_GAMESCORE_POS_2         (D3DXVECTOR3(280.0f,380.0f,0.0f))	// 3P�̃X�R�A
#define UI_GAMESCORE_POS_3         (D3DXVECTOR3(920.0f,380.0f,0.0f))	// 4P�̃X�R�A
#define UI_GAMESCORE_POS_4         (D3DXVECTOR3(10000.0f,380.0f,0.0f))	// �G�̃X�R�A(��ʊO�֔z�u)
#define UI_GAMESCORE_SIZE          (D3DXVECTOR3(20.0f,40.0f,0.0f))		// �X�R�A�̐����̃T�C�Y

// �L�����A�C�R���p
#define UI_CHARAICON_WIDTH_LARGE   (50.0f)								// 1�ʂ̃L�����A�C�R���̕�
#define UI_CHARAICON_WIDTH_MIDDLE  (15.0f)								// 2�ʂ̃L�����A�C�R���̕�
#define UI_CHARAICON_WIDTH_SMALL   (10.0f)								// 3�ʂ̃L�����A�C�R���̕�
#define UI_CHARAICON_HEIGHT_LARGE  (50.0f)								// 1�ʂ̃L�����A�C�R���̍���
#define UI_CHARAICON_HEIGHT_MIDDLE (15.0f)								// 2�ʂ̃L�����A�C�R���̍���
#define UI_CHARAICON_HEIGHT_SMALL  (10.0f)								// 3�ʂ̃L�����A�C�R���̍���
#define UI_CHARAICON_TEXIDX        (1)									// �L�����A�C�R���̎g�p����e�N�X�`���̔ԍ�

// �v���C���[�ԍ��A�C�R���p
#define UI_PLAYERIDXICON_POS       (D3DXVECTOR3(0.0f, 120.0f, 0.0f))	// �v���C���[�ԍ��A�C�R���̑傫��
#define UI_PLAYERIDXICON_COL       (D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	// �v���C���[�ԍ��A�C�R���̐F
#define UI_PLAYERIDXICON_WIDTH     (25.0f)								// �v���C���[�ԍ��A�C�R���̕�
#define UI_PLAYERIDXICON_HEIGHT    (15.0f)								// �v���C���[�ԍ��A�C�R���̍���
#define UI_PLAYERIDXICON_TEXIDX    (0)									// �v���C���[�ԍ��A�C�R���̎g�p����e�N�X�`���̔ԍ�

// �����A�C�R���p
#define UI_CROWNICON_POS           (D3DXVECTOR3(0.0f, 120.0f, 0.0f))	// �����A�C�R���̑傫��
#define UI_CROWNICON_COL           (D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	// �����A�C�R���̐F
#define UI_CROWNICON_WIDTH         (20.0f)								// �����A�C�R���̕�
#define UI_CROWNICON_HEIGHT        (17.0f)								// �����A�C�R���̍���
#define UI_CROWNICON_TEXIDX        (1)									// �����A�C�R���̎g�p����e�N�X�`���̔ԍ�

//*****************************************************************************
//     �ÓI�����o�ϐ��錾
//*****************************************************************************


//*****************************************************************************
//     CUI�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CUI::CUI()
{
	m_nNumPlayer = 0;
	m_nNumAllCharacter = 0;
	m_pMiniMap = NULL;

	// �X�R�A
	for (int nCntScore = 0; nCntScore < MAX_PLAYERNUM * 2; nCntScore++)
	{
		m_pScoreGame[nCntScore] = NULL;
	}

	// �X�L���A�C�R��
	for (int nCntSkil = 0; nCntSkil < MAX_PLAYERNUM; nCntSkil++)
	{
		m_pSkilicon[nCntSkil] = NULL;
	}

	// �L�����A�C�R��
	for (int nCntChara = 0; nCntChara < MAX_PLAYERNUM; nCntChara++)
	{
		for (int nCntIcon = 0; nCntIcon < UI_RANKICON_NUM; nCntIcon++)
		{
			m_pChraicon[nCntChara][nCntIcon] = NULL;
		}
	}

	// ����(8�l��)
	for (int nCntIcon = 0; nCntIcon < MAX_PLAYERNUM * 2; nCntIcon++)
	{
		m_nRank[nCntIcon] = nCntIcon;
	}

	// �v���C���[�ԍ��A�C�R��
	for (int nCntIdx = 0; nCntIdx < MAX_PLAYERNUM * 2; nCntIdx++)
	{
		m_pPlayerIdxicon[nCntIdx] = NULL;
	}

	// �����A�C�R��
	for (int nCntCrown = 0; nCntCrown < UI_RANKICON_NUM; nCntCrown++)
	{
		m_pCrownicon[nCntCrown] = NULL;
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

	// �L�����A�C�R���𐶐�����
	CreateCharaicon();

	// �v���C���[�ԍ��A�C�R���𐶐�����
	CreatePlayerIdxicon();

	// �����A�C�R���𐶐�����
	CreateCrownRank();

	// �e�N�X�`�������蓖�Ă鏈��
	BindTextureToIcon();

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

	// �L�����A�C�R�����J������
	ReleaseCharaicon();

	// �v���C���[�ԍ��A�C�R�����J������
	ReleasePlayerIdxicon();

	// �����A�C�R�����J������
	ReleaseCrownIcon();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CUI::Update(void)
{
	// �Q�[���N���X�̎擾
	CGame *pGame = (CGame*)CManager::GetModeBase();
	if (pGame == NULL) { return; }

	// �X�L���A�C�R���̍X�V����
	UpdateSkilicon();

	// �L�����A�C�R���̍X�V����
	UpdateCharaicon();

	// �v���C���[�ԍ��A�C�R���̍X�V����
	UpdatePlayerIdxicon(pGame);

	// �����A�C�R���̍X�V����
	UpdateCrownicon(pGame);


	for (int nCntRank = 0; nCntRank < m_nNumAllCharacter; nCntRank++)
	{
		CDebugProc::Print("�X�R�A : %d\n", m_pScoreGame[nCntRank]->GetScore());
	}
	CDebugProc::Print("\n");
	for (int nCntRank = 0; nCntRank < m_nNumAllCharacter; nCntRank++)
	{
		CDebugProc::Print("���� : %d\n", m_nRank[nCntRank]);
	}
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
//    �L�����A�C�R���𐶐����鏈��
//=============================================================================
void CUI::CreateCharaicon(void)
{
	for (int nCntChara = 0; nCntChara < MAX_PLAYERNUM; nCntChara++)
	{
		for (int nCntIcon = 0; nCntIcon < UI_RANKICON_NUM; nCntIcon++)
		{
			if (m_pChraicon[nCntChara][nCntIcon] != NULL) { continue; }
			m_pChraicon[nCntChara][nCntIcon] = CCharaicon::Create(INITIALIZE_VECTOR3, D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),
				UI_CHARAICON_WIDTH_LARGE, UI_CHARAICON_HEIGHT_LARGE);
		}
	}
}

//=============================================================================
//    �v���C���[�ԍ��A�C�R���𐶐����鏈��
//=============================================================================
void CUI::CreatePlayerIdxicon(void)
{
	D3DXVECTOR3 pos[MAX_PLAYERNUM] =
	{
		INITIALIZE_VECTOR3,
		INITIALIZE_VECTOR3,
		INITIALIZE_VECTOR3,
		INITIALIZE_VECTOR3,
	};
	D3DXCOLOR col;
	for (int nCntIdx = 0; nCntIdx < m_nNumAllCharacter; nCntIdx++)
	{
		col = CCharacter::m_CountryColor[nCntIdx];
		if (nCntIdx >= m_nNumPlayer) { col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f); }
		if (m_pPlayerIdxicon[nCntIdx] != NULL) { continue; }
		m_pPlayerIdxicon[nCntIdx] = CIconBillboard::Create(pos[nCntIdx], col,
			UI_PLAYERIDXICON_WIDTH, UI_PLAYERIDXICON_HEIGHT);
	}
}

//=============================================================================
//    �����A�C�R���𐶐����鏈��
//=============================================================================
void CUI::CreateCrownRank(void)
{
	D3DXVECTOR3 pos[UI_RANKICON_NUM] =
	{
		INITIALIZE_VECTOR3,
		INITIALIZE_VECTOR3,
		INITIALIZE_VECTOR3,
	};
	for (int nCntCrown = 0; nCntCrown < UI_RANKICON_NUM; nCntCrown++)
	{
		if (m_pCrownicon[nCntCrown] != NULL) { continue; }
		m_pCrownicon[nCntCrown] = CIconBillboard::Create(pos[nCntCrown], UI_CROWNICON_COL,
			UI_CROWNICON_WIDTH, UI_CROWNICON_HEIGHT);
	}
}

//=============================================================================
//    �A�C�R���Ƀe�N�X�`�������蓖�Ă鏈��
//=============================================================================
void CUI::BindTextureToIcon(void)
{
	// �Q�[���N���X�̎擾
	CGame *pGame = (CGame*)CManager::GetModeBase();
	if (pGame == NULL) { return; }

	// �e�N�X�`���ւ̃|�C���^��ۑ�
	m_pTexture = pGame->GetTexture();
	if (m_pTexture == NULL) { return; }

	// �L�����A�C�R���Ƀe�N�X�`�������蓖�Ă�
	BindTextureToIcon_Chara(pGame);

	// �v���C���[�ԍ��A�C�R���Ƀe�N�X�`�������蓖�Ă�
	BindTextureToIcon_PlayerIdx(pGame);

	// �����A�C�R���Ƀe�N�X�`�������蓖�Ă�
	BindTextureToIcon_Crown(pGame);
}

//=============================================================================
//    �L�����A�C�R���Ƀe�N�X�`�������蓖�Ă鏈��
//=============================================================================
void CUI::BindTextureToIcon_Chara(CGame *pGame)
{
	for (int nCntChara = 0; nCntChara < m_nNumPlayer; nCntChara++)
	{
		for (int nCntIcon = 0; nCntIcon < UI_RANKICON_NUM; nCntIcon++)
		{
			if (m_pChraicon[nCntChara][nCntIcon] == NULL) { continue; }

			// �L�����N�^�[�ւ̃|�C���^��ݒ�
			CCharacter *pParentChara = pGame->GetChara(nCntChara);

			// �e�ƂȂ�L�����N�^�[�ւ̃|�C���^��ݒ�
			m_pChraicon[nCntChara][nCntIcon]->SetParentChara(pParentChara);

			// �e�N�X�`���̐ݒ�
			m_pChraicon[nCntChara][nCntIcon]->BindTexture(m_pTexture[UI_CHARAICON_TEXIDX + nCntIcon]);
		}
	}
}

//=============================================================================
//    �v���C���[�ԍ��A�C�R���Ƀe�N�X�`�������蓖�Ă鏈��
//=============================================================================
void CUI::BindTextureToIcon_PlayerIdx(CGame *pGame)
{
	float fHeight = 0.0f;
	for (int nCntIdx = 0; nCntIdx < m_nNumAllCharacter; nCntIdx++)
	{
		// �L�����N�^�[�N���X���擾
		CCharacter *pChara = pGame->GetChara(nCntIdx);
		if (pChara == NULL || m_pPlayerIdxicon[nCntIdx] == NULL) { continue; }

		// �e�N�X�`�������蓖��
		m_pPlayerIdxicon[nCntIdx]->BindTexture(m_pTexture[UI_PLAYERIDXICON_TEXIDX]);

		// ���Ԃɂ���ăe�N�X�`���̍��W��ݒ�
		fHeight = 0.20f * nCntIdx;
		if (nCntIdx >= m_nNumPlayer)
		{
			fHeight = 0.80f;
		}
		m_pPlayerIdxicon[nCntIdx]->SetTexV(fHeight);
		m_pPlayerIdxicon[nCntIdx]->SetTexHeight(0.20f);
		m_pPlayerIdxicon[nCntIdx]->SetVtxBuffTex();
	}
}

//=============================================================================
//    �����A�C�R���Ƀe�N�X�`�������蓖�Ă鏈��
//=============================================================================
void CUI::BindTextureToIcon_Crown(CGame *pGame)
{
	for (int nCntCrown = 0; nCntCrown < UI_RANKICON_NUM; nCntCrown++)
	{
		// �e�N�X�`�������蓖��
		m_pCrownicon[nCntCrown]->BindTexture(m_pTexture[UI_CROWNICON_TEXIDX + nCntCrown]);
	}
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
//    �L�����A�C�R�����J�����鏈��
//=============================================================================
void CUI::ReleaseCharaicon(void)
{
	for (int nCntChara = 0; nCntChara < MAX_PLAYERNUM; nCntChara++)
	{
		for (int nCntIcon = 0; nCntIcon < UI_RANKICON_NUM; nCntIcon++)
		{
			if (m_pChraicon[nCntChara][nCntIcon] != NULL)
			{
				m_pChraicon[nCntChara][nCntIcon]->Uninit();
				delete m_pChraicon[nCntChara][nCntIcon];
				m_pChraicon[nCntChara][nCntIcon] = NULL;
			}
		}
	}
}

//=============================================================================
//    �v���C���[�ԍ��A�C�R�����J�����鏈��
//=============================================================================
void CUI::ReleasePlayerIdxicon(void)
{
	for (int nCntIdx = 0; nCntIdx < MAX_PLAYERNUM * 2; nCntIdx++)
	{
		if (m_pPlayerIdxicon[nCntIdx] != NULL)
		{
			m_pPlayerIdxicon[nCntIdx]->Uninit();
			delete m_pPlayerIdxicon[nCntIdx];
			m_pPlayerIdxicon[nCntIdx] = NULL;
		}
	}
}

//=============================================================================
//    �����|���S�����J�����鏈��
//=============================================================================
void CUI::ReleaseCrownIcon(void)
{
	for (int nCntCrown = 0; nCntCrown < UI_RANKICON_NUM; nCntCrown++)
	{
		if (m_pCrownicon[nCntCrown] != NULL)
		{
			m_pCrownicon[nCntCrown]->Uninit();
			delete m_pCrownicon[nCntCrown];
			m_pCrownicon[nCntCrown] = NULL;
		}
	}
}

//=============================================================================
//    �X�R�A�̃\�[�g
//=============================================================================
void CUI::SortRankFromScore(void)
{
	int nScore = 0;
	int nRankScore[MAX_PLAYERNUM * 2];

	// �X�R�A���擾
	for (int nCntRank = 0; nCntRank < MAX_PLAYERNUM * 2; nCntRank++)
	{// 1�`8�ʂ܂ŌJ��Ԃ�
		if (m_pScoreGame[nCntRank] == NULL) { continue; }
		nRankScore[nCntRank] = m_pScoreGame[nCntRank]->GetScore();
		m_nRank[nCntRank] = 0;
	}

	// ���ʔ���(�������ʂ�����)
	int nJudgScore = 0;
	for (int nCntRank = 0; nCntRank < MAX_PLAYERNUM * 2; nCntRank++)
	{
		nJudgScore = nRankScore[nCntRank];
		for (int nCntScore = 0; nCntScore < MAX_PLAYERNUM * 2; nCntScore++)
		{
			if (nJudgScore > nRankScore[nCntScore])
			{
				m_nRank[nCntScore]++;
			}
		}
	}

	// �����ʔ���(�z��̎Ⴂ���ɒ���)
	int nSet = 0;
	int nSetIdx[MAX_PLAYERNUM * 2] = {};
	for (int nCntRank = 0; nCntRank < MAX_PLAYERNUM * 2; nCntRank++)
	{
		for (int nCntScore = (nCntRank + 1); nCntScore < MAX_PLAYERNUM * 2; nCntScore++)
		{
			if (m_nRank[nCntRank] == m_nRank[nCntScore])
			{// �������ʂ������ꍇ
				nSetIdx[nSet] = nCntScore;
				nSet++;
			}
		}

		for (int nCntSet = 0; nCntSet < nSet; nCntSet++)
		{// �������ʂ������������������炵�Ă���
			m_nRank[nSetIdx[nCntSet]]++;
		}
		nSet = 0;
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
//    �L�����A�C�R���N���X�̍X�V����
//=============================================================================
void CUI::UpdateCharaicon(void)
{
	CGame *pGame = CManager::GetGame();
	if (pGame == NULL) { return; }

	for (int nCntChara = 0; nCntChara < m_nNumPlayer; nCntChara++)
	{
		for (int nCntIcon = 0; nCntIcon < UI_RANKICON_NUM; nCntIcon++)
		{
			// �L�����A�C�R���ɖړI�̃v���C���[��ݒ�
			CCharacter *pDestChara = pGame->GetChara(m_nRank[nCntIcon]);

			// �L�����A�C�R���̍X�V����
			if (m_pChraicon[nCntChara][nCntIcon] != NULL)
			{
				m_pChraicon[nCntChara][nCntIcon]->SetDestChara(pDestChara);
				m_pChraicon[nCntChara][nCntIcon]->Update();
			}
		}
	}
}

//=============================================================================
//    �v���C���[�ԍ��A�C�R���̍X�V����
//=============================================================================
void CUI::UpdatePlayerIdxicon(CGame *pGame)
{
	for (int nCntIdx = 0; nCntIdx < m_nNumAllCharacter; nCntIdx++)
	{
		// �L�����N�^�[�N���X���擾
		CCharacter *pChara = pGame->GetChara(nCntIdx);
		if (pChara == NULL || m_pPlayerIdxicon[nCntIdx] == NULL) { continue; }

		// �L�����N�^�[�̈ʒu���擾
		D3DXVECTOR3 pos = pChara->GetPos();

		// �A�C�R���̈ʒu��ݒ�
		m_pPlayerIdxicon[nCntIdx]->SetPos(pos + UI_PLAYERIDXICON_POS);
	}
}

//=============================================================================
//    �����A�C�R���̍X�V����
//=============================================================================
void CUI::UpdateCrownicon(CGame *pGame)
{
	int nRank = 0;
	int nSetIcon[UI_RANKICON_NUM] = {-1,-1,-1};
	for (int nCntCrown = 0; nCntCrown < UI_RANKICON_NUM; nCntCrown++)
	{
		// �����N��ݒ�
		nRank = (m_nRank[nCntCrown]);
		for (int nCntRank = 0; nCntRank < UI_RANKICON_NUM; nCntRank++)
		{
			if (nSetIcon[nCntRank] == nRank)
			{
				nRank = (m_nRank[nCntCrown + 1]);
			}
		}

		// �L�����N�^�[�N���X���擾
		CCharacter *pChara = pGame->GetChara(nRank);
		if (pChara == NULL || m_pCrownicon[nCntCrown] == NULL) { continue; }

		// �L�����N�^�[�̈ʒu���擾
		D3DXVECTOR3 pos = pChara->GetPos();

		// �A�C�R���̈ʒu��ݒ�
		m_pCrownicon[nCntCrown]->SetPos(pos + UI_CROWNICON_POS);
		nSetIcon[nCntCrown] = nCntCrown;
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

//=============================================================================
//    �A�C�R����`�悷�鏈��
//=============================================================================
void CUI::DrawIcon(int nPlayerIdx)
{
	for (int nCntIcon = 0; nCntIcon < UI_RANKICON_NUM; nCntIcon++)
	{
		if (m_pChraicon[nPlayerIdx][nCntIcon] != NULL)
		{
			m_pChraicon[nPlayerIdx][nCntIcon]->Draw();
		}
	}

	// �����A�C�R���̕`��
	DrawCrownIcon();

	// �v���C���[�ԍ��A�C�R���̕`��
	DrawPlayerIdxicon();
}

//=============================================================================
//    �v���C���[�ԍ��A�C�R����`�悷�鏈��
//=============================================================================
void CUI::DrawPlayerIdxicon(void)
{
	for (int nCntIdx = 0; nCntIdx < m_nNumAllCharacter; nCntIdx++)
	{
		if (m_pPlayerIdxicon[nCntIdx] != NULL)
		{
			m_pPlayerIdxicon[nCntIdx]->Draw();
		}
	}
}

//=============================================================================
//    �����A�C�R����`�悷�鏈��
//=============================================================================
void CUI::DrawCrownIcon(void)
{
	for (int nCntCrown = 0; nCntCrown < UI_RANKICON_NUM; nCntCrown++)
	{
		if (m_pCrownicon[nCntCrown] != NULL)
		{
			m_pCrownicon[nCntCrown]->Draw();
		}
	}
}


//*****************************************************************************
//     CIconBillboard�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CIconBillboard::CIconBillboard()
{
	m_pTexture = NULL;
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CIconBillboard::~CIconBillboard()
{

}

//=============================================================================
//    ��������
//=============================================================================
CIconBillboard *CIconBillboard::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, bool bLighting, float fRot, float fTexU, float fTexV, float fTexWidth, float fTexHeight)
{
	// ���������m��
	CIconBillboard *pIconBillboard = NULL;
	pIconBillboard = new CIconBillboard;
	if (pIconBillboard == NULL) { return NULL; }

	// �ϐ��̃N���A
	pIconBillboard->ClearVariable();

	// �e��l�̑��
	pIconBillboard->SetPos(pos);              // �|���S���̍��W
	pIconBillboard->SetCol(col);              // �|���S���̐F
	pIconBillboard->SetRot(fRot);             // �|���S���̌���
	pIconBillboard->SetWidth(fWidth);         // �|���S���̕�
	pIconBillboard->SetHeight(fHeight);       // �|���S���̍���
	pIconBillboard->SetTexU(fTexU);           // �|���S���̍���e�N�X�`��U���W
	pIconBillboard->SetTexV(fTexV);           // �|���S���̍���e�N�X�`��V���W
	pIconBillboard->SetTexWidth(fTexWidth);   // �|���S���̃e�N�X�`�����W�̉���
	pIconBillboard->SetTexHeight(fTexHeight); // �|���S���̃e�N�X�`�����W�̏c��
	pIconBillboard->SetLighting(bLighting);   // ���C�e�B���O���邩���Ȃ���

	// ����������
	if (FAILED(pIconBillboard->Init()))
	{
		pIconBillboard->Uninit();
		delete pIconBillboard;
		return NULL;
	}

	return pIconBillboard;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CIconBillboard::Init(void)
{
	// �`��f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

	// ���_�o�b�t�@�̍쐬
	MakeVertex(pDevice);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CIconBillboard::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	DIRECT_RELEASE(m_pVtxBuff);
}

//=============================================================================
//    �X�V����
//=============================================================================
void CIconBillboard::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CIconBillboard::Draw(void)
{
	// �`��f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return; }

	// ���C�e�B���O�̐ݒ�
	DWORD Lighting;
	pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
	if (m_bLighting == false)
	{// ���C�e�B���O���O��
		pDevice->SetRenderState(D3DRS_LIGHTING, false);
	}

	// �g�����X�t�H�[�������v�Z���f�o�C�X�ɐݒ�
	CalcNotScaleTransform(pDevice);
	SetTransformToDevice(pDevice);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// Z�e�X�g�����Ȃ��ݒ��
	DWORD ZTest;
	pDevice->GetRenderState(D3DRS_ZENABLE, &ZTest);
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// Z�e�X�g������ݒ��
	pDevice->SetRenderState(D3DRS_ZENABLE, ZTest);

	// ���C�e�B���O��߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);
}

//=============================================================================
//    �g�����X�t�H�[�����(�傫�����܂߂Ȃ�)���v�Z����
//=============================================================================
void CIconBillboard::CalcNotScaleTransform(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxView, mtxTrans; // �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_MtxWorld);

	// �J�����̌������擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// ���[���h�}�g���b�N�X�Ɍ�����ݒ�(�J�����̋t�s������邱�ƂŃJ�����̐��ʂɌ�����␳)
	m_MtxWorld._11 = mtxView._11;
	m_MtxWorld._12 = mtxView._21;
	m_MtxWorld._13 = mtxView._31;
	m_MtxWorld._21 = mtxView._12;
	m_MtxWorld._22 = mtxView._22;
	m_MtxWorld._23 = mtxView._32;
	m_MtxWorld._31 = mtxView._13;
	m_MtxWorld._32 = mtxView._23;
	m_MtxWorld._33 = mtxView._33;

	// �ʒu�𔽉f
	m_MtxWorld._41 = m_Pos.x;
	m_MtxWorld._42 = m_Pos.y;
	m_MtxWorld._43 = m_Pos.z;
}

//=============================================================================
//    �g�����X�t�H�[�������f�o�C�X�ɐݒ肷��
//=============================================================================
void CIconBillboard::SetTransformToDevice(const LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    ���_�o�b�t�@��������
//=============================================================================
void CIconBillboard::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_�o�b�t�@�֒l��ݒ肷��
	SetVtxBuffValue();
}

//=============================================================================
//    �|���S���̑傫����ݒ肷�鏈��
//=============================================================================
void CIconBillboard::SetScale(float fWidth, float fHeight)
{
	// �I�t�Z�b�g�̒����Ɗp�x�����߂�
	m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));
	m_fAngle = atan2f(m_fWidth, m_fHeight);
}

//=============================================================================
//    ���_�o�b�t�@�֒l��ݒ肷�鏈��
//=============================================================================
void CIconBillboard::SetVtxBuffValue(void)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �傫����ݒ�
	SetScale(m_fWidth, m_fHeight);

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength, 0.0f);

	// �@���v�Z�p�x�N�g��
	D3DXVECTOR3 vecVer[4];

	// ���_�̖@��
	D3DXVECTOR3 nor1, nor2;

	// �O�ϗp�Ƀx�N�g�������
	vecVer[0] = pVtx[0].pos - pVtx[2].pos;
	vecVer[1] = pVtx[1].pos - pVtx[2].pos;
	vecVer[2] = pVtx[2].pos - pVtx[1].pos;
	vecVer[3] = pVtx[3].pos - pVtx[1].pos;

	// �x�N�g�����O�ςŌv�Z���Ė@�����o��
	D3DXVec3Cross(&nor1, &vecVer[0], &vecVer[1]);
	D3DXVec3Cross(&nor2, &vecVer[3], &vecVer[2]);
	D3DXVec3Normalize(&nor1, &nor1);
	D3DXVec3Normalize(&nor2, &nor2);

	// �@������
	pVtx[0].nor = nor1;
	pVtx[1].nor = (nor1 + nor2) / 2;
	pVtx[2].nor = (nor1 + nor2) / 2;
	pVtx[3].nor = nor2;

	// ���_�J���[
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(m_fTexU, m_fTexV);
	pVtx[1].tex = D3DXVECTOR2(m_fTexU + m_fTexWidth, m_fTexV);
	pVtx[2].tex = D3DXVECTOR2(m_fTexU, m_fTexV + m_fTexHeight);
	pVtx[3].tex = D3DXVECTOR2(m_fTexU + m_fTexWidth, m_fTexV + m_fTexHeight);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    ���_�o�b�t�@�֒��_���W��ݒ肷�鏈��
//=============================================================================
void CIconBillboard::SetVtxBuffPos(void)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �傫����ݒ�
	SetScale(m_fWidth, m_fHeight);

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    ���_�o�b�t�@�֖@���x�N�g����ݒ肷�鏈��
//=============================================================================
void CIconBillboard::SetVtxBuffNor(void)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �@���v�Z�p�x�N�g��
	D3DXVECTOR3 vecVer[4];

	// ���_�̖@��
	D3DXVECTOR3 nor1, nor2;

	// �O�ϗp�Ƀx�N�g�������
	vecVer[0] = pVtx[0].pos - pVtx[2].pos;
	vecVer[1] = pVtx[1].pos - pVtx[2].pos;
	vecVer[2] = pVtx[2].pos - pVtx[1].pos;
	vecVer[3] = pVtx[3].pos - pVtx[1].pos;

	// �x�N�g�����O�ςŌv�Z���Ė@�����o��
	D3DXVec3Cross(&nor1, &vecVer[0], &vecVer[1]);
	D3DXVec3Cross(&nor2, &vecVer[3], &vecVer[2]);
	D3DXVec3Normalize(&nor1, &nor1);
	D3DXVec3Normalize(&nor2, &nor2);

	// �@������
	pVtx[0].nor = nor1;
	pVtx[1].nor = (nor1 + nor2) / 2;
	pVtx[2].nor = (nor1 + nor2) / 2;
	pVtx[3].nor = nor2;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    ���_�o�b�t�@�֒��_�J���[��ݒ肷�鏈��
//=============================================================================
void CIconBillboard::SetVtxBuffCol(void)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    ���_�o�b�t�@�փe�N�X�`�����W��ݒ肷��
//=============================================================================
void CIconBillboard::SetVtxBuffTex(void)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(m_fTexU, m_fTexV);
	pVtx[1].tex = D3DXVECTOR2(m_fTexU + m_fTexWidth, m_fTexV);
	pVtx[2].tex = D3DXVECTOR2(m_fTexU, m_fTexV + m_fTexHeight);
	pVtx[3].tex = D3DXVECTOR2(m_fTexU + m_fTexWidth, m_fTexV + m_fTexHeight);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    �ϐ��̃N���A
//=============================================================================
void CIconBillboard::ClearVariable(void)
{
	// �e��l�̃N���A
	m_pTexture = NULL;                         // �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;                         // ���_�o�b�t�@�ւ̃|�C���^
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // �|���S���̍��W
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // �|���S���̐F
	m_fRot = 0.0f;                             // �|���S���̌���
	m_fWidth = 0.0f;                           // �|���S���̕�
	m_fHeight = 0.0f;                          // �|���S���̍���
	m_fAngle = 0.0f;                           // �|���S�����o���p�x
	m_fLength = 0.0f;                          // �|���S���̒���
	m_fTexU = 0.0f;                            // �|���S���̍���e�N�X�`��U���W
	m_fTexV = 0.0f;                            // �|���S���̍���e�N�X�`��V���W
	m_fTexWidth = 0.0f;                        // �|���S���̃e�N�X�`�����W�̉���
	m_fTexHeight = 0.0f;                       // �|���S���̃e�N�X�`�����W�̏c��
	D3DXMatrixIdentity(&m_MtxWorld);           // ���[���h�}�g���b�N�X
	m_bLighting = false;                       // ���C�e�B���O���邩���Ȃ���
}