//=============================================================================
//
// ���C�g���� [light.cpp]
// Author : �������� Sato_Asumi 
//
//=============================================================================
#include "light.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "renderer.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_TIME	(60 * 2)

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLight::CLight(){}
	
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLight::~CLight(){}

//=============================================================================
// ���C�g�̏���������
//=============================================================================
void CLight::Init(void)
{
	//�ϐ��̏�����
	m_bSwich = true;
	m_bSetLight = false;
	m_type = TYPE_NORMAL;
	m_nNumLight = 0;
	m_nNumDir = 0;
	m_nCntTime = 0;
	m_Col = D3DXCOLOR(0.7f, 0.75f, 1.0f, 1.0f);
	m_AimCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// ���C�g���N���A����
	for (int nCntLight = 0; nCntLight < MAXLIGHT; nCntLight++)
	{
		ZeroMemory(&m_light[nCntLight], sizeof(D3DLIGHT9));

		// ���C�g�̎�ނ�ݒ�
		m_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;
	}

	// ���C�g�̊g�U����ݒ�
	m_light[0].Diffuse = D3DXCOLOR(0.7f, 0.75f, 1.0f, 1.0f);
	m_light[1].Diffuse = D3DXCOLOR(0.30f, 0.35f, 0.40f, 1.0f);
	m_light[2].Diffuse = D3DXCOLOR(0.30f, 0.35f, 0.40f, 1.0f);

	// ���C�g�̕����̐ݒ�
	m_vecDir[0] = D3DXVECTOR3(0.2f, -0.74f, 0.64f);		//���𓖂������
	m_vecDir[1] = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);	//���𓖂������
	m_vecDir[2] = D3DXVECTOR3(0.89f, -0.11f, 0.44f);	//���𓖂������

	SetLight();		//���C�g�̐ݒ�
}

//=============================================================================
// ���C�g�̏I������
//=============================================================================
void CLight::Uninit(void)
{
}

//=============================================================================
// ���C�g�̍X�V����
//=============================================================================
void CLight::Update(void)
{
	//�ϐ��錾
	bool bSet = false;

	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//�L�[�{�[�h�̎擾

	if (pInputKeyboard->GetKeyboardTrigger(DIK_7) == true)
	{
		m_bSwich = m_bSwich ? false : true;
	}

	if (pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD0) == true)
	{
		// ���C�g�̊g�U����ݒ�
		m_light[0].Diffuse = D3DXCOLOR(0.90f, 0.95f, 1.0f, 1.0f);
		m_light[1].Diffuse = D3DXCOLOR(0.40f, 0.40f, 0.40f, 1.0f);
		m_light[2].Diffuse = D3DXCOLOR(0.40f, 0.40f, 0.40f, 1.0f);
		SetLight();
	}

	if (m_bSwich == true)
	{//���C�g�̍X�V
		ChangeNumber(&m_nNumLight, pInputKeyboard);				//�����̐؂�ւ�
		bSet = SetValue(&m_light[m_nNumLight].Diffuse.r,		//���l�̐ݒ�
				 &m_light[m_nNumLight].Diffuse.g,
				 &m_light[m_nNumLight].Diffuse.b,
				 pInputKeyboard);
	}
	else
	{//���C�g�̌����̐ݒ�
		ChangeNumber(&m_nNumDir, pInputKeyboard);		//�����̐؂�ւ�
		bSet = SetValue(&m_vecDir[m_nNumDir].x,				//���l�̐ݒ�
				 &m_vecDir[m_nNumDir].y,
				 &m_vecDir[m_nNumDir].z,
				 pInputKeyboard);
	}

	if (bSet == true) { SetLight(); }						//���C�g�̐ݒ�
}

//=============================================================================
// ���C�g�̐ݒ�
//=============================================================================
void CLight::SetLight(void)
{
	//���C�g��ݒ肷��
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	for (int nCntLight = 0; nCntLight < MAXLIGHT; nCntLight++)
	{
		D3DXVec3Normalize(&m_vecDir[nCntLight], &m_vecDir[nCntLight]);		//���K������
		m_light[nCntLight].Direction = m_vecDir[nCntLight];				//���߂��傫����������

																		// ���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &m_light[nCntLight]);

		// ���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}
}


//=============================================================================
// �F�̐ݒ�
//=============================================================================
void  CLight::SetColor(void)
{
	//�F�̊�����
	m_light[0].Diffuse = D3DXCOLOR(m_AimCol.r, m_AimCol.g, m_AimCol.b, 1.0f);
	m_light[1].Diffuse = D3DXCOLOR(0.45f, 0.45f, 0.45f, 1.0f);
	m_light[2].Diffuse = D3DXCOLOR(0.45f, 0.45f, 0.45f, 1.0f);

	SetLight();	//���C�g�̐ݒ�
}

//=============================================================================
// �����̐؂�ւ�
//=============================================================================
void CLight::ChangeNumber(int * nNum, CInputKeyboard * pInputKeyboard)
{
	//�����̐؂�ւ�
	if (pInputKeyboard->GetKeyboardTrigger(DIK_NUMPADPLUS) == true)
	{
		*nNum = (*nNum + (MAXLIGHT - 1)) % MAXLIGHT;
	}
	else if (pInputKeyboard->GetKeyboardTrigger(DIK_NUMPADMINUS) == true)
	{
		*nNum = (*nNum + 1) % MAXLIGHT;
	}
}

//=============================================================================
// ���l�̐؂�ւ�
//=============================================================================
bool CLight::SetValue(float * fValue_X, float * fValue_Y, float * fValue_Z, CInputKeyboard * pInputKeyboard)
{
	//�ϐ��錾
	bool bSet = false;

	//fValue_X�̐��l�̐؂�ւ�
	if (pInputKeyboard->GetKeyboardPress(DIK_NUMPAD7) == true)
	{
		*fValue_X += 0.01f;
		if (*fValue_X > 1.0f) { *fValue_X = 1.0f; }
		bSet = true;
	}
	if (pInputKeyboard->GetKeyboardPress(DIK_NUMPAD4) == true)
	{
		*fValue_X -= 0.01f;
		if (*fValue_X < 0.0f) { *fValue_X = 0.0f; }
		bSet = true;
	}

	//fValue_Y�̐��l�̐؂�ւ�
	if (pInputKeyboard->GetKeyboardPress(DIK_NUMPAD8) == true)
	{
		*fValue_Y += 0.01f;
		if (*fValue_Y > 1.0f) { *fValue_Y = 1.0f; }
		bSet = true;
	}
	if (pInputKeyboard->GetKeyboardPress(DIK_NUMPAD5) == true)
	{
		*fValue_Y -= 0.01f;
		if (*fValue_Y < 0.0f) { *fValue_Y = 0.0f; }
		bSet = true;
	}

	//fValue_Z�̐��l�̐؂�ւ�
	if (pInputKeyboard->GetKeyboardPress(DIK_NUMPAD9) == true)
	{
		*fValue_Z += 0.01f;
		if (*fValue_Z > 1.0f) { *fValue_Z = 1.0f; }
		bSet = true;
	}
	if (pInputKeyboard->GetKeyboardPress(DIK_NUMPAD6) == true)
	{
		*fValue_Z -= 0.01f;
		if (*fValue_Z < 0.0f) { *fValue_Z = 0.0f; }
		bSet = true;
	}

	return bSet;
}

//=============================================================================
// ���C�g�̖��邳�𒲐�
//=============================================================================
void CLight::UpdateLight(void){}