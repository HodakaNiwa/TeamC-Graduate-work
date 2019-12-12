//*****************************************************************************
//
//     �n���̏���[mine.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "mine.h"
#include "manager.h"
#include "renderer.h"
#include "collision.h"
#include "loadEffect.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define MINE_MODEL_FILENAME      "data/MODEL/mine000.x"			// �n�����f���̃t�@�C���p�X��
#define MINE_SCALE_INI           (D3DXVECTOR3(5.0f,5.0f,5.0f))	// �n���̑傫��(�����l)
#define MINE_LINE_INI            (15 * 60)						// �n���̎���(�����l)
#define MINE_COLRADIUS           (30.0f)						// �n�����N�����锻��͈̔�
#define MINE_EXPLOSION_SCALEUP   (0.3f)							// �n������������O�ɂǂꂭ�炢�傫���Ȃ邩
#define MINE_EXPLOSION_TIME      (45)							// �n���̔�������
#define MINE_EXPLOSION_COLRADIUS (50.0f)						// �n���̔����L���͈�
#define MINE_EXPLOSION_STARTTIME (5)							// �n������������܂ł̎���

//*****************************************************************************
//     �ÓI�����o�ϐ��錾
//*****************************************************************************
LPD3DXMESH CMine::m_pMesh = NULL;		// ���b�V���ւ̃|�C���^
LPD3DXBUFFER CMine::m_pBuffMat = NULL;	// �}�e���A�����ւ̃|�C���^
DWORD CMine::m_nNumMat = 0;				// �}�e���A�����̐�

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CMine::CMine(int nPriority, OBJTYPE objtype) : CSceneX(nPriority, objtype)
{
	m_nType = TYPE_NONE;
	m_nLife = 0;
	m_fScaleUp = 0.0f;
	m_nExplosionTimer = 0;
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CMine::~CMine()
{

}

//=============================================================================
//    ��������
//=============================================================================
CMine *CMine::Create(D3DXVECTOR3 pos, CScene *pParentChara)
{
	// ���������m��
	CMine *pMine = NULL;
	if (pMine != NULL) { return NULL; }
	pMine = new CMine;
	if (pMine == NULL) { return NULL; }

	// �e��l�̐ݒ�
	pMine->SetPos(pos);
	pMine->SetParentChara(pParentChara);

	// ����������
	if (FAILED(pMine->Init()))
	{
		pMine->Uninit();
		delete pMine;
		return NULL;
	}

	return pMine;
}

//=============================================================================
//    ���f���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CMine::Load(void)
{
	// �|�C���^���N���A����Ă��Ȃ��Ȃ玸�s
	if (m_pMesh != NULL || m_pBuffMat != NULL) { return E_FAIL; }

	// ���f���̓ǂݍ���
	D3DXLoadMeshFromX(MINE_MODEL_FILENAME,
		D3DXMESH_SYSTEMMEM,
		CManager::GetRenderer()->GetDevice(),
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	// ���f�����ǂݍ��߂ĂȂ��Ȃ玸�s
	if (m_pMesh == NULL || m_pBuffMat == NULL) { return E_FAIL; }

	return S_OK;
}

//=============================================================================
//    ���f���̊J������
//=============================================================================
void CMine::UnLoad(void)
{
	// ���b�V���ւ̃|�C���^
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// �}�e���A�����ւ̃|�C���^
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	// �}�e���A�����̐���������
	m_nNumMat = 0;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CMine::Init(void)
{
	// ���f�����蓖��
	BindModel(m_pMesh, m_pBuffMat, m_nNumMat, NULL);
	SetMat(1.0f);
	SetScale(D3DXVECTOR3(5.0f, 5.0f, 5.0f));

	// �����̏����l��ݒ�
	m_nLife = MINE_LINE_INI;

	// �n���N���p�̓����蔻���t�^����
	CreateStartUpCollider();

	//�ϐ��̏�����
	m_pLoadEffect = NULL;

	//�G�t�F�N�g�̏�����
	if (m_pLoadEffect == NULL)
	{
		m_pLoadEffect = CLoadEffect::Create(3, GetPos(), 5);
	}
	return S_OK;
}

//=============================================================================
//    �N���p�̓����蔻��𐶐����鏈��
//=============================================================================
void CMine::CreateStartUpCollider(void)
{
	// �R���C�_�[�Ǌ��N���X�𐶐�
	CColliderManager *pColManager = CColliderManager::Create(2);
	if (pColManager == NULL) { return; }

	// �~���𐶐�
	CCylinderCollider *pCylinder = CCylinderCollider::Create(GetPos(), D3DXVECTOR3(1.0f, 1.0f, 1.0f),
		MINE_COLRADIUS, MINE_COLRADIUS, false);
	if (pCylinder == NULL) { return; }

	// �e��ݒ�
	pCylinder->SetParent(this);

	// �R���C�_�[�Ǌ��N���X�̃|�C���^��ݒ�
	pColManager->SetCollider(pCylinder, 0);
	SetCOlliderManager(pColManager);
}

//=============================================================================
//    �I������
//=============================================================================
void CMine::Uninit(void)
{
	if (m_pLoadEffect != NULL)
	{//�G�t�F�N�g�̔j��
		m_pLoadEffect->Uninit();
		delete m_pLoadEffect;
		m_pLoadEffect = NULL;
	}

	// ���ʂ̏I������
	CSceneX::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CMine::Update(void)
{
	switch (m_nType)
	{// ��Ԃɂ���ď����킯
	case TYPE_NONE:
		None();
		break;
	case TYPE_STARTUP:
		StartUp();
		break;
	case TYPE_EXPLOSION:
		Explosion();
		break;
	}
}

//=============================================================================
//    �ʏ��Ԃ̍X�V����
//=============================================================================
void CMine::None(void)
{
	// ���������炷
	m_nLife--;
	if (m_nLife <= 0)
	{// �������Ȃ��Ȃ���
		m_nExplosionTimer = 0;
		m_nType = TYPE_STARTUP;
	}
}

//=============================================================================
//    �N����Ԃ̍X�V����
//=============================================================================
void CMine::StartUp(void)
{
	// �傫������
	m_fScaleUp += MINE_EXPLOSION_SCALEUP;
	D3DXVECTOR3 Scale = MINE_SCALE_INI;
	Scale.x += m_fScaleUp;
	Scale.y += m_fScaleUp;
	Scale.z += m_fScaleUp;
	SetScale(Scale);

	// �J�E���^�[�����܂����甚��
	m_nExplosionTimer++;
	if (m_nExplosionTimer >= MINE_EXPLOSION_STARTTIME)
	{
		// �J�E���^�[�����������ď�ԕύX
		m_nExplosionTimer = 0;
		m_nType = TYPE_EXPLOSION;

		// �����̃G�t�F�N�g�Ɠ����蔻��𐶐�����
		CreateExplosionEffect();
		CreateExplosionCollider();
	}
}

//=============================================================================
//    �����̃G�t�F�N�g�𐶐����鏈��
//=============================================================================
void CMine::CreateExplosionEffect(void)
{
	//�X�s�[�hUP�̎��͂�����
	m_pLoadEffect->OnTimeEffect();
}

//=============================================================================
//    �����p�̓����蔻��𐶐����鏈��
//=============================================================================
void CMine::CreateExplosionCollider(void)
{
	// ���𐶐�
	CPlayerAttackSphereCollider *pShere = CPlayerAttackSphereCollider::Create(GetPos(), D3DXVECTOR3(1.0f, 1.0f, 1.0f),
		MINE_EXPLOSION_COLRADIUS, MINE_EXPLOSION_TIME + 2, 1);
	if (pShere == NULL) { return; }

	// �e��ݒ�
	pShere->SetParent(this);

	// �R���C�_�[�Ǌ��N���X�Ƀ|�C���^��ݒ�
	CColliderManager *pColManager = GetColloderManger();
	pColManager->SetCollider(pShere, 1);
}

//=============================================================================
//    ������Ԃ̍X�V����
//=============================================================================
void CMine::Explosion(void)
{
	// �����̎������؂ꂽ��I��
	m_nExplosionTimer++;
	if (m_nExplosionTimer >= MINE_EXPLOSION_TIME)
	{
		Uninit();
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CMine::Draw(void)
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

	if (m_pLoadEffect != NULL)
	{//�G�t�F�N�g�̕`��
		m_pLoadEffect->Draw();
	}
}

//=============================================================================
//    �����蔻�菈��
//=============================================================================
void CMine::Collision(void)
{
	if (m_nType != TYPE_NONE) { return; }

	// �e��l�̎擾
	D3DXVECTOR3 pos = m_pos;
	float fLength = MINE_COLRADIUS;

	// ����J�n
	CCollider *pCol = NULL;
	CCollider *pColNext = NULL;
	for (int nCntType = 0; nCntType < CCollider::TYPE_MAX; nCntType++)
	{
		pCol = CCollider::GetTop(nCntType);

		while (pCol != NULL)
		{
			pColNext = pCol->GetNext();
			CollisionCollider(pCol, pos, fLength);
			pCol = pColNext;
		}
	}
}

//=============================================================================
//    �R���C�_�[�Ƃ̓����蔻�菈��
//=============================================================================
void CMine::CollisionCollider(CCollider *pCollider, D3DXVECTOR3 pos, float fLength)
{
	if (pCollider->GetType() == CCollider::TYPE_SPHERE_PLAYERATTACK)
	{
		if (CollisionPlayerAttackSphereCollider((CPlayerAttackSphereCollider*)pCollider, pos, fLength) == true)
		{
			CScene *pParent = pCollider->GetParent();
			if (pParent->GetObjType() == OBJTYPE_PLAYER || pParent->GetObjType() == OBJTYPE_ENEMY)
			{// �v���C���[���G��������n�����N��������
				SetType(TYPE_STARTUP);
			}
		}
	}
}

//=============================================================================
//    �v���C���[�̍U�����Ƃ̓����蔻�菈��
//=============================================================================
bool CMine::CollisionPlayerAttackSphereCollider(CPlayerAttackSphereCollider *pShere, D3DXVECTOR3 pos, float fLength)
{
	if (pShere->Collision(&pos, fLength, this) == true)
	{
		return true;
	}

	return false;
}