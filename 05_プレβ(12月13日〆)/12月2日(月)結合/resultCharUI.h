//=============================================================================
//
// ���U���g�L�����\���̏��� [resultCharUI.h]
// Author : ���� �����@[ SATO ASUMI ]
//
//=============================================================================
#ifndef _RESULT_CHARUI_H_
#define _RESULT_CHARUI_H_

//=================================================
//		include / ���̃f�[�^�E�R�[�h�̌Ăяo���E���p
//=================================================
#include "main.h"

//==============================================
//					�O���錾
//==============================================
class CMoveUI;
class CCharMultRender;

//==============================================
//			�L�����N�^�[�̔h�� �� �v���C���[
//==============================================
class CResultCharUI
{
public:
	CResultCharUI();
	~CResultCharUI();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nNumPlayer, int nRank, int nCuntry, int nType);
	void Uninit(void);
	void Update(void);
	void UninitUI(void);
	static CResultCharUI * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nNumPlayer, int nRank, int nCuntry, int nType);
	void SetDraw(bool bDraw);
	void UpdateMoveDest(D3DXVECTOR3 AimPos, float fSpeed);
	
private:
	void LoadMultRendering(int nCuntry, int nType);

	CMoveUI * m_pBack;
	CMoveUI * m_pFlam;
	CMoveUI * m_pCharMult;
	CMoveUI * m_pRank;
	CMoveUI * m_pCharNo;
	CMoveUI * m_pCharType;
	CCharMultRender * m_pCharMultRender;
	int m_nNumPlayer;
	int m_nCountTime;
	int m_nRank;
};
#endif