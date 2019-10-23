
//=============================================================================
//
// ���f������ [model.cpp]
// Author : Kobayashi & Yamashita
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "scene3D.h"
#include "game.h"
#include "title.h"
#include "player.h"
#include "tutorial.h"
#include "collision.h"
#include "model.h"
#include "Territory.h"
#include "library.h"
#include "line.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SPEED			(2.0f)
#define MODEL_RENG		(50)
#define MAX_BUNDCNT		(120)
#define MAX_BULLET		(50)
#define FIRST_LINETIME	(60 * 20)	//���C���̎��Ԃ̏�����
#define MIN_TERRITORY	(3)			//�e���g���[�̍Œ����
#define TERRITORY_TOP	(0)			//�e���g���[�̐擪���
#define NOT_GETTIME		(60)		//�e���g���[�̎擾�ł��Ȃ�����
#define LINE_TIME		(90)		//���C���������Ȃ�����

//�F
#define COLOR_RED	 (D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	//��
#define COLOR_BULE	 (D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f))	//��
#define COLOR_GREEN	 (D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f))	//��
#define COLOR_WHITE	 (D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	//��
#define COLOR_BRACK	 (D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))	//��
#define COLOR_YELLOW (D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f))	//��
#define COLOR_ORENGE (D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f))	//�I�����W
#define COLOR_PAPULE (D3DXCOLOR(0.5f, 0.0f, 1.0f, 1.0f))	//��

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�ÓI�����o�ϐ��錾
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CSceneX *CPlayer::m_apSceneX = NULL;

//���̐F�̐ݒ�
D3DXCOLOR CPlayer::m_CountryColor[CPlayer::TYPE_MAX] =
{
	COLOR_WHITE,	//���{
	COLOR_BULE,		//���V�A
	COLOR_RED,		//�A�����J
	COLOR_YELLOW,	//�u���W��
	COLOR_ORENGE,	//�C�M���X
	COLOR_PAPULE,	//�C�^���A
	COLOR_BRACK,	//�j���[�W�[�����h
	COLOR_GREEN,	//��A�t���J
};


//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CPlayer::CPlayer()
{
	m_Angle = D3DXVECTOR3(0.0f,0.0f,0.0f);
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CPlayer::~CPlayer(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, int nNumPlayer, char ModelTxt[40], char MotionTxt[40])
{
	m_nNumPlayer = nNumPlayer;
	CCharacter::Init(ModelTxt,MotionTxt);
	m_pModel = CCharacter::GetModel();
	CCharacter::SetPos(pos);
	m_nMaxLineTime = FIRST_LINETIME;
	m_nLineTime = m_nMaxLineTime;
	m_bBlockStartTerritory = false;
	m_pOldStartTerritory = NULL;
	m_nCountTime = 0;
	m_pOrbitLine = NULL;
	m_bMakeShape = false;
	m_nCntTimeCopyLine = 0;

	//�R�s�[���C���̏�����
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		m_apCopyLine[nCnt] = NULL;
	}

	ResetLine();	//���C���̏�����
	ResetList();	//�e���g���[�|�C���^�̏�����
	return S_OK;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void  CPlayer::Uninit(void)
{
	CCharacter::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void  CPlayer::Update(void)
{
	CCharacter::Update();

	//���C���̃J�E���g�_�E������
	CountDownLineTime();

	//�v���C���[�̈ړ�����
	PlayerMove();

	//���Ԃ̉Z
	if (m_bBlockStartTerritory) 
	{ 
		m_nCountTime++;
		if ((m_nCountTime % NOT_GETTIME) == 0) { m_bBlockStartTerritory = false; }
	}

	//�}�`������������̏���
	UpdateShapeComplete();
}

//=============================================================================
// �`�揈��
//=============================================================================
void  CPlayer::Draw(void)
{
	//���[�e�V����
	//�ʒu

	CCharacter::Draw();
	//D3DXMATRIX mtxWorld;
	//D3DXMatrixIdentity(&mtxWorld);
	//CManager::GetRenderer()->GetDevice()->SetTransform(D3DTS_WORLD, &mtxWorld);
}

//=============================================================================
//�I�u�W�F�N�g�̐�������
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, int nNumPlayer, char ModelTxt[40], char MotionTxt[40],TYPE type)
{
	CPlayer *pPlayer = NULL;

	if (pPlayer == NULL)
	{
		//�������𓮓I�m��
		pPlayer = new CPlayer;

		if (pPlayer != NULL)
		{//NULL�`�F�b�N

			//����������
			pPlayer->SetType(type);
			pPlayer->Init(pos,nNumPlayer,ModelTxt,MotionTxt);

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
	return pPlayer;
}

//=============================================================================
//
//=============================================================================
void  CPlayer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size){}

//=============================================================================
//�@�v���C���[�̈ړ�����
//=============================================================================
void  CPlayer::PlayerMove(void)
{
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//�L�[�{�[�h�̎擾
	CGamePad * pGamePad = CManager::GetInputGamePad();				//�Q�[���p�b�h�̎擾
	CInputXPad * pXPad = CManager::GetXPad();
	float fRot = pXPad->GetStickRot(0, m_nNumPlayer);


	CManager *pManager = NULL;
	CGame *pGame = pManager->GetGame();
	CGameCamera *pGameCamera = pGame->GetGameCamera(0);

	//�����ƈʒu�̎擾
	D3DXVECTOR3 rot = pGameCamera->GetRot();
	D3DXVECTOR3 pos = CCharacter::GetPos();

	//�X�e�[�^�X�̎擾
	int   nAttack      = GetAttack();
	float fAttackSpeed = GetAttackSpeed();
	float fRange       = GetRange();
	float fSpeed       = GetSpeed();
	float fInertia     = GetInertia();
	float fLineTime    = GetLineTime();

	//�O��̈ʒu���i�[
	m_posOld = pos;

	//�v���C���[�̈ړ������i�L�[�{�[�h�j
	if (pInputKeyboard->GetKeyboardPress(DIK_LEFT) == true)
	{
		if (pInputKeyboard->GetKeyboardPress(DIK_UP) == true)
		{//�|���S��������Ɉړ�
			m_move.x -= sinf(rot.y + (-D3DX_PI * 0.25f)) * fSpeed;
			m_move.z -= cosf(rot.y + (-D3DX_PI * 0.25f)) * fSpeed;
			m_Angle.y = rot.y - (D3DX_PI * 0.25f);
		}
		else if (pInputKeyboard->GetKeyboardPress(DIK_DOWN) == true)
		{//�|���S���������Ɉړ�
			m_move.x -= sinf(rot.y + (-D3DX_PI * 0.75f)) * fSpeed;
			m_move.z -= cosf(rot.y + (-D3DX_PI * 0.75f)) * fSpeed;
			m_Angle.y = rot.y - (D3DX_PI * 0.75f);
		}
		else
		{//�|���S�������Ɉړ�
			m_move.x -= sinf(rot.y + (-D3DX_PI * 0.5f)) * fSpeed;
			m_move.z -= cosf(rot.y + (-D3DX_PI * 0.5f)) * fSpeed;
			m_Angle.y = rot.y - (D3DX_PI * 0.5f);
		}
	}
	else if (pInputKeyboard->GetKeyboardPress(DIK_RIGHT) == true)
	{
		if (pInputKeyboard->GetKeyboardPress(DIK_UP) == true)
		{//�|���S�����E��Ɉړ�
			m_move.x -= sinf(rot.y + (D3DX_PI * 0.25f)) * fSpeed;
			m_move.z -= cosf(rot.y + (D3DX_PI * 0.25f)) * fSpeed;
			m_Angle.y = rot.y + (D3DX_PI * 0.25f);
		}
		else if (pInputKeyboard->GetKeyboardPress(DIK_DOWN) == true)
		{//�|���S�����E���Ɉړ�
			m_move.x -= sinf(rot.y + (D3DX_PI * 0.75f)) * fSpeed;
			m_move.z -= cosf(rot.y + (D3DX_PI * 0.75f)) * fSpeed;
			m_Angle.y = rot.y + (D3DX_PI * 0.75f);
		}
		else
		{//�|���S�����E�Ɉړ�
			m_move.x -= sinf(rot.y + (D3DX_PI * 0.5f)) * SPEED;
			m_move.z -= cosf(rot.y + (D3DX_PI * 0.5f)) * SPEED;
			m_Angle.y = rot.y + (D3DX_PI * 0.5f);
		}
	}
	else if (pInputKeyboard->GetKeyboardPress(DIK_UP) == true)
	{
		m_move.x -= sinf(rot.y) * SPEED;
		m_move.z -= cosf(rot.y) * SPEED;
		m_Angle.y = rot.y;
	}
	else if (pInputKeyboard->GetKeyboardPress(DIK_DOWN) == true)
	{
		m_move.x -= sinf(rot.y + D3DX_PI * 1.0f) * fSpeed;
		m_move.z -= cosf(rot.y + D3DX_PI * 1.0f) * fSpeed;
		m_Angle.y = D3DX_PI + rot.y;
	}

		//�R���g���[���[�iXInput�j
		if (pXPad->GetStick(0, m_nNumPlayer) == true)
		{
			m_move.x -= sinf(fRot + rot.y) * fSpeed;
			m_move.z -= cosf(fRot + rot.y) * fSpeed;
			m_Angle.y = rot.y + fRot;
		}
	
		D3DXVECTOR3 DiffAngle;

		// �v���C���[�̊p�x���C��
		DiffAngle.y = m_Angle.y - m_rot.y;   //�������v�Z

		if (DiffAngle.y > D3DX_PI)
		{//D3DX_PI�𒴂���
			DiffAngle.y -= D3DX_PI * 2.0f;
		}
		if (DiffAngle.y < -D3DX_PI)
		{//-D3DX_PI�𒴂���
			DiffAngle.y += D3DX_PI * 2.0f;
		}

		//������{���Œ���
		m_rot.y += DiffAngle.y * 0.1f;

		if (m_rot.y > D3DX_PI)
		{//D3DX_PI�𒴂���
			m_rot.y -= D3DX_PI * 2.0f;
		}
		if (m_rot.y < -D3DX_PI)
		{//-D3DX_PI�𒴂���
			m_rot.y += D3DX_PI * 2.0f;
		}

		pos += m_move;

		//����
		m_move.x += (0.0f - m_move.x) * fInertia;
		m_move.z += (0.0f - m_move.z) * fInertia;
	
		CCharacter::SetPos(pos);
		CCharacter::SetRot(m_rot);
}


//=============================================================================
//�@�v���C���[�ƃe���g���[�̓����蔻��
//=============================================================================
void CPlayer::Collision(void)
{
	//�e��l�̎擾
	D3DXVECTOR3 pos = CCharacter::GetPos();
	D3DXVECTOR3 posOld = m_posOld;
	D3DXVECTOR3 Move = INITIALIZE_VECTOR3;
	D3DXVECTOR3 ColRange;
	//����J�n
	CCollider *pCol = NULL;
	CCollider *pColNext = NULL;

	for (int nCntType = 0; nCntType < CCollider::TYPE_MAX; nCntType++)
	{
		pCol = CCollider::GetTop(nCntType);

		while (pCol != NULL)
		{
			pColNext = pCol->GetNext();
			CollisionCollider(pCol, pos, posOld, m_move, D3DXVECTOR3(15.0f,15.0f,15.0f));
			pCol = pColNext;
		}
	}
	CCharacter::SetPos(pos);
}

//=============================================================================
//�@�R���C�_�[�Ƃ̓����蔻�菈��
//=============================================================================
bool CPlayer::CollisionCollider(CCollider *pCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
{
	if (pCollider->GetType() == CCollider::TYPE_BOX)
	{
		if (CollisionBoxCollider((CBoxCollider*)pCollider, pos, posOld, Move, ColRange) == true)
		{
			int nData = 0;
		}
	}
	return false;
}

//=============================================================================
//�@�{�b�N�X�R���C�_�[�Ƃ̓����蔻�菈��
//=============================================================================
bool CPlayer::CollisionBoxCollider(CBoxCollider *pBoxCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
{
	bool bLand = false;

	if (pBoxCollider->Collision(&pos, &posOld, &Move, ColRange, this, &bLand) == true)
	{
		if (bLand == true)
		{
			
		}
		return true;
	}
	return false;
}

//=============================================================================
// �e���g���[�̎擾�X�V����
//=============================================================================
void CPlayer::CountDownLineTime(void)
{
	if (m_apTerritory[TERRITORY_TOP] != NULL)
	{
		//����`�����Ԃ����炷
		m_nLineTime--;

		if (m_nLineTime < 0)
		{
			m_nLineTime = m_nMaxLineTime;					//�`��ł��鎞�Ԃ����Z�b�g����
			GetTerritory(m_apTerritory[TERRITORY_TOP]);		//�N�_�̂ݓ_��������
			ResetList();									//���X�g������������
			UninitOrtbitLine();								//��Ղ̔j��
			UninitLine();										//���C���̔j��
		}
	}
}

//=============================================================================
//�@�}�`������������̏���
//=============================================================================
void CPlayer::UpdateShapeComplete(void)
{
	if (!m_bMakeShape) { return; }

	//���Ԃ̉Z
	m_nCntTimeCopyLine++;

	if ((m_nCntTimeCopyLine % LINE_TIME) == 0)
	{
		m_bMakeShape = false;
		UninitCopyLine();
	}
}

//=============================================================================
// �e���g���[�����X�g�ɒǉ�
//=============================================================================
void CPlayer::AddTerritoryList(CTerritory * pTerritory)
{	
	//�}�`���������Ă��邩�ǂ����̔���
	if (m_nCountTerritory >= MIN_TERRITORY)
	{
		if (true == ChackSamePos(pTerritory, TERRITORY_TOP))
		{
			ShapeComplete(pTerritory);	//�}�`�����������Ƃ��̏���
			ChackInShape();				//�}�`���Ƀe���g���[���邩����
			CreateEndLine();			//�Ō�̐��̐���
			ResetList();				//���X�g�̏�����
			UninitOrtbitLine();			//��Ղ̔j��
			ResetLine();				//���C���̏�����
			return;
		}
	}

	//���X�g�̒ǉ�����
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		if (m_apTerritory[nCnt] == NULL)
		{
			UninitOrtbitLine();			//��Ղ̔j��

			if (m_bBlockStartTerritory)
			{//�擾��A�����N�_�����Ȃ��悤�ɂ���
				D3DXVECTOR3 Pos = pTerritory->GetPos();
				D3DXVECTOR3 OldStartPos = m_pOldStartTerritory->GetPos();

				if ((nCnt != TERRITORY_TOP) && (Pos != OldStartPos))
				{
					m_apTerritory[nCnt] = pTerritory;
					m_nCountTerritory++;
					CreateOrbitLine();			//��Ղ̐���
				}
			}
			else
			{//�ʏ�
				m_apTerritory[nCnt] = pTerritory;
				m_nCountTerritory++;
				CreateOrbitLine();				//��Ղ̐���
			}

			//���C���𐶐�����
			if ((nCnt != 0)) 
			{ 
				if (m_apLine[nCnt - 1] == NULL) 
				{ 
					m_apLine[nCnt - 1] = CLine::Create(m_apTerritory[nCnt - 1]->GetPos(), m_apTerritory[nCnt]->GetPos(), m_nNumPlayer, nCnt - 1);
				}
			}
			break;
		}
		else if (m_apTerritory[nCnt] != NULL)
		{//�����e���g���[�̏��ł͂Ȃ����ǂ���
			if (true == ChackSamePos(pTerritory, nCnt)) { break; }
		}
	}
}

//=============================================================================
// �e���g���[�Ő}�`���o���Ă��邩�ǂ���
//=============================================================================
bool CPlayer::ChackSamePos(CTerritory * pTerritory, int nNumTerritory)
{
	//�ϐ��錾
	bool bMakeShape = false;

	D3DXVECTOR3 TopPos = m_apTerritory[nNumTerritory]->GetPos();
	D3DXVECTOR3 CurPos = pTerritory->GetPos();

	if (TopPos == CurPos) { bMakeShape = true; }

	return bMakeShape;
}

//=============================================================================
// �e���g���[�����������Ƃ��̏���
//=============================================================================
void CPlayer::ShapeComplete(CTerritory * pTerritory)
{
	//�}�`������
	m_bBlockStartTerritory = true;
	m_pOldStartTerritory = pTerritory;
	m_bMakeShape = true;
	m_nLineTime = m_nMaxLineTime;		//�`��ł��鎞�Ԃ����Z�b�g����

	//�e���g���[�̎擾
	for (int nCnt = 0; nCnt < m_nCountTerritory; nCnt++)
	{
		if (m_apTerritory[nCnt] != NULL) { GetTerritory(m_apTerritory[nCnt]); }
	}
}

//=============================================================================
// �}�`���Ƀe���g���[�����邩�ǂ���
//=============================================================================
void CPlayer::ChackInShape(void)
{
	//�ϐ��錾
	D3DXVECTOR3 * pListPos = new D3DXVECTOR3[m_nCountTerritory];
	D3DXVECTOR3 * pVecA = new D3DXVECTOR3[m_nCountTerritory];

	//�ʒu���擾
	for (int nCnt = 0; nCnt < m_nCountTerritory; nCnt++)
	{
		if (m_apTerritory[nCnt] != NULL)
		{
			pListPos[nCnt] = m_apTerritory[nCnt]->GetPos();
		}
	}

	//VecA�����߂�
	for (int nCntVec = 0; nCntVec < m_nCountTerritory; nCntVec++)
	{
		int nCnt = nCntVec + 1;
		if ((nCnt % m_nCountTerritory) == 0) { nCnt = 0; }

		pVecA[nCntVec] = pListPos[nCnt] - pListPos[nCntVec];
	}

	//�e���g���[���擾����
	CScene * pSceneTop = CScene::GetTop(TERRITORY_PRIORITY); //�I�u�W�F�N�g�|�C���^�̐擪���擾			
	CScene * pScene = pSceneTop;							 //�g�b�v�̈ʒu��ۑ�����
	CTerritory * m_pTerritory = NULL;

	while (pScene != NULL)
	{
		CScene * pSceneNext = pScene->GetpNext();	//���̃I�u�W�F�N�g�̃|�C���^��ۑ�����

		if (CScene::OBJTYPE_TERRITORY == pScene->GetObjType())
		{
			CTerritory * pTerritory = (CTerritory *)pScene;

			if (pTerritory->GetPlayerNumber() != m_nNumPlayer)
			{
				D3DXVECTOR3 TerritoryPos = pTerritory->GetPos();
				int nCountEnter = 0;

				for (int nCnt = 0; nCnt < m_nCountTerritory; nCnt++)
				{
					if (m_apTerritory[nCnt] != NULL)
					{
						D3DXVECTOR3 VecB = TerritoryPos - pListPos[nCnt];

						//�O�ς����߂�
						float fCross = CFunctionLib::Vec2Cross(&D3DXVECTOR2(pVecA[nCnt].x, pVecA[nCnt].z), &D3DXVECTOR2(VecB.x, VecB.z));

						//�x�N�g�����傫�����ǂ���
						if (fCross >= 0) { nCountEnter++; }
					}
				}

				//�͈͓��Ȃ�e���g���[�̎擾
				if (nCountEnter == m_nCountTerritory) { GetTerritory(pTerritory); }
			}
		}

		pScene = pSceneNext;	//�|�C���^��i�߂�
	}

	//�������̔j��
	if (pListPos != NULL)
	{
		delete[] pListPos;
		pListPos = NULL;
	}
	if (pVecA != NULL)
	{
		delete[] pVecA;
		pVecA = NULL;
	}
}

//=============================================================================
// �e���g���[�̎擾����
//=============================================================================
void CPlayer::GetTerritory(CTerritory * pTerritory)
{
	int nOldNumPlayer = pTerritory->GetPlayerNumber();
	pTerritory->SetColor(m_CountryColor[(int)m_type]);	//�F�̕ύX
	pTerritory->SetPlayerNumber(m_nNumPlayer);			//�v���C���[�ԍ��̕ۑ�

	//�O��̃v���C���[�̌��_����
	if (nOldNumPlayer != -1)
	{

	}

	//�_���̒ǉ�����
}

//=============================================================================
// �e���g���[�̃��X�g��������
//=============================================================================
void CPlayer::ResetList(void)
{
	//�e���g���[�|�C���^�̏�����
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		m_apTerritory[nCnt] = NULL;
	}

	//�J�E���^�[�̏�����
	m_nCountTerritory = 0;
}

//=============================================================================
// ���C���̐�������
//=============================================================================
void CPlayer::CreateOrbitLine(void)
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
void CPlayer::UninitOrtbitLine(void)
{
	if (m_pOrbitLine != NULL)
	{
		m_pOrbitLine->Uninit();
		m_pOrbitLine = NULL;
	}
}

//=============================================================================
// ���C���̏�����
//=============================================================================
void CPlayer::ResetLine(void)
{
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		m_apLine[nCnt] = NULL;
	}
}

//=============================================================================
// ���C���̔j��
//=============================================================================
void CPlayer::UninitLine(void)
{
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		if (m_apLine[nCnt] != NULL)
		{
			m_apLine[nCnt]->Uninit();
			m_apLine[nCnt] = NULL;
		}
	}
}

//=============================================================================
// ���C���̍Ō�̐��̐�������
//=============================================================================
void CPlayer::CreateEndLine(void)
{
	//�ϐ��錾
	int nCountTerritory = 0;

	//�e���g���[�̃��X�g�̍Ō�������߂�
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		if (m_apTerritory[nCnt] == NULL)
		{
			nCountTerritory = nCnt - 1;
			break;
		}
	}

	//�Ō�̃��C���̐���
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		if (m_apLine[nCnt] == NULL)
		{
			m_apLine[nCnt] = CLine::Create(m_apTerritory[nCountTerritory]->GetPos(), m_apTerritory[TERRITORY_TOP]->GetPos(), m_nNumPlayer, nCnt);
			break;
		}
	}

	//�����������C�����R�s�[����
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		if (m_apLine[nCnt] != NULL)
		{
			m_apCopyLine[nCnt] = m_apLine[nCnt];
		}
	}
}

//=============================================================================
// �R�s�[���C���̔j��
//=============================================================================
void CPlayer::UninitCopyLine(void)
{
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		if (m_apCopyLine[nCnt] != NULL)
		{
			m_apCopyLine[nCnt]->Uninit();
			m_apCopyLine[nCnt] = NULL;
		}
	}
}

//=============================================================================
// ���C���̐ؒf����
//=============================================================================
void CPlayer::CutLine(int nID)
{
	//���C���̔j��
	for (int nCnt = nID; nCnt >= 0; nCnt--)
	{
		if (m_apLine[nCnt] != NULL)
		{
			m_apLine[nCnt]->Uninit();
			m_apLine[nCnt] = NULL;
		}
	}

	//���������ւ���
	int nLine = MAX_TERRITORY - nID - 1;

	for (int nCnt = 0; nCnt < nLine; nCnt++)
	{ 
		int nMoveID = nCnt + nID + 1;

		if ((m_apLine[nCnt] == NULL) && (m_apLine[nMoveID] != NULL))
		{
			m_apLine[nCnt] = m_apLine[nMoveID];	//ID�����炷
			m_apLine[nCnt]->SetID(nCnt);
			m_apLine[nMoveID] = NULL;
		}
	}

	//�ؒf�����̃e���g���[��j������
	for (int nCnt = nID; nCnt >= 0; nCnt--)
	{
		m_apTerritory[nCnt] = NULL;
	}

	//�e���g���[�̎n�_���ړ�������
	int nNumTerritory = MAX_TERRITORY - (nID + 1);
	for (int nCnt = 0; nCnt < nNumTerritory; nCnt++)
	{
		if ((m_apTerritory[nCnt] == NULL)/* && (m_apTerritory[nCnt + nID] != NULL)*/)
		{
			m_apTerritory[nCnt] = m_apTerritory[nCnt + (nID + 1)];
			m_apTerritory[nCnt + (nID + 1)] = NULL;
		}
	}
}