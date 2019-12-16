//=============================================================================
//
// ���U���g�p�v���C���[�̏��� [charResult.h]
// Author : ���� �����@[ SATO ASUMI ]
//
//=============================================================================
#ifndef _CHRRESULT_H_
#define _CHRRESULT_H_

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
#define PLAYER_MAX_PATTERN		(10)
#define MAX_PLAY (2)

//==============================================
//					�O���錾
//==============================================
class CSound;
class CModel;
class CScene3D;
class CEnemy;
class CSceneX;
class CModel;
class CMotion;

//==============================================
//			�L�����N�^�[�̔h�� �� �v���C���[
//==============================================
class CCharResult : public CCharacter
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
		MOTION_TYPE_NONE = 0,
		MOTION_TYPE_WALK,		//�ړ����
		MOTION_TYPE_ACTION,		//
		MOTION_TYPE_BLOWAWAY,
		MOTION_TYPE_DOWN,
		MOTION_TYPE_WAKEUP,
		MOTION_TYPE_WIN,
		MOTION_TYPE_LOSE,
		MOTION_TYPE_SELECT,
		MOTION_TYPE_MAX
	}MOTION_TYPE;

	CCharResult(int nPriority = 3, OBJTYPE objType = OBJTYPE_NONE);
	~CCharResult();

	HRESULT Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer, int nCuntry);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CCharResult *Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], CHARCTERTYPE charactertype,int nCharaNum, int nCuntry);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void SetMotionType(MOTION_TYPE MotionType) { m_MotionType = MotionType; }
	PLAYERTYPE GetPlayerType(void) { return m_PlayerType; }

protected:								//�e�Ǝq�N���X���g�p�ł���
	//	�ʒu�E��]���
	CModel** m_pModel;
	PLAYERTYPE m_PlayerType;

	D3DXVECTOR3 m_move;					//�ړ���
	D3DXVECTOR3 m_rot;					//��]��

	MOTION_TYPE m_MotionType;			//���[�V�����̎��
	CMotion *m_pMotion;					//���[�V�����̏��
	void CreateOrbitLine(void);
	void UninitOrtbitLine(void);

private:
	void InitMove(void);
	void UpdateMove(void);
	bool m_bStopMotion;

};
#endif