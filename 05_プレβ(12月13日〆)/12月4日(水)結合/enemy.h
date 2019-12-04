//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
//					�G�l�~�[���� [enemy.h]
//			Author : Kobayashi_Sho-hei / ���� ����
//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
#ifndef _ENEMY_H_
#define _ENEMY_H_

//=================================================
//		include / ���̃f�[�^�E�R�[�h�̌Ăяo���E���p
//=================================================
#include "main.h"
#include "game.h"
#include "character.h"
//==============================================
//					�}�N����`
//==============================================
#define AREA_MAX	(4)			//	�G���A�̕�����
#define TERRITORY_MAX	(10)	//	�ő�łȂ��鐔

//==============================================
//					�O���錾
//==============================================
class CSound;
class CModel;
class CMotion;
class CEnemy;
class CTerritory;
class CLoadEffect;
class CCollider;
class CBoxCollider;
class CCylinderCollider;
class CPlayerAttackSphereCollider;

//==============================================
//			�L�����N�^�[�̔h�� �� �v���C���[
//==============================================
class CEnemy : public CCharacter
{
public:
	//	=============�\����==========================
	// ���f���^�C�v�\����
	typedef enum
	{
		PLAYERTYPE_SPEED = 0,
		PLAYERTYPE_POWER,
		PLAYERTYPE_TECHNIQUE,
		PLAYERTYPE_MAX
	}MODEL_TYPE;

	//	�e���g���[���\����
	typedef struct
	{
		D3DXVECTOR3 pos;
		int nAreaNum;		//	�G���A�ԍ�
		float fDistance;	//	����
		float fDisSort;		//	�\�[�g��̋���(�Z����)
		float fRadian;		//	�p�x
		bool bFlag;			//	��x�ʂ�����
	}TERRITORY_INFO;

	//	�v���C���[�̏��
	typedef enum
	{
		STATE_NONE = 0,
		STATE_WALK,
		STATE_ACTION,
		STATE_BLOWAWAY,
		STATE_DAMAGE,
		STATE_MAX
	}STATE;

	//	====================================================

	//	---<<�R���X�g���N�^�E�f�X�g���N�^>>---
	CEnemy(int nPriority = 3, OBJTYPE objType = OBJTYPE_PLAYER);
	~CEnemy();

	//	---<<����>>---
	static CEnemy *Create(int nNum, TYPE type, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], CHARCTERTYPE charatype);

	//	---<<�f�t�H���g�֐�>>---
	HRESULT Init(void);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);

	//	---<<��Պ֐�>>---
	HRESULT Init(int nNum, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nType);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//	---<<�ǉ��֐�>>---
	void InitSort(D3DXVECTOR3 pos);	//	�Q�[���J�n���̋ߏ�̃G���A�E�e���g���[��������
	void DisSort(D3DXVECTOR3 pos);	//	�����ԃ\�[�g�v�Z(�G�l�~�[�̌��ݒn)
	void AIBasicAction(void);		//	AI�̋��ʏ���
	void Program_Move(D3DXVECTOR3 pos,TERRITORY_INFO territory);		//	�ړ��E�ʒu�ύX
	void Program_Line(void);		//	���C���֐��܂Ƃ�
	void Program_State(void);		//	�X�e�[�^�X�Ǘ�
	void Program_Motion(void);//	���[�V�����A�N�V����

	// ---<<�����蔻��>>---
	void Collision(void);

	//!	---<<���C���̔���>>---
	void CreateOrbitLine(void);
	void UninitOrtbitLine(void);

	// �}�[�W�֘A
	void MergeSort(TERRITORY_INFO*, int, int, int); // (TERRITORY_INFO*, 0, ���_��-1, ���_��)

	// ���C�����q���菇
	void LineConnect(int);

protected:
	int m_nEnemyNo;					//	���蓖�Ă�ꂽ�L�����ԍ�
	CModel** m_pModel;				//	���f���̏��
	CMotion* m_pMotion;				//	���[�V�����̏��
	STATE m_state;					//	�L�����N�^�[�̏��
	CLoadEffect * m_pLoadEffect;	//	�G�t�F�N�g�̏��
	bool m_bWalk;					//	�ړ������Ă邩
	bool m_bSprintMotion;
	float m_fSpeed;					//	�v���C���[�̑���
	bool m_bStop;					//	�����񏈗������s�����Ȃ��p(�e�^�C�v��AI�X�L�������Ŏg�p)
	int m_nDamageCount;				// �_���[�W���󂯂Ă���̎��Ԃ��v������
	float m_fBlowAngle;				// ������ԕ���
	float m_fBlowLength;			// ������ԋ���

	//!	---<<�L�������m���Ԃ�������>>---
	void BlowAway(D3DXVECTOR3 AnotherPos);

	//!	---<<���C�����q���E�}�`�̊���>>---
	int m_nLineNum;							//	���݂̃��C����
	bool m_bFinish;							//	�}�`�����������邩�ǂ���
	TERRITORY_INFO m_nTerrStart;			//	�}�`�ƂȂ郉�C�����q���ۂ̎n�_�E�I�_���L��(�n�_�E�I�_�͓����ʒu)

	//!	---<<�X�L���g�p�֘A>>---
	bool m_bTarget;							//	�^�[�Q�b�g�̐؂�ւ�

	//!	---<<�_���[�W����>>---
	int m_nDamageCounter;					//	�_���[�W�J�E���^�[
	bool m_bSuperArmor;						//	�X�[�p�[�A�[�}���
	D3DXVECTOR3 m_rot;						//	�ړ���

	int m_nLevel;							// AI���Ƃ̃��x��

private:
	//D3DXVECTOR3 m_pos;						//	�ʒu
	D3DXVECTOR3 m_posOld;					//	�Â��ʒu���
	D3DXVECTOR3 m_move;						//	�ړ���
	D3DXCOLOR   m_col;
	D3DXMATRIX	m_mtxWorld;					//	���[���h�}�g���b�N�X
	bool m_bUse;							//	�g�p���Ă��邩�ǂ���


	bool CollisionCollider(CCollider *pCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionBoxCollider(CBoxCollider *pBoxCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionCylinderyCollider(CCylinderCollider *pCylinderCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionPlayerAttackSphereCollider(CPlayerAttackSphereCollider *pShere, D3DXVECTOR3 &pos, D3DXVECTOR3 &ColRange);
	bool CollisionRobotAttackSphereCollider(CPlayerAttackSphereCollider *pShere, D3DXVECTOR3 &pos, D3DXVECTOR3 &ColRange); 

	// ---<<���f���^�C�v>>---
	MODEL_TYPE m_modelType;

	//!	---<<AI�֘A>---
	int m_nTargetNum;							//	(��)���_�ԍ��̊m��
	int m_nTargetCnt;							//	���_�̒ʉ߉񐔂��L��
	bool m_bBreak;								//	���[�v�����p

	//!	---<<�e���g���[�֘A>>---
	CTerritory* m_pTerritory;				//	�N���X�|�C���^
	TERRITORY_INFO* m_TerritoryInfo;		//	�\���̃|�C���^
	TERRITORY_INFO* m_AreaInfo[AREA_MAX];	//	�\���̃|�C���^
	int m_nAreaTerrNum[AREA_MAX];			//	�e�G���A�̃e���g���[��
	int m_nAreaNow;							//	���݂���G���A�ԍ�


	//	�}�[�W�֘A�ϐ�
	float *m_tmp[4]; // [Temporary]�ꎞ�I�ɒl���L������ϐ�
	bool m_bCheck; // ������new�����Ȃ�����
};
//==============================================
//		�X�s�[�h�^
//==============================================
class CTypeSpeed : public CEnemy
{
public:
	CTypeSpeed(int nPriority = 3, OBJTYPE objType = OBJTYPE_ENEMY);
	~CTypeSpeed();

	static CTypeSpeed *Create(int nChara, int nLevel, int country, CHARCTERTYPE type, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40]);
	HRESULT Init(int nChara, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int country);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SprintUpdate(void);	//�X�s�[�h�^�̃X�L������
	void Trigger(void);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);

private:
	int m_nCnt;
	bool m_bSkillFlag;
	int m_nTimingCnt;	// ���x�����Ƃɔ�������^�C�~���O��ς���
};

//==============================================
//		�]�،^
//==============================================
class CTypePower : public CEnemy
{
public:
	//	---<<�R���X�g���N�^�E�f�X�g���N�^>>---
	CTypePower(int nPriority = 3, OBJTYPE objType = OBJTYPE_ENEMY);
	~CTypePower();
	//	---<<����>>---
	static CTypePower *Create(int nChara, int country, CHARCTERTYPE type, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40]);
	//	---<<�f�t�H���g�֐�>>---
	HRESULT Init(void);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	//	---<<��Պ֐�>>---
	HRESULT Init(int nChara, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int country);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	//	---<<�ǉ��֐�>>---
	void ActionUpdate(void); // �X�L������


							 //!	---<<�X�L�������͈͂̔���>>---
	bool CollisionSkillTiming(CCylinderCollider *pCylinderCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld,
		D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);

private:

	void CreateColliderSphere(void); // �Ռ��g�̔���

									 //!	---<<�X�L���g�p�֘A>>---
	bool m_bTrigger;	//	�X�L���̎g�p
	bool m_bBreakTime;	//	�g�p�\��
};

//==============================================
//		�n���^
//==============================================
class CTypeSweeper : public CEnemy
{
public:
	//	---<<�R���X�g���N�^�E�f�X�g���N�^>>---
	CTypeSweeper();
	~CTypeSweeper();
	//	---<<����>>---
	static CTypeSweeper *Create(int nChara, int country, CHARCTERTYPE type, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40]);
	//	---<<�f�t�H���g�֐�>>---
	HRESULT Init(void);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	//	---<<��Պ֐�>>---
	HRESULT Init(int nChara, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int country);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	//	---<<�ǉ��֐�>>---
	void ActionUpdate(void); // �X�L������


							 //! ---<<�n���֘A>>---
	void Process();	//	�n���g�p���̏���

private:
	int m_nMinePoint;			//	�}�C���̒u����|�C���g
	int m_nReCounter;			//	�񕜃J�E���^�[
	int m_nReTimer;				//	�񕜃^�C�}�[
	int m_nInstallationCounter;	//	�ݒu�J�E���^�[
	int m_nInstallationTimer;	//	�ݒu�^�C�}�[
	int m_nTiming;				//	�^�C�~���O
	bool m_nUse;
	int m_nActionCnt;
};


#endif