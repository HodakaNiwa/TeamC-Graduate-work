//*****************************************************************************
//
//     ���̋O�Ղ̏���[lightorbit.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "lightorbit.h"
#include "manager.h"
#include "library.h"
#include "effect3D.h"
#include "character.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define LIGHTORBIT_MAX_ACCEL        (13.0f)		// �ʒu�̉����l�̍ő�l
#define LIGHTORBIT_MAX_ACCELTIME    (120)		// �ő�̑����ɂȂ�܂ł̎���
#define LIGHTORBIT_DEATH_LENGTH     (10.0f)		// �����͈�
#define LIGHTORBIT_MAX_HEIGHT       (50.0f)		// ����

#define LIGHTORBIT_EFFECT_SPAWN     (1)			// �G�t�F�N�g���o���Ԋu
#define LIGHTORBIT_EFFECT_WIDTH     (23.0f)		// �G�t�F�N�g�̕�
#define LIGHTORBIT_EFFECT_HEIGHT    (23.0f)		// �G�t�F�N�g�̍���
#define LIGHTORBIT_EFFECT_LIFE      (90)		// �G�t�F�N�g�̎���
#define LIGHTORBIT_EFFECT_CUTALPHA  (0.08f)		// �G�t�F�N�g�̓����x������
#define LIGHTORBIT_EFFECT_CUTLENGTH (0.7f)		// �G�t�F�N�g�̑傫��������
#define LIGHTORBIT_EFFECT_PRIORITY  (7)			// �G�t�F�N�g�̏����D�揇��

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CLightOrbit::CLightOrbit(int nPriority, OBJTYPE objType) : CScene(nPriority, objType){}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CLightOrbit::~CLightOrbit(){}

//=============================================================================
//    ��������
//=============================================================================
CLightOrbit *CLightOrbit::Create(D3DXVECTOR3 pos, D3DXVECTOR3 *pDestPos, int nDestPlayerIdx, int nPriority)
{
	// �������̊m��
	CLightOrbit *pLightOrbit = NULL;
	pLightOrbit = new CLightOrbit(nPriority);
	if (pLightOrbit == NULL) { return NULL; }

	// �ϐ��̏�����
	pLightOrbit->ClearVariable();

	// �e��l�̑��
	pLightOrbit->SetPos(pos);						// �|���S���̍��W
	pLightOrbit->SetDestPos(pDestPos);				// �ړI�̍��W
	pLightOrbit->SetDestPlayerIdx(nDestPlayerIdx);	// �ړI�̃v���C���[�ԍ�

	// ����������
	if (FAILED(pLightOrbit->Init()))
	{
		pLightOrbit->Uninit();
		return NULL;
	}

	return pLightOrbit;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CLightOrbit::Init(void)
{
	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CLightOrbit::Uninit(void)
{
	// �����[�X����
	CScene::Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CLightOrbit::Update(void)
{
	// �J�E���^�[���Z
	m_nCounter++;

	// �ړ��ʌv�Z����
	bool bDeath = false;
	bDeath = CalcMove();

	// �ʒu���ړ�������
	Movement();

	// �G�t�F�N�g(3D)�𐶐�����
	CreateEffect3D();

	// ���S����
	if (bDeath == true)
	{
		Uninit();
	}
}

//=============================================================================
//    �ړ��ʂ��v�Z���鏈��
//=============================================================================
bool CLightOrbit::CalcMove(void)
{
	bool bDeath = false;

	// �p�x���o��
	float fAngle = CFunctionLib::CalcAngleToDest(m_Pos.x, m_Pos.z, m_pDestPos->x, m_pDestPos->z);

	// �ړI�n�Ɉړ�������
	m_Move.x = sinf(fAngle) * m_fAccel;
	m_Move.z = cosf(fAngle) * m_fAccel;

	// �������v�Z
	float fLength = CFunctionLib::Vec2Length(m_Pos.x, m_Pos.z, m_pDestPos->x, m_pDestPos->z);
	if (fLength <= LIGHTORBIT_DEATH_LENGTH) { bDeath = true; }

	return bDeath;
}

//=============================================================================
//    �ʒu���ړ������鏈��
//=============================================================================
void CLightOrbit::Movement(void)
{
	// �ʒu���ړ�������
	m_Pos += m_Move;
	m_Pos.y = LIGHTORBIT_MAX_HEIGHT;

	// �����l�̌v�Z
	float fRivision = (float)m_nCounter / (float)LIGHTORBIT_MAX_ACCELTIME;
	if (fRivision >= 1.0f)
	{
		fRivision = 1.0f;
	}
	m_fAccel = sinf((D3DX_PI * 0.5f) * fRivision) * LIGHTORBIT_MAX_ACCEL;
}

//=============================================================================
//    �G�t�F�N�g(3D)�𐶐����鏈��
//=============================================================================
void CLightOrbit::CreateEffect3D(void)
{
	if (m_nCounter % LIGHTORBIT_EFFECT_SPAWN != 0) { return; }
	D3DXCOLOR col = CCharacter::m_CountryColor[m_nDestPlayerIdx];
	CEffect3D::Create(m_Pos, col, LIGHTORBIT_EFFECT_WIDTH, LIGHTORBIT_EFFECT_HEIGHT,
		LIGHTORBIT_EFFECT_LIFE, LIGHTORBIT_EFFECT_CUTALPHA, LIGHTORBIT_EFFECT_CUTLENGTH,
		false, LIGHTORBIT_EFFECT_PRIORITY);
	switch (m_nDestPlayerIdx)
	{
	case 1:
		CEffect3D::Create(m_Pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), LIGHTORBIT_EFFECT_WIDTH * 0.6f, LIGHTORBIT_EFFECT_HEIGHT  * 0.6f,
			LIGHTORBIT_EFFECT_LIFE, LIGHTORBIT_EFFECT_CUTALPHA, LIGHTORBIT_EFFECT_CUTLENGTH,
			false, LIGHTORBIT_EFFECT_PRIORITY);
		break;
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CLightOrbit::Draw(void)
{

}

//=============================================================================
//    �ϐ��̏���������
//=============================================================================
void CLightOrbit::ClearVariable(void)
{
	m_Pos = INITIALIZE_VECTOR3;
	m_Move = INITIALIZE_VECTOR3;
	m_pDestPos = NULL;
	m_nCounter = 0;
	m_nDestPlayerIdx = 0;
	m_fAccel = 0.0f;
}