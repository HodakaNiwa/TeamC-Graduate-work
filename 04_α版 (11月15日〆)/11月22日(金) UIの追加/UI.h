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
#include "manager.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define UI_RANKICON_NUM (3)	// ���ʃA�C�R���̑���

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CScoreGame;
class CMiniMap;
class CCharacter;
class CSkilicon;
class CCharaicon;
class CIcon2D;
class CGame;

//*****************************************************************************
//    �A�C�R��(�r���{�[�h)�N���X�̒�`
//*****************************************************************************
class CIconBillboard
{
public:    // �N�ł��A�N�Z�X�\
	CIconBillboard();
	~CIconBillboard();

	static CIconBillboard *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, bool bLighting = false, float fRot = 0.0f, float fTexU = 0.0f, float fTexV = 0.0f, float fTexWidth = 1.0f, float fTexHeight = 1.0f);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	virtual void CalcNotScaleTransform(const LPDIRECT3DDEVICE9 pDevice);
	void SetTransformToDevice(const LPDIRECT3DDEVICE9 pDevice);
	void SetScale(float fWidth, float fHeight);
	virtual void SetVtxBuffValue(void);
	virtual void SetVtxBuffPos(void);
	void SetVtxBuffNor(void);
	void SetVtxBuffCol(void);
	void SetVtxBuffTex(void);


	//--------------------
	//	Set & Get
	//--------------------
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff) { m_pVtxBuff = pVtxBuff; }
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }
	void SetPos(const D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetCol(const D3DXCOLOR col) { m_Col = col; }
	void SetRot(const float fRot) { m_fRot = fRot; }
	void SetWidth(const float fWidth) { m_fWidth = fWidth; }
	void SetHeight(const float fHeight) { m_fHeight = fHeight; }
	void SetAngle(const float fAngle) { m_fAngle = fAngle; }
	void SetLength(const float fLength) { m_fLength = fLength; }
	void SetTexU(const float fTexU) { m_fTexU = fTexU; }
	void SetTexV(const float fTexV) { m_fTexV = fTexV; }
	void SetTexWidth(const float fTexWidth) { m_fTexWidth = fTexWidth; }
	void SetTexHeight(const float fTexHeight) { m_fTexHeight = fTexHeight; }
	void SetMtxWorld(const D3DXMATRIX mtxWorld) { m_MtxWorld = mtxWorld; }
	void SetLighting(const bool bLighting) { m_bLighting = bLighting; }

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }
	LPDIRECT3DTEXTURE9 GetTexture(void) { return m_pTexture; }
	D3DXVECTOR3 GetPos(void) { return m_Pos; }
	D3DXCOLOR GetCol(void) { return m_Col; }
	float GetRot(void) { return m_fRot; }
	float GetWidth(void) { return m_fHeight; }
	float GetHeight(void) { return m_fHeight; }
	float GetAngle(void) { return m_fAngle; }
	float GetLength(void) { return m_fLength; }
	float GetTexU(void) { return m_fTexU; }
	float GetTexV(void) { return m_fTexV; }
	float GetTexWidth(void) { return m_fTexWidth; }
	float GetTexHeight(void) { return m_fTexHeight; }
	D3DXMATRIX GetMtxWorld(void) { return m_MtxWorld; }
	bool GetLighting(void) { return m_bLighting; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\
	void ClearVariable(void);

private:   // ���̃N���X�������A�N�Z�X�\
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;     // ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTexture;	    // �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3             m_Pos;          // �|���S���̍��W
	D3DXCOLOR               m_Col;          // �|���S���̐F
	float                   m_fRot;         // �|���S���̌���
	float                   m_fWidth;       // �|���S���̕�
	float                   m_fHeight;      // �|���S���̍���
	float                   m_fAngle;       // �|���S�����o���p�x
	float                   m_fLength;      // �|���S���̒���
	float                   m_fTexU;        // �|���S���̍���e�N�X�`��U���W
	float                   m_fTexV;        // �|���S���̍���e�N�X�`��V���W
	float                   m_fTexWidth;    // �|���S���̃e�N�X�`�����W�̉���
	float                   m_fTexHeight;   // �|���S���̃e�N�X�`�����W�̏c��
	D3DXMATRIX              m_MtxWorld;     // ���[���h�}�g���b�N�X
	bool                    m_bLighting;    // ���C�e�B���O���邩���Ȃ���
};

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
	void DrawIcon(int nPlayerIdx);
	void BindTextureToIcon(void);
	void SortRankFromScore(void);

	//--------------------
	//	Set & Get
	//--------------------
	void SetNumPlayer(const int nNumPlayer) { m_nNumPlayer = nNumPlayer; }
	void SetNumAllCharacter(const int nNumAllCharacter) { m_nNumAllCharacter = nNumAllCharacter; }
	void SetScoreGame(CScoreGame *pScoreGame, const int nIdx) { m_pScoreGame[nIdx] = pScoreGame; }
	void SetMiniMap(CMiniMap *pMiniMap) { m_pMiniMap = pMiniMap; }
	void SetSkilicon(CSkilicon *pSkilicon, const int nIdx) { m_pSkilicon[nIdx] = pSkilicon; }
	void SetCharaicon(CCharaicon *pCharaicon, const int nPlayerIdx, const int nIdx) { m_pChraicon[nPlayerIdx][nIdx] = pCharaicon; }
	void SetPlayerIdxicon(CIconBillboard *pPlayerIdxicon, const int nIdx) { m_pPlayerIdxicon[nIdx] = pPlayerIdxicon; }
	void SetCrownicon(CIconBillboard *pCrownicon, const int nIdx) { m_pCrownicon[nIdx] = pCrownicon; }
	int GetNumPlayer(void) { return m_nNumPlayer; }
	int GetNumAllCharacter(void) { return m_nNumAllCharacter; }
	CScoreGame *GetScoreGame(const int nIdx) { return m_pScoreGame[nIdx]; }
	CMiniMap *GetMiniMap(void) { return m_pMiniMap; }
	CSkilicon *GetSkilicon(const int nIdx) { return m_pSkilicon[nIdx]; }
	CCharaicon *GetCharaicon(const int nPlayerIdx, const int nIdx) { return m_pChraicon[nPlayerIdx][nIdx]; }
	CIconBillboard *GetPlayerIdxicon(const int nIdx) { return m_pPlayerIdxicon[nIdx]; }
	CIconBillboard *GetCrownicon(const int nIdx) { return m_pCrownicon[nIdx]; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	//--------------------
	//	�֐�
	//--------------------
	void CreateScoreGame(void);
	void CreateMiniMap(void);
	void CreateCharaicon(void);
	void CreatePlayerIdxicon(void);
	void CreateCrownRank(void);
	void BindTextureToIcon_Chara(CGame *pGame);
	void BindTextureToIcon_PlayerIdx(CGame *pGame);
	void BindTextureToIcon_Crown(CGame *pGame);

	void ReleaseScoreGame(void);
	void ReleaseMiniMap(void);
	void ReleaseSkilicon(void);
	void ReleaseCharaicon(void);
	void ReleasePlayerIdxicon(void);
	void ReleaseCrownIcon(void);

	void UpdateSkilicon(void);
	void UpdateCharaicon(void);
	void UpdatePlayerIdxicon(CGame *pGame);
	void UpdateCrownicon(CGame *pGame);

	void DrawSkilicon(void);
	void DrawCharaicon(void);
	void DrawPlayerIdxicon(void);
	void DrawCrownIcon(void);

	//--------------------
	//	�����o�ϐ�
	//--------------------
	int m_nNumPlayer;											// �Q�[���ɎQ�����Ă���v���C���[�̐l��(�ۑ��p)
	int m_nNumAllCharacter;										// �Q�[���ɎQ�����Ă���l��(�ۑ��p)
	CScoreGame *m_pScoreGame[MAX_PLAYERNUM * 2];				// �Q�[���X�R�A�N���X�ւ̃|�C���^
	CMiniMap *m_pMiniMap;										// �~�j�}�b�v�N���X�ւ̃|�C���^
	CSkilicon *m_pSkilicon[MAX_PLAYERNUM];						// �X�L���A�C�R���N���X�ւ̃|�C���^
	CCharaicon *m_pChraicon[MAX_PLAYERNUM][UI_RANKICON_NUM];	// �L�����A�C�R���ւ̃|�C���^
	int m_nRank[MAX_PLAYERNUM * 2];								// ���݂̏���(8�l��)
	CIconBillboard *m_pPlayerIdxicon[MAX_PLAYERNUM * 2];		// �v���C���[�ԍ��A�C�R��
	CIconBillboard *m_pCrownicon[UI_RANKICON_NUM];				// �����A�C�R��
	LPDIRECT3DTEXTURE9 *m_pTexture;								// �Q�[���N���X���ۗL����e�N�X�`���ւ̃|�C���^
};

#endif