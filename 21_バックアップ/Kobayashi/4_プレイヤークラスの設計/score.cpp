//=============================================================================
//
// �X�R�A�̏��� [score.cpp]
// Author : ���������@SATOU ASUMI
//
//=============================================================================
#include "score.h"
#include "manager.h"
#include "scene2D.h"
#include "number2D.h"
#include "scene.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define CLEAR_TIME	(15)
#define RED_TIME	(20)

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
//�X�R�A�N���X
int CScore::m_nScoreData[MAX_NUMBER] = {};
int CScore::m_nScore = 0;

//�X�R�A�Q�[���N���X
int CScoreGame::m_nNumber[MAX_NUMBER] = {};

//�X�R�A�����L���O�N���X
int CScoreRanking::m_nNumber[RANK][MAX_NUMBER] = {};
int CScoreRanking::m_nHighScore[RANK] = {};
int CScoreRanking::m_nTime = 0;
CScoreRanking::FLASH CScoreRanking::m_Flash[RANK] = {};
bool CScoreRanking::m_bFlash = false;

//=============================================================================
// ��������
//=============================================================================
CScore * CScore::Create(TYPE type, int Priority, D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CScore * pScore = NULL;

	if (type == TYPE_GAME)
	{
		pScore = new CScoreGame(Priority);
		pScore->SetSizePos(pos, size);
		pScore->Init();
	}
	else if (type == TYPE_RANKING)
	{
		pScore = new CScoreRanking(Priority);
		pScore->SetSizePos(pos, size);
		pScore->Init();
	}

	return pScore;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScore::CScore(int nPriority):CScene(nPriority){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScore::~CScore(){}

//=============================================================================
// �X�R�A�̉��Z
//=============================================================================
void CScore::AddScore(int nScore)
{
	//�ϐ��錾
	int nCntNum;			//�X�R�A�̌����̃J�E���^�[
	int nNum = 1;
	int nNum2 = 1;

	//�X�R�A�̉��Z
    m_nScore += nScore;

	//�����̌v�Z
	for (nCntNum = 0; nCntNum < MAX_NUMBER; nCntNum++)
	{
		nNum *= 10;
	}
	for (nCntNum = 0; nCntNum < MAX_NUMBER - 1; nCntNum++)
	{
		nNum2 *= 10;
	}

	for (nCntNum = 0; nCntNum < MAX_NUMBER; nCntNum++)
	{
		m_nScoreData[nCntNum] = (m_nScore % nNum) / nNum2;	//�i�[���鐔�����v�Z

		//�������P�����炷
		nNum = nNum / 10;
		nNum2 = nNum2 / 10;
	}
}

//=============================================================================
// �X�R�A�̎擾
//=============================================================================
int CScore::GetScore(void)
{
	return m_nScore;
}

//=============================================================================
//
// �X�R�A�Q�[��
//
//=============================================================================

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScoreGame::CScoreGame(int nPriority):CScore(nPriority){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScoreGame::~CScoreGame(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScoreGame::Init()
{
	//�ϐ��錾
	int nCntNum;
	int nCntNumber;

	//�����̃|�C���^�̏�����
	for (nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
	{
		m_pNumber[nCntNumber] = NULL;
	}

	//�����̏�����
	for (nCntNum = 0; nCntNum < MAX_NUMBER; nCntNum++)
	{
		m_nNumber[nCntNum] = 0;
	}

	m_nScore = 0;

	Set(m_pos, m_size);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CScoreGame::Uninit(void)
{
	//�ϐ��錾
	int nCntNumber;

	CManager::SetScore(m_nScore);	//�X�R�A�̃Z�b�g

	for (nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
	{
		if (m_pNumber[nCntNumber] != NULL)
		{
			m_pNumber[nCntNumber]->Uninit();

			delete m_pNumber[nCntNumber];
			m_pNumber[nCntNumber] = NULL;
		}
	}

	m_nScore = 0;

	for (int nCntNum = 0; nCntNum < MAX_NUMBER; nCntNum++)
	{
		m_nScoreData[nCntNum] = 0;
	}

	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CScoreGame::Update(void)
{
	//�ϐ��錾
	int nCntNumber;

	for (nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
	{
		if (m_pNumber[nCntNumber] != NULL)
		{
			m_pNumber[nCntNumber]->Update();
			m_pNumber[nCntNumber]->SetNumber(m_nScoreData[nCntNumber]);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CScoreGame::Draw(void)
{
	//�ϐ��錾
	int nCntNumber;

	for (nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
	{
		if (m_pNumber[nCntNumber] != NULL)
		{
			m_pNumber[nCntNumber]->Draw();
		}
	}
}

//=============================================================================
// �ݒ菈��
//=============================================================================
void CScoreGame::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//�ϐ��錾
	int nCntNumber;

	for (nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
	{
		if (m_pNumber[nCntNumber] == NULL)
		{
			m_pNumber[nCntNumber] = CNumber2D::Create(D3DXVECTOR3(pos.x + (nCntNumber * (size.x * 2)), pos.y, pos.z), size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			if (m_pNumber[nCntNumber] != NULL)
			{
				m_pNumber[nCntNumber]->SetbUse(true);
			}
		}
	}
}

//=============================================================================
//
// �X�R�A�����L���O
//
//=============================================================================

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScoreRanking::CScoreRanking(int nPriority) :CScore(nPriority)
{
	//�ϐ��錾
	int nCntNumber;

	for (int nCntRank = 0; nCntRank < RANK; nCntRank++)
	{
		for (nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
		{
			m_pNumber[nCntRank][nCntNumber] = NULL;
		}
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScoreRanking::~CScoreRanking()
{
	//�ϐ��錾
	int nCntNum;

	for (int nCntRank = 0; nCntRank < RANK; nCntRank++)
	{
		for (nCntNum = 0; nCntNum < MAX_NUMBER; nCntNum++)
		{
			m_nNumber[nCntRank][nCntNum] = 0;
		}
	}	
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScoreRanking::Init()
{
	//�ϐ��錾
	int nCntNum;
	m_bFlash = false;	//�V�����X�R�A�̍X�V������Ԃ���Ȃ�

	for (int nCntRank = 0; nCntRank < RANK; nCntRank++)
	{
		for (nCntNum = 0; nCntNum < MAX_NUMBER; nCntNum++)
		{
			m_nNumber[nCntRank][nCntNum] = 0;
		}

		//�X�R�A�̐ݒ�
		m_nHighScore[nCntRank] = CManager::GetRankScore(nCntRank);
		m_Flash[nCntRank] = FLASH_OFF;
	}

	//�����L���O�̃\�[�g
	SortRanking();

	Set(m_pos, m_size);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CScoreRanking::Uninit(void)
{
	//�ϐ��錾
	int nCntNumber;

	//�����L���O�̃X�R�A��ۑ�����
	for (int nCntRank = 0; nCntRank < RANK; nCntRank++)
	{
		CManager::SetRankScore(nCntRank, m_nHighScore[nCntRank]);
	}

	for (int nCntRank = 0; nCntRank < RANK; nCntRank++)
	{
		for (nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
		{
			if (m_pNumber[nCntRank][nCntNumber] != NULL)
			{
				m_pNumber[nCntRank][nCntNumber]->Uninit();

				delete m_pNumber[nCntRank][nCntNumber];
				m_pNumber[nCntRank][nCntNumber] = NULL;
			}
		}
	}

	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CScoreRanking::Update(void)
{
	for (int nCntRank = 0; nCntRank < RANK; nCntRank++)
	{
		switch (m_Flash[nCntRank])
		{
		case FLASH_CLEAR:
			m_nTime++;	//���Ԃ̉��Z

			//�F�̐ݒ�
			for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
			{
				m_pNumber[nCntRank][nCntNumber]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
			}

			//��Ԃ̕ύX
			if ((m_nTime % CLEAR_TIME) == 0)
			{
				m_Flash[nCntRank] = FLASH_RED;
				m_nTime = 0;
			}
			break;

		case FLASH_RED:
			m_nTime++;	//���Ԃ̉��Z

			//�F�̐ݒ�
			for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
			{
				m_pNumber[nCntRank][nCntNumber]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			//��Ԃ̕ύX
			if ((m_nTime % RED_TIME) == 0)
			{
				m_Flash[nCntRank] = FLASH_CLEAR;
				m_nTime = 0;
			}
			break;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CScoreRanking::Draw(void)
{
	//�ϐ��錾
	int nCntNumber;
	for (int nCntRank = 0; nCntRank < RANK; nCntRank++)
	{
		for (nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
		{
			if (m_pNumber[nCntRank][nCntNumber] != NULL)
			{
				m_pNumber[nCntRank][nCntNumber]->Draw();
			}
		}
	}
}

//=============================================================================
// �ݒ菈��
//=============================================================================
void CScoreRanking::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//�ϐ��錾
	int nCntNumber;

	for (int nCntRank = 0; nCntRank < RANK; nCntRank++)
	{
		for (nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
		{
			if (m_pNumber[nCntRank][nCntNumber] == NULL)
			{
				m_pNumber[nCntRank][nCntNumber] = CNumber2D::Create(D3DXVECTOR3(pos.x + (nCntNumber * (size.x * 2)), pos.y + (nCntRank * 100.0f), pos.z), size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				if (m_pNumber[nCntRank][nCntNumber] != NULL)
				{
					m_pNumber[nCntRank][nCntNumber]->SetbUse(true);
					m_pNumber[nCntRank][nCntNumber]->SetNumber(m_nNumber[nCntRank][nCntNumber]);
				}
			}
		}
	}
}

//=============================================================================
// �����L���O�̃\�[�g
//=============================================================================
void CScoreRanking::SortRanking(void)
{
	//�ϐ��錾
	int nSubScore = 0;
	int nSubScore2 = 0;
	bool bGetNewScore = false;
	int nNum = 1;
	int nNum2 = 1;

	int nScore = CManager::GetScore();	//�}�l�[�W���[����l���擾

	//�����L���O�̃\�[�g
	for (int nCntRank = 0; nCntRank < RANK; nCntRank++)
	{
		for (int nCntRank2 = 0; nCntRank2 < RANK - 1; nCntRank2++)
		{
			if (m_nHighScore[nCntRank] > m_nHighScore[nCntRank2])
			{
				nSubScore = m_nHighScore[nCntRank2];
				m_nHighScore[nCntRank2] = m_nHighScore[nCntRank];
				m_nHighScore[nCntRank] = nSubScore;
			}
		}
	}
	
	for (int nCntRank = 0; nCntRank < RANK; nCntRank++)
	{
		if (m_nHighScore[nCntRank] < nScore)
		{
			nSubScore = m_nHighScore[nCntRank];
			m_nHighScore[nCntRank] = nScore;
			nScore = nSubScore;

			if (m_bFlash == false)
			{
				//��Ԃ̐ݒ�
				SetState(nCntRank, STATE_NEW);
				m_bFlash = true;
			}
		}
	}

	//�X�R�A������������
	nScore = 0;
	CManager::SetScore(nScore);

	for (int nCntRank = 0; nCntRank < RANK; nCntRank++)
	{
		//�ϐ��̏�����
		nNum = 1;
		nNum2 = 1;

		//�����̌v�Z
		for (int nCntNum = 0; nCntNum < MAX_NUMBER; nCntNum++)
		{
			nNum *= 10;
		}
		for (int nCntNum = 0; nCntNum < MAX_NUMBER - 1; nCntNum++)
		{
			nNum2 *= 10;
		}

		//�����̐ݒ�
		for (int nCntNum = 0; nCntNum < MAX_NUMBER; nCntNum++)
		{
			m_nNumber[nCntRank][nCntNum] = (m_nHighScore[nCntRank] % nNum) / nNum2;	//�i�[���鐔�����v�Z

			//�������P�����炷
			nNum = nNum / 10;
			nNum2 = nNum2 / 10;
		}
	}
}

//=============================================================================
// ��Ԃ̐ݒ�
//=============================================================================
void CScoreRanking::SetState(int nCnt, STATE state)
{
	if (state == STATE_NEW)
	{
		m_Flash[nCnt] = FLASH_CLEAR;
	}
}
