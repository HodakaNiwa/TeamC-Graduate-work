//=============================================================================
//
// �O�p�`�|���S������ [sceneTriangle3D.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#include "sceneTriangle3D.h"
#include "manager.h"
#include "renderer.h"
#include "Player.h"
#include "character.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEXTURE_NAME00	("data//TEXTURE//grand.jpg")
#define MAX_POS			(4)

//=============================================================================
// ��������
//=============================================================================
CSceneTriangle3D * CSceneTriangle3D::Create(D3DXVECTOR3 Pos1, D3DXVECTOR3 Pos2, D3DXVECTOR3 Pos3)
{
	//�C���X�^���X�̐���
	CSceneTriangle3D * pScene3D;
	pScene3D = new CSceneTriangle3D;

	//����������
	pScene3D->Init();
	pScene3D->SetPos(Pos1, Pos2, Pos3);
	return pScene3D;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSceneTriangle3D::CSceneTriangle3D() : CScene(4, OBJTYPE_FLOR)
{
	m_pVtxBuff = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSceneTriangle3D::~CSceneTriangle3D(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSceneTriangle3D::Init(void)
{
	//�ϐ��錾
	m_pVtxBuff = NULL;
	m_nLife = LINE_TIME;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 3,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D * pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(200.0f, 0.0f, -100.0f);
	pVtx[2].pos = D3DXVECTOR3(-200.0f, 0.0f, -100.0f);

	pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//UV�l�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CSceneTriangle3D::Uninit(void)
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//���S�t���O�𗧂Ă�
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CSceneTriangle3D::Update(void)
{
	//�ϐ��錾
	bool bDeth = false;	//���S�t���O

	//�\�����Ԃ����炷
	m_nLife--;

	if (m_nLife < 0)
	{//�\�����Ԃ�0�ȉ��ɂȂ�����
		m_col.a -= 0.05f;

		if (m_col.a < 0.0f) 
		{
			m_col.a = 0.0f;
			bDeth = true;	//���S�t���O��L���ɂ���
		}

		//�F�̐ݒ�
		SetColor(m_col);
	}

	//�j������
	if (bDeth) { Uninit(); }
}

//=============================================================================
// �`�揈��
//=============================================================================
void CSceneTriangle3D::Draw(void)
{
	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX		  mtxRot, mtxTrans;			// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 5.0f, 0.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (m_pVtxBuff != NULL)
	{
		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			1);
	}

	//�f�t�H���g�ɖ߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//=============================================================================
// �F�̐ݒ�
//=============================================================================
void CSceneTriangle3D::SetColor(D3DXCOLOR col)
{
	//�F�̕ۑ�
	m_col = col;

	VERTEX_3D * pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �ʒu�ݒ�
//=============================================================================
void CSceneTriangle3D::SetPos(D3DXVECTOR3 Pos1, D3DXVECTOR3 Pos2, D3DXVECTOR3 Pos3)
{
	VERTEX_3D * pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = Pos1;
	pVtx[1].pos = Pos2;
	pVtx[2].pos = Pos3;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �ݒ菈��
//=============================================================================
void CSceneTriangle3D::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size){}