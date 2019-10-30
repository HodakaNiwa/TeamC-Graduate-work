//=============================================================================
//
// �^�C���̏��� [time.cpp]
// Author : ���������@SATOU ASUMI
//
//=============================================================================
#include "timer.h"
#include "manager.h"
#include "scene2D.h"
#include "number2D.h"
#include "scene.h"
#include "fade.h"
#include "timerlogo.h"
#include "game.h"
#include "fieldmanager.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_TIME	(180)

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
//�^�C�}�[�N���X
int CTimer::m_nTimeData[MAX_TIMENUMBER] = {};
int CTimer::m_nTime = 0;

//�J�E���g�_�E���^�C�}�[�N���X
int CCountDownTimer::m_nNumber[MAX_TIMENUMBER] = {};

//�Q�[���^�C�}�[�N���X
int CGameTimer::m_nNumber[MAX_TIMENUMBER] = {};

//=============================================================================
// ��������
//=============================================================================
CTimer * CTimer::Create(TYPE type, int Priority, D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CTimer * pTimer = NULL;

	if (type == TYPE_GAME)
	{
		if (pTimer == NULL)
		{
			pTimer = new CGameTimer(Priority, OBJTYPE_2DPOLYGON);

			if (pTimer != NULL)
			{
				pTimer->SetSizePos(pos, size);
				pTimer->Init();
			}
		}
	}
	if (type == TYPE_COUNTER)
	{
		if (pTimer == NULL)
		{
			pTimer = new CCountDownTimer(Priority, OBJTYPE_2DPOLYGON);

			if (pTimer != NULL)
			{
				pTimer->SetSizePos(pos, size);
				pTimer->Init();
			}
		}
	}
	return pTimer;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTimer::CTimer(int nPriority,OBJTYPE objType) : CScene(nPriority, objType) {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTimer::~CTimer() {}

//=============================================================================
// �^�C���̑��
//=============================================================================
void CTimer::AddTime(int nTime)
{
	//�ϐ��錾
	int nCntNum;			//�X�R�A�̌����̃J�E���^�[
	int nNum = 1;
	int nNum2 = 1;

	//�X�R�A�̉��Z
	m_nTime = nTime;

	//�����̌v�Z
	for (nCntNum = 0; nCntNum < MAX_TIMENUMBER; nCntNum++)
	{
		nNum *= 10;
	}
	for (nCntNum = 0; nCntNum < MAX_TIMENUMBER - 1; nCntNum++)
	{
		nNum2 *= 10;
	}

	for (nCntNum = 0; nCntNum < MAX_TIMENUMBER; nCntNum++)
	{
		m_nTimeData[nCntNum] = (m_nTime % nNum) /  nNum2;	//�i�[���鐔�����v�Z

															//�������P�����炷
		nNum = nNum / 10;
		nNum2 = nNum2 / 10;
	}
}

//=============================================================================
// �^�C���̎擾
//=============================================================================
int CTimer::GetTime(void)
{
	return m_nTime;
}



//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCountDownTimer::CCountDownTimer(int nPriority, OBJTYPE objType) : CTimer(nPriority,objType) {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCountDownTimer::~CCountDownTimer() {}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCountDownTimer::Init()
{
	//�ϐ��錾
	int nCntNum;
	int nCntNumber;

	//�����̃|�C���^�̏�����
	for (nCntNumber = 0; nCntNumber < MAX_TIMENUMBER; nCntNumber++)
	{
		m_pNumber[nCntNumber] = NULL;
	}

	//�����̏�����
	for (nCntNum = 0; nCntNum < MAX_TIMENUMBER; nCntNum++)
	{
		m_nNumber[nCntNum] = 0;
	}

	//�����l
	m_nCountDown = 3;
	m_nCounter = 0;
	m_nDeathCount = 0;
	m_bDeath = false;
	m_bCountStop = false;
	AddTime(m_nCountDown);
	Set(m_pos, m_size);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCountDownTimer::Uninit(void)
{
	//�ϐ��錾
	int nCntNumber;

	for (nCntNumber = 0; nCntNumber < MAX_TIMENUMBER; nCntNumber++)
	{
		if (m_pNumber[nCntNumber] != NULL)
		{
			m_pNumber[nCntNumber]->Uninit();
			delete m_pNumber[nCntNumber];
			m_pNumber[nCntNumber] = NULL;
		}
	}

	m_nTime = 0;

	for (int nCntNum = 0; nCntNum < MAX_TIMENUMBER; nCntNum++)
	{
		m_nTimeData[nCntNum] = 0;
	}

	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CCountDownTimer::Update(void)
{
	//�ϐ��錾
	int nCntNumber;

	//�J�E���g�_�E���̍X�V
	TimeCountUpdate();

	for (nCntNumber = 0; nCntNumber < MAX_TIMENUMBER; nCntNumber++)
	{
		if (m_pNumber[nCntNumber] != NULL)
		{
			m_pNumber[nCntNumber]->Update();
			m_pNumber[nCntNumber]->SetNumber(m_nTimeData[nCntNumber]);
		}
	}
}

//=============================================================================
// �J�E���g�_�E���̍X�V����
//=============================================================================
void CCountDownTimer::TimeCountUpdate(void)
{
	int nState = CManager::GetGameState();

	if (nState == 1)
	{//�Q�[���J�n���̃J�E���g�̎�
		m_nCounter--;

		if (m_nCounter % 60 == 0)
		{
			m_nCountDown--;
			CTimer::AddTime(m_nCountDown);

			if (m_nCountDown == 0)
			{
				//����������
				m_nCountDown = 0;
				m_bDeath = true;

				//�Q�[����Ԃɂ���
				CManager::SetGameState(CManager::GAMESTATE_GAME);
			}
		}
	}

	if (m_bDeath == true)
	{
		CTimer::AddTime(1);

		m_nDeathCount++;

		if (m_nDeathCount == 5)
		{
			//����������
			m_nDeathCount = 0;
			m_bDeath = false;

			//�^�C�}�[�̏I������
			Uninit();

			//�X�^�[�g���S�̐���
			CTimerLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(100.0f, 50.0f, 0.0f), CTimerLogo::TIMERLOGO_START);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CCountDownTimer::Draw(void)
{
	//�ϐ��錾
	int nCntNumber;

	for (nCntNumber = 0; nCntNumber < MAX_TIMENUMBER; nCntNumber++)
	{
		if (m_pNumber[nCntNumber] != NULL)
		{
			m_pNumber[nCntNumber]->Draw();
		}
	}
}

//=============================================================================
// �ݒ菈��
//=============================================================================
void CCountDownTimer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//�ϐ��錾
	int nCntNumber;

	for (nCntNumber = 0; nCntNumber < MAX_TIMENUMBER; nCntNumber++)
	{
		if (m_pNumber[nCntNumber] == NULL)
		{
			m_pNumber[nCntNumber] = CNumber2D::Create(D3DXVECTOR3(pos.x + (nCntNumber * (size.x * 2)), pos.y, pos.z), size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			if (m_pNumber[nCntNumber] != NULL)
			{
				m_pNumber[nCntNumber]->SetbUse(true);
			}
		}
	}
}


//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGameTimer::CGameTimer(int nPriority, OBJTYPE objType) : CTimer(nPriority,objType) {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGameTimer::~CGameTimer() {}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGameTimer::Init()
{
	//�ϐ��錾
	int nCntNum;
	int nCntNumber;

	//�����̃|�C���^�̏�����
	for (nCntNumber = 0; nCntNumber < MAX_TIMENUMBER; nCntNumber++)
	{
		m_pNumber[nCntNumber] = NULL;
	}

	//�����̏�����
	for (nCntNum = 0; nCntNum < MAX_TIMENUMBER; nCntNum++)
	{
		m_nNumber[nCntNum] = 0;
	}

	//�����l
	m_nTimeCounter = 0;
	m_nTime = MAX_TIME;
	m_bTimeStop = false;
	m_nDeathGameCount = 0;
	Set(m_pos, m_size);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CGameTimer::Uninit(void)
{
	//�ϐ��錾
	int nCntNumber;

	for (nCntNumber = 0; nCntNumber < MAX_TIMENUMBER; nCntNumber++)
	{
		if (m_pNumber[nCntNumber] != NULL)
		{
			m_pNumber[nCntNumber]->Uninit();

			delete m_pNumber[nCntNumber];
			m_pNumber[nCntNumber] = NULL;
		}
	}

	m_nTime = 0;

	for (int nCntNum = 0; nCntNum < MAX_TIMENUMBER; nCntNum++)
	{
		m_nTimeData[nCntNum] = 0;
	}

	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CGameTimer::Update(void)
{
	//�ϐ��錾
	int nCntNumber;

	//�Q�[���^�C���̍X�V
	TimeGameUpdate();

	for (nCntNumber = 0; nCntNumber < MAX_TIMENUMBER; nCntNumber++)
	{
		if (m_pNumber[nCntNumber] != NULL)
		{
			m_pNumber[nCntNumber]->Update();
			m_pNumber[nCntNumber]->SetNumber(m_nTimeData[nCntNumber]);
		}
	}
}

//=============================================================================
// �J�E���g�_�E���̍X�V����
//=============================================================================
void CGameTimer::TimeGameUpdate(void)
{
	int nState = CManager::GetGameState();

	if (nState == 2)
	{//�Q�[����Ԃ̎�
		m_nTimeCounter--;

		if (m_nTimeCounter % 60 == 0)
		{
			m_nTime--;
			CTimer::AddTime(m_nTime);

			if (m_nTime == 120)
			{//�c��2���ɂȂ�����e�N�X�`���𐶐�
				CTimerLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(100.0f, 50.0f, 0.0f), CTimerLogo::TIMERLOGO_2MIN);
				CManager::GetGame()->GetFieldManger()->SetState(CFieldManager::STATE_TIME_2);
			}
			if (m_nTime == 60)
			{//�c��1���ɂȂ�����e�N�X�`���𐶐�
				CTimerLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(100.0f, 50.0f, 0.0f), CTimerLogo::TIMERLOGO_1MIN);
				CManager::GetGame()->GetFieldManger()->SetState(CFieldManager::STATE_TIME_1);
			}
			if (m_nTime == 30)
			{ //�c��30�b�ɂȂ�����e�N�X�`���𐶐�
				CTimerLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(100.0f, 50.0f, 0.0f), CTimerLogo::TIMERLOGO_30SEC);
				CManager::GetGame()->GetFieldManger()->SetState(CFieldManager::STATE_TIME_30);
			}
			if (m_nTime == 0)
			{
				//�l������������
				m_bTimeStop = true;
				m_nTime = 0;

				//�Q�[�����I����Ԃɂ���
				CManager::SetGameState(CManager::GAMESTATE_FIN);
			}
		}
	}

	if (m_bTimeStop == true)
	{
		CTimer::AddTime(1);

		m_nDeathGameCount++;

		if (m_nDeathGameCount == 5)
		{
			//�t�B�j�b�V�����S�̐���
			CTimerLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(100.0f, 50.0f, 0.0f), CTimerLogo::TIMERLOGO_FIN);

			//�^�C�}�[�𓧖��ɂ���
			m_pNumber[0]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
		if (m_nDeathGameCount >= 190)
		{
			//����������
			m_nDeathGameCount = 0;
			m_bTimeStop = false;

			//�^�C�}�[��0�ɂȂ����烊�U���g�Ƀt�F�[�h����
			CFade::SetFade(CManager::MODE_RESULT);

			//�^�C�}�[�̏I������
			Uninit();
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGameTimer::Draw(void)
{
	//�ϐ��錾
	int nCntNumber;

	if (m_nTime <= 5)
	{//�Q�[���^�C�}�[��5�b�ɂȂ�����`�悷��
		for (nCntNumber = 0; nCntNumber < MAX_TIMENUMBER; nCntNumber++)
		{
			if (m_pNumber[nCntNumber] != NULL)
			{
				m_pNumber[nCntNumber]->Draw();
			}
		}
	}
}

//=============================================================================
// �ݒ菈��
//=============================================================================
void CGameTimer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//�ϐ��錾
	int nCntNumber;

	for (nCntNumber = 0; nCntNumber < MAX_TIMENUMBER; nCntNumber++)
	{
		if (m_pNumber[nCntNumber] == NULL)
		{
			m_pNumber[nCntNumber] = CNumber2D::Create(D3DXVECTOR3(pos.x + (nCntNumber * (size.x * 2)), pos.y, pos.z), size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			if (m_pNumber[nCntNumber] != NULL)
			{
				m_pNumber[nCntNumber]->SetbUse(true);
			}
		}
	}
}

