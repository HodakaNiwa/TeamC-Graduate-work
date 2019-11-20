//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
//					�n���^ [typeSweeper.h]
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
#include "mine.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_SPRINTTIMER (8)		//�X�v�����g����
#define RECAST (10)				//�X�v�����g�̃��L���X�g����
#define MAX_MINE_POINT (100)		//�}�C���̒u����|�C���g
#define REDUCED_MINE_POINT (20)		//�}�C����u�����Ƃ��Ɍ���|�C���g

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CTypeSweeper::CTypeSweeper()
{

}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CTypeSweeper::~CTypeSweeper()
{

}

//=============================================================================
//�I�u�W�F�N�g�̐�������
//=============================================================================
CTypeSweeper *CTypeSweeper::Create(int nChara, int country, CHARCTERTYPE type, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40])
{
	CTypeSweeper *pSpeedType = NULL;
	if (pSpeedType == NULL)
	{
		pSpeedType = new CTypeSweeper;

		if (pSpeedType != NULL)
		{
			pSpeedType->SetType(country);
			pSpeedType->m_CharcterType = type;
			pSpeedType->Init(nChara, pos, ModelTxt, MotionTxt);
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
HRESULT CTypeSweeper::Init(int nChara, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40])
{
	//�v���C���[�̃i���o�[���i�[
	m_nEnemyNo = nChara;

	//������
	m_bWalk = true;
	m_bSprintMotion = false;
	//�L�����N�^�[�̏�����
	CCharacter::Init(nChara,ModelTxt, MotionTxt,m_CharcterType);
	CCharacter::SetPos(pos);

	m_nLineNum = 2;	//	�Œ���̃��C������������n�_�ɖ߂�(���_��2�q������n�_�ɖ߂�A�n�_�ɖ߂��Ă����烉�C����3�ɂȂ�|�C���g���Z�̏����𖞂���
	InitSort(pos);	//	�Q�[���J�n���̋ߏ�̃G���A�E�e���g���[��������


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
		m_pLoadEffect = CLoadEffect::Create(1, D3DXVECTOR3(pos.x, pos.y + 60.0f, pos.z), 8);
	}

	//���[�V�����̏����擾
	m_pMotion = CCharacter::GetMotion();
	ResetLine();	//���C���̏�����

	
	m_fSpeed = 1.0f;				//	�������x
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_bRecast = false;
	m_bCollision = false;
	m_nMinePoint = MAX_MINE_POINT;
	m_nCreateTime = (rand() % 4);
	return S_OK;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTypeSweeper::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void  CTypeSweeper::Uninit(void)
{
//	if (m_pLoadEffect != NULL)
//	{//�G�t�F�N�g�̔j��
//		m_pLoadEffect->Uninit();
//		delete m_pLoadEffect;
//		m_pLoadEffect = NULL;
//	}

	//�v���C���[�̏I������
	CEnemy::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void  CTypeSweeper::Update(void)
{
	//�X�s�[�h�^�̃X�L������
	//ActionUpdate();

	//�v���C���[�̍X�V
	CEnemy::Update();
}

//=============================================================================
// �X�v�����g����
//=============================================================================
void  CTypeSweeper::ActionUpdate(void)
{
	CGame *pGame = CManager::GetGame();
	CUI *pUi = pGame->GetUI();
	CCharacter *pCharacter = pGame->GetChara(m_nEnemyNo);

	if (m_bRecast == false && m_nButtonCounter == 0)
	{
		//�n����ݒu
		CMine::Create(CCharacter::GetPos(), pCharacter);

		m_nButtonCounter = 0;	//�{�^���������Ȃ��悤�ɂ���

		pUi->GetSkilicon(m_nEnemyNo)->RevivalIconMask();	//�X�L���A�C�R��

		m_bRecast = false;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void  CTypeSweeper::Draw(void)
{
	CEnemy::Draw();
}

//=============================================================================
//
//=============================================================================
void  CTypeSweeper::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{

}

//=============================================================================
// ���C���̐�������
//=============================================================================
void CTypeSweeper::CreateOrbitLine(void)
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
void CTypeSweeper::UninitOrtbitLine(void)
{
	if (m_pOrbitLine != NULL)
	{
		m_pOrbitLine->Uninit();
		m_pOrbitLine = NULL;
	}
}