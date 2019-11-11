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
class CEnemyAttackSphereCollider;
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
	{//���̃^�C�v
		TYPE_JAPAN = 0,		//���{
		TYPE_RUSSIA,		//���V�A
		TYPE_AMERICA,		//�A�����J
		TYPE_BRAZIL,		//�u���W��
		TYPE_ENGLAND,		//�C�M���X
		TYPE_ITALY,			//�C�^���A
		TYPE_NEWZEALAND,	//�j���[�W�[�����h
		TYPE_SOUTHAFRICA,	//��A�t���J
		TYPE_MAX,
	}TYPE;

	typedef enum
	{//�v���C���[�̏��
		PLAYERSTATE_NONE = 0,
		PLAYERSTATE_WALK,
		PLAYERSTATE_ACTION,
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
	void AddTerritoryList(CTerritory * pTerritory);
	void CutLine(int nID);
	PLAYERTYPE GetPlayerType(void) { return m_PlayerType; }


	//���̃^�C�v
	void SetType(TYPE type) { m_type = type; }
	TYPE GetType(void) { return m_type; }
	static D3DXCOLOR m_CountryColor[CPlayer::TYPE_MAX];	//���̐F

protected:							//�e�Ǝq�N���X���g�p�ł���
	void BlowAway(D3DXVECTOR3 AnotherPos);


	int m_nDamageCount;		// �_���[�W���󂯂Ă���̎��Ԃ��v������
	float m_fBlowAngle;		// ������ԕ���
	float m_fBlowLength;	// ������ԋ���

	//	�ʒu�E��]���
	CModel** m_pModel;
	int m_nNumPlayer;
	PLAYERTYPE m_PlayerType;


	D3DXVECTOR3 m_pos;					//�ʒu
	D3DXVECTOR3 m_posOld;				//�ʒu
	D3DXVECTOR3 m_move;					//�ړ���
	D3DXVECTOR3 m_rot;					//��]��
	D3DXCOLOR   m_col;					//�F
	D3DXVECTOR3 m_Angle;				//�p�x

	int m_nLineTime;						//���C���̎���
	int m_nMaxLineTime;						//���C���̎��ԍő吔
	bool m_bWalk;

	//�e���g���[
	CTerritory * m_apTerritory[MAX_TERRITORY];	//�e���g���[�̃��X�g
	CTerritory * m_pOldStartTerritory;			//�����ʒu�̃e���g���[
	int m_nCountTerritory;						//�J�E���^�[
	int m_nCountTime;							//���ԃJ�E���^�[
	bool m_bBlockStartTerritory;				//�����e���g���[���u���b�N
	bool m_bMakeShape;							//�}�`�������t���O

	//���C��
	CSceneOrbit * m_pOrbitLine;					//���C��
	CLine *		  m_apLine[MAX_TERRITORY];		//���C��
	CLine *		  m_apCopyLine[MAX_TERRITORY];	//�R�s�[���C��
	int			  m_nCntTimeCopyLine;			//���C���̃^�C�}�[

	PLAYERSTATE m_PlayerState;					//�v���C���[�̏��
	CMotion *m_pMotion;							//���[�V�����̏��

	CLoadEffect * m_pLoadEffect;				//�G�t�F�N�g�̏��
private:
	void CountDownLineTime(void);										//�e���g���[�̎擾�X�V����
	void ResetList(void);												//���X�g�̏�����
	bool ChackSamePos(CTerritory * pTerritory, int nNumTerritory);		//�����ʒu���ǂ���
	void ShapeComplete(CTerritory * pTerritory);						//���������Ƃ��̏���
	void ChackInShape(void);											//�}�`�ɓ����Ă��邩�ǂ����̏���
	void GetTerritory(CTerritory * pTerritory);							//�e���g���[�̎擾
	void CutTerritoryPoint(CTerritory * pTerritory, int nOldPlayer);	// �e���g���[�X�R�A�̍폜
	void AddTerritoryPoint(CTerritory * pTerritory);					// �e���g���[�X�R�A�̒ǉ�
	void CreateOrbitLine(void);											//��Ղ̐��̐���
	void UninitOrtbitLine(void);										//���C���̔j��
	void ResetLine(void);												//���C���̐�������
	void UninitLine(void);												//���C���̔j��
	void UninitCopyLine(void);											//�R�s�[���C���̔j��
	void CreateEndLine(void);											//�Ō�̐��̐���
	void UpdateShapeComplete(void);										//������̍X�V����


	bool CollisionCollider(CCollider *pCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionBoxCollider(CBoxCollider *pBoxCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionCylinderCollider(CCylinderCollider *pCylinderCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionPlayerAttackSphereCollider(CPlayerAttackSphereCollider *pShere, D3DXVECTOR3 &pos, D3DXVECTOR3 &ColRange);

	//���̃^�C�v
	TYPE m_type;
	bool m_bUse;							//�g�p���Ă��邩�ǂ���

	CSpeedPlayer *m_pSpeedPlayer;
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
	static CSpeedPlayer *Create(D3DXVECTOR3 pos,char ModelTxt[40], char MotionTxt[40],int nNumPlayer,TYPE type, PLAYERTYPE playertype);
private:

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
	static CPowerPlayer *Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, TYPE type, PLAYERTYPE playertype);
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
	static CTechniquePlayer *Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, TYPE type, PLAYERTYPE playertype);
private:

};
#endif