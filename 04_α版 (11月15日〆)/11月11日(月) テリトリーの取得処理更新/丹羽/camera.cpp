//=============================================================================
//
// �J�������� [camera.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "gamepad.h"
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

//=============================================================================
// �}�N����`
//=============================================================================
#define DRAW_CAMERA		(3500.0f)	//�J�����̕`��͈�
#define CAMERA_SPEED	(2.0f)		//�J�����̃X�s�[�h
#define LENGTH_X		(0.0f)		//���_�ƒ����_�̋����i�w�j
#define LENGTH_Y		(500.0f)	//���_�ƒ����_�̋����i�x�j30 700.0f
#define LENGTH_Z		(180.0f)	//���_�ƒ����_�̋����i�y�j150
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
#define MOVE_CLEAR		(0.2f)		//�N���A���̈ړ���


//�����L���O�J����
#define WINDY			(0.05f)		//����
#define SPEED			(0.3f)		//�J�����̈ړ����x
#define MOVE			(20.0f)		//�ړ��͈�
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

	if ((pInputKeyboard->GetKeyboardPress(DIK_Z) == true) || (pGamePad->GetGamePadRightStickPress(CGamePad::ANALOG_STICK_RIGHT) == true))	 //Z�L�[(��)�������ꂽ���ǂ���
	{
		//�J�����̉�]��
		m_RotDest.y += 0.01f;

		//�ړI�̊p�x�C��
		RemakeAngle(&m_RotDest.y);

		//��]���Ă����Ԃɂ���
		m_bRot = true;
	}
	if ((pInputKeyboard->GetKeyboardPress(DIK_C) == true) || (pGamePad->GetGamePadRightStickPress(CGamePad::ANALOG_STICK_LEFT) == true))	 //C�L�[(��)�������ꂽ���ǂ���
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
	int nTypePlayer = m_pTargetPlayer->GetPlayerType();

	if (m_pTargetPlayer != NULL)
	{
		PlayerPos = m_pTargetPlayer->GetPos();
		PlayerOldPos = m_pTargetPlayer->GetPos();
		PlayerRot = m_pTargetPlayer->GetRot();
	}

	if (pGamePad->GetGamePadRightStickPress(CGamePad::ANALOG_STICK_UP) == true)
	{
		//���_�X�V
		m_posV.y += 1.0f;
		m_bHight = true;

		if (m_posV.y > (MAX_HIGHT + PlayerPos.y))
		{
			m_posV.y = MAX_HIGHT + PlayerPos.y;
			m_bHight = false;
		}
	}
	if (pGamePad->GetGamePadRightStickPress(CGamePad::ANALOG_STICK_DOWN) == true)
	{
		//���_�X�V
		m_posV.y -= 1.0f;
		m_bHight = true;

		if (m_posV.y < MIN_HIGHT + PlayerPos.y)
		{
			m_posV.y = MIN_HIGHT + PlayerPos.y;
			m_bHight = false;
		}
	}

	if (PlayerPos.y == 0.0f)
	{
		m_fOldHight = m_posV.y;		//������ۑ�����
	}
	else
	{
		m_fOldHight = m_posV.y - PlayerPos.y;		//������ۑ�����
	}

	//�J�����̒ǔ�����
	//�ړI�̒����_�����߂�
	m_posRDest.x = PlayerPos.x + sinf(PlayerRot.y) * -25.0f;
	m_posRDest.y = PlayerPos.y + POSR_Y;
	m_posRDest.z = PlayerPos.z + cosf(PlayerRot.y) * -25.0f;

	//�ړI�̎��_�����߂�
	m_posVDest.x = PlayerPos.x + sinf(m_Rot.y) * m_Lngth;
	m_posVDest.y = PlayerPos.y + m_fOldHight;
	m_posVDest.z = PlayerPos.z + cosf(m_Rot.y) * m_Lngth;

	//�����_�̍X�V
	m_posR.x += (m_posRDest.x - m_posR.x) * 0.245f;
	m_posR.y += (m_posRDest.y - m_posR.y) * 0.245f;
	m_posR.z += (m_posRDest.z - m_posR.z) * 0.245f;

	//���_�̍X�V
	m_posV.x += (m_posVDest.x - m_posV.x) * 0.3f;
	m_posV.y += (m_posVDest.y - m_posV.y) * 0.3f;
	m_posV.z += (m_posVDest.z - m_posV.z) * 0.3f;
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