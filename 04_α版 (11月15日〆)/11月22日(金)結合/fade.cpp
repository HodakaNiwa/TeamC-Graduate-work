//=============================================================================
//
// �t�F�[�h���� [fade.cpp]
// Author : ���������@SATOU ASUMI
//
//=============================================================================
#include "fade.h"
#include "manager.h"
#include "scene.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define FADE_SPEED	(0.025f)		//�t�F�[�h����X�s�[�h
#define FADE_SIZE	(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f))
#define FADE_POS

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CFade::FADE CFade::m_fade = FADE_NONE;							// �t�F�[�h�̏��
D3DXCOLOR CFade::m_Coloer = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �F�̐ݒ�
CManager::MODE CFade::m_ModeNext = CManager::MODE_MAX;			//���̃��[�h

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CFade::CFade() :CScene(MAX_LAYER - 1, OBJTYPE_FADE)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CFade::~CFade()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CFade::Init(void)
{
	//�ϐ��̏�����
	m_fade = FADE_IN;
	m_Coloer = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	m_ModeNext = CManager::GetMode();

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
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

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
void CFade::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
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
void CFade::Update(void)
{
	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)						//�t�F�[�h�C���̏�ԏ���
		{
			m_Coloer.a -= FADE_SPEED;				//��ʂ𓧖��ɂ��Ă���

			if (m_Coloer <= 0.0f)
			{
				m_Coloer.a = 0.0f;					//0.0f�ɍĐݒ肷��
				m_fade = FADE_NONE;					//�������Ă��Ȃ���Ԃɂ���
			}
		}
		else if (m_fade == FADE_OUT)				//�t�F�[�h�A�E�g�̏�ԏ���
		{
			m_Coloer.a += FADE_SPEED;				//��ʂ�s�����ɂ��Ă���

			if (m_Coloer.a >= 1.0f)
			{
				m_Coloer.a = 1.0f;					//1.0f�ɐݒ肷��
				m_fade = FADE_IN;					//�t�F�[�h�C����Ԃɂ���

				//���[�h�ݒ�
				CManager::SetMode(m_ModeNext);
			}
		}

		// ���_�o�b�t�@�̏���ݒ�
		VERTEX_2D*pVtx;

		//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, m_Coloer.a);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, m_Coloer.a);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, m_Coloer.a);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, m_Coloer.a);

		//���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CFade::Draw(void)
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
	pDevice->SetTexture(0, NULL);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		2);
}

//=============================================================================
// �t�F�[�h�ݒ菈��
//=============================================================================
void CFade::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
}

//=============================================================================
// ���߂�؂�ւ��ݒ菈��
//=============================================================================
void CFade::SetFade(CManager::MODE ModeNext)
{
	m_fade = FADE_OUT;								//�t�F�[�h�A�E�g��Ԃɂ���
	m_ModeNext = ModeNext;
	m_Coloer = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//������Ԃɂ���
}

//=============================================================================
// ���߂�؂�ւ��ݒ菈��
//=============================================================================
CFade::FADE CFade::GetFadeMode(void)
{
	return m_fade;
}