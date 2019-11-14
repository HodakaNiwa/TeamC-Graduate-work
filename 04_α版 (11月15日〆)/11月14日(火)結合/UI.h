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
class CCharacter;
class CSkilicon;

//*****************************************************************************
//    UI�N���X�̒�`
//*****************************************************************************
class CUI
{
public:    // �N�ł��A�N�Z�X�\
	CUI();
	~CUI();

	static CUI *Create(int nNumPlayer, int nAllCharacter);

	HRESULT Init(int nNumPlayer, int nAllCharacter);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CreateSkilicon(CCharacter *pChara, int nIdx, int nRecastTime);

	//--------------------
	//	Set & Get
	//--------------------
	void SetNumPlayer(const int nNumPlayer) { m_nNumPlayer = nNumPlayer; }
	void SetNumAllCharacter(const int nNumAllCharacter) { m_nNumAllCharacter = nNumAllCharacter; }
	void SetScoreGame(CScoreGame *pScoreGame, const int nIdx) { m_pScoreGame[nIdx] = pScoreGame; }
	void SetMiniMap(CMiniMap *pMiniMap) { m_pMiniMap = pMiniMap; }
	void SetSkilicon(CSkilicon *pSkilicon, const int nIdx) { m_pSkilicon[nIdx] = pSkilicon; }
	int GetNumPlayer(void) { return m_nNumPlayer; }
	int GetNumAllCharacter(void) { return m_nNumAllCharacter; }
	CScoreGame *GetScoreGame(const int nIdx) { return m_pScoreGame[nIdx]; }
	CMiniMap *GetMiniMap(void) { return m_pMiniMap; }
	CSkilicon *GetSkilicon(const int nIdx) { return m_pSkilicon[nIdx]; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	//--------------------
	//	�֐�
	//--------------------
	void CreateScoreGame(void);
	void CreateMiniMap(void);

	void ReleaseScoreGame(void);
	void ReleaseMiniMap(void);
	void ReleaseSkilicon(void);

	void UpdateSkilicon(void);

	void DrawSkilicon(void);

	//--------------------
	//	�����o�ϐ�
	//--------------------
	int m_nNumPlayer;								// �Q�[���ɎQ�����Ă���v���C���[�̐l��(�ۑ��p)
	int m_nNumAllCharacter;							// �Q�[���ɎQ�����Ă���l��(�ۑ��p)
	CScoreGame *m_pScoreGame[MAX_PLAYERNUM * 2];	// �Q�[���X�R�A�N���X�ւ̃|�C���^
	CMiniMap *m_pMiniMap;							// �~�j�}�b�v�N���X�ւ̃|�C���^
	CSkilicon *m_pSkilicon[MAX_PLAYERNUM];			// �X�L���A�C�R���N���X�ւ̃|�C���^
};

#endif