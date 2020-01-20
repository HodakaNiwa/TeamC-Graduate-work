//*****************************************************************************
//
//     ���f�C�x���g�p�̕ǂ̏���[divisionWall.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "divisionWall.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define DIVISIONWALL_MAXHEIGHT (500.0f)

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CDivisionWall::CDivisionWall(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	m_pTexture = NULL;							// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;							// ���_�o�b�t�@�ւ̃|�C���^
	m_Pos = INITIALIZE_VECTOR3;					// �|���S���̈ʒu
	m_Rot = INITIALIZE_VECTOR3;					// �|���S���̌���
	D3DXMatrixIdentity(&m_MtxWorld);			// ���[���h�}�g���b�N�X
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �|���S���̐F
	m_fWidth = 0.0f;							// �|���S���̕�
	m_fHeight = 0.0f;							// �|���S���̍���
	m_fAngle = 0.0f;							// �|���S���̊p�x
	m_fLength = 0.0f;							// �|���S���̒���
	m_fTexU = 0.0f;								// ����e�N�X�`��U���W
	m_fTexV = 0.0f;								// ����e�N�X�`��V���W
	m_fTexWidth = 0.0f;							// �e�N�X�`�����W�̉���
	m_fTexHeight = 0.0f;						// �e�N�X�`�����W�̏c��
	m_fMoveHeight = 0.0f;						// �����̈ړ���
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CDivisionWall::~CDivisionWall()
{

}

//=============================================================================
//    ��������
//=============================================================================
CDivisionWall *CDivisionWall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth,
	float fHeight, float fMoveHeight, int nPriority, float fTexU, float fTexV, float fTexWidth, float fTexHeight)
{
	CDivisionWall *pScene3D = NULL;             // 3D�|���S���N���X�^�̃|�C���^
	if (pScene3D == NULL)
	{// ����������ɂȂ��Ă���
		pScene3D = new CDivisionWall(nPriority);
		if (pScene3D != NULL)
		{// �C���X�^���X�𐶐��ł���
		 // �e��l�̑��
			pScene3D->SetPos(pos);					// �|���S���̍��W
			pScene3D->SetRot(rot);					// �|���S���̌���
			pScene3D->SetCol(col);					// �|���S���̐F
			pScene3D->SetWidth(fWidth);				// �|���S���̕�
			pScene3D->SetHeight(fHeight);			// �|���S���̍���
			pScene3D->SetTexU(fTexU);				// �|���S���̍���e�N�X�`��U���W
			pScene3D->SetTexV(fTexV);				// �|���S���̍���e�N�X�`��V���W
			pScene3D->SetTexWidth(fTexWidth);		// �|���S���̃e�N�X�`�����W�̉���
			pScene3D->SetTexHeight(fTexHeight);		// �|���S���̃e�N�X�`�����W�̏c��
			pScene3D->SetMoveHeight(fMoveHeight);	// �����̈ړ���

			if (FAILED(pScene3D->Init()))
			{// �������Ɏ��s����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}

	return pScene3D;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CDivisionWall::Init(void)
{
	// �`��f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

	// ���_�o�b�t�@�̍쐬
	MakeVertex(pDevice);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CDivisionWall::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	DIRECT_RELEASE(m_pVtxBuff);

	// �����[�X����
	CScene::Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CDivisionWall::Update(void)
{
	// ���W�����炷
	m_Pos.y += m_fMoveHeight;
	if (m_Pos.y <= 0.0f - (m_fHeight * 0.5f))
	{
		m_Pos.y = -(m_fHeight * 0.5f);
	}
	else if (m_Pos.y >= DIVISIONWALL_MAXHEIGHT)
	{
		m_Pos.y = DIVISIONWALL_MAXHEIGHT;
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CDivisionWall::Draw(void)
{
	// �`��f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return; }

	// �g�����X�t�H�[�������v�Z���f�o�C�X�ɐݒ�
	CalcNotScaleTransform();
	SetTransformToDevice(pDevice);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �J�����O���Ȃ��ݒ��
	DWORD CullMode;
	pDevice->GetRenderState(D3DRS_CULLMODE, &CullMode);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// �J�����O�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_CULLMODE, CullMode);

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
//    �傫�����܂߂Ȃ��g�����X�t�H�[�������v�Z���鏈��
//=============================================================================
void CDivisionWall::CalcNotScaleTransform(void)
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
//    �g�����X�t�H�[�������f�o�C�X�ɐݒ肷�鏈��
//=============================================================================
void CDivisionWall::SetTransformToDevice(const LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    ���_�o�b�t�@��������
//=============================================================================
void CDivisionWall::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_�o�b�t�@�֒l��ݒ肷��
	SetVtxBuffValue();
}

//=============================================================================
//    �|���S���̑傫����ݒ肷�鏈��
//=============================================================================
void CDivisionWall::SetScale(float fWidth, float fHeight)
{
	// ��������ۑ�
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	// �I�t�Z�b�g�̒����Ɗp�x�����߂�
	m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));
	m_fLength *= 0.5f;
	m_fAngle = atan2f(m_fWidth, m_fHeight);
}

//=============================================================================
//    ���_�o�b�t�@�֒l��ݒ肷�鏈��
//=============================================================================
void CDivisionWall::SetVtxBuffValue(void)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �傫����ݒ�
	SetScale(m_fWidth, m_fHeight);

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength, 0.0f);

	// �@���v�Z�p�x�N�g��
	D3DXVECTOR3 vecVer[4];

	// ���_�̖@��
	D3DXVECTOR3 nor1, nor2;

	// �O�ϗp�Ƀx�N�g�������
	vecVer[0] = pVtx[0].pos - pVtx[2].pos;
	vecVer[1] = pVtx[1].pos - pVtx[2].pos;
	vecVer[2] = pVtx[2].pos - pVtx[1].pos;
	vecVer[3] = pVtx[3].pos - pVtx[1].pos;

	// �x�N�g�����O�ςŌv�Z���Ė@�����o��
	D3DXVec3Cross(&nor1, &vecVer[0], &vecVer[1]);
	D3DXVec3Cross(&nor2, &vecVer[3], &vecVer[2]);
	D3DXVec3Normalize(&nor1, &nor1);
	D3DXVec3Normalize(&nor2, &nor2);

	// �@������
	pVtx[0].nor = nor1;
	pVtx[1].nor = (nor1 + nor2) / 2;
	pVtx[2].nor = (nor1 + nor2) / 2;
	pVtx[3].nor = nor2;

	// ���_�J���[
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(m_fTexU, m_fTexV);
	pVtx[1].tex = D3DXVECTOR2(m_fTexU + m_fTexWidth, m_fTexV);
	pVtx[2].tex = D3DXVECTOR2(m_fTexU, m_fTexV + m_fTexHeight);
	pVtx[3].tex = D3DXVECTOR2(m_fTexU + m_fTexWidth, m_fTexV + m_fTexHeight);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    ���_�o�b�t�@�֒��_���W��ݒ肷�鏈��
//=============================================================================
void CDivisionWall::SetVtxBuffPos(void)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �傫����ݒ�
	SetScale(m_fWidth, m_fHeight);

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    ���_�o�b�t�@�֒��_�J���[��ݒ肷�鏈��
//=============================================================================
void CDivisionWall::SetVtxBuffCol(void)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    ���_�o�b�t�@�փe�N�X�`�����W��ݒ肷��
//=============================================================================
void CDivisionWall::SetVtxBuffTex(void)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(m_fTexU, m_fTexV);
	pVtx[1].tex = D3DXVECTOR2(m_fTexU + m_fTexWidth, m_fTexV);
	pVtx[2].tex = D3DXVECTOR2(m_fTexU, m_fTexV + m_fTexHeight);
	pVtx[3].tex = D3DXVECTOR2(m_fTexU + m_fTexWidth, m_fTexV + m_fTexHeight);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}
