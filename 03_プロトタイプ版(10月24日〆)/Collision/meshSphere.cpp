//*****************************************************************************
//
//     ���b�V���X�t�B�A�̏���[meshSphere.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "3DMesh.h"
#include "manager.h"
#include "library.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************


//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CMeshSphere::CMeshSphere(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CMeshSphere::~CMeshSphere()
{

}

//=============================================================================
//    ��������
//=============================================================================
CMeshSphere *CMeshSphere::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, int nPriority)
{
	// �������m��
	CMeshSphere *pMeshSphere = NULL;
	pMeshSphere = new CMeshSphere(nPriority);
	if (pMeshSphere == NULL)
	{
		return NULL;
	}

	// �e��l�̑��
	pMeshSphere->SetPos(pos);         // ���b�V���X�t�B�A�̍��W
	pMeshSphere->SetRot(rot);         // ���b�V���X�t�B�A�̌���
	pMeshSphere->SetCol(col);         // ���b�V���X�t�B�A�̐F
	pMeshSphere->SetRadius(fRadius);  // ���b�V���X�t�B�A�̔��a
	pMeshSphere->SetXBlock(nXBlock);  // ���̕�����
	pMeshSphere->SetYBlock(nYBlock);  // �c�̕�����

	// ����������
	if (FAILED(pMeshSphere->Init()))
	{
		pMeshSphere->Uninit();
		return NULL;
	}

	return pMeshSphere;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CMeshSphere::Init(void)
{
	// �`��f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

	// �쐬�ɕK�v�Ȓ��_��,�C���f�b�N�X��,�|���S�������v�Z
	m_nNumVertex = (m_nXBlock + 1) * (m_nYBlock + 1);
	m_nNumIndex = ((m_nXBlock + 1) * (m_nYBlock + 1)) + (2 * (m_nYBlock - 1)) + (m_nXBlock + 1) * (m_nYBlock - 1);
	m_nNumPolygon = ((m_nXBlock * m_nYBlock) * 2) + ((m_nYBlock - 1) * 4);

	// �㑤�Ɖ����̊W�������k�ރ|���S���̕����l���Ă��ꂼ����Z
	m_nNumVertex += 2;
	m_nNumIndex += ((m_nXBlock + 1) * 2 + 4) * 2;
	m_nNumPolygon += (((m_nXBlock * 2) + 4)) * 2;
	if (m_nXBlock % 2 == 0)
	{
		m_nNumPolygon += 4;
	}

	// ���_���̍쐬
	MakeVertex(pDevice);

	// �C���f�b�N�X���̍쐬
	MakeIndex(pDevice);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CMeshSphere::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̔j��
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// ���S�t���O�𗧂Ă�
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CMeshSphere::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CMeshSphere::Draw(void)
{
	// �`��f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetDevice();
	if (pDevice == NULL) { return; }

	// �g�����X�t�H�[�������v�Z���f�o�C�X�ɐݒ�
	CalcNotScaleTransform();
	SetTransformToDevice(pDevice);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);
}

//=============================================================================
//    �g�����X�t�H�[�����(�傫�����܂߂Ȃ�)���v�Z����
//=============================================================================
void CMeshSphere::CalcNotScaleTransform(void)
{
	D3DXMATRIX mtxRot; // �v�Z�p�}�g���b�N�X

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
}

//=============================================================================
//    �g�����X�t�H�[�������f�o�C�X�ɐݒ肷��
//=============================================================================
void CMeshSphere::SetTransformToDevice(const LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    ���_�o�b�t�@��������
//=============================================================================
void CMeshSphere::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_�o�b�t�@�ɒl��ݒ肷��
	SetVtxBuffValue();
}

//=============================================================================
//    �C���f�b�N�X�o�b�t�@��������
//=============================================================================
void CMeshSphere::MakeIndex(const LPDIRECT3DDEVICE9 pDevice)
{
	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	WORD *pIdx;       // �C���f�b�N�X�f�[�^�ւ̃|�C���^
	int nCntIdx = 1;  // �C���f�b�N�X�ԍ�

	// �C���f�b�N�X�o�b�t�@�����b�N��,�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// �k�ރ|���S�����l�������㑤�̊W�����̃C���f�b�N�X����ݒ�
	for (int nCntIdxX = 0; nCntIdxX < m_nXBlock + 1; nCntIdxX++, nCntIdx++)
	{// ���̕�����+�P�J��Ԃ�
	    // �㉺�̓���C���f�b�N�X�ԍ���ݒ�
		pIdx[0] = nCntIdx;     // ����
		pIdx[1] = 0;           // �㑤

		pIdx += 2;  // 2���i�߂�
		if (nCntIdxX == m_nXBlock)
		{// ���̕��������ݒ肪�I�����
			pIdx[0] = 0; // �㑤
			pIdx[1] = 1; // ���̉���

			pIdx += 2; // 2���i�߂�
		}
	}
	// �k�ރ|���S���ł���1���B�؂�
	pIdx[0] = 1;             // �㑤
	pIdx[1] = m_nXBlock + 2; // ���̉���
	pIdx += 2; // 2���i�߂�

	nCntIdx = 1;
	for (int nCntIdxY = 0; nCntIdxY < m_nYBlock; nCntIdxY++)
	{// �c�̕��������J��Ԃ�
		for (int nCntIdxX = 0; nCntIdxX < m_nXBlock + 1; nCntIdxX++, nCntIdx++)
		{// ���̕�����+�P�J��Ԃ�
		 // �㉺�̓���C���f�b�N�X�ԍ���ݒ�
			pIdx[0] = nCntIdx + m_nXBlock + 1; // ����
			pIdx[1] = nCntIdx;                 // �㑤

			pIdx += 2;  // 2���i�߂�
			if (nCntIdxY < m_nYBlock && nCntIdxX == m_nXBlock)
			{// 1 , �������c�̕������̍ŉ��w��菬����
			 // 2 , ���̕��������ݒ肪�I�����
				pIdx[0] = nCntIdx;                 // �㑤
				pIdx[1] = nCntIdx + m_nXBlock + 2; // ���̉���

				pIdx += 2; // 2���i�߂�
			}
		}
	}

	// �����̊W�����̃C���f�b�N�X����ݒ�
	for (int nCntIdxX = 0; nCntIdxX < m_nXBlock + 1; nCntIdxX++, nCntIdx++)
	{// ���̕�����+�P�J��Ԃ�
	    // �㉺�̓���C���f�b�N�X�ԍ���ݒ�
		pIdx[0] = m_nNumVertex - 1; // ����
		pIdx[1] = nCntIdx;          // �㑤

		pIdx += 2;  // 2���i�߂�
		if (nCntIdxX == m_nXBlock)
		{// ���̕��������ݒ肪�I�����
			pIdx[1] = m_nNumVertex - 1 - m_nXBlock - 1;    // �㑤
			pIdx[0] = m_nNumVertex - 1;                    // ���̉���
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();
}

//=============================================================================
//    ���_�o�b�t�@�ɒl��ݒ肷��
//=============================================================================
void CMeshSphere::SetVtxBuffValue(void)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fAngle = 0;     // �p�x
	float fRadius = 0;    // ���a
	float fSphere = 0.0f; // �X�t�B�A�v�Z�p(���a���k�߂�{��)
	D3DXVECTOR3 pos;      // ���W
	float Utex;           // �e�N�X�`��U���W
	float Vtex;           // �e�N�X�`��V���W

	fAngle = 0.0f;                         // �p�x��0��
	fSphere = 0.0f;                        // �{����ݒ�
	fRadius = cosf(fSphere) * m_fRadius;   // ���a��ݒ�
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // ���W��ݒ�
	Utex = 0.0f;                           // �e�N�X�`��U���W��ݒ�
	Vtex = 0.01f;                          // �e�N�X�`��V���W��ݒ�

	// �㑤�̊W�����̒l��ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, m_fRadius, 0.0f);
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.005f);
	pVtx++;
	fSphere -= D3DX_PI / (m_nYBlock + 2);    // ���a���k�߂�{�����グ��
	fRadius = sinf(fSphere) * m_fRadius;     // ���a���T�C���J�[�u�ōL���Ă���

	for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
	{// ���������̕����������J��Ԃ�
		for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
		{// ���������̕����������J��Ԃ�
			pos.x = sinf(fAngle) * fRadius;
			pos.y = cosf(fSphere) * m_fRadius;
			pos.z = cosf(fAngle) * fRadius;

			// ���_���W
			pVtx[nCntH].pos = pos;

			// ���_�J���[
			pVtx[nCntH].col = m_Col;

			// �e�N�X�`�����W
			pVtx[nCntH].tex = D3DXVECTOR2(Utex, Vtex);
			if (nCntV >= ((m_nYBlock + 1) / 2))
			{
				pVtx[nCntH].tex = D3DXVECTOR2(Utex, 1.005f);
			}

			fAngle += D3DX_PI / (m_nXBlock / 2);  // �p�x��i�߂�
			Utex += 1.0f / (m_nXBlock / 2);       // �e�N�X�`��U���W��i�߂�

			if (fAngle >= D3DX_PI)
			{// �{�����~�����𒴂���
				fAngle -= D3DX_PI * 2.0f;
			}
		}
		fSphere -= D3DX_PI / (m_nYBlock + 2);    // ���a���k�߂�{�����グ��
		fRadius = sinf(fSphere) * m_fRadius;     // ���a���T�C���J�[�u�ōL���Ă���
		fAngle = 0.0f;                           // �p�x��0��
		Utex = 0.0f;                             // �e�N�X�`��U���W��ݒ�
		Vtex += 2.0f / (m_nYBlock + 2);          // �e�N�X�`��V���W��i�߂�
		pVtx += m_nXBlock + 1;                   // ���_�̐擪�̔ԍ���i�߂�
	}

	// �����̊W�����̒l��ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, -m_fRadius, 0.0f);
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.01f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// ���_�o�b�t�@�ɖ@����ݒ肷��
	SetVtxBuffNor();
}

//=============================================================================
//    ���_�o�b�t�@�ɒ��_���W��ݒ肷��
//=============================================================================
void CMeshSphere::SetVtxBuffPos(void)
{

}

//=============================================================================
//    ���_�o�b�t�@�ɖ@����ݒ肷��
//=============================================================================
void CMeshSphere::SetVtxBuffNor(void)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 Pos = GetPos();
	D3DXVECTOR3 VerPos;
	D3DXVECTOR3 VerNormal;
	for (int nCnt = 0; nCnt < m_nNumVertex; nCnt++, pVtx++)
	{// ���_�̐������J��Ԃ�
	    // ���_���W���擾
		VerPos = pVtx[0].pos;

		// �@���x�N�g����ݒ�
		CFunctionLib::Vec3Direction(&VerNormal, &Pos, &VerPos);
		D3DXVec3Normalize(&VerNormal, &VerNormal);
		pVtx[0].nor = VerNormal;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    ���_�o�b�t�@�ɒ��_�J���[��ݒ肷��
//=============================================================================
void CMeshSphere::SetVtxBuffCol(void)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
	{// ���������̕����� + 1�����J��Ԃ�
		for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
		{// ���������̕����� + 1�����J��Ԃ�
		 // ���_�J���[
			pVtx[nCntH].col = m_Col;
		}
		// �|�C���^��i�߂�
		pVtx += m_nXBlock + 1;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    ���_�o�b�t�@�Ƀe�N�X�`�����W��ݒ肷��
//=============================================================================
void CMeshSphere::SetVtxBuffTex(void)
{

}

//=============================================================================
//    �ϐ����N���A����
//=============================================================================
void CMeshSphere::ClearVariable(void)
{
	// �e��l�̃N���A
	m_pVtxBuff = NULL;                      // ���_�o�b�t�@�ւ̃|�C���^
	m_pIdxBuff = NULL;                      // �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	m_pTexture = NULL;	                    // �e�N�X�`���ւ̃|�C���^
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ���b�V���X�t�B�A�̍��W
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ���b�V���X�t�B�A�̌���
	D3DXMatrixIdentity(&m_MtxWorld);        // ���b�V���X�t�B�A�̃��[���h�}�g���b�N�X
	m_Col = INITIALIZE_VECTOR4;             // ���b�V���X�t�B�A�̐F
	m_fRadius = 0.0f;                       // ���b�V���X�t�B�A�̔��a
	m_nXBlock = 0;                          // ���̕�����
	m_nYBlock = 0;                          // ���s�̕�����
	m_nNumVertex = 0;                       // ���_��
	m_nNumIndex = 0;                        // �C���f�b�N�X��
	m_nNumPolygon = 0;                      // �|���S����
}

//=============================================================================
//    �e�N�X�`���ݒ菈��
//=============================================================================
void CMeshSphere::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    ���_�o�b�t�@�ݒ菈��
//=============================================================================
void CMeshSphere::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
//    �C���f�b�N�X�o�b�t�@�ݒ菈��
//=============================================================================
void CMeshSphere::SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff)
{
	m_pIdxBuff = pIdxBuff;
}

//=============================================================================
//    ���b�V���X�t�B�A�̍��W�ݒ菈��
//=============================================================================
void CMeshSphere::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    ���b�V���X�t�B�A�̌����ݒ菈��
//=============================================================================
void CMeshSphere::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    ���b�V���X�t�B�A�̃��[���h�}�g���b�N�X�ݒ菈��
//=============================================================================
void CMeshSphere::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    ���b�V���X�t�B�A�̐F�ݒ菈��
//=============================================================================
void CMeshSphere::SetCol(const D3DXCOLOR col)
{
	m_Col = col;
}

//=============================================================================
//    ���b�V���X�t�B�A�̔��a�ݒ菈��
//=============================================================================
void CMeshSphere::SetRadius(const float fRadius)
{
	m_fRadius = fRadius;
}

//=============================================================================
//    ���̕������ݒ菈��
//=============================================================================
void CMeshSphere::SetXBlock(const int nXBlock)
{
	m_nXBlock = nXBlock;
}

//=============================================================================
//    ���s�̕������ݒ菈��
//=============================================================================
void CMeshSphere::SetYBlock(const int nYBlock)
{
	m_nYBlock = nYBlock;
}

//=============================================================================
//    ���_�o�b�t�@�擾����
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CMeshSphere::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
//    �C���f�b�N�X�o�b�t�@�擾����
//=============================================================================
LPDIRECT3DINDEXBUFFER9 CMeshSphere::GetIdxBuff(void)
{
	return m_pIdxBuff;
}

//=============================================================================
//    �e�N�X�`���擾����
//=============================================================================
LPDIRECT3DTEXTURE9 CMeshSphere::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    ���b�V���X�t�B�A�̍��W�擾����
//=============================================================================
D3DXVECTOR3 CMeshSphere::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    ���b�V���X�t�B�A�̌����擾����
//=============================================================================
D3DXVECTOR3 CMeshSphere::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    ���b�V���X�t�B�A�̃��[���h�}�g���b�N�X�擾����
//=============================================================================
D3DXMATRIX CMeshSphere::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    ���b�V���X�t�B�A�̐F�擾����
//=============================================================================
D3DXCOLOR CMeshSphere::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    ���b�V���X�t�B�A�̔��a�擾����
//=============================================================================
float CMeshSphere::GetRadius(void)
{
	return m_fRadius;
}

//=============================================================================
//    ���̕������擾����
//=============================================================================
int CMeshSphere::GetXBlock(void)
{
	return m_nXBlock;
}

//=============================================================================
//    ���s�̕������擾����
//=============================================================================
int CMeshSphere::GetYBlock(void)
{
	return m_nYBlock;
}

//=============================================================================
//    ���_���擾����
//=============================================================================
int CMeshSphere::GetNumVertex(void)
{
	return m_nNumVertex;
}

//=============================================================================
//    �C���f�b�N�X���擾����
//=============================================================================
int CMeshSphere::GetNumIndex(void)
{
	return m_nNumIndex;
}

//=============================================================================
//    �|���S�����擾����
//=============================================================================
int CMeshSphere::GetNumPolygon(void)
{
	return m_nNumPolygon;
}