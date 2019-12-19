//====================================================================================
//
// ��9��͏���	[robot.cpp]
// Author;�r�J�R�N
//
//====================================================================================
#include "robot.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "collision.h"
#include "model.h"
#include "loadEffect.h"
#include "debuglog.h"
#include "effectManager.h"
#include "robotUI.h"
#include "emitter.h"
#include "bullet.h"
#include "crack.h"
#include "shadow.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CRobot::CRobot(int nPriority, OBJTYPE objType) : CCharacter(nPriority, objType)
{
	m_RobotState		= ROBOTSTATE_DAWN;		// ��������Ԃ�
	m_pos				= INITIALIZE_VECTOR3;
	m_rot				= INITIALIZE_VECTOR3;
	m_move				= INITIALIZE_VECTOR3;
	m_nNumTaget			= 0;
	m_nNumTagetOld		= 0;
	m_nCntEnd			= 0;
	m_nCntWalk			= 0;
	m_nNumAttack		= 0;
	m_nNumShoot			= 0;
	m_nCntShoot			= 0;
	m_fDropTime			= 0.0f;
	m_fJumpTime			= 0.0f;
	m_bDeth				= false;
	m_bEndGravity		= false;
	m_bAttack			= false;
	m_bShoot			= false;
	m_pRobotUI			= NULL;
	m_pEmitter			= NULL;
	m_nCntEmitter		= 0;
	m_nCntAttackEffect	= 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CRobot::~CRobot()
{
}

//=============================================================================
// ��������
//=============================================================================
CRobot *CRobot::Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer, TYPE type, CHARCTERTYPE charctertype)
{
	CRobot *pRobot = NULL;

	if (pRobot == NULL)
	{
		pRobot = new CRobot;

		if (pRobot != NULL)
		{
			pRobot->m_CharcterType = charctertype;
			pRobot->Init(pos, ModelTxt, MotionTxt, nNumPlayer);
		}
		else
		{
			MessageBox(0, "pRobot��NULL�ł���", "�x��", MB_OK);
		}
	}
	else
	{
		MessageBox(0, "pRobot��NULL����Ȃ��ł�", "�x��", MB_OK);
	}
	return pRobot;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CRobot::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int nNumPlayer)
{
	//�v���C���[�̃i���o�[���擾
	m_nNumPlayer = nNumPlayer;

	//�L�����N�^�[�̏�����
	CCharacter::Init(nNumPlayer, ModelTxt, MotionTxt, m_CharcterType, 0);
	CCharacter::SetPos(pos);

	m_pos = pos;	// ���W�̐ݒ�

	//���f���̏����擾
	m_pModel = CCharacter::GetModel();

	//���[�V�����̏����擾
	m_pMotion = CCharacter::GetMotion();
	m_pMotion->SetNumMotion(m_RobotState);

	if (m_pShadow != NULL)
	{
		m_pShadow->Set(m_pShadow->GetPos(),D3DXVECTOR3(150.0f,0.0f,150.0f));
	}

	return S_OK;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CRobot::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void  CRobot::Uninit(void)
{
	CGame *pGame = CManager::GetGame();		// �Q�[���̎擾

	//�L�����N�^�[�̔j��
	CCharacter::Uninit();

	pGame->SetRobot(NULL);
	if (m_pEmitter != NULL) { m_pEmitter = NULL; }

	ReleaseRobotUI();
}

//=============================================================================
// �X�V����
//=============================================================================
void CRobot::Update(void)
{
	CSound *pSound = CManager::GetSound();

	CGame *pGame = CManager::GetGame();		// �Q�[���̎擾
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//�L�[�{�[�h�̎擾

	// �e��Ԃ̏���
	switch (m_RobotState)
	{
	case ROBOTSTATE_NONE:			// �j���[�g����
		Search();	// �{������
		break;
	case ROBOTSTATE_WALK:			// �ړ�
		Wake();		// �ړ�����
		if (m_nNumAttack != ROBOT_SHOOT_MODEINTERVAL){ Attack(); }	// �U������
		break;
	case ROBOTSTATE_APPEARANCE:		// �o��
		GetUp();
		break;
	case ROBOTSTATE_JUMP:			// �W�����v
		Jump();		// �W�����v����
		break;
	case ROBOTSTATE_ATTACK:			// �ߐڍU��
		if (m_nCntAttackEffect == ROBOT_ATTACKEFFECT && m_bAttack == false)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE035);
			CreateColliderSphere();		// �Ռ��g�̐���
			m_bAttack = true;			// �U�����Ă����Ԃ�
		}
		if (m_pMotion->GetNumMotion() == 0)
		{// �j���[�g�����̃��[�V�����������Ƃ�
			m_RobotState = ROBOTSTATE_NONE;			// �j���[�g������Ԃ�
			m_pMotion->SetNumMotion(m_RobotState);	// ���[�V�����̐ݒ�
			m_bAttack			= false;			// �U��(�ߐ�)���Ă��Ȃ���Ԃ�
			m_nCntAttackEffect	= 0;				// �G�t�F�N�g�J�E���^�̃��Z�b�g
			m_nNumAttack++;							// �U���񐔂̉��Z
		}
		else { m_nCntAttackEffect++; }	// �G�t�F�N�g�J�E���^�̉��Z
		break;
	case ROBOTSTATE_SHOOT:			// �ˌ��U��
		Shoot();	// �ˌ�����
		break;
	case ROBOTSTATE_DAWN:
		Gravity();	// ��������
		break;
	}

	if (m_nCntEnd >= ROBOT_ENDTIME && m_RobotState != ROBOTSTATE_JUMP)
	{// �ޏꎞ�ԂɂȂ�����
		m_RobotState = ROBOTSTATE_JUMP;			// �W�����v��Ԃ�
		m_pMotion->SetNumMotion(m_RobotState);	// ���[�V�����̐ݒ�
	}
	m_nCntEnd++;	// �ޏ�J�E���^�[�̉��Z

	CCharacter::Update();		// �L�����N�^�[�̍X�V
	UpdateRobotUI();			// �o���e���b�v�̍X�V
	if (m_pShadow != NULL)
	{
		m_pShadow->SetPos(D3DXVECTOR3(m_pos.x, 0.3f, m_pos.z));
	}
	if (m_bDeth)
	{
		if (m_pShadow != NULL)
		{
			m_pShadow->Uninit();
			m_pShadow = NULL;
		}

		Uninit();
	}	// ���S�t���O�`�F�b�N
}

//=============================================================================
// �`�揈��
//=============================================================================
void  CRobot::Draw(void)
{
	CCharacter::Draw();	// �L�����N�^�[�̕`��
}

//=============================================================================
// ��* �K�v *������������������������������������������������������������������
//=============================================================================
void  CRobot::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
}

//=============================================================================
// �{���̔���
//=============================================================================
void CRobot::Search(void)
{
	CGame *pGame = CManager::GetGame();		// �Q�[���̎擾

	if (m_nNumAttack != ROBOT_SHOOT_MODEINTERVAL)
	{// �ߐڍU�����w�萔�ȏ����Ă��Ȃ������Ƃ�
		float VecShortest = 100000.0f;	// �ŒZ�x�N�g��

		for (int nCntChara = 0; nCntChara < MAX_CHARACTER; nCntChara++)
		{
			CCharacter *pCharacter	= pGame->GetChara(nCntChara);	// �L�����N�^�[�̎擾
			D3DXVECTOR3 CharaPos	= pCharacter->GetPos();			// �L�����N�^�[���W�̎擾

			if (pCharacter->GetCharaState() != 3 && pCharacter->GetCharaState() != 4 && pCharacter->GetCharaState() != 5 && m_nNumTagetOld != nCntChara)
			{// �v���C���[���������łȂ��Ƃ�
				// �x�N�g���̌v�Z
				D3DXVECTOR3 VecP	= CharaPos - m_pos;				// �x�N�g��P = �ڕW - ����
				float VecP_Length	= D3DXVec3Length(&VecP);		// �x�N�g��P�̒������Z�o

				if (VecP_Length <= VecShortest)
				{// �v�Z�����x�N�g�����ŒZ�x�N�g����菬����������
					VecShortest		= VecP_Length;	// �ŒZ�x�N�g���̍X�V
					m_nNumTagetOld	= m_nNumTaget;	// �O��W�I�ԍ��̍X�V
					m_nNumTaget		= nCntChara;	// �W�I�ԍ��̍X�V
					m_nCntWalk		= 0;			// �^�[�Q�b�g�؂�ւ����Ԃ̃��Z�b�g
				}
			}
		}

		if (VecShortest < 10000.0f)
		{// �ŒZ�x�N�g�����X�V����Ă��邩�ǂ���
			m_RobotState = ROBOTSTATE_WALK;			// �ړ���Ԃ�
			m_pMotion->SetNumMotion(m_RobotState);	// ���[�V�����̐ݒ�
		}
	}
	else
	{
		m_RobotState = ROBOTSTATE_SHOOT;		// �ˌ���Ԃ�
		m_pMotion->SetNumMotion(m_RobotState);	// ���[�V�����̐ݒ�
	}
}

//=============================================================================
// �ړ�����
//=============================================================================
void CRobot::Wake(void)
{
	CCharacter *pCharacter = CManager::GetGame()->GetChara(m_nNumTaget);	// �L�����N�^�[�̎擾

	if (pCharacter->GetCharaState() != 4 && m_nCntWalk <= ROBOT_SWITCHTIME)
	{
		D3DXVECTOR3 CharaPos = pCharacter->GetPos();							// �L�����N�^�[���W�̎擾

		// �����̐���
		float fAngle = atan2f(CharaPos.x - m_pos.x, CharaPos.z - m_pos.z) + D3DX_PI;

		if (fAngle > D3DX_PI)
		{
			fAngle -= D3DX_PI * 2;
		}
		if (fAngle < -D3DX_PI)
		{
			fAngle += D3DX_PI * 2;
		}

		m_rot.y = fAngle;	// �����ɑ��

		m_pos.x -= sinf(m_rot.y) * ROBOT_VIGILANCESPEED;
		m_pos.z -= cosf(m_rot.y) * ROBOT_VIGILANCESPEED;

		//	�ʒu�E��]���̔��f
		CCharacter::SetPos(m_pos);
		CCharacter::SetRot(m_rot);

		m_nCntWalk++;
	}
	else
	{
		m_nNumTagetOld	= m_nNumTaget;		// �O��W�I�ԍ��̍X�V
		m_RobotState	= ROBOTSTATE_NONE;	// �j���[�g������Ԃ�
		m_pMotion->SetNumMotion(m_RobotState);
	}
}

//=============================================================================
// �U���̔���
//=============================================================================
void CRobot::Attack(void)
{
	CGame *pGame = CManager::GetGame();		// �Q�[���̎擾

	for (int nCntChara = 0; nCntChara < MAX_CHARACTER; nCntChara++)
	{
		CCharacter *pCharacter = pGame->GetChara(nCntChara);	// �L�����N�^�[�̎擾
		D3DXVECTOR3 CharaPos = pCharacter->GetPos();			// �L�����N�^�[���W�̎擾

		if (pCharacter->GetCharaState() != 4)
		{// �v���C���[���������łȂ��Ƃ�
			// �x�N�g���̌v�Z
			D3DXVECTOR3 VecP = CharaPos - m_pos;					// �x�N�g��P = �ڕW - ����
			float VecP_Length = D3DXVec3Length(&VecP);				// �x�N�g��P�̒������Z�o

			if (VecP_Length <= ROBOT_ATTACK_RADIUS)
			{// �x�N�g��P���w�肵���͈͂����������Ƃ�
				m_RobotState = ROBOTSTATE_ATTACK;
				m_pMotion->SetNumMotion(m_RobotState);
			}
		}
	}
}

//=============================================================================
// �ˌ�����
//=============================================================================
void CRobot::Shoot(void)
{
	CSound *pSound = CManager::GetSound();

	m_rot.y += ROBOT_SHOOT_ROTATION;	// ��]
	CCharacter::SetRot(m_rot);			// ������ݒ�

	if (m_nCntShoot >= ROBOT_SHOOT_BULLETINTERVAL)
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE036);

		// �~�T�C���̐���
		CBullet::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 50.0f, m_pos.z), D3DXVECTOR3(m_rot.x, m_rot.y, m_rot.z), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		CBullet::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 50.0f, m_pos.z), D3DXVECTOR3(m_rot.x, m_rot.y + 0.1f, m_rot.z), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		CBullet::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 50.0f, m_pos.z), D3DXVECTOR3(m_rot.x, m_rot.y - 0.1f, m_rot.z), D3DXVECTOR3(1.0f, 1.0f, 1.0f));

		m_nCntShoot = 0;
		m_nNumShoot++;

		if (m_nNumShoot >= ROBOT_SHOOT_MAXBULLET)
		{
			m_RobotState = ROBOTSTATE_NONE;		// �j���[�g������Ԃ�
			m_pMotion->SetNumMotion(m_RobotState);
			m_nNumShoot = 0;
			m_nNumAttack = 0;
		}
	}
	m_nCntShoot++;	// �J�E���^�[�̉��Z
}

//=============================================================================
// ��������
//=============================================================================
void CRobot::Gravity(void)
{
	CGame *pGame = CManager::GetGame();		// �Q�[���̎擾
	m_fDropTime++;							// �؋󂵂Ă��鎞�Ԃ̉��Z
	float fDrop = m_fDropTime * (ROBOT_GRAVITY/50);		// ������ = �؋󂵂Ă��鎞�� * �d�͉����x

	if (m_pos.y + fDrop > 0)
	{// �����ʂɂ���āA�v���C���[���n�ʂɂ߂荞�܂Ȃ����
		m_pos.y += fDrop;
	}
	else
	{
		m_pos.y = 0;			// �n�ʂɂ߂荞�ނ̂ŁA�n�ʂɂ߂荞�܂Ȃ��ʒu�Ɉړ�
		m_fDropTime = 0.0f;		// �؋󂵂Ă��鎞�Ԃ̃��Z�b�g

		//�G�t�F�N�g
		CEffectManager *pEffectManager = pGame->GetEffectManager();

		if (m_bEndGravity == false)
		{
			m_pEmitter = pEffectManager->SetEffect(D3DXVECTOR3(m_pos.x, m_pos.y - 30.0f, m_pos.z), INITIALIZE_VECTOR3, 4);
		}
		m_bEndGravity = true;

		CEventCamera *pEveCam = pGame->GetEveCam();

		if (m_nCntEmitter >= ROBOT_ENDEMITTER)
		{
			if (m_pEmitter != NULL)
			{
				m_pEmitter->Uninit();
				m_pEmitter = NULL;
			}
			if (m_RobotState != ROBOTSTATE_APPEARANCE)
			{
				m_RobotState = ROBOTSTATE_APPEARANCE;		// �N���オ���Ԃ�
				m_pMotion->SetNumMotion(m_RobotState);
			}
		}
		m_nCntEmitter++;
	}
	//	�ʒu���̔��f
	CCharacter::SetPos(m_pos);
}

//=============================================================================
// �W�����v����
//=============================================================================
void CRobot::Jump(void)
{
	m_fJumpTime++;		// �؋󂵂Ă��鎞�Ԃ̉��Z
	float fDrop = m_fJumpTime * (ROBOT_GRAVITY / 10) * -1.0f;		// �㏸�� = �؋󂵂Ă��鎞�� * �d�͉����x * �u-�v�ŏ㏸��
	if (m_pos.y + fDrop < ROBOT_UNINITPOS)
	{// �����ʂɂ���āA�v���C���[���n�ʂɂ߂荞�܂Ȃ����
		m_pos.y += fDrop;
		//	�ʒu���̔��f
		CCharacter::SetPos(m_pos);
	}
	else{ m_bDeth = true; }
}

//=============================================================================
// �N���オ�菈��
//=============================================================================
void CRobot::GetUp(void)
{
	CGame *pGame = CManager::GetGame();		// �Q�[���̎擾
	CEventCamera *pEveCam = pGame->GetEveCam();
	if (pEveCam == NULL)
	{
		m_RobotState = ROBOTSTATE_NONE;		// �j���[�g������Ԃ�
		m_pMotion->SetNumMotion(m_RobotState);
	}
}

//=============================================================================
//    �o���e���b�v�̐�������
//=============================================================================
void CRobot::CreateRobotUI(void)
{
	if (m_pRobotUI != NULL) { return; }
	m_pRobotUI = CRobotUI::Create();
}

//=============================================================================
// 2�̃x�N�g��AB�̂Ȃ��p�x�Ƃ����߂�
//=============================================================================
double CRobot::VectorAngle(D3DXVECTOR3 A, D3DXVECTOR3 B)
{
	//�x�N�g��P��E�̒������v�Z����
	double Length_A = GetVecLength(A);
	double Length_B = GetVecLength(B);

	//���ςƃx�N�g���������g����cos�Ƃ����߂�
	double cos_sita = D3DXVec3Dot(&A, &B) / (Length_A * Length_B);

	//cos�Ƃ���Ƃ����߂�
	double sita = acos(cos_sita);

	//���W�A����0�`180�̊p�x�ɂ���
	sita = sita * 180.0 / D3DX_PI;

	return sita;
}

//==============================================
//			�~�����쐬���鏈��
//==============================================
void CRobot::CreateCylinderCollider(void)
{
	// �R���C�_�[�Ǌ��N���X�𐶐�
	CColliderManager *pColManager = CColliderManager::Create();
	if (pColManager == NULL) { return; }

	// �~�����쐬
	CCylinderCollider *pCylinder = CCylinderCollider::Create(D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f),
		ROBOT_COLLISION_RADIUS, ROBOT_COLLISION_HEIGHT, false);
	if (pCylinder == NULL) { return; }

	// �e��ݒ�
	pCylinder->SetParent(this);
	pCylinder->SetParentMtxWorld(&GetMtx());

	// �R���C�_�[�Ǌ��N���X�̃|�C���^��ݒ�
	pColManager->SetCollider(pCylinder, 0);
	SetCOlliderManager(pColManager);
}

//=============================================================================
//    �Ռ��g�̔���𐶐����鏈��
//=============================================================================
void CRobot::CreateColliderSphere(void)
{
	CGame *pGame = CManager::GetGame();		// �Q�[���̎擾

	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVec3TransformCoord(&pos, &m_pModel[5]->GetPos(), &m_pModel[5]->GetMtxWorld());
	// �X�t�B�A�𐶐�
	CPlayerAttackSphereCollider *pSphere = CPlayerAttackSphereCollider::Create(D3DXVECTOR3(pos.x, pos.y , pos.z),
		D3DXVECTOR3(1.0f, 1.0f, 1.0f), ROBOT_COLLISION_ATTACKRADIUS, ROBOT_COLLISION_ATTACKLIFE, 1);

	//�G�t�F�N�g
	CEffectManager *pEffectManager = pGame->GetEffectManager();
	pEffectManager->SetEffect(pos, INITIALIZE_VECTOR3, 0);

	//�Ђъ����\��
	CCrack::Create(D3DXVECTOR3(pos.x, 1.0f, pos.z));

	if (pSphere == NULL) { return; }

	// �e��ݒ�
	pSphere->SetParent(this);
}

//=============================================================================
//    ���{�b�gUI���J�����鏈��
//=============================================================================
void CRobot::ReleaseRobotUI(void)
{
	if (m_pRobotUI != NULL)
	{
		m_pRobotUI->Uninit();
		delete m_pRobotUI;
		m_pRobotUI = NULL;
	}
}

//=============================================================================
//    ���{�b�gUI�̍X�V����
//=============================================================================
void CRobot::UpdateRobotUI(void)
{
	if (m_pRobotUI != NULL)
	{
		m_pRobotUI->Update();
	}
}