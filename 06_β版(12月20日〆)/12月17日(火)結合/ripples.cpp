//=============================================================================
//
// �g�䏈�� [ripples.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#include "ripples.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "fieldmanager.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define SIZE		(D3DXVECTOR3(1.0f, 0.0f, 1.0f))
#define LIFE		(12)
#define TEX_NAME	("data/TEXTURE/game/ripples.png")
#define ADD_SIZE	(2.0f)

//=============================================================================
// �O���錾
//=============================================================================
LPDIRECT3DTEXTURE9 CRipples::m_pTexture = NULL;

//=============================================================================
// ��������
//=============================================================================
CRipples * CRipples::Create(const D3DXVECTOR3 pos)
{
	//�C���X�^���X�̐���
	CRipples * pField;
	pField = new CRipples;

	//����������
	pField->Init();

	//�ݒ菈��
	pField->Set(pos, SIZE);
	pField->BindTexture(m_pTexture);
	pField->SetColor(D3DXCOLOR(0.0f, 0.9f, 1.0f, 1.0f));

	return pField;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CRipples::CRipples() : CScene3D(10, OBJTYPE_GRANDEFFECT){}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CRipples::CRipples(int nPriority, CScene::OBJTYPE obj) : CScene3D(nPriority, obj){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CRipples::~CRipples(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CRipples::Init(void)
{
	CScene3D::Init();

	//�ϐ��̏�����
	m_nLife = LIFE;		//�\������
	m_bDeth = false;	//���S�t���O
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CRipples::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CRipples::Update(void)
{
	CScene3D::Update();

	//�T�C�Y�̍X�V
	D3DXVECTOR3 Size = CScene3D::GetSize();
	Size.x += ADD_SIZE;
	Size.z += ADD_SIZE;

	//�T�C�Y�̐ݒ�
	CScene3D::Set(CScene3D::GetPos(), Size);

	//�\�����Ԃ����炷
	m_nLife--;

	if (m_nLife < 0)
	{
		//���l���X�V����
		D3DXCOLOR col = CScene3D::GetCol();
		col.a -= 0.05f;

		//�l�̏C��
		if (col.a < 0.0f)
		{
			col.a = 0.0f;
			m_bDeth = true;
		}

		//�F��ݒ�
		CScene3D::SetColor(col);
	}

	//���S�t���O���L����������j��
	if (m_bDeth) { Uninit(); }
}

//=============================================================================
// �`�揈��
//=============================================================================
void CRipples::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//�f�t�H���g�̏ꍇFALSE�ɂȂ��Ă��邩��TRUE�ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	CScene3D::Draw();

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�f�t�H���g�̏ꍇFALSE�ɂȂ��Ă��邩��TRUE�ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
void CRipples::LoadTex(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEX_NAME, &m_pTexture);	//������
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CRipples::UnloadTex(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}