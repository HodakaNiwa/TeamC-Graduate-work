//*****************************************************************************
//
//     �R���W�����̏���[collider.cpp]
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
bool      CCollider::m_bDrawAll = true;                    // �S�ẴR���C�_�[��`�悷�邩�ǂ���
CCollider *CCollider::m_apTop[CCollider::TYPE_MAX] = {};   // �擪�̃R���C�_�[�N���X�ւ̃|�C���^
CCollider *CCollider::m_apCur[CCollider::TYPE_MAX] = {};   // ���݂̍Ō���|�C���^�ۑ��p

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CCollider::CCollider(TYPE type, CScene *pParent)
{
	// �e��l�̃N���A
	m_pParent = pParent;                       // �R���C�_�[���������Ă���I�u�W�F�N�g�ւ̃|�C���^
	m_pPrev = NULL;                            // ���g�̑O�̃A�h���X
	m_pNext = NULL;                            // ���g�̎��̃A�h���X
	m_bDeath = false;                          // ���S�t���O
	m_Type = type;                             // �R���C�_�[�̎��
	m_bDeath = false;                          // ���S�t���O
	m_Pos = INITIALIZE_VECTOR3;                // ���݂̈ʒu
	m_PosOld = INITIALIZE_VECTOR3;             // �O��̈ʒu
	m_Rot = INITIALIZE_VECTOR3;                // ����
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);   // �傫��
	D3DXMatrixIdentity(&m_MtxWorld);           // ���[���h�}�g���b�N�X
	m_pParentMtxWorld = NULL;                  // �e�̃��[���h�}�g���b�N�X
	m_bDisp = true;                            // �`�悷�邩�ǂ���

	if (m_apTop[type] == NULL)
	{// �擪�A�h���X����ł���
		m_apTop[type] = this; // ���g�̃|�C���^����
	}
	if (m_apCur[type] != NULL)
	{// �Ō���A�h���X����ł͂Ȃ�
		m_apCur[type]->m_pNext = this;
		this->m_pPrev = m_apCur[type];
	}
	m_apCur[type] = this;     // �Ō���A�h���X�X�V
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CCollider::~CCollider()
{

}

//=============================================================================
//    �R���C�_�[�̍X�V����
//=============================================================================
void CCollider::UpdateAll(void)
{
	CCollider *pCollider;  // �X�V�����̊J�n�|�C���^��錾
	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{// �R���W�����̎�ނ̐������J��Ԃ�
		pCollider = m_apTop[nCntType];  // �X�V�����̊J�n�|�C���^����
		while (pCollider != NULL)
		{// ����������ɂȂ�܂Ń��[�v
			CCollider *pColliderNext = pCollider->m_pNext; // ���g�̎��̃A�h���X��ۑ�(�X�V�����Ŏ��g�̃|�C���^����ɂȂ�\��������̂�)
			pCollider->Update();                           // �X�V����
			pCollider = pColliderNext;                     // ���̍X�V�����̎��g�̎��̃A�h���X�̕`�揈�����s��
		}
	}
}

//=============================================================================
//    �����蔻�����������
//=============================================================================
void CCollider::DrawAll(void)
{
	// �������Ȃ��Ȃ珈���I��
	if (m_bDrawAll == false) { return; }

	CCollider *pCollider;  // �`�揈���̊J�n�|�C���^��錾
	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{// �R���W�����̎�ނ̐������J��Ԃ�
		pCollider = m_apTop[nCntType];  // �`�揈���̊J�n�|�C���^����
		while (pCollider != NULL)
		{// ����������ɂȂ�܂Ń��[�v
			CCollider *pColliderNext = pCollider->m_pNext; // ���g�̎��̃A�h���X��ۑ�(�`�揈���Ŏ��g�̃|�C���^����ɂȂ�\��������̂�)
			pCollider->Draw();                             // �`�揈��
			pCollider = pColliderNext;                     // ���̕`�揈���̎��g�̎��̃A�h���X�̕`�揈�����s��
		}
	}
}

//=============================================================================
//    �R���C�_�[��S�ĊJ�����鏈��
//=============================================================================
void CCollider::ReleaseAll(void)
{
	CCollider *pCollider;  // �I�������̊J�n�|�C���^��錾
	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{// �R���W�����̎�ނ̐������J��Ԃ�
		pCollider = m_apTop[nCntType];  // �I�������̊J�n�|�C���^����
		while (pCollider != NULL)
		{// ����������ɂȂ�܂Ń��[�v
			CCollider *pColliderNext = pCollider->m_pNext; // ���g�̎��̃A�h���X��ۑ�(�`�揈���Ŏ��g�̃|�C���^����ɂȂ�\��������̂�)
			pCollider->Uninit();                           // �I������
			pCollider = pColliderNext;                     // ���̏I�������̎��g�̎��̃A�h���X�̏I�������̍s��
		}
	}

	// ���S�t���O�`�F�b�N
	DeathCheck();

	// �����������������Ă���
	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{// �R���W�����̎�ނ̐������J��Ԃ�
		m_apTop[nCntType] = NULL;   // �擪�A�h���X
		m_apCur[nCntType] = NULL;   // ���݂̍Ō���A�h���X
	}
}

//=============================================================================
//    ���S�t���O�𗧂Ă鏈��
//=============================================================================
void CCollider::Release(void)
{
	if (m_bDeath == false)
	{// ���S�t���O�������Ă��Ȃ�
		m_bDeath = true;  // ���S�t���O�𗧂Ă�
	}
	else
	{// ���S�t���O�����łɗ����Ă���
		if (m_pPrev != NULL)
		{// ���g�̑O�̃A�h���X����ł͂Ȃ�
			m_pPrev->m_pNext = m_pNext;
		}
		if (m_pNext != NULL)
		{// ���g�̎��̃A�h���X����ł͂Ȃ�
			m_pNext->m_pPrev = m_pPrev;
		}
		if (this == m_apTop[m_Type])
		{// ���g�̃A�h���X���擪�A�h���X������
			m_apTop[m_Type] = m_pNext;
		}
		if (this == m_apCur[m_Type])
		{// ���g�̃A�h���X�����݂̍Ō���A�h���X������
			m_apCur[m_Type] = m_pPrev;
		}
		// �������̊J��
		delete this;
	}
}

//=============================================================================
//    ���S�t���O���m�F���鏈��
//=============================================================================
void CCollider::DeathCheck(void)
{
	CCollider *pCollider;  // �`�F�b�N�J�n�|�C���^
	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{// �`��D�揇�ʂ̐������J��Ԃ�
		pCollider = m_apTop[nCntType]; // �`�F�b�N�J�n�|�C���^��擪�ɂ���
		while (pCollider != NULL)
		{// ����������ɂȂ�܂Ń��[�v
			CCollider *pColliderNext = pCollider->m_pNext; // ���g�̎��̃A�h���X��ۑ�
			if (pCollider->m_bDeath == true)
			{// ���S�t���O�������Ă���
				pCollider->Release();                      // ���X�g����폜����
			}
			pCollider = pColliderNext;                     // ���̎��S�t���O�`�F�b�N�͎��g�̎��̃A�h���X�̃`�F�b�N���s��
		}
	}
}

//=============================================================================
//    �����蔻�����ON/OFF��؂�ւ���
//=============================================================================
void CCollider::SwicthDraw(void)
{
	m_bDrawAll = m_bDrawAll ? false : true;
}

//=============================================================================
//    �����蔻����������邩�ǂ������擾����
//=============================================================================
bool CCollider::GetDrawAll(void)
{
	return m_bDrawAll;
}

//=============================================================================
//    �擪�̃R���C�_�[�N���X�ւ̃|�C���^�擾
//=============================================================================
CCollider *CCollider::GetTop(int nType)
{
	return m_apTop[nType];
}

//=============================================================================
//    ���[���h�}�g���b�N�X���v�Z����(�X�P�[���͔��f���Ȃ�)����
//=============================================================================
void CCollider::CalcNotScaleTransform(void)
{
	D3DXMATRIX mtxRot, mtxParent; // �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_MtxWorld);

	// ��]�s����쐬(D3DXMatrixRotationYawPitchRoll�Q��)
	float fSinPitch = sinf(m_Rot.x);
	float fCosPitch = cosf(m_Rot.x);
	float fSinYaw = sinf(m_Rot.y);
	float fCosYaw = cosf(m_Rot.y);
	float fSinRoll = sinf(m_Rot.z);
	float fCosRoll = cosf(m_Rot.z);
	mtxRot._11 = fSinRoll * fSinPitch * fSinYaw + fCosRoll * fCosYaw;
	mtxRot._12 = fSinRoll * fCosPitch;
	mtxRot._13 = fSinRoll * fSinPitch * fCosYaw - fCosRoll * fSinYaw;
	mtxRot._21 = fCosRoll * fSinPitch * fSinYaw - fSinRoll * fCosYaw;
	mtxRot._22 = fCosRoll * fCosPitch;
	mtxRot._23 = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fSinYaw;
	mtxRot._31 = fCosPitch * fSinYaw;
	mtxRot._32 = -fSinPitch;
	mtxRot._33 = fCosPitch * fCosYaw;

	// ��]�𔽉f����
	m_MtxWorld._11 = mtxRot._11;
	m_MtxWorld._12 = mtxRot._12;
	m_MtxWorld._13 = mtxRot._13;
	m_MtxWorld._21 = mtxRot._21;
	m_MtxWorld._22 = mtxRot._22;
	m_MtxWorld._23 = mtxRot._23;
	m_MtxWorld._31 = mtxRot._31;
	m_MtxWorld._32 = mtxRot._32;
	m_MtxWorld._33 = mtxRot._33;

	// �I�t�Z�b�g�ʒu�𔽉f
	m_MtxWorld._41 = m_Pos.x;
	m_MtxWorld._42 = m_Pos.y;
	m_MtxWorld._43 = m_Pos.z;

	// �e������Ώ��𔽉f
	if (m_pParentMtxWorld != NULL)
	{
		mtxParent._11 = m_pParentMtxWorld->_11;
		mtxParent._12 = m_pParentMtxWorld->_12;
		mtxParent._13 = m_pParentMtxWorld->_13;
		mtxParent._14 = m_pParentMtxWorld->_14;
		mtxParent._21 = m_pParentMtxWorld->_21;
		mtxParent._22 = m_pParentMtxWorld->_22;
		mtxParent._23 = m_pParentMtxWorld->_23;
		mtxParent._24 = m_pParentMtxWorld->_24;
		mtxParent._31 = m_pParentMtxWorld->_31;
		mtxParent._32 = m_pParentMtxWorld->_32;
		mtxParent._33 = m_pParentMtxWorld->_33;
		mtxParent._34 = m_pParentMtxWorld->_34;
		mtxParent._41 = m_pParentMtxWorld->_41;
		mtxParent._42 = m_pParentMtxWorld->_42;
		mtxParent._43 = m_pParentMtxWorld->_43;
		mtxParent._44 = m_pParentMtxWorld->_44;
		D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxParent);
	}
}

//=============================================================================
//    �g�����X�t�H�[�������f�o�C�X�ɐݒ肷��
//=============================================================================
void CCollider::SetTransformToDevice(const LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}


//=============================================================================
//    �R���C�_�[���������Ă���I�u�W�F�N�g�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CCollider::SetParent(CScene *pParent)
{
	m_pParent = pParent;
}

//=============================================================================
//    ���݂̈ʒu��ݒ�
//=============================================================================
void CCollider::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    �O��̈ʒu��ݒ�
//=============================================================================
void CCollider::SetPosOld(const D3DXVECTOR3 posOld)
{
	m_PosOld = posOld;
}

//=============================================================================
//    ������ݒ�
//=============================================================================
void CCollider::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    �傫����ݒ�
//=============================================================================
void CCollider::SetScale(const D3DXVECTOR3 scale)
{
	m_Scale = scale;
}

//=============================================================================
//    ���[���h�}�g���b�N�X��ݒ�
//=============================================================================
void CCollider::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    �e�̃��[���h�}�g���b�N�X�ւ̃|�C���^��ݒ�
//=============================================================================
void CCollider::SetParentMtxWorld(D3DXMATRIX *pParentMtxWorld)
{
	m_pParentMtxWorld = pParentMtxWorld;
}

//=============================================================================
//    �����蔻����������邩�ǂ����ݒ�
//=============================================================================
void CCollider::SetDisp(const bool bDisp)
{
	m_bDisp = bDisp;
}

//=============================================================================
//    �R���C�_�[���������Ă���I�u�W�F�N�g�ւ̃|�C���^���擾
//=============================================================================
CScene *CCollider::GetParent(void)
{
	return m_pParent;
}

//=============================================================================
//    ���̃A�h���X���擾
//=============================================================================
CCollider *CCollider::GetNext(void)
{
	return m_pNext;
}

//=============================================================================
//    �O�̃A�h���X���擾
//=============================================================================
CCollider *CCollider::GetPrev(void)
{
	return m_pPrev;
}

//=============================================================================
//    ��ނ��擾
//=============================================================================
CCollider::TYPE CCollider::GetType(void)
{
	return m_Type;
}

//=============================================================================
//    ���݂̈ʒu���擾
//=============================================================================
D3DXVECTOR3 CCollider::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    �O��̈ʒu���擾
//=============================================================================
D3DXVECTOR3 CCollider::GetPosOld(void)
{
	return m_PosOld;
}

//=============================================================================
//    �������擾
//=============================================================================
D3DXVECTOR3 CCollider::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    �傫�����擾
//=============================================================================
D3DXVECTOR3 CCollider::GetScale(void)
{
	return m_Scale;
}

//=============================================================================
//    ���[���h�}�g���b�N�X���擾
//=============================================================================
D3DXMATRIX CCollider::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    �e�̃��[���h�}�g���b�N�X�ւ̃|�C���^���擾
//=============================================================================
D3DXMATRIX *CCollider::GetParentMtxWorld(void)
{
	return m_pParentMtxWorld;
}

//=============================================================================
//    �����蔻����������邩�ǂ������擾
//=============================================================================
bool CCollider::GetDisp(void)
{
	return m_bDisp;
}