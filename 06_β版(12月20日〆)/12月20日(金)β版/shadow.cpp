//=============================================================================
//
// �e���� [shadow.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "fieldmanager.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEX_NAME	("data/TEXTURE/Effect/effect000.jpg")

//=============================================================================
// �O���錾
//=============================================================================
LPDIRECT3DTEXTURE9 CShadow::m_pTexture = NULL;

//=============================================================================
// ��������
//=============================================================================
CShadow * CShadow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	//�C���X�^���X�̐���
	CShadow * pShadow;
	pShadow = new CShadow;

	//����������
	pShadow->Init();

	//�ݒ菈��
	pShadow->Set(pos, size);
	pShadow->BindTexture(m_pTexture);
	pShadow->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f)); 

	return pShadow;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CShadow::CShadow() : CScene3D(3, OBJTYPE_GRANDEFFECT){}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CShadow::CShadow(int nPriority, CScene::OBJTYPE obj) : CScene3D(nPriority, obj){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CShadow::~CShadow(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CShadow::Init(void)
{
	CScene3D::Init();
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CShadow::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CShadow::Update(void)
{
	CScene3D::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CShadow::Draw(void)
{
	CGame *pGame = CManager::GetGame();								// �Q�[���̎擾
	CEventCamera *pEveCam = pGame->GetEveCam();						// �C�x���g�J�����̎擾
	if (pEveCam == NULL)											// �C�x���g�J�����������Ă�����
	{
		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		// ���u�����f�B���O�����Z�����ɐݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		CScene3D::Draw();

		// ���u�����f�B���O�����ɖ߂�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
void CShadow::LoadTex(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEX_NAME, &m_pTexture);	//������
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CShadow::UnloadTex(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}