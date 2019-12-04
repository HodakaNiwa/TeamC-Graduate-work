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

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CRobot::CRobot(int nPriority, OBJTYPE objType) : CCharacter(nPriority, objType)
{
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nNumTaget		= 0;
	m_nCntAttack	= 0;
	m_fDropTime		= 0.0f;
	m_fJumpTime		= 0.0f;
	m_nCntEnd		= 0;
	m_RobotState	= ROBOTSTATE_APPEARANCE;		// �o����Ԃ�
	m_pRobotUI		= NULL;
	m_pEmitter		= NULL;
	m_nCntEmitter	= 0;
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

	//�ϐ��̏�����
	m_bPlayerMotionState = false;
	m_bDeth = false;
	m_bEndGravity = false;

	//�L�����N�^�[�̏�����
	CCharacter::Init(nNumPlayer, ModelTxt, MotionTxt, m_CharcterType, 0);
	CCharacter::SetPos(pos);

	m_pos = pos;

	//���f���̏����擾
	m_pModel = CCharacter::GetModel();

	//���[�V�����̏����擾
	m_pMotion = CCharacter::GetMotion();
	m_pMotion->SetNumMotion(m_RobotState);

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

	//if (m_pLoadEffect != NULL)
	//{//�G�t�F�N�g�̔j��
	//	m_pLoadEffect->Uninit();
	//	delete m_pLoadEffect;
	//	m_pLoadEffect = NULL;
	//}
}

//=============================================================================
// �X�V����
//=============================================================================
void CRobot::Update(void)
{
	switch (m_RobotState)
	{
	case ROBOTSTATE_NONE:			// �j���[�g����
		Search();	// �{������
		break;
	case ROBOTSTATE_WALK:			// �ړ�
		Wake();		// �ړ�����
		Attack();	// �U������
		break;
	case ROBOTSTATE_APPEARANCE:		// �o��
		Gravity();	// ��������
		break;
	case ROBOTSTATE_JUMP:			// �W�����v
		Jump();		// �W�����v����
		break;
	case ROBOTSTATE_ATTACK:			// �ߐڍU��
		if (m_pMotion->GetNumMotion() == 0)
		{
			m_RobotState = ROBOTSTATE_NONE;		// �j���[�g������Ԃ�
		}
		break;
	case ROBOTSTATE_SHOOT:			// �ˌ��U��
		break;
	}

	if (m_nCntEnd >= ROBOT_ENDTIME && m_RobotState != ROBOTSTATE_JUMP)
	{// �ޏꎞ�ԂɂȂ�����
		m_RobotState = ROBOTSTATE_JUMP;		// �W�����v��Ԃ�
		m_pMotion->SetNumMotion(m_RobotState);
	}
	m_nCntEnd++;	// �ޏ�J�E���^�[�̉��Z

	//�L�����N�^�[�̍X�V
	CCharacter::Update();
	UpdateRobotUI();	// UI�̍X�V

	D3DXVECTOR3 ModelPos = m_pModel[0]->GetPos();

	CDebugProc::Print("���{�b�g:%f,%f,%f", m_pos.x, m_pos.y, m_pos.z);

	if (m_bDeth) { Uninit(); }
}

//=============================================================================
// �`�揈��
//=============================================================================
void  CRobot::Draw(void)
{
	//�L�����N�^�[�̕`��
	CCharacter::Draw();
	DrawRobotUI();	// UI�̕`�揈��
}

//=============================================================================
//
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

	float VecOld = 10000.0f;	// �ŒZ�x�N�g��

	for (int nCntChara = 0; nCntChara < MAX_CHARACTER; nCntChara++)
	{
		CCharacter *pCharacter = pGame->GetChara(nCntChara);	// �L�����N�^�[�̎擾
		D3DXVECTOR3 CharaPos = pCharacter->GetPos();			// �L�����N�^�[���W�̎擾

		if (pCharacter->GetCharaState() != 4)
		{// �v���C���[���������łȂ��Ƃ�
			// �x�N�g���̌v�Z
			D3DXVECTOR3 VecP = CharaPos - m_pos;					// �x�N�g��P = �ڕW - ����
			float VecP_Length = D3DXVec3Length(&VecP);				// �x�N�g��P�̒������Z�o

			if (VecP_Length <= VecOld)
			{// �v�Z�����x�N�g�����ŒZ�x�N�g����菬����������
				VecOld = VecP_Length;		// �ŒZ�x�N�g���̍X�V
				m_nNumTaget = nCntChara;	// �W�I�ԍ��̍X�V
			}
		}
	}

	if (VecOld < 10000.0f)
	{
		m_RobotState = ROBOTSTATE_WALK;	// �ړ���Ԃ�
		m_pMotion->SetNumMotion(m_RobotState);
	}
}

//=============================================================================
// �ړ�����
//=============================================================================
void CRobot::Wake(void)
{
	// �f�[�^�̎擾
	CCharacter *pCharacter = CManager::GetGame()->GetChara(m_nNumTaget);	// �L�����N�^�[�̎擾
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
				CreateColliderSphere();
			}
		}
	}
}
//=============================================================================
// ��������
//=============================================================================
void CRobot::Gravity(void)
{
	CGame *pGame = CManager::GetGame();		// �Q�[���̎擾
	m_fDropTime++;								// �؋󂵂Ă��鎞�Ԃ̉��Z
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

		if (pEveCam == NULL)
		{
			m_RobotState = ROBOTSTATE_NONE;		// �j���[�g������Ԃ�
			m_pMotion->SetNumMotion(m_RobotState);
		}
		else
		{
			if (m_nCntEmitter >= ROBOT_ENDEMITTER)
			{
				if (m_pEmitter != NULL)
				{
					m_pEmitter->Uninit();
					m_pEmitter = NULL;
				}
			}
			m_nCntEmitter++;
		}
	}
	//	�ʒu���̔��f
	CCharacter::SetPos(m_pos);
}

//=============================================================================
// �W�����v����
//=============================================================================
void CRobot::Jump(void)
{
	m_fJumpTime++;								// �؋󂵂Ă��鎞�Ԃ̉��Z
	float fDrop = m_fJumpTime * (ROBOT_GRAVITY / 10) * -1.0f;		// �㏸�� = �؋󂵂Ă��鎞�� * �d�͉����x * �u-�v�ŏ㏸��
	if (m_pos.y + fDrop < ROBOT_UNINITPOS)
	{// �����ʂɂ���āA�v���C���[���n�ʂɂ߂荞�܂Ȃ����
		m_pos.y += fDrop;
		//	�ʒu���̔��f
		CCharacter::SetPos(m_pos);
	}
	else
	{
		m_bDeth = true;
	}
}

//=============================================================================
//    ���{�b�gUI�𐶐����鏈��
//=============================================================================
void CRobot::CreateRobotUI(int nNum)
{
	if (m_pRobotUI != NULL) { return; }
	m_pRobotUI = CRobotUI::Create(nNum);
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

void CRobot::CreateOrbitLine(void)
{
}
void CRobot::UninitOrtbitLine(void)
{
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
	D3DXVECTOR3 pos = m_pModel[0]->GetPos();
	//D3DXVec3TransformCoord(&pos, &m_pModel[0]->GetPos(), &m_pModel[0]->GetMtxWorld());
	pos += m_pos;
	// �X�t�B�A�𐶐�
	CPlayerAttackSphereCollider *pSphere = CPlayerAttackSphereCollider::Create(D3DXVECTOR3(pos.x, pos.y - 70.0f, pos.z),
		D3DXVECTOR3(1.0f, 1.0f, 1.0f), 100.0f, 100, 1);

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

//=============================================================================
//    ���{�b�gUI�̕`�揈��
//=============================================================================
void CRobot::DrawRobotUI(void)
{
	/*if (m_pRobotUI != NULL)
	{
		m_pRobotUI->Draw();
	}*/
}
