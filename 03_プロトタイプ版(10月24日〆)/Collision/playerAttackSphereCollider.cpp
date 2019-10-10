//*****************************************************************************
//
//     �v���C���[�̍U�������f���̏���[playerAttackSphereCollider.cpp]
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
#define PLAYERATTACKSPHERECOLLIER_MESH_PRIORITY (5)                              // �����p���b�V���X�t�B�A�̏����D�揇��
#define PLAYERATTACKSPHERECOLLIER_MESH_COL      (D3DXCOLOR(1.0f,1.0f,0.0f,0.5f)) // �����p���b�V���X�t�B�A�̐F
#define PLAYERATTACKSPHERECOLLIER_MESH_XBLOCK   (10)                             // �����p���b�V���X�t�B�A�̉��̕�����
#define PLAYERATTACKSPHERECOLLIER_MESH_YBLOCK   (10)                             // �����p���b�V���X�t�B�A�̏c�̕�����

//*****************************************************************************
//     �ÓI�����o�ϐ�
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CPlayerAttackSphereCollider::CPlayerAttackSphereCollider(TYPE type, CScene *pParent) : CSphereCollider(type, pParent)
{
	// �e��l�̃N���A
	m_nPower = 0;
	m_nLife = 0;
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CPlayerAttackSphereCollider::~CPlayerAttackSphereCollider()
{

}


//=============================================================================
//    ��������
//=============================================================================
CPlayerAttackSphereCollider *CPlayerAttackSphereCollider::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, float fRadius, int nLife, int nPower)
{
	CPlayerAttackSphereCollider *pSphereCollider = NULL;   // �v���C���[�̍U�������f���N���X�^�̃|�C���^
	pSphereCollider = new CPlayerAttackSphereCollider;
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
HRESULT CPlayerAttackSphereCollider::Init(void)
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
void CPlayerAttackSphereCollider::Uninit(void)
{
	CSphereCollider::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CPlayerAttackSphereCollider::Update(void)
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
void CPlayerAttackSphereCollider::Draw(void)
{
	CSphereCollider::Draw();
}

//=============================================================================
//    �����蔻������p�̃��b�V���X�t�B�A���쐬���鏈��
//=============================================================================
void CPlayerAttackSphereCollider::MakeColDispSphere(void)
{
	D3DXVECTOR3 scale = GetScale();
	float fScale = (scale.x + scale.y + scale.z) / 3.0f;
	float fRadius = GetRadius();
	CColDispSphere *pColDispSphere = CColDispSphere::Create(GetPos(), PLAYERATTACKSPHERECOLLIER_MESH_COL, fRadius / fScale,
		PLAYERATTACKSPHERECOLLIER_MESH_XBLOCK, PLAYERATTACKSPHERECOLLIER_MESH_YBLOCK, PLAYERATTACKSPHERECOLLIER_MESH_PRIORITY);
	SetColDispSphere(pColDispSphere);
}

//=============================================================================
//    �U���͂�ݒ肷�鏈��
//=============================================================================
void CPlayerAttackSphereCollider::SetPower(const int nPower)
{
	m_nPower = nPower;
}

//=============================================================================
//    ������ݒ肷�鏈��
//=============================================================================
void CPlayerAttackSphereCollider::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    �U���͂��擾���鏈��
//=============================================================================
int CPlayerAttackSphereCollider::GetPower(void)
{
	return m_nPower;
}

//=============================================================================
//    �������擾���鏈��
//=============================================================================
int CPlayerAttackSphereCollider::GetLife(void)
{
	return m_nLife;
}