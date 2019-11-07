//=============================================================================
//
// �Z���N�g�̏��� [select.cpp]
// Author : ���� ����
//
//=============================================================================
#include "select.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "loadText.h"
#include "camera.h"
#include "loadText.h"
#include "fieldmanager.h"
#include "playericon.h"
#include "moveUI.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define LOAD_UI				("data\\TEXT\\UI\\Select.txt")
#define LOAD_OBJ			("data/TEXT/Stage.txt")
#define LOAD_TERRITORY		("data/TEXT/Territory.txt")
#define FLAG_JAPAN			("data/TEXTURE/Select/NationalFlag/Japan.png")
#define FLAG_RUSSIA			("data/TEXTURE/Select/NationalFlag/Russia.png")
#define FLAG_AMERICA		("data/TEXTURE/Select/NationalFlag/America.png")
#define FLAG_BRAZIL			("data/TEXTURE/Select/NationalFlag/Brazil.png")
#define FLAG_ENGLAND		("data/TEXTURE/Select/NationalFlag/Engrand.png")
#define FLAG_ITALY			("data/TEXTURE/Select/NationalFlag/Italy.png")
#define FLAG_NEWZEALAND		("data/TEXTURE/Select/NationalFlag/Newzealand.png")
#define FLAG_SOUTHAFRICA	("data/TEXTURE/Select/NationalFlag/SouthAfrica.png")
#define SELECT_1P			("data/TEXTURE/Select/NumPlayer/1P.png")
#define SELECT_2P			("data/TEXTURE/Select/NumPlayer/2P.png")
#define SELECT_3P			("data/TEXTURE/Select/NumPlayer/3P.png")
#define SELECT_4P			("data/TEXTURE/Select/NumPlayer/4P.png")
#define NAME_JAPAN			("data/TEXTURE/Select/CountryName/JPN.png")
#define NAME_RUSSIA			("data/TEXTURE/Select/CountryName/RUS.png")
#define NAME_AMERICA		("data/TEXTURE/Select/CountryName/USA.png")
#define NAME_BRAZIL			("data/TEXTURE/Select/CountryName/BRA.png")
#define NAME_ENGLAND		("data/TEXTURE/Select/CountryName/GBR.png")
#define NAME_ITALY			("data/TEXTURE/Select/CountryName/ITA.png")
#define NAME_NEWZEALAND		("data/TEXTURE/Select/CountryName/NGL.png")
#define NAME_SOUTHAFRICA	("data/TEXTURE/Select/CountryName/ZAF.png")
#define NO_ENTRY			("data/TEXTURE/Select/NoEntry.png")
#define LOGO				("data/TEXTURE/Select/CountrySelect_Title.png")
#define FLAM				("data/TEXTURE/Select/select_window.png")
#define FLAG_SIZE			(D3DXVECTOR3(100.0f, 70.0f, 0.0f))
#define PLAYER_SIZE			(D3DXVECTOR3(130.0f, 75.0f, 0.0f))
#define NAME_SIZE			(D3DXVECTOR3(80.0f, 20.0f, 0.0f))
#define NAME_POS			(D3DXVECTOR3(150.0f, 527.0f, 0.0f))
#define NAME_MOVE			(320.0f)
#define MOVE_X				(280.0f)
#define MOVE_Y				(200.0f)
#define NOENTRY_SIZE		(D3DXVECTOR3(150.0f, 100.0f, 0.0f))
#define NOENTRY_POS			(D3DXVECTOR3(170.0f, 605.0f, 0.0f))
#define NOENTRY_MOVE		(315.0f)


//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CMoveUI * CSelect::m_pNationalFlag[MAX_COUNTRY] = {};
int CSelect::m_nEntryPlayer = 0;

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CSelect::CSelect() {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSelect::~CSelect() {}

//=============================================================================
// ����������
//=============================================================================
void CSelect::Init(void)
{
	//�x�[�X�̏�����
	CModeBase::Init();

	//�ϐ��̏�����
	m_pCamera = NULL;
	m_pFieldManager = NULL;
	m_pLoadTextObj = NULL;
	m_pLoadTerritory = NULL;
	m_pTexNoEntry = NULL;
	m_pLogo = NULL;
	m_nEntryPlayer = 0;
	m_state = STATE_CUNTRY_SELECT;

	for (int nCnt = 0; nCnt < MAX_COUNTRY; nCnt++)
	{
		m_apTextureCountry[nCnt] = NULL;	//�����e�N�X�`��
		m_apTexCuntoryName[nCnt] = NULL;	//�����e�N�X�`��
		m_pNationalFlag[nCnt] = NULL;		//�����|�C���^
	}

	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		m_apTexPlayer[nCnt] = NULL;	//�v���C���[�A�C�R���e�N�X�`��
		m_pNumPlayer[nCnt] = NULL;	//�v���C���[�A�C�R���|�C���^
		m_pNoEntry[nCnt] = NULL;	//�G���g���[���Ă��邩�ǂ���
		m_pFram[nCnt] = NULL;		//�t���[��
	}

	for (int nCnt = 0; nCnt < SELECT_TEX; nCnt++)
	{
		m_pTex[nCnt] = NULL;
	}

	//�e�N�X�`���̓ǂݍ��ݏ���
	LoadTexture();

	//�J�����̐���
	if (m_pCamera == NULL)
	{
		D3DVIEWPORT9 viewport;
		viewport.X = 0;		//�r���[�|�[�g�̍���X���W
		viewport.Y = 0;		//�r���[�|�[�g�̍���Y���W
		viewport.Width = SCREEN_WIDTH;	//��
		viewport.Height = SCREEN_HEIGHT;//����
		viewport.MaxZ = 1.0f;
		viewport.MinZ = 0.0f;

		m_pCamera = new CCamera;
		m_pCamera->Init();
		m_pCamera->SetViewport(viewport);
		m_pCamera->SetPosV(D3DXVECTOR3(14.0f, 1000.0f, -600.0f));
		m_pCamera->SetPosR(D3DXVECTOR3(900.0f, 255.0f, 600.0f));
	}

	//�n�ʂ̐���
	if (m_pFieldManager == NULL)
	{
		m_pFieldManager = new CFieldManager;
		m_pFieldManager->Init();
	}

	if (m_pLoadTextObj == NULL) { m_pLoadTextObj = CLoadTextObject::Create(LOAD_OBJ); }					//�I�u�W�F�N�g�̐���
	if (m_pLoadTerritory == NULL) { m_pLoadTerritory = CLoadTextTerritory::Create(LOAD_TERRITORY); }	//�e���g���[�̓ǂݍ���

	//���I�����[�h��UI��������
	InitCuntrySelect();
}

//=============================================================================
// �I������
//=============================================================================
void CSelect::Uninit(void)
{
	//�x�[�X�̔j��
	CModeBase::Uninit();

	//�J�����̔j��
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	//�n�ʃ}�l�[�W���[�̔j��
	if (m_pFieldManager != NULL)
	{
		m_pFieldManager->Uninit();
		delete m_pFieldManager;
		m_pFieldManager = NULL;
	}

	//�I�u�W�F�N�g�̔j��
	if (m_pLoadTextObj != NULL)
	{
		m_pLoadTextObj->Uninit();
		delete m_pLoadTextObj;
		m_pLoadTextObj = NULL;
	}

	//�e���g���[�̔j��
	if (m_pLoadTerritory != NULL)
	{
		m_pLoadTerritory->Uninit();
		delete m_pLoadTerritory;
		m_pLoadTerritory = NULL;
	}

	//�e�N�X�`���̔j��
	UnloadTexture();

	//�t�F�[�h�ȊO�폜
	CScene::ReleaseFade();
}

//=============================================================================
// �X�V����
//=============================================================================
void CSelect::Update(void)
{
	//�T�E���h�̎擾
	CSound *pSound = CManager::GetSound();

	//�C���v�b�g�̎擾�@
	CGamePad	*pGamePad = CManager::GetInputGamePad();
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();
	CInputXPad * pXPad = CManager::GetXPad();

	if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
	{//�Z���N�g����Q�[����
		
		if (CFade::GetFadeMode() != CFade::FADE_OUT) 
		{ 
			//���艹
			pSound->PlaySound(CSound::SOUND_LABEL_SE007);
			CFade::SetFade(CManager::MODE_GAME); 
		}
	}

	//�����̃e�N�X�`���̐؂�ւ�
	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		if (m_pCuntoryName[nCnt] != NULL)
		{
			m_pCuntoryName[nCnt]->BindTexture(m_apTexCuntoryName[m_pNumPlayer[nCnt]->GetNumCuntry()]);
		}
	}

	//��Ԃ��Ƃ̍X�V����
	switch (m_state)
	{
	case STATE_CUNTRY_SELECT: UpdateCuntrySelect(); break;			//���I��
	case STATE_CHARACTER_SELECT: UpdateCharacterSelect(); break;	//�L�����I��
	case STATE_CHANGE: UpdateChange(); break;						//���[�h�ؑ�
	case STATE_DICISION: UpdateDicision(); break;					//����
	}

	//pSound->SetVolume(CSound::SOUND_LABEL_SE007, 10.0f);	//�{�����[���̐ݒ�
}

//=============================================================================
// �`�揈��
//=============================================================================
void CSelect::Draw(void)
{
	//�J�����̐ݒ�
	if (m_pCamera != NULL) { m_pCamera->Set(); }

	//���ׂĕ`�悷��
	CScene::DrawAll();
}

//=============================================================================
// ���I���̏���������
//=============================================================================
void CSelect::InitCuntrySelect(void)
{
	//���S�̐ݒ�
	if (m_pLogo == NULL) { m_pLogo = CMoveUI::Create(D3DXVECTOR3(640.0f, 50.0f, 0.0f), D3DXVECTOR3(640.0f, 50.0f, 0.0f), m_pTex[0]); }

	//�v���C���[�̑I��\������
	for (int nCnt = NUMPLAYER - 1; nCnt >= 0; nCnt--)
	{
		m_pNumPlayer[nCnt] = CPlayerIcon::Create(D3DXVECTOR3(255.0f, 200.0f, 0.0f), nCnt);
		m_pNumPlayer[nCnt]->BindTexture(m_apTexPlayer[nCnt]);
		m_pNumPlayer[nCnt]->SetDraw(false);
	}
	
	//�����̕\��
	int nCountFlag = 0;;
	for (int nCntY = 0; nCntY < 2; nCntY++)
	{
		for (int nCntX = 0; nCntX < 4; nCntX++)
		{
			m_pNationalFlag[nCountFlag] = CMoveUI::Create(D3DXVECTOR3(230.0f + (nCntX * MOVE_X), 200.0f + (nCntY * MOVE_Y), 0.0f), FLAG_SIZE, m_apTextureCountry[nCountFlag]);
			nCountFlag++;
		}
	}

	//�t���[���E�G���g���[���Ă��Ȃ��e�N�X�`���̕\��
	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		if (m_pFram[nCnt] == NULL)
		{
			m_pFram[nCnt] = CMoveUI::Create(D3DXVECTOR3(NOENTRY_POS.x + (nCnt * NOENTRY_MOVE), NOENTRY_POS.y, 0.0f), NOENTRY_SIZE, m_pTex[1]);
		}

		if (m_pNoEntry[nCnt] == NULL)
		{
			m_pNoEntry[nCnt] = CMoveUI::Create(D3DXVECTOR3(NOENTRY_POS.x + (nCnt * NOENTRY_MOVE), NOENTRY_POS.y, 0.0f), NOENTRY_SIZE, m_pTexNoEntry);
		}
	}
}

//=============================================================================
// ���I���̍X�V����
//=============================================================================
void CSelect::UpdateCuntrySelect(void)
{
	//�T�E���h�̎擾
	CSound *pSound = CManager::GetSound();

	CGamePad * pGamePad = CManager::GetInputGamePad();	//�Q�[���p�b�h�̎擾
	CInputXPad * pXPad = CManager::GetXPad();

	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		if (pXPad->GetTrigger(XINPUT_GAMEPAD_A, nCnt) == true)
		{
			if (CFade::FADE_OUT != CFade::GetFadeMode())
			{//�t�F�[�h�A�E�g�̎�����Ȃ������特��炷
				//���艹
				pSound->PlaySound(CSound::SOUND_LABEL_SE007);
				//pSound->SetVolume(CSound::SOUND_LABEL_SE007, 10.0f);	//�{�����[���̐ݒ�
			}
			m_pNumPlayer[nCnt]->SetDraw(true);

			//�����̐���
			if (m_pCuntoryName[nCnt] == NULL)
			{
				m_pCuntoryName[nCnt] = CMoveUI::Create(D3DXVECTOR3(NAME_POS.x + (nCnt * NAME_MOVE), NAME_POS.y, 0.0f), NAME_SIZE, m_apTexCuntoryName[m_pNumPlayer[nCnt]->GetNumCuntry()]);

				//�G���g���[���̉��Z
				m_nEntryPlayer++;

				//NO ENTRY�̔j��
				if (m_pNoEntry[nCnt] != NULL)
				{
					m_pNoEntry[nCnt]->Uninit();
					m_pNoEntry[nCnt] = NULL;
				}
			}
		}
	}
}

//=============================================================================
// �L�����I���̏���������
//=============================================================================
void CSelect::InitCharacterSelect(void)
{

}

//=============================================================================
// �L�����I���̍X�V����
//=============================================================================
void CSelect::UpdateCharacterSelect(void)
{

}

//=============================================================================
// �L�����I���̔j������
//=============================================================================
void CSelect::UninitCharacterSelect(void)
{

}

//=============================================================================
// ���[�h�؂�ւ�����
//=============================================================================
void CSelect::UpdateChange(void)
{

}

//=============================================================================
// ���菈��
//=============================================================================
void CSelect::UpdateDicision(void)
{

}

//=============================================================================
// �e�N�X�`���̓ǂݍ��ݏ���
//=============================================================================
void CSelect::LoadTexture(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�����̃e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, FLAG_JAPAN, &m_apTextureCountry[0]);			//���{
	D3DXCreateTextureFromFile(pDevice, FLAG_RUSSIA, &m_apTextureCountry[1]);		//���V�A
	D3DXCreateTextureFromFile(pDevice, FLAG_AMERICA, &m_apTextureCountry[2]);		//�A�����J
	D3DXCreateTextureFromFile(pDevice, FLAG_BRAZIL, &m_apTextureCountry[3]);		//�u���W��
	D3DXCreateTextureFromFile(pDevice, FLAG_ENGLAND, &m_apTextureCountry[4]);		//�C�M���X
	D3DXCreateTextureFromFile(pDevice, FLAG_ITALY, &m_apTextureCountry[5]);			//�C�^���A
	D3DXCreateTextureFromFile(pDevice, FLAG_NEWZEALAND, &m_apTextureCountry[6]);	//�j���[�W�[�����h
	D3DXCreateTextureFromFile(pDevice, FLAG_SOUTHAFRICA, &m_apTextureCountry[7]);	//��A�t���J

																					//�v���C���[�̑I���摜
	D3DXCreateTextureFromFile(pDevice, SELECT_1P, &m_apTexPlayer[0]);	//1P
	D3DXCreateTextureFromFile(pDevice, SELECT_2P, &m_apTexPlayer[1]);	//2P
	D3DXCreateTextureFromFile(pDevice, SELECT_3P, &m_apTexPlayer[2]);	//3P
	D3DXCreateTextureFromFile(pDevice, SELECT_4P, &m_apTexPlayer[3]);	//4P

	//�����̓ǂݍ��ݏ���
	D3DXCreateTextureFromFile(pDevice, NAME_JAPAN, &m_apTexCuntoryName[0]);			//���{
	D3DXCreateTextureFromFile(pDevice, NAME_RUSSIA, &m_apTexCuntoryName[1]);		//���V�A
	D3DXCreateTextureFromFile(pDevice, NAME_AMERICA, &m_apTexCuntoryName[2]);		//�A�����J
	D3DXCreateTextureFromFile(pDevice, NAME_BRAZIL, &m_apTexCuntoryName[3]);		//�u���W��
	D3DXCreateTextureFromFile(pDevice, NAME_ENGLAND, &m_apTexCuntoryName[4]);		//�C�M���X
	D3DXCreateTextureFromFile(pDevice, NAME_ITALY, &m_apTexCuntoryName[5]);			//�C�^���A
	D3DXCreateTextureFromFile(pDevice, NAME_NEWZEALAND, &m_apTexCuntoryName[6]);	//�j���[�W�[�����h
	D3DXCreateTextureFromFile(pDevice, NAME_SOUTHAFRICA, &m_apTexCuntoryName[7]);	//��A�t���J

	//�G���g���[���Ă��Ȃ��e�N�X�`���̓ǂݍ��ݏ���
	D3DXCreateTextureFromFile(pDevice, NO_ENTRY, &m_pTexNoEntry);			//���{

																			//���̑��̃e�N�X�`��
	D3DXCreateTextureFromFile(pDevice, LOGO, &m_pTex[0]);	//���S
	D3DXCreateTextureFromFile(pDevice, FLAM, &m_pTex[1]);	//�t���[��
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CSelect::UnloadTexture(void)
{
	//�����e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < MAX_COUNTRY; nCnt++)
	{
		if (m_apTextureCountry[nCnt] != NULL)
		{
			m_apTextureCountry[nCnt]->Release();
			m_apTextureCountry[nCnt] = NULL;
		}
	}

	//�v���C���[�̃A�C�R��
	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		if (m_apTexPlayer[nCnt] != NULL)
		{
			m_apTexPlayer[nCnt]->Release();
			m_apTexPlayer[nCnt] = NULL;
		}
	}

	//�����e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < MAX_COUNTRY; nCnt++)
	{
		if (m_apTexCuntoryName[nCnt] != NULL)
		{
			m_apTexCuntoryName[nCnt]->Release();
			m_apTexCuntoryName[nCnt] = NULL;
		}
	}

	//�G���g���[���Ă��Ă��Ȃ�
	if (m_pTexNoEntry != NULL)
	{
		m_pTexNoEntry->Release();
		m_pTexNoEntry = NULL;
	}

	//���S�e�N�X�`��
	for (int nCnt = 0; nCnt < SELECT_TEX; nCnt++)
	{
		if (m_pTex[nCnt] != NULL)
		{
			m_pTex[nCnt]->Release();
			m_pTex[nCnt] = NULL;
		}
	}
}