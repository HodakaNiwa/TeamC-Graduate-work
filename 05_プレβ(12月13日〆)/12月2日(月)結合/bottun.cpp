//=============================================================================
//
// �{�^���̏��� [bottun.cpp]
// Author : ���������@SATOU ASUMI
//
//=============================================================================
#include "bottun.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
#include "scene2D.h"
#include "RawMouse.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define VECTOR_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	//�x�N�g���̏�����
#define CHANGE_COL		(0.02f)							//���l�̕ω���

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBottun::CBottun(int nPriority, CScene::OBJTYPE type) : CScene2D(nPriority, type){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBottun::~CBottun(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBottun::Init(void)
{
	//�ϐ��̏�����
	m_State = STATE_NORMAL;
	m_fCol_a = 1.0f;
	m_fChangeCol = CHANGE_COL;

	CScene2D::Init();
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBottun::Uninit()
{
	CScene2D::Uninit();

	//���S�t���O�𗧂Ă�
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBottun::Update()
{
	//�C���v�b�g�̎擾�@
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();
	CRawMouse *pRawMouse = CManager::GetRawMouse();

	//�L�[����������
	CInputXPad * pXPad = CManager::GetXPad();
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) || pXPad->GetTrigger(XINPUT_GAMEPAD_A, nCnt) || 
			pRawMouse->GetTrigger(CRawMouse::RIMS_BUTTON_LEFT, nCnt))
		{
			m_State = STATE_ENTER;
		}
	}

	switch (m_State)
	{
	case STATE_NORMAL:

		//�F�̕ω��ʂ̍X�V
		m_fCol_a += m_fChangeCol;

		//�F�̏C��
		if (m_fCol_a < 0.0f)
		{
			m_fCol_a = 0.0f;
			m_fChangeCol *= -1;
		}
		else if (m_fCol_a > 1.0f)
		{
			m_fCol_a = 1.0f;
			m_fChangeCol *= -1;
		}

		//�F��ݒ肷��
		CScene2D::SetColoer(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fCol_a));
		break;

	case STATE_ENTER:
		CScene2D::SetColoer(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));		//�ԐF�ɐݒ肷��
		break;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBottun::Draw()
{
	CScene2D::Draw();
}