//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
//					�]�،^ [typePower.h]
//			Author : Kobayashi_Sho-hei / ���� ����
//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
#include "enemy.h"
#include "manager.h"
#include "model.h"
#include "loadEffect.h"
#include "effectManager.h"
#include "line.h"
#include "collision.h"
#include "debuglog.h"
#include "crack.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_SPRINTTIMER (8)		//	�X�v�����g����
#define RECAST (10)				//	�X�v�����g�̃��L���X�g����
#define SKILL_RANGE			(150.0f)	// �X�L���̎��ʔ͈�

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CTypePower::CTypePower(int nPriority, OBJTYPE objType) : CEnemy(nPriority, objType)
{ 
	m_bTrigger = false;
	m_bBreakTime = false;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CTypePower::~CTypePower() {}

//=============================================================================
//�I�u�W�F�N�g�̐�������
//=============================================================================
CTypePower *CTypePower::Create(int nChara, int country, CHARCTERTYPE type, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40])
{
	CTypePower *pPowerType = NULL;
	if (pPowerType == NULL)
	{
		pPowerType = new CTypePower;

		if (pPowerType != NULL)
		{
			pPowerType->SetType(country);
			pPowerType->m_CharcterType = type;
			pPowerType->Init(nChara, pos, ModelTxt, MotionTxt,country);
			//�����ǉ����܂���
			pPowerType->m_CharcterTypeResult[nChara] = type;
			pPowerType->m_nCuntry[nChara] = country;
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
	return pPowerType;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTypePower::Init(int nChara, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int country)
{
	m_bWalk = true;

	m_nEnemyNo = nChara;	//	�L�����ԍ��̋L��(������)
	m_bSprintMotion = false;

	CCharacter::Init(nChara, ModelTxt, MotionTxt,m_CharcterType,country);	//	������
	CCharacter::SetPos(pos);						//	�ʒu���f
	m_nLineNum = 2;	//	�Œ���̃��C������������n�_�ɖ߂�(���_��2�q������n�_�ɖ߂�A�n�_�ɖ߂��Ă����烉�C����3�ɂȂ�|�C���g���Z�̏����𖞂���
	InitSort(pos);	//	�Q�[���J�n���̋ߏ�̃G���A�E�e���g���[��������


	m_pModel = CCharacter::GetModel();	//	���f�����̎擾
	m_pMotion = CCharacter::GetMotion();//	���[�V�������̎擾

	//	���C���ϐ��̏�����
	m_bBlockStartTerritory = false;
	m_pOldStartTerritory = NULL;
	m_bMakeShape = false;
	m_pLoadEffect = NULL;

	//�G�t�F�N�g�̏�����
	if (m_pLoadEffect == NULL)
	{
		m_pLoadEffect = CLoadEffect::Create(1, D3DXVECTOR3(pos.x, pos.y + 60.0f, pos.z), 8);
	}


	ResetLine();	//	���C���̏�����


	m_fSpeed = 1.0f;				//	�������x
	m_nRecastCounter = 0;
	m_bBreakTime = false;
	m_bRecast = false;
	m_nLineNum = 2;
	m_bStop = false;
	m_bTrigger = false;
	m_state = STATE_NONE;

	return S_OK;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTypePower::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void  CTypePower::Uninit(void)
{
	if (m_pLoadEffect != NULL)
	{//�G�t�F�N�g�̔j��
		m_pLoadEffect->Uninit();
		delete m_pLoadEffect;
		m_pLoadEffect = NULL;
	}
	//�v���C���[�̏I������
	CEnemy::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void  CTypePower::Update(void)
{
	CGame *pGame = CManager::GetGame();				// �Q�[���̎擾
	CEventCamera *pEveCam = pGame->GetEveCam();		// �C�x���g�J�����̎擾

	//�Q�[���̏�Ԃ��擾
	int nGameState = CGame::GetGameState();

	if (nGameState != CGame::GAMESTATE_FIRSTCOUNTDOWN && nGameState != CGame::GAMESTATE_END)
	{

		CEnemy::Update();
		if (pEveCam == NULL)	// �C�x���g�J�����������Ă�����
		{
			ActionUpdate();	//	�X�L������
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void  CTypePower::Draw(void)
{
	CEnemy::Draw();
}
//=============================================================================
//
//=============================================================================
void  CTypePower::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size) { }

//=============================================================================
// �X�L������
//=============================================================================
void  CTypePower::ActionUpdate(void)
{
	// �u���C�N�^�C���ł͂Ȃ� && �X�L���̔����t���O�𗧂Ă� && ��������񂵂��ʂ��Ȃ�
	if (m_bBreakTime == false && m_bTrigger == true && m_bStop == false && m_state != STATE_DAMAGE)
	{
		m_bStop = true;							//	�X�V���ꎞ�I�Ɏ~�߂�
		m_pMotion->SetNumMotion(STATE_ACTION);	//�@�U�����[�V����
		m_state = STATE_ACTION;
		m_fSpeed = 0.0f;						//	�A�N�V�������͓������~�߂�
		m_bTarget = true;						//	�^�[�Q�b�g��ύX
	}

	//	�X�L���̔����t���O�������Ă���
	if (m_bTrigger == true)
	{
		m_nRecastCounter++;
	}

	switch (m_nRecastCounter)
	{
	case 60:
		if (m_state != STATE_DAMAGE && m_state != STATE_BLOWAWAY && m_state != STATE_ROBOTDAMAGE)
		{//�_���[�W������Ȃ�������Ռ��g���o��
			CreateColliderSphere();	//�Ռ��g�̓����蔻���t�^
		}
		break;

	case 120:
		m_fSpeed = 1.0f;			//	�A�N�V�����I�����A������悤��
		m_bBreakTime = true;		//	�u���C�N�^�C���̔���
		m_bTarget = false;			//	�^�[�Q�b�g�����_�ɖ߂�
		m_bSuperArmor = false;
		m_bWalk = true;
		if (m_state != STATE_DAMAGE && m_state != STATE_ROBOTDAMAGE)
		{//�_���[�W������Ȃ�������
			m_state = STATE_NONE;
		}

		break;

	case 60 * RECAST:
		m_bStop = false;		//	�܂��X�V�ł���悤��
		m_nRecastCounter = 0;	//	�J�E���g�̏�����(�Ռ��g�̔����̂��߂�)
		m_bBreakTime = false;	//	�u���C�N�^�C���̏I��
		m_bTrigger = false;		//	�X�L���̔����t���O�����Z�b�g
		break;
	}
}

//=============================================================================
//    �Ռ��g�̔���𐶐����鏈��
//=============================================================================
void CTypePower::CreateColliderSphere(void)
{
	//�Q�[���̎擾
	CGame *pGame = CManager::GetGame();
	//�G�t�F�N�g
	CEffectManager *pEffectManager = pGame->GetEffectManager();

	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVec3TransformCoord(&pos, &m_pModel[15]->GetPos(), &m_pModel[15]->GetMtxWorld());

	// �X�t�B�A�𐶐�
	CPlayerAttackSphereCollider *pSphere = CPlayerAttackSphereCollider::Create(D3DXVECTOR3(pos.x + sinf(m_rot.y - D3DX_PI) * 100, pos.y, pos.z + cosf(m_rot.y - D3DX_PI) * 100),
		D3DXVECTOR3(1.0f, 1.0f, 1.0f), 150.0f, 40, 1);

	D3DXVECTOR3 EffectPos = D3DXVECTOR3(pos.x + sinf(m_rot.y - D3DX_PI) * 100, pos.y - 30.0f, pos.z + cosf(m_rot.y - D3DX_PI) * 100);

	//�Ռ��g�̃G�t�F�N�g�\��
	pEffectManager->SetEffect(EffectPos, INITIALIZE_VECTOR3, 1);
	pEffectManager->SetEffect(EffectPos, INITIALIZE_VECTOR3, 2);
	pEffectManager->SetEffect(EffectPos, INITIALIZE_VECTOR3, 3);

	//�Ђъ����\��
	CCrack::Create(D3DXVECTOR3(EffectPos.x, 1.0f, EffectPos.z));

	if (pSphere == NULL) { return; }

	//	�e��ݒ�
	pSphere->SetParent(this);

}


//=============================================================================
//	�X�L�����g�p����͈�
//=============================================================================
bool CTypePower::CollisionSkillTiming(CCylinderCollider *pCylinderCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
{
	bool bLand = false;

	// [[���X�L�������͈�]]
	if (pCylinderCollider->Collision(&pos, &posOld, &Move, SKILL_RANGE, 0.0f, this, &bLand) == true)
	{
		if (m_CharcterType == CCharacter::CHARCTERTYPE_POWER)
		{
			D3DXVECTOR3 thisPos = CCharacter::GetPos(); // ���g�̈ʒu���
			CScene *pParent = pCylinderCollider->GetParent(); // ���L�����̏����擾

			if (m_bBreakTime == false && m_state != STATE_DAMAGE && m_state != STATE_BLOWAWAY && m_state != STATE_ROBOTDAMAGE) // �u���C�N�^�C���łȂ����...
			{
				m_bTrigger = true;	//	�g�p�t���O�𗧂Ă�
				
				m_bSuperArmor = true;
				CCharacter *pCharacter = (CCharacter*)pParent;
				if (pCharacter == NULL) { return true; }
				D3DXVECTOR3 targetPos = pCharacter->GetPos(); // �Ώۂ̈ʒu�����擾

				// [[���p�x�����̏���]]^^^^^^^^^^^^^^^^^^^^^^^^^^^^
				float fDestAngle = atan2f(thisPos.x - targetPos.x, thisPos.z - targetPos.z);
				float fAngle = fDestAngle - m_rot.y;
				if (fAngle > D3DX_PI) { fAngle -= D3DX_PI * 2; }
				if (fAngle < -D3DX_PI) { fAngle += D3DX_PI * 2; }
				m_rot.y += fAngle * 0.1f;
				if (m_rot.y > D3DX_PI) { m_rot.y -= D3DX_PI * 2; }
				if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2; }
				CCharacter::SetPos(targetPos);
				CCharacter::SetRot(m_rot);
				// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
			}
			return true;
		}
	}

	return false;
}