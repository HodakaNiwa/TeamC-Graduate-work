//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
//					�v���C���[���� [player.h]
//			Author : Kobayashi_Sho-hei / ���� ����
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
#define MAX_TERRITORY	(10)

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
class CPlayer : public CCharacter
{
public:
	typedef enum
	{//�v���C���[�̃^�C�v
		PLAYERTYPE_SPEED = 0,
		PLAYERTYPE_POWER,
		PLAYERTYPE_TECHNIQUE,
		PLAYERTYPE_MAX
	}PLAYERTYPE;

	typedef enum
	{//�v���C���[�̏��
		PLAYERSTATE_NONE = 0,
		PLAYERSTATE_WALK,		//�ړ����
		PLAYERSTATE_ACTION,		//�_�b�V����ԁi�X�s�[�h�^�̂݁j
		PLAYERSTATE_BLOWAWAY,
		PLAYERSTATE_MAX
	}PLAYERSTATE;

	CPlayer(int nPriority = 3, OBJTYPE objType = OBJTYPE_PLAYER);
	~CPlayer();

	HRESULT Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer *Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer,TYPE type,PLAYERTYPE playertype);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void PlayerMove(void);				//�v���C���[�̈ړ�����
	void Collision(void);

	//�v���C���[�̏��擾�֐�
	PLAYERTYPE GetPlayerType(void) { return m_PlayerType; }
	PLAYERSTATE GetPlayerState(void) { return m_PlayerState; }


	bool GetBool(void) { return m_bSprintMotion; }
protected:							//�e�Ǝq�N���X���g�p�ł���

	void BlowAway(D3DXVECTOR3 AnotherPos);

	int m_nDamageCount;		// �_���[�W���󂯂Ă���̎��Ԃ��v������
	float m_fBlowAngle;		// ������ԕ���
	float m_fBlowLength;	// ������ԋ���


	//�e���ϐ�
	CModel** m_pModel;						//���f���̏��
	PLAYERTYPE m_PlayerType;				//�v���C���[�̃^�C�v
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

	void CutTerritoryPoint(CTerritory * pTerritory, int nOldPlayer);	// �e���g���[�X�R�A�̍폜
	void AddTerritoryPoint(CTerritory * pTerritory);					// �e���g���[�X�R�A�̒ǉ�
private:
	bool m_bPlayerMotionState;				//�v���C���[�����[�V�������Ă邩

	
	bool CollisionCollider(CCollider *pCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionBoxCollider(CBoxCollider *pBoxCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionCylinderyCollider(CCylinderCollider *pCylinderCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionPlayerAttackSphereCollider(CPlayerAttackSphereCollider *pShere, D3DXVECTOR3 &pos, D3DXVECTOR3 &ColRange);

	void CreateOrbitLine(void);
	void UninitOrtbitLine(void);
};

//==============================================
//		�X�s�[�h�^�v���C���[
//==============================================
class CSpeedPlayer : public CPlayer
{
public:
	CSpeedPlayer();
	~CSpeedPlayer();

	HRESULT Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SprintUpdate(void);	//�X�s�[�h�^�̃X�L������
	static CSpeedPlayer *Create(D3DXVECTOR3 pos,char ModelTxt[40], char MotionTxt[40],int nNumPlayer,int nType, PLAYERTYPE playertype);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void CreateOrbitLine(void);
	void UninitOrtbitLine(void);
private:
	int m_nSprintCounter;	//�X�v�����g�p�J�E���^�[
	int m_nSprintTimer;		//�X�v�����g�p�^�C�}�[
	int m_nRecastCounter;	//���L���X�g�p�J�E���^�[
	int m_nRecastTimer;		//���L���X�g����
	int m_nButtonCounter;	//�{�^���J�E���^�[
	
	bool m_bRecast;			//���L���X�g���Ă邩

};
//==============================================
//		�p���[�^�v���C���[
//==============================================
class CPowerPlayer : public CPlayer
{
public:
	CPowerPlayer();
	~CPowerPlayer();

	HRESULT Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPowerPlayer *Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType, PLAYERTYPE playertype);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);

private:

};

//==============================================
//		�n���^�^�v���C���[
//==============================================
class CTechniquePlayer : public CPlayer
{
public:
	
	CTechniquePlayer();
	~CTechniquePlayer();

	HRESULT Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CTechniquePlayer *Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType, PLAYERTYPE playertype);
private:

};
#endif