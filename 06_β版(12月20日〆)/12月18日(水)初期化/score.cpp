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
#include "game.h"
#include "select.h"
#include "moveUI.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define CLEAR_TIME	(15)
#define RED_TIME	(20)
#define GAMESCORE_INTERVAL (1.8f)
#define SCORE_DESTPOS (1000.0f)
#define FLAG_POS_X	  (310.0f)
#define NAME_POS_X	  (215.0f)
#define CHARNAME_POS_X	(560.0f)
#define POS_Y	  (180.0f)

//����
#define JAPAN_FLAG			("data/TEXTURE/Select/NationalFlag/Japan.png")
#define ROSSIA_FLAG			("data/TEXTURE/Select/NationalFlag/Russia.png")
#define AMERICA_FLAG		("data/TEXTURE/Select/NationalFlag/America.png")
#define BLAZIL_FLAG			("data/TEXTURE/Select/NationalFlag/Brazil.png")
#define ENGLAND_FLAG		("data/TEXTURE/Select/NationalFlag/Engrand.png")
#define ITARY_FLAG			("data/TEXTURE/Select/NationalFlag/Italy.png")
#define NEWZYLAND_FLAG		("data/TEXTURE/Select/NationalFlag/Newzealand.png")
#define SOUTHAFRICA_FLAG	("data/TEXTURE/Select/NationalFlag/SouthAfrica.png")

//����
#define JAPAN_NAME			("data/TEXTURE/Select/CountryName/JPN.png")
#define ROSSIA_NAME			("data/TEXTURE/Select/CountryName/RUS.png")
#define AMERICA_NAME		("data/TEXTURE/Select/CountryName/USA.png")
#define BLAZIL_NAME			("data/TEXTURE/Select/CountryName/BRA.png")
#define ENGLAND_NAME		("data/TEXTURE/Select/CountryName/GBR.png")
#define ITARY_NAME			("data/TEXTURE/Select/CountryName/ITA.png")
#define NEWZYLAND_NAME		("data/TEXTURE/Select/CountryName/NGL.png")
#define SOUTHAFRICA_NAME	("data/TEXTURE/Select/CountryName/ZAF.png")

//�L������
#define JAPAN_NAME_CHAR			("data/TEXTURE/Ranking/CharName/JapanName.png")
#define ROSSIA_NAME_CHAR		("data/TEXTURE/Ranking/CharName/RossiaName.png")
#define AMERICA_NAME_CHAR		("data/TEXTURE/Ranking/CharName/AmericaName.png")
#define BLAZIL_NAME_CHAR		("data/TEXTURE/Ranking/CharName/BrazilName.png")
#define ENGLAND_NAME_CHAR		("data/TEXTURE/Ranking/CharName/EnglandName.png")
#define ITARY_NAME_CHAR			("data/TEXTURE/Ranking/CharName/ItalyName.png")
#define NEWZYLAND_NAME_CHAR		("data/TEXTURE/Ranking/CharName/NewzyLandName.png")
#define SOUTHAFRICA_NAME_CHAR	("data/TEXTURE/Ranking/CharName/SouthAfricaName.png")

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
//�X�R�A�N���X

//�X�R�A�Q�[���N���X

//�X�R�A�����L���O�N���X
int CScoreRanking::m_nNumber[RANK][MAX_NUMBER] = {};
int CScoreRanking::m_nHighScore[RANK] = {};
CScoreRanking::FLASH CScoreRanking::m_Flash[RANK] = {};
LPDIRECT3DTEXTURE9 CScoreRanking::m_pCuntryTex[CUNTRY] = {};
LPDIRECT3DTEXTURE9 CScoreRanking::m_pCuntryNameTex[CUNTRY] = {};
LPDIRECT3DTEXTURE9 CScoreRanking::m_pCharNameTex[CUNTRY] = {};

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
CScore::CScore(int nPriority):CScene(nPriority , OBJTYPE_SCORE)
{
	m_nScore = 0;
	for (int nCnt = 0; nCnt < MAX_NUMBER; nCnt++)
	{
		m_nScoreData[nCnt] = 0;
	}
	m_size = INITIALIZE_VECTOR3;
	m_pos = INITIALIZE_VECTOR3;
}

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
CScoreGame::CScoreGame(int nPriority):CScore(nPriority)
{
	for (int nCnt = 0; nCnt < MAX_NUMBER; nCnt++)
	{
		m_pNumber[nCnt] = NULL;
		m_nNumber[nCnt] = 0;
	}
}

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

	CGame *pGame = CManager::GetGame();				// �Q�[���̎擾���ǉ�(�悵�낤)
	CEventCamera *pEveCam = pGame->GetEveCam();		// �C�x���g�J�����̎擾���ǉ�(�悵�낤)
	if (pEveCam == NULL)							// �C�x���g�J�����������Ă����灩�ǉ�(�悵�낤)
	{
		for (nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
		{
			if (m_pNumber[nCntNumber] != NULL)
			{
				m_pNumber[nCntNumber]->Draw();
			}
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
			m_pNumber[nCntNumber] = CNumber2D::Create(D3DXVECTOR3(pos.x + (nCntNumber * (size.x * GAMESCORE_INTERVAL)), pos.y, pos.z), size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

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
	m_bFlash = false;
	m_nCounter = 0;
	m_nMovingIdx = 0;

	for (int nCntRank = 0; nCntRank < RANK; nCntRank++)
	{
		for (nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
		{
			m_pNumber[nCntRank][nCntNumber] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < RANK; nCnt++)
	{
		m_nCuntry[nCnt] = 0;
		m_nCharType[nCnt] = 0;
		m_nTime[nCnt] = 0;
		m_bMoving[nCnt] = false;
		m_ScorePos[nCnt] = INITIALIZE_VECTOR3;
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
	
	for (int nCntRank = 0; nCntRank < RANK; nCntRank++)
	{
		for (nCntNum = 0; nCntNum < MAX_NUMBER; nCntNum++)
		{
			m_nNumber[nCntRank][nCntNum] = 0;
		}

		//�ϐ��̏�����
		m_bFlash = false;
		m_nTime[nCntRank] = 0;
		m_nCuntry[nCntRank] = 0;
		m_nCharType[nCntRank] = 0;
		m_pCuntryFlag[nCntRank] = NULL;
		m_pCuntryName[nCntRank] = NULL;
		m_pCharName[nCntRank] = NULL;

		//�X�R�A�̐ݒ�
		m_nHighScore[nCntRank] = CManager::GetRankScore(nCntRank);
		m_nCuntry[nCntRank] = CManager::GetRankCuntry(nCntRank);
		m_nCharType[nCntRank] = CManager::GetRankCharType(nCntRank);
		m_Flash[nCntRank] = FLASH_OFF;
	}
	//�����L���O�̃\�[�g
	SortRanking();

	Set(m_pos, m_size);

	for (int nCnt = 0; nCnt < RANK; nCnt++)
	{
		m_ScorePos[nCnt] = m_pNumber[nCnt][0]->GetPos();
		m_bMoving[nCnt] = false;

		//�����̐���
		if (m_pCuntryFlag[nCnt] == NULL)
		{
			m_pCuntryFlag[nCnt] = CMoveUI::Create(D3DXVECTOR3(1460.0f, POS_Y + (nCnt * 60.0f), 0.0f), D3DXVECTOR3(37.0f, 27.0f, 0.0f), m_pCuntryTex[m_nCuntry[nCnt]]);
		}

		//�����̐���
		if (m_pCuntryName[nCnt] == NULL)
		{
			m_pCuntryName[nCnt] = CMoveUI::Create(D3DXVECTOR3(1400.0f, POS_Y + (nCnt * 60.0f), 0.0f), D3DXVECTOR3(50.0f, 27.0f, 0.0f), m_pCuntryNameTex[m_nCuntry[nCnt]]);
		}

		//�L�������̐���
		if (m_pCharName[nCnt] == NULL)
		{
			m_pCharName[nCnt] = CMoveUI::Create(D3DXVECTOR3(1660.0f, POS_Y + (nCnt * 60.0f), 0.0f), D3DXVECTOR3(200.0f, 27.0f, 0.0f), m_pCharNameTex[m_nCuntry[nCnt]]);

			//UV�ݒ�
			int nType = m_nCharType[nCnt];
			m_pCharName[nCnt]->SetTexUV(0.0f, 1.0f, 0.0f + (nType * 0.25f), 0.25f + (nType * 0.25f));
		}
	}

	m_bMoving[RANK - 1] = true;
	m_nMovingIdx = RANK -1;

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
		CManager::SetRankScore(nCntRank, m_nHighScore[nCntRank]);	//�X�R�A�̕ۑ�
		CManager::SetRankCuntry(nCntRank, m_nCuntry[nCntRank]);		//���̕ۑ�
		CManager::SetRankCharType(nCntRank, m_nCharType[nCntRank]);	//�L�����^�C�v�̕ۑ�

		m_pCuntryFlag[nCntRank] = NULL;
		m_pCuntryName[nCntRank] = NULL;
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
			m_nTime[nCntRank]++;	//���Ԃ̉��Z

			//�F�̐ݒ�
			for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
			{
				m_pNumber[nCntRank][nCntNumber]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
			}

			//��Ԃ̕ύX
			if ((m_nTime[nCntRank] % CLEAR_TIME) == 0)
			{
				m_Flash[nCntRank] = FLASH_RED;
				m_nTime[nCntRank] = 0;
			}
			break;

		case FLASH_RED:
			m_nTime[nCntRank]++;	//���Ԃ̉��Z

						//�F�̐ݒ�
			for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
			{
				m_pNumber[nCntRank][nCntNumber]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			//��Ԃ̕ύX
			if ((m_nTime[nCntRank] % RED_TIME) == 0)
			{
				m_Flash[nCntRank] = FLASH_CLEAR;
				m_nTime[nCntRank] = 0;
			}
			break;
		}
	}

	// �J�E���^�[�𑝉�
	m_nCounter++;

	if (m_nCounter == 10 && 0 <= m_nMovingIdx)
	{
		m_nCounter = 0;
		m_nMovingIdx--;
		m_bMoving[m_nMovingIdx] = true;
	}


	for (int nCnt = 0; nCnt < RANK; nCnt++)
	{
		if (m_bMoving[nCnt] == true)
		{
			m_ScorePos[nCnt].x -= 60.0f;

			//�ړ�����
			if (m_pCuntryFlag[nCnt] != NULL) { m_pCuntryFlag[nCnt]->UpdateMoveDest(D3DXVECTOR3(FLAG_POS_X, POS_Y + (nCnt * 60.0f), 0.0f), 60.0f); }
			if (m_pCuntryName[nCnt] != NULL) { m_pCuntryName[nCnt]->UpdateMoveDest(D3DXVECTOR3(NAME_POS_X, POS_Y + (nCnt * 60.0f), 0.0f), 60.0f); }
			if (m_pCharName[nCnt] != NULL) { m_pCharName[nCnt]->UpdateMoveDest(D3DXVECTOR3(CHARNAME_POS_X, POS_Y + (nCnt * 60.0f), 0.0f), 60.0f); }
		}

		// �ړI�̈ʒu����ɓ������Ă�����߂�
		if (m_ScorePos[nCnt].x < SCORE_DESTPOS)
		{
			m_ScorePos[nCnt].x = SCORE_DESTPOS;
		}
		m_pNumber[nCnt][2]->SetPos(D3DXVECTOR3(m_ScorePos[nCnt].x, 180.0f + (60 * nCnt), 0.0f));
		m_pNumber[nCnt][1]->SetPos(D3DXVECTOR3(m_ScorePos[nCnt].x - 40, 180.0f + (60 * nCnt), 0.0f));
		m_pNumber[nCnt][0]->SetPos(D3DXVECTOR3(m_ScorePos[nCnt].x - 80, 180.0f + (60 * nCnt), 0.0f));
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
				m_pNumber[nCntRank][nCntNumber] = CNumber2D::Create(D3DXVECTOR3(pos.x + (nCntNumber * (size.x * 2)), pos.y + (nCntRank * 60.0f), pos.z), size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

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
	int nScore = 0;
	int nSubCuntry = 0;
	int nCuntry = 0;
	int nSubCharType = 0;
	int nCharType = 0;
	int nMaxPlayer = CSelect::GetEntryPlayer();

	for (int nCntChar = 0; nCntChar < nMaxPlayer; nCntChar++)
	{
		nScore = CManager::GetScore(nCntChar);			//�}�l�[�W���[����l���擾
		nCuntry = CManager::GetCuntry(nCntChar);		//�}�l�[�W���[���獑�̃^�C�v���擾
		nCharType = CManager::GetCharType(nCntChar);	//�}�l�[�W���[����L�����^�C�v�擾

		for (int nCntRank = 0; nCntRank < RANK; nCntRank++)
		{
			if (m_nHighScore[nCntRank] < nScore)
			{
				//�X�R�A�̓��ւ�
				nSubScore = m_nHighScore[nCntRank];
				m_nHighScore[nCntRank] = nScore;
				nScore = nSubScore;

				//���̓��ւ�
				nSubCuntry = m_nCuntry[nCntRank];
				m_nCuntry[nCntRank] = nCuntry;
				nCuntry = nSubCuntry;

				//�L�����^�C�v�̓��ւ�
				nSubCharType = m_nCharType[nCntRank];
				m_nCharType[nCntRank] = nCharType;
				nCharType = nSubCharType;

				if (m_bFlash == false)
				{
					for (int nCnt = RANK - 1; nCnt > nCntRank; nCnt--)
					{
						if (m_Flash[nCnt] == FLASH_CLEAR)
						{
							m_Flash[nCnt] = FLASH_OFF;
							
							if (nCnt < RANK - 1) { m_Flash[nCnt + 1] = FLASH_CLEAR; }
						}
					}

					//��Ԃ̐ݒ�
					SetState(nCntRank, STATE_NEW);
					m_bFlash = true;
				}
			}
		}

		m_bFlash = false;
	}

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

//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
void CScoreRanking::LoadTex(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�����e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, JAPAN_FLAG, &m_pCuntryTex[0]);
	D3DXCreateTextureFromFile(pDevice, ROSSIA_FLAG, &m_pCuntryTex[1]);
	D3DXCreateTextureFromFile(pDevice, AMERICA_FLAG, &m_pCuntryTex[2]);
	D3DXCreateTextureFromFile(pDevice, BLAZIL_FLAG, &m_pCuntryTex[3]);
	D3DXCreateTextureFromFile(pDevice, ENGLAND_FLAG, &m_pCuntryTex[4]);
	D3DXCreateTextureFromFile(pDevice, ITARY_FLAG, &m_pCuntryTex[5]);
	D3DXCreateTextureFromFile(pDevice, NEWZYLAND_FLAG, &m_pCuntryTex[6]);
	D3DXCreateTextureFromFile(pDevice, SOUTHAFRICA_FLAG, &m_pCuntryTex[7]);

	//�����e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, JAPAN_NAME, &m_pCuntryNameTex[0]);
	D3DXCreateTextureFromFile(pDevice, ROSSIA_NAME, &m_pCuntryNameTex[1]);
	D3DXCreateTextureFromFile(pDevice, AMERICA_NAME, &m_pCuntryNameTex[2]);
	D3DXCreateTextureFromFile(pDevice, BLAZIL_NAME, &m_pCuntryNameTex[3]);
	D3DXCreateTextureFromFile(pDevice, ENGLAND_NAME, &m_pCuntryNameTex[4]);
	D3DXCreateTextureFromFile(pDevice, ITARY_NAME, &m_pCuntryNameTex[5]);
	D3DXCreateTextureFromFile(pDevice, NEWZYLAND_NAME, &m_pCuntryNameTex[6]);
	D3DXCreateTextureFromFile(pDevice, SOUTHAFRICA_NAME, &m_pCuntryNameTex[7]);

	//�L�������e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, JAPAN_NAME_CHAR, &m_pCharNameTex[0]);
	D3DXCreateTextureFromFile(pDevice, ROSSIA_NAME_CHAR, &m_pCharNameTex[1]);
	D3DXCreateTextureFromFile(pDevice, AMERICA_NAME_CHAR, &m_pCharNameTex[2]);
	D3DXCreateTextureFromFile(pDevice, BLAZIL_NAME_CHAR, &m_pCharNameTex[3]);
	D3DXCreateTextureFromFile(pDevice, ENGLAND_NAME_CHAR, &m_pCharNameTex[4]);
	D3DXCreateTextureFromFile(pDevice, ITARY_NAME_CHAR, &m_pCharNameTex[5]);
	D3DXCreateTextureFromFile(pDevice, NEWZYLAND_NAME_CHAR, &m_pCharNameTex[6]);
	D3DXCreateTextureFromFile(pDevice, SOUTHAFRICA_NAME_CHAR, &m_pCharNameTex[7]);
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CScoreRanking::UnloadTex(void)
{
	for (int nCnt = 0; nCnt < CUNTRY; nCnt++)
	{
		if (m_pCuntryTex[nCnt] != NULL)
		{//�����e�N�X�`���̔j��
			m_pCuntryTex[nCnt]->Release();
			m_pCuntryTex[nCnt] = NULL;
		}

		if (m_pCuntryNameTex[nCnt] != NULL)
		{//�����e�N�X�`���̔j��
			m_pCuntryNameTex[nCnt]->Release();
			m_pCuntryNameTex[nCnt] = NULL;
		}

		if (m_pCharNameTex[nCnt] != NULL)
		{//�L�������e�N�X�`���̔j��
			m_pCharNameTex[nCnt]->Release();
			m_pCharNameTex[nCnt] = NULL;
		}
	}
}