//=============================================================================
//
// �J�������� [camera.cpp]
// Author : �������� Sato_Asumi & �O�H�ۋM
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "tutorial.h"
#include "player.h"
#include "sceneX.h"
#include "model.h"
#include "model.h"
#include "character.h"
#include "library.h"
#include "territory.h"
#include "debuglog.h"
#include "robot.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define DRAW_CAMERA		(15000.0f)	//�J�����̕`��͈�
#define CAMERA_SPEED	(2.0f)		//�J�����̃X�s�[�h
#define LENGTH_X		(0.0f)		//���_�ƒ����_�̋����i�w�j
#define LENGTH_Y		(500.0f)		//���_�ƒ����_�̋����i�x�j30 700.0f 500.0f
#define LENGTH_Z		(180.0f)	//���_�ƒ����_�̋����i�y�j150 180.0f
#define POSR_Y			(35.0f)		//�����_�̍���
#define BEHIND_TIME		(60)		//�v���C���[�̌��ɃJ��������荞�ގ���
#define BEHIND_SPEED	(0.01f)		//�J�����̉�荞�ނƂ��̊���
#define VIEW_ANGLE		(45)		//��p

//�^�C�g���J����
#define TITILE_LENGTH_X (0.0f)
#define TITILE_LENGTH_Y (90.0f)
#define TITILE_LENGTH_Z (200.0f)
#define MAX_HIGHT		(110.0f)
#define MIN_HIGHT		(15.0f)
#define CHANGE_TIME		(60 * 4)	//�J�����̕W������
#define MOVE_Y			(0.5f)		//�J�����̈ړ���
#define MOVE_Z			(1.5f)		//�ړ���

//�Q�[���J����
#define MOVE_CLEAR			(0.2f)		//�N���A���̈ړ���
#define ADDHEIGHT_CUTVALUE	(10.0f)
#define ADDDEPTH_CUTVALUE	(-6.0f)
#define ADDHEIGHT_ADDVALUE	(7.5f)
#define ADDDEPTH_ADDVALUE	(-2.3f)
#define ADDHEIGHT_MAX		(400.0f)
#define ADDDEPTH_MAX		(-120.0f)
#define SCREEN_ADDSIZE_X	(60.0f)
#define SCREEN_ADDSIZE_Y	(40.0f)
#define SCREEN_MIDDLESIZE_X	(500.0f)
#define SCREEN_MIDDLESIZE_Y	(250.0f)


//�����L���O�J����
#define WINDY			(0.05f)		//����
#define SPEED			(0.3f)		//�J�����̈ړ����x
#define MOVE			(20.0f)		//�ړ��͈�

// �C�x���g�J����
#define UNINIT_TIME_ROBOT				(200)		// ���{�b�g�����n���Ă���J������������܂ł̎���
#define UNINIT_TIME_AREABONUS_START		(240)
#define UNINIT_TIME_AREABONUS_FINISH	(240)
#define UNINIT_TIME_DIVISION_START		(240)
#define UNINIT_TIME_DIVISION_FINISH		(240)
#define DIVISION_ROTSPPED				(0.003f)

//=============================================================================
//	�R���X�g���N�^
//=============================================================================
CCamera::CCamera(){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCamera::~CCamera(){}

//=============================================================================
// �J�����̏���������
//=============================================================================
void CCamera::Init(void)
{
	m_posV = D3DXVECTOR3(LENGTH_X, LENGTH_Y, -LENGTH_Z);	//���_
	m_posVDest = D3DXVECTOR3(0, 0, 0);						//�ړI�̎��_
	m_posR = D3DXVECTOR3(0, POSR_Y, 0);						//�����_
	m_posRDest = D3DXVECTOR3(0, 0, 0);						//�ړI�̎��_
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);					//������x�N�g��
	m_Rot =	D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);				//����
	m_RotDest = D3DXVECTOR3(0, 0, 0);						//�ړI�̌���
	m_Lngth = LENGTH_Z;										//����
	m_fAngle = 0.0f;										//�p�x
	m_nViewAngle = VIEW_ANGLE;								//��p
	m_bRot = false;											//�J��������]���Ă��邩�ǂ���
	m_bHight = false;										//�������ω��������ǂ���
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void CCamera::Uninit(void)
{
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void CCamera::Update(void)
{
	//�J�����̍������ω����Ă��Ȃ���Ԃɂ���
	m_bHight = false;

	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//�L�[�{�[�h�̎擾
	CGamePad * pGamePad = CManager::GetInputGamePad();				//�Q�[���p�b�h�̎擾

	//�f�o�b�O�L�[�̍X�V����
	DebugUpdate(pInputKeyboard);

	if ((pInputKeyboard->GetKeyboardPress(DIK_Z) == true))	 //Z�L�[(��)�������ꂽ���ǂ���
	{
		//�J�����̉�]��
		m_RotDest.y += 0.01f;

		//�ړI�̊p�x�C��
		RemakeAngle(&m_RotDest.y);

		//��]���Ă����Ԃɂ���
		m_bRot = true;
	}
	if ((pInputKeyboard->GetKeyboardPress(DIK_C) == true))	 //C�L�[(��)�������ꂽ���ǂ���
	{
		//�J�����̉�]��
		m_RotDest.y -= 0.01f;

		//�ړI�̊p�x�C��
		RemakeAngle(&m_RotDest.y);

		//��]���Ă����Ԃɂ���
		m_bRot = true;
	}

	if (m_bRot == true)
	{
		m_Rot.y += m_RotDest.y;

		//�ړI�̊p�x�C��
		RemakeAngle(&m_Rot.y);

		//�����_�X�V
		m_posV.x = m_posR.x - sinf(m_Rot.y - D3DX_PI) * m_Lngth;
		m_posV.z = m_posR.z - cosf(m_Rot.y - D3DX_PI) * m_Lngth;

		//����
		m_RotDest.y += (0.0f - m_RotDest.y) * 0.4f;
	}
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void CCamera::Set()
{
	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �`��̈��ݒ�
	pDevice->SetViewport(&m_Viewport);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
								D3DXToRadian(m_nViewAngle),							//�J�����̉�p
								(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
								10.0f,
								DRAW_CAMERA);										//�J�����̕`��͈�

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixLookAtLH(&m_mtxView,
					   &m_posV,
					   &m_posR,
					   &m_vecU);

	// �r���[�}�g���b�N�X���쐬
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//=============================================================================
// �`��̈�̐ݒ菈��
//=============================================================================
void CCamera::SetViewport(D3DVIEWPORT9 viewport)
{
	m_Viewport = viewport;
}


//=============================================================================
// �J�����Ɏʂ��Ă��邩�ǂ����̔��菈��
//=============================================================================
bool CCamera::ReflectObject(D3DXVECTOR3 ObjectPos)
{
	//�ϐ��錾
	D3DXVECTOR3 Pos[4];
	D3DXVECTOR3 VecA[4];
	D3DXVECTOR3 VecB;
	bool		bDraw = false;
	int nCount = 0;

	//�S���_�̈ʒu�����߂�
	Pos[0] = m_posV + D3DXVECTOR3(-500.0f, 0.0f, 450.0f);
	Pos[1] = m_posV + D3DXVECTOR3(-400.0f, 0.0f, -200.0f);
	Pos[2] = m_posV + D3DXVECTOR3(450.0f, 0.0f, -200.0f);
	Pos[3] = m_posV + D3DXVECTOR3(500.0f, 0.0f, 450.0f);

	//VecA�����߂�
	for (int nCntVec = 0; nCntVec < 4; nCntVec++)
	{
		int nCnt = nCntVec + 1;
		if ((nCnt % 4) == 0) { nCnt = 0; }

		VecA[nCntVec] = Pos[nCnt] - Pos[nCntVec];
	}

	//�͈̓`�F�b�N
	for (int nCntVec = 0; nCntVec < 4; nCntVec++)
	{
		VecB = ObjectPos - Pos[nCntVec];						//��p�̏I�_�ƈʒu�̃x�N�g�������߂�
		float fCross = (VecA[nCntVec].z * VecB.x) - (VecA[nCntVec].x * VecB.z);	//�O�ς����߂�

		//�x�N�g����菬�������ǂ���
		if (fCross <= 0)
		{
			nCount++;
		}
		else
		{
			break;
		}
	}

	//�͈͓��ɓ����Ă��邩�ǂ���
	if (nCount >= 4) { bDraw = true; }

	return bDraw;
}

//=============================================================================
// �J�����̏������ʒu�ɐݒ�
//=============================================================================
void CCamera::ReSetCamera(void)
{
	m_posV = D3DXVECTOR3(LENGTH_X, LENGTH_Y, -LENGTH_Z);	//���_
	m_posR = D3DXVECTOR3(0, POSR_Y, 0);						//�����_
}

//=============================================================================
// �p�x�C��
//=============================================================================
void CCamera::RemakeAngle(float * pAngle)
{
	//�p�x�̏C��
	if (*pAngle < -D3DX_PI)
	{
		*pAngle += D3DX_PI * 2.0f;
	}
	if (*pAngle > D3DX_PI)
	{
		*pAngle -= D3DX_PI * 2.0f;
	}
}

//=============================================================================
// �J�����̉�]���擾
//=============================================================================
D3DXVECTOR3 CCamera::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
// �f�o�b�O�p�̃J�����̃L�[
//=============================================================================
void CCamera::DebugUpdate(CInputKeyboard * pInputKeyboard)
{
	//�J�����̓��쏈��
#if 1
	if (pInputKeyboard->GetKeyboardPress(DIK_D) == true)		 //A�L�[(��)�������ꂽ���ǂ���
	{
		//���_�X�V
		m_posV.x += sinf(D3DX_PI * -0.5f + m_Rot.y) * CAMERA_SPEED;
		m_posV.z += cosf(D3DX_PI * -0.5f + m_Rot.y) * CAMERA_SPEED;

		//�����_�X�V
		m_posR.x = m_posV.x + sinf(D3DX_PI + m_Rot.y) * m_Lngth;
		m_posR.z = m_posV.z + cosf(D3DX_PI + m_Rot.y) * m_Lngth;
	}
	if (pInputKeyboard->GetKeyboardPress(DIK_A) == true)	 //D�L�[(�E)�������ꂽ���ǂ���
	{
		//���_�X�V
		m_posV.x += sinf(D3DX_PI * 0.5f + m_Rot.y) * CAMERA_SPEED;
		m_posV.z += cosf(D3DX_PI * 0.5f + m_Rot.y) * CAMERA_SPEED;

		//�����_�X�V
		m_posR.x = m_posV.x + sinf(D3DX_PI + m_Rot.y) * m_Lngth;
		m_posR.z = m_posV.z + cosf(D3DX_PI + m_Rot.y) * m_Lngth;
	}
	if (pInputKeyboard->GetKeyboardPress(DIK_S) == true)	 //W�L�[(��)�������ꂽ���ǂ���
	{
		//���_�X�V
		m_posV.x += sinf(0.0f + m_Rot.y) * CAMERA_SPEED;
		m_posV.z += cosf(0.0f + m_Rot.y) * CAMERA_SPEED;

		//�����_�X�V
		m_posR.x = m_posV.x + sinf(D3DX_PI + m_Rot.y) * m_Lngth;
		m_posR.z = m_posV.z + cosf(D3DX_PI + m_Rot.y) * m_Lngth;
	}
	if (pInputKeyboard->GetKeyboardPress(DIK_W) == true)	 //S�L�[(��)�������ꂽ���ǂ���
	{
		//���_�X�V
		m_posV.x += sinf(D3DX_PI + m_Rot.y) * CAMERA_SPEED;
		m_posV.z += cosf(D3DX_PI + m_Rot.y) * CAMERA_SPEED;

		//�����_�X�V
		m_posR.x = m_posV.x + sinf(D3DX_PI + m_Rot.y) * m_Lngth;
		m_posR.z = m_posV.z + cosf(D3DX_PI + m_Rot.y) * m_Lngth;
	}

	if (pInputKeyboard->GetKeyboardPress(DIK_Y) == true)	 //Z�L�[(��)�������ꂽ���ǂ���
	{
		//���_�X�V
		m_posV.y += 1.0f;
	}
	if (pInputKeyboard->GetKeyboardPress(DIK_N) == true)	 //C�L�[(��)�������ꂽ���ǂ���
	{
		//���_�X�V
		m_posV.y -= 1.0f;
	}
	if (pInputKeyboard->GetKeyboardPress(DIK_T) == true)	 //Z�L�[(��)�������ꂽ���ǂ���
	{
		//���_�X�V
		m_posV.y += 1.0f;

		//�����_�X�V
		m_posR.y += 1.0f;
	}
	if (pInputKeyboard->GetKeyboardPress(DIK_B) == true)	 //C�L�[(��)�������ꂽ���ǂ���
	{
		//���_�X�V
		m_posV.y -= 1.0f;

		//�����_�X�V
		m_posR.y -= 1.0f;
	}

#endif
}

//*****************************************************************************
//
// ��]�J�����N���X
//
//*****************************************************************************
//=============================================================================
//	�R���X�g���N�^
//=============================================================================
CRotationCamera::CRotationCamera()
{
	m_fRotSpeed = 0.0f;
}

//=============================================================================
//	�f�X�g���N�^
//=============================================================================
CRotationCamera::~CRotationCamera()
{

}

//=============================================================================
//	��������
//=============================================================================
CRotationCamera *CRotationCamera::Create(float posVHeight, D3DXVECTOR3 posR, float fRotSpeed, float fLength)
{
	// �������m��
	CRotationCamera *pRotCamera = new CRotationCamera;
	if (pRotCamera == NULL) { return NULL; }

	// ����������
	if (FAILED(pRotCamera->Init(posVHeight, posR, fRotSpeed, fLength)))
	{
		pRotCamera->Uninit();
		delete pRotCamera;
		return NULL;
	}

	return pRotCamera;  // �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//	����������
//=============================================================================
HRESULT CRotationCamera::Init(float posVHeight, D3DXVECTOR3 posR, float fRotSpeed, float fLength)
{
	// �e��l�̐ݒ�
	m_posV.y = posVHeight;
	m_posVDest.y = posVHeight;
	m_posR = posR;
	m_fRotSpeed = fRotSpeed;
	m_Lngth = fLength;
	m_Rot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_nViewAngle = VIEW_ANGLE;

	// ���_�̈ʒu���v�Z
	m_posV.x = sinf(m_Rot.y) * m_Lngth;
	m_posV.z = cosf(m_Rot.y) * m_Lngth;

	return S_OK;
}

//=============================================================================
//	�I������
//=============================================================================
void CRotationCamera::Uninit(void)
{

}

//=============================================================================
//	�X�V����
//=============================================================================
void CRotationCamera::Update(void)
{
	// ��]������
	m_Rot.y += m_fRotSpeed;
	m_Rot.y = CFunctionLib::CheckRotationOverPiX(m_Rot.y);
	m_Rot.y = CFunctionLib::CheckRotationOverMinusPiX(m_Rot.y);

	// ���_�̈ʒu���v�Z
	m_posV.x = m_posR.x - sinf(m_Rot.y) * m_Lngth;
	m_posV.z = m_posR.z - cosf(m_Rot.y) * m_Lngth;
}


//*****************************************************************************
//
// �Q�[���J�����N���X
//
//*****************************************************************************

//=============================================================================
//	�R���X�g���N�^
//=============================================================================
CGameCamera::CGameCamera()
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGameCamera::~CGameCamera()
{
}

//=============================================================================
// ����������
//=============================================================================
void CGameCamera::Init(CPlayer *pPlayer)
{
	CCamera::Init();

	//�ϐ��̏�����
	m_pTargetPlayer = pPlayer;
	m_fOldHight = 0.0f;
	m_type = TYPE_NORMAL;
	m_bSetRdest = false;
	m_fAddHeight = 0.0f;
	m_fAddDepth = 0.0f;

	D3DXVECTOR3 PlayerPos = INITIALIZE_VECTOR3;
	if (m_pTargetPlayer != NULL)
	{
		PlayerPos = m_pTargetPlayer->GetPos();
		if (PlayerPos.y == 0.0f)
		{
			m_fOldHight = m_posV.y;		//������ۑ�����
		}
		else
		{
			m_fOldHight = m_posV.y - PlayerPos.y;		//������ۑ�����
		}
	}
}

//=============================================================================
// �I������
//=============================================================================
void CGameCamera::Uninit(void)
{
	CCamera::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CGameCamera::Update(void)
{
	if (m_type == TYPE_NORMAL)
	{
		CCamera::Update();
	}
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void CGameCamera::Set()
{
	CCamera::Set();

	switch (m_type)
	{
	case TYPE_NORMAL: TypeNormal(); break;
	case TYPE_CLEAR:  TypeClear();  break;
	}
}

//=============================================================================
// �m�[�}����Ԃ̏���
//=============================================================================
void CGameCamera::TypeNormal(void)
{
	//�ϐ��錾
	D3DXVECTOR3 PlayerPos;
	D3DXVECTOR3 PlayerOldPos;
	D3DXVECTOR3 PlayerRot;

	CGamePad * pGamePad = CManager::GetInputGamePad();				//�Q�[���p�b�h�̎擾

	if (m_pTargetPlayer != NULL)
	{
		PlayerPos = m_pTargetPlayer->GetPos();
		PlayerOldPos = m_pTargetPlayer->GetPos();
		PlayerRot = m_pTargetPlayer->GetRot();
	}

	//�J�����̒ǔ�����
	//�ړI�̒����_�����߂�
	m_posRDest.x = PlayerPos.x + sinf(PlayerRot.y) * -10.0f;
	m_posRDest.y = PlayerPos.y + POSR_Y;
	m_posRDest.z = PlayerPos.z + cosf(PlayerRot.y) * -10.0f;

	//�ړI�̎��_�����߂�
	m_posVDest.x = PlayerPos.x + sinf(m_Rot.y) * m_Lngth;
	m_posVDest.y = PlayerPos.y + m_fOldHight + m_fAddHeight;
	m_posVDest.z = PlayerPos.z + cosf(m_Rot.y) * m_Lngth + m_fAddDepth;

	//�����_�̍X�V
	m_posR.x += (m_posRDest.x - m_posR.x) * 0.245f;
	m_posR.y += (m_posRDest.y - m_posR.y) * 0.245f;
	m_posR.z += (m_posRDest.z - m_posR.z) * 0.245f;

	//���_�̍X�V
	m_posV.x += (m_posVDest.x - m_posV.x) * 0.3f;
	m_posV.y += (m_posVDest.y - m_posV.y) * 0.3f;
	m_posV.z += (m_posVDest.z - m_posV.z) * 0.3f;
	CDebugProc::Print("%.1f\n", m_fAddHeight);
	CDebugProc::Print("%.1f\n", m_fAddDepth);

	// �N�_���l������Ă���Ȃ��ʊO���������
	if (m_pTargetPlayer == NULL)
	{
		CutAddValue(ADDHEIGHT_CUTVALUE, ADDDEPTH_CUTVALUE);
		return;
	}
	CTerritory *pTerritoryTop = m_pTargetPlayer->GetTerritoryTop();
	if (pTerritoryTop == NULL)
	{
		CutAddValue(ADDHEIGHT_CUTVALUE, ADDDEPTH_CUTVALUE);
		return;
	}

	// ��ʊO����
	D3DXVECTOR3 TopPos;
	if (CheckTerritoryTopInScreen(pTerritoryTop, TopPos) == true) { return; }
	CDebugProc::Print("��ʊO�ɋN�_\n");

	// ��ʂɋN�_�����߂�悤�Ɍv�Z
	CDebugProc::Print("�N�_ : %.1f %.1f %.1f\n", TopPos.x, TopPos.y, TopPos.z);
	m_fAddHeight += ADDHEIGHT_ADDVALUE;
	if (m_fAddHeight >= ADDHEIGHT_MAX)
	{
		m_fAddHeight = ADDHEIGHT_MAX;
	}
	m_fAddDepth += ADDDEPTH_ADDVALUE;
	if (m_fAddDepth <= ADDDEPTH_MAX)
	{
		m_fAddDepth = ADDDEPTH_MAX;
	}
}

//=============================================================================
// �N���A��Ԃ̏���
//=============================================================================
void CGameCamera::TypeClear(void)
{
	if (m_bSetRdest == false)
	{
		m_bSetRdest = true;
		m_posRDest = D3DXVECTOR3(m_posRDest.x, m_posRDest.y + 25.0f, m_posRDest.z);
	}

	m_posR.y += MOVE_CLEAR;

	if (m_posR.y > m_posRDest.y)
	{
		m_posR.y = m_posRDest.y;
	}
}

//=============================================================================
// �N�_�̉�ʊO����
//=============================================================================
bool CGameCamera::CheckTerritoryTopInScreen(CTerritory *pTerritoryTop, D3DXVECTOR3 &pos)
{
	// �r���[�|�[�g�s����쐬
	D3DXMATRIX mtxWorld, mtxViewport;
	D3DXMatrixIdentity(&mtxViewport);
	float fValue1 = (float)m_Viewport.Width / 2;
	float fValue2 = (float)m_Viewport.Height / 2;
	mtxViewport._11 = fValue1;
	mtxViewport._22 = -1.0f * fValue2;
	mtxViewport._41 = fValue1;
	mtxViewport._42 = fValue2;

	// �X�N���[���ɓ��e����s����쐬
	mtxWorld = m_mtxView * m_mtxProjection * mtxViewport;

	// �N�_���X�N���[���ɓ��e
	pos = pTerritoryTop->GetPos();
	D3DXVec3TransformCoord(&pos, &pos, &mtxWorld);

	// ��ʊO����
	float fScreenAddSizeX = SCREEN_ADDSIZE_X;
	float fScreenAddSizeY = SCREEN_ADDSIZE_Y;
	float fScreenMiddleSizeX = SCREEN_MIDDLESIZE_X;
	float fScreenMiddleSizeY = SCREEN_MIDDLESIZE_Y;
	CGame *pGame = CManager::GetGame();
	if (pGame != NULL)
	{
		if (pGame->GetNumPlay() >= 2)
		{// 2�l�ȏオ�Q�����Ă���
			fScreenAddSizeX *= 0.5f;
			fScreenMiddleSizeX *= 0.5f;
		}
		if (pGame->GetNumPlay() >= 3)
		{// 3�l�ȏオ�Q�����Ă���
			fScreenAddSizeY *= 0.5f;
			fScreenMiddleSizeY *= 0.5f;
		}
	}
	if (pos.x > fScreenAddSizeX && pos.x < m_Viewport.Width - fScreenAddSizeX
		&& pos.y > fScreenAddSizeY && pos.y < m_Viewport.Height - fScreenAddSizeY)
	{// ��ʓ��Ɏ��܂��Ă���Ȃ�^
		if (pos.x > fValue1 - fScreenMiddleSizeX && pos.x < fValue1 + fScreenMiddleSizeX
			&& pos.y > fValue2 - fScreenMiddleSizeY && pos.y < fValue2 + fScreenMiddleSizeY)
		{// ��ʂ̒����Ɏ��܂��Ă���Ȃ�J�������߂Â���
			CutAddValue(ADDHEIGHT_ADDVALUE, ADDDEPTH_ADDVALUE);
		}
		return true;
	}

	return false;
}

//=============================================================================
// ������l����鏈��
//=============================================================================
void CGameCamera::CutAddValue(float fCutHeight, float fCutDepth)
{
	m_fAddHeight -= fCutHeight;
	if (m_fAddHeight <= 0.0f)
	{
		m_fAddHeight = 0.0f;
	}
	m_fAddDepth -= fCutDepth;
	if (m_fAddDepth >= 0.0f)
	{
		m_fAddDepth = 0.0f;
	}
}

//*****************************************************************************
//
// �^�C�g���J�����N���X
//
//*****************************************************************************

//=============================================================================
//	�R���X�g���N�^
//=============================================================================
CTitleCamera::CTitleCamera()
{
	m_nCounterTime = 0;		//�J�����̎��ԃJ�E���^�[
	m_type = TYPE_00;		//�J�����̃A���O���^�C�v
	m_bSetCamera = false;	//�J�����̈ʒu��ݒ肵�����ǂ���
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTitleCamera::~CTitleCamera()
{
}

//=============================================================================
// ����������
//=============================================================================
void CTitleCamera::Init(void)
{
	CCamera::Init();

	m_posV = D3DXVECTOR3(TITILE_LENGTH_X, TITILE_LENGTH_Y, -350.0f);	//���_
	m_posR = D3DXVECTOR3(0, 38, 0);												//�����_
	m_bSetCamera = true;
}

//=============================================================================
// �I������
//=============================================================================
void CTitleCamera::Uninit(void)
{
	CCamera::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTitleCamera::Update(void)
{
	CCamera::Update();

	m_nCounterTime++;

	if (m_nCounterTime > CHANGE_TIME)
	{
		m_nCounterTime = 0;
		m_type = (TYPE)((m_type + 1) % TYPE_MAX);	//�^�C�v�̕ύX
		m_bSetCamera = false;
	}

	switch (m_type)
	{
	case TYPE_00:
		if (m_bSetCamera == false)
		{
			m_posV = D3DXVECTOR3(TITILE_LENGTH_X, TITILE_LENGTH_Y, -TITILE_LENGTH_Z);	//���_
			m_posR = D3DXVECTOR3(0, 38, 0);												//�����_
			m_bSetCamera = true;
		}
		m_posV.y += MOVE_Y;
		m_posR.y += MOVE_Y;
		break;

	case TYPE_01:
		if (m_bSetCamera == false)
		{
			m_posV = D3DXVECTOR3(TITILE_LENGTH_X, 150, -500);	//���_
			m_posR = D3DXVECTOR3(0, -38, 0);					//�����_
			m_bSetCamera = true;
		}

		m_posV.z += MOVE_Z;
		m_posR.z += MOVE_Z;
		break;
	}
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void CTitleCamera::Set(void)
{
	CCamera::Set();
}

//*****************************************************************************
//
// ���U���g�J�����N���X
//
//*****************************************************************************

//=============================================================================
//	�R���X�g���N�^
//=============================================================================
CResultCamera::CResultCamera()
{
	m_TargetCharPos = INITIALIZE_VECTOR3;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CResultCamera::~CResultCamera()
{
}

//=============================================================================
// ����������
//=============================================================================
void CResultCamera::Init(void)
{
	CCamera::Init();

	m_posV = D3DXVECTOR3(500, TITILE_LENGTH_Y, 000);	//���_
	m_posR = D3DXVECTOR3(500, 10, 300);					//�����_
	m_EndPosV = D3DXVECTOR3(0.0f, 60.0f, -100.0f);	//�ŏI���_
	m_ZoomOutPos = D3DXVECTOR3(0.0f, 40.0f, -190.0f);	//�Y�[���A�E�g�p�̕ϐ�
	m_MoveV = INITIALIZE_VECTOR3;						//���_�̈ړ���
	m_MoveR = INITIALIZE_VECTOR3;						//�����_�̈ړ���
	m_state = STATE_CHAR_STOP;							//�J������~���
	m_nCountTime = 0;

	//�ŏI�����_�̐ݒ�
	D3DXVECTOR3 Pos = m_TargetCharPos;
	Pos.y += 80.0f;
	Pos.x += -30.0f;

	m_EndPosR = Pos;		//�ŏI�����_
}

//=============================================================================
// �I������
//=============================================================================
void CResultCamera::Uninit(void)
{
	CCamera::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CResultCamera::Update(void)
{
	CCamera::Update();

	switch (m_state)
	{
	case STATE_CHAR_IN: UpdateCharIn(); break;
	case STATE_CHAR_ZOOM: UpdateCharZoom(); break;
	case STATE_CHAR_ZOOMOUT: UpdateCharZoomOut(); break;
	}
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void CResultCamera::Set(void)
{
	CCamera::Set();
}

//=============================================================================
// �L�������J�������ɓ������
//=============================================================================
void CResultCamera::UpdateCharIn(void)
{
	//�ϐ��錾
	bool bEndX = false;
	bool bEndY = false;
	bool bEndZ = false;

	//�J�E���^�[�̉��Z
	m_nCountTime++;

	//�ړI�̒����_�̊p�x�����߂�
	float fAngle = CFunctionLib::CalcAngleToDest(m_posR.x, m_posR.z, m_EndPosR.x, m_EndPosR.z);

	//�ړ��ʂ̍X�V����
	m_MoveR.x += sinf(fAngle) * 0.25f;
	m_MoveR.z += cosf(fAngle) * 0.25f;
	m_MoveR.y += -0.25f;

	//�����_�̍X�V
	m_posR.x += m_MoveR.x;
	m_posR.z += m_MoveR.z;
	m_posR.y += m_MoveR.y;

	//�l�̏C��
	if (m_posR.x > m_EndPosR.x) { m_posR.x = m_EndPosR.x; } // X��
	if (m_posR.z < m_EndPosR.z) { m_posR.z = m_EndPosR.z; } // Z��
	if (m_posR.y < m_EndPosR.y) { m_posR.y = m_EndPosR.y; } // Y��

	if (m_nCountTime > 20)
	{
		//�ړI�̒����_�̊p�x�����߂�
		float fAnglePosV = CFunctionLib::CalcAngleToDest(m_posV.x, m_posV.z, m_ZoomOutPos.x, m_ZoomOutPos.z);

		//�ړ��ʂ̍X�V����
		m_MoveV.x += sinf(fAnglePosV) * 0.2f;
		m_MoveV.z += cosf(fAnglePosV) * 0.2f;
		m_MoveV.y += -0.04f;

		//�����_�̍X�V
		m_posV.x += m_MoveV.x;
		m_posV.z += m_MoveV.z;
		m_posV.y += m_MoveV.y;

		//�l�̏C��
		if (m_posV.x > m_ZoomOutPos.x)
		{// X��
			m_posV.x = m_ZoomOutPos.x;
			bEndX = true;
		}
		if (m_posV.z < m_ZoomOutPos.z)
		{// Z��
			m_posV.z = m_ZoomOutPos.z;
			bEndZ = true;
		}
		if (m_posV.y < m_ZoomOutPos.y)
		{// Y��
			m_posV.y = m_ZoomOutPos.y;
			bEndY = true;
		}
	}

	//��������
	m_MoveR.x += (0.0f - m_MoveR.x) * 0.1f;
	m_MoveR.y += (0.0f - m_MoveR.y) * 0.1f;
	m_MoveR.z += (0.0f - m_MoveR.z) * 0.1f;

	m_MoveV.x += (0.0f - m_MoveV.x) * 0.1f;
	m_MoveV.y += (0.0f - m_MoveV.y) * 0.1f;
	m_MoveV.z += (0.0f - m_MoveV.z) * 0.1f;

	//�ړI�̈ʒu�܂ňړ��������Ԃ�ύX����
	if ((bEndX == true) && (bEndY == true) && (bEndZ == true))
	{
		SetState(STATE_CHAR_ZOOM);
		m_MoveV = INITIALIZE_VECTOR3;
	}
}

//=============================================================================
// �L�������Y�[��������
//=============================================================================
void CResultCamera::UpdateCharZoom(void)
{
	//�ϐ��錾
	bool bEndX = false;
	bool bEndY = false;
	bool bEndZ = false;

	//�ړI�̒����_�̊p�x�����߂�
	float fAnglePosV = CFunctionLib::CalcAngleToDest(m_posV.x, m_posV.z, m_EndPosV.x, m_EndPosV.z);

	//�ړ��ʂ̍X�V����
	m_MoveV.x += sinf(fAnglePosV) * 0.25f;
	m_MoveV.z += cosf(fAnglePosV) * 0.25f;
	m_MoveV.y += 0.0525f;

	//�����_�̍X�V
	m_posV.x += m_MoveV.x;
	m_posV.z += m_MoveV.z;
	m_posV.y += m_MoveV.y;

	//�l�̏C��
	if (m_posV.x < m_EndPosV.x)
	{// X��
		m_posV.x = m_EndPosV.x;
		bEndX = true;
	}
	if (m_posV.z > m_EndPosV.z)
	{// Z��
		m_posV.z = m_EndPosV.z;
		bEndZ = true;
	}
	if (m_posV.y > m_EndPosV.y)
	{// Y��
		m_posV.y = m_EndPosV.y;
		bEndY = true;
	}

	//�ړI�̈ʒu�܂ňړ��������Ԃ�ύX����
	if ((bEndX == true) && (bEndY == true) && (bEndZ == true)) { SetState(STATE_CHAR_STOP); }
}

//=============================================================================
// �L�������Y�[���A�E�g������
//=============================================================================
void CResultCamera::UpdateCharZoomOut(void)
{
}

//*****************************************************************************
//
// �����L���O�J�����N���X
//
//*****************************************************************************

//=============================================================================
//	�R���X�g���N�^
//=============================================================================
CRankingCamera::CRankingCamera()
{
	m_fSpeed = SPEED;
	m_fCountDistance = 0.0f;
	m_fMove = MOVE;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CRankingCamera::~CRankingCamera()
{
}

//=============================================================================
// ����������
//=============================================================================
void CRankingCamera::Init(void)
{
	CCamera::Init();

	m_posV = D3DXVECTOR3(500, TITILE_LENGTH_Y, 000);	//���_
	m_posR = D3DXVECTOR3(500, 10, 300);												//�����_
}

//=============================================================================
// �I������
//=============================================================================
void CRankingCamera::Uninit(void)
{
	CCamera::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CRankingCamera::Update(void)
{
	CCamera::Update();

	//�J�����̈ʒu�X�V
	m_posV.z += m_fMove;
	m_posR.z += m_fMove;

	//�J�����Ɋ�����t����
	m_fMove += (0.0f - m_fMove) * WINDY;
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void CRankingCamera::Set(void)
{
	CCamera::Set();
}

//*****************************************************************************
//
// �C�x���g�J�����N���X
//
//*****************************************************************************

//=============================================================================
//	�R���X�g���N�^
//=============================================================================
CEventCamera::CEventCamera()
{
	m_pTargetRobot = NULL;
	m_nCounter = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEventCamera::~CEventCamera()
{
}

//=============================================================================
// ����������
//=============================================================================
void CEventCamera::Init(D3DXVECTOR3 posV, D3DXVECTOR3 posR, CCharacter *pChar, TYPE type)
{
	CCamera::Init();

	m_pTargetRobot = pChar;
	m_Type = type;

	m_posV = posV;		// ���_
	m_posR = posR;		// �����_

	// ������ۑ�
	m_Lngth = CFunctionLib::Vec2Length(posV.x, posV.z, posR.x, posR.z);
}

//=============================================================================
// �I������
//=============================================================================
void CEventCamera::Uninit(void)
{
	CCamera::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEventCamera::Update(void)
{
	switch (m_Type)
	{// �C�x���g�̎�ނɂ���ď����킯
	case TYPE_ROBOT:
		RobotEvent();
		break;
	case TYPE_AREABONUS_START:
		AreaBonusEvent_Start();
		break;
	case TYPE_AREABONUS_FINISH:
		AreaBonusEvent_Finish();
		break;
	case TYPE_DIVISION_START:
		DivisionEvent_Start();
		break;
	case TYPE_DIVISION_FINISH:
		DivisionEvent_Finish();
		break;
	}
}

//=============================================================================
// ���{�b�g�C�x���g���̍X�V����
//=============================================================================
void CEventCamera::RobotEvent(void)
{
	CGame *pGame = CManager::GetGame();		// �Q�[���̎擾

	CCamera::Update();

	D3DXVECTOR3 RobotPos = m_pTargetRobot->GetPos();	// ���{�b�g���W�̎擾

	m_posR = RobotPos + D3DXVECTOR3(0.0f, 100.0f, 0.0f);	// �����_�̍X�V

	CRobot *pRobot = pGame->GetRobot();		// ���{�b�g�̎擾

	if (pRobot->GetEndGravity() == true)
	{// ���{�b�g�����n���Ă�����
		if (m_nCounter >= UNINIT_TIME_ROBOT)
		{// �����鎞�ԂɂȂ�����
			pGame->SetEveCamFlag(true);	// ���S�t���O
			m_nCounter = 0;
		}
		m_nCounter++;	// �J�E���^�[�̉��Z
	}
}

//=============================================================================
// �G���A�{�[�i�X�C�x���g�J�n���̍X�V����
//=============================================================================
void CEventCamera::AreaBonusEvent_Start(void)
{
	// �Q�[���N���X�̎擾
	CGame *pGame = CManager::GetGame();
	if (pGame == NULL) { return; }

	// �n�ʂ̍X�V
	pGame->UpdateAreaBonusField_Start();

	// �J�E���^�[����
	m_nCounter++;
	if (m_nCounter >= UNINIT_TIME_AREABONUS_START)
	{// �I�����ԂɂȂ���
		pGame->SetEveCamFlag(true);
		pGame->ReleaseAreaBounsEventUI();
	}
}

//=============================================================================
// �G���A�{�[�i�X�C�x���g�I�����̍X�V����
//=============================================================================
void CEventCamera::AreaBonusEvent_Finish(void)
{
	// �Q�[���N���X�̎擾
	CGame *pGame = CManager::GetGame();
	if (pGame == NULL) { return; }

	// �n�ʂ̍X�V
	pGame->UpdateAreaBonusField_Finish();

	// �J�E���^�[����
	m_nCounter++;
	if (m_nCounter >= UNINIT_TIME_AREABONUS_FINISH)
	{// �I�����ԂɂȂ���
		pGame->SetEveCamFlag(true);
		pGame->ReleaseAreaBounsEventUI();
		pGame->SetAreaBonusEventFlag(false);
		pGame->HideEventField();
	}
}

//=============================================================================
// ���f�C�x���g�J�n���̍X�V����
//=============================================================================
void CEventCamera::DivisionEvent_Start(void)
{
	// �J��������]������
	m_Rot.y += DIVISION_ROTSPPED;
	m_Rot.y = CFunctionLib::CheckRotationOverPiX(m_Rot.y);
	m_Rot.y = CFunctionLib::CheckRotationOverMinusPiX(m_Rot.y);

	// ���_�ʒu�̐ݒ�
	m_posV.x = m_posR.x + sinf(m_Rot.y) * m_Lngth;
	m_posV.z = m_posR.z + cosf(m_Rot.y) * m_Lngth;

	// �Q�[���N���X�̎擾
	CGame *pGame = CManager::GetGame();
	if (pGame == NULL) { return; }

	// �ǂ̍X�V
	pGame->UpdateDivisionWall_Start();

	// �J�E���^�[����
	m_nCounter++;
	if (m_nCounter >= UNINIT_TIME_DIVISION_START)
	{// �I�����ԂɂȂ���
		pGame->SetEveCamFlag(true);
		pGame->ReleaseDivisionEventUI();
	}
}

//=============================================================================
// ���f�C�x���g�I�����̍X�V����
//=============================================================================
void CEventCamera::DivisionEvent_Finish(void)
{
	// �J��������]������
	m_Rot.y += DIVISION_ROTSPPED;
	m_Rot.y = CFunctionLib::CheckRotationOverPiX(m_Rot.y);
	m_Rot.y = CFunctionLib::CheckRotationOverMinusPiX(m_Rot.y);

	// ���_�ʒu�̐ݒ�
	m_posV.x = m_posR.x + sinf(m_Rot.y) * m_Lngth;
	m_posV.z = m_posR.z + cosf(m_Rot.y) * m_Lngth;

	// �Q�[���N���X�̎擾
	CGame *pGame = CManager::GetGame();
	if (pGame == NULL) { return; }

	// �ǂ̍X�V
	pGame->UpdateDivisionWall_Finish();

	// �J�E���^�[����
	m_nCounter++;
	if (m_nCounter >= UNINIT_TIME_DIVISION_FINISH)
	{// �I�����ԂɂȂ���
		pGame->SetEveCamFlag(true);
		pGame->ReleaseDivisionEventUI();
		pGame->ReleaseDivisionWall();
		pGame->SetDivisionEventFlag(false);
	}
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void CEventCamera::Set(void)
{
	CCamera::Set();
}