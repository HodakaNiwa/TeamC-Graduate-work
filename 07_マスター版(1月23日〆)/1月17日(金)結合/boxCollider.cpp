//*****************************************************************************
//
//     �����蔻��p�����f���̏���[boxCollider.cpp]
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
#define BOXCOLLIDER_LINE_COL (D3DXCOLOR(1.0f,0.0f,1.0f,1.0f))

//*****************************************************************************
//     �ÓI�����o�ϐ�
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CBoxCollider::CBoxCollider(TYPE type, CScene *pParent) : CCollider(type, pParent)
{
	// �e��l�̃N���A
	m_pVtxBuff = NULL;      // ���_�o�b�t�@�ւ̃|�C���^
	m_pIdxBuff = NULL;      // �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	m_fWidth = 0.0f;        // ���̉���
	m_fHeight = 0.0f;       // ���̍���
	m_fDepth = 0.0f;        // ���̉��s
	m_bReturnFlag = false;  // ���������ۂɖ߂����ǂ���
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CBoxCollider::~CBoxCollider()
{

}

//=============================================================================
//    ��������
//=============================================================================
CBoxCollider *CBoxCollider::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, float fWidth, float fHeight, float fDepth, bool bReturnFlag)
{
	CBoxCollider *pBoxCollider = NULL;   // �����蔻��p�����f���N���X�^�̃|�C���^
	pBoxCollider = new CBoxCollider;
	if (pBoxCollider == NULL) { return NULL; }

	// �e��l�̐ݒ�
	pBoxCollider->SetPos(pos);                 // ���W
	pBoxCollider->SetScale(scale);             // �傫��
	pBoxCollider->SetWidth(fWidth);            // ���̉���
	pBoxCollider->SetHeight(fHeight);          // ���̍���
	pBoxCollider->SetDepth(fDepth);            // ���̉��s
	pBoxCollider->SetReturnFlag(bReturnFlag);  // ���������ۂɖ߂����ǂ���
	if (FAILED(pBoxCollider->Init()))
	{
		pBoxCollider->Uninit();
		pBoxCollider = NULL;
		return NULL;
	}

	return pBoxCollider;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CBoxCollider::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

#ifdef _DEBUG
	// ���_�o�b�t�@�̐���
	MakeVertex(pDevice);

	// �C���f�b�N�X�o�b�t�@�̐���
	MakeIndex(pDevice);
#endif

	// �g�����X�t�H�[�������v�Z
	CalcNotScaleTransform();

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CBoxCollider::Uninit(void)
{
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	Release();	// ���S�t���O�𗧂Ă�
}

//=============================================================================
//    �X�V����
//=============================================================================
void CBoxCollider::Update(void)
{
	D3DXMATRIX mtxWorld = GetMtxWorld();
	SetPosOld(D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43));

	// �g�����X�t�H�[�������v�Z
	CalcNotScaleTransform();
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CBoxCollider::Draw(void)
{
#ifdef _DEBUG
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return; }

	DWORD Culling;
	DWORD Lighting;

	// ���C�e�B���O���Ȃ��ݒ��
	pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �J�����O���O���Ă���
	pDevice->GetRenderState(D3DRS_CULLMODE, &Culling);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// �g�����X�t�H�[�������f�o�C�X�ɐݒ�
	SetTransformToDevice(pDevice);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(BOX_VERTEX));

	// �C���f�b�N�X�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF((D3DFVF_XYZ | D3DFVF_DIFFUSE));

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	// ���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, 8, 0, 12);

	// �J�����O�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_CULLMODE, Culling);

	// ���C�e�B���O�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);
#endif
}

//=============================================================================
//    ���_��񐶐�����
//=============================================================================
void CBoxCollider::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(BOX_VERTEX) * 8,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	BOX_VERTEX *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W��ݒ�(���Ԃ� �㑤�� ���� -> �E�� -> -> ���O -> �E��
	//                       ������ ���� -> �E�� -> -> ���O -> �E��)
	D3DXVECTOR3 scale = GetScale();
	float fCutWidth = -m_fWidth / 2.0f / scale.x;
	float fAddWidth = m_fWidth / 2.0f / scale.x;
	float fCutDepth = -m_fDepth / 2.0f / scale.z;
	float fAddDepth = m_fDepth / 2.0f / scale.z;
	float fHeight = m_fHeight / scale.y;
	pVtx[0].pos = D3DXVECTOR3(fCutWidth, fHeight, fAddDepth);
	pVtx[1].pos = D3DXVECTOR3(fAddWidth, fHeight, fAddDepth);
	pVtx[2].pos = D3DXVECTOR3(fCutWidth, fHeight, fCutDepth);
	pVtx[3].pos = D3DXVECTOR3(fAddWidth, fHeight, fCutDepth);
	pVtx[4].pos = D3DXVECTOR3(fCutWidth, 0.0f, fAddDepth);
	pVtx[5].pos = D3DXVECTOR3(fAddWidth, 0.0f, fAddDepth);
	pVtx[6].pos = D3DXVECTOR3(fCutWidth, 0.0f, fCutDepth);
	pVtx[7].pos = D3DXVECTOR3(fAddWidth, 0.0f, fCutDepth);

	// ���_�J���[��ݒ�(�S�ĐԐF�ɐݒ�)
	for (int nCntVer = 0; nCntVer < 8; nCntVer++)
	{
		pVtx[nCntVer].col = BOXCOLLIDER_LINE_COL;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}


//=============================================================================
//    �C���f�b�N�X��񐶐�����
//=============================================================================
void CBoxCollider::MakeIndex(const LPDIRECT3DDEVICE9 pDevice)
{
	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * 24,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	WORD *pIdx;       // �C���f�b�N�X�f�[�^�ւ̃|�C���^
	int nCntIdx = 0;  // �C���f�b�N�X�ԍ�

	// �C���f�b�N�X�o�b�t�@�����b�N��,�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// �C���f�b�N�X����ݒ�
	pIdx[0] = 0;
	pIdx[1] = 1;
	pIdx[2] = 1;
	pIdx[3] = 3;
	pIdx[4] = 3;
	pIdx[5] = 2;
	pIdx[6] = 2;
	pIdx[7] = 0;
	pIdx[8] = 0;
	pIdx[9] = 4;
	pIdx[10] = 1;
	pIdx[11] = 5;
	pIdx[12] = 2;
	pIdx[13] = 6;
	pIdx[14] = 3;
	pIdx[15] = 7;
	pIdx[16] = 4;
	pIdx[17] = 5;
	pIdx[18] = 5;
	pIdx[19] = 7;
	pIdx[20] = 7;
	pIdx[21] = 6;
	pIdx[22] = 6;
	pIdx[23] = 4;

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();
}

//=============================================================================
//    �����蔻�菈��
//=============================================================================
bool CBoxCollider::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 ColRange, CScene *pScene, bool *pLand)
{
	// �����̐e�Ɠ����蔻��͂��Ȃ�
	CScene *pParent = GetParent();
	if (pParent == pScene) { return false; }

	bool bHit = false;  // �����������ǂ���
	D3DXMATRIX mtxWorld = GetMtxWorld();
	D3DXVECTOR3 pos = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
	D3DXVECTOR3 posOld = GetPosOld();
	D3DXVECTOR3 Move = pos - posOld;
	float fCutWidth = -m_fWidth / 2.0f;
	float fAddWidth = m_fWidth / 2.0f;
	float fCutDepth = -m_fDepth / 2.0f;
	float fAddDepth = m_fDepth / 2.0f;

	// Y����̔���
	if (pPos->y < pos.y + m_fHeight && pPos->y + ColRange.y > pos.y)
	{// ���݂̍��������̒��ɂ���
	 // X����̔���
		if (pPos->z + ColRange.z > pos.z + fCutDepth && pPos->z - ColRange.z < pos.z + fAddDepth)
		{// Z����ɂ��邩�ǂ�������
			if (pPosOld->x + ColRange.x <= pos.x + fCutWidth  && pPos->x + ColRange.x > pos.x + fCutWidth
				|| pPosOld->x + ColRange.x <= posOld.x + fCutWidth && pPos->x + ColRange.x >= posOld.x + fCutWidth
				|| Move.x < 0.0f && posOld.x + fCutWidth >= pPos->x + ColRange.x && pos.x + fCutWidth < pPos->x + ColRange.x)
			{// ���̍������������
				if (m_bReturnFlag == true)
				{// �ʒu��߂��Ȃ��
					pPos->x = pos.x - ColRange.x + fCutWidth - 0.1f;
					if (pMove->y > 0.0f)
					{
						D3DXVECTOR3 Vec = *pPos - *pPosOld;
						D3DXVECTOR3 Reflect;
						CFunctionLib::CalcReflectVector3(&Reflect, &Vec, &D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
						D3DXVec3Normalize(&Reflect, &Reflect);
						pMove->y *= Reflect.y;
					}
				}
				bHit = true;
			}
			else if (pPosOld->x - ColRange.x >= pos.x + fAddWidth && pPos->x - ColRange.x < pos.x + fAddWidth
				|| pPosOld->x - ColRange.x >= posOld.x + fAddWidth && pPos->x - ColRange.x <= posOld.x + fAddWidth
				|| Move.x > 0.0f && posOld.x + fAddWidth <= pPos->x - ColRange.x && pos.x + fAddWidth > pPos->x - ColRange.x)
			{// ���̉E�����������
				if (m_bReturnFlag == true)
				{// �ʒu��߂��Ȃ��
					pPos->x = pos.x + fAddWidth + ColRange.x + 0.1f;
					if (pMove->y > 0.0f)
					{
						D3DXVECTOR3 Vec = *pPos - *pPosOld;
						D3DXVECTOR3 Reflect;
						CFunctionLib::CalcReflectVector3(&Reflect, &Vec, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
						D3DXVec3Normalize(&Reflect, &Reflect);
						pMove->y *= Reflect.y;
					}
				}
				bHit = true;
			}
		}
		// Z����̔���
		if (pPos->x + ColRange.x > pos.x + fCutWidth && pPos->x - ColRange.x < pos.x + fAddWidth)
		{// X����ɂ��邩�ǂ�������
			if (pPosOld->z - ColRange.z >= pos.z + fAddDepth && pPos->z - ColRange.z < pos.z + fAddDepth
				|| pPosOld->z - ColRange.z >= posOld.z + fAddDepth && pPos->z - ColRange.z < posOld.z + fAddDepth
				|| Move.z > 0.0f && posOld.z + fAddDepth <= pPos->z - ColRange.z && pos.z + fAddDepth > pPos->z - ColRange.z)
			{// ���̉������������
				if (m_bReturnFlag == true)
				{// �ʒu��߂��Ȃ��
					pPos->z = pos.z + fAddDepth + ColRange.z + 0.1f;
					if (pMove->y > 0.0f)
					{
						D3DXVECTOR3 Vec = *pPos - *pPosOld;
						D3DXVECTOR3 Reflect;
						CFunctionLib::CalcReflectVector3(&Reflect, &Vec, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
						D3DXVec3Normalize(&Reflect, &Reflect);
						pMove->y *= Reflect.y;
					}
				}
				bHit = true;
			}
			else if (pPosOld->z + ColRange.z <= pos.z + fCutDepth && pPos->z + ColRange.z > pos.z + fCutDepth
				|| pPosOld->z + ColRange.z <= posOld.z + fCutDepth && pPos->z + ColRange.z > posOld.z + fCutDepth
				|| Move.z < 0.0f && posOld.z + fCutDepth >= pPos->z + ColRange.z && pos.z + fCutDepth < pPos->z + ColRange.z)
			{// ���̎�O�����������
				if (m_bReturnFlag == true)
				{// �ʒu��߂��Ȃ��
					pPos->z = pos.z - ColRange.z + fCutDepth - 0.1f;
					if (pMove->y > 0.0f)
					{
						D3DXVECTOR3 Vec = *pPos - *pPosOld;
						D3DXVECTOR3 Reflect;
						CFunctionLib::CalcReflectVector3(&Reflect, &Vec, &D3DXVECTOR3(0.0f, 0.0f, -1.0f));
						D3DXVec3Normalize(&Reflect, &Reflect);
						pMove->y *= Reflect.y;
					}
				}
				bHit = true;
			}
		}
	}

	if (pPosOld->y >= pos.y + m_fHeight && pPos->y <= pos.y + m_fHeight
		 || pPosOld->y >= posOld.y + m_fHeight && pPos->y <= posOld.y + m_fHeight)
	{// �������ォ�甠�̓����ɓ����Ă��܂���
		if (pPos->x + ColRange.x > pos.x + fCutWidth && pPos->x - ColRange.x < pos.x + fAddWidth
			&& pPos->z + ColRange.z > pos.z + fCutDepth && pPos->z - ColRange.z < pos.z + fAddDepth)
		{// ���̓����ɂ��锻�肾����
			if (m_bReturnFlag == true)
			{// �ʒu��߂��Ȃ��
				pPos->x += Move.x;
				pPos->z += Move.z;
				pPos->y = pos.y + m_fHeight;
				pMove->y = 0.0f;
			}
			bHit = true;
			if (pLand != NULL)
			{
				*pLand = true;
			}
		}
	}
	else if (pPosOld->y + ColRange.y <= pos.y && pPos->y + ColRange.y > pos.y
		|| pPosOld->y >= posOld.y + m_fHeight && pPos->y < posOld.y + m_fHeight)
	{// �����������甠�̓����ɓ����Ă��܂���
		if (pPos->x + ColRange.x > pos.x + fCutWidth && pPos->x - ColRange.x < pos.x + fAddWidth
			&& pPos->z + ColRange.z > pos.z + fCutDepth && pPos->z - ColRange.z < pos.z + fAddDepth)
		{// ���̓����ɂ��锻�肾����
			if (m_bReturnFlag == true)
			{// �ʒu��߂��Ȃ��
				pPos->y = pos.y - ColRange.y;
				pMove->y = 0.0f;
			}
			bHit = true;
		}
	}

	return bHit;
}

//=============================================================================
//    ���̉����ݒ菈��
//=============================================================================
void CBoxCollider::SetWidth(const float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
//    ���̍����ݒ菈��
//=============================================================================
void CBoxCollider::SetHeight(const float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
//    ���̉��s�ݒ菈��
//=============================================================================
void CBoxCollider::SetDepth(const float fDepth)
{
	m_fDepth = fDepth;
}

//=============================================================================
//    ���������ۂɖ߂����ǂ����ݒ菈��
//=============================================================================
void CBoxCollider::SetReturnFlag(const bool bReturnFlag)
{
	m_bReturnFlag = bReturnFlag;
}

//=============================================================================
//    ���̉����擾����
//=============================================================================
float CBoxCollider::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    ���̍����擾����
//=============================================================================
float CBoxCollider::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    ���̉��s�擾����
//=============================================================================
float CBoxCollider::GetDepth(void)
{
	return m_fDepth;
}

//=============================================================================
//    ���������ۂɖ߂����ǂ����擾����
//=============================================================================
bool CBoxCollider::GetReturnFlag(void)
{
	return m_bReturnFlag;
}