//*****************************************************************************
//
//     �G�t�F�N�g(3D)�̏���[effect3D.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "effect3D.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define EFFECT3D_TEX_FILENAME "data/TEXTURE/Effect/effect000.jpg"

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CEffect3D::m_pTexture = NULL;// �e�N�X�`���ւ̃|�C���^

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CEffect3D::CEffect3D(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CEffect3D::~CEffect3D()
{

}

//=============================================================================
//    ��������
//=============================================================================
CEffect3D *CEffect3D::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nLife, float fCutAlpha, float fCutLength, bool bLighting, int nPriority, float fRot, float fTexU, float fTexV, float fTexWidth, float fTexHeight)
{
	// �������̊m��
	CEffect3D *pEffect3D = NULL;
	pEffect3D = new CEffect3D(nPriority);
	if (pEffect3D == NULL) { return NULL; }

	// �ϐ��̏�����
	pEffect3D->ClearVariable();

	// �e��l�̑��
	pEffect3D->SetPos(pos);					// �|���S���̍��W
	pEffect3D->SetCol(col);					// �|���S���̐F
	pEffect3D->SetRot(fRot);				// �|���S���̌���
	pEffect3D->SetWidth(fWidth);			// �|���S���̕�
	pEffect3D->SetHeight(fHeight);			// �|���S���̍���
	pEffect3D->SetTexU(fTexU);				// �|���S���̍���e�N�X�`��U���W
	pEffect3D->SetTexV(fTexV);				// �|���S���̍���e�N�X�`��V���W
	pEffect3D->SetTexWidth(fTexWidth);		// �|���S���̃e�N�X�`�����W�̉���
	pEffect3D->SetTexHeight(fTexHeight);	// �|���S���̃e�N�X�`�����W�̏c��
	pEffect3D->SetLighting(bLighting);		// ���C�e�B���O���邩���Ȃ���
	pEffect3D->SetLife(nLife);				// ����
	pEffect3D->SetCutAlpha(fCutAlpha);		// �����x������
	pEffect3D->SetCutLength(fCutLength);	// �傫��������

	// ����������
	if (FAILED(pEffect3D->Init()))
	{
		pEffect3D->Uninit();
		return NULL;
	}

	return pEffect3D;
}

//=============================================================================
//    �`�揈��
//=============================================================================
HRESULT CEffect3D::Load(void)
{
	if (m_pTexture != NULL) { return E_FAIL; }
	// �e�N�X�`���̓ǂݍ���
	if (FAILED(D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), EFFECT3D_TEX_FILENAME, &m_pTexture)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CEffect3D::UnLoad(void)
{
	DIRECT_RELEASE(m_pTexture);
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CEffect3D::Init(void)
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
void CEffect3D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	DIRECT_RELEASE(m_pVtxBuff);

	// �����[�X����
	CScene::Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CEffect3D::Update(void)
{
	// �����x�����
	m_Col.a -= m_fCutAlpha;

	// �傫�������
	m_fLength -= m_fCutLength;

	// ���_�o�b�t�@�ɒl�ݒ�
	SetValue();

	// ���������
	m_nLife--;

	// ���S����
	if (m_nLife <= 0 || m_Col.a <= 0.0f || m_fLength <= 0.0f)
	{
		Uninit();
	}
}

//=============================================================================
//    ���_�o�b�t�@�ɒl��ݒ肷�鏈��
//=============================================================================
void CEffect3D::SetValue(void)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength, 0.0f);

	// ���_�J���[
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CEffect3D::Draw(void)
{
	// �`��f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return; }

	// �g�����X�t�H�[�������v�Z���f�o�C�X�ɐݒ�
	CalcNotScaleTransform(pDevice);
	SetTransformToDevice(pDevice);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// ���C�e�B���O�̐ݒ�
	DWORD Lighting;
	pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
	if (m_bLighting == false)
	{// ���C�e�B���O���O��
		pDevice->SetRenderState(D3DRS_LIGHTING, false);
	}

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// Z�o�b�t�@�ɏ������܂Ȃ��ݒ��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// Z�o�b�t�@�ɏ������ސݒ��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ���C�e�B���O��߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);
}

//=============================================================================
//    �g�����X�t�H�[�����(�傫�����܂߂Ȃ�)���v�Z����
//=============================================================================
void CEffect3D::CalcNotScaleTransform(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxView, mtxTrans; // �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_MtxWorld);

	// �J�����̌������擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// ���[���h�}�g���b�N�X�Ɍ�����ݒ�(�J�����̋t�s������邱�ƂŃJ�����̐��ʂɌ�����␳)
	m_MtxWorld._11 = mtxView._11;
	m_MtxWorld._12 = mtxView._21;
	m_MtxWorld._13 = mtxView._31;
	m_MtxWorld._21 = mtxView._12;
	m_MtxWorld._22 = mtxView._22;
	m_MtxWorld._23 = mtxView._32;
	m_MtxWorld._31 = mtxView._13;
	m_MtxWorld._32 = mtxView._23;
	m_MtxWorld._33 = mtxView._33;

	// �ʒu�𔽉f
	m_MtxWorld._41 = m_Pos.x;
	m_MtxWorld._42 = m_Pos.y;
	m_MtxWorld._43 = m_Pos.z;
}

//=============================================================================
//    �g�����X�t�H�[�������f�o�C�X�ɐݒ肷��
//=============================================================================
void CEffect3D::SetTransformToDevice(const LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    ���_�o�b�t�@��������
//=============================================================================
void CEffect3D::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
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
void CEffect3D::SetScale(float fWidth, float fHeight)
{
	// �I�t�Z�b�g�̒����Ɗp�x�����߂�
	m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));
	m_fAngle = atan2f(m_fWidth, m_fHeight);
}

//=============================================================================
//    ���_�o�b�t�@�֒l��ݒ肷�鏈��
//=============================================================================
void CEffect3D::SetVtxBuffValue(void)
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
void CEffect3D::SetVtxBuffPos(void)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �傫����ݒ�
	SetScale(m_fWidth, m_fHeight);

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, 0.0f, cosf(m_fAngle) * m_fLength);
	pVtx[1].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, 0.0f, cosf(m_fAngle) * m_fLength);
	pVtx[2].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, 0.0f, -cosf(m_fAngle) * m_fLength);
	pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, 0.0f, -cosf(m_fAngle) * m_fLength);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    ���_�o�b�t�@�֖@���x�N�g����ݒ肷�鏈��
//=============================================================================
void CEffect3D::SetVtxBuffNor(void)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    ���_�o�b�t�@�֒��_�J���[��ݒ肷�鏈��
//=============================================================================
void CEffect3D::SetVtxBuffCol(void)
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
void CEffect3D::SetVtxBuffTex(void)
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

//=============================================================================
//    �ϐ��̃N���A
//=============================================================================
void CEffect3D::ClearVariable(void)
{
	// �e��l�̃N���A
	m_pVtxBuff = NULL;							// ���_�o�b�t�@�ւ̃|�C���^
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �|���S���̍��W
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �|���S���̐F
	m_fRot = 0.0f;								// �|���S���̌���
	m_fWidth = 0.0f;							// �|���S���̕�
	m_fHeight = 0.0f;							// �|���S���̍���
	m_fAngle = 0.0f;							// �|���S�����o���p�x
	m_fLength = 0.0f;							// �|���S���̒���
	m_fTexU = 0.0f;								// �|���S���̍���e�N�X�`��U���W
	m_fTexV = 0.0f;								// �|���S���̍���e�N�X�`��V���W
	m_fTexWidth = 0.0f;							// �|���S���̃e�N�X�`�����W�̉���
	m_fTexHeight = 0.0f;						// �|���S���̃e�N�X�`�����W�̏c��
	D3DXMatrixIdentity(&m_MtxWorld);			// ���[���h�}�g���b�N�X
	m_bLighting = false;						// ���C�e�B���O���邩���Ȃ���
	m_nLife = 0;								// ����
	m_fCutAlpha = 0.0f;							// �����x������
}