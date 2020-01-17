//*****************************************************************************
//
//     �G�t�F�N�g(3D)�̏���[effect3D.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _EFFECT3D_H_
#define _EFFECT3D_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
//    �G�t�F�N�g(3D)�N���X�̒�`
//*****************************************************************************
class CEffect3D : public CScene
{
public:		// �N�ł��A�N�Z�X�\
	CEffect3D(int nPriority = 3, OBJTYPE objType = OBJTYPE_EFFECT3D);
	~CEffect3D();

	static CEffect3D *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nLife, float fCutAlpha, float fCutLength, bool bLighting = false, int nPriority = 3, float fRot = 0.0f, float fTexU = 0.0f, float fTexV = 0.0f, float fTexWidth = 1.0f, float fTexHeight = 1.0f);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size) {}
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	virtual void CalcNotScaleTransform(const LPDIRECT3DDEVICE9 pDevice);
	void SetTransformToDevice(const LPDIRECT3DDEVICE9 pDevice);
	void SetScale(float fWidth, float fHeight);
	virtual void SetVtxBuffValue(void);
	virtual void SetVtxBuffPos(void);
	void SetVtxBuffNor(void);
	void SetVtxBuffCol(void);
	void SetVtxBuffTex(void);


	//------------------------
	//  Set & Get
	//------------------------
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
	void SetMtxWorld(const D3DXMATRIX mtxWorld) { m_MtxWorld = mtxWorld; }
	void SetLighting(const bool bLighting) { m_bLighting = bLighting; }
	void SetLife(const int nLife) { m_nLife = nLife; }
	void SetCutAlpha(const float fCutAlpha) { m_fCutAlpha = fCutAlpha; }
	void SetCutLength(const float fCutLength) { m_fCutLength = fCutLength; }

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
	D3DXMATRIX GetMtxWorld(void) { return m_MtxWorld; }
	bool GetLighting(void) { return m_bLighting; }
	int GetLife(void) { return m_nLife; }
	float GetCutAlpha(void) { return m_fCutAlpha; }
	float GetCutLength(void) { return m_fCutLength; }

protected:	// ���̃N���X�Ɣh���N���X�������A�N�Z�X�\
	void ClearVariable(void);

private:	// ���̃N���X�������A�N�Z�X�\
	//------------------------
	//  �֐�
	//------------------------
	void SetValue(void);

	//------------------------
	//  �ÓI�����o�ϐ�
	//------------------------
	static LPDIRECT3DTEXTURE9 m_pTexture;

	//------------------------
	//  �����o�ϐ�
	//------------------------
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3             m_Pos;			// �|���S���̍��W
	D3DXCOLOR               m_Col;			// �|���S���̐F
	float                   m_fRot;			// �|���S���̌���
	float                   m_fWidth;		// �|���S���̕�
	float                   m_fHeight;		// �|���S���̍���
	float                   m_fAngle;		// �|���S�����o���p�x
	float                   m_fLength;		// �|���S���̒���
	float                   m_fTexU;		// �|���S���̍���e�N�X�`��U���W
	float                   m_fTexV;		// �|���S���̍���e�N�X�`��V���W
	float                   m_fTexWidth;	// �|���S���̃e�N�X�`�����W�̉���
	float                   m_fTexHeight;	// �|���S���̃e�N�X�`�����W�̏c��
	D3DXMATRIX              m_MtxWorld;		// ���[���h�}�g���b�N�X
	bool                    m_bLighting;	// ���C�e�B���O���邩���Ȃ���
	int                     m_nLife;		// ����
	float                   m_fCutAlpha;	// �����x������
	float                   m_fCutLength;	// �傫��������
};

#endif