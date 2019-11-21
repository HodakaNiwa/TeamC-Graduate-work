//=============================================================================
//
// �p���[�^�̏��� [power.cpp]
// Author : Yamashita
//
//=============================================================================
#include "player.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "inputmouce.h"
#include "loadEffect.h"
#include "collision.h"
#include "model.h"
#include "rawmouse.h"
#include "UI.h"
#include "skilicon.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
#define WAVE (1)
#define RECASTTIME (10)

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
	m_fScale = 0.0f;
	m_bAction = false;
	m_bRecast = false;
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

	//������
	m_bWalk = true;

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
	m_fSpeed = 1.0f;
	ResetLine();	//���C���̏�����

	m_nColliderCnt = 0;
	m_nColliderTimer = 0;
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_fScale = 0.0f;
	m_bAction = false;
	m_bRecast = false;
	m_bCollision = false;
	m_nControllerIndx = 0;
	m_nControllerType = 0;
	m_bSuperArmor = false;
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
	//�A�N�V�����̍X�V
	ActionUpdate();
	//�v���C���[�̍X�V
	CPlayer::Update();
}

//=============================================================================
// �A�N�V�����̍X�V����
//=============================================================================
void  CPowerPlayer::ActionUpdate(void)
{
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
	
	//�A�N�V������
	if (m_bAction == true)
	{
		m_bSuperArmor = true;
		m_nColliderCnt++;

		if (m_nColliderCnt % 60 == 0)
		{
			m_nColliderTimer++;
			if (m_nColliderTimer >= WAVE)
			{//1�b�o�߂�����Ռ��g���o��

				CreateColliderSphere();	//�Ռ��g�̓����蔻���t�^
				m_nColliderTimer = 0;	//�^�C�}�[��������
				m_nColliderCnt = 0;
				m_bRecast = true;		//���L���X�g���ɂ���	
				m_bAction = false;		//�A�N�V�������I��
				if (m_bAction == false)
				{//�A�N�V�������I�������X�[�p�[�A�[�}������
					m_bSuperArmor = false;
					//m_PlayerState = PLAYERSTATE_NONE;
				}
			}
		}
	}

	//���L���X�g��
	if (m_bRecast == true)
	{
		m_nRecastCounter++;
		if (m_nRecastCounter % 60 == 0)
		{
			m_nRecastTimer++;
			if (m_nRecastTimer <= RECASTTIME)
			{//���L���X�g���̓A�N�V�������g�p�ł��Ȃ�
				m_bRecast = true;
			}
			else
			{//10�b�o�߂�����A�N�V�������g�p�ł���
				m_nRecastTimer = 0;					//�^�C�}�[��������
				m_bRecast = false;					//���L���X�g�I��
				m_bAction = false;					//�A�N�V�������g�p�ł���
				m_nButtonCounter = 0;				//�A�N�V�����{�^�����g�p�ł���
			}
		}
	}
}

//=============================================================================
// PAD����
//=============================================================================
void  CPowerPlayer::PlayerActionPad(void)
{
	CGame *pGame = CManager::GetGame();
	CUI *pUi = pGame->GetUI();
	CInputXPad * pXPad = CManager::GetXPad();


	if (pXPad->GetTrigger(XINPUT_GAMEPAD_B, m_nNumPlayer) == true)
	{
		if (m_bRecast == false && m_nButtonCounter == 0)
		{
			CreateStartUpCollider();	//�n���}�[�ɓ����蔻���t�^����
			m_pMotion->SetNumMotion(2);	//�U�����[�V����
			m_bAction = true;			//�A�N�V�������ɂ���
			m_nButtonCounter = 1;		//�A�N�V�����{�^�����g�p�ł��Ȃ��悤�ɂ���
			m_bCollision = true;
			pUi->GetSkilicon(m_nNumPlayer)->RevivalIconMask();
			m_PlayerState = PLAYERSTATE_ACTION;
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

	m_nNumPlayer;
	if (pRawMouse->GetTrigger(CRawMouse::RIMS_BUTTON_LEFT, m_nNumPlayer) == true)
	{
		if (m_bRecast == false && m_nButtonCounter == 0)
		{
			CreateStartUpCollider();	//�n���}�[�ɓ����蔻���t�^����
			m_pMotion->SetNumMotion(2);	//�U�����[�V����
			m_bAction = true;			//�A�N�V�������ɂ���
			m_nButtonCounter = 1;		//�A�N�V�����{�^�����g�p�ł��Ȃ��悤�ɂ���
			m_bCollision = true;
			pUi->GetSkilicon(m_nNumPlayer)->RevivalIconMask();
			m_PlayerState = PLAYERSTATE_ACTION;

		}
	}
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
//    �n���}�[�̓����蔻��𐶐����鏈��
//=============================================================================
void CPowerPlayer::CreateStartUpCollider(void)
{
	// �~���𐶐�
	CPlayerAttackSphereCollider *pSphere = CPlayerAttackSphereCollider::Create(D3DXVECTOR3(0.0f, 0.0f, -60.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f),
		30.0f, 100,1);

	if (pSphere == NULL) { return; }

	// �e��ݒ�
	pSphere->SetParent(this);

	//����̏ꏊ�ɔ����t����
	pSphere->SetParentMtxWorld(&m_pModel[15]->GetMtxWorld());
}

//=============================================================================
//    �Ռ��g�̔���𐶐����鏈��
//=============================================================================
void CPowerPlayer::CreateColliderSphere(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVec3TransformCoord(&pos, &m_pModel[15]->GetPos(), &m_pModel[15]->GetMtxWorld());

	// �X�t�B�A�𐶐�
	CPlayerAttackSphereCollider *pSphere = CPlayerAttackSphereCollider::Create(D3DXVECTOR3(pos.x,pos.y,pos.z + 35.0f), 
		D3DXVECTOR3(1.0f, 1.0f, 1.0f),100.0f, 100, 1);

	if (pSphere == NULL) { return; }

	// �e��ݒ�
	pSphere->SetParent(this);

}
////=============================================================================
////    �ʏ��Ԃ̍X�V����
////=============================================================================
//void CMine::None(void)
//{
//	// ���������炷
//	m_nLife--;
//	if (m_nLife <= 0)
//	{// �������Ȃ��Ȃ���
//		m_nExplosionTimer = 0;
//		m_nType = TYPE_STARTUP;
//	}
//}

////=============================================================================
////    �N����Ԃ̍X�V����
////=============================================================================
//void CMine::StartUp(void)
//{
//	// �傫������
//	m_fScaleUp += MINE_EXPLOSION_SCALEUP;
//	D3DXVECTOR3 Scale = MINE_SCALE_INI;
//	Scale.x += m_fScaleUp;
//	Scale.y += m_fScaleUp;
//	Scale.z += m_fScaleUp;
//	SetScale(Scale);
//
//	// �J�E���^�[�����܂����甚��
//	m_nExplosionTimer++;
//	if (m_nExplosionTimer >= MINE_EXPLOSION_STARTTIME)
//	{
//		// �J�E���^�[�����������ď�ԕύX
//		m_nExplosionTimer = 0;
//		m_nType = TYPE_EXPLOSION;
//
//		// �����̃G�t�F�N�g�Ɠ����蔻��𐶐�����
//		CreateExplosionEffect();
//		CreateExplosionCollider();
//	}
//}

////=============================================================================
////    �����̃G�t�F�N�g�𐶐����鏈��
////=============================================================================
//void CPowerPlayer::CreateExplosionEffect(void)
//{
//
//}

////=============================================================================
////    �����p�̓����蔻��𐶐����鏈��
////=============================================================================
//void CMine::CreateExplosionCollider(void)
//{
//	// ���𐶐�
//	CPlayerAttackSphereCollider *pShere = CPlayerAttackSphereCollider::Create(GetPos(), D3DXVECTOR3(1.0f, 1.0f, 1.0f),
//		MINE_EXPLOSION_COLRADIUS, MINE_EXPLOSION_TIME + 2, 1);
//	if (pShere == NULL) { return; }
//
//	// �e��ݒ�
//	pShere->SetParent(this);
//
//	// �R���C�_�[�Ǌ��N���X�Ƀ|�C���^��ݒ�
//	CColliderManager *pColManager = GetColloderManger();
//	pColManager->SetCollider(pShere, 1);
//}

////=============================================================================
////    ������Ԃ̍X�V����
////=============================================================================
//void CMine::Explosion(void)
//{
//	// �����̎������؂ꂽ��I��
//	m_nExplosionTimer++;
//	if (m_nExplosionTimer >= MINE_EXPLOSION_TIME)
//	{
//		Uninit();
//	}
//}