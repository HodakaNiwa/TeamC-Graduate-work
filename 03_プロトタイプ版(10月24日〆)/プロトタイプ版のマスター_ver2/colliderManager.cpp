//*****************************************************************************
//
//     �R���C�_�[�Ǌ��̏���[colliderManager.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "collision.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************

//*****************************************************************************
//    �ÓI�����o�ϐ�
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CColliderManager::CColliderManager()
{
	m_nNumCollider = 0;
	m_pCollider = NULL;
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CColliderManager::~CColliderManager()
{

}

//=============================================================================
//    ��������
//=============================================================================
CColliderManager *CColliderManager::Create(int nNumCollider)
{
	// ���������m��
	CColliderManager *pColliderManager = NULL;
	pColliderManager = new CColliderManager;
	if (pColliderManager == NULL)return NULL;

	// ����������
	if (FAILED(pColliderManager->Init(nNumCollider)))
	{
		pColliderManager->Uninit();
		delete pColliderManager;
		pColliderManager = NULL;
		return NULL;
	}

	return pColliderManager;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CColliderManager::Init(int nNumCollider)
{
	// �R���C�_�[�̐���ݒ�
	SetNumCollider(nNumCollider);

	// �R���C�_�[�̐��������������m�ۂ��Ă���
	MakeMemoryCollider();

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CColliderManager::Uninit(void)
{
	// �R���C�_�[�̏I������
	if (m_pCollider != NULL)
	{
		ReleaseCollider();
	}

	// ���g�̃|�C���^���폜
	delete this;
}

//=============================================================================
//    �R���C�_�[���J�����鏈��
//=============================================================================
void CColliderManager::MakeMemoryCollider(void)
{
	// ���������m�ۂ���
	if (m_pCollider != NULL) { return; }
	m_pCollider = new CCollider*[m_nNumCollider];
	if (m_pCollider == NULL) { return; }

	// ���������m�ۂ�������ɂ��Ă���
	for (int nCntCol = 0; nCntCol < m_nNumCollider; nCntCol++)
	{
		m_pCollider[nCntCol] = NULL;
	}
}

//=============================================================================
//    �R���C�_�[���J�����鏈��
//=============================================================================
void CColliderManager::ReleaseCollider(void)
{
	// �R���C�_�[�̐������I������
	for (int nCntCol = 0; nCntCol < m_nNumCollider; nCntCol++)
	{
		if (m_pCollider[nCntCol] != NULL)
		{
			m_pCollider[nCntCol]->Uninit();
			m_pCollider[nCntCol] = NULL;
		}
	}

	// �������̊J��
	delete[] m_pCollider;
	m_pCollider = NULL;
}

//=============================================================================
//    �R���C�_�[�̐���ݒ肷��
//=============================================================================
void CColliderManager::SetNumCollider(const int nNumCollider)
{
	m_nNumCollider = nNumCollider;
}

//=============================================================================
//    �R���C�_�[�N���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CColliderManager::SetCollider(CCollider *pCollider, int nIdx)
{
	m_pCollider[nIdx] = pCollider;
}

//=============================================================================
//    �R���C�_�[�̐����擾����
//=============================================================================
int CColliderManager::GetNumCollider(void)
{
	return m_nNumCollider;
}

//=============================================================================
//    �R���C�_�[�N���X�ւ̃|�C���^���擾����
//=============================================================================
CCollider **CColliderManager::GetCollider(void)
{
	return m_pCollider;
}