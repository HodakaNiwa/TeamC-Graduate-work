//=============================================================================
//
// �n�ʃ}�l�[�W���[���� [fieldmanager.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#include "fieldmanager.h"
#include "field.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAIN_SIZE	(D3DXVECTOR3(2000.0f, 0.0f, 2000.0f))
#define TEX_NAME	("data/TEXTURE/Field/Stage000.jpg")
#define TEX_NAME2	("data/TEXTURE/Field/Stage_tex.png")
#define TEX_NAME3	("data/TEXTURE/Field/Stage_Red.jpg")
#define TEX_NAME4	("data/TEXTURE/Field/Stage_Yellow.jpg")
#define START_X		(-2000.0f)
#define START_Y		(-2000.0f)
#define TIME_X		(30)
#define TIME_RANDUM		(20)
#define TIME_RANDUM2	(180)
#define TIME_WAVE		(10)
#define TIME_WAVE2		(50)
#define TIME_WAVE3		(90)
#define TIME_WAVE4		(130)
#define TIME			(90)
#define TEX_GRAUND		(0)		//�n��
#define TEX_BLUE		(1)		//��
#define TEX_RED			(2)		//��
#define TEX_YELLOW		(3)		//���F

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CFieldManager::CFieldManager() {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CFieldManager::~CFieldManager(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CFieldManager::Init(void)
{
	//�e�N�X�`���̐���
	m_pTexture[FIELD_TEX] = {};

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEX_NAME, &m_pTexture[0]);	//�n��
	D3DXCreateTextureFromFile(pDevice, TEX_NAME2, &m_pTexture[1]);	//��
	D3DXCreateTextureFromFile(pDevice, TEX_NAME3, &m_pTexture[2]);	//��
	D3DXCreateTextureFromFile(pDevice, TEX_NAME4, &m_pTexture[3]);	//��

	//�ϐ��̏�����
	for (int nCnt = 0; nCnt < MAX_FIELD; nCnt++)
	{
		m_pField[nCnt] = NULL;
		m_pFieldGraund[nCnt] = NULL;
	}

	m_nCountTime = 0;
	m_nCountY = 1;
	m_nCountX = 0;
	m_nCountY2 = 1;
	m_nCountX2 = 0;
	m_nCountY3 = 1;
	m_nCountX3 = 0;
	m_nCountY4 = 1;
	m_nCountX4 = 0;
	m_state = STATE_NORMAL;
	m_pMainField = NULL;
	m_pCountDownField = NULL;
	m_fCol_a = 1.0f;

	//�f�W�^���n�ʂ̐���
	int nCountField = 0;
	for (int nCntY = 0; nCntY < LINE; nCntY++)
	{
		float PosY = ((BLOCK_SIZE.z * 2.0f) * nCntY) + START_Y;

		for (int nCntX = 0; nCntX < LINE; nCntX++)
		{
			float PosX = ((BLOCK_SIZE.x * 2.0f) * nCntX) + START_X;
			m_pField[nCountField] = CField::Create(D3DXVECTOR3(PosX, 0.0f, PosY), BLOCK_SIZE, CField::TYPE_BLOCK);
			m_pField[nCountField]->BindTexture(m_pTexture[TEX_BLUE]);
			m_pField[nCountField]->SetLight(true);
			m_pField[nCountField]->SetDraw(false);
			nCountField++;
		}
	}

	//�O�����h�n��
	int nCountField2 = 0;
	for (int nCntY = 0; nCntY < LINE; nCntY++)
	{
		float PosY = ((BLOCK_SIZE.z * 2.0f) * nCntY) + START_Y;

		for (int nCntX = 0; nCntX < LINE; nCntX++)
		{
			float PosX = ((BLOCK_SIZE.x * 2.0f) * nCntX) + START_X;
			m_pFieldGraund[nCountField2] = CField::Create(D3DXVECTOR3(PosX, 0.0f, PosY), BLOCK_SIZE, CField::TYPE_BLOCK);
			m_pFieldGraund[nCountField2]->BindTexture(m_pTexture[TEX_GRAUND]);
			m_pFieldGraund[nCountField2]->SetRot(D3DXVECTOR3(/*D3DX_PI*/ 0.0f, 0.0f, 0.0f));
			m_pFieldGraund[nCountField2]->SetDraw(false);

			nCountField2++;
		}
	}

	//�J�E���g�_�E���p�̃e�N�X�`��
	/*if (m_pCountDownField == NULL)
	{
		m_pCountDownField = CField::Create(D3DXVECTOR3(-BLOCK_SIZE.x, 0.1f, START_Y), D3DXVECTOR3(MAIN_SIZE.x, 0.0f, BLOCK_SIZE.z * 2.0f), CField::TYPE_COUNTDOWN);
		m_pCountDownField->BindTexture(m_pTexture[4]);
		m_pCountDownField->SetDivision(D3DXVECTOR2(LINE / 2.0f, 1));
		m_pCountDownField->SetLight(true);
		m_pCountDownField->SetState(CField::STATE_MOVE_Z);
	}*/

	//�傫�Ȓn�ʂ𐶐�����
	if (m_pMainField == NULL)
	{
		m_pMainField = CField::Create(D3DXVECTOR3(-BLOCK_SIZE.x, 0.01f, -BLOCK_SIZE.z), MAIN_SIZE, CField::TYPE_MAINFIELD);
		m_pMainField->BindTexture(m_pTexture[TEX_GRAUND]);
		m_pMainField->SetDivision(D3DXVECTOR2(LINE, LINE));
		m_pMainField->SetDraw(true);
	}

	//��Ԃ̐ݒ�
	SetState(m_state);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CFieldManager::Uninit(void)
{
	//�ϐ��̏�����
	for (int nCnt = 0; nCnt < MAX_FIELD; nCnt++)
	{
		m_pField[nCnt] = NULL;
		m_pFieldGraund[nCnt] = NULL;
	}

	//�e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < FIELD_TEX; nCnt++)
	{
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CFieldManager::Update(void)
{
	//��Ԃ��Ƃ̍X�V����
	switch (m_state)
	{
	case STATE_XTURN:  UpdateXTurn(); break;	//X���̉�]
	case STATE_RANDOM: UpdateRamdom(); break;	//�����_��
	case STATE_WAVE:   UpdateWave();   break;	//�g
	case STATE_COUNTDOWN: UpdateCountDown(); break;	//�J�E���g�_�E��
	case STATE_TIME_2:
	case STATE_TIME_1:
	case STATE_TIME_30: UpdateTime(); break;	//�������Ԏ��̕\��
	}
}

//=============================================================================
// ��]�̍X�V����
//=============================================================================
void CFieldManager::UpdateXTurn(void)
{
	if (m_nCountY > LINE)
	{
		m_state = STATE_NORMAL;
		m_nCountTime = 0;
		m_nCountX = 0;
		m_nCountY = 1;
		ChangeField(true);	//�n�ʕ\���̐؂�ւ�
		return; 
	}

	m_nCountTime++;

	if ((m_nCountTime % TIME_X) == 0)
	{
		for (int nCnt = 0; nCnt < LINE; nCnt++)
		{
			int nBlock = (int)(m_nCountX + nCnt);

			if (m_pField[nBlock] != NULL)
			{ 
				m_pField[nBlock]->SetState(CField::STATE_X_TURN); 
			}
		}

		m_nCountX = m_nCountY * LINE;
		m_nCountY++;
	}
}

//=============================================================================
// �����_��
//=============================================================================
void CFieldManager::UpdateRamdom(void)
{
	if (m_nCountY2 > LINE)
	{
		m_state = STATE_NORMAL;
		m_nCountTime = 0;
		m_nCountX = 0;
		m_nCountY = 1;
		m_nCountX2 = 0;
		m_nCountY2 = 1;
		ChangeField(true);	//�n�ʕ\���̐؂�ւ�
		return;
	}
	m_nCountTime++;

	if ((m_nCountTime % TIME_RANDUM) != 0) { return; }

	if (m_nCountY < LINE)
	{
		for (int nCnt = 0; nCnt < LINE; nCnt++)
		{
			int nBlock = (int)(m_nCountX + nCnt);

			if (m_pField[nBlock] == NULL) { continue; }

			if ((m_nCountY % 2) != 0)
			{
				if ((nBlock % 2) != 0) 
				{ 
					m_pField[nBlock]->SetState(CField::STATE_X_TURN);
					m_pFieldGraund[nBlock]->SetState(CField::STATE_X_TURNBACK);
				}
			}
			else
			{
				if ((nBlock % 2) == 0) 
				{ 
					m_pField[nBlock]->SetState(CField::STATE_X_TURN);
					m_pFieldGraund[nBlock]->SetState(CField::STATE_X_TURNBACK);
				}
			}
		}

		m_nCountX = m_nCountY * LINE;
		m_nCountY++;
	}

	if (m_nCountTime < TIME_RANDUM2) { return; }

	for (int nCnt = 0; nCnt < LINE; nCnt++)
	{
		int nBlock = (int)(m_nCountX2 + nCnt);

		if (m_pField[nBlock] == NULL) { continue; }

		if ((m_nCountY2 % 2) != 0)
		{
			if ((nBlock % 2) == 0) 
			{
				m_pField[nBlock]->SetState(CField::STATE_X_TURN); 
				m_pFieldGraund[nBlock]->SetState(CField::STATE_X_TURNBACK);
			}
		}
		else
		{
			if ((nBlock % 2) != 0) 
			{ 
				m_pField[nBlock]->SetState(CField::STATE_X_TURN); 
				m_pFieldGraund[nBlock]->SetState(CField::STATE_X_TURNBACK);
			}
		}
	}

	m_nCountX2 = m_nCountY2 * LINE;
	m_nCountY2++;
}

//=============================================================================
// �g�̍X�V����
//=============================================================================
void CFieldManager::UpdateWave(void)
{
	m_nCountTime++;

	if ((m_nCountTime % TIME_WAVE) != 0) { return; }

	if (m_nCountY < LINE)
	{
		for (int nCnt = 0; nCnt < LINE; nCnt++)
		{
			int nBlock = nCnt + (m_nCountY * LINE);
			m_pFieldGraund[nBlock]->BindTexture(m_pTexture[TEX_RED]);
			m_pFieldGraund[nBlock]->SetLight(true);
		}

		m_nCountY++;
	}

	if (m_nCountTime < TIME_WAVE2) { return; }

	if (m_nCountY2 < LINE)
	{
		for (int nCnt = 0; nCnt < LINE; nCnt++)
		{
			int nBlock = nCnt + (m_nCountY2 * LINE);
			m_pFieldGraund[nBlock]->BindTexture(m_pTexture[TEX_RED]);
			m_pFieldGraund[nBlock]->SetLight(true);
		}

		m_nCountY2++;
	}

	if (m_nCountTime < TIME_WAVE3) { return; }

	if (m_nCountY3 < LINE)
	{
		for (int nCnt = 0; nCnt < LINE; nCnt++)
		{
			int nBlock = nCnt + (m_nCountY3 * LINE);
			m_pFieldGraund[nBlock]->BindTexture(m_pTexture[TEX_BLUE]);
			m_pFieldGraund[nBlock]->SetLight(true);
		}

		m_nCountY3++;
	}

	if (m_nCountTime < TIME_WAVE4) { return; }

	if (m_nCountY4 < LINE)
	{
		for (int nCnt = 0; nCnt < LINE; nCnt++)
		{
			int nBlock = nCnt + (m_nCountY4 * LINE);
			m_pFieldGraund[nBlock]->BindTexture(m_pTexture[TEX_GRAUND]);
			m_pFieldGraund[nBlock]->SetLight(false);
		}

		m_nCountY4++;
	}
	else
	{
		ChangeField(true);
		m_state = STATE_NORMAL;
	}
}

//=============================================================================
// �J�E���g�_�E������
//=============================================================================
void CFieldManager::UpdateCountDown(void)
{
	if (m_pCountDownField == NULL) { return; }

	D3DXVECTOR3 Pos = m_pCountDownField->GetPos();

	if (Pos.z > MAIN_SIZE.z)
	{
		SetState(STATE_NORMAL);
	}
}

//=============================================================================
// �������Ԃ̕\������
//=============================================================================
void CFieldManager::UpdateTime(void)
{
	//���Ԃ̉Z
	m_nCountTime++;
	if (m_nCountTime < TIME) { return; }

	bool bDeth = false;
	m_fCol_a -= 0.02f;
	
	if (m_fCol_a < 0.0f)
	{
		m_fCol_a = 0;
		bDeth = true;
	}

	//�F�̐ݒ�
	if (m_pCountDownField != NULL) { m_pCountDownField->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fCol_a)); }

	//��Ԃ̐ݒ�����ɖ߂�
	if (bDeth) { SetState(STATE_NORMAL); }
}

//=============================================================================
// �n�ʂ̕\���ؑ�
//=============================================================================
void CFieldManager::ChangeField(bool bChangeMain)
{
	bool bFieldDraw = bChangeMain ? false : true;

	if (m_pMainField != NULL) { m_pMainField->SetDraw(bChangeMain); }	//���C���̒n�ʂ�`�悷��
		
	for (int nCnt = 0; nCnt < MAX_FIELD; nCnt++)
	{
		if (m_pField[nCnt] != NULL)
		{ 
			m_pField[nCnt]->SetDraw(bFieldDraw); 
			m_pField[nCnt]->SetState(CField::STATE_NOT_UPDATE);
			/*m_pField[nCnt]->Uninit();
			m_pField[nCnt] = NULL;*/
		}

		if (m_pFieldGraund[nCnt] != NULL)
		{
			m_pFieldGraund[nCnt]->SetDraw(bFieldDraw);
			m_pFieldGraund[nCnt]->SetState(CField::STATE_NOT_UPDATE);
		/*	m_pFieldGraund[nCnt]->Uninit();
			m_pFieldGraund[nCnt] = NULL;*/
		}
	}
}

//=============================================================================
// ��Ԑݒ�
//=============================================================================
void CFieldManager::SetState(STATE state)
{
	//�O��̏�Ԃ̌㏈��
	switch (m_state)
	{
	case STATE_RANDOM:	//�����_��
		for (int nCnt = 0; nCnt < MAX_FIELD; nCnt++)
		{
			if (m_pField[nCnt] != NULL)
			{
				m_pField[nCnt]->SetDraw(false);
				m_pField[nCnt]->SetState(CField::STATE_NOT_UPDATE);
			}

			if (m_pFieldGraund[nCnt] != NULL)
			{
				m_pFieldGraund[nCnt]->SetDraw(false);
				m_pFieldGraund[nCnt]->SetState(CField::STATE_NOT_UPDATE);
			}
		}
		break;	

	case STATE_WAVE:	//�g
		for (int nCnt = 0; nCnt < MAX_FIELD; nCnt++)
		{
			if (m_pFieldGraund[nCnt] != NULL)
			{
				m_pFieldGraund[nCnt]->SetDraw(false);
				m_pFieldGraund[nCnt]->SetState(CField::STATE_NOT_UPDATE);
			}
		}
		break;	

	case STATE_COUNTDOWN:	//�J�E���g�_�E��
		if(m_pCountDownField != NULL)
		{
			m_pCountDownField->Uninit();
			m_pCountDownField = NULL;
		}
		break;	

	case STATE_TIME_2:
	case STATE_TIME_1:
	case STATE_TIME_30:
		if (m_pMainField != NULL) 
		{ 
			m_pMainField->BindTexture(m_pTexture[TEX_GRAUND]); 
			m_pMainField->SetLight(false);
		}

		//�J�E���g�_�E���p
		if (m_pCountDownField != NULL)
		{
			m_pCountDownField->Uninit();
			m_pCountDownField = NULL;
		}
		break;
	}

	//��Ԃ̍X�V
	m_state = state;

	//���݂̏�Ԃ̐ݒ�
	switch (m_state)
	{
	case STATE_RANDOM:	//�����_��
		for (int nCnt = 0; nCnt < MAX_FIELD; nCnt++)
		{
			if (m_pField[nCnt] != NULL)
			{
				m_pField[nCnt]->SetDraw(true);
				m_pField[nCnt]->BindTexture(m_pTexture[TEX_BLUE]);
				m_pField[nCnt]->SetLight(true);
			}

			if (m_pFieldGraund[nCnt] != NULL)
			{
				m_pFieldGraund[nCnt]->SetDraw(true);
				m_pFieldGraund[nCnt]->BindTexture(m_pTexture[TEX_GRAUND]);
				m_pFieldGraund[nCnt]->SetRot(D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f));
			}
		}

		//���C���̒n��
		if (m_pMainField != NULL) { m_pMainField->SetDraw(false); }	
		break;

	case STATE_WAVE:	//�g
		for (int nCnt = 0; nCnt < MAX_FIELD; nCnt++)
		{
			if (m_pFieldGraund[nCnt] != NULL)
			{
				m_pFieldGraund[nCnt]->SetDraw(true);
				m_pFieldGraund[nCnt]->SetState(CField::STATE_NORMAL);
				m_pFieldGraund[nCnt]->BindTexture(m_pTexture[TEX_GRAUND]);
				m_pFieldGraund[nCnt]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
		}

		//���C���̒n��
		if (m_pMainField != NULL) { m_pMainField->SetDraw(false); }
		break;

	case STATE_COUNTDOWN:	//�J�E���g�_�E��
		if (m_pCountDownField == NULL)
		{
			m_pCountDownField = CField::Create(D3DXVECTOR3(-BLOCK_SIZE.x, 0.1f, START_Y), D3DXVECTOR3(MAIN_SIZE.x, 0.0f, BLOCK_SIZE.z * 2.0f), CField::TYPE_COUNTDOWN);
			m_pCountDownField->BindTexture(m_pTexture[TEX_RED]);
			m_pCountDownField->SetDivision(D3DXVECTOR2(LINE / 2.0f, 1));
			m_pCountDownField->SetLight(true);
			m_pCountDownField->SetState(CField::STATE_MOVE_Z);
		}

		//���C���̒n��
		if (m_pMainField != NULL) { m_pMainField->SetDraw(true); }
		break;

	case STATE_TIME_2:
		m_fCol_a = 1.0f;
		m_nCountTime = 0;
		if (m_pCountDownField == NULL)
		{
			m_pCountDownField = CField::Create(D3DXVECTOR3(-BLOCK_SIZE.x, 0.02f, -BLOCK_SIZE.z), MAIN_SIZE, CField::TYPE_MAINFIELD);
			m_pCountDownField->BindTexture(m_pTexture[TEX_BLUE]);
			m_pCountDownField->SetLight(true);
			m_pCountDownField->SetDivision(D3DXVECTOR2(LINE /** 0.5f*/, LINE/* * 0.5f*/));
		}
		break;

	case STATE_TIME_1:
		m_fCol_a = 1.0f;
		m_nCountTime = 0;
		if (m_pCountDownField == NULL)
		{
			m_pCountDownField = CField::Create(D3DXVECTOR3(-BLOCK_SIZE.x, 0.02f, -BLOCK_SIZE.z), MAIN_SIZE, CField::TYPE_MAINFIELD);
			m_pCountDownField->BindTexture(m_pTexture[TEX_YELLOW]);
			m_pCountDownField->SetLight(true);
			m_pCountDownField->SetDivision(D3DXVECTOR2(LINE/* * 0.5f*/, LINE/* * 0.5f*/));
		}
		break;

	case STATE_TIME_30:
		m_fCol_a = 1.0f;
		m_nCountTime = 0;
		if (m_pCountDownField == NULL)
		{
			m_pCountDownField = CField::Create(D3DXVECTOR3(-BLOCK_SIZE.x, 0.02f, -BLOCK_SIZE.z), MAIN_SIZE, CField::TYPE_MAINFIELD);
			m_pCountDownField->BindTexture(m_pTexture[TEX_RED]);
			m_pCountDownField->SetLight(true);
			m_pCountDownField->SetDivision(D3DXVECTOR2(LINE/* * 0.5f*/, LINE/* * 0.5f*/));
		}
		break;
	}
}