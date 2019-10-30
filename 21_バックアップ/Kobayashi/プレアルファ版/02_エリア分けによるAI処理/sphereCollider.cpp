//*****************************************************************************
//
//     �����蔻��p�����f���̏���[sphereCollider.cpp]
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
#define SPHERECOLLIER_MESH_PRIORITY (5)                              // �����p���b�V���X�t�B�A�̏����D�揇��
#define SPHERECOLLIER_MESH_COL      (D3DXCOLOR(0.0f,0.0f,1.0f,0.5f)) // �����p���b�V���X�t�B�A�̐F
#define SPHERECOLLIER_MESH_XBLOCK   (10)                             // �����p���b�V���X�t�B�A�̉��̕�����
#define SPHERECOLLIER_MESH_YBLOCK   (10)                             // �����p���b�V���X�t�B�A�̏c�̕�����

//*****************************************************************************
//     �ÓI�����o�ϐ�
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CSphereCollider::CSphereCollider(TYPE type, CScene *pParent) : CCollider(type, pParent)
{
	m_pColDispSphere = NULL;
	m_fRadius = 0.0f;
	m_bReturnFlag = false;
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CSphereCollider::~CSphereCollider()
{

}


//=============================================================================
//    ��������
//=============================================================================
CSphereCollider *CSphereCollider::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, float fRadius, bool bReturnFlag)
{
	CSphereCollider *pSphereCollider = NULL;   // �����蔻��p�����f���N���X�^�̃|�C���^
	pSphereCollider = new CSphereCollider;
	if (pSphereCollider == NULL) { return NULL; }

	// �e��l�̐ݒ�
	pSphereCollider->SetPos(pos);                 // ���̍��W
	pSphereCollider->SetScale(scale);             // ���̑傫��
	pSphereCollider->SetRadius(fRadius);          // ���̔��a
	pSphereCollider->SetReturnFlag(bReturnFlag);  // ���������ۂɉ����߂����ǂ���
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
HRESULT CSphereCollider::Init(void)
{
	// �g�����X�t�H�[�������v�Z
	CalcNotScaleTransform();

#ifdef _DEBUG
	// �����蔻������p���b�V���X�t�B�A���쐬
	MakeColDispSphere();
#endif

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CSphereCollider::Uninit(void)
{
	// �|�C���^����ɂ��Ă���
	m_pColDispSphere = NULL;

	Release();	// ���S�t���O�𗧂Ă�
}

//=============================================================================
//    �X�V����
//=============================================================================
void CSphereCollider::Update(void)
{
	D3DXMATRIX mtxWorld = GetMtxWorld();
	SetPosOld(D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43));

	// �g�����X�t�H�[�������v�Z
	CalcNotScaleTransform();
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CSphereCollider::Draw(void)
{
#ifdef _DEBUG
	// �����蔻������p���b�V���X�t�B�A�𓮂���
	if (m_pColDispSphere != NULL)
	{
		D3DXMATRIX mtxWorld = GetMtxWorld();
		m_pColDispSphere->SetPos(D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43));
		m_pColDispSphere->Disp();
	}
#endif
}

//=============================================================================
//    �����蔻�菈��
//=============================================================================
bool CSphereCollider::Collision(D3DXVECTOR3 *pPos, float fColRadius, CScene *pScene)
{
	// �����̐e�Ɠ����蔻��͂��Ȃ�
	CScene *pParent = GetParent();
	if (pParent == pScene) { return false; }

	bool bHit = false;
	D3DXMATRIX mtxWorld = GetMtxWorld();
	D3DXVECTOR3 pos = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);

	// ���肷����W�Ƃ̋������v�Z
	float fLength = CFunctionLib::Vec3Length(pPos, &pos);

	// �������Ă���Ɣ��肳���͈͂��v�Z
	float fHitRange = m_fRadius + fColRadius;

	// ���������������Ɣ��肳���͈͂�菬����
	if (fLength <= fHitRange)
	{
		if (m_bReturnFlag == true)
		{// �ʒu��߂��Ȃ��
		    // �x�N�g���ŉ����Ԃ�
			D3DXVECTOR3 VecA = pos - *pPos;
			D3DXVec3Normalize(&VecA, &VecA);
			pPos->x -= VecA.x * (fHitRange - fLength);
			pPos->y -= VecA.y * (fHitRange - fLength);
			pPos->z -= VecA.z * (fHitRange - fLength);
		}
		bHit = true;
	}

	return bHit;
}

//=============================================================================
//    �����蔻������p�̃��b�V���X�t�B�A���쐬���鏈��
//=============================================================================
void CSphereCollider::MakeColDispSphere(void)
{
	D3DXVECTOR3 scale = GetScale();
	float fScale = (scale.x + scale.y + scale.z) / 3.0f;
	m_pColDispSphere = CColDispSphere::Create(GetPos(), SPHERECOLLIER_MESH_COL, m_fRadius / fScale,
		SPHERECOLLIER_MESH_XBLOCK, SPHERECOLLIER_MESH_YBLOCK, SPHERECOLLIER_MESH_PRIORITY);
}

//=============================================================================
//    ���̔��a��ݒ肷�鏈��
//=============================================================================
void CSphereCollider::SetRadius(const float fRadius)
{
	m_fRadius = fRadius;
}

//=============================================================================
//    ���������ۂɖ߂����ǂ����ݒ菈��
//=============================================================================
void CSphereCollider::SetReturnFlag(const bool bReturnFlag)
{
	m_bReturnFlag = bReturnFlag;
}

//=============================================================================
//    �����蔻������p���b�V���N���X�ւ̃|�C���^��ݒ肷�鏈��
//=============================================================================
void CSphereCollider::SetColDispSphere(CColDispSphere *pColDispSphere)
{
	m_pColDispSphere = pColDispSphere;
}

//=============================================================================
//    ���̔��a���擾���鏈��
//=============================================================================
float CSphereCollider::GetRadius(void)
{
	return m_fRadius;
}

//=============================================================================
//    ���������ۂɖ߂����ǂ����擾����
//=============================================================================
bool CSphereCollider::GetReturnFlag(void)
{
	return m_bReturnFlag;
}

//=============================================================================
//    �����蔻������p���b�V���N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CColDispSphere *CSphereCollider::GetColDispSphere(void)
{
	return m_pColDispSphere;
}