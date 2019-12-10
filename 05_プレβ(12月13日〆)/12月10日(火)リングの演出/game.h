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
class CEffectManager;
class CEmitter;
class CRobot;	
class CRobotUI;	
class CEventCamera; 
class CIcon2D;
class CRingRender; //��

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
		GAMESTATE_NONE = 0,				// �������Ă��Ȃ����
		GAMESTATE_NORMAL,				// �ʏ���
		GAMESTATE_CLEAR,				// �Q�[���N���A
		GAMESTATE_OVER,					// �Q�[���I�[�o�[
		GAMESTATE_END,					// �Q�[���I��
		GAMESTATE_FIRSTCOUNTDOWN,		// �Q�[���J�n���̃J�E���g�_�E��
		GAMESTATE_MAX					// �ő吔
	}GAMESTATE;

	CGame();							// �R���X�g���N�^
	~CGame();							// �f�X�g���N�^
	void Init(void);					// ����������
	void Uninit(void);					// �I������
	void Update(void);					// �X�V����
	void Draw(void);					// �`�揈��

	void CreateInitPlayer(void);		// �v���C���[�̐���
	void CreateInitCamera(void);		// �J�����̐���
	void CreateInitEventCamera(void);	// �C�x���g�J�����̐���
	void CreateRobot(void);				// ���{�b�g�̐���

	// �G���A�{�[�i�X�C�x���g�p(�O���ŌĂ�)
	void ReleaseAreaBounsEventUI(void);
	void HideEventField(void);
	void UpdateAreaBonusField_Start(void);
	void UpdateAreaBonusField_Finish(void);

	//--------------------
	//	Set & Get
	//--------------------
	static void SetGameState(GAMESTATE state) { m_gameState = state; }
	static void SetDebugCamera(bool bDebugCamera) { m_bDebugCamera = bDebugCamera; }
	static bool GetDebugCamera(void) { return m_bDebugCamera; }
	static CLoadTextEffect * GetEffectLoad(void) { return m_pLoadEffect; }
	static GAMESTATE GetGameState(void) { return m_gameState; }
	static int GetScore(int nIdx) { return m_nScore[nIdx]; }
	CEffectManager *GetEffectManager(void) { return m_pEffectManager; }
	CGameCamera * GetGameCamera(int nIdx) { return m_pGameCamera[nIdx]; }
	CFieldManager * GetFieldManger(void) { return m_pFieldManager; }
	int GetNumPlay(void) { return m_nNumPlay; }
	CUI *GetUI(void) { return m_pUI; }
	CRobotUI *GetRobotUI(void) { return m_pRobotUI; }			

	//	---<<���т��ǉ����܂���>>��	��	��	��	��	��	��	��	��	��
	CCharacter* GetChara(int nCnt) { return m_pCharacter[nCnt]; }
	int GetEnemyNum(void) { return m_nEnemyNum; }
	//	��	��	��	��	��	��	��	��	��	��	��	��	��	��	��	��
	int GetCountry(int nNum) { return m_nCountry[nNum]; }

	//���U���g�Ŏg�p����֐�
	static int GetEnemyNumResult(void) { return m_nEnemyNumResult; }
	static int GetCountMakeShape(int nIndx) { return m_nCountMakeScore[nIndx]; }
	static int GetCountGetTerritory(int nIndx) { return m_nCountGetTerritry[nIndx]; }
	static int GetRobbotedTerritory(int nIndx) { return m_nCountRobottedTerritory[nIndx]; }

	//���{�b�gGet&Set�֐�
	void SetRobot(CRobot *pRobot) { m_pRoboCharacter = pRobot; }	
	CRobot *GetRobot(void) { return m_pRoboCharacter; }				
	void SetEveCam(CEventCamera *pEventCamera) { m_pEventCamera = pEventCamera; }	
	CEventCamera *GetEveCam(void) { return m_pEventCamera; }						
	void SetEveCamFlag(bool EndFlag) { m_bEveCam = EndFlag; }		
	bool GetEVeCamFlag(void) { return m_bEveCam; }		

	// �G���A�{�[�i�X�C�x���g�pSet&Get�֐�
	void SetAreaBonusEventFlag(const bool bAreaBonusEventFlag) { m_bAreaBonusEventFlag = bAreaBonusEventFlag; }
	bool GetAreaBonusEventFlag(void) { return m_bAreaBonusEventFlag; }
	int GetAreaBonusIdx(void) { return m_nAreaBonusIdx; }

	//�����O�p
	CRingRender * GetRingRender(void) { return m_pRingRender; }

private:
	//--------------------
	//	�֐�
	//--------------------
	void CreateUI(void);
	void SettingCharaPosition(void);
	void CreateMapCollider(void);
	void ReleaseUI(void);
	void UpdateUI(void);
	void DrawUI_Icon(const int nIdx);
	void GetCharInfo(void);

	// �G���A�{�[�i�X�C�x���g�p�֐�
	void StartAreaBonusEvent(void);
	void CreateEventCamera_AreaBonusStart(void);
	void CreateAreaBonusBg_Start(void);
	void CreateAreaBonusLogo_Start(void);
	void SetEventFieldPosition(void);
	void CreateEventCamera_AreaBonusFinish(void);
	void CreateAreaBonusBg_Finish(void);
	void CreateAreaBonusLogo_Finish(void);
	void DecideAreaBonusIdxFromRandom(void);
	void FinishAreaBonusEvent(void);
	void UpdateAreaBonusEvent(void);
	void UpdateAreaBonusUI(void);
	void DrawAreaBounsEventUI(void);

	//���{�b�g�֐�
	void ReleaseRobotUI(void);		   
	void UpdateRobotUI(void);		   
	void DrawRobotUI(void);			   
	//--------------------
	//	�����o�ϐ�
	//--------------------
	static GAMESTATE m_gameState;							// �Q�[�����
	GAMESTATE m_NowGameState;								// ���݂̃Q�[����
	int m_nCounterGameState;								// ��ԃJ�E���^
	static bool m_bDebugCamera;								// �f�o�b�N�J�����̃I���I�t
	static int m_nScore[MAX_CHARACTER];						// �X�R�A��ۑ�
	static CSceneMeshFiled * m_pMeshFiled;					// ���b�V���t�B�[���h
	static CLoadTextEffect * m_pLoadEffect;					// �G�t�F�N�g�ǂݍ���
	CEffectManager *m_pEffectManager;						// �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^
	CLoadTextObject * m_pLoadObject;						// �I�u�W�F�N�g�̓ǂݍ���
	CLoadTextTerritory * m_pLoadTerritory;					// �e���g���[�̓ǂݍ���
	CGameCamera * m_pGameCamera[MAX_GAMECAMERA];			// �Q�[���J����
	static CTime *m_pTime[2];								// �^�C�}�[
	CCamera * m_pDebugCamera;								// �f�o�b�O�J����
	int m_nNumPlay;											// �v���C���[�̑���
	int m_nEnemyNum;										// �G�l�~�[�̑���
	static int m_nEnemyNumResult;							// ���U���g�p�G�l�~�[�̑���

	int m_nPlayerType[4];									// �v���C���[�̃^�C�v
	int m_nCountry[4];										// ��
	CFieldManager * m_pFieldManager;						// �n�ʃ}�l�[�W���[
	CUI *m_pUI;												//  UI�N���X�ւ̃|�C���^
	CCharacter* m_pCharacter[MAX_CHARACTER];				// �L�����N�^�[�̑���
	CRotationCamera *m_pRotCamera;							// ��]�J����
	int m_nControllerNum[4];								// �R���g���[���[�̔ԍ�
	int m_nControllerType[4];								// �R���g���[���[�̃^�C�v

	// ���{�b�g�ϐ�
	int m_nCntRobot;								
	CRobotUI *m_pRobotUI;							
	CRobot* m_pRoboCharacter;						
	static CEventCamera *m_pEventCamera;				
	bool m_bEveCam;											// �C�x���g�J�����̎��S�t���O

	//	�G�l�~�[�̃^�C�v�I��(�v���C���[�̑I�΂ꂽ�^�C�v�ƂȂ�ׂ����Ԃ�Ȃ��悤�ɂ��邽��)
	int m_nSpeedNo, m_nPowerNo, m_nSweeperNo;
	bool m_bTypeMax;
	D3DXVECTOR3 InitPos[MAX_CHARACTER];						// �e�L�����N�^�[�̏����ʒu

	//���U���g�p�ϐ�
	static int m_nCountMakeScore[MAX_CHARACTER];			// �}�`���������
	static int m_nCountGetTerritry[MAX_CHARACTER];			// �e���g���[�̎擾��
	static int m_nCountRobottedTerritory[MAX_CHARACTER];	// �e���g���[�̒D��ꂽ��

	// �G���A�{�[�i�X�C�x���g�p�ϐ�
	bool m_bAreaBonusEventFlag;								// �G���A�{�[�i�X�C�x���g���n�܂������ǂ���
	int m_nAreaBonusIdx;									// �{�[�i�X�|�C���g��t�^����G���A�̔ԍ�
	int m_nAreaBonusEventCounter;							// �G���A�{�[�i�X�C�x���g���Ǘ�����J�E���^�[
	int m_nAreaBonusUICounter;								// �G���A�{�[�i�XUI���Ǘ�����J�E���^�[
	CIcon2D *m_pAreaBonusBgUp;								// �G���A�{�[�i�X�p�w�i(�㑤)
	CIcon2D *m_pAreaBonusBgDown;							// �G���A�{�[�i�X�p�w�i(����)
	CIcon2D *m_pAreaBonusLogo[2];							// �G���A�{�[�i�X�p���S
	CRingRender * m_pRingRender;						    //�����O�����_���@��
};
#endif