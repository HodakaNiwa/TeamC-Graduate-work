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
#define BASE_MAX	(5)
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
//==============================================
//			�L�����N�^�[�̔h�� �� �v���C���[
//==============================================
class CEnemy : public CCharacter
{
public:
	typedef struct
	{
		float fDistance;	//	���_�Ƃ̋���
		float fChange;
		float fRadian;		//	�V�̊p�x
	}INFO;

	//	---<<�R���X�g���N�^�E�f�X�g���N�^>>---
	CEnemy();
	~CEnemy();

	//	---<<����>>---
	static CEnemy *Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40]);

	//	---<<��Պ֐�>>---
	HRESULT Init(void);	//	�f�t�H���g�֐�
	HRESULT Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40]);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//	---<<�ǉ��֐�>>---
	void Move(void);				//	�ړ�����
	void Collision(void);
	//void CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);

	//	---<<Set�֐�>>---
	void  Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);	//	�f�t�H���g�֐�

	//	---<<���т��ǉ����܂���>>��	��	��	��	��	��	��	��	��	��
	float dist(D3DXVECTOR3 obj, D3DXVECTOR3 enemy);
	//	��	��	��	��	��	��	��	��	��	��	��	��	��	��	��	��

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

	//	��AI�ɕK�v�ȏ���
	D3DXVECTOR3 m_beforePos;	//	�����t���[���O�̈ʒu���
	//D3DXVECTOR3 nextPos;		//	���̈ʒu�̗\���l

	D3DXVECTOR3 m_objPos[BASE_MAX];	//	(��)���_�̈ʒu���
	int m_nNum;						//	(��)���_�ԍ��̊m��
	INFO m_Info[BASE_MAX];			//	(��)���_���
	int m_nNext;
};
#endif