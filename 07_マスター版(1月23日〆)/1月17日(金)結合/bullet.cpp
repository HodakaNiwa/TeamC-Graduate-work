//====================================================================================
//
// ���{�b�g�̒e����	[bullet.cpp]
// Author;�r�J�R�N
//
//====================================================================================
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "collision.h"
#include "loadEffect.h"
#include "debuglog.h"
#include "collision.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define BULLETMODEL_TEXT			("data/MODEL/ROBOT/99_Robot_bullet.x")

#define BULLET_COLRADIUS			(30.0f)		// �~�T�C�����N�����锻��͈̔�
#define BULLET_LIFE					(40)		// �~�T�C���̎���
#define BULLET_EXPLOSION_TIME		(15)		// �~�T�C���̔�������
#define BULLET_EXPLOSION_COLRADIUS	(100.0f)	// �n���̔����L���͈�
#define BULLET_MOVESPEED			(0.5f)		// �ړ����x

//=============================================================================
// �O���錾
//=============================================================================
LPDIRECT3DTEXTURE9	CBullet::m_ppTexture	= NULL;
LPD3DXMESH			CBullet::m_pMesh		= NULL;
LPD3DXBUFFER		CBullet::m_pBuffMat		= NULL;
DWORD				CBullet::m_pNumMat		= NULL;
D3DXVECTOR3			CBullet::m_pVtxMax		= {};
D3DXVECTOR3			CBullet::m_pVtxMin		= {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBullet::CBullet(int nPriority, OBJTYPE objtype) : CSceneX(nPriority, objtype)
{
	m_pos				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Angle				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nType				= TYPE_MOVE;
	m_nLife				= BULLET_LIFE;
	m_bDeth				= false;
	m_nExplosionTimer	= 0;
	m_bExplosion		= false;

	for (int nCnt = 0; nCnt < BULLET_MAX_EFFECT; nCnt++)
	{
		m_pLoadEffect[nCnt] = NULL;
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBullet::~CBullet()
{
}

//=============================================================================
// ��������
//=============================================================================
CBullet * CBullet::Create(const D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{
	// ���������m��
	CBullet *pBullet = NULL;
	if (pBullet != NULL) { return NULL; }
	pBullet = new CBullet;
	if (pBullet == NULL) { return NULL; }

	// ����������
	if (FAILED(pBullet->Init()))
	{
		pBullet->Uninit();
		delete pBullet;
		return NULL;
	}
	pBullet->SetPos		(pos);
	pBullet->m_pos = pos;
	pBullet->SetRot		(rot);
	pBullet->m_rot = rot;
	pBullet->SetScale	(scale);
	pBullet->m_Scale = scale;

	// ���f���E�e�N�X�`���̊��蓖��
	pBullet->BindModel	(m_pMesh, m_pBuffMat, m_pNumMat, m_ppTexture);

	// �N���p�̓����蔻���t�^����
	pBullet->CreateCollider();

	return pBullet;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBullet::Init(void)
{
	CSceneX::Init();

	//�ϐ��̏�����
	for (int nCnt = 0; nCnt < BULLET_MAX_EFFECT; nCnt++)
	{
		m_pLoadEffect[nCnt] = NULL;
	}

	//�G�t�F�N�g�̏�����
	if (m_pLoadEffect[0] == NULL)
	{
		m_pLoadEffect[0] = CLoadEffect::Create(3, GetPos(), 5);
	}
	if (m_pLoadEffect[1] == NULL)
	{
		m_pLoadEffect[1] = CLoadEffect::Create(1, GetPos(), 6);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBullet::Uninit(void)
{
	for (int nCnt = 0; nCnt < BULLET_MAX_EFFECT; nCnt++)
	{
		if (m_pLoadEffect[nCnt] != NULL)
		{//�G�t�F�N�g�̔j��
			m_pLoadEffect[nCnt]->Uninit();
			delete m_pLoadEffect[nCnt];
			m_pLoadEffect[nCnt] = NULL;
		}
	}

	CSceneX::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBullet::Update(void)
{
	//�O��̈ʒu���i�[
	m_posOld = m_pos;

	switch (m_nType)
	{
	case TYPE_NONE:			// �j���[�g����
		break;
	case TYPE_MOVE:			// �ړ�
		Move();			// �ړ�����
		break;
	case TYPE_EXPLOSION:	// ����
		Explosion();	// ��������
		break;
	}
	CSceneX::Update();

	//CDebugProc::Print("�~�T�C�����W:%f,%f,%f", m_pos.x, m_pos.y, m_pos.z);
	//CDebugProc::Print("�~�T�C������:%f,%f,%f", m_rot.x, m_rot.y, m_rot.z);

	if (m_bDeth == true) { Uninit(); }		// ���S�t���O�`�F�b�N
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBullet::Draw(void)
{
	// ������ԂȂ�`�悵�Ȃ�
	if (m_nType == TYPE_EXPLOSION) { return; }

	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �@���𐳋K������(�傫���ɂ���Ė@�����ς��̂�)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	// ���ʂ̕`�揈��
	CSceneX::Draw();

	// �@���𐳋K�����Ȃ�
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, false);

	for (int nCnt = 0; nCnt < BULLET_MAX_EFFECT; nCnt++)
	{
		if (m_pLoadEffect[nCnt] != NULL)
		{//�G�t�F�N�g�̕`��
			m_pLoadEffect[nCnt]->Draw();
		}
	}
}

//=============================================================================
// �ǂݍ��ݏ���
//=============================================================================
void CBullet::LoadModel(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(BULLETMODEL_TEXT,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_pNumMat,
		&m_pMesh);

	D3DXMATERIAL *pmat;			// �}�e���A���f�[�^�ւ̃|�C���^
	pmat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (DWORD tex = 0; tex < m_pNumMat; tex++)
	{// �J�E���g
		if (pmat[tex].pTextureFilename != NULL &&
			lstrlen(pmat[tex].pTextureFilename) > 0)
		{// �e�N�X�`�����g�p���Ă���
			if (FAILED(D3DXCreateTextureFromFile(pDevice,
				pmat[tex].pTextureFilename,
				&m_ppTexture)))
			{
				MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
			}
		}
	}

	// �ϐ��錾
	int		nNumVtx		= 0;	// ���_��
	DWORD	sizeFVF		= 0;	// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *	pVtxBuff	= 0;	// ���_�o�b�t�@�ւ̃|�C���^

	// ���_�����擾
	nNumVtx = m_pMesh->GetNumVertices();

	// ���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	m_pVtxMax = D3DXVECTOR3(-100000.0f, 0.0f, -100000.0f);	// ���_�̍ő�l
	m_pVtxMin = D3DXVECTOR3(100000.0f, 0.0f, 100000.0f);	// ���_�̍ŏ��l

	// ���_�o�b�t�@�����b�N
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// ���_���W�̑��

		// �ő�l���r
		if (m_pVtxMax.x < vtx.x)
		{
			m_pVtxMax.x = vtx.x;
		}
		if (m_pVtxMax.y < vtx.y)
		{
			m_pVtxMax.y = vtx.y;
		}
		if (m_pVtxMax.z < vtx.z)
		{
			m_pVtxMax.z = vtx.z;
		}

		// �ŏ��l���r
		if (m_pVtxMin.x > vtx.x)
		{
			m_pVtxMin.x = vtx.x;
		}
		if (m_pVtxMin.y > vtx.y)
		{
			m_pVtxMin.y = vtx.y;
		}
		if (m_pVtxMin.z > vtx.z)
		{
			m_pVtxMin.z = vtx.z;
		}
		// �T�C�Y���|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}

	// ���_�o�b�t�@���A�����b�N
	m_pMesh->UnlockVertexBuffer();
}

//=============================================================================
// �ǂݍ��݂̔j��
//=============================================================================
void CBullet::UnloadModel(void)
{
	// ���b�V���̔j��
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// �o�b�t�@�̔j��
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	// �e�N�X�`���̔j��
	if (m_ppTexture != NULL)
	{
		m_ppTexture->Release();
		m_ppTexture = NULL;
	}
}

//=============================================================================
// �ړ�����
//=============================================================================
void CBullet::Move(void)
{
	m_move.x -= sinf(m_rot.y) * BULLET_MOVESPEED;
	m_move.z -= cosf(m_rot.y) * BULLET_MOVESPEED;
	m_pos += m_move;
	SetPos(m_pos);
	if (m_pLoadEffect[1] != NULL)
	{
		//�ړ��G�t�F�N�g
		m_pLoadEffect[1]->SetPos(D3DXVECTOR3(GetPos().x, GetPos().y + 20.0f, GetPos().z));
		//�ʏ�̈ړ��͂�����
		m_pLoadEffect[1]->Update();
	}
}

//=============================================================================
// ��������
//=============================================================================
void CBullet::Explosion(void)
{
	if (m_bExplosion == false)
	{// �������Ă��Ȃ������Ƃ�
		CreateExplosionEffect();	// �����G�t�F�N�g�̐���
		CreateExplosionCollider();	// ���������蔻��̐���
		m_bExplosion = true;
	}
	else
	{
		// �����̎������؂ꂽ��I��
		m_nExplosionTimer++;
		if (m_nExplosionTimer >= BULLET_EXPLOSION_TIME)
		{
			m_bDeth = true;
		}
	}
}

//=============================================================================
//    �N���p�̓����蔻��𐶐����鏈��
//=============================================================================
void CBullet::CreateCollider(void)
{
	// �R���C�_�[�Ǌ��N���X�𐶐�
	CColliderManager *pColManager = CColliderManager::Create(2);
	if (pColManager == NULL) { return; }

	// �~���𐶐�
	CCylinderCollider *pCylinder = CCylinderCollider::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f),
		BULLET_COLRADIUS, BULLET_COLRADIUS + 100.0f, false);
	if (pCylinder == NULL) { return; }

	// �e��ݒ�
	pCylinder->SetParent(this);
	pCylinder->SetParentMtxWorld(&this->m_mtxWorld);

	// �R���C�_�[�Ǌ��N���X�̃|�C���^��ݒ�
	pColManager->SetCollider(pCylinder, 0);
	SetCOlliderManager(pColManager);
}

//=============================================================================
//    �����̃G�t�F�N�g�𐶐����鏈��
//=============================================================================
void CBullet::CreateExplosionEffect(void)
{
	// ����
	if (m_pLoadEffect[0] != NULL)
	{
		m_pLoadEffect[0]->SetPos(m_pos);
		m_pLoadEffect[0]->OnTimeEffect();
	}
}

//=============================================================================
//    �����p�̓����蔻��𐶐����鏈��
//=============================================================================
void CBullet::CreateExplosionCollider(void)
{
	// ���𐶐�
	CPlayerAttackSphereCollider *pShere = CPlayerAttackSphereCollider::Create(GetPos(), D3DXVECTOR3(1.0f, 1.0f, 1.0f),
		BULLET_EXPLOSION_COLRADIUS, BULLET_EXPLOSION_TIME + 2, 1);
	if (pShere == NULL) { return; }

	// �e��ݒ�
	pShere->SetParent(this);

	// �R���C�_�[�Ǌ��N���X�Ƀ|�C���^��ݒ�
	CColliderManager *pColManager = GetColloderManger();
	pColManager->SetCollider(pShere, 1);
}

//=============================================================================
//�@�v���C���[�ƃe���g���[�̓����蔻��
//=============================================================================
void CBullet::Collision(void)
{
	//�e��l�̎擾
	D3DXVECTOR3 pos = m_pos;
	D3DXVECTOR3 posOld = m_posOld;
	D3DXVECTOR3 Move = INITIALIZE_VECTOR3;
	D3DXVECTOR3 ColRange;
	//����J�n
	CCollider *pCol = NULL;
	CCollider *pColNext = NULL;

	for (int nCntType = 0; nCntType < CCollider::TYPE_MAX; nCntType++)
	{
		pCol = CCollider::GetTop(nCntType);

		while (pCol != NULL)
		{
			pColNext = pCol->GetNext();
			CollisionCollider(pCol, pos, posOld, m_move, D3DXVECTOR3(15.0f, 15.0f, 15.0f));
			pCol = pColNext;
		}
	}
}

//=============================================================================
//�@�R���C�_�[�Ƃ̓����蔻�菈��
//=============================================================================
bool CBullet::CollisionCollider(CCollider *pCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
{
	if (pCollider->GetType() == CCollider::TYPE_BOX)
	{
		if (CollisionBoxCollider((CBoxCollider*)pCollider, pos, posOld, Move, ColRange) == true)
		{

		}
	}
	return false;
}

//=============================================================================
//�@�{�b�N�X�R���C�_�[�Ƃ̓����蔻�菈��
//=============================================================================
bool CBullet::CollisionBoxCollider(CBoxCollider *pBoxCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
{
	bool bLand = false;

	if (pBoxCollider->Collision(&pos, &posOld, &Move, ColRange, this, &bLand) == true)
	{
		if (bLand == true)
		{
		}
		m_bDeth = true;
		return true;
	}
	return false;
}