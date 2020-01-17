//=============================================================================
//
// �`���[�g���A���p���[�^�̏��� [tutorialpawerplayer.cpp]
// Author : �R���֎j�@&�@��������
//
//=============================================================================
#include "tutorialplayer.h"
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
#include "tutorial.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
#define WAVE (1)
#define RECASTTIME (5)

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CTutorialPowerPlayer::CTutorialPowerPlayer()
{
	m_nColliderCnt = 0;
	m_nColliderTimer = 0;
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_bAction = false;
	m_bRecast = false;
	m_bInit = false;
	m_bUseSkill = false;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CTutorialPowerPlayer::~CTutorialPowerPlayer(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTutorialPowerPlayer::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer,int nType)
{
	//�v���C���[�̃i���o�[���i�[
	m_nNumPlayer = nNumPlayer;

	//�L�����N�^�[�̏�����
	CCharacter::Init(nNumPlayer,ModelTxt, MotionTxt, m_CharcterType,nType);
	CCharacter::SetPos(pos);

	//���f���̎擾
	m_pModel = CCharacter::GetModel();  

	//���C���ϐ��̏�����
	m_bBlockStartTerritory = false;
	m_pOldStartTerritory = NULL;
	m_bMakeShape = false;

	//���[�V�����̏����擾
	m_pMotion = CCharacter::GetMotion();
	ResetLine();	//���C���̏�����

	//������
	m_bWalk = true;
	m_nColliderCnt = 0;
	m_nColliderTimer = 0;
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_bAction = false;
	m_bRecast = false;
	m_bUseSkill = false;

	//�ړI�n�܂ł̃��X�g�ݒ�
	SetAimPos();

	//�ړ����[�g�̐ݒ�
	SetMovePlayer(true, 6);
	return S_OK;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTutorialPowerPlayer::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void  CTutorialPowerPlayer::Uninit(void)
{
	//�v���C���[�̔j��
	CTutorialPlayer::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void  CTutorialPowerPlayer::Update(void)
{
	//���S�t���O���L����������
	if (GetDeth() == true) { return; }

	//�A�N�V�����̍X�V
	ActionUpdate();

	//�v���C���[�̍X�V
	CTutorialPlayer::Update();

#if _DEBUG
	DebugInput();
#endif
}

//=============================================================================
// �A�N�V�����̍X�V����
//=============================================================================
void  CTutorialPowerPlayer::ActionUpdate(void)
{
	//�T�E���h�̎擾
	CSound *pSound = CManager::GetSound();
	
	//�A�N�V������
	if (m_bAction == true)
	{
		m_nColliderCnt++;
		if (m_nColliderCnt % 60 == 0)
		{
			m_nColliderTimer++;
			if (m_nColliderTimer >= WAVE)
			{
				if (m_PlayerState != PLAYERSTATE_DAMAGE && m_nColliderTimer == 1)
				{
					pSound->PlaySound(CSound::SOUND_LABEL_SE022);
					CreateColliderSphere();	//�Ռ��g�̓����蔻���t�^
					m_nColliderTimer = 0;
					m_nColliderCnt = 0;
					m_bAction = false;
				}
			}
		}
	}

	if ((m_bMoveEnd) && (!m_bUseSkill))
	{
		SkillOn();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void  CTutorialPowerPlayer::Draw(void)
{
	//�v���C���[�̕`��
	CTutorialPlayer::Draw();
}

//=============================================================================
//�I�u�W�F�N�g�̐�������
//=============================================================================
CTutorialPowerPlayer *CTutorialPowerPlayer::Create(D3DXVECTOR3 pos, int nNumPlayer, int nType)
{
	CTutorialPowerPlayer *pPowerplayer = NULL;
	if (pPowerplayer == NULL)
	{
		//�������𓮓I�m��
		pPowerplayer = new CTutorialPowerPlayer;
		if (pPowerplayer != NULL)
		{//NULL�`�F�b�N
			pPowerplayer->SetType(nType);											//�Q�[���p���^�C�v
			pPowerplayer->m_nCuntry[nNumPlayer] = nType;							//���U���g�p�̍��^�C�v
			pPowerplayer->m_CharcterType = CCharacter::CHARCTERTYPE_POWER;			//�Q�[���p�̃L�����^�C�v
			pPowerplayer->m_CharcterTypeResult[nNumPlayer] = CHARCTERTYPE_POWER;	//���U���g�p�̃L�����^�C�v
			pPowerplayer->Init(pos, "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt",
							   "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt", nNumPlayer,nType);		//�v���C���[�̏�����
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
void  CTutorialPowerPlayer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size){}

//=============================================================================
//    �Ռ��g�̔���𐶐����鏈��
//=============================================================================
void CTutorialPowerPlayer::CreateColliderSphere(void)
{
	if (GetDeth() == true) { return; }

	//�G�t�F�N�g
	CEffectManager *pEffectManager = CManager::GetTutorial()->GetEffectManager();

	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVec3TransformCoord(&pos, &m_pModel[15]->GetPos(), &m_pModel[15]->GetMtxWorld());

	// �X�t�B�A�𐶐�
	CPlayerAttackSphereCollider *pSphere = CPlayerAttackSphereCollider::Create(D3DXVECTOR3(pos.x + sinf(m_rot.y - D3DX_PI) * 100,pos.y,pos.z + cosf(m_rot.y - D3DX_PI) * 100),
		D3DXVECTOR3(1.0f, 1.0f, 1.0f),150.0f, 40, 1);

	D3DXVECTOR3 EffectPos = D3DXVECTOR3(pos.x + sinf(m_rot.y - D3DX_PI) * 100, pos.y - 30.0f, pos.z + cosf(m_rot.y - D3DX_PI) * 100);

	if (pEffectManager != NULL)
	{
		pEffectManager->SetEffect(EffectPos, INITIALIZE_VECTOR3, 1);
		pEffectManager->SetEffect(EffectPos, INITIALIZE_VECTOR3, 2);
		pEffectManager->SetEffect(EffectPos, INITIALIZE_VECTOR3, 3);
	}

	if (pSphere == NULL) { return; }

	// �e��ݒ�
	pSphere->SetParent(this);
}

//=============================================================================
// �f�o�b�O�p���͏���
//=============================================================================
void CTutorialPowerPlayer::DebugInput(void)
{
	//���̓f�o�C�X�̎擾
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();

	if (pKeyboard->GetKeyboardTrigger(DIK_SPACE) == true)
	{
		SkillOn();
	}
}

//=============================================================================
// �X�L����������
//=============================================================================
void CTutorialPowerPlayer::SkillOn(void)
{
	m_bAction = true;								//�A�N�V�������ɂ���
	m_bUseSkill = true;		//�X�L�����g�p������Ԃɂ���
	m_bMoveEnd = false;		//����������
	m_PlayerState = PLAYERSTATE_ACTION;
	m_pMotion->SetNumMotion(PLAYERSTATE_ACTION);	//�U�����[�V����
}
