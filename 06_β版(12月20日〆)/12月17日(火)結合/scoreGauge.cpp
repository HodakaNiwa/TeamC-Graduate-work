//*****************************************************************************
//
//     �X�R�A�Q�[�W�̏���[skilicon.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "scoreGauge.h"
#include "skilicon.h"
#include "character.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
// �Q�[�W�w�i�p
#define SCOREGAUGE_BG_POS           (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 640.0f,0.0f))
#define SCOREGAUGE_BG_COL           (D3DXCOLOR(0.3f,0.3f,0.3f,1.0f))
#define SCOREGAUGE_BG_WIDTH         (1200.0f)
#define SCOREGAUGE_BG_HEIGHT        (80.0f)
#define SCOREGAUGE_BG_FRAMESIZE_X   (10.0f)
#define SCOREGAUGE_BG_FRAMESIZE_Y   (10.0f)
#define SCOREGAUGE_BG_HEIGHT_CUT    (0.8f)

// �Q�[�W�p
#define SCOREGAUGE_RING_PRIORITY    (3)

// �v���C���[�ԍ��p
#define SCOREGAUGE_PLAYERIDX_POS    (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 680.0f,0.0f))
#define SCOREGAUGE_PLAYERIDX_WIDTH  (60.0f)
#define SCOREGAUGE_PLAYERIDX_HEIGHT (40.0f)

// �����p
#define SCOREGAUGE_CROWN_COL        (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define SCOREGAUGE_CROWN_WIDTH      (70.0f)
#define SCOREGAUGE_CROWN_HEIGHT     (70.0f)
#define SCOREGAUGE_CROWN_WIDTH_MIN  (61.5f)
#define SCOREGAUGE_CROWN_HEIGHT_MIN (61.5f)
#define SCOREGAUGE_CROWN_DISPPOINT  (11)

//*****************************************************************************
//     �ÓI�����o�ϐ��錾
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CScoreGauge::CScoreGauge()
{
	m_nNumPlayer = 0;
	m_pGaugeBg = NULL;
	for (int nCnt = 0; nCnt < MAX_NUMPLAYER * 2; nCnt++)
	{
		m_pGauge2D[nCnt] = NULL;
		m_pPlayerIdx[nCnt] = NULL;
	}
	for (int nCntCrown = 0; nCntCrown < MAX_CROWNNUM; nCntCrown++)
	{
		m_pCrown[nCntCrown] = NULL;
		m_bDispCrown[nCntCrown] = false;
	}
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CScoreGauge::~CScoreGauge()
{

}

//=============================================================================
//    ��������
//=============================================================================
CScoreGauge *CScoreGauge::Create(const int nNumPlayer)
{
	// ���������m��
	CScoreGauge *pScoreGauge = NULL;
	pScoreGauge = new CScoreGauge;
	if (pScoreGauge == NULL) { return NULL; }

	// ����������
	if (FAILED(pScoreGauge->Init(nNumPlayer)))
	{
		pScoreGauge->Uninit();
		delete pScoreGauge;
		return NULL;
	}

	return pScoreGauge;	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CScoreGauge::Init(const int nNumPlayer)
{
	// �Q�����Ă���v���C���[�̐l����ۑ�
	m_nNumPlayer = nNumPlayer;
	m_fGaugePosY = SCOREGAUGE_BG_POS.y;
	m_fGaugeHeight = SCOREGAUGE_BG_HEIGHT;
	if (m_nNumPlayer >= 3)
	{// 3�l�ȏ�Ȃ�΃Q�[�W���o��������ς���
		m_fGaugePosY = SCREEN_HEIGHT * 0.5f;
		m_fGaugeHeight *= SCOREGAUGE_BG_HEIGHT_CUT;
	}

	// �Q�[�W�w�i�̐���
	CreateGaugeBg();

	// �Q�[�W�̐���
	CreateGauge2D();

	// �v���C���[�ԍ��̐���
	CreatePlayerIdx();

	// �����̐���
	CreateCrown();

	// 1�|�C���g���Ƃ̃Q�[�W�ʂ��v�Z����
	CalcGaugeOnceFromRing();

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CScoreGauge::Uninit(void)
{
	// �Q�[�W�w�i�̊J��
	ReleaseGaugeBg();

	// �Q�[�W�̊J��
	ReleaseGauge2D();

	// �v���C���[�ԍ��̊J��
	ReleasePlayerIdx();

	// �����̊J��
	ReleaseCrown();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CScoreGauge::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CScoreGauge::Draw(void)
{
	// �Q�[�W�w�i�̕`��
	DrawGaugeBg();

	// �Q�[�W�̕`��
	DrawGauge2D();

	// �v���C���[�ԍ��̕`��
	DrawPlayerIdx();

	// �����̕`��
	DrawCrown();
}

//=============================================================================
//    �Q�[�W�ɉ����鏈��
//=============================================================================
void CScoreGauge::AddGauge(const int nIdx, const int nPoint)
{
	if (m_pGauge2D[nIdx] == NULL) { return; }

	// �Q�[�W�����炷
	float fWidth = m_pGauge2D[nIdx]->GetWidth();
	D3DXVECTOR3 pos = m_pGauge2D[nIdx]->GetPos();
	fWidth += m_fGaugeOnce * nPoint;
	float fCutValue = (m_fGaugeOnce * nPoint) * 0.5f;
	if (fWidth <= 0.0f)
	{
		fWidth = 0.0f;
		fCutValue = 0.0f;
	}
	pos.x += fCutValue;
	m_pGauge2D[nIdx]->SetPos(pos);
	m_pGauge2D[nIdx]->SetScale(fWidth, m_pGauge2D[nIdx]->GetHeight());
	m_pGauge2D[nIdx]->SetVtxBuffPos();

	// �v���C���[�ԍ������炷
	if (m_pPlayerIdx[nIdx] != NULL)
	{
		m_pPlayerIdx[nIdx]->SetPos(pos + D3DXVECTOR3(0.0f, (SCOREGAUGE_PLAYERIDX_HEIGHT * 0.5f) + (m_fGaugeHeight * 0.5f), 0.0f));
		m_pPlayerIdx[nIdx]->SetVtxBuffPos();
	}

	// �����ȍ~�̃Q�[�W�̏ꏊ�����炷
	D3DXVECTOR3 GaugePos = pos;
	fWidth = fWidth * 0.5f;
	for (int nCnt = nIdx + 1; nCnt < MAX_NUMPLAYER * 2; nCnt++)
	{
		if (m_pGauge2D[nCnt] == NULL) { continue; }
		GaugePos.x += fWidth;
		GaugePos.x += m_pGauge2D[nCnt]->GetWidth() * 0.5f;
		m_pGauge2D[nCnt]->SetPos(GaugePos);
		m_pGauge2D[nCnt]->SetVtxBuffPos();
		if (m_pPlayerIdx[nCnt] != NULL)
		{
			GaugePos.y += (SCOREGAUGE_PLAYERIDX_HEIGHT * 0.5f) + (m_fGaugeHeight * 0.5f);
			m_pPlayerIdx[nCnt]->SetPos(GaugePos);
			m_pPlayerIdx[nCnt]->SetVtxBuffPos();
		}
		fWidth = m_pGauge2D[nCnt]->GetWidth() * 0.5f;
		GaugePos = m_pGauge2D[nCnt]->GetPos();
	}
}

//=============================================================================
//    �Q�[�W����鏈��
//=============================================================================
void CScoreGauge::CutGauge(const int nIdx, const int nPoint)
{
	if (m_pGauge2D[nIdx] == NULL) { return; }

	// �Q�[�W�����炷
	float fWidth = m_pGauge2D[nIdx]->GetWidth();
	D3DXVECTOR3 pos = m_pGauge2D[nIdx]->GetPos();
	fWidth -= m_fGaugeOnce * nPoint;
	float fCutValue = (m_fGaugeOnce * nPoint) * 0.5f;
	if (fWidth <= 0.0f)
	{
		fWidth = 0.0f;
		fCutValue = 0.0f;
	}
	pos.x -= fCutValue;
	if (nIdx == 0 && fWidth <= 0.0f)
	{
		pos.x = SCOREGAUGE_BG_POS.x;
		pos.x -= SCOREGAUGE_BG_WIDTH * 0.5f;
	}
	m_pGauge2D[nIdx]->SetPos(pos);
	m_pGauge2D[nIdx]->SetScale(fWidth, m_pGauge2D[nIdx]->GetHeight());
	m_pGauge2D[nIdx]->SetVtxBuffPos();

	// �v���C���[�ԍ������炷
	if (m_pPlayerIdx[nIdx] != NULL)
	{
		m_pPlayerIdx[nIdx]->SetPos(pos + D3DXVECTOR3(0.0f, (SCOREGAUGE_PLAYERIDX_HEIGHT * 0.5f) + (m_fGaugeHeight * 0.5f), 0.0f));
		m_pPlayerIdx[nIdx]->SetVtxBuffPos();
	}

	// �����ȍ~�̃Q�[�W�̏ꏊ�����炷
	D3DXVECTOR3 GaugePos = pos;
	fWidth = fWidth * 0.5f;
	for (int nCnt = nIdx + 1; nCnt < MAX_NUMPLAYER * 2; nCnt++)
	{
		if (m_pGauge2D[nCnt] == NULL) { continue; }
		GaugePos.x += fWidth;
		GaugePos.x += m_pGauge2D[nCnt]->GetWidth() * 0.5f;
		m_pGauge2D[nCnt]->SetPos(GaugePos);
		m_pGauge2D[nCnt]->SetVtxBuffPos();
		if (m_pPlayerIdx[nCnt] != NULL)
		{
			GaugePos.y += (SCOREGAUGE_PLAYERIDX_HEIGHT * 0.5f) + (m_fGaugeHeight * 0.5f);
			m_pPlayerIdx[nCnt]->SetPos(GaugePos);
			m_pPlayerIdx[nCnt]->SetVtxBuffPos();
		}
		fWidth = m_pGauge2D[nCnt]->GetWidth() * 0.5f;
		GaugePos = m_pGauge2D[nCnt]->GetPos();
	}
}

//=============================================================================
//    ������\�����邩���肷�鏈��
//=============================================================================
void CScoreGauge::CheckDispCrown(const int nIdx, const int nRank)
{
	if (m_pCrown[nIdx] == NULL) { return; }

	// �Q�[�W�̕���������x����Ε`��
	if (m_pGauge2D[nRank]->GetWidth() >= m_fGaugeOnce * SCOREGAUGE_CROWN_DISPPOINT)
	{
		m_bDispCrown[nIdx] = true;
		m_pCrown[nIdx]->SetPos(m_pGauge2D[nRank]->GetPos());
		m_pCrown[nIdx]->SetVtxBuffPos();
	}
	else
	{
		m_bDispCrown[nIdx] = false;
	}
}

//=============================================================================
//    �Q�[�W�w�i�̐�������
//=============================================================================
void CScoreGauge::CreateGaugeBg(void)
{
	D3DXVECTOR3 GaugePos = SCOREGAUGE_BG_POS;
	GaugePos.y = m_fGaugePosY;
	if (m_pGaugeBg != NULL) { return; }
	m_pGaugeBg = CIcon2D::Create(GaugePos, SCOREGAUGE_BG_COL,
		SCOREGAUGE_BG_WIDTH + SCOREGAUGE_BG_FRAMESIZE_X, m_fGaugeHeight + SCOREGAUGE_BG_FRAMESIZE_Y);
}

//=============================================================================
//    �Q�[�W�̐�������
//=============================================================================
void CScoreGauge::CreateGauge2D(void)
{
	D3DXVECTOR3 GaugePos = SCOREGAUGE_BG_POS;
	GaugePos.x -= SCOREGAUGE_BG_WIDTH * 0.5f;
	GaugePos.y = m_fGaugePosY;
	for (int nCnt = 0; nCnt < MAX_NUMPLAYER * 2; nCnt++)
	{
		if (m_pGauge2D[nCnt] != NULL) { continue; }
		m_pGauge2D[nCnt] = CIcon2D::Create(GaugePos, CCharacter::m_CountryColor[nCnt],
			0.0f, m_fGaugeHeight);
	}
}

//=============================================================================
//    �v���C���[�ԍ��̐�������
//=============================================================================
void CScoreGauge::CreatePlayerIdx(void)
{
	D3DXVECTOR3 PlayerIdxPos = INITIALIZE_VECTOR3;
	for (int nCnt = 0; nCnt < MAX_NUMPLAYER * 2; nCnt++)
	{
		if (m_pPlayerIdx[nCnt] != NULL || m_pGauge2D[nCnt] == NULL) { continue; }
		PlayerIdxPos = m_pGauge2D[nCnt]->GetPos();
		PlayerIdxPos.y += (SCOREGAUGE_PLAYERIDX_HEIGHT * 0.5f) + (m_fGaugeHeight * 0.5f);
		m_pPlayerIdx[nCnt] = CIcon2D::Create(PlayerIdxPos, CCharacter::m_CountryColor[nCnt],
			SCOREGAUGE_PLAYERIDX_WIDTH, SCOREGAUGE_PLAYERIDX_HEIGHT);
	}
}

//=============================================================================
//    �����̐�������
//=============================================================================
void CScoreGauge::CreateCrown(void)
{
	float fWidth = SCOREGAUGE_CROWN_WIDTH;
	float fHeight = SCOREGAUGE_CROWN_HEIGHT;
	if (m_nNumPlayer >= 3)
	{// 3�l�ȏオ�Q�����Ă���
		fWidth = SCOREGAUGE_CROWN_WIDTH_MIN;
		fHeight = SCOREGAUGE_CROWN_HEIGHT_MIN;
	}
	for (int nCnt = 0; nCnt < MAX_CROWNNUM; nCnt++)
	{
		if (m_pCrown[nCnt] != NULL) { continue; }
		m_pCrown[nCnt] = CIcon2D::Create(INITIALIZE_VECTOR3, SCOREGAUGE_CROWN_COL,
			fWidth, fHeight);
	}
}

//=============================================================================
//    1�|�C���g���Ƃ̃Q�[�W�ʂ��v�Z���鏈��
//=============================================================================
void CScoreGauge::CalcGaugeOnceFromRing(void)
{
	// ��������Ă��郊���O�̐������߂�
	int nNumRing = 0;
	CScene *pScene = CScene::GetTop(SCOREGAUGE_RING_PRIORITY);
	CScene *pSceneNext = NULL;
	while (pScene != NULL)
	{
		pSceneNext = pScene->GetpNext();
		if (pScene->GetObjType() == CScene::OBJTYPE_RING)
		{// �����O��������
			nNumRing++;
		}
		pScene = pSceneNext;
	}

	// ��������Ă��郊���O�̐�����Q�[�W�̕����v�Z����
	if (nNumRing <= 0) { return; }
	m_fGaugeOnce = SCOREGAUGE_BG_WIDTH / nNumRing;
}

//=============================================================================
//    �Q�[�W�w�i�̊J������
//=============================================================================
void CScoreGauge::ReleaseGaugeBg(void)
{
	if (m_pGaugeBg != NULL)
	{
		m_pGaugeBg->Uninit();
		delete m_pGaugeBg;
		m_pGaugeBg = NULL;
	}
}

//=============================================================================
//    �Q�[�W�̊J������
//=============================================================================
void CScoreGauge::ReleaseGauge2D(void)
{
	for (int nCnt = 0; nCnt < MAX_NUMPLAYER * 2; nCnt++)
	{
		if (m_pGauge2D[nCnt] != NULL)
		{
			m_pGauge2D[nCnt]->Uninit();
			delete m_pGauge2D[nCnt];
			m_pGauge2D[nCnt] = NULL;
		}
	}
}

//=============================================================================
//    �v���C���[�ԍ��̊J������
//=============================================================================
void CScoreGauge::ReleasePlayerIdx(void)
{
	for (int nCnt = 0; nCnt < MAX_NUMPLAYER * 2; nCnt++)
	{
		if (m_pPlayerIdx[nCnt] != NULL)
		{
			m_pPlayerIdx[nCnt]->Uninit();
			delete m_pPlayerIdx[nCnt];
			m_pPlayerIdx[nCnt] = NULL;
		}
	}
}

//=============================================================================
//    �����̊J������
//=============================================================================
void CScoreGauge::ReleaseCrown(void)
{
	for (int nCnt = 0; nCnt < MAX_CROWNNUM; nCnt++)
	{
		if (m_pCrown[nCnt] != NULL)
		{
			m_pCrown[nCnt]->Uninit();
			delete m_pCrown[nCnt];
			m_pCrown[nCnt] = NULL;
		}
	}
}

//=============================================================================
//    �Q�[�W�w�i�̕`�揈��
//=============================================================================
void CScoreGauge::DrawGaugeBg(void)
{
	if (m_pGaugeBg != NULL)
	{
		m_pGaugeBg->Draw();
	}
}

//=============================================================================
//   �Q�[�W�̕`�揈��
//=============================================================================
void CScoreGauge::DrawGauge2D(void)
{
	for (int nCnt = 0; nCnt < MAX_NUMPLAYER * 2; nCnt++)
	{
		if (m_pGauge2D[nCnt] != NULL)
		{
			m_pGauge2D[nCnt]->Draw();
		}
	}
}

//=============================================================================
//    �v���C���[�ԍ��̕`�揈��
//=============================================================================
void CScoreGauge::DrawPlayerIdx(void)
{
	for (int nCnt = (MAX_NUMPLAYER * 2) - 1; nCnt >= 0; nCnt--)
	{
		if (m_pPlayerIdx[nCnt] != NULL)
		{
			m_pPlayerIdx[nCnt]->Draw();
		}
	}
}

//=============================================================================
//    �����̕`�揈��
//=============================================================================
void CScoreGauge::DrawCrown(void)
{
	for (int nCnt = 0; nCnt < MAX_CROWNNUM; nCnt++)
	{
		if (m_pCrown[nCnt] != NULL && m_bDispCrown[nCnt] == true)
		{
			m_pCrown[nCnt]->Draw();
		}
	}
}