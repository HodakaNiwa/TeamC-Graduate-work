//*****************************************************************************
//
//     �G�̍U�������f���̏���[enemyAttackSphereCollider.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "collision.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define ENEMYATTACKSPHERECOLLIER_MESH_PRIORITY (5)                              // �����p���b�V���X�t�B�A�̏����D�揇��
#define ENEMYATTACKSPHERECOLLIER_MESH_COL      (D3DXCOLOR(1.0f,0.0f,1.0f,0.5f)) // �����p���b�V���X�t�B�A�̐F
#define ENEMYATTACKSPHERECOLLIER_MESH_XBLOCK   (10)                             // �����p���b�V���X�t�B�A�̉��̕�����
#define ENEMYATTACKSPHERECOLLIER_MESH_YBLOCK   (10)                             // �����p���b�V���X�t�B�A�̏c�̕�����

//*****************************************************************************
//     �ÓI�����o�ϐ�
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CEnemyAttackSphereCollider::CEnemyAttackSphereCollider(TYPE type, CScene *pParent) : CSphereCollider(type, pParent)
{
	// �e��l�̃N���A
	m_nPower = 0;
	m_nLife = 0;
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CEnemyAttackSphereCollider::~CEnemyAttackSphereCollider()
{

}


//=============================================================================
//    ��������
//=============================================================================
CEnemyAttackSphereCollider *CEnemyAttackSphereCollider::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, float fRadius, int nLife, int nPower)
{
	CEnemyAttackSphereCollider *pSphereCollider = NULL;   // �G�̍U�������f���N���X�^�̃|�C���^
	pSphereCollider = new CEnemyAttackSphereCollider;
	if (pSphereCollider == NULL) { return NULL; }

	// �e��l�̐ݒ�
	pSphereCollider->SetPos(pos);           // ���̍��W
	pSphereCollider->SetScale(scale);       // ���̑傫��
	pSphereCollider->SetRadius(fRadius);    // ���̔��a
	pSphereCollider->SetReturnFlag(false);  // ���������ۂɉ����߂����ǂ���
	pSphereCollider->SetPower(nPower);      // �U����
	pSphereCollider->SetLife(nLife);        // ����
	if (FAILED(pSphereCollider->Init()))
	{
		pSphereCollider->Uninit();
		pSphereCollider = NULL;
		return NULL;
	}

	return pSphereCollider;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CEnemyAttackSphereCollider::Init(void)
{
	if (FAILED(CSphereCollider::Init()))
	{
		return E_FAIL;
	}

	// �g�����X�t�H�[�������v�Z
	CalcNotScaleTransform();

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CEnemyAttackSphereCollider::Uninit(void)
{
	CSphereCollider::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CEnemyAttackSphereCollider::Update(void)
{
	CSphereCollider::Update();

	// ���������炷
	m_nLife--;
	if (m_nLife <= 0)
	{// �������Ȃ��Ȃ���
		Uninit();
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CEnemyAttackSphereCollider::Draw(void)
{
	CSphereCollider::Draw();
}

//=============================================================================
//    �����蔻������p�̃��b�V���X�t�B�A���쐬���鏈��
//=============================================================================
void CEnemyAttackSphereCollider::MakeColDispSphere(void)
{
	D3DXVECTOR3 scale = GetScale();
	float fScale = (scale.x + scale.y + scale.z) / 3.0f;
	float fRadius = GetRadius();
	CColDispSphere *pColDispSphere = CColDispSphere::Create(GetPos(), ENEMYATTACKSPHERECOLLIER_MESH_COL, fRadius / fScale,
		ENEMYATTACKSPHERECOLLIER_MESH_XBLOCK, ENEMYATTACKSPHERECOLLIER_MESH_YBLOCK, ENEMYATTACKSPHERECOLLIER_MESH_PRIORITY);
	SetColDispSphere(pColDispSphere);
}

//=============================================================================
//    �U���͂�ݒ肷�鏈��
//=============================================================================
void CEnemyAttackSphereCollider::SetPower(const int nPower)
{
	m_nPower = nPower;
}

//=============================================================================
//    ������ݒ肷�鏈��
//=============================================================================
void CEnemyAttackSphereCollider::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    �U���͂��擾���鏈��
//=============================================================================
int CEnemyAttackSphereCollider::GetPower(void)
{
	return m_nPower;
}

//=============================================================================
//    �������擾���鏈��
//=============================================================================
int CEnemyAttackSphereCollider::GetLife(void)
{
	return m_nLife;
}