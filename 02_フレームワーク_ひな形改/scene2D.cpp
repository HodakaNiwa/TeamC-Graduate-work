//=============================================================================
//
// 2D�|���S���̏��� [scene2D.cpp]
// Author : ���������@SATOU ASUMI
//
//=============================================================================
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
#include "logo.h"
#include "bottun.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define VECTOR_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	//�x�N�g���̏�����

//=============================================================================
// ��������
//=============================================================================
CScene2D * CScene2D::Create(const D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	//�C���X�^���X�̐���
	CScene2D * pScene2D = NULL;

	if (type == TYPE_LOGO)
	{
		pScene2D = new CLogo(6, CScene::OBJTYPE_2DPOLYGON);
	}
	else if (type == TYPE_BOTTUN)
	{
		pScene2D = new CBottun(6, CScene::OBJTYPE_2DPOLYGON);
	}
	else
	{
		pScene2D = new CScene2D;
	}
	
	//����������
	pScene2D->Init();

	//�ݒ菈��
	pScene2D->Set(pos, size);	//�ʒu�A�T�C�Y�̐ݒ�
	return pScene2D;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScene2D::CScene2D(int nPriority, CScene::OBJTYPE type) : CScene(nPriority, type)
{
	m_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
	m_pos = VECTOR_ZERO;	// �|���S���̈ʒu
	m_Other = VECTOR_ZERO;	// �|���S���̑Ίp��
	m_Size = VECTOR_ZERO;	// �|���S���̃T�C�Y
	m_bUse = false;			// �g�p���Ă��邩�ǂ���
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScene2D::CScene2D() : CScene(6, CScene::OBJTYPE_2DPOLYGON)
{
	m_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
	m_pos = VECTOR_ZERO;	// �|���S���̈ʒu
	m_Other = VECTOR_ZERO;	// �|���S���̑Ίp��
	m_Size = VECTOR_ZERO;	// �|���S���̃T�C�Y
	m_bUse = false;			// �g�p���Ă��邩�ǂ���
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene2D::~CScene2D(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScene2D::Init(void)
{
	//�ϐ��錾
	float fAngle = 0.0f;	//�p�x
	float fRadius = 0.0f;	//�ӂ̒���
	m_pTexture = NULL;

	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	VERTEX_2D*pVtx;			//���_���ւ̃|�C���^

	// �|���S���̈ʒu��ݒ�
	m_pos	= VECTOR_ZERO;							// �|���S���̈ʒu
	m_Size	= VECTOR_ZERO;							// �|���S���̃T�C�Y
	m_Other = VECTOR_ZERO;							// �|���S���̂S���_
	m_Tex	= D3DXVECTOR2(0.0f, 0.0f);				// �e�N�X�`����UV�l
	m_bUse	= false;								// �g�p���Ă��邩�ǂ���
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//�F�̐ݒ�

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

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
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CScene2D::Uninit()
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//�e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture = NULL;
	}

	//���S�t���O�𗧂Ă�
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CScene2D::Update(){}

//=============================================================================
// �`�揈��
//=============================================================================
void CScene2D::Draw()
{
	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	if (m_pVtxBuff != NULL)
	{
		//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_pTexture);

		if (m_bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2); //�|���S���̕`��
		}
	}
}

//=============================================================================
// �ݒ菈��
//=============================================================================
void CScene2D :: Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//�ϐ��錾
	float fAngle = 0.0f;	//�p�x
	float fRadius = 0.0f;	//�ӂ̒���

	if (m_bUse == false)
	{
		VERTEX_2D*pVtx;		//���_���ւ̃|�C���^

		m_pos = pos;	//���W�ݒ�
		m_Size = size;	//�T�C�Y�̐ݒ�

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

		m_bUse = true;	//�g�p���Ă����Ԃɂ���
	}
}

//=============================================================================
// ���L�e�N�X�`���̊��蓖��
//=============================================================================
void CScene2D::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
// �e�N�X�`����UV�l�̎擾
//=============================================================================
D3DXVECTOR2 CScene2D::GetTex(void)
{
	return m_Tex;
}

//=============================================================================
// �e�N�X�`����UV�l�̐ݒ�
//=============================================================================
void CScene2D::SetTex(float uv, int nPattrn)
{
	VERTEX_2D*pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(uv * nPattrn, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(uv * nPattrn + uv, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(uv * nPattrn, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(uv * nPattrn + uv, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �e�N�X�`����UV�l�̐ݒ�
//=============================================================================
void CScene2D::SetTex2(float uv, float uv2, int nPattrn, int Width)
{
	VERTEX_2D*pVtx;		//���_���ւ̃|�C���^

						//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(uv * (nPattrn % Width), uv2 * (nPattrn / Width));
	pVtx[1].tex = D3DXVECTOR2(uv * (nPattrn % Width) + uv, uv2 * (nPattrn / Width));
	pVtx[2].tex = D3DXVECTOR2(uv * (nPattrn % Width), uv2 * (nPattrn / Width) + uv2);
	pVtx[3].tex = D3DXVECTOR2(uv * (nPattrn % Width) + uv, uv2 * (nPattrn / Width) + uv2);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �X�N���[������
//=============================================================================
void CScene2D::SetTexMove(const float uv)
{
	VERTEX_2D*pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f + uv);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f + uv);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f + uv);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f + uv);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �X�N���[������
//=============================================================================
void CScene2D::SetTexMove2(const float uv)
{
	VERTEX_2D*pVtx;		//���_���ւ̃|�C���^

						//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f + uv, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f + uv, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + uv, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f + uv, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// UV�l�̐ݒ�
//=============================================================================
void CScene2D::SetTexUV(float u, float u2, float v, float v2)
{
	VERTEX_2D*pVtx;		//���_���ւ̃|�C���^

						//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(u, v);
	pVtx[1].tex = D3DXVECTOR2(u2, v);
	pVtx[2].tex = D3DXVECTOR2(u, v2);
	pVtx[3].tex = D3DXVECTOR2(u2, v2);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ���W�̎擾����
//=============================================================================
D3DXVECTOR3 CScene2D::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// �S���_���W�̎擾����
//=============================================================================
D3DXVECTOR3 CScene2D::GetOtherPos(void)
{
	return m_Other;
}

//=============================================================================
// ���W�ݒ菈��
//=============================================================================
void CScene2D::SetPos(const D3DXVECTOR3 pos, const D3DXVECTOR3 Otherpos)
{
	VERTEX_2D*pVtx;		//���_���ւ̃|�C���^

	//���l�̍X�V
	m_pos = pos;
	m_Other = Otherpos;

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
// ��]�|���S���̍��W�ݒ菈��
//=============================================================================
void CScene2D::SetRotPos(const D3DXVECTOR3 pos, const float fAngle, const float fRadius)
{
	m_pos = pos;			//���S���W

	VERTEX_2D*pVtx;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pVtx[0].pos = D3DXVECTOR3(sinf(fAngle + D3DX_PI) * fRadius + m_pos.x, cosf(fAngle + D3DX_PI) * fRadius + m_pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(sinf(fAngle + D3DX_PI * 0.5f) * fRadius + m_pos.x, cosf(fAngle + D3DX_PI * 0.5f) * fRadius + m_pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(sinf(fAngle + D3DX_PI * -0.5f) * fRadius + m_pos.x, cosf(fAngle + D3DX_PI * -0.5f) * fRadius + m_pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(sinf(fAngle) * fRadius + m_pos.x, cosf(fAngle) * fRadius + m_pos.y, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �T�C�Y�̎擾����
//=============================================================================
D3DXVECTOR3 CScene2D::GetSize(void)
{
	return m_Size;
}

//=============================================================================
// �T�C�Y�̐ݒ菈��
//=============================================================================
void CScene2D::SetSize(const D3DXVECTOR3 size)
{
	m_Size = size;

	VERTEX_2D*pVtx;		//���_���ւ̃|�C��

	//�|���S���̑Ίp���̊p�x���߂�
	float fAngle = (float)atan2(m_Size.x, m_Size.y);

	//�Ίp���̒��������߂�
	float fRadius = sqrtf((m_Size.x * m_Size.x) + (m_Size.y * m_Size.y));

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

//=============================================================================
// �g�p��ԏ���
//=============================================================================
bool CScene2D::GetUse(void)
{
	return m_bUse;
}

//=============================================================================
// �T�C�Y�̐ݒ菈��
//=============================================================================
void CScene2D::SetUse(const bool bUse)
{
	m_bUse = bUse;
}

//=============================================================================
// �F�̐ݒ菈��
//=============================================================================
void CScene2D::SetColoer(const D3DXCOLOR col)
{
	m_Color = col;		//�F�̐ݒ�
	VERTEX_2D*pVtx;		//���_���ւ̃|�C���^

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