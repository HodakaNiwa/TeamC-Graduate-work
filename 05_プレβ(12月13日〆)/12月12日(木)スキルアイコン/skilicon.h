//*****************************************************************************
//
//     �X�L���A�C�R���̏���[skilicon.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SKILICON_H_
#define _SKILICON_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define SKILICON_TEXTURE_MAX (6)
#define SKILLICON_GROW_TEX	 (4)	//��

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CScene2D;
class CCharacter;
class CScene2D;

//*****************************************************************************
//    �X�L���A�C�R���p�|���S���N���X�̒�`
//*****************************************************************************
class CIcon2D
{
public:     // �N�ł��A�N�Z�X�\
	CIcon2D();
	~CIcon2D();

	static CIcon2D *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fTexU = 0.0f, float fTexV = 0.0f, float fTexWidth = 1.0f, float fTexHeight = 1.0f, float fRot = 0.0f);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetScale(float fWidth, float fHeight);
	virtual void SetVtxBuffValue(void);
	virtual void SetVtxBuffPos(void);
	void SetVtxBuffCol(void);
	void SetVtxBuffTex(void);

	//--------------------
	//	Set & Get
	//--------------------
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff) { m_pVtxBuff = pVtxBuff; }
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
	void SetUpdateFlag(bool bUpdate) { m_bUpdate = bUpdate; }	//��
	void SetEffectFlag(bool bEffect) { m_bEffect = bEffect; }	//��
	void SetRingFlag(bool bRing) { m_bRing = bRing; }			//��
	void SetLife(int nLife) { m_nLife = nLife; }				//��
	void SetOneTime(bool bOneTime) { m_bOneTime = bOneTime; }	//��

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }
	D3DXVECTOR3 GetPos(void) { return m_Pos; }
	D3DXCOLOR GetCol(void) { return m_Col; }
	float GetRot(void) { return m_fRot; }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
	float GetAngle(void) { return m_fAngle; }
	float GetLength(void) { return m_fLength; }
	float GetTexU(void) { return m_fTexU; }
	float GetTexV(void) { return m_fTexV; }
	float GetTexWidth(void) { return m_fTexWidth; }
	float GetTexHeight(void) { return m_fTexHeight; }
	bool GetUpdateFlag(void) { return m_bUpdate; }	//��
	bool GetEffectFlag(void) { return m_bEffect; }	//��
	bool GetRingFlag(void) { return m_bRing; }	//��
	bool GetOneTime(void) { return m_bOneTime;}		//��

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

private:   // ���̃N���X�������A�N�Z�X�\
	void UpdateGrow(void);
	void UpdateRight(void);

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;     // ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTexture;	    // �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3				m_Pos;	        // �|���S���̈ʒu
	D3DXCOLOR               m_Col;          // �|���S���̐F
	float                   m_fRot;         // �|���S���̌���
	float                   m_fAngle;       // �|���S�����o���p�x
	float                   m_fWidth;       // �|���S���̕�
	float                   m_fHeight;      // �|���S���̍���
	float                   m_fLength;      // �|���S���̒���
	float                   m_fTexU;        // �|���S���̍���e�N�X�`��U���W
	float                   m_fTexV;        // �|���S���̍���e�N�X�`��V���W
	float                   m_fTexWidth;    // �|���S���̃e�N�X�`�����W�̉���
	float                   m_fTexHeight;   // �|���S���̃e�N�X�`�����W�̏c��

	//��
	bool					m_bUpdate;		// �X�V���邩�ǂ���
	bool					m_bEffect;		// �G�t�F�N�g���X�V���邩�ǂ���
	bool					m_bOneTime;		// ��x������������ǂ���
	bool					m_bRing;		// �����O�̍X�V����
	float					m_fChangeCol;	// �F�̕ω���
	int						m_nLife;		// �\������
};

//*****************************************************************************
//    �X�L���A�C�R���N���X�̒�`
//*****************************************************************************
class CSkilicon
{
public:    // �N�ł��A�N�Z�X�\
	CSkilicon();
	~CSkilicon();

	static CSkilicon *Create(CCharacter *pChara, int nIdx, int nRecastTime);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(CCharacter *pChara, int nIdx, int nRecastTime);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void RevivalIconMask(void);
	void RevivalIconMask_teq(void);

	//--------------------
	//	Set & Get
	//--------------------
	void SetIconBG(CIcon2D *pIconBG) { m_pIconBG = pIconBG; }
	void SetIcon(CIcon2D *pIcon) { m_pIcon = pIcon; }
	void SetIconMask(CIcon2D *pIconMask) { m_pIconMask = pIconMask; }
	void SetParentChara(CCharacter *pChara) { m_pParentChara = pChara; }
	void SetIdx(const int nIdx) { m_nIdx = nIdx; }
	void SetRecastTime(const int nRecastTime) { m_nRecastTime = nRecastTime; }

	CIcon2D *GetIconBG(void) { return m_pIconBG; }
	CIcon2D *GetIcon(void) { return m_pIcon; }
	CIcon2D *GetIconMask(void) { return m_pIconMask; }
	CCharacter *GetCharacter(void) { return m_pParentChara; }
	int GetIdx(void) { return m_nIdx; }
	int GetRecastTime(void) { return m_nRecastTime; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	//--------------------
	//	�֐�
	//--------------------
	void CreateIconBG(void);
	void CreateIconMask(void);
	void CreateIcon(void);
	void CreateIconGrow(void);	//��
	void CreateIconRight(void);	//��
	void ReleaseIconBG(void);
	void ReleaseIconMask(void);
	void ReleaseIcon(void);
	void ReleaseIconGrow(void);	//��
	void ReleaseIconRight(void);	//��
	void RecastIcon(void);
	void DrawIconBG(void);
	void DrawIconMask(const LPDIRECT3DDEVICE9 pDevice);
	void DrawIcon(const LPDIRECT3DDEVICE9 pDevice);
	void DrawIconGrow(const LPDIRECT3DDEVICE9 pDevice);	//��
	void DrawIconRight(const LPDIRECT3DDEVICE9 pDevice);	//��
	void ClearStencil(const LPDIRECT3DDEVICE9 pDevice);

	//--------------------
	//	�ÓI�����o�ϐ�
	//--------------------
	static LPDIRECT3DTEXTURE9 m_apTexture[SKILICON_TEXTURE_MAX];
	
	//��
	static LPDIRECT3DTEXTURE9 m_apTectureGrow[SKILLICON_GROW_TEX];

	//--------------------
	//	�����o�ϐ�
	//--------------------
	CIcon2D *m_pIconBG;				// �X�L���A�C�R��(�w�i)
	CIcon2D *m_pIconMask;			// �X�L���A�C�R���}�X�N�p
	CIcon2D *m_pIcon;				// �X�L���A�C�R��
	CIcon2D * m_pGrowIcon;			// �A�C�R���̌�
	CIcon2D * m_pRightEffect;		// ���̃G�t�F�N�g
	CIcon2D * m_pRingEffect;		// �~�̃G�t�F�N�g
	CCharacter *m_pParentChara;		// ��Ԃ�\������L�����N�^�[�ւ̃|�C���^
	int m_nIdx;						// ���g�̔ԍ�
	int m_nRecastTime;				// ���L���X�g�ɂ����鎞��
};

#endif