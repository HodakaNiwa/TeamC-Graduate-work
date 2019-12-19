//====================================================================================
//
// ��9��͏���	[robot.h]
// Author;�r�J�R�N
//
//====================================================================================
#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "main.h"
#include "sceneX.h"
#include "game.h"
#include "scene.h"
#include "character.h"

//-------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------
#define ROBOT_CHARNUM					(8)
#define ROBOT_VIGILANCESPEED			(10.0f)		// �ړ����x
#define ROBOT_ATTACK_RADIUS				(70.0f)		// �U������͈̔�
#define ROBOT_ATTACK_INTERVAL			(320)		// �U��(�ߐ�)�̊Ԋu50
#define ROBOT_SHOOT_MODEINTERVAL		(8)			// �U��(������)�̊Ԋu5
#define ROBOT_SHOOT_ROTATION			(0.07f)		// �U��(������)�̉�]��
#define ROBOT_SHOOT_BULLETINTERVAL		(14)		// �~�T�C�������Ԋu8
#define ROBOT_SHOOT_MAXBULLET			(12)		// �~�T�C��������
#define ROBOT_GRAVITY					(-9.8f)		// �d��(-9.8m/s^2)
#define ROBOT_ENDTIME					(2700)		// �o������
#define ROBOT_SWITCHTIME				(200)		// �ǐՎ���

#define ROBOT_UNINITPOS					(1000)		// �����鎞��Y�����W
#define ROBOT_ATTACKEFFECT				(50)		// �U���G�t�F�N�g�̃^�C�~���O
#define ROBOT_ENDEMITTER				(60)		// ���n�G�t�F�N�g�̏����鎞��

// �R���W�����֌W
#define ROBOT_COLLISION_RADIUS			(50.0f)		// �����蔻��̕�
#define ROBOT_COLLISION_HEIGHT			(180.0f)	// �����蔻��̍���
#define ROBOT_COLLISION_ATTACKRADIUS	(180.0f)	// �U�����̓����蔻��̕�
#define ROBOT_COLLISION_ATTACKLIFE		(25)		// �U�����̓����蔻��̎���

//-------------------------------------------------------------
// �O���錾
//-------------------------------------------------------------
class CRobot;
class CLoadEffect;
class CModel;
class CMotion;
class CGame;
class CCollider;
class CMeshCollider;
class CBoxCollider;
class CSphereCollider;
class CCylinderCollider;
class CPlayerAttackSphereCollider;
class CRobotUI;
class CEmitter;

//-------------------------------------------------------------
// �N���X�錾
//-------------------------------------------------------------
class CRobot : CCharacter
{
public:
	typedef enum
	{// ���
		ROBOTSTATE_NONE = 0,	// �j���[�g����
		ROBOTSTATE_WALK,		// �ړ�
		ROBOTSTATE_APPEARANCE,	// �o��
		ROBOTSTATE_JUMP,		// �W�����v
		ROBOTSTATE_ATTACK,		// �ߐڍU��
		ROBOTSTATE_SHOOT,		// �ˌ��U��
		ROBOTSTATE_DAWN,		// ������
	}ROBOTSTATE;

	CRobot(int nPriority = 3, OBJTYPE objType = OBJTYPE_ROBOT);	// �R���X�g���N�^
	~CRobot();													// �f�X�g���N�^

	static CRobot	*Create			(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, TYPE type, CHARCTERTYPE charctertype);
	HRESULT			Init			(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer);
	HRESULT			Init			(void);
	void			Uninit			(void);
	void			Update			(void);
	void			Draw			(void);
	void			Set				(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);

	void			Search			(void);		// �{������
	void			Wake			(void);		// �ړ�����
	void			Attack			(void);		// �U������
	void			Shoot			(void);		// �ˌ�����
	void			Gravity			(void);		// ��������
	void			Jump			(void);		// �W�����v����
	void			GetUp			(void);		// �N���オ�菈��

	void			CreateRobotUI	(void);		// �o���e���b�v�̐�������
	void			ReleaseRobotUI	(void);

	void			SetRobotUI		(CRobotUI *pRobotUI)	{ m_pRobotUI = pRobotUI; }
	CRobotUI		*GetRobotUI		(void)					{ return m_pRobotUI; }
	bool			GetEndGravity	(void)					{ return m_bEndGravity; }

	// �x�N�g���v�Z
	double VectorAngle	(D3DXVECTOR3 A, D3DXVECTOR3 B);										// 2�̃x�N�g��AB�̂Ȃ��p�x�Ƃ����߂�
	double GetVecLength	(D3DXVECTOR3 v) { return pow((v.x * v.x) + (v.z * v.z), 0.5); }		// �x�N�g���̒����̎Z�o

protected:		// �e�Ǝq�N���X���g�p�ł���
	//	---<<�R���W�����֘A�֐�>>---
	void CreateCylinderCollider(void);

	// �e���ϐ�
	CModel		**m_pModel;			// ���f���̏��
	CMotion		*m_pMotion;			// ���[�V�����̏��

	// �v���C���[�ϐ�
	int m_nNumPlayer;				// �v���C���[�̐�

private:
	void CreateColliderSphere	(void);
	void UpdateRobotUI			(void);

	D3DXMATRIX	m_mtxWorld;				// ���[���h�}�g���b�N�X
	ROBOTSTATE	m_RobotState;			// ���{�b�g�̏��
	D3DXVECTOR3 m_pos;					// �ʒu
	D3DXVECTOR3 m_rot;					// ����
	D3DXVECTOR3 m_move;					// �ړ���
	D3DXVECTOR3 m_col;					// �F

	int			m_nNumTaget;			// �W�I�̔ԍ�
	int			m_nNumTagetOld;			// �O��W�I�̔ԍ�
	int			m_nCntEnd;				// �ޏ�J�E���^�[
	int			m_nCntWalk;				// �^�[�Q�b�g�؂�ւ�����
	int			m_nNumAttack;			// ���{�b�g���U��(�ߐ�)������
	int			m_nNumShoot;			// ���{�b�g���U��(������)������
	int			m_nCntShoot;			// �~�T�C�������܂ł̃J�E���^

	float		m_fDropTime;			// ��������
	float		m_fJumpTime;			// �㏸����

	bool		m_bDeth;				// ���S�t���O
	bool		m_bEndGravity;			// �����I���t���O
	bool		m_bAttack;				// �U��(�ߐ�)���Ă��邩�ǂ���
	bool		m_bShoot;				// �U��(������)���Ă��邩�ǂ���

	// �|�C���^
	CRobotUI	*m_pRobotUI;			// �o���e���b�v�̃|�C���^

	// �G�t�F�N�g�֌W
	CEmitter	*m_pEmitter;			// �G�~�b�^�̃|�C���^
	int			m_nCntEmitter;			// �G�~�b�^�̃J�E���^
	int			m_nCntAttackEffect;		// �U�����̃G�t�F�N�g�J�E���^

	//!	---<<���C���̔���ɕK�v>>---
	void		CreateOrbitLine	(void);	// �O�Ղ̐��̐���
	void		UninitOrtbitLine(void);	// ���C���̔j��
};
#endif