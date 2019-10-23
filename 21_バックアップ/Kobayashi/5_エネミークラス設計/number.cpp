//=============================================================================
//
// �i���o�[�̏��� [number.cpp]
// Author :Yamashita
//
//=============================================================================
#include "number.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "score.h"


//============================================================================
//	�}�N����`
//============================================================================
#define MAX (20)
#define BG_TEXTURE_UV_U			(1.0f)							// �e�N�X�`���A�j���[�V����U�͈�
#define BG_TEXTURE_UV_V			(1.0f)	

//�ÓI�����o�ϐ��錾
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = {};
//=============================================================================
//	�R���X�g���N�^
//=============================================================================
CNumber::CNumber()
{
	m_pVtxBuff = NULL;
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CNumber::~CNumber()
{

}

//=============================================================================
// �e�N�X�`���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CNumber::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�C���X�^���X
	CManager Manager;

	//�f�o�C�X���擾
	pDevice = Manager.GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &m_pTexture);

	return S_OK;
}
//=============================================================================
// �i���o�[�̏���������
//=============================================================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, float fWeight, float fHeight)
{
	LPDIRECT3DDEVICE9 pDevice;
	
	//�f�o�C�X���擾
	CManager Manager;
	pDevice = Manager.GetRenderer()->GetDevice();

	m_pos = pos;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	//���_���ւ̃|�C���^
	VERTEX_2D*pVtx;

	//�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - fWeight, m_pos.y - fHeight, 0.0f) + m_pos;
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + fWeight, m_pos.y - fHeight, 0.0f) + m_pos;
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - fWeight, m_pos.y + fHeight, 0.0f) + m_pos;
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + fWeight, m_pos.y + fHeight, 0.0f) + m_pos;

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//�F�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`���̍��W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �i���o�[�̏I������
//=============================================================================
void CNumber::Uninit(void)
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// �i���o�[�̍X�V����
//=============================================================================
void CNumber::Update(void)
{
	
}

//=============================================================================
// �i���o�[�̕`�揈��
//=============================================================================
void CNumber::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X���擾
	CManager Manager;
	pDevice = Manager.GetRenderer()->GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
//�i���o�[�̐�������
//=============================================================================
CNumber *CNumber::Create(D3DXVECTOR3 pos, float fWeight, float fHeight)
{

	CNumber *pNumber = NULL;

	if (pNumber == NULL)
	{
		//�������𓮓I�m��
		pNumber = new CNumber;

		if (pNumber != NULL)
		{
			// �|���S���̏���������
			pNumber->Init(pos,fWeight,fHeight);
		}
		else
		{
			MessageBox(0, "NULL�ł���", "�x��", MB_OK);
		}
	}
	else
	{
		MessageBox(0, "NULL����Ȃ��ł�", "�x��", MB_OK);
	}

	return pNumber;
}

//=============================================================================
// �i���o�[�̊J������
//=============================================================================
void CNumber::Unload(void)
{
	//�e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// �i���o�[�̐ݒ菈��
//=============================================================================
void CNumber::SetNumber(int nNumber)
{
	//���_���ւ̃|�C���^
	VERTEX_2D*pVtx;

	//�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2((nNumber % 10) * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((nNumber % 10) * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2((nNumber % 10) * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((nNumber % 10) * 0.1f + 0.1f, 1.0f);
	
	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}


//=============================================================================
// �i���o�[�̐F�̐ݒ菈��
//=============================================================================
void CNumber::SetColNumber(D3DXCOLOR col)
{
	//���_���ւ̃|�C���^
	VERTEX_2D*pVtx;

	//�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`���ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}