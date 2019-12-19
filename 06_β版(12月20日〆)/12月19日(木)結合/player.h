//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
//					�v���C���[���� [player.h]
//			Author : Kobayashi_Sho-hei / ���� ���� : �R���֎j : �O�H�ۋM
//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
#ifndef _PLAYER_H_
#define _PLAYER_H_

//=================================================
//		include / ���̃f�[�^�E�R�[�h�̌Ăяo���E���p
//=================================================
#include "main.h"
#include "sceneX.h"
#include "game.h"
#include "scene.h"
#include "character.h"
//==============================================
//					�}�N����`
//==============================================
#define MODEL_UV	("DATA/TEXTURE/MODEL/player.png")
#define FIRST_LINETIME	(60 * 20)	//���C���̎��Ԃ̏�����
#define MIN_TERRITORY	(3)			//�e���g���[�̍Œ����
#define TERRITORY_TOP	(0)			//�e���g���[�̐擪���
#define NOT_GETTIME		(60)		//�e���g���[�̎擾�ł��Ȃ�����
#define LINE_TIME		(90)		//���C���������Ȃ�����

//�v���C���[�X�e�[�^�X
#define PLAYER_SPEED	(2.2f)
#define PLAYER_GRAVITY	(0.5f)
#define PLAYER_MAX_PATTERN		(10)
#define MAX_PLAY (2)
#define MAX_TERRITORY	(20)

//==============================================
//					�O���錾
//==============================================
class CSound;
class CModel;
class CScene3D;
class CObject;
class CEnemy;
class CSceneX;
class CCollider;
class CMeshCollider;
class CBoxCollider;
class CSphereCollider;
class CCylinderCollider;
class CPlayerAttackSphereCollider;
class CModel;
class CTerritory;
class CLine;
class CLoadEffect;
class CMotion;

//==============================================
//			�L�����N�^�[�̔h�� �� �v���C���[
//==============================================
class CPlayer : public CCharacter
{
public:
	typedef enum
	{//�v���C���[�̏��
		PLAYERSTATE_NONE = 0,
		PLAYERSTATE_WALK,			//�ړ����
		PLAYERSTATE_ACTION,			//�A�N�V�������
		PLAYERSTATE_BLOWAWAY,		//�Ԃ��������
		PLAYERSTATE_DAMAGE,			//�_���[�W���
		PLAYERSTATE_ROBOTDAMAGE,	// ���{�b�g�ɉ���ꂽ���
		PLAYERSTATE_MAX
	}PLAYERSTATE;

	CPlayer(int nPriority = 3, OBJTYPE objType = OBJTYPE_PLAYER);
	~CPlayer();

	HRESULT Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer, int nType);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer *Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer,TYPE type,CHARCTERTYPE playertype);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void PlayerMovePad(D3DXVECTOR3 &CameraRot, D3DXVECTOR3 &pos);				//�R���g���[���[(�X�e�B�b�N)�ł̈ړ�����
	void PlayerMoveMouse(D3DXVECTOR3 &CameraRot, D3DXVECTOR3 &pos);				//�}�E�X�ł̈ړ�����
	void PlayerMoveKeyboard(D3DXVECTOR3 &CameraRot, D3DXVECTOR3 &pos);			//�L�[�{�[�h�ł̈ړ�����

	void PlayerMove(void);														//�v���C���[�̈ړ�����
	void Collision(void);

	//�v���C���[�̏��擾�֐�
	PLAYERSTATE GetPlayerState(void) { return m_PlayerState; }
	bool GetBool(void) { return m_bSprintMotion; }
protected:							//�e�Ǝq�N���X���g�p�ł���

	void BlowAway(D3DXVECTOR3 AnotherPos, int Num);


	int m_nDamageCount;															// �_���[�W���󂯂Ă���̎��Ԃ��v������
	float m_fBlowAngle;															// ������ԕ���
	float m_fBlowLength;														// ������ԋ���
	int m_nDownCount;

	//�e���ϐ�
	CModel** m_pModel;															//���f���̏��
	PLAYERSTATE m_PlayerState;													//�v���C���[�̏��
	CMotion *m_pMotion;															//���[�V�����̏��
	CLoadEffect * m_pLoadEffect;												//�G�t�F�N�g�̏��

	//	�ʒu�E��]���
	D3DXVECTOR3 m_posOld;														//�ʒu
	D3DXVECTOR3 m_move;															//�ړ���
	D3DXVECTOR3 m_rot;															//��]��
	D3DXVECTOR3 m_Angle;														//�p�x

	//�v���C���[�ϐ�
	int m_nNumPlayer;															//�v���C���[�̐�
	bool m_bWalk;																//�ړ������Ă邩
	bool m_bSprint;																//�X�v�����g���g�p���Ă邩
	bool m_bSprintMotion;														//�X�s�[�h�^�C�v�̃A�N�V����
	float m_fSpeed;																//�v���C���[�̑���
	float m_fInertia;															//����

	int m_nControllerIndx;														//�g�p���Ă�R���g���[���[�̔ԍ�
	int m_nControllerType;														//�g�p���Ă�R���g���[���[�̃^�C�v
	int m_nDamageCounter;														//�_���[�W�J�E���^�[
	bool m_bSuperArmor;															//�X�[�p�[�A�[�}���
	bool m_bStateFlag;
	//��Ԃ̃t���O
private:
	bool CollisionCollider(CCollider *pCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionBoxCollider(CBoxCollider *pBoxCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionCylinderyCollider(CCylinderCollider *pCylinderCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionPlayerAttackSphereCollider(CPlayerAttackSphereCollider *pShere, D3DXVECTOR3 &pos, D3DXVECTOR3 &ColRange);
	bool CollisionRobotAttackSphereCollider(CPlayerAttackSphereCollider *pShere, D3DXVECTOR3 &pos, D3DXVECTOR3 &ColRange);

	void CreateOrbitLine(void);
	void UninitOrtbitLine(void);
	bool m_bMouse;
	float m_fMouse;
	int m_nCountTime;															//�J�E���^�[
	bool m_bWalkFrag;															//�����Ă邩�ǂ����̃t���O
};

//==============================================
//		�X�s�[�h�^�v���C���[
//==============================================
class CSpeedPlayer : public CPlayer
{
public:
	CSpeedPlayer();
	~CSpeedPlayer();

	HRESULT Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void PlayerActionPad();						//�R���g���[���ł̃A�N�V��������
	void PlayerActionMouse();					//�}�E�X�ł̃A�N�V�����ړ�����
	void PlayerActionKeyboard();					//�}�E�X�ł̃A�N�V�����ړ�����
	void SprintUpdate(void);					//�X�s�[�h�^�̃X�L������
	static CSpeedPlayer *Create(D3DXVECTOR3 pos,char ModelTxt[40], char MotionTxt[40],int nNumPlayer,int nType, 
		CHARCTERTYPE charctertype,int nControllerIndx, int nControllerType);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void CreateOrbitLine(void);
	void UninitOrtbitLine(void);
	bool GetSprintState(void) { return m_bSprint; }

private:
	int m_nSprintCounter;						//�X�v�����g�p�J�E���^�[
	int m_nSprintTimer;							//�X�v�����g�p�^�C�}�[
	CLoadEffect * m_pLoadEffectSpeed;			//�G�t�F�N�g�̏��
};
//==============================================
//		�p���[�^�v���C���[
//==============================================
class CPowerPlayer : public CPlayer
{
public:
	CPowerPlayer();
	~CPowerPlayer();

	HRESULT Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ActionUpdate(void);			//�A�N�V��������
	void PlayerActionPad();				//�R���g���[���ł̃A�N�V��������
	void PlayerActionMouse();			//�}�E�X�ł̃A�N�V�����ړ�����
	void PlayerActionKeyboard();		//�L�[�{�[�h�ł̃A�N�V�����ړ�����
	void RotAction(int nType);			//�R���g���[���[�̌����̏���					
	void InitNum(void);					//�ϐ���������
	static CPowerPlayer *Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType, 
		CHARCTERTYPE charctertype,int nControllerIndx, int nControllerType);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
private:
	void CreateColliderSphere(void);	//�Ռ��g�̓����蔻��
	int m_nColliderCnt;					//�Ռ��g�J�E���^�[
	int m_nColliderTimer;				//�Ռ��g�^�C�}�[
	bool m_bAction;						//�A�N�V���������ǂ���
	bool m_bInit;						//���������邩�ǂ���
	int m_nInitCnt;						//�������܂ł̃J�E���^�[
	int m_nRotCounter;					//������ς����J�E���^�[
	bool m_bCollision;					//�����蔻��
};

//==============================================
//		�n���^�^�v���C���[
//==============================================
class CTechniquePlayer : public CPlayer
{
public:
	
	CTechniquePlayer();
	~CTechniquePlayer();

	HRESULT Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ActionUpdate(void);			//�A�N�V��������
	void PlayerActionPad();				//�R���g���[���ł̃A�N�V��������
	void PlayerActionMouse();			//�}�E�X�ł̃A�N�V�����ړ�����
	void PlayerActionKeyboard();		//�L�[�{�[�h�ł̃A�N�V�����ړ�����
	static CTechniquePlayer *Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType,
		CHARCTERTYPE charctertype, int nControllerIndx, int nControllerType);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);

private:
	int m_nMinePoint;					//�}�C���̒u����|�C���g
	int m_nReCounter;					//�񕜃J�E���^�[
	int m_nReTimer;						//�񕜃^�C�}�[
	int m_nInstallationCounter;			//�ݒu�J�E���^�[
	int m_nInstallationTimer;			//�ݒu�^�C�}�[
	bool m_bMineUse;					//�n�����g�p������
	int m_nInitCnt;						//�������܂ł̃J�E���^�[
	bool m_bInit;						//���������邩�ǂ���
};
#endif