//=============================================================================
//
// ���U���g���� [result.cpp]
// Author : �����@����
//
//=============================================================================
#include "result.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "loadText.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FADE_OUT_TIME			(60  * 5)		// �t�F�[�h����܂ł̎���
//#define FILENAME_CLEAR			("data\\TEXTURE\\result\\Clear.png")	//�N���A
//#define FILENAME_OVER			("data\\TEXTURE\\result\\over.png")		//�I�[�o�[
#define UI_FILENAME				("data\\TEXT\\UI\\result.txt")

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CResult::TYPE CResult::m_type = TYPE_MAX;

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CResult::CResult(){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CResult::~CResult(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CResult::Init()
{
	//�ϐ��̏�����
	m_nCountTime = 0;
	m_ppTexture = NULL;

	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//�e�N�X�`�����̎擾
	CLoadTextTexture * pLoadTextTexture = CLoadTextTexture::Create(UI_FILENAME);		//2D�e�N�X�`���̃|�C���^���擾
	CLoadTextTexture::TEX_INFO TexInfo = pLoadTextTexture->GetTexInfo();	//�e�N�X�`�����̎擾
	m_nMaxTexture = TexInfo.nMaxTex;										//�e�N�X�`���̍ő吔���擾

	//�e�N�X�`�����T�C�Y���q�[�v�̈悩�烁�����𓮓I�m�ۂ���
	if (m_ppTexture == NULL)
	{
		m_ppTexture = new LPDIRECT3DTEXTURE9[m_nMaxTexture];
	}

	// �e�N�X�`���̐���
	for (int nCntTex = 0; nCntTex < m_nMaxTexture; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			TexInfo.ppFileName[nCntTex],
			&m_ppTexture[nCntTex]);
	}

	//UI���̎擾
	int nMaxPolygon = pLoadTextTexture->GetMaxPolygon();

	for (int nCnt = 0; nCnt < nMaxPolygon; nCnt++)
	{
		CLoadTextTexture::POLYGON_INFO PolygonInfo = pLoadTextTexture->GetPolygonInfo(nCnt);

		//UI�̐���
		CScene2D * pScene2D = CScene2D::Create(PolygonInfo.pos, PolygonInfo.size, PolygonInfo.type);
		pScene2D->BindTexture(m_ppTexture[PolygonInfo.nTex]);
	}

	//�e�L�X�g����j������
	if (pLoadTextTexture != NULL)
	{
		pLoadTextTexture->Uninit();
		delete pLoadTextTexture;
	}

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CResult::Uninit(void)
{
	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < m_nMaxTexture; nCntTex++)
	{
		if (m_ppTexture[nCntTex] != NULL)
		{
			m_ppTexture[nCntTex]->Release();
			m_ppTexture[nCntTex] = NULL;
		}
	}

	//�e�N�X�`���|�C���^�̔j��
	if (m_ppTexture != NULL)
	{
		delete[] m_ppTexture;
		m_ppTexture = NULL;
	}

	//�t�F�[�h�ȊO���폜
	CScene::ReleaseFade();
}
//=============================================================================
// �X�V����
//=============================================================================
void CResult::Update(void)
{
	//�J�E���^�[�̉��Z
	m_nCountTime++;
	
	//�C���v�b�g�̎擾�@
	CGamePad	*pGamePad = CManager::GetInputGamePad();
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();

	if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (CFade::FADE_OUT != CFade::GetFadeMode())
		{
			CFade::SetFade(CManager::MODE_TITLE);
		}
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void CResult::Draw(void){}