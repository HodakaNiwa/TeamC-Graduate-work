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
class CRotationCamera;
class CSceneMeshFiled;
class CLoadTextObject;
class CLoadTextTerritory;
class CLoadTextEffect;
class CPlayer;
class CTime;
class CEnemy;
class CCamera;
class CFieldManager;
class CUI;
class CCharacter;
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_NUMPLAYER (4)	//�v���C���[�̍ő吔
#define ENEMY_MAX (8)		//�G�l�~�[�̍ő吔
#define MAX_GAMECAMERA (4)	//�Q�[���J�����̍ő吔
#define MAX_CHARACTER (8)	//�L�����N�^�[�̍ő吔
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
		GAMESTATE_FIRSTCOUNTDOWN,			//�Q�[���J�n���̃J�E���g�_�E��
		GAMESTATE_MAX						//�ő吔
	}GAMESTATE;

	

	CGame();				//�R���X�g���N�^
	~CGame();				//�f�X�g���N�^
	void Init(void);		//����������
	void Uninit(void);		//�I������
	void Update(void);		//�X�V����
	void Draw(void);		//�`�揈��

	void CreateInitPlayer(void);		//�v���C���[�̐���
	void CreateInitCamera(void);		//�J�����̐���
	//--------------------
	//	Set & Get
	//--------------------
	static void SetGameState(GAMESTATE state) { m_gameState = state; }
	static void SetPause(bool bPause);
	static bool GetPause(void) { return m_bPause; }
	static void SetDebugCamera(bool bDebugCamera) { m_bDebugCamera = bDebugCamera; }
	static bool GetDebugCamera(void) { return m_bDebugCamera; }
	static CLoadTextEffect * GetEffectLoad(void) { return m_pLoadEffect; }
	static GAMESTATE GetGameState(void) { return m_gameState; }
	static int GetScore(int nIdx) { return m_nScore[nIdx]; }
	CGameCamera * GetGameCamera(int nIdx) { return m_pGameCamera[nIdx]; }
	CPlayer * GetPlayer(int nNumPlayer) { return m_pPlayer[nNumPlayer]; }
	CFieldManager * GetFieldManger(void) { return m_pFieldManager; }
	int GetNumPlay(void) { return m_nNumPlay; }
	CUI *GetUI(void) { return m_pUI; }
	//	---<<���т��ǉ����܂���>>��	��	��	��	��	��	��	��	��	��
	CCharacter* GetChara(int nCnt) { return m_pCharacter[nCnt]; }
	CEnemy * GetEnemy(int nNum) { return m_pEnemy[nNum]; }
	int GetEnemyNum(void) { return m_nEnemyNum; }
	//	��	��	��	��	��	��	��	��	��	��	��	��	��	��	��	��
	int GetCountry(int nNum) { return m_nCountry[nNum]; }

	static int GetEnemyNumResult(void) { return m_nEnemyNumResult; }
private:
	//--------------------
	//	�֐�
	//--------------------
	void CreateUI(void);
	void SettingCharaPosition(void);
	void CreateMapCollider(void);
	void ReleaseUI(void);
	void UpdateUI(void);


	//--------------------
	//	�����o�ϐ�
	//--------------------
	static GAMESTATE m_gameState;				//�Q�[�����
	GAMESTATE m_NowGameState;					//���݂̃Q�[����
	int m_nCounterGameState;					//��ԃJ�E���^
	static CPause *m_pPause;					//�|�[�Y�̃|�C���^
	static bool m_bDebugCamera;					//�f�o�b�N�J�����̃I���I�t
	static bool m_bPause;						//���݃|�[�Y���|�[�Y����Ȃ���
	static int m_nScore[MAX_CHARACTER];			// �X�R�A��ۑ�
	static CSceneMeshFiled * m_pMeshFiled;		//���b�V���t�B�[���h
	static CLoadTextEffect * m_pLoadEffect;		// �G�t�F�N�g�ǂݍ���
	CLoadTextObject * m_pLoadObject;			// �I�u�W�F�N�g�̓ǂݍ���
	CLoadTextTerritory * m_pLoadTerritory;		// �e���g���[�̓ǂݍ���
	CGameCamera * m_pGameCamera[MAX_GAMECAMERA];				// �Q�[���J����
	CPlayer *m_pPlayer[MAX_NUMPLAYER];
	static CTime *m_pTime[2];
	CEnemy *m_pEnemy[ENEMY_MAX];
	CCamera * m_pDebugCamera;						//
	int m_nNumPlay;									//�v���C���[�̑���
	int m_nEnemyNum;								//�G�l�~�[�̑���
	static int m_nEnemyNumResult;							//���U���g�p�G�l�~�[�̑���

	int m_nPlayerType[4];							//�v���C���[�̃^�C�v
	int m_nCountry[4];								//��
	CFieldManager * m_pFieldManager;				//�n�ʃ}�l�[�W���[
	CUI *m_pUI;										// UI�N���X�ւ̃|�C���^
	CCharacter* m_pCharacter[MAX_CHARACTER];		//�L�����N�^�[�̑���
	CRotationCamera *m_pRotCamera;					//��]�J����

	//	�G�l�~�[�̃^�C�v�I��(�v���C���[�̑I�΂ꂽ�^�C�v�ƂȂ�ׂ����Ԃ�Ȃ��悤�ɂ��邽��)
	int m_nSpeedNo, m_nPowerNo, m_nSweeperNo;
	bool m_bTypeMax;
	D3DXVECTOR3 InitPos[MAX_CHARACTER];	//	�e�L�����N�^�[�̏����ʒu
};
#endif