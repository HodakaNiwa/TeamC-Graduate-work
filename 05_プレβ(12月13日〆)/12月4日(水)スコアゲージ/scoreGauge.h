//*****************************************************************************
//
//     �X�R�A�Q�[�W�̏���[scoreGauge.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SCOREGAUGE_H_
#define _SCOREGAUGE_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "game.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define MAX_CROWNNUM (3)

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CIcon2D;

//*****************************************************************************
//    �X�R�A�Q�[�W�N���X�̒�`
//*****************************************************************************
class CScoreGauge
{
public:     // �N�ł��A�N�Z�X�\
	CScoreGauge();
	~CScoreGauge();

	static CScoreGauge *Create(const int nNumPlayer);

	HRESULT Init(const int nNumPlayer);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddGauge(const int nIdx, const int nPoint);
	void CutGauge(const int nIdx, const int nPoint);
	void CheckDispCrown(const int nIdx, const int nRank);

	//--------------------
	//	Set & Get
	//--------------------
	void SetNumPlayer(const int nNumPlayer) { m_nNumPlayer = nNumPlayer; }
	void SetGaugeBg(CIcon2D *pGaugeBg) { m_pGaugeBg = pGaugeBg; }
	void SetGauge2D(CIcon2D *pGauge2D, const int nIdx) { m_pGauge2D[nIdx] = pGauge2D; }
	void SetPlayerIdx(CIcon2D *pPlayerIdx, const int nIdx) { m_pPlayerIdx[nIdx] = pPlayerIdx; }
	void SetCrown(CIcon2D *pCrown, const int nIdx) { m_pCrown[nIdx] = pCrown; }
	void SetDispCrown(const bool bDisp, const int nIdx) { m_bDispCrown[nIdx] = bDisp; }
	void SetGaugeOnce(const float fGaugeOnce) { m_fGaugeOnce = fGaugeOnce; }

	int GetNumPlayer(void) { return m_nNumPlayer; }
	CIcon2D *GetGaugeBg(const int nIdx) { return m_pGaugeBg; }
	CIcon2D *GetGauge2D(const int nIdx) { return m_pGauge2D[nIdx]; }
	CIcon2D *GetPlayerIdx(const int nIdx) { return m_pPlayerIdx[nIdx]; }
	CIcon2D *GetCrown(const int nIdx) { return m_pCrown[nIdx]; }
	float GetGaugeOnce(void) { return m_fGaugeOnce; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void CreateGaugeBg(void);
	void CreateGauge2D(void);
	void CreatePlayerIdx(void);
	void CreateCrown(void);
	void CalcGaugeOnceFromRing(void);

	void ReleaseGaugeBg(void);
	void ReleaseGauge2D(void);
	void ReleasePlayerIdx(void);
	void ReleaseCrown(void);

	void DrawGaugeBg(void);
	void DrawGauge2D(void);
	void DrawPlayerIdx(void);
	void DrawCrown(void);

	float m_fGaugePosY;							// �Q�[�W�̈ʒu(Y���W)
	float m_fGaugeHeight;						// �Q�[�W�̍���
	int m_nNumPlayer;							// �Q�[���ɎQ�����Ă���v���C���[�̐l��
	CIcon2D *m_pGaugeBg;						// �Q�[�W�̔w�i
	CIcon2D *m_pGauge2D[MAX_NUMPLAYER * 2];		// �e�v���C���[�̃Q�[�W��
	CIcon2D *m_pPlayerIdx[MAX_NUMPLAYER * 2];	// �e�v���C���[�̃Q�[�W��\���ԍ�
	CIcon2D *m_pCrown[MAX_CROWNNUM];			// ����
	bool m_bDispCrown[MAX_CROWNNUM];			// ������`�悷�邩�ǂ���
	float m_fGaugeOnce;							// 1�|�C���g���̃Q�[�W��
};

#endif