//*****************************************************************************
//
//     UI�̏���[UI.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _UI_H_
#define _UI_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "manager.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CScoreGame;
class CMiniMap;

//*****************************************************************************
//    UI�N���X�̒�`
//*****************************************************************************
class CUI
{
public:    // �N�ł��A�N�Z�X�\
	CUI();
	~CUI();

	static CUI *Create(int nNumPlayer);

	HRESULT Init(int nNumPlayer);
	void Uninit(void);
	void Update(void);

	//--------------------
	//	Set & Get
	//--------------------
	void SetNumPlayer(const int nNumPlayer) { m_nNumPlayer = nNumPlayer; }
	void SetScoreGame(CScoreGame *pScoreGame, const int nIdx) { m_pScoreGame[nIdx] = pScoreGame; }
	void SetMiniMap(CMiniMap *pMiniMap) { m_pMiniMap = pMiniMap; }
	int GetNumPlayer(void) { return m_nNumPlayer; }
	CScoreGame *GetScoreGame(const int nIdx) { return m_pScoreGame[nIdx]; }
	CMiniMap *GetMiniMap(void) { return m_pMiniMap; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	//--------------------
	//	�֐�
	//--------------------
	void CreateScoreGame(void);
	void CreateMiniMap(void);

	void ReleaseScoreGame(void);
	void ReleaseMiniMap(void);

	//--------------------
	//	�����o�ϐ�
	//--------------------
	int m_nNumPlayer;							// �Q�[���ɎQ�����Ă���l��(�ۑ��p)
	CScoreGame *m_pScoreGame[MAX_PLAYERNUM];	// �Q�[���X�R�A�N���X�ւ̃|�C���^
	CMiniMap *m_pMiniMap;						// �~�j�}�b�v�N���X�ւ̃|�C���^
};

#endif