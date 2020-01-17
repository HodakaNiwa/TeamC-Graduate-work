//====================================================================================
//
// ���{�b�g�̒e����	[bullet.h]
// Author;�r�J�R�N
//
//====================================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"
#include "sceneX.h"

#define BULLET_MAX_EFFECT (2)

//====================================================================================
// �O���錾
//====================================================================================
class CLoadEffect;
class CCollider;
class CBoxCollider;

//-------------------------------------------------------------
// �N���X�錾
//-------------------------------------------------------------
class CBullet : public CSceneX
{
public:
	typedef enum
	{
		TYPE_NONE = 0,	// �ʏ���
		TYPE_MOVE,		// �ړ����
		TYPE_EXPLOSION,	// �������
		TYPE_MAX
	}TYPE;

	CBullet(int nPriority = 3, OBJTYPE objtype = OBJTYPE_ROBOTBULLET);
	~CBullet();
	static CBullet * Create	(const D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale);
	HRESULT Init			(void);
	void	Uninit			(void);
	void	Update			(void);
	void	Draw			(void);

	void	Collision		(void);

	static void LoadModel	(void);
	static void UnloadModel	(void);

	//--------------------
	//	Set & Get
	//--------------------
	void	SetType(const int nType)	{ m_nType = nType; }
	void	SetLife(const int nLife)	{ m_nLife = nLife; }

	int		GetType(void)				{ return m_nType; }
	int		GetLife(void)				{ return m_nLife; }

private:
	//--------------------
	// �֐��錾
	//--------------------
	void Move					(void);		// �ړ�����
	void Explosion				(void);		// ��������
	void CreateCollider			(void);		// �����蔻��̐���
	void CreateExplosionEffect	(void);		// �����G�t�F�N�g�̐���
	void CreateExplosionCollider(void);		// ���������蔻��̐���
	bool CollisionCollider		(CCollider *pCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);
	bool CollisionBoxCollider	(CBoxCollider *pBoxCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange);

	//--------------------
	// �ÓI�����o�ϐ�
	//--------------------
	static LPDIRECT3DTEXTURE9	m_ppTexture;		// �e�N�X�`���ւ̃|�C���^
	static LPD3DXMESH 			m_pMesh;			// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER 		m_pBuffMat;			// �}�e���A�����ւ̃|�C���^
	static DWORD 				m_pNumMat;			// �}�e���A�����̐�
	static D3DXVECTOR3 			m_pVtxMin;			// ���f���ŏ��l
	static D3DXVECTOR3 			m_pVtxMax;			// ���f���ő�l
	static int					m_nMaxTex;			// �e�N�X�`���̍ő吔
	static int					m_nMaxModel;		// ���f���̍ő吔

	//--------------------
	// �����o�ϐ�
	//--------------------
	D3DXVECTOR3					m_pos;				// �ʒu
	D3DXVECTOR3					m_posOld;			// �ʒu(�ߋ�)
	D3DXVECTOR3					m_rot;				// ����
	D3DXVECTOR3					m_move;				// �ړ���
	D3DXVECTOR3					m_Angle;			// �p�x
	int							m_nType;			// ���
	int							m_nLife;			// ����
	bool						m_bDeth;			// ���S�t���O
	int							m_nExplosionTimer;	// �����܂ł̎��Ԃ��v������
	bool						m_bExplosion;		// �����t���O
	CLoadEffect				*	m_pLoadEffect[BULLET_MAX_EFFECT];	// �G�t�F�N�g�̏��

};


#endif