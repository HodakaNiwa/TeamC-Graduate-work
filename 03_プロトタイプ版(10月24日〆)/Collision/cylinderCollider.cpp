//*****************************************************************************
//
//     �����蔻��p�����f���̏���[boxCollider.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "collision.h"
#include "manager.h"
#include "system.h"
#include "library.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define CYLINDERCOLLIDER_LINE_COL (D3DXCOLOR(1.0f,0.0f,1.0f,1.0f))

//*****************************************************************************
//     �ÓI�����o�ϐ�
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CCylinderCollider::CCylinderCollider(TYPE type, CScene *pParent) : CCollider(type, pParent)
{
	// �e��l�̃N���A
	m_pVtxBuff = NULL;      // ���_�o�b�t�@�ւ̃|�C���^
	m_pIdxBuff = NULL;      // �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	m_fRadius = 0.0f;       // ���̔��a
	m_fHeight = 0.0f;       // ���̍���
	m_bReturnFlag = false;  // ���������ۂɖ߂����ǂ���
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CCylinderCollider::~CCylinderCollider()
{

}

//=============================================================================
//    ��������
//=============================================================================
CCylinderCollider *CCylinderCollider::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, float fRadius, float fHeight, bool bReturnFlag, CScene *pParent)
{
	CCylinderCollider *pCylinderCollider = NULL;   // �����蔻��p�����f���N���X�^�̃|�C���^
	pCylinderCollider = new CCylinderCollider;
	if (pCylinderCollider == NULL) { return NULL; }

	// �e��l�̐ݒ�
	pCylinderCollider->SetPos(pos);                 // ���W
	pCylinderCollider->SetScale(scale);             // �傫��
	pCylinderCollider->SetRadius(fRadius);          // ���̉���
	pCylinderCollider->SetHeight(fHeight);          // ���̍���
	pCylinderCollider->SetReturnFlag(bReturnFlag);  // ���������ۂɖ߂����ǂ���
	if (FAILED(pCylinderCollider->Init()))
	{
		pCylinderCollider->Uninit();
		pCylinderCollider = NULL;
		return NULL;
	}

	return pCylinderCollider;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CCylinderCollider::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

	// ���_�o�b�t�@�̐���
	MakeVertex(pDevice);

	// �C���f�b�N�X�o�b�t�@�̐���
	MakeIndex(pDevice);

	// �g�����X�t�H�[�������v�Z
	CalcNotScaleTransform();

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CCylinderCollider::Uninit(void)
{
	Release();	// ���S�t���O�𗧂Ă�
}

//=============================================================================
//    �X�V����
//=============================================================================
void CCylinderCollider::Update(void)
{
	D3DXMATRIX mtxWorld = GetMtxWorld();
	SetPosOld(D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43));

	// �g�����X�t�H�[�������v�Z
	CalcNotScaleTransform();
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CCylinderCollider::Draw(void)
{
#ifdef _DEBUG
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetDevice();
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
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(CYLINDER_VERTEX));

	// �C���f�b�N�X�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF((D3DFVF_XYZ | D3DFVF_DIFFUSE));

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	// ���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, 18, 0, 40);
	//pDevice->DrawIndexedPrimitiveUP(D3DPT_LINELIST, 0, 18, 40, m_wIndex, D3DFMT_INDEX16, m_Vertex, sizeof(CYLINDER_VERTEX));

	// �J�����O�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_CULLMODE, Culling);

	// ���C�e�B���O�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);
#endif
}

//=============================================================================
//    ���_��񐶐�����
//=============================================================================
void CCylinderCollider::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(CYLINDER_VERTEX) * 18,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	CYLINDER_VERTEX *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W��ݒ�(���Ԃ͏㑤 -> ����)
	D3DXVECTOR3 scale = GetScale();
	float fScale = (scale.x + scale.z) / 2.0f;
	float fRadius = m_fRadius / fScale;
	float fPosY = m_fHeight / scale.y;
	float fCurve = 0.0f;
	for (int nCntUp = 0; nCntUp < 2; nCntUp++)
	{
		for (int nCntDown = 0; nCntDown < 8; nCntDown++)
		{
			pVtx[nCntDown + (nCntUp * 8)].pos = D3DXVECTOR3(sinf(fCurve) * -fRadius, fPosY, cosf(fCurve) * -fRadius);
			fCurve += D3DX_PI / 4;  // �p�x��i�߂�
			if (fCurve >= D3DX_PI)
			{// �{�����~�����𒴂���
				fCurve -= D3DX_PI * 2.0f;
			}
		}
		fPosY = 0.0f;
		fCurve = 0.0f;
	}
	pVtx[16].pos = D3DXVECTOR3(0.0f, m_fHeight / scale.y, 0.0f);
	pVtx[17].pos = INITIALIZE_VECTOR3;

	// ���_�J���[��ݒ�(�S�ĐԐF�ɐݒ�)
	for (int nCntVer = 0; nCntVer < 18; nCntVer++)
	{
		pVtx[nCntVer].col = CYLINDERCOLLIDER_LINE_COL;
	}
}

//=============================================================================
//    �C���f�b�N�X��񐶐�����
//=============================================================================
void CCylinderCollider::MakeIndex(const LPDIRECT3DDEVICE9 pDevice)
{
	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * 80,
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
	// ��̈͂�
	pIdx[0] = 0;
	pIdx[1] = 1;
	pIdx[2] = 1;
	pIdx[3] = 2;
	pIdx[4] = 2;
	pIdx[5] = 3;
	pIdx[6] = 3;
	pIdx[7] = 4;
	pIdx[8] = 4;
	pIdx[9] = 5;
	pIdx[10] = 5;
	pIdx[11] = 6;
	pIdx[12] = 6;
	pIdx[13] = 7;
	pIdx[14] = 7;
	pIdx[15] = 0;

	// ��->���̃��C��
	pIdx[16] = 0;
	pIdx[17] = 8;
	pIdx[18] = 1;
	pIdx[19] = 9;
	pIdx[20] = 2;
	pIdx[21] = 10;
	pIdx[22] = 3;
	pIdx[23] = 11;
	pIdx[24] = 4;
	pIdx[25] = 12;
	pIdx[26] = 5;
	pIdx[27] = 13;
	pIdx[28] = 6;
	pIdx[29] = 14;
	pIdx[30] = 7;
	pIdx[31] = 15;

	// ���̈͂�
	pIdx[32] = 8;
	pIdx[33] = 9;
	pIdx[34] = 9;
	pIdx[35] = 10;
	pIdx[36] = 10;
	pIdx[37] = 11;
	pIdx[38] = 11;
	pIdx[39] = 12;
	pIdx[40] = 12;
	pIdx[41] = 13;
	pIdx[42] = 13;
	pIdx[43] = 14;
	pIdx[44] = 14;
	pIdx[45] = 15;
	pIdx[46] = 15;
	pIdx[47] = 8;

	// ��̊W
	pIdx[48] = 16;
	pIdx[49] = 0;
	pIdx[50] = 16;
	pIdx[51] = 1;
	pIdx[52] = 16;
	pIdx[53] = 2;
	pIdx[54] = 16;
	pIdx[55] = 3;
	pIdx[56] = 16;
	pIdx[57] = 4;
	pIdx[58] = 16;
	pIdx[59] = 5;
	pIdx[60] = 16;
	pIdx[61] = 6;
	pIdx[62] = 16;
	pIdx[63] = 7;

	// ���̊W
	pIdx[64] = 17;
	pIdx[65] = 8;
	pIdx[66] = 17;
	pIdx[67] = 9;
	pIdx[68] = 17;
	pIdx[69] = 10;
	pIdx[70] = 17;
	pIdx[71] = 11;
	pIdx[72] = 17;
	pIdx[73] = 12;
	pIdx[74] = 17;
	pIdx[75] = 13;
	pIdx[76] = 17;
	pIdx[77] = 14;
	pIdx[78] = 17;
	pIdx[79] = 15;

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();
}

//=============================================================================
//    �����蔻�菈��
//=============================================================================
bool CCylinderCollider::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fColRadius, float fHeight, CScene *pScene, bool *pLand)
{
	// �����̐e�Ɠ����蔻��͂��Ȃ�
	CScene *pParent = GetParent();
	if (pParent == pScene) { return false; }

	bool bHit = false;

	// �ڂ��Ă��邩�`�F�b�N
	bHit = LandCollision(pPos, pPosOld, pMove, fColRadius, fHeight, pScene, pLand);
	if (bHit == true) { return bHit; }

	// �͈͓��`�F�b�N
	bHit = RangeCollision(pPos, pPosOld, pMove, fColRadius, fHeight, pScene, pLand);

	return bHit;
}

//=============================================================================
//    �ڂ��Ă邩�ǂ������`�F�b�N���鏈��
//=============================================================================
bool CCylinderCollider::LandCollision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fColRadius, float fHeight, CScene *pScene, bool *pLand)
{
	bool bHit = false;
	D3DXMATRIX mtxWorld = GetMtxWorld();
	D3DXVECTOR3 pos = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
	D3DXVECTOR3 posOld = GetPosOld();
	D3DXVECTOR3 Move = pos - posOld;
	float fLength = CFunctionLib::Vec2Length(pos.x, pos.z, pPos->x, pPos->z);
	float fHitRange = m_fRadius + fColRadius;

	if (pPosOld->y >= pos.y + m_fHeight && pPos->y <= pos.y + m_fHeight
		|| pPosOld->y >= posOld.y + m_fHeight && pPos->y <= posOld.y + m_fHeight)
	{// �������ォ�甠�̓����ɓ����Ă��܂���
		if (fLength <= fHitRange)
		{// ���̓����ɂ��锻�肾����
			if (m_bReturnFlag == true)
			{// �ʒu��߂��Ȃ��
				pPos->x += Move.x;
				pPos->z += Move.z;
				pPos->y = pos.y + m_fHeight;
				pMove->y = 0.0f;
				CDebugProc::Print("���ɏ���Ă��܂�\n");
			}
			bHit = true;
			if (pLand != NULL)
			{
				*pLand = true;
			}
		}
	}
	else if (pPosOld->y + fHeight <= pos.y && pPos->y + fHeight > pos.y
		|| pPosOld->y >= posOld.y && pPos->y < posOld.y)
	{// �����������甠�̓����ɓ����Ă��܂���
		if (fLength <= fHitRange)
		{// ���̓����ɂ��锻�肾����
			if (m_bReturnFlag == true)
			{// �ʒu��߂��Ȃ��
				pPos->y = pos.y - fHeight;
				pMove->y = 0.0f;
			}
			bHit = true;
		}
	}

	return bHit;
}

//=============================================================================
//    �͈͓��`�F�b�N����
//=============================================================================
bool CCylinderCollider::RangeCollision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fColRadius, float fHeight, CScene *pScene, bool *pLand)
{
	// �����̐e�Ɠ����蔻��͂��Ȃ�
	CScene *pParent = GetParent();
	if (pParent == pScene) { return false; }

	bool bHit = false;
	D3DXMATRIX mtxWorld = GetMtxWorld();
	D3DXVECTOR3 pos = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);

	// ���肷����W�Ƃ̋������v�Z
	float fLength = CFunctionLib::Vec2Length(pPos->x, pPos->z, pos.x, pos.z);

	// �������Ă���Ɣ��肳���͈͂��v�Z
	float fHitRange = m_fRadius + fColRadius;

	// ���������������Ɣ��肳���͈͂�菬����
	if (fLength < fHitRange && pPos->y <= pos.y + m_fHeight && pPos->y + fHeight >= pos.y)
	{
		if (m_bReturnFlag == true)
		{// �ʒu��߂��Ȃ��
		    // �x�N�g���ŉ����Ԃ�
			D3DXVECTOR3 VecA = pos - *pPos;
			D3DXVec3Normalize(&VecA, &VecA);
			pPos->x -= VecA.x * (fHitRange - fLength);
			pPos->z -= VecA.z * (fHitRange - fLength);
		}
		bHit = true;
	}

	return bHit;
}

//=============================================================================
//    ���̔��a�ݒ菈��
//=============================================================================
void CCylinderCollider::SetRadius(const float fRadius)
{
	m_fRadius = fRadius;
}

//=============================================================================
//    ���̍����ݒ菈��
//=============================================================================
void CCylinderCollider::SetHeight(const float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
//    ���������ۂɖ߂����ǂ����ݒ菈��
//=============================================================================
void CCylinderCollider::SetReturnFlag(const bool bReturnFlag)
{
	m_bReturnFlag = bReturnFlag;
}

//=============================================================================
//    ���̔��a�擾����
//=============================================================================
float CCylinderCollider::GetRadius(void)
{
	return m_fRadius;
}

//=============================================================================
//    ���̍����擾����
//=============================================================================
float CCylinderCollider::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    ���������ۂɖ߂����ǂ����擾����
//=============================================================================
bool CCylinderCollider::GetReturnFlag(void)
{
	return m_bReturnFlag;
}