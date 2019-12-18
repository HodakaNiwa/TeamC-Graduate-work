//=============================================================================
//
// �^�C�}�[���S�̏��� [timerlogo.cpp]
// Author : �R���֎j & ��������
//
//=============================================================================
#include "timerlogo.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
#include "logo.h"
#include "bottun.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CTimerLogo::m_pTexture[TIMERLOGO_MAX] = {};

//=============================================================================
// �}�N����`
//=============================================================================
#define VECTOR_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	//�x�N�g���̏�����

//=============================================================================
// ��������
//=============================================================================
CTimerLogo * CTimerLogo::Create(const D3DXVECTOR3 pos,const D3DXVECTOR3 size,TIMERLOGO timerlogo)
{
	//�C���X�^���X�̐���
	CTimerLogo * pTimerLogo = NULL;

	if (pTimerLogo == NULL)
	{
		pTimerLogo = new CTimerLogo;

		if (pTimerLogo != NULL)
		{
			pTimerLogo->m_Size = size;
			pTimerLogo->m_FirstSize = size * 2.0f;	//��
			pTimerLogo->m_pos = pos;
			pTimerLogo->m_TimerLogo = timerlogo;
			pTimerLogo->BindTexture(m_pTexture[timerlogo]);
			pTimerLogo->Init();
			pTimerLogo->SetColoer(INITIALIZE_VECTOR4);
		}
	}
	return pTimerLogo;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTimerLogo::CTimerLogo(int nPriority, CScene::OBJTYPE type)
{
	m_pVtxBuff = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
	m_pos = VECTOR_ZERO;			// �|���S���̈ʒu
	m_Size = VECTOR_ZERO;			// �|���S���̃T�C�Y
	m_FirstSize = VECTOR_ZERO;		//�ŏ��̃T�C�Y�@��
	m_Color = INITIALIZE_VECTOR4;
	m_bDeth = false;				//���S�t���O�@��
	m_TimerLogo = TIMERLOGO_MAX;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTimerLogo::CTimerLogo()
{
	m_pVtxBuff = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
	m_pos = VECTOR_ZERO;			// �|���S���̈ʒu
	m_Size = VECTOR_ZERO;			// �|���S���̃T�C�Y
	m_FirstSize = VECTOR_ZERO;		//�ŏ��̃T�C�Y�@��
	m_Color = INITIALIZE_VECTOR4;
	m_bDeth = false;				//���S�t���O�@��
	m_TimerLogo = TIMERLOGO_MAX;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTimerLogo::~CTimerLogo() {}

//=============================================================================
// �e�N�X�`���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CTimerLogo::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�C���X�^���X�i�}�l�[�W���[�j
	CManager Manager;

	//�f�o�C�X���擾
	pDevice = Manager.GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\game\\game_start000.png", &m_pTexture[0]);			//�X�^�[�g�̉摜
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\game\\time_announce000.png", &m_pTexture[1]);		//2���̉摜
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\game\\time_announce001.png", &m_pTexture[2]);		//1���̉摜
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\game\\time_announce002.png", &m_pTexture[3]);		//30�b�̉摜
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\game\\game_finish000.png", &m_pTexture[4]);			//�t�B�j�b�V���̉摜

	return S_OK;
}
//=============================================================================
// �e�N�X�`���̊J������
//=============================================================================
void CTimerLogo::Unload(void)
{
	for (int nCnt = 0; nCnt < TIMERLOGO_MAX; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTimerLogo::Init(void)
{
	CScene2D::Init();

	//�����l
	m_Color = D3DXCOLOR(1.0f, 1.0f,1.0f,1.0f);
	SetColoer(m_Color);

	if (m_TimerLogo == TIMERLOGO_2MIN || m_TimerLogo == TIMERLOGO_1MIN || m_TimerLogo == TIMERLOGO_30SEC)
	{
		Set(m_pos, m_FirstSize);
	}
	else
	{
		Set(m_pos, m_Size);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTimerLogo::Uninit()
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTimerLogo::Update()
{//��
	if (m_TimerLogo == TIMERLOGO_2MIN || m_TimerLogo == TIMERLOGO_1MIN || m_TimerLogo == TIMERLOGO_30SEC)
	{//FIN�e�N�X�`������Ȃ�������

		//�ϐ��錾
		bool m_SetSizeX = false;
		bool m_SetSizeY = false;

		//�k������
		m_FirstSize.x -= 2.8f;
		m_FirstSize.y -= 2.5f;

		//�l�̏C��
		if (m_FirstSize.x < m_Size.x)
		{
			m_FirstSize.x = m_Size.x;
			m_SetSizeX = true;
		}
		if (m_FirstSize.y < m_Size.y)
		{
			m_FirstSize.y = m_Size.y;
			m_SetSizeY = true;
		}

		//�l��ݒ肷��
		Set(m_pos, m_FirstSize);

		if (m_SetSizeX && m_SetSizeY)
		{
			m_Color.a -= 0.01f;

			if (m_Color.a < 0.0f) { m_bDeth = true; }
		}

		SetColoer(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Color.a));
	}
	else if (m_TimerLogo == TIMERLOGO_START)
	{//�F�̐ݒ�
		
		m_Color.a -= 0.01f;
		SetColoer(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Color.a));
		if (m_Color.a < 0.0f) { m_bDeth = true; }
	}
	else if(m_TimerLogo == TIMERLOGO_FIN)
	{//�F�̐ݒ�
		SetColoer(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	//���S�t���O���L����������
	if (m_bDeth) { Uninit(); }
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTimerLogo::Draw()
{
	CScene2D::Draw();
}