//=============================================================================
//
// �����̏��� [number.cpp]
// Author : ���������@SATOU ASUMI
//
//=============================================================================
#include "number2D.h"
#include "manager.h"
#include "scene.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEXTURE_NAME	("data/TEXTURE/number/number.png")	
#define VECTOR_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))		//�x�N�g���̏�����

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CNumber2D::m_pTexture = NULL;

//=============================================================================
// ��������
//=============================================================================
CNumber2D * CNumber2D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, D3DXCOLOR col)
{
	//�C���X�^���X�̐���
	CNumber2D * pNumber2D;
	pNumber2D = new CNumber2D;

	//����������
	pNumber2D->Init();

	pNumber2D->Set(pos, size, col);

	return pNumber2D;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CNumber2D::CNumber2D()
{
	m_pVtxBuff	= NULL;				// ���_�o�b�t�@�ւ̃|�C���^
	m_pos		= VECTOR_ZERO;		// ���S���W
	m_Other		= VECTOR_ZERO;		// �S���_�̍��W
	m_Size		= VECTOR_ZERO;		// �T�C�Y
	m_HoldPos	= VECTOR_ZERO;		// ���W�̕ۑ�
	m_bUse = false;					// �g�p���
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CNumber2D::~CNumber2D(){}

//=============================================================================
// �e�N�X�`���̐���
//=============================================================================
HRESULT CNumber2D::Load(void)
{
	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_NAME,
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CNumber2D::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		//m_pTexture = NULL;
	}
}


//=============================================================================
// ����������
//=============================================================================
HRESULT CNumber2D::Init()
{
	//�ϐ��̏�����
	m_pos	= VECTOR_ZERO;		// ���S���W
	m_Other = VECTOR_ZERO;		// �S���_�̍��W
	m_Size	= VECTOR_ZERO;		// �T�C�Y
	m_bUse = false;				// �g�p���

	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	VERTEX_2D*pVtx;			//���_���ւ̃|�C���^

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pVtx[0].pos = VECTOR_ZERO;
	pVtx[1].pos = VECTOR_ZERO;
	pVtx[2].pos = VECTOR_ZERO;
	pVtx[3].pos = VECTOR_ZERO;

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CNumber2D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void CNumber2D::Update(void){}

//=============================================================================
// �`�揈��
//=============================================================================
void CNumber2D::Draw(void)
{
	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	if (m_bUse == true)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=============================================================================
// �ݒ菈��
//=============================================================================
void CNumber2D::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, D3DXCOLOR col)
{
	//�ϐ��錾
	float fAngle = 0.0f;	//�p�x
	float fRadius = 0.0f;	//�ӂ̒���

	VERTEX_2D*pVtx;			//���_���ւ̃|�C���^

	m_pos = pos;			//���W�ݒ�
	m_HoldPos = pos;		//���W�̕ۑ�
	m_Size = size;			//�S���_�̍��W

	//�|���S���̑Ίp���̊p�x���߂�
	fAngle = (float)atan2(m_Size.x, m_Size.y);

	//�Ίp���̒��������߂�
	fRadius = sqrtf((m_Size.x * m_Size.x) + (m_Size.y * m_Size.y));

	//�|���S���̒�����ۑ�����
	m_Other.x = sinf(fAngle) * fRadius;
	m_Other.y = cosf(fAngle) * fRadius;
	
	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	// ���_����ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_Other.x, m_pos.y - m_Other.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_Other.x, m_pos.y - m_Other.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_Other.x, m_pos.y + m_Other.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_Other.x, m_pos.y + m_Other.y, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[1].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[2].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[3].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	
	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ���̐ݒ菈��
//=============================================================================
void CNumber2D::SetNumber(const int nNumber)
{
	VERTEX_2D*pVtx;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * nNumber), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * nNumber), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * nNumber), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * nNumber), 1.0f);

		//���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// �g�p���
//=============================================================================
void CNumber2D::SetbUse(bool bUse)
{
	m_bUse = bUse;
}

//=============================================================================
// �ړ�������
//=============================================================================
void CNumber2D::SetPos(D3DXVECTOR3 pos)
{
	VERTEX_2D*pVtx;			//���_���ւ̃|�C���^

	m_pos = pos;			//���W�ݒ�

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_Other.x, m_pos.y - m_Other.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_Other.x, m_pos.y - m_Other.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_Other.x, m_pos.y + m_Other.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_Other.x, m_pos.y + m_Other.y, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ���W�̃��Z�b�g
//=============================================================================
void CNumber2D::ResetPos(void)
{
	VERTEX_2D*pVtx;			//���_���ւ̃|�C���^

	m_pos.x = m_HoldPos.x;		//���W�ݒ�

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_Other.x, m_pos.y - m_Other.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_Other.x, m_pos.y - m_Other.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_Other.x, m_pos.y + m_Other.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_Other.x, m_pos.y + m_Other.y, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �F�̐ݒ�
//=============================================================================
void CNumber2D::SetColor(D3DXCOLOR col)
{
	VERTEX_2D*pVtx;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[1].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[2].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[3].col = D3DXCOLOR(col.r, col.g, col.b, col.a);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �T�C�Y�̐ݒ�
//=============================================================================
void CNumber2D::SetSize(D3DXVECTOR3 size)
{
	//�ϐ��錾
	float fAngle = 0.0f;	//�p�x
	float fRadius = 0.0f;	//�ӂ̒���

	m_Size = size;			//�T�C�Y�̐ݒ�

	VERTEX_2D*pVtx;			//���_���ւ̃|�C���^

	//�|���S���̑Ίp���̊p�x���߂�
	fAngle = (float)atan2(m_Size.x, m_Size.y);

	//�Ίp���̒��������߂�
	fRadius = sqrtf((m_Size.x * m_Size.x) + (m_Size.y * m_Size.y));

	//�|���S���̒�����ۑ�����
	m_Other.x = sinf(fAngle) * fRadius;
	m_Other.y = cosf(fAngle) * fRadius;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_Other.x, m_pos.y - m_Other.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_Other.x, m_pos.y - m_Other.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_Other.x, m_pos.y + m_Other.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_Other.x, m_pos.y + m_Other.y, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}