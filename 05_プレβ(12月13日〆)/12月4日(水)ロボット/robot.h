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
#define ROBOT_CHARNUM			(8)		// ���ǉ�(�悵�낤)
#define ROBOT_SEARCH_DISTANCE	(300.0f)	// �{���̋���
#define ROBOT_VIGILANCESPEED	(4.0f)		// �ړ����x
#define ROBOT_COLLISION_RADIUS	(50.0f)		// �����蔻��̕�
#define ROBOT_COLLISION_HEIGHT	(200.0f)	// �����蔻��̍���
#define ROBOT_ATTACK_RADIUS		(100.0f)	// �U������͈̔�
#define ROBOT_ATTACK_INTERVAL	(50)		// �U���̊Ԋu
#define ROBOT_GRAVITY			(-9.8f)		// �d��(-9.8m/s^2)
//#define ROBOT_ENDTIME			(3000)		// �o������
#define ROBOT_ENDTIME			(300)		// �o������

#define ROBOT_UNINITPOS			(1000)		// �����鎞��Y�����W
#define ROBOT_ENDEMITTER		(60)		// ���n�G�t�F�N�g�̏����鎞��

// �R���W�����֌W
#define ROBOT_COLRADIUS (20.0f)
#define ROBOT_COLHEIGHT (100.0f)

//-------------------------------------------------------------
// �O���錾
//-------------------------------------------------------------
class CRobot;
class CLoadEffect;
class CModel;
class CMotion;
class CLoadEffect;
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
	}ROBOTSTATE;

	CRobot(int nPriority = 3, OBJTYPE objType = OBJTYPE_ROBOT);	// �R���X�g���N�^
	~CRobot();													// �f�X�g���N�^

	static CRobot	*Create	(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, TYPE type, CHARCTERTYPE charctertype);
	HRESULT			Init	(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer);
	HRESULT			Init	(void);
	void			Uninit	(void);
	void			Update	(void);
	void			Draw	(void);
	void			Set		(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void			Search	(void);
	void			Wake	(void);
	void			Attack	(void);
	void			Gravity	(void);		// ��������
	void			Jump	(void);

	void CreateRobotUI(int nNum);
	void SetRobotUI(CRobotUI *pRobotUI) { m_pRobotUI = pRobotUI; }
	CRobotUI *GetRobotUI(void) { return m_pRobotUI; }

	bool GetEndGravity(void) { return m_bEndGravity; }

	// �x�N�g���v�Z
	double VectorAngle(D3DXVECTOR3 A, D3DXVECTOR3 B);										// 2�̃x�N�g��AB�̂Ȃ��p�x�Ƃ����߂�
	double GetVecLength(D3DXVECTOR3 v) { return pow((v.x * v.x) + (v.z * v.z), 0.5); }		// �x�N�g���̒����̎Z�o

protected:		// �e�Ǝq�N���X���g�p�ł���
	//	---<<�R���W�����֘A�֐�>>---
	void CreateCylinderCollider(void);

	// �e���ϐ�
	CModel		**m_pModel;			// ���f���̏��
	CMotion		*m_pMotion;			// ���[�V�����̏��
	CLoadEffect *m_pLoadEffect;		// �G�t�F�N�g�̏��

	// �v���C���[�ϐ�
	int m_nNumPlayer;				// �v���C���[�̐�

private:
	void CreateColliderSphere(void);
	void ReleaseRobotUI(void);
	void UpdateRobotUI(void);
	void DrawRobotUI(void);

	D3DXVECTOR3 m_pos;				// �ʒu
	D3DXVECTOR3 m_rot;				// ����
	D3DXVECTOR3 m_move;				// �ړ���
	D3DXVECTOR3 m_col;				// �F
	D3DXVECTOR3	m_PosDest;			// �ړI�̍��W
	D3DXVECTOR3	m_posAngle;			// ���W�̍���
	int			m_nNumTaget;		// �W�I�̔ԍ�
	int			m_nCntAttack;
	float		m_fDropTime;		// ��������
	float		m_fJumpTime;		// �㏸����
	int			m_nCntEnd;			// �ޏ�J�E���^�[

	D3DXMATRIX	m_mtxWorld;			// ���[���h�}�g���b�N�X
	bool		m_bUse;				// �g�p���Ă��邩�ǂ���

	bool		m_bPlayerMotionState;	// ���{�b�g�����[�V�������Ă邩
	bool		m_bDeth;			// ���S�t���O
	bool		m_bEndGravity;		// �����I���t���O

	ROBOTSTATE m_RobotState;	// ���{�b�g�̏��

	CRobotUI	*m_pRobotUI;		// ���{�b�g�o������UI
	CEmitter	*m_pEmitter;		// �G�~�b�^�̃|�C���^
	int			m_nCntEmitter;		// �G�~�b�^�̃J�E���^�[

	//!	---<<���C���̔���ɕK�v>>---
	void		CreateOrbitLine(void);	// �O�Ղ̐��̐���
	void		UninitOrtbitLine(void);	// ���C���̔j��
};
#endif