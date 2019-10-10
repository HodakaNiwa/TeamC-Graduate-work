//*****************************************************************************
//
//     �����蔻��p���b�V�����f���̏���[meshCollider.cpp]
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

//*****************************************************************************
//     �ÓI�����o�ϐ�
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CMeshCollider::CMeshCollider(TYPE type, CScene *pParent) : CCollider(type, pParent)
{
	// �e��l�̃N���A
	m_pVerPos = NULL;           // ���_���W�ւ̃|�C���^
	m_fWidth = 0.0f;            // 1�u���b�N���̕�
	m_fDepth = 0.0f;            // 1�u���b�N���̉��s
	m_nXBlock = 0;              // ���̕�����
	m_nZBlock = 0;              // ���s�̕�����
	m_nNumVertex = 0;           // ���_��
	m_pVtxBuffParent = NULL;    // �e���_�o�b�t�@�ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CMeshCollider::~CMeshCollider()
{

}

//=============================================================================
//    ��������
//=============================================================================
CMeshCollider *CMeshCollider::Create(float fWidth, float fDepth, int nXBlock, int nZBlock, int nNumVertex, LPDIRECT3DVERTEXBUFFER9 pVtxBuffParent, D3DXMATRIX *pMtxParent, bool bReturnFlag, CScene *pParent)
{
	CMeshCollider *pMeshCollider = NULL;   // �����蔻��p���b�V�����f���N���X�^�̃|�C���^
	pMeshCollider = new CMeshCollider;
	if (pMeshCollider == NULL) { return NULL; }

	// �e��l�̐ݒ�
	pMeshCollider->SetWidth(fWidth);                   // 1�u���b�N���̕�
	pMeshCollider->SetDepth(fDepth);                   // 1�u���b�N���̉��s
	pMeshCollider->SetXBlock(nXBlock);                 // ���̕�����
	pMeshCollider->SetZBlock(nZBlock);                 // ���s�̕�����
	pMeshCollider->SetVtxBuffParent(pVtxBuffParent);   // �e���_�o�b�t�@�ւ̃|�C���^
	pMeshCollider->SetParentMtxWorld(pMtxParent);      // �e�}�g���b�N�X�ւ̃|�C���^
	pMeshCollider->SetReturnFlag(bReturnFlag);         // ���������ۂɖ߂����ǂ���
	if (FAILED(pMeshCollider->Init()))
	{
		pMeshCollider->Uninit();
		pMeshCollider = NULL;
		return NULL;
	}

	return pMeshCollider;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CMeshCollider::Init(void)
{
	// �`��f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

	// �쐬�ɕK�v�Ȓ��_��,�C���f�b�N�X��,�|���S�������v�Z
	m_nNumVertex = (m_nXBlock + 1) * (m_nZBlock + 1);

	// ���_���̍쐬
	MakeVertex(pDevice);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CMeshCollider::Uninit(void)
{
	// ���_���W�ւ̃|�C���^���J��
	if (m_pVerPos != NULL)
	{
		delete[] m_pVerPos;
		m_pVerPos = NULL;
	}

	// ���S�t���O�𗧂Ă�
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CMeshCollider::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CMeshCollider::Draw(void)
{

}

//=============================================================================
//    ���_�����쐬���鏈��
//=============================================================================
void CMeshCollider::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_���W�p�̃��������m��
	if (m_pVerPos == NULL)
	{
		m_pVerPos = new D3DXVECTOR3[m_nNumVertex];
	}

	// ���[���h��ԏ�ɒ��_���W��ݒ�
	SetWorldVertexPos();
}

//=============================================================================
//    ���[���h��ԏ�ɒ��_���W��ݒ肷�鏈��
//=============================================================================
void CMeshCollider::SetWorldVertexPos(void)
{
	if (m_pVerPos == NULL) { return; }

	// ���_���̐ݒ�
	VERTEX_3D *pVtxParent = NULL;
	D3DXMATRIX *m_pMtxParent = GetParentMtxWorld();

	// �e���_�o�b�t�@�ւ̃|�C���^����f�[�^���擾
	if (m_pVtxBuffParent != NULL)
	{
		m_pVtxBuffParent->Lock(0, 0, (void**)&pVtxParent, 0);
	}

	D3DXVECTOR3 VerPos = INITIALIZE_VECTOR3;
	VerPos.x = -(m_fWidth * m_nXBlock) / 2;
	VerPos.z = (m_fDepth * m_nZBlock) / 2;
	for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
	{// ���������̕����� + 1�����J��Ԃ�
		for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
		{// ���������̕����� + 1�����J��Ԃ�
			// �e���_�o�b�t�@��������擾
			if (pVtxParent != NULL)
			{
				VerPos = pVtxParent[nCntH].pos;
			}

			// �e�}�g���b�N�X�Ɗ|�����킹��
			if (m_pMtxParent != NULL)
			{
				D3DXVec3TransformCoord(&VerPos, &VerPos, m_pMtxParent);
			}

			// ���_���W��ݒ�
			m_pVerPos[nCntH + ((m_nXBlock + 1) * nCntV)] = VerPos;
		}

		// �|�C���^��i�߂�
		if (pVtxParent != NULL)
		{
			pVtxParent += m_nXBlock + 1;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	if (m_pVtxBuffParent != NULL)
	{
		m_pVtxBuffParent->Unlock();
	}
}

//=============================================================================
//    �|���S���̍������擾���鏈��
//=============================================================================
float CMeshCollider::GetPolyHeight(D3DXVECTOR3 pos, bool *pLand, D3DXVECTOR3 *pPolyNormal, D3DXVECTOR3 *pVerPos)
{
	if (m_pVerPos == NULL) { return 0.0f; }

	float fHeight = 0.0f;               // ����(�ŏI�I�ɂ��̒l��Ԃ�)
	float fRivision = 0.0f;             // �����̕␳�l(�@��������o���̂�0.0�̍����ɂ��낦�邽��)
	int nVertexIdxStart = 0;            // ����̒��_�ԍ�(���[�v���邽�тɂ���Ă���)
	D3DXVECTOR3 VertexLeftWorldPos[3];  // �����̃|���S���̒��_�̃��[���h���W3��(���_���W�ւ̃|�C���^���甲���o��)
	D3DXVECTOR3 VertexRightWorldPos[3]; // �E���̃|���S���̒��_�̃��[���h���W3��(���_���W�ւ̃|�C���^���甲���o��)
	D3DXVECTOR3 VectorVerToVer;         // �O�σx�N�g��1�{��(���_���璸�_�ւ̃x�N�g��)
	D3DXVECTOR3 VectorVerToPos;         // �O�σx�N�g��2�{��(���_��������̍��W�ւ̃x�N�g��)
	D3DXVECTOR3 PolygonVerPos[3];       // �|���S���̍��W(����p�̖@���𓱂����߂ɕK�v)
	D3DXVECTOR3 PolygonNormal;          // �|���S���̖@��(����p�̖@���Ȃ̂Ō����ڂ̖@���Ƃ͈قȂ�)
	D3DXVECTOR3 PolygonVector[2];       // �|���S���̖@��������o�����߂̃x�N�g��
	D3DXVECTOR3 VecPosToPolygon;        // �����̍��W�ƃ|���S���Ƃ̃x�N�g��
	int nCntLeftPolygon = 0;            // �����̃|���S���̃x�N�g�������ʂ�����
	int nCntRightPolygon = 0;           // �E���̃|���S���̃x�N�g�������ʂ�����

	// �t�B�[���h���ɂ��邩�ǂ�������
	D3DXVECTOR3 FieldPos = GetPos();
	if (pos.x <= FieldPos.x - (m_fWidth * m_nXBlock) || pos.x >= FieldPos.x + (m_fWidth * m_nXBlock)
		&& pos.z <= FieldPos.z - (m_fDepth * m_nZBlock) || pos.z >= FieldPos.z + (m_fDepth * m_nZBlock))
	{
		if (pLand != NULL)
		{
			*pLand = false;
		}
		return 0.0f;
	}

	for (int nCntV = 0; nCntV < m_nZBlock; nCntV++)
	{// ���s�̕��������J��Ԃ�
		for (int nCntH = 0; nCntH < m_nXBlock; nCntH++)
		{// ���̕��������J��Ԃ�
			// �����̃|���S���̔���J�n
			VertexLeftWorldPos[0] = m_pVerPos[nVertexIdxStart];
			VertexLeftWorldPos[1] = m_pVerPos[nVertexIdxStart + (m_nXBlock + 1) + 1];
			VertexLeftWorldPos[2] = m_pVerPos[nVertexIdxStart + (m_nXBlock + 1)];
			for (int nCntLeft = 0; nCntLeft < 3; nCntLeft++)
			{// �|���S���̒��_�����J��Ԃ�
			    // �O�ς̃x�N�g�������o��
				VectorVerToVer = VertexLeftWorldPos[(nCntLeft + 1) % 3] - VertexLeftWorldPos[nCntLeft];
				VectorVerToPos = pos - VertexLeftWorldPos[nCntLeft];

				if ((VectorVerToVer.x * VectorVerToPos.z) - (VectorVerToVer.z * VectorVerToPos.x) <= 0)
				{// �����̍��W�����_�ƒ��_�̃x�N�g���̉E���ɂ���
					nCntLeftPolygon++;  // �����ʂ����񐔂�i�߂�
				}
			}
			if (nCntLeftPolygon == 3)
			{// �O�ς̔����S�Ēʂ���
			    // ����p�̒��_���W�����
				PolygonVerPos[0] = D3DXVECTOR3(VertexLeftWorldPos[2].x, VertexLeftWorldPos[2].y - VertexLeftWorldPos[2].y, VertexLeftWorldPos[2].z);
				PolygonVerPos[1] = D3DXVECTOR3(VertexLeftWorldPos[1].x, VertexLeftWorldPos[1].y - VertexLeftWorldPos[2].y, VertexLeftWorldPos[1].z);
				PolygonVerPos[2] = D3DXVECTOR3(VertexLeftWorldPos[0].x, VertexLeftWorldPos[0].y - VertexLeftWorldPos[2].y, VertexLeftWorldPos[0].z);

				// �O�σx�N�g�������
				PolygonVector[0] = PolygonVerPos[1] - PolygonVerPos[0];
				PolygonVector[1] = PolygonVerPos[2] - PolygonVerPos[0];

				// �O�σx�N�g������@��������o��
				D3DXVec3Cross(&PolygonNormal, &PolygonVector[0], &PolygonVector[1]);
				D3DXVec3Normalize(&PolygonNormal, &PolygonNormal);

				// ����̎d����0�ɖ߂��������Ōv�Z���Ă���̂ŕ␳�����邽�߂ɒl��ێ�
				fRivision = VertexLeftWorldPos[2].y;

				// �����̍��W�ƒ��_���W�̃x�N�g�����v�Z
				VecPosToPolygon = pos - PolygonVerPos[0];

				// ���ς̊T�O�𗘗p���č���������o��
				float DotX = (PolygonNormal.x * VecPosToPolygon.x);     // X�����̓��ς��v�Z
				float DotZ = (PolygonNormal.z * VecPosToPolygon.z);     // Z�����̓��ς��v�Z
				float Dot = -DotX - DotZ;                               // X������Z�����̓��ϓ��m������
				if (PolygonNormal.y != 0.0f)
				{
					fHeight = (Dot / PolygonNormal.y) + fRivision;      // ���������̂�Y�����̖@���Ŋ����ĕ␳�l�𑫂��Ă�����
				}
				else
				{
					return 0.0f;
				}

				// ��������p�x�����߂�
				//float VecA = sqrtf((VecPosToPolygon.x * VecPosToPolygon.x) + (fHeight * fHeight) + (VecPosToPolygon.z * VecPosToPolygon.z));
				//float VecB = sqrtf((PolygonNormal.x * PolygonNormal.x) + (PolygonNormal.y * PolygonNormal.y) + (PolygonNormal.z * PolygonNormal.z));
				//float DotY = (PolygonNormal.y * fHeight);
				//float cosSita = (DotX + DotY + DotZ) / (VecA * VecB);
				//float fAngle = acosf(cosSita);

				if (pLand != NULL)
				{
					*pLand = true;
				}
				if (pPolyNormal != NULL)
				{
					// ����p�̒��_���W�����
					PolygonVerPos[0] = D3DXVECTOR3(VertexLeftWorldPos[2].x, VertexLeftWorldPos[2].y, VertexLeftWorldPos[2].z);
					PolygonVerPos[1] = D3DXVECTOR3(VertexLeftWorldPos[1].x, VertexLeftWorldPos[1].y, VertexLeftWorldPos[1].z);
					PolygonVerPos[2] = D3DXVECTOR3(VertexLeftWorldPos[0].x, VertexLeftWorldPos[0].y, VertexLeftWorldPos[0].z);

					// �O�σx�N�g�������
					PolygonVector[0] = PolygonVerPos[1] - PolygonVerPos[0];
					PolygonVector[1] = PolygonVerPos[2] - PolygonVerPos[0];

					// �O�σx�N�g������@��������o��
					D3DXVec3Cross(&PolygonNormal, &PolygonVector[1], &PolygonVector[0]);
					D3DXVec3Normalize(&PolygonNormal, &PolygonNormal);


					*pPolyNormal = PolygonNormal;
				}
				if (pVerPos != NULL)
				{
					*pVerPos = VertexLeftWorldPos[2];
				}
			}

			// �E���̃|���S���̔���J�n
			VertexRightWorldPos[0] = m_pVerPos[nVertexIdxStart];
			VertexRightWorldPos[1] = m_pVerPos[nVertexIdxStart + 1];
			VertexRightWorldPos[2] = m_pVerPos[nVertexIdxStart + (m_nXBlock + 1) + 1];
			for (int nCntRight = 0; nCntRight < 3; nCntRight++)
			{// �|���S���̒��_�����J��Ԃ�
			 // �O�ς̃x�N�g�������o��
				VectorVerToVer = VertexRightWorldPos[(nCntRight + 1) % 3] - VertexRightWorldPos[nCntRight];
				VectorVerToPos = pos - VertexRightWorldPos[nCntRight];

				if ((VectorVerToVer.x * VectorVerToPos.z) - (VectorVerToVer.z * VectorVerToPos.x) <= 0)
				{// �����̍��W�����_�ƒ��_�̃x�N�g���̉E���ɂ���
					nCntRightPolygon++;  // �����ʂ����񐔂�i�߂�
				}
			}
			if (nCntRightPolygon == 3)
			{// �O�ς̔����S�Ēʂ���
			    // ����p�̒��_���W�����
				PolygonVerPos[0] = D3DXVECTOR3(VertexRightWorldPos[1].x, VertexRightWorldPos[1].y - VertexRightWorldPos[1].y, VertexRightWorldPos[1].z);
				PolygonVerPos[1] = D3DXVECTOR3(VertexRightWorldPos[0].x, VertexRightWorldPos[0].y - VertexRightWorldPos[1].y, VertexRightWorldPos[0].z);
				PolygonVerPos[2] = D3DXVECTOR3(VertexRightWorldPos[2].x, VertexRightWorldPos[2].y - VertexRightWorldPos[1].y, VertexRightWorldPos[2].z);

				// �O�σx�N�g�������
				PolygonVector[0] = PolygonVerPos[1] - PolygonVerPos[0];
				PolygonVector[1] = PolygonVerPos[2] - PolygonVerPos[0];

				// �O�σx�N�g������@��������o��
				D3DXVec3Cross(&PolygonNormal, &PolygonVector[1], &PolygonVector[0]);
				D3DXVec3Normalize(&PolygonNormal, &PolygonNormal);

				// ����̎d����0�ɖ߂��������Ōv�Z���Ă���̂ŕ␳�����邽�߂ɒl��ێ�
				fRivision = VertexRightWorldPos[1].y;

				// �����̍��W�ƒ��_���W�̃x�N�g�����v�Z
				VecPosToPolygon = pos - PolygonVerPos[0];

				// ���ς̊T�O�𗘗p���č���������o��
				float DotX = (PolygonNormal.x * VecPosToPolygon.x);     // X�����̓��ς��v�Z
				float DotZ = (PolygonNormal.z * VecPosToPolygon.z);     // Z�����̓��ς��v�Z
				float Dot = -DotX - DotZ;                               // X������Z�����̓��ϓ��m������
				if (PolygonNormal.y != 0.0f)
				{
					fHeight = (Dot / PolygonNormal.y) + fRivision;          // ���������̂�Y�����̖@���Ŋ����ĕ␳�l�𑫂��Ă�����
				}
				else
				{
					return 0.0f;
				}

				// ��������p�x�����߂�
				//float VecA = sqrtf((VecPosToPolygon.x * VecPosToPolygon.x) + (fHeight * fHeight) + (VecPosToPolygon.z * VecPosToPolygon.z));
				//float VecB = sqrtf((PolygonNormal.x * PolygonNormal.x) + (PolygonNormal.y * PolygonNormal.y) + (PolygonNormal.z * PolygonNormal.z));
				//float DotY = (PolygonNormal.y * fHeight);
				//float cosSita = (DotX + DotY + DotZ) / (VecA * VecB);
				//float fAngle = acosf(cosSita);

				if (pLand != NULL)
				{
					*pLand = true;
				}
				if (pPolyNormal != NULL)
				{
					// ����p�̒��_���W�����
					PolygonVerPos[0] = D3DXVECTOR3(VertexRightWorldPos[1].x, VertexRightWorldPos[1].y, VertexRightWorldPos[1].z);
					PolygonVerPos[1] = D3DXVECTOR3(VertexRightWorldPos[0].x, VertexRightWorldPos[0].y, VertexRightWorldPos[0].z);
					PolygonVerPos[2] = D3DXVECTOR3(VertexRightWorldPos[2].x, VertexRightWorldPos[2].y, VertexRightWorldPos[2].z);

					// �O�σx�N�g�������
					PolygonVector[0] = PolygonVerPos[1] - PolygonVerPos[0];
					PolygonVector[1] = PolygonVerPos[2] - PolygonVerPos[0];

					// �O�σx�N�g������@��������o��
					D3DXVec3Cross(&PolygonNormal, &PolygonVector[0], &PolygonVector[1]);
					D3DXVec3Normalize(&PolygonNormal, &PolygonNormal);


					*pPolyNormal = PolygonNormal;
				}
				if (pVerPos != NULL)
				{
					*pVerPos = VertexRightWorldPos[1];
				}
			}


			// ����p�̕ϐ������������Ă���
			if (nCntLeftPolygon == 3 || nCntRightPolygon == 3)
			{// �|���S���ɏ���Ă���Ɣ��肳��Ă���
				break;
			}
			else
			{// �܂��|���S���ɏ���Ă��Ȃ��Ɣ��肳��Ă���
				nCntLeftPolygon = 0;
				nCntRightPolygon = 0;
				nVertexIdxStart++;
			}
		}
		if (nCntLeftPolygon == 3 || nCntRightPolygon == 3)
		{// �|���S���ɏ���Ă���Ɣ��肳��Ă���
			break;
		}
		else
		{// �܂��|���S���ɏ���Ă��Ȃ��Ɣ��肳��Ă���
		 // �E�[�̃u���b�N�ɔ��肷��ׂ��|���S���͑��݂��Ȃ��̂Ŕԍ������炵�Ă���
			nVertexIdxStart++;
		}
	}

	return fHeight;
}

//=============================================================================
//    ���b�V���Ɋђʂ��Ă��邩���肷�鏈��
//=============================================================================
bool CMeshCollider::CheckPenetration(D3DXVECTOR3 pos, int *pXBlock, int *pYBlock, int *pLeftVerIndex)
{
	bool bPene = false;

	return bPene;
}

//=============================================================================
//    ���_���W�ւ̃|�C���^��ݒ肷�鏈��
//=============================================================================
void CMeshCollider::SetVerPos(D3DXVECTOR3 *pVerPos)
{
	m_pVerPos = pVerPos;
}

//=============================================================================
//    1�u���b�N���̕���ݒ肷�鏈��
//=============================================================================
void CMeshCollider::SetWidth(const float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
//    1�u���b�N���̉��s��ݒ肷�鏈��
//=============================================================================
void CMeshCollider::SetDepth(const float fDepth)
{
	m_fDepth = fDepth;
}

//=============================================================================
//    ���̕�������ݒ肷�鏈��
//=============================================================================
void CMeshCollider::SetXBlock(const int nXBlock)
{
	m_nXBlock = nXBlock;
}

//=============================================================================
//    ���s�̕�������ݒ肷�鏈��
//=============================================================================
void CMeshCollider::SetZBlock(const int nZBlock)
{
	m_nZBlock = nZBlock;
}

//=============================================================================
//    �e���_�o�b�t�@�ւ̃|�C���^��ݒ肷�鏈��
//=============================================================================
void CMeshCollider::SetVtxBuffParent(const LPDIRECT3DVERTEXBUFFER9 pVtxBuffParent)
{
	m_pVtxBuffParent = pVtxBuffParent;
}

//=============================================================================
//    ���������ۂɖ߂����ǂ����ݒ肷�鏈��
//=============================================================================
void CMeshCollider::SetReturnFlag(const bool bReturnFlag)
{
	m_bReturnFlag = bReturnFlag;
}

//=============================================================================
//    ���_���W�ւ̃|�C���^���擾���鏈��
//=============================================================================
D3DXVECTOR3 *CMeshCollider::GetVerPos(void)
{
	return m_pVerPos;
}

//=============================================================================
//    1�u���b�N���̕����擾���鏈��
//=============================================================================
float CMeshCollider::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    1�u���b�N���̉��s���擾���鏈��
//=============================================================================
float CMeshCollider::GetDepth(void)
{
	return m_fDepth;
}

//=============================================================================
//    ���̕��������擾���鏈��
//=============================================================================
int CMeshCollider::GetXBlock(void)
{
	return m_nXBlock;
}

//=============================================================================
//    ���s�̕��������擾���鏈��
//=============================================================================
int CMeshCollider::GetZBlock(void)
{
	return m_nZBlock;
}

//=============================================================================
//    ���_�����擾���鏈��
//=============================================================================
int CMeshCollider::GetNumVertex(void)
{
	return m_nNumVertex;
}

//=============================================================================
//    �e���_�o�b�t�@�ւ̃|�C���^���擾���鏈��
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CMeshCollider::GetVtxBuffParent(void)
{
	return m_pVtxBuffParent;
}

//=============================================================================
//    ���������ۂɖ߂����ǂ����擾���鏈��
//=============================================================================
bool CMeshCollider::GetReturnFlag(void)
{
	return m_bReturnFlag;
}