//====================================================================================
//
// ��9���UI����	[robotUI.cpp]
// Author;�r�J�R�N
//
//====================================================================================
#include "robotUI.h"
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "character.h"
#include "library.h"
#include "debuglog.h"
#include "scene.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define WARNING_TEX_FILENAME_0 "data/TEXTURE/game/Warning000.png"	// �댯�x��
#define WARNING_TEX_FILENAME_1 "data/TEXTURE/game/Robot_start.png"	// �o���e���b�v

// �댯�x��
#define ROBOTUI_WARNING_POS		(D3DXVECTOR3(SCREEN_WIDTH/2,70.0f,1.0f))	// ���W
#define ROBOTUI_WARNING_COL		(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))			// �F
#define ROBOTUI_WARNING_WIDTH	(SCREEN_WIDTH/2)							// ��
#define ROBOTUI_WARNING_HEIGHT	(40.0f)										// ����

// �o���e���b�v
#define ROBOTUI_TELOP_POS		(D3DXVECTOR3(SCREEN_WIDTH/2,600.0f,1.0f))	// ���W
#define ROBOTUI_TELOP_COL		(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))			// �F
#define ROBOTUI_TELOP_WIDTH		(SCREEN_WIDTH/2)							// ��
#define ROBOTUI_TELOP_HEIGHT	(100.0f)									// ����

//*****************************************************************************
//     �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CRobotUI::m_apTexture[ROBOTUI_TEXTURE_MAX] = {};

//*****************************************************************************
//     CRobotUI�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CRobotUI::CRobotUI()
{
	m_pRobotWarning = NULL;
	m_pRobotTelop	= NULL;
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CRobotUI::~CRobotUI()
{
}

//=============================================================================
//    ��������
//=============================================================================
CRobotUI *CRobotUI::Create(void)
{
	CRobotUI *pRobotUI = NULL;
	if (pRobotUI != NULL) { return NULL; }

	// ���������m��
	pRobotUI = new CRobotUI;
	if (pRobotUI == NULL) { return NULL; }

	// ����������
	if (FAILED(pRobotUI->Init()))
	{
	}

	return pRobotUI;	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    �e�N�X�`���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CRobotUI::Load(void)
{
	char aFileName[ROBOTUI_TEXTURE_MAX][256]
	{
		WARNING_TEX_FILENAME_0,
		WARNING_TEX_FILENAME_1,
	};

	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer == NULL) { return E_FAIL; }

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

	// �e�N�X�`���̓ǂݍ���
	for (int nCntTex = 0; nCntTex < ROBOTUI_TEXTURE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice, aFileName[nCntTex], &m_apTexture[nCntTex]);
	}

	return S_OK;
}

//=============================================================================
//    �e�N�X�`���̊J������
//=============================================================================
void CRobotUI::UnLoad(void)
{
	for (int nCntTex = 0; nCntTex < ROBOTUI_TEXTURE_MAX; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CRobotUI::Init(void)
{
	// �e��������
	CreateWarning();	// �댯�x��
	CreateTelop();		// �o���e���b�v

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CRobotUI::Uninit(void)
{
	// �e�I������
	ReleaseWarning();	// �댯�x�����J��
	ReleaseTelop();		// �o���e���b�v���J��
}

//=============================================================================
//    �X�V����
//=============================================================================
void CRobotUI::Update(void)
{
	// �e�X�V����
	UpdateWarning();	// �댯�x��
	UpdateTelop();		// �o���e���b�v
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CRobotUI::Draw(void)
{
	if (m_pRobotWarning != NULL)	{ m_pRobotWarning->Draw(); }
	if (m_pRobotTelop != NULL)		{ m_pRobotTelop->Draw(); }
}

//=============================================================================
//    �댯�x�����J�����鏈��
//=============================================================================
void CRobotUI::CreateWarning(void)
{
	m_pRobotWarning = CRobotUI2D::Create(ROBOTUI_WARNING_POS, ROBOTUI_WARNING_COL, m_apTexture[0],
		(float)ROBOTUI_WARNING_WIDTH, ROBOTUI_WARNING_HEIGHT);
	if (m_pRobotWarning == NULL) { return; }
}

//=============================================================================
//    �댯�x�����J�����鏈��
//=============================================================================
void CRobotUI::ReleaseWarning(void)
{
	if (m_pRobotWarning != NULL)
	{
		m_pRobotWarning->Uninit();
		m_pRobotWarning = NULL;
	}
}

//=============================================================================
//    �댯�x���̓���
//=============================================================================
void CRobotUI::UpdateWarning(void)
{
	if (m_pRobotWarning != NULL)
	{// �댯�x����NULL�`�F�b�N
		CGame *pGame = CManager::GetGame();		// �Q�[���̎擾

		// �e���̎擾
		float fTexU		= m_pRobotWarning->GetTexU() + 0.001f;
		float fTexV		= m_pRobotWarning->GetTexV();
		float fWidth	= m_pRobotWarning->GetTexWidth();
		float fHeight	= m_pRobotWarning->GetTexHeight();

		m_pRobotWarning->SetTexU(fTexU);
		m_pRobotWarning->SetTexUV(fTexU, fTexU + fWidth, fTexV, fTexV + fHeight);

		bool bEveCamFlag = pGame->GetEVeCamFlag();	// �C�x���g�J�����̎��S�t���O���擾

		if (bEveCamFlag == true)
		{// �C�x���g�J�����������Ă��������
			bEveCamFlag = false;	// �C�x���g�J�����������Ă����Ԃ�
			Uninit();				// �I������
		}
	}
}

//=============================================================================
//    �댯�x�����J�����鏈��
//=============================================================================
void CRobotUI::CreateTelop(void)
{
	m_pRobotTelop = CRobotUI2D::Create(ROBOTUI_TELOP_POS, ROBOTUI_TELOP_COL, m_apTexture[1],
		(float)ROBOTUI_TELOP_WIDTH, ROBOTUI_TELOP_HEIGHT);
	if (m_pRobotTelop == NULL) { return; }
}

//=============================================================================
//    �댯�x�����J�����鏈��
//=============================================================================
void CRobotUI::ReleaseTelop(void)
{
	if (m_pRobotTelop != NULL)
	{
		m_pRobotTelop->Uninit();
		m_pRobotTelop = NULL;
	}
}

//=============================================================================
//    �댯�x���̓���
//=============================================================================
void CRobotUI::UpdateTelop(void)
{
	if (m_pRobotTelop != NULL)
	{// �o���e���b�v��NULL�`�F�b�N

	}
}

//*****************************************************************************
//     CRobotUI2D�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CRobotUI2D::CRobotUI2D(): CScene2D(10, CScene::OBJTYPE_UI)
{
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CRobotUI2D::~CRobotUI2D()
{

}

//=============================================================================
//    ��������
//=============================================================================
CRobotUI2D *CRobotUI2D::Create(D3DXVECTOR3 pos, D3DXCOLOR col, LPDIRECT3DTEXTURE9 pTexture, float fWidth, float fHeight, float fTexU, float fTexV, float fTexWidth, float fTexHeight, float fRot)
{
	// ���������m��
	CRobotUI2D *pRobotUI2D = NULL;
	pRobotUI2D = new CRobotUI2D;
	if (pRobotUI2D == NULL) { return NULL; }

	// ����������
	pRobotUI2D->Init();
	pRobotUI2D->Set(pos, D3DXVECTOR3(fWidth, fHeight, 0.0f));
	pRobotUI2D->CScene2D::BindTexture(pTexture);

	// �e��l�̐ݒ�
	pRobotUI2D->SetPos(pos);              // �|���S���̍��W
	pRobotUI2D->SetRot(fRot);             // �|���S���̌���
	pRobotUI2D->SetCol(col);              // �|���S���̐F
	pRobotUI2D->SetWidth(fWidth);         // �|���S���̕�
	pRobotUI2D->SetHeight(fHeight);       // �|���S���̍���
	pRobotUI2D->SetTexU(fTexU);           // �|���S���̍���e�N�X�`��U���W
	pRobotUI2D->SetTexV(fTexV);           // �|���S���̍���e�N�X�`��V���W
	pRobotUI2D->SetTexWidth(fTexWidth);   // �|���S���̃e�N�X�`�����W�̉���
	pRobotUI2D->SetTexHeight(fTexHeight); // �|���S���̃e�N�X�`�����W�̏c��
	pRobotUI2D->BindTexture(pTexture);

	return pRobotUI2D;		// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CRobotUI2D::Init(void)
{
	CScene2D::Init();

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CRobotUI2D::Uninit(void)
{
	CScene2D::Uninit();
	// ���S�t���O�𗧂Ă�
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CRobotUI2D::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CRobotUI2D::Draw(void)
{
	CScene2D::Draw();
}
