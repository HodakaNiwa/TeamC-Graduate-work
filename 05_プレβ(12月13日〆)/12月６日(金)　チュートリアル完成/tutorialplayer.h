//======================================================================
//
//			�`���[�g���A���v���C���[���� [tutorialplayer.h]
//			Author : Kobayashi_Sho-hei / ���� ����
//
//======================================================================
#ifndef _TUTORIALPLAYER_H_
#define _TUTORIALPLAYER_H_

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
#define PLAYER_GRAVITY	(0.5f)
#define PLAYER_MAX_PATTERN		(10)
#define MAX_PLAY (2)
#define MAX_TERRITORY	(10)

//�ړI�n�̃��X�g
#define MAX_LIST		(8)

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
class CSceneOrbit;
class CLine;
class CLoadEffect;
class CMotion;

//==============================================
//			�L�����N�^�[�̔h�� �� �v���C���[
//==============================================
class CTutorialPlayer : public CCharacter
{
public:
	typedef enum
	{//�v���C���[�̏��
		PLAYERSTATE_NONE = 0,
		PLAYERSTATE_WALK,		//�ړ����
		PLAYERSTATE_ACTION,		//
		PLAYERSTATE_BLOWAWAY,
		PLAYERSTATE_DAMAGE,
		PLAYERSTATE_MAX
	}PLAYERSTATE;

	typedef struct
	{
		D3DXVECTOR3 * List;	//�ړI�n�̃��X�g
		int nMaxList;		//���X�g�̍ő吔
	}AIMPOS_LIST;

	CTutorialPlayer(int nPriority = 3, OBJTYPE objType = OBJTYPE_PLAYER);
	~CTutorialPlayer();

	HRESULT Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer, int nType);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTutorialPlayer *Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer,TYPE type,CHARCTERTYPE playertype);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);

	//�v���C���[�̏��擾�֐�
	PLAYERSTATE GetPlayerState(void) { return m_PlayerState; }
	bool GetBool(void) { return m_bSprintMotion; }
	void SetMovePlayer(bool bMovePlayer, int nNumList) { m_bMovePlayer = bMovePlayer; m_nNumList = nNumList; }
	bool GetMovePlayer(void) { return m_bMovePlayer; }
	void SetMoveEnd(bool bMoveEnd) { m_bMoveEnd = bMoveEnd; }
	bool GetMoveEnd(void) { return m_bMoveEnd; }
	void Collision(void);

protected:							//�e�Ǝq�N���X���g�p�ł���
	void SetAimPos(void);
	void BlowAway(D3DXVECTOR3 AnotherPos);

	int m_nDamageCount;		// �_���[�W���󂯂Ă���̎��Ԃ��v������
	float m_fBlowAngle;		// ������ԕ���
	float m_fBlowLength;	// ������ԋ���
	int m_nDownCount;

	//�e���ϐ�
	CModel** m_pModel;						//���f���̏��
	PLAYERSTATE m_PlayerState;				//�v���C���[�̏��
	CMotion *m_pMotion;						//���[�V�����̏��
	CLoadEffect * m_pLoadEffect;			//�G�t�F�N�g�̏��

	//	�ʒu�E��]���
	D3DXVECTOR3 m_posOld;					//�ʒu
	D3DXVECTOR3 m_move;						//�ړ���
	D3DXVECTOR3 m_rot;						//��]��
	D3DXVECTOR3 m_Angle;					//�p�x

	//�v���C���[�ϐ�
	int m_nNumPlayer;						//�v���C���[�̐�
	bool m_bWalk;							//�ړ������Ă邩
	bool m_bSprint;							//�X�v�����g���g�p���Ă邩
	bool m_bSprintMotion;
	float m_fSpeed;							//�v���C���[�̑���
	float m_fInertia;						//����
	bool m_bMoveEnd;

	int m_nDamageCounter;					//�_���[�W�J�E���^�[
	int m_nNumPos;							//���Ԗڈʒu���Q�Ƃ��Ă���̂�

private:
	void DebugMove(void);
	void UpdateMoveTutorial(void);
	void SetMotion(PLAYERSTATE PlayerState);
	void CreateOrbitLine(void);
	void UninitOrtbitLine(void);

	bool m_bPlayerMotionState;				//�v���C���[�����[�V�������Ă邩
	bool m_bMovePlayer;
	int m_nNumList;							//���X�g�̉��Ԗڂ�
	AIMPOS_LIST m_AimPosList[MAX_LIST];

	bool CollisionCollider(CCollider *pCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionBoxCollider(CBoxCollider *pBoxCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionCylinderyCollider(CCylinderCollider *pCylinderCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionPlayerAttackSphereCollider(CPlayerAttackSphereCollider *pShere, D3DXVECTOR3 &pos, D3DXVECTOR3 &ColRange);
};

//==============================================
//		�X�s�[�h�^�v���C���[
//==============================================
class CTutorialSpeedPlayer : public CTutorialPlayer
{
public:
	CTutorialSpeedPlayer();
	~CTutorialSpeedPlayer();

	HRESULT Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SprintUpdate(void);	//�X�s�[�h�^�̃X�L������
	static CTutorialSpeedPlayer *Create(D3DXVECTOR3 pos, int nNumPlayer,int nType);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void CreateOrbitLine(void);
	void UninitOrtbitLine(void);
	void SkillOn(void);

private:
	void DebugInput(void);
	int m_nSprintCounter;						//�X�v�����g�p�J�E���^�[
	int m_nSprintTimer;							//�X�v�����g�p�^�C�}�[
	bool m_bUseSkill;							//�X�L�����g�p�������ǂ���
	CLoadEffect * m_pLoadEffectSpeed;			//�G�t�F�N�g�̏��

};

//==============================================
//		�p���[�^�v���C���[
//==============================================
class CTutorialPowerPlayer : public CTutorialPlayer
{
public:
	CTutorialPowerPlayer();
	~CTutorialPowerPlayer();

	HRESULT Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ActionUpdate(void);
	static CTutorialPowerPlayer *Create(D3DXVECTOR3 pos, int nNumPlayer, int nType);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void SkillOn(void);

protected:
	

private:
	void CreateStartUpCollider(void);
	void CreateColliderSphere(void);
	void DebugInput(void);

	int m_nColliderCnt;
	int m_nColliderTimer;
	float m_fScale;
	bool m_bAction;
	bool m_bInit;
	int m_nInitCnt;
	bool m_bUseSkill;
};

//==============================================
//		�n���^�^�v���C���[
//==============================================
class CTutorialTechniquePlayer : public CTutorialPlayer
{
public:
	CTutorialTechniquePlayer();
	~CTutorialTechniquePlayer();

	HRESULT Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ActionUpdate(void);
	static CTutorialTechniquePlayer *Create(D3DXVECTOR3 pos, int nNumPlayer, int nType);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void SetMine(void);

private:
	void DebugInput(void);		//�f�o�b�O�p���͏���
	int m_nInstallationCounter;	//�ݒu�J�E���^�[
	int m_nInstallationTimer;	//�ݒu�^�C�}�[
	bool m_bMineUse;			//�n�����g�p������
	bool m_bSetMine;			//�n����u�������ǂ���
};
#endif