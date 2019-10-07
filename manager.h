//=============================================================================
//
// �}�l�[�W���[�̏��� [manager.h]
// Author : ���� �����@[ SATO ASUMI ]
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

//=============================================================================
// �O���錾
//=============================================================================
class CRenderer;
class CSound;
class CInputKeyboard;
class CGamePad;
class CLight;
class CGame;
class CRanking;
class CTitle;
class CTutorial;
class CFade;
class CResult;

//=============================================================================
// �}�N����`
//=============================================================================
#define RANK (5)			//�����L���O�̐�
#define MAX_PLAYER	(10)

//=============================================================================
// �}�l�[�W���[�N���X
//=============================================================================
class CManager
{
public:
	CManager();
	~CManager();

	//���[�h�̎��
	typedef enum
	{
		MODE_TITLE = 0,
		MODE_TUTORIAL,
		MODE_GAME,
		MODE_RESULT,
		MODE_RANKING,
		MODE_MAX,
	}MODE;

	//�����o�֐�
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CRenderer * GetRenderer(void);
	static CInputKeyboard * GetInputkeyboard(void);
	static CGamePad * GetInputGamePad(void) { return m_pInputGamePad; };
	static CSound * GetSound(void);
	static CLight * GetLight(void) { return m_pLight; };												//���C�g�̎擾
	static void SetMode(MODE mode);																		//���[�h�̐ݒ�
	static CFade * GetFade(void) { return m_pFade; };													//�t�F�[�h�|�C���^�̎擾
	static MODE GetMode(void) { return m_mode; };														//���[�h�̎擾
	static CGame * GetGame(void) { return m_pGame; };													//�Q�[���̃|�C���^�̎擾
	static CTutorial * GetTutorial(void) { return m_pTutorial; };										//�`���[�g���A���̃|�C���^���擾
	static void SetScore(int nScore) { m_nScore = nScore; };											//�X�R�A�̐ݒ�
	static int GetScore(void) { return m_nScore; };														//�X�R�A�̎擾
	static int GetNumDethButterfly(void) { return m_nDethButterfly; };									//�|�������̐����擾
	static int GetMaxCombo(void) { return m_nMaxConbo; };												//�R���{���̒ǉ�
	static int GetRankScore(int nCntRank) { return m_nRankScore[nCntRank]; };							//�����L���O�X�R�A�̎擾
	static void SetRankScore(int nCntRank, int nRankScore) { m_nRankScore[nCntRank] = nRankScore; };	//�����L���O�X�R�A�̐ݒ�

protected:
	
private:
	static CRenderer * m_pRenderer;
	static CSound * m_pSound;
	static CInputKeyboard * m_pInputKeyboard;
	static CGamePad * m_pInputGamePad;
	static CLight * m_pLight;
	static CGame * m_pGame;
	static CResult * m_pResult;
	static CRanking * m_pRanking;
	static CTitle * m_pTitle;
	static CTutorial * m_pTutorial;
	static CFade * m_pFade;
	static MODE m_mode;
	static int m_nScore;
	static int m_nRankScore[RANK];
	static int m_nMaxConbo;
	static int m_nDethButterfly;
};
#endif