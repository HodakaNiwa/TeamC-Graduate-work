//*****************************************************************************
//
//     �X�R�A�ϓ��̏���[scoreChange.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "scoreChange.h"
#include "number2D.h"
#include "manager.h"
#include "game.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define SCORECHANGE_PRIORITY	(3)		// �X�R�A�ϓ��̏����D�揇��
#define SCORECHANGE_INTERVAL	(1.4f)	// �X�R�A�ϓ��̐����̊Ԋu
#define SCORECHANGE_ALPHA_CUT	(0.03f)	// �X�R�A�ϓ��̓����x���ǂꂭ�炢��邩
#define SCORECHANGE_POS_DOWN	(0.70f)	// �X�R�A�ϓ����ǂꂭ�炢���ɉ����邩
#define SCORECHANGE_ALPHA_TIME	(70)	// �X�R�A�ϓ��̓����x���ǂꂭ�炢������n�߂邩
#define SCORECHANGE_SIZE_CUT	(0.7f)	// �X�R�A�ϓ��̃T�C�Y���ǂꂭ�炢���������邩(�l���ɂ���ĕϓ�)

//*****************************************************************************
//     �ÓI�����o�ϐ��錾
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CScoreChange::CScoreChange(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
{
	m_pNumber2D = NULL;
	m_nNumberDigit = 0;
	m_nScoreChange = 0;
	m_nChangeTimer = 0;
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CScoreChange::~CScoreChange()
{

}

//=============================================================================
//    ��������
//=============================================================================
CScoreChange *CScoreChange::Create(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 size, int nScoreChange)
{
	// ���������m��
	CScoreChange *pScoreChange = NULL;
	pScoreChange = new CScoreChange(SCORECHANGE_PRIORITY);
	if (pScoreChange == NULL) { return NULL; }

	// �l�̐ݒ�
	pScoreChange->SetScoreChange(nScoreChange);

	// ����������
	if (FAILED(pScoreChange->Init()))
	{
		pScoreChange->Uninit();
		return NULL;
	}

	// �T�C�Y�̒���
	size = pScoreChange->SizeAdjust(size);

	// �ʒu�̐ݒ�
	pos.x += (size.x * 0.75f) * pScoreChange->GetNumberDigit();

	// �|���S���̐ݒ�
	pScoreChange->Set(pos, size);
	pScoreChange->SetColoer(col);
	pScoreChange->CreateNumber();

	return pScoreChange;	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CScoreChange::Init(void)
{
	// ���ʂ̏���������
	if (FAILED(CScene2D::Init()))
	{
		return E_FAIL;
	}

	// �X�R�A�ϓ��̌������v�Z
	CalcScoreChangeDigit();

	return S_OK;
}

//=============================================================================
//    �����̌������v�Z���鏈��
//=============================================================================
void CScoreChange::CalcScoreChangeDigit(void)
{
	m_nNumberDigit = (int)log10f((float)m_nScoreChange) + 1;
	if (m_nNumberDigit <= 0) { m_nNumberDigit = 1; }
}

//=============================================================================
//    �����|���S���𐶐����鏈��
//=============================================================================
void CScoreChange::CreateNumber(void)
{
	if (m_pNumber2D != NULL) { return; }

	// ���������������m��
	m_pNumber2D = new CNumber2D*[m_nNumberDigit];
	if (m_pNumber2D == NULL) { return; }

	// �����|���S��������������
	D3DXVECTOR3 pos = GetPos();
	pos.x += (GetSize().x * SCORECHANGE_INTERVAL) * (m_nNumberDigit + 1);
	int nNumber = 0;
	for (int nCntDigit = 0; nCntDigit < m_nNumberDigit; nCntDigit++)
	{
		// �|�C���^��������
		m_pNumber2D[nCntDigit] = NULL;

		// �\�����鐔���̌v�Z
		nNumber = m_nScoreChange % ((int)powf(10.0f, (float)nCntDigit) * 10) / (int)powf(10.0f, (float)nCntDigit);

		// �����̏ꏊ�����炵�Ă���
		pos.x -= GetSize().x * SCORECHANGE_INTERVAL;

		// ����
		m_pNumber2D[nCntDigit] = CNumber2D::Create(pos, GetSize(), GetColor());
		if (m_pNumber2D[nCntDigit] == NULL) { continue; }
		m_pNumber2D[nCntDigit]->SetbUse(true);
		m_pNumber2D[nCntDigit]->SetNumber(nNumber);
	}
}

//=============================================================================
//    �I������
//=============================================================================
void CScoreChange::Uninit(void)
{
	// �����|���S���N���X�̊J��
	ReleaseNumber();

	// ���ʂ̏I������
	CScene2D::Uninit();
}

//=============================================================================
//    �����|���S�����J�����鏈��
//=============================================================================
void CScoreChange::ReleaseNumber(void)
{
	if (m_pNumber2D == NULL) { return; }
	for (int nCntDigit = 0; nCntDigit < m_nNumberDigit; nCntDigit++)
	{
		if (m_pNumber2D[nCntDigit] != NULL)
		{
			m_pNumber2D[nCntDigit]->Uninit();
			delete m_pNumber2D[nCntDigit];
			m_pNumber2D[nCntDigit] = NULL;
		}
	}
	delete[] m_pNumber2D;
	m_pNumber2D = NULL;
}

//=============================================================================
//    �X�V����
//=============================================================================
void CScoreChange::Update(void)
{
	// �ʒu��������
	PosDown();

	// �����x���グ��
	AlphaAdd();
}

//=============================================================================
//    �ʒu�������鏈��
//=============================================================================
void CScoreChange::PosDown(void)
{
	// �ʒu�̎擾
	D3DXVECTOR3 pos = GetPos();

	// ���ɉ�����
	pos.y += SCORECHANGE_POS_DOWN;

	// �ʒu�̐ݒ�
	SetPos(pos, GetOtherPos());
	PosDown_Number();
}
void CScoreChange::PosDown_Number(void)
{
	D3DXVECTOR3 pos;
	for (int nCntDigit = 0; nCntDigit < m_nNumberDigit; nCntDigit++)
	{
		if (m_pNumber2D[nCntDigit] != NULL)
		{
			pos = m_pNumber2D[nCntDigit]->GetPos();
			pos.y += SCORECHANGE_POS_DOWN;
			m_pNumber2D[nCntDigit]->SetPos(pos);
		}
	}
}

//=============================================================================
//    �����x���グ�鏈������
//=============================================================================
void CScoreChange::AlphaAdd(void)
{
	m_nChangeTimer++;
	if (m_nChangeTimer <= SCORECHANGE_ALPHA_TIME) { return; }

	// �F�̎擾
	D3DXCOLOR col = GetColor();

	// �����x�𑝉�
	col.a -= SCORECHANGE_ALPHA_CUT;

	// �F�̐ݒ�
	SetColoer(col);
	AlphaAdd_Number(col);

	// ���S����
	if (col.a <= 0.0f)
	{
		Uninit();
	}
}
void CScoreChange::AlphaAdd_Number(const D3DXCOLOR col)
{
	for (int nCntDigit = 0; nCntDigit < m_nNumberDigit; nCntDigit++)
	{
		if (m_pNumber2D[nCntDigit] != NULL)
		{
			m_pNumber2D[nCntDigit]->SetColor(col);
		}
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CScoreChange::Draw(void)
{
	// �����̕`��
	DrawNumber();

	// ���ʂ̕`�揈��
	CScene2D::Draw();
}

//=============================================================================
//    ������`�悷�鏈��
//=============================================================================
void CScoreChange::DrawNumber(void)
{
	if (m_pNumber2D == NULL) { return; }

	for (int nCntDigit = 0; nCntDigit < m_nNumberDigit; nCntDigit++)
	{
		if (m_pNumber2D[nCntDigit] != NULL)
		{
			m_pNumber2D[nCntDigit]->Draw();
		}
	}
}

//=============================================================================
//    �T�C�Y�𒲐����鏈��
//=============================================================================
D3DXVECTOR3 CScoreChange::SizeAdjust(D3DXVECTOR3 size)
{
	// �Q�[���N���X���擾
	CGame *pGame = CManager::GetGame();
	if (pGame == NULL) { return size; }

	// 2�l�ȏ�Q�����Ă�����T�C�Y��ύX����
	if (pGame->GetNumPlay() >= 2)
	{
		size.x *= SCORECHANGE_SIZE_CUT;
		size.z *= SCORECHANGE_SIZE_CUT;
	}

	return size;
}