//=============================================================================
//
// �L�����I���̏��� [charselect.h]
// Author : ���� �����@[ SATO ASUMI ]
//
//=============================================================================
#ifndef _SELECTPLAYER_H_
#define _SELECTPLAYER_H_

//=================================================
//		include / ���̃f�[�^�E�R�[�h�̌Ăяo���E���p
//=================================================
#include "main.h"
#include "select.h"

//==============================================
//					�}�N����`
//==============================================
#define MAX_TYPE	(3)
#define MAX_CUNTRY	(8)
#define SELECTCHAR_TEX	(2)

//==============================================
//					�O���錾
//==============================================
class CMoveUI;
class CCharMultRender;

//==============================================
//			�L�����N�^�[�̔h�� �� �v���C���[
//==============================================
class CSelectCharacter
{
public:
	typedef enum
	{
		STATE_SPEED = 0,
		STATE_PAWER,
		STATE_MINE,
		STATE_RANDUM,
		STATE_MAX,
	}STATE;

	CSelectCharacter();
	~CSelectCharacter();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nCuntry, int nNumPlayer, int nNumControler);
	void Uninit(void);
	void Update(void);
	static CSelectCharacter * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nCuntry, int nNumPlayer, int nNumControler);
	static void LoadTex(void);
	static void UnloadTex(void);
	void UpdateScaleUp(D3DXVECTOR3 AimSize, D3DXVECTOR2 Move);
	void UpdateScaleDown(D3DXVECTOR3 AimSize, D3DXVECTOR2 Move);
	void UninitUI(void);
	bool GetbEnter(void) { return m_bEnter; }
	int GetType(void) { return m_nType; }
	int GetCuntry(void) { return m_nCuntry; }
	void SetInputType(CSelect::INPUTTYPE InputType) { m_InputType = InputType; }	//���̓f�o�C�X�̃^�C�v��ݒ�

private:
	void LoadMultRendering(void);
	void UpdateInput(void);
	void MoveRight(void);
	void MoveLeft(void);
	void PressEnter(void);

	static LPDIRECT3DTEXTURE9 m_pTexture[SELECTCHAR_TEX];
	LPDIRECT3DTEXTURE9 m_pStatusTex;
	CMoveUI * m_pMoveUI;
	CMoveUI * m_pTypeName;
	CMoveUI * m_pStatus;
	CMoveUI * m_pEnter;
	CCharMultRender * m_pCharMultRender[MAX_TYPE];
	int m_nCuntry;
	int m_nType;
	int m_nOldType;
	int m_nNumPlayer;
	int m_nNumControler;
	int m_nCountTime;
	bool m_bEnter;
	STATE m_state;
	CSelect::INPUTTYPE m_InputType;	//���̓f�o�C�X�̎��
};
#endif