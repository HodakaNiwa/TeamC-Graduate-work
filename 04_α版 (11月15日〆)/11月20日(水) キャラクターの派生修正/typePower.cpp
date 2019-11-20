//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
//					�]�،^ [typePower.h]
//			Author : Kobayashi_Sho-hei / ���� ����
//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
#include "enemy.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "inputmouce.h"
#include "loadEffect.h"
#include "line.h"
#include "sceneOrbit.h"
#include "model.h"
#include "UI.h"
#include "skilicon.h"
#include "RawMouse.h"
#include "collision.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_SPRINTTIMER (8)		//	�X�v�����g����
#define WAVE (1)				//	
#define RECAST (10)				//	�X�v�����g�̃��L���X�g����
//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CTypePower::CTypePower(int nPriority, OBJTYPE objType) : CEnemy(nPriority, objType)
{

}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CTypePower::~CTypePower()
{

}

//=============================================================================
//�I�u�W�F�N�g�̐�������
//=============================================================================
CTypePower *CTypePower::Create(int nChara, int country, CHARCTERTYPE type, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40])
{
	CTypePower *pSpeedType = NULL;
	if (pSpeedType == NULL)
	{
		pSpeedType = new CTypePower;

		if (pSpeedType != NULL)
		{
			pSpeedType->SetType(country);
			pSpeedType->m_CharcterType = type;
			pSpeedType->Init(nChara, pos, ModelTxt, MotionTxt, country);
			
			//�����ǉ����܂���
			pSpeedType->m_CharcterTypeResult[nChara] = type;
			pSpeedType->m_nCuntry[nChara] = country;
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
	return pSpeedType;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTypePower::Init(int nChara, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], int country)
{
	m_bWalk = true;

	m_nEnemyNo = nChara;	//	�L�����ԍ��̋L��(������)
	m_bSprintMotion = false;

	CCharacter::Init(nChara, ModelTxt, MotionTxt,m_CharcterType, country);	//	������
	CCharacter::SetPos(pos);						//	�ʒu���f
	m_nLineNum = 2;	//	�Œ���̃��C������������n�_�ɖ߂�(���_��2�q������n�_�ɖ߂�A�n�_�ɖ߂��Ă����烉�C����3�ɂȂ�|�C���g���Z�̏����𖞂���
	InitSort(pos);	//	�Q�[���J�n���̋ߏ�̃G���A�E�e���g���[��������


	m_pModel = CCharacter::GetModel();	//	���f�����̎擾
	m_pMotion = CCharacter::GetMotion();//	���[�V�������̎擾

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
		m_pLoadEffect = CLoadEffect::Create(1, D3DXVECTOR3(pos.x, pos.y + 60.0f, pos.z), 8);
	}


	ResetLine();	//	���C���̏�����


	m_fSpeed = 1.0f;				//	�������x
	m_nColliderCnt = 0;
	m_nColliderTimer = 0;
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_fScale = 0.0f;
	m_bAction = false;
	m_bRecast = false;
	//m_bCollision = false;
	m_nCreateTime = (rand() % 4);	//	�n�_�ɖ߂�܂ł̎��Ԓ���
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
	ActionUpdate();	//	�X�L������
	CEnemy::Update();
}

//=============================================================================
// �X�L������
//=============================================================================
void  CTypePower::ActionUpdate(void)
{
	if (m_bRecast == false && m_bCheckS == false)
	{
		m_bCheckS = true;			//	�����񂱂̏�����ʂ�Ȃ��悤�ɂ���
		CreateStartUpCollider();	//�@�n���}�[���̂̓����蔻��
		m_pMotion->SetNumMotion(2);	//�@�U�����[�V����
		m_bAction = true;			//�@�A�N�V�������ɂ���

	}

	//�A�N�V������
	if (m_bAction == true)
	{
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
			if (m_nRecastTimer <= RECAST)
			{//���L���X�g���̓A�N�V�������g�p�ł��Ȃ�
				m_bRecast = true;
			}
			else
			{//10�b�o�߂�����A�N�V�������g�p�ł���
				m_bCheckS = false;
				m_nRecastTimer = 0;					//�^�C�}�[��������
				m_bRecast = false;					//���L���X�g�I��
				m_bAction = false;					//�A�N�V�������g�p�ł���
			}
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
void  CTypePower::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{

}

//=============================================================================
//    �n���}�[�̓����蔻��𐶐����鏈��
//=============================================================================
void CTypePower::CreateStartUpCollider(void)
{
	//	�~���𐶐�
	CPlayerAttackSphereCollider *pSphere = CPlayerAttackSphereCollider::Create(D3DXVECTOR3(0.0f, 0.0f, -60.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f),
		30.0f, 100, 1);

	if (pSphere == NULL) { return; }

	//	�e��ݒ�
	pSphere->SetParent(this);

	//	����̏ꏊ�ɔ����t����
	pSphere->SetParentMtxWorld(&m_pModel[15]->GetMtxWorld());
}

//=============================================================================
//    �Ռ��g�̔���𐶐����鏈��
//=============================================================================
void CTypePower::CreateColliderSphere(void)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVec3TransformCoord(&pos, &m_pModel[15]->GetPos(), &m_pModel[15]->GetMtxWorld());

	//	�X�t�B�A�𐶐�
	CPlayerAttackSphereCollider *pSphere = CPlayerAttackSphereCollider::Create(D3DXVECTOR3(pos.x, pos.y, pos.z + 35.0f),
		D3DXVECTOR3(1.0f, 1.0f, 1.0f), 100.0f, 100, 1);

	if (pSphere == NULL) { return; }

	//	�e��ݒ�
	pSphere->SetParent(this);

}

//=============================================================================
// ���C���̐�������
//=============================================================================
void CTypePower::CreateOrbitLine(void)
{
	if (m_pOrbitLine == NULL)
	{
		m_pOrbitLine = CSceneOrbit::Create(CSceneOrbit::TYPE_PLAYER, CCharacter::GetPos());
		m_pOrbitLine->SetMtxParent(&m_pModel[10]->GetMtxWorld());
		m_pOrbitLine->SetMtxParentEnd(&m_pModel[1]->GetMtxWorld());
	}
}

//=============================================================================
// ���C���̔j��
//=============================================================================
void CTypePower::UninitOrtbitLine(void)
{
	if (m_pOrbitLine != NULL)
	{
		m_pOrbitLine->Uninit();
		m_pOrbitLine = NULL;
	}
}