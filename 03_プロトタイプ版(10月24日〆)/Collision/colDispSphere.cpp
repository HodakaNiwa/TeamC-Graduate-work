//*****************************************************************************
//
//     �����蔻������p���b�V���X�t�B�A�̏���[colDispSphere.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "collision.h"
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
CColDispSphere::CColDispSphere(int nPriority, OBJTYPE objType) : CMeshSphere(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CColDispSphere::~CColDispSphere()
{

}

//=============================================================================
//    ��������
//=============================================================================
CColDispSphere *CColDispSphere::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, int nPriority)
{
	// �������m��
	CColDispSphere *pColDispSphere = NULL;
	pColDispSphere = new CColDispSphere(nPriority);
	if (pColDispSphere == NULL)
	{
		return NULL;
	}

	// ����������
	if (FAILED(pColDispSphere->Init(pos, col, fRadius, nXBlock, nYBlock)))
	{
		pColDispSphere->Uninit();
		return NULL;
	}

	return pColDispSphere;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CColDispSphere::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock)
{
	// �ϐ��̃N���A
	ClearVariable();

	// �e��l�̐ݒ�
	GetObject3D()->SetPos(pos);                 // ���̍��W
	GetObject3D()->SetRot(INITIALIZE_VECTOR3);  // ���̌���
	SetCol(col);                                // ���̐F
	SetRadius(fRadius);                         // ���̔��a
	SetXBlock(nXBlock);                         // ���̕�����
	SetYBlock(nYBlock);                         // �c�̕�����

	// ���ʂ̏���������
	if (FAILED(CMeshSphere::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CColDispSphere::Uninit(void)
{
	CMeshSphere::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CColDispSphere::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CColDispSphere::Draw(void)
{

}

//=============================================================================
//    ��������
//=============================================================================
void CColDispSphere::Disp(void)
{
	if (CCollider::GetDrawAll() == false) { return; }

	// �`��f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetDevice();
	if (pDevice == NULL) { return; }

	// ���C�e�B���O�𖳌���
	DWORD Lighting;
	pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �\�ʃJ�����O�ɐ؂�ւ�
	DWORD Culling;
	pDevice->GetRenderState(D3DRS_CULLMODE, &Culling);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	// ���ʂ̕`�揈��
	CMeshSphere::Draw();

	// �J�����O�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_CULLMODE, Culling);

	// ���C�e�B���O�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
//    ���_�o�b�t�@�ɒl��ݒ肷��
//=============================================================================
void CColDispSphere::SetVtxBuffValue(void)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff == NULL) { return; }
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fAngle = 0;                   // �p�x
	float fRadius = 0;                  // ���a
	float fSphere = 0.0f;               // �X�t�B�A�v�Z�p(���a���k�߂�{��)
	D3DXVECTOR3 pos;                    // ���W
	D3DXCOLOR col = GetCol();           // �F
	int nXBlock = GetXBlock();          // ���̕�����
	int nYBlock = GetYBlock();          // ���̕�����
	float fSphereRadius = GetRadius();  // ���̔��a
	float Utex;                         // �e�N�X�`��U���W
	float Vtex;                         // �e�N�X�`��V���W

	fAngle = 0.0f;                            // �p�x��0��
	fSphere = 0.0f;                           // �{����ݒ�
	fRadius = cosf(fSphere) * fSphereRadius;  // ���a��ݒ�
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // ���W��ݒ�
	Utex = 0.0f;                              // �e�N�X�`��U���W��ݒ�
	Vtex = 0.01f;                             // �e�N�X�`��V���W��ݒ�

	// �㑤�̊W�����̒l��ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, fSphereRadius, 0.0f);
	pVtx[0].col = col;
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.005f);
	pVtx++;
	fSphere -= D3DX_PI / (nYBlock + 2);      // ���a���k�߂�{�����グ��
	fRadius = sinf(fSphere) * fSphereRadius; // ���a���T�C���J�[�u�ōL���Ă���

	for (int nCntV = 0; nCntV < nYBlock + 1; nCntV++)
	{// ���������̕����������J��Ԃ�
		for (int nCntH = 0; nCntH < nXBlock + 1; nCntH++)
		{// ���������̕����������J��Ԃ�
			pos.x = sinf(fAngle) * -fRadius;
			pos.y = cosf(fSphere) * fSphereRadius;
			pos.z = cosf(fAngle) * -fRadius;

			// ���_���W
			pVtx[nCntH].pos = pos;

			// ���_�J���[
			pVtx[nCntH].col = col;

			// �e�N�X�`�����W
			pVtx[nCntH].tex = D3DXVECTOR2(Utex, Vtex);
			if (nCntV >= ((nYBlock + 1) / 2))
			{
				pVtx[nCntH].tex = D3DXVECTOR2(Utex, 1.005f);
			}

			fAngle += D3DX_PI / (nXBlock / 2);  // �p�x��i�߂�
			Utex += 1.0f / (nXBlock / 2);       // �e�N�X�`��U���W��i�߂�

			if (fAngle >= D3DX_PI)
			{// �{�����~�����𒴂���
				fAngle -= D3DX_PI * 2.0f;
			}
		}
		fSphere -= D3DX_PI / (nYBlock + 2);         // ���a���k�߂�{�����グ��
		fRadius = sinf(fSphere) * fSphereRadius;    // ���a���T�C���J�[�u�ōL���Ă���
		fAngle = 0.0f;                              // �p�x��0��
		Utex = 0.0f;                                // �e�N�X�`��U���W��ݒ�
		Vtex += 2.0f / (nYBlock + 2);               // �e�N�X�`��V���W��i�߂�
		pVtx += nXBlock + 1;                        // ���_�̐擪�̔ԍ���i�߂�
	}

	// �����̊W�����̒l��ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, -fSphereRadius, 0.0f);
	pVtx[0].col = col;
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.01f);

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

	// ���_�o�b�t�@�ɖ@����ݒ肷��
	SetVtxBuffNor();
}