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
class CTitle;
class CSelect;
class CTutorial;
class CGame;
class CResult;
class CRanking;
class CFade;
class CModeBase;
class CInputXPad;
class CInputMouse;
class CRawMouse;

//=============================================================================
// �}�N����`
//=============================================================================
#define RANK (5)			//�����L���O�̐�
#define MAX_PLAYERNUM	(4)	//�v���C���[�̍ő吔

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
		MODE_SELECT,
		MODE_TUTORIAL,
		MODE_GAME,
		MODE_RESULT,
		MODE_RANKING,
		MODE_MAX,
	}MODE;

	//�Q�[�����[�h�̏��
	typedef enum
	{
		GAMESTATE_NONE = 0,
		GAMESTATE_FIRSTCOUNTDOWN,
		GAMESTATE_GAME,
		GAMESTATE_FIN,
		GAMESTATE_MAX
	}GAMESTATE;

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
	static void SetScore(int nScore) { m_nScore = nScore; };											//�X�R�A�̐ݒ�
	static int GetScore(void) { return m_nScore; };														//�X�R�A�̎擾
	static int GetNumDethButterfly(void) { return m_nDethButterfly; };									//�|�������̐����擾
	static int GetMaxCombo(void) { return m_nMaxConbo; };												//�R���{���̒ǉ�
	static int GetRankScore(int nCntRank) { return m_nRankScore[nCntRank]; };							//�����L���O�X�R�A�̎擾
	static void SetRankScore(int nCntRank, int nRankScore) { m_nRankScore[nCntRank] = nRankScore; };	//�����L���O�X�R�A�̐ݒ�
	static CInputXPad * GetXPad(void) { return m_pXPad; }												//X�C���v�b�g�̎擾
	static CInputMouse * GetInputMouse(void) { return m_pInputMouse; }									//�}�E�X�̎擾
	static GAMESTATE GetGameState(void) { return m_GameState; }											//�Q�[���̏�Ԃ��擾
	static void SetGameState(GAMESTATE GameState) { m_GameState = GameState; }
	static CRawMouse * GetRawMouse(void) { return m_pRawMouse; }										//���E�}�E�X�̎擾
	static void CreateRawMouse(HWND hWnd);
	//��ʂ̎擾
	static CTitle * GetTitle(void) { return (CTitle *)m_pModeBase; }			//�^�C�g��
	static CSelect * GetSelect(void) { return(CSelect *)m_pModeBase; }			//�Z���N�g
	static CTutorial * GetTutorial(void) { return(CTutorial *)m_pModeBase; }	//�`���[�g���A��
	static CGame * GetGame(void) { return (CGame *)m_pModeBase; };				//�Q�[��
	static CResult * GetResult(void) { return (CResult *)m_pModeBase; }			//���U���g
	static CRanking * GetRanking(void) { return (CRanking *)m_pModeBase; }		//�����L���O
	static CModeBase * GetModeBase(void) { return m_pModeBase; }				//���[�h�x�[�X�̎擾
private:
	static CRenderer * m_pRenderer;
	static CSound * m_pSound;
	static CInputKeyboard * m_pInputKeyboard;
	static CGamePad * m_pInputGamePad;
	static CLight * m_pLight;
	static CModeBase * m_pModeBase;
	static CFade * m_pFade;
	static MODE m_mode;
	static int m_nScore;
	static int m_nRankScore[RANK];
	static int m_nMaxConbo;
	static int m_nDethButterfly;
	static CInputXPad * m_pXPad;
	static GAMESTATE m_GameState;
	static CInputMouse * m_pInputMouse;
	static CRawMouse * m_pRawMouse;
};
#endif