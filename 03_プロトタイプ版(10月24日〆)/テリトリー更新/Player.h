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

//�v���C���[�X�e�[�^�X
#define PLAYER_SPEED	(2.2f)
#define PLAYER_GRAVITY	(0.5f)
#define PLAYER_MAX_PATTERN		(10)
#define MAX_PLAY (2)
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
class CEnemyAttackSphereCollider;
class CModel;
class CTerritory;
//==============================================
//			�L�����N�^�[�̔h�� �� �v���C���[
//==============================================
class CPlayer : public CCharacter
{
public:

	typedef enum
	{//�v���C���[�̐l��
		NUM_PLAYER_01 = 0,
		NUM_PLAYER_02,
		NUM_PLAYER_03,
		NUM_PLAYER_04,
		NUM_PLAYER_MAX
	}NUMPLAYER;

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

	CPlayer();
	~CPlayer();

	HRESULT Init(D3DXVECTOR3 pos, int nNumPlayer, char ModelTxt[40], char MotionTxt[40]);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	//void CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);
	static CPlayer *Create(D3DXVECTOR3 pos,int nNumPlayer, char ModelTxt[40], char MotionTxt[40], TYPE type);
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void PlayerMove(void);				//�v���C���[�̈ړ�����
	void Collision(void);


	//�e���g���[�̃��X�g�p�֐�
	CTerritory * GetTop(void) { return m_pTerritoryTop; }
	CTerritory * GetCur(void) { return m_pTerritoryCur; }
	void SetTop(CTerritory * pTop) { m_pTerritoryTop = pTop; }
	void SetCur(CTerritory * pCur) { m_pTerritoryCur = pCur; }

	//���̃^�C�v
	void SetType(TYPE type) { m_type = type; }
	TYPE GetType(void) { return m_type; }

protected:
	//	�ʒu�E��]���
	CModel** m_pModel;
	int m_nNumPlayer;
	NUMPLAYER m_NumPlayer;					//�v���C���[�̐l��

private:
	D3DXVECTOR3 m_pos;				//�ʒu
	D3DXVECTOR3 m_posOld;				//�ʒu
	D3DXVECTOR3 m_move;					//�ړ���
	D3DXVECTOR3 m_rot;					//�ړ���
	D3DXCOLOR   m_col;
	D3DXMATRIX	m_mtxWorld;				//���[���h�}�g���b�N�X
	D3DXVECTOR3 m_vtxMax;
	D3DXVECTOR3 m_vtxMin;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	static LPD3DXMESH m_pMeshModel;			//���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER m_pBufferMatModel;		//�}�e���A�����ւ̃|�C���^
	static DWORD m_nNumMatModel;				//�}�e���A�����̐�
	bool m_bUse;						//�g�p���Ă��邩�ǂ���

	static CSceneX *m_apSceneX;
	D3DXVECTOR3 m_Angle;					//�p�x

	bool CollisionCollider(CCollider *pCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionBoxCollider(CBoxCollider *pBoxCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);

	//�e���g���[�̃��X�g
	CTerritory * m_pTerritoryTop;		//�擪�e���g���[�ւ̃|�C���^
	CTerritory * m_pTerritoryCur;		//���݂̃e���g���[�ւ̃|�C���^�i�Ō���j

	//���̃^�C�v
	TYPE m_type;

};

////==============================================
////		�L�����N�^�[�̔h�� �� �v���C���[
////==============================================
//class CSpeedPlayer : public CPlayer
//{
//public:
//	CSpeedPlayer();
//	~CSpeedPlayer();
//
//	HRESULT Init(D3DXVECTOR3 pos, NUMPLAYER nNumPlayer, char ModelTxt[40],char ModelTxt2[40], char MotionTxt[40], char MotionTxt2[40]);
//	HRESULT Init(void);
//	void Uninit(void);
//	void Update(void);
//	void Draw(void);
//	static CSpeedPlayer *Create(D3DXVECTOR3 pos, NUMPLAYER nNumPlayer, char ModelTxt[40], char ModelTxt2[40], char MotionTxt[40], char MotionTxt2[40]);
//private:
//};

#endif