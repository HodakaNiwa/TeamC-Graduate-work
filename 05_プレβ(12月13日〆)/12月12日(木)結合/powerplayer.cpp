//=============================================================================
//
// �p���[�^�̏��� [powerplayer.cpp]
// Author : �R���֎j
//
//=============================================================================
#include "player.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "loadEffect.h"
#include "collision.h"
#include "model.h"
#include "rawmouse.h"
#include "UI.h"
#include "skilicon.h"
#include "effectManager.h"
#include "debuglog.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
#define WAVE (1)
#define RECASTTIME (5)		//5
#define ROT_COUNTER (20)
//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CPowerPlayer::CPowerPlayer()
{
	m_nColliderCnt = 0;
	m_nColliderTimer = 0;
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_nRotCounter = 0;
	m_bAction = false;
	m_bRecast = false;
	m_bInit = false;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CPowerPlayer::~CPowerPlayer()
{

}



//=============================================================================
// ����������
//=============================================================================
HRESULT CPowerPlayer::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer,int nType)
{
	//�v���C���[�̃i���o�[���i�[
	m_nNumPlayer = nNumPlayer;

	//�L�����N�^�[�̏�����
	CCharacter::Init(nNumPlayer,ModelTxt, MotionTxt, m_CharcterType,nType);
	CCharacter::SetPos(pos);

	//���f���̎擾
	m_pModel = CCharacter::GetModel();  

	//���C���ϐ��̏�����
	m_nMaxLineTime = FIRST_LINETIME;
	m_nLineTime = m_nMaxLineTime;
	m_bBlockStartTerritory = false;
	m_pOldStartTerritory = NULL;
	m_nCountTime = 0;
	m_pOrbitLine = NULL;
	m_bMakeShape = false;
	m_nCntTimeCopyLine = 0;
	m_pLoadEffect = NULL;

	//�R�s�[���C���̏�����
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		m_apCopyLine[nCnt] = NULL;
	}
	//�G�t�F�N�g�̏�����
	if (m_pLoadEffect == NULL)
	{
		m_pLoadEffect = CLoadEffect::Create(1, D3DXVECTOR3(pos.x, pos.y + 20.0f, pos.z), 5);
	}
	//���[�V�����̏����擾
	m_pMotion = CCharacter::GetMotion();
	ResetLine();	//���C���̏�����

	//������
	m_fSpeed = 1.0f;
	m_bWalk = true;
	m_nColliderCnt = 0;
	m_nColliderTimer = 0;
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_bAction = false;
	m_bRecast = false;
	m_bCollision = false;
	m_nControllerIndx = 0;
	m_nControllerType = 0;
	m_bSuperArmor = false;
	m_nInitCnt = 0;
	m_fInertia = GetInertia();			//����
	m_Angle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nRotCounter = 0;

	return S_OK;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPowerPlayer::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void  CPowerPlayer::Uninit(void)
{
	//�v���C���[�̔j��
	CPlayer::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void  CPowerPlayer::Update(void)
{
	CGame *pGame = CManager::GetGame();				// �Q�[���̎擾
	CEventCamera *pEveCam = pGame->GetEveCam();		// �C�x���g�J�����̎擾
	
	//�Q�[���̏�Ԃ��擾
	int nGameState = CGame::GetGameState();

	if (nGameState != CGame::GAMESTATE_FIRSTCOUNTDOWN && nGameState != CGame::GAMESTATE_END)
	{
		if (pEveCam == NULL)	// �C�x���g�J�����������Ă�����
		{
			//�A�N�V�����̍X�V
			ActionUpdate();
		}
	}
	//�v���C���[�̍X�V
	CPlayer::Update();
}

//=============================================================================
// �A�N�V�����̍X�V����
//=============================================================================
void  CPowerPlayer::ActionUpdate(void)
{
	//�T�E���h�̎擾
	CSound *pSound = CManager::GetSound();

	if (m_nControllerType == 0)
	{
		//�R���g���[���[����
		PlayerActionPad();
	}
	else if (m_nControllerType == 1)
	{
		//�}�E�X����
		PlayerActionMouse();
	}
	else
	{
		//�L�[�{�[�h����
		PlayerActionKeyboard();
	}
	
	//�A�N�V������
	if (m_bAction == true)
	{
		pSound->StopSound(CSound::SOUND_LABEL_SE018);	//����
		m_nColliderCnt++;
		if (m_nColliderCnt % 60 == 0)
		{
			m_nColliderTimer++;
			if (m_nColliderTimer >= WAVE)
			{//

				if (m_PlayerState != PLAYERSTATE_DAMAGE && m_nColliderTimer == 1)
				{
					pSound->PlaySound(CSound::SOUND_LABEL_SE022);
					pSound->SetVolume(CSound::SOUND_LABEL_SE022, 20.0f);
					CreateColliderSphere();	//�Ռ��g�̓����蔻���t�^
				}
				if (m_nColliderTimer == 2)
				{
					m_bSuperArmor = false;
					m_bInit = true;
				}
			}
		}
	}

	if (m_bInit == true)
	{
		//�l�̏�����
		InitNum();
	}

	//���L���X�g��
	if (m_bRecast == true)
	{
		m_nRecastCounter++;
		if (m_nRecastCounter == (60 * RECASTTIME))
		{
			m_bRecast = false;					//���L���X�g�I��
			m_bAction = false;					//�A�N�V�������g�p�ł���
			m_nButtonCounter = 0;				//�A�N�V�����{�^�����g�p�ł���
			m_nRecastCounter = 0;
		}
	}
}

//=============================================================================
// �l�̏���������
//=============================================================================
void CPowerPlayer::InitNum(void)
{
	if (m_PlayerState != PLAYERSTATE_DAMAGE)
	{
		m_PlayerState = PLAYERSTATE_NONE;
	}

	m_nInitCnt = 0;
	m_nColliderTimer = 0;	//�^�C�}�[��������
	m_nColliderCnt = 0;
	m_bRecast = true;		//���L���X�g���ɂ���	
	m_bAction = false;		//�A�N�V�������I��
	m_bInit = false;
	m_nRotCounter = 0;

	if (m_bAction == false)
	{//�A�N�V�������I�������X�[�p�[�A�[�}������
	}

	CDebugProc::Print("������ : %d\n", m_nInitCnt);
}

//=============================================================================
// PAD����
//=============================================================================
void  CPowerPlayer::PlayerActionPad(void)
{
	CGame *pGame = CManager::GetGame();
	CUI *pUi = pGame->GetUI();
	CInputXPad * pXPad = CManager::GetXPad();
	CGameCamera *pGameCamera = pGame->GetGameCamera(0);				//�Q�[���J�����̎擾
	D3DXVECTOR3 rot = pGameCamera->GetRot();						//�����̎擾
	if (pXPad->GetTrigger(XINPUT_GAMEPAD_B, m_nControllerIndx) == true)
	{
		if (m_bRecast == false && m_nButtonCounter == 0 
			&& m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_BLOWAWAY)
		{
			m_bAction = true;								//�A�N�V�������ɂ���
			m_nButtonCounter = 1;							//�A�N�V�����{�^�����g�p�ł��Ȃ��悤�ɂ���
			m_bCollision = true;
			pUi->GetSkilicon(m_nNumPlayer)->RevivalIconMask();
			m_PlayerState = PLAYERSTATE_ACTION;
			m_pMotion->SetNumMotion(PLAYERSTATE_ACTION);	//�U�����[�V����
			m_bSuperArmor = true;
		}
	}

	//�R���g���[���[�iXInput�j
	float fRot = pXPad->GetStickRot(0, m_nControllerIndx);				//�X�e�B�b�N�̎擾
	if (pXPad->GetStick(0, m_nControllerIndx) == true)
	{//�A�N�V�������Ɍ�����ς����
		if (m_bAction == true)
		{
			m_nRotCounter++;
			if (m_nRotCounter <= ROT_COUNTER)
			{
				m_Angle.y = rot.y + fRot;
			}
		}
	}
}

//=============================================================================
// MOUSE����
//=============================================================================
void  CPowerPlayer::PlayerActionMouse(void)
{
	CGame *pGame = CManager::GetGame();
	CUI *pUi = pGame->GetUI();
	CRawMouse *pRawMouse = CManager::GetRawMouse();					//RawMouse�̎擾
	CGameCamera *pGameCamera = pGame->GetGameCamera(0);				//�Q�[���J�����̎擾
	D3DXVECTOR3 rot = pGameCamera->GetRot();						//�����̎擾
	//�T�E���h�̎擾
	CSound *pSound = CManager::GetSound();

	//�}�E�X�̊p�x���擾
	int MouseX = pRawMouse->GetMouse(m_nControllerIndx).lLastX;
	int MouseY = pRawMouse->GetMouse(m_nControllerIndx).lLastY;
	//�g���b�N�{�[���p
	float Mouse = atan2f((float)MouseX * -1.0f, (float)MouseY * -1.0f);

	if (pRawMouse->GetTrigger(m_nControllerIndx, CRawMouse::RIMS_BUTTON_RIGHT) == true)
	{
		if (m_bRecast == false && m_nButtonCounter == 0  
			&& m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_BLOWAWAY)
		{
			m_bAction = true;								//�A�N�V�������ɂ���
			m_nButtonCounter = 1;							//�A�N�V�����{�^�����g�p�ł��Ȃ��悤�ɂ���
			m_bCollision = true;
			pUi->GetSkilicon(m_nNumPlayer)->RevivalIconMask();
			m_PlayerState = PLAYERSTATE_ACTION;
			m_pMotion->SetNumMotion(PLAYERSTATE_ACTION);	//�U�����[�V����
			m_bSuperArmor = true;
		}
	}

	if (MouseX != 0 || MouseY != 0)
	{//�}�E�X
		if (m_bAction == true)
		{//�A�N�V�������Ɍ�����ς����
			m_nRotCounter++;
			if (m_nRotCounter <= ROT_COUNTER)
			{
				m_Angle.y = rot.y + Mouse;
			}
		}
	}
}

//=============================================================================
// MOUSE����
//=============================================================================
void  CPowerPlayer::PlayerActionKeyboard(void)
{
	CGame *pGame = CManager::GetGame();
	CUI *pUi = pGame->GetUI();
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//�L�[�{�[�h�̎擾
	CGameCamera *pGameCamera = pGame->GetGameCamera(0);				//�Q�[���J�����̎擾
	D3DXVECTOR3 rot = pGameCamera->GetRot();						//�����̎擾

	if (pInputKeyboard->GetKeyboardPress(DIK_RETURN) == true)
	{
		if (m_bRecast == false && m_nButtonCounter == 0
			&& m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_BLOWAWAY)
		{
			m_bAction = true;								//�A�N�V�������ɂ���
			m_nButtonCounter = 1;							//�A�N�V�����{�^�����g�p�ł��Ȃ��悤�ɂ���
			m_bCollision = true;
			pUi->GetSkilicon(m_nNumPlayer)->RevivalIconMask();
			m_PlayerState = PLAYERSTATE_ACTION;
			m_pMotion->SetNumMotion(PLAYERSTATE_ACTION);	//�U�����[�V����
			m_bSuperArmor = true;
		}
	}

	if (m_bAction == true)
	{//�A�N�V�������Ɍ�����ς����
		m_nRotCounter++;
		if (m_nRotCounter <= ROT_COUNTER)
		{
			//�v���C���[�̈ړ������i�L�[�{�[�h�j
			if (pInputKeyboard->GetKeyboardPress(DIK_A) == true)
			{
				if (pInputKeyboard->GetKeyboardPress(DIK_W) == true)
				{//�|���S��������Ɉړ�
					m_Angle.y = rot.y - (D3DX_PI * 0.25f);
				}
				else if (pInputKeyboard->GetKeyboardPress(DIK_S) == true)
				{//�|���S���������Ɉړ�

					m_Angle.y = rot.y - (D3DX_PI * 0.75f);
				}
				else
				{//�|���S�������Ɉړ�
					m_Angle.y = rot.y - (D3DX_PI * 0.5f);
				}
			}
			else if (pInputKeyboard->GetKeyboardPress(DIK_D) == true)
			{

				if (pInputKeyboard->GetKeyboardPress(DIK_W) == true)
				{//�|���S�����E��Ɉړ�

					m_Angle.y = rot.y + (D3DX_PI * 0.25f);
				}
				else if (pInputKeyboard->GetKeyboardPress(DIK_S) == true)
				{//�|���S�����E���Ɉړ�

					m_Angle.y = rot.y + (D3DX_PI * 0.75f);
				}
				else
				{//�|���S�����E�Ɉړ�

					m_Angle.y = rot.y + (D3DX_PI * 0.5f);
				}
			}
			else if (pInputKeyboard->GetKeyboardPress(DIK_W) == true)
			{
				m_Angle.y = rot.y;
			}
			else if (pInputKeyboard->GetKeyboardPress(DIK_S) == true)
			{
				m_Angle.y = D3DX_PI + rot.y;
			}
		}
	}
}

//=============================================================================
// �R���g���[���[�̌����̏�������
//=============================================================================
void  CPowerPlayer::RotAction(int nType)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void  CPowerPlayer::Draw(void)
{
	//�v���C���[�̕`��
	CPlayer::Draw();
}

//=============================================================================
//�I�u�W�F�N�g�̐�������
//=============================================================================
CPowerPlayer *CPowerPlayer::Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, int nType, 
	CHARCTERTYPE charctertype, int nControllerIndx, int nControllerType)
{
	CPowerPlayer *pPowerplayer = NULL;
	if (pPowerplayer == NULL)
	{
		//�������𓮓I�m��
		pPowerplayer = new CPowerPlayer;
		if (pPowerplayer != NULL)
		{//NULL�`�F�b�N
			pPowerplayer->SetType(nType);									//�Q�[���p���^�C�v
			pPowerplayer->m_nCuntry[nNumPlayer] = nType;					//���U���g�p�̍��^�C�v
			pPowerplayer->m_CharcterType = charctertype;					//�Q�[���p�̃L�����^�C�v
			pPowerplayer->m_CharcterTypeResult[nNumPlayer] = charctertype;	//���U���g�p�̃L�����^�C�v
			pPowerplayer->Init(pos, ModelTxt, MotionTxt, nNumPlayer,nType);		//�v���C���[�̏�����
			pPowerplayer->m_nControllerIndx = nControllerIndx;				//�g�p���Ă�R���g���[���[�̔ԍ�
			pPowerplayer->m_nControllerType = nControllerType;				//�g�p���Ă�R���g���[���[�̃^�C�v
		}
		else
		{
			MessageBox(0, "�v���C���[��NULL�ł���", "�x��", MB_OK);
		}
	}
	else
	{
		MessageBox(0, "�v���C���[��NULL����Ȃ��ł�", "�x��", MB_OK);
	}
	return pPowerplayer;
}

//=============================================================================
//
//=============================================================================
void  CPowerPlayer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{

}

//=============================================================================
//    �Ռ��g�̔���𐶐����鏈��
//=============================================================================
void CPowerPlayer::CreateColliderSphere(void)
{
	//�Q�[���̎擾
	CGame *pGame = CManager::GetGame();
	//�G�t�F�N�g
	CEffectManager *pEffectManager = pGame->GetEffectManager();

	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVec3TransformCoord(&pos, &m_pModel[15]->GetPos(), &m_pModel[15]->GetMtxWorld());

	// �X�t�B�A�𐶐�
	CPlayerAttackSphereCollider *pSphere = CPlayerAttackSphereCollider::Create(D3DXVECTOR3(pos.x + sinf(m_rot.y - D3DX_PI) * 100,pos.y,pos.z + cosf(m_rot.y - D3DX_PI) * 100),
		D3DXVECTOR3(1.0f, 1.0f, 1.0f),150.0f, 40, 1);

	D3DXVECTOR3 EffectPos = D3DXVECTOR3(pos.x + sinf(m_rot.y - D3DX_PI) * 100, pos.y - 30.0f, pos.z + cosf(m_rot.y - D3DX_PI) * 100);

	pEffectManager->SetEffect(EffectPos, INITIALIZE_VECTOR3, 1);
	pEffectManager->SetEffect(EffectPos, INITIALIZE_VECTOR3, 2);
	pEffectManager->SetEffect(EffectPos, INITIALIZE_VECTOR3, 3);

	if (pSphere == NULL) { return; }

	// �e��ݒ�
	pSphere->SetParent(this);

}
