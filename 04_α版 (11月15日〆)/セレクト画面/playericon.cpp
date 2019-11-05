//=============================================================================
//
// �v���C���[�̃A�C�R���̏��� [playericon.cpp]
// Author : ���������@SATOU ASUMI
//
//=============================================================================
#include "playericon.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
#include "logo.h"
#include "scene2D.h"
#include "select.h"
#include "moveUI.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define VECTOR_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	//�x�N�g���̏�����
#define SIZE			(D3DXVECTOR3(130.0f, 75.0f, 0.0f))
#define WIDTH_DIVISION	(4)		//���̕�����
#define HIGHT_DIVISION	(2)		//�c�̕�����
#define MAX_CUNTORY		(8)		//���̍ő吔
#define ADDPOS_X		(25.0f)	//X�̒ǉ���
#define CUNTRY_SIZE		(D3DXVECTOR3(115.0f,  70.0f, 0.0f))	//�����̃T�C�Y
#define CUNTRY_POS		(D3DXVECTOR3(170.0f, 630.0f, 0.0f))
#define ADD_X			(310.0f)

//=============================================================================
// ��������
//=============================================================================
CPlayerIcon * CPlayerIcon::Create(D3DXVECTOR3 Pos, int nNumPlayer)
{
	CPlayerIcon * pPlayerIcon = new CPlayerIcon;
	pPlayerIcon->Init();
	pPlayerIcon->Set(Pos, SIZE);
	pPlayerIcon->SetNumPlayer(nNumPlayer);
	
	return pPlayerIcon;
}

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CPlayerIcon::CPlayerIcon() : CScene2D(6, CScene::OBJTYPE_UI) {}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayerIcon::CPlayerIcon(int nPriority, CScene::OBJTYPE type) : CScene2D(nPriority, type){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPlayerIcon::~CPlayerIcon(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayerIcon::Init(void)
{
	CScene2D::Init();
	m_nNumCuntry = 0;
	m_nOldNumCuntry = 0;
	m_pNationalFlag = NULL;
	m_nCountInput = 0;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPlayerIcon::Uninit()
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPlayerIcon::Update()
{
	if (GetDraw() == false) { return; }

	UpdateInputSelect();	//�A�C�R���I������
	UpdatePos();			//�ʒu�̍X�V����
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlayerIcon::Draw()
{
	CScene2D::Draw();
}

//=============================================================================
// ���̑I������
//=============================================================================
void CPlayerIcon::UpdateInputSelect(void)
{
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//�L�[�{�[�h�̎擾
	CGamePad * pGamePad = CManager::GetInputGamePad();				//�Q�[���p�b�h�̎擾
	CInputXPad * pXPad = CManager::GetXPad();
	m_nOldNumCuntry = m_nNumCuntry;									//�O��̏���ۑ�����

	if (pXPad->GetStick(0, m_nNumPlayer) == true)
	{
		if (m_pNationalFlag != NULL)
		{
			m_pNationalFlag->Uninit();
			m_pNationalFlag = NULL;
		}
	}

	if (pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_UP, m_nNumPlayer) == true)
	{//��ړ�
		if (m_nNumCuntry < WIDTH_DIVISION)
		{
			m_nNumCuntry += WIDTH_DIVISION;
		}
		else
		{
			m_nNumCuntry -= WIDTH_DIVISION;
		}
	}
	else if (pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_RIGHT, m_nNumPlayer) == true)
	{//�E�ړ�
		m_nNumCuntry += 1;

		if (m_nNumCuntry <= WIDTH_DIVISION)
		{
			if ((m_nNumCuntry % WIDTH_DIVISION) == 0) { m_nNumCuntry = 0; }
		}
		else
		{
			if ((m_nNumCuntry % MAX_CUNTORY) == 0) { m_nNumCuntry = MAX_CUNTORY / 2; }
		}
	}
	else if (pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_DOWN, m_nNumPlayer) == true)
	{//���ړ�
		if (m_nNumCuntry < WIDTH_DIVISION)
		{
			m_nNumCuntry += WIDTH_DIVISION;
		}
		else
		{
			m_nNumCuntry -= WIDTH_DIVISION;
		}
	}
	else if (pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_LEFT, m_nNumPlayer) == true)
	{//���ړ�
		m_nNumCuntry -= 1;

		if (m_nNumCuntry < 0)
		{
			m_nNumCuntry = WIDTH_DIVISION - 1;
		}
		else if (m_nNumCuntry == WIDTH_DIVISION - 1)
		{
			m_nNumCuntry = MAX_CUNTORY - 1;
		}
	}

	//����{�^������������
	if (pXPad->GetTrigger(XINPUT_GAMEPAD_A, m_nNumPlayer) == true)
	{
		//�����̐���
		m_nCountInput++;
		if (m_nCountInput < 2) { return; }

		if (m_pNationalFlag == NULL)
		{
			//�e�N�X�`���̊�����
			LPDIRECT3DTEXTURE9 pTexture = CManager::GetSelect()->GetCuntryTex(m_nNumCuntry);
			
			//�����̐���
			m_pNationalFlag = CMoveUI::Create(D3DXVECTOR3(CUNTRY_POS.x + (ADD_X * m_nNumPlayer), CUNTRY_POS.y, 0.0f), CUNTRY_SIZE, pTexture);
		}
		else
		{
			CManager::GetSelect()->SetState(CSelect::STATE_CHANGE);
		}
	}
}

//=============================================================================
// �ʒu�̐ݒ�
//=============================================================================
void CPlayerIcon::UpdatePos(void)
{
	if (m_nOldNumCuntry == m_nNumCuntry) { return; }

	//�ʒu���擾����
	D3DXVECTOR3 CuntryPos = CSelect::GetNatonalFlag(m_nNumCuntry)->GetPos();
	CuntryPos.x += ADDPOS_X;

	D3DXVECTOR3 OtherPos = CScene2D::GetOtherPos();
	CScene2D::SetPos(CuntryPos, OtherPos);
}