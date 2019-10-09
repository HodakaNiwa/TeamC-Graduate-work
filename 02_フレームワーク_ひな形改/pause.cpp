//=============================================================================
//
// �|�[�Y���� [pause.cpp]
// Author : ���������@SATOU ASUMI
//
//=============================================================================
#include "pause.h"
#include "scene.h"
#include "scene2D.h"
#include "input.h"
#include "gamepad.h"
#include "manager.h"
#include "game.h"
#include "Fade.h"
#include "sound.h"

//=============================================================================
// ��������
//=============================================================================
CPause * CPause::Create(void)
{
	//�C���X�^���X�̐���
	CPause * pPause = new CPause;

	//����������
	pPause->Init();
	return pPause;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPause::CPause()
{
	//�|�C���^�̏�����
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		m_pScene2D[nCntPolygon] = NULL;
	}

	m_state = STATE_CONTINUE;				//�I������Ă���{�^���ݒ�
	m_StateBottun = STATEBOTTUN_NORMAL;		//�{�^���̑I����Ԃ̐ݒ�
	m_bDeth = false;						//���S��Ԃ��ǂ���
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPause::Init(void)
{
	//�|���S���̐���
	if (m_pScene2D[0] == NULL)
	{
		/*m_pScene2D[0] = CScene2D::Create(D3DXVECTOR3(640.0f, 370.0f, 0.0f),
			D3DXVECTOR3(640.0f, 370.0f, 0.0f),
			CScene2D::TEX_MAX,
			CScene2D::TYPE_UI);*/

		if (m_pScene2D[0] != NULL)
		{
			m_pScene2D[0]->SetColoer(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f));
		}
	}

	if (m_pScene2D[1] == NULL)
	{
		/*m_pScene2D[1] = CScene2D::Create(D3DXVECTOR3(640.0f, 370.0f, 0.0f),
										  D3DXVECTOR3(275.0f, 350.0f, 0.0f),
										  CScene2D::TEX_PAUSEBACE,
										  CScene2D::TYPE_UI);*/
	}

	if (m_pScene2D[2] == NULL)
	{
		/*m_pScene2D[2] = CScene2D::Create(D3DXVECTOR3(640.0f, 250.0f, 0.0f),
										 D3DXVECTOR3(150.0f, 75.0f, 0.0f),
										 CScene2D::TEX_PAUSE,
										 CScene2D::TYPE_UI);*/

		if (m_pScene2D[2] != NULL)
		{
			m_pScene2D[2]->SetTexUV(0.0f, 1.0f, 0.0f, 0.25f);
		}
	}

	if (m_pScene2D[3] == NULL)
	{
		/*m_pScene2D[3] = CScene2D::Create(D3DXVECTOR3(640.0f, 380.0f, 0.0f),
			D3DXVECTOR3(150.0f, 75.0f, 0.0f),
			CScene2D::TEX_PAUSE,
			CScene2D::TYPE_UI);*/

		if (m_pScene2D[3] != NULL)
		{
			m_pScene2D[3]->SetTexUV(0.0f, 1.0f, 0.25f, 0.5f);
			m_pScene2D[3]->SetColoer(D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f));
		}
	}

	if (m_pScene2D[4] == NULL)
	{
		/*m_pScene2D[4] = CScene2D::Create(D3DXVECTOR3(640.0f, 510.0f, 0.0f),
			D3DXVECTOR3(150.0f, 75.0f, 0.0f),
			CScene2D::TEX_PAUSE,
			CScene2D::TYPE_UI);*/

		if (m_pScene2D[4] != NULL)
		{
			m_pScene2D[4]->SetTexUV(0.0f, 1.0f, 0.5f, 0.75f);
			m_pScene2D[4]->SetColoer(D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f));
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPause::Uninit(void)
{
	//2D�|���S���̃������̔j��
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		if (m_pScene2D[nCntPolygon] != NULL)
		{
			m_pScene2D[nCntPolygon]->Uninit();
			m_pScene2D[nCntPolygon] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CPause::Update(void)
{
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();		//�L�[�{�[�h���̎擾
	CGamePad * pGamePad = CManager::GetInputGamePad();					//�Q�[���p�b�h�̎擾

	//�{�^���̑I��
	if (pInputKeyboard->GetKeyboardTrigger(DIK_UP) == true || pGamePad->GetGamePadLeftStickTrigger(CGamePad::ANALOG_STICK_UP) == true)
	{
		m_state = (STATE)((m_state + (STATE_MAX - 1)) % STATE_MAX);
	}
	if (pInputKeyboard->GetKeyboardTrigger(DIK_DOWN) == true || pGamePad->GetGamePadLeftStickTrigger(CGamePad::ANALOG_STICK_DOWN) == true)
	{
		m_state = (STATE)((m_state + 1) % STATE_MAX);
	}
	if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || pGamePad->GetGamePadTrigger(CGamePad::ELECOM_A) == true)
	{
		m_StateBottun = STATEBOTTUN_SELECT;
	}

	//2D�|���S���̍X�V
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		if (m_pScene2D[nCntPolygon] != NULL)
		{
			//�F�̐ݒ�
			if (m_state == (STATE)(nCntPolygon - 2))
			{
				if (m_StateBottun == STATEBOTTUN_NORMAL)
				{
					m_pScene2D[nCntPolygon]->SetColoer(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				else
				{
					m_pScene2D[nCntPolygon]->SetColoer(D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f));	//�F�̐ݒ�
					BottunUpdate();
				}
			}
			else if(nCntPolygon != 0 && nCntPolygon != 1)
			{
				m_pScene2D[nCntPolygon]->SetColoer(D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f));
			}

			//�X�V����
			m_pScene2D[nCntPolygon]->Update();
		}
	}

	//���S��Ԃ�������
	if (m_bDeth == true)
	{
		Uninit();		//�I������
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPause::Draw(void)
{
	//2D�|���S���̕`��
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		if (m_pScene2D[nCntPolygon] != NULL)
		{
			m_pScene2D[nCntPolygon]->Draw();
		}
	}
}

//=============================================================================
// �{�^���̍X�V����
//=============================================================================
void CPause::BottunUpdate(void)
{
	//�Q�[���|�C���^�̎擾
	CGame * pGame = CManager::GetGame();

	//�t�F�[�h��Ԃ̎擾
	CFade::FADE FadeMode = CFade::GetFadeMode();

	pGame->SetPause(false);	//�|�[�Y��Ԃ����Ƃɖ߂�
	m_bDeth = true;			//���S��Ԃɂ���

	switch (m_state)
	{
	case STATE_CONTINUE:
		break;

	case STATE_RETRY:
		if (FadeMode != CFade::FADE_OUT)
		{
			//�{�^������������Ԃɂ���
			CFade::SetFade(CManager::MODE_GAME);	//�`���[�g���A����ʂɈړ�����
		}
		break;

	case STATE_QUIT:
		if (FadeMode != CFade::FADE_OUT)
		{
			//�{�^������������Ԃɂ���
			CFade::SetFade(CManager::MODE_TITLE);	//�`���[�g���A����ʂɈړ�����
		}
		break;
	}
}