//=============================================================================
//
// �X�R�A�̏��� [score.h]
// Author : ���� �����@[ SATO ASUMI ]
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// �O���錾
//=============================================================================
class CNumber2D;
class CIcon2D;
class CMoveUI;

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_NUMBER	(3)
#define RANK		(8)
#define CUNTRY		(8)

//=============================================================================
// �X�R�A�N���X
//=============================================================================
class CScore : public CScene
{
public:
	typedef enum
	{
		TYPE_GAME,
		TYPE_RANKING,
		TYPE_NONE
	}TYPE;

	//�����o�֐�
	CScore(int nPriority);
	virtual ~CScore();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	static CScore * Create(TYPE type, int Priority, D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void AddScore(int nScore);
	int GetScore(void);
	void SetSizePos(const D3DXVECTOR3 pos, const D3DXVECTOR3 size) { m_size = size; m_pos = pos; };

protected:
	int m_nScore;
	int m_nScoreData[MAX_NUMBER];
	D3DXVECTOR3 m_size;
	D3DXVECTOR3 m_pos;
};

//=============================================================================
// �X�R�A�Q�[��
//=============================================================================
class CScoreGame : public CScore
{
public:
	CScoreGame(int Priority);
	~CScoreGame();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	//�֐��錾
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	CNumber2D * m_pNumber[MAX_NUMBER];	// �i���o�[�̃|�C���^
	int m_nNumber[MAX_NUMBER];			// �e�����ԍ�
};

//=============================================================================
// �X�R�A�����L���O
//=============================================================================
class CScoreRanking : public CScore
{
public:
	typedef enum
	{
		STATE_NEW = 0,
		STATE_NOMAL,
	}STATE;

	typedef enum
	{
		FLASH_CLEAR = 0,
		FLASH_RED,
		FLASH_OFF,
	}FLASH;

	CScoreRanking(int Priority);
	~CScoreRanking();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void LoadTex(void);
	static void UnloadTex(void);

private:
	void SortRanking(void);
	static void SetState(int nCnt, STATE state);

	//�֐��錾
	static LPDIRECT3DTEXTURE9 m_pCuntryTex[CUNTRY];
	static LPDIRECT3DTEXTURE9 m_pCuntryNameTex[CUNTRY];
	static LPDIRECT3DTEXTURE9 m_pCharNameTex[CUNTRY];
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	CNumber2D * m_pNumber[RANK][MAX_NUMBER];					//�i���o�[�̃|�C���^
	CMoveUI * m_pCuntryFlag[RANK];								//����
	CMoveUI * m_pCuntryName[RANK];								//����
	CMoveUI * m_pCharName[RANK];								//�L������
	static int			m_nNumber[RANK][MAX_NUMBER];			//�e�����ԍ�
	static int			m_nHighScore[RANK];						//�X�R�A�̐�
	int					m_nCuntry[RANK];						//���̎��
	int					m_nCharType[RANK];						//�L�����^�C�v
	static FLASH		m_Flash[RANK];							//�t���b�V�����
	bool				m_bFlash;								//�V�����X�R�A���擾�������ǂ���
	int					m_nTime[RANK];							//���Ԃ̉��Z
	int					m_nCounter = 0;
	bool                m_bMoving[RANK];
	int                 m_nMovingIdx;
	D3DXVECTOR3			m_ScorePos[RANK];
};

#endif
