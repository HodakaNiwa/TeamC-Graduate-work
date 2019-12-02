//=============================================================================
//
// ���U���g���� [result.h]
// Author : ���� ����
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
#include "modebase.h"
#include "game.h"
#include "select.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_RANKING				(5)		// �����x�̐�
#define MAX_RESULTLOGO			(2)		// result���S�̍ő吔
#define CUNTRY					(8)		// ���̐�
#define RESULT_TEX				(21)	// ���U���g�̃e�N�X�`��
#define MAX_PLAYER				(7)		// �v���C���[�̍ő吔
#define CHARIMAGE_TEX			(8)
#define MAX_COLOR				(5)		//�F�̍ő吔

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CResultCamera;
class CFieldManager;
class CLoadTextObject;
class CLoadTextTerritory;
class CCharResult;
class CMoveUI;
class CScene2D;
class CResultCharUI;
class CScore;
class CSelect;

//=====================
// ��{�N���X
//=====================
class CResult : public CModeBase
{
public:
	typedef enum
	{
		TYPE_OVER = 0,
		TYPE_CLEAR,
		TYPE_MAX,
	}TYPE;

	typedef enum
	{
		STATE_WHITE = 0,
		STATE_ZOOM,
		STATE_RANKING,
		STATE_SCORERESULT,
	}STATE;

	typedef struct
	{
		int nCuntry;					//���̎��
		int nType;						//�^�C�v
		int nScore;						//�X�R�A
		int nCharNo;					//�L�����ԍ�
		int nControlerNo;				//�R���g���[���[�̔ԍ�
		CSelect::INPUTTYPE m_InputType;	//�R���g���[���[�̃^�C�v
	}CHARACTER_INFO;

	CResult();				// �R���X�g���N�^
	~CResult();				// �f�X�g���N�^
	void Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void LoadTex(void);
	void UnloadTex(void);
	static void SetType(TYPE type) { m_type = type; }
	LPDIRECT3DTEXTURE9 GetCuntryTex(int nCuntry) { return m_pCuntryTex[nCuntry]; }
	LPDIRECT3DTEXTURE9 GetTexture(int nTex) { return m_pTexture[nTex]; }
	int GetMaxPlayer(void) { return m_nMaxPlayer; }

private:
	void UpdateWhite(void);
	void UpdateZoom(void);
	void UpdateRanking(void);
	void UpdateScoreResult(void);
	void InitCharUI(void);
	void InitCharImage(void);
	void InitWinerChar(int nCuntry, int nType, int nNumPlayer);
	void InitScoreResult(void);
	void SortScore(void);
	void CreateThankyou(int nCnt);
	void UninitCharRanking(void);
	void CreateBarun(void);
	static TYPE m_type;					//���ʏ��

	STATE m_state;
	LPDIRECT3DTEXTURE9 m_pCuntryTex[CUNTRY];
	LPDIRECT3DTEXTURE9 m_pTexture[RESULT_TEX];
	LPDIRECT3DTEXTURE9 m_pCharaTex[MAX_CHARACTER];
	CResultCamera * m_pCamera;							//�J����
	CFieldManager * m_pFieldManager;					//�t�B�[���h�}�l�[�W���[
	CLoadTextObject * m_pLoadTextObj;					//�I�u�W�F�N�g�ǂݍ���
	CLoadTextTerritory * m_pLoadTerritory;				// �e���g���[�̓ǂݍ���
	CCharResult * m_pWinChar;							//�����L����
	CScene2D * m_pWhiteBack;							//���w�i
	CMoveUI * m_pNatinalFlag;							//����
	CMoveUI * m_pWinerName;								//���Җ�
	CMoveUI * m_pBackLine;								//���̃��C��
	CMoveUI * m_pRankNo1;								//�����N�i���o�[�P
	CMoveUI * m_pTotalLogo;								//���v���S
	CMoveUI * m_pGetNumLogo;							//�l����
	CMoveUI * m_pMakeNum;								//������
	CMoveUI * m_pRobbedNum;								//�D��ꂽ��
	CMoveUI * m_pCharImage[MAX_CHARACTER];				//�L�����摜
	CMoveUI * m_pRankNum[MAX_CHARACTER];				//�L�����̃����L���O�i���o�[
	CMoveUI * m_pFlag[MAX_CHARACTER];					//����
	CMoveUI * m_pTable[MAX_CHARACTER];					//�\
	CMoveUI * m_pNameFlam[MAX_CHARACTER];				//���O�̃t���[��
	CMoveUI * m_pName[MAX_CHARACTER];					//���O�̃t���[��
	CMoveUI * m_pNameJapanese[MAX_CHARACTER];			//���{��̖��O�\�L
	CMoveUI * m_pNumber[MAX_CHARACTER];					//����
	CMoveUI * m_pBackCharImage[MAX_CHARACTER];			//�L�����w�i
	CMoveUI * m_pThanksBack[MAX_CHARACTER];				//Thankyou�̔w�i
	CMoveUI * m_pThanksBG[MAX_CHARACTER];				//Thankyou�̔w�i
	CMoveUI * m_pThanks[MAX_CHARACTER];					//Thankyou
	CScore * m_pTotalScore[MAX_CHARACTER];				//���v�X�R�A
	CScore * m_pGetScore[MAX_CHARACTER];				//�l����
	CScore * m_pMakeScore[MAX_CHARACTER];				//�쐬��
	CScore * m_pRobotedScore[MAX_CHARACTER];			//�D��ꂽ��
	CResultCharUI * m_pResultCharUI[MAX_PLAYER];		//���U���g��UI
	CHARACTER_INFO m_CharInfo[MAX_CHARACTER];			//�L�����̍ő吔
	CHARACTER_INFO m_CharResultScore[MAX_CHARACTER];	//�L�������U���g�X�R�A�p�̕ϐ�

	int m_nCounter;
	int m_nTime;
	int m_nFadeOutTime;
	int m_nCountID;							//ID�̃J�E���^�[
	int m_nMaxPlayer;						//�v���C���[�̍ő吔
	int m_nCountMakeShape[MAX_CHARACTER];			//�}�`���������
	int m_nCountGetTerritory[MAX_CHARACTER];		//�e���g���[�̎擾��
	int m_nCountRobbotedTerritory[MAX_CHARACTER];	//�e���g���[�̒D��ꂽ��
	bool m_bWhiteIn;						//�t�F�[�h�C���o���邩�ǂ���
	bool m_bCharFlagDown[MAX_CHARACTER];	//�L�����N�^�[�摜�������t���O
	bool m_bThanksFlagDown[MAX_CHARACTER];	//�摜�������t���O
	D3DXCOLOR m_Color[MAX_COLOR];			//�F

};
#endif