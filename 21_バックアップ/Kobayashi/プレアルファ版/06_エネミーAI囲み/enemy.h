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
#include "sceneX.h"
#include "game.h"
#include "scene.h"
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
class CLine;
class CSceneOrbit;

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

	//	====================================================



	//	---<<�R���X�g���N�^�E�f�X�g���N�^>>---
	CEnemy();
	~CEnemy();

	//	---<<����>>---
	static CEnemy *Create(int nNum, TYPE type, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40]);

	//	---<<��Պ֐�>>---
	HRESULT Init(void);	//	�f�t�H���g�֐�
	HRESULT Init(int nNum, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40]);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//	---<<Set�֐�>>---
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);	//	�f�t�H���g�֐�

	//	---<<�ǉ��֐�>>---
	void Move(void);				//	�ړ�����
	void Collision(void);
	void InitSort(D3DXVECTOR3 pos);	//	�Q�[���J�n���̋ߏ�̃G���A�E�e���g���[��������

	// ---<<���f���^�C�v>>---
	MODEL_TYPE GetModelType(void) { return m_modelType; }

private:
	D3DXVECTOR3 m_pos;						//	�ʒu
	D3DXVECTOR3 m_posOld;					//	�Â��ʒu���
	D3DXVECTOR3 m_move;						//	�ړ���
	D3DXVECTOR3 m_rot;						//	�ړ���
	D3DXCOLOR   m_col;
	D3DXMATRIX	m_mtxWorld;					//	���[���h�}�g���b�N�X
	D3DXVECTOR3 m_vtxMax;
	D3DXVECTOR3 m_vtxMin;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	static LPD3DXMESH m_pMeshModel;			//	���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER m_pBufferMatModel;	//	�}�e���A�����ւ̃|�C���^
	static DWORD m_nNumMatModel;			//	�}�e���A�����̐�
	bool m_bUse;							//	�g�p���Ă��邩�ǂ���

	static CSceneX *m_apSceneX;
	D3DXVECTOR3 m_Angle;					//	�p�x

	bool CollisionCollider(CCollider *pCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionBoxCollider(CBoxCollider *pBoxCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);

	//	�ʒu�E��]���
	CModel** m_pModel;

	// ---<<���f���^�C�v>>---
	MODEL_TYPE m_modelType;


	//!	---<<AI�֘A>---
	D3DXVECTOR3 m_beforePos;					//	�����t���[���O�̈ʒu���
	int m_nTargetNum;							//	(��)���_�ԍ��̊m��
	int m_nTargetCnt;							//	���_�̒ʉ߉񐔂��L��
	bool m_bBreak;								//	���[�v�����p

	//!	---<<�e���g���[�֘A>>---
	CTerritory* m_pTerritory;				//	�N���X�|�C���^
	TERRITORY_INFO* m_TerritoryInfo;		//	�\���̃|�C���^
	TERRITORY_INFO* m_AreaInfo[AREA_MAX];	//	�\���̃|�C���^
	int m_nMax;								//	������
	int m_nAreaTerrNum[AREA_MAX];			//	�e�G���A�̃e���g���[��
	int m_nAreaNow;							//	���݂���G���A�ԍ�

	//!	---<<���C���̔���ɕK�v>>---
	void CreateOrbitLine(void);				//	�O�Ղ̐��̐���
	void UninitOrtbitLine(void);			//	���C���̔j��


};
#endif