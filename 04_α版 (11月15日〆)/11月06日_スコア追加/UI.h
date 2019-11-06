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

	void SetNumPlayer(const int nNumPlayer) { m_nNumPlayer = nNumPlayer; }
	void SetScoreGame(CScoreGame *pScoreGame, const int nIdx) { m_pScoreGame[nIdx] = pScoreGame; }
	int GetNumPlayer(void) { return m_nNumPlayer; }
	CScoreGame *GetScoreGame(const int nIdx) { return m_pScoreGame[nIdx]; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	//--------------------
	//	�֐�
	//--------------------
	void CreateScoreGame(void);
	void ReleaseScoreGame(void);

	//--------------------
	//	�����o�ϐ�
	//--------------------
	int m_nNumPlayer;							// �Q�[���ɎQ�����Ă���l��(�ۑ��p)
	CScoreGame *m_pScoreGame[MAX_PLAYERNUM];	// �Q�[���X�R�A�N���X�ւ̃|�C���^
};

#endif