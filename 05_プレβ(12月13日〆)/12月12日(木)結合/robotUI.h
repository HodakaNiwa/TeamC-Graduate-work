//====================================================================================
//
// ��9���UI����	[robotUI.h]
// Author;�r�J�R�N
//
//====================================================================================
#ifndef _ROBOTUI_H_
#define _ROBOTUI_H_

//=============================================================================
//    �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
//    �}�N����`
//=============================================================================
#define ROBOTUI_TEXTURE_MAX (1)		// �e�N�X�`���̍ő吔

//=============================================================================
//    �O���錾
//=============================================================================
class CScene2D;
class CCharacter;
class CGame;

//=============================================================================
//    UI�N���X�̒�`
//=============================================================================
class CRobotUI2D : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	CRobotUI2D();
	~CRobotUI2D();

	static CRobotUI2D *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fTexU = 0.0f, float fTexV = 0.0f, float fTexWidth = 1.0f, float fTexHeight = 1.0f, float fRot = 0.0f , int nNum = 0);

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
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size) {};

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

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

private:   // ���̃N���X�������A�N�Z�X�\
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
};

//=============================================================================
// ���{�b�g�o��UI�N���X�̒�`
//=============================================================================
class CRobotUI
{
public:    // �N�ł��A�N�Z�X�\
	CRobotUI();
	~CRobotUI();

	static CRobotUI *Create(int nNum);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(int nNum);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//--------------------
	//	Set & Get
	//--------------------
	void SetWarning(CRobotUI2D *pWarning) { m_pRobotWarning = pWarning; }

	CRobotUI2D *GetWarning(void) { return m_pRobotWarning; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	//--------------------
	//	�֐�
	//--------------------
	void CreateWarning(int nNum);
	void ReleaseWarning(void);
	void UpdateWarning(void);

	//--------------------
	//	�ÓI�����o�ϐ�
	//--------------------
	static LPDIRECT3DTEXTURE9 m_apTexture[ROBOTUI_TEXTURE_MAX];

	//--------------------
	//	�����o�ϐ�
	//--------------------
	CRobotUI2D	*m_pRobotWarning;	// �댯�x��
	int			m_nNum;				// �ԍ�
};
#endif