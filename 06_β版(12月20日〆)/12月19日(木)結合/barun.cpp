//=============================================================================
//
// ���D���� [barun.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#include "barun.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEX_NAME	("data/TEXTURE/object/barun.png")

//=============================================================================
// �O���錾
//=============================================================================
LPDIRECT3DTEXTURE9 CBarun::m_pTexture = NULL;

//=============================================================================
// ��������
//=============================================================================
CBarun * CBarun::Create(const D3DXVECTOR3 pos, int nType, D3DXVECTOR3 move)
{
	//�C���X�^���X�̐���
	CBarun * pSceneBillboard;
	pSceneBillboard = new CBarun;

	//����������
	pSceneBillboard->Init(nType, move);

	//�ݒ菈��
	pSceneBillboard->Set(pos, D3DXVECTOR3(10.0f, 20.0f, 0.0f));

	return pSceneBillboard;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBarun::CBarun() : CSceneBillborad(3, OBJTYPE_BILLBORAD)
{
	m_move = INITIALIZE_VECTOR3;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBarun::CBarun(int nPriority, OBJTYPE ObjType) : CSceneBillborad(nPriority, ObjType)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBarun::~CBarun(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBarun::Init(int nType, D3DXVECTOR3 move)
{
	//�r���{�[�h�̏�����
	CSceneBillborad::Init();
	CSceneBillborad::BindTexture(m_pTexture);
	CSceneBillborad::SetUV(D3DXVECTOR2(0.0f + (nType * 0.2f), 0.0f), D3DXVECTOR2(0.2f + (nType * 0.2f), 1.0f));

	//�ϐ��錾
	m_move = move;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBarun::Uninit(void)
{
	CSceneBillborad::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBarun::Update(void)
{
	D3DXVECTOR3 Pos = CSceneBillborad::GetPos();

	Pos += m_move;

	CSceneBillborad::SetPos(Pos);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBarun::Draw(void)
{
	//�`�揈��
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//�f�o�C�X�̎擾

	//���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	CSceneBillborad::Draw();

	//���C�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE); 

}

//=============================================================================
// �e�N�X�`���̓ǂ݂��ݏ���
//=============================================================================
void CBarun::LoadTex(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���D�e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEX_NAME, &m_pTexture);
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CBarun::UnloadTex(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}