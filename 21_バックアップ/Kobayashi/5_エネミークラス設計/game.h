//=============================================================================
//
// �Q�[���̏��� [game.h]
// Author : ��������
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "modebase.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPause;
class CGameCamera;
class CSceneMeshFiled;
class CLoadTextObject;
class CLoadTextTerritory;
class CPlayer;
class CEnemy;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_NUMPLAYER (4)	//�v���C���[�̍ő吔
#define ENEMY_MAX (1)		//	�G�l�~�[�̍ő吔

//=====================
// ��{�N���X
//=====================
class CGame : public CModeBase
{
public:
	typedef enum
	{
		GAMESTATE_NONE = 0,					//�������Ă��Ȃ����
		GAMESTATE_NORMAL,					//�ʏ���
		GAMESTATE_CLEAR,					//�Q�[���N���A
		GAMESTATE_OVER,						//�Q�[���I�[�o�[
		GAMESTATE_END,						//�Q�[���I��
		GAMESTATE_MAX						//�ő吔
	}GAMESTATE;

	CGame();				//�R���X�g���N�^
	~CGame();				//�f�X�g���N�^
	void Init(void);		//����������
	void Uninit(void);		//�I������
	void Update(void);		//�X�V����
	void Draw(void);		//�`�揈��

	//--------------------
	//	Set & Get
	//--------------------
	static void SetGameState(GAMESTATE state) { m_gameState = state; }
	static void SetPause(bool bPause);
	static bool GetPause(void) { return m_bPause; }
	static GAMESTATE GetGameState(void) { return m_gameState; }
	CGameCamera * GetGameCamera(int nIdx) { return m_pGameCamera[nIdx]; }
	//	---<<���т��ǉ����܂���>>��	��	��	��	��	��	��	��	��	��
	CEnemy* GetEnemyInfo(int nCnt) { return m_pEnemy[nCnt]; }
	CLoadTextTerritory* GetTerritoryInfo() { return m_pLoadTerritory; }
	//	��	��	��	��	��	��	��	��	��	��	��	��	��	��	��	��


private:
	static GAMESTATE m_gameState;				//�Q�[�����
	GAMESTATE m_NowGameState;					//���݂̃Q�[����
	int m_nCounterGameState;					//��ԃJ�E���^
	static CPause *m_pPause;					//�|�[�Y�̃|�C���^
	static bool m_bPause;						//���݃|�[�Y���|�[�Y����Ȃ���
	
	static CSceneMeshFiled * m_pMeshFiled;		//���b�V���t�B�[���h
	CLoadTextObject * m_pLoadObject;			// �I�u�W�F�N�g�̓ǂݍ���
	CLoadTextTerritory * m_pLoadTerritory;		// �e���g���[�̓ǂݍ���
	CGameCamera * m_pGameCamera[4];				// �Q�[���J����
	CPlayer *m_pPlayer[MAX_NUMPLAYER];

	//	---<<���т��ǉ����܂���>>��	��	��	��	��	��	��	��	��	��
	CEnemy *m_pEnemy[MAX_NUMPLAYER];
	//	��	��	��	��	��	��	��	��	��	��	��	��	��	��	��	��

};
#endif