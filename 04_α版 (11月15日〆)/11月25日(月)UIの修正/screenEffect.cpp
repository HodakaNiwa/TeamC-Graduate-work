//*****************************************************************************
//
//     ��ʃG�t�F�N�g�̏���[screenEffect.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "screenEffect.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define SCREENEFFECT_PRIORITY  (8)		// ��ʃG�t�F�N�g�̏����D�揇��
#define SCREENEFFECT_ALPHA_CUT (0.01f)	// ��ʃG�t�F�N�g�̓����x���ǂꂭ�炢��邩

//*****************************************************************************
//     �ÓI�����o�ϐ��錾
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CScreenEffect::CScreenEffect(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CScreenEffect::~CScreenEffect()
{

}

//=============================================================================
//    ��������
//=============================================================================
CScreenEffect *CScreenEffect::Create(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 size)
{
	// ���������m��
	CScreenEffect *pScreenEffect = NULL;
	pScreenEffect = new CScreenEffect(SCREENEFFECT_PRIORITY);
	if (pScreenEffect == NULL) { return NULL; }

	// ����������
	if (FAILED(pScreenEffect->Init()))
	{
		pScreenEffect->Uninit();
		return NULL;
	}

	// �|���S���̐ݒ�
	pScreenEffect->Set(pos, size);
	pScreenEffect->SetColoer(col);

	return pScreenEffect;	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CScreenEffect::Init(void)
{
	// ���ʂ̏���������
	if (FAILED(CScene2D::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CScreenEffect::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CScreenEffect::Update(void)
{
	// �����x���グ��
	AlphaAdd();
}

//=============================================================================
//    �����x���グ�鏈������
//=============================================================================
void CScreenEffect::AlphaAdd(void)
{
	// �F�̎擾
	D3DXCOLOR col = GetColor();

	// �����x�𑝉�
	col.a -= SCREENEFFECT_ALPHA_CUT;

	// �F�̐ݒ�
	SetColoer(col);

	// ���S����
	if (col.a <= 0.0f)
	{
		Uninit();
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CScreenEffect::Draw(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer == NULL) { return; }

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == NULL) { return; }

	bool bChange = false;
	if (GetColor().r == 0.0f && GetColor().b == 0.0f && GetColor().g == 0.0f)
	{
		// ���u�����f�B���O�����Z�����ɐݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		SetColoer(D3DXCOLOR(1.0f, 1.0f, 1.0f, GetColor().a));
		bChange = true;
	}
	else
	{
		// ���u�����f�B���O�����Z�����ɐݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	// ���ʂ̕`�揈��
	CScene2D::Draw();

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (bChange == false) { return; }
	SetColoer(D3DXCOLOR(0.0f, 0.0f, 0.0f, GetColor().a));
}