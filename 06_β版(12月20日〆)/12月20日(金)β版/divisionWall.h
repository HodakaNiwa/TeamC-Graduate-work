//*****************************************************************************
//
//     ���f�C�x���g�p�̕ǂ̏���[divisionWall.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _DIVISIONWALL_H_
#define _DIVISIONWALL_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
//    ���f�C�x���g�p�̕ǃN���X�̒�`
//*****************************************************************************
class CDivisionWall : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	CDivisionWall(int nPriority = 3, OBJTYPE objType = OBJTYPE_DIVISIONWALL);
	~CDivisionWall();

	static CDivisionWall *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, float fMoveHeight, int nPriority = 3, float fTexU = 0.0f, float fTexV = 0.0f, float fTexWidth = 1.0f, float fTexHeight = 1.0f);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size) {}
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void SetScale(float fWidth, float fHeight);
	void SetVtxBuffValue(void);
	void SetVtxBuffPos(void);
	void SetVtxBuffCol(void);
	void SetVtxBuffTex(void);

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff) { m_pVtxBuff = pVtxBuff; }
	void SetPos(const D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetRot(const D3DXVECTOR3 rot) { m_Rot = rot; }
	void SetMtxWorld(const D3DXMATRIX mtxWorld) { m_MtxWorld = mtxWorld; }
	void SetCol(const D3DXCOLOR col) { m_Col = col; }
	void SetWidth(const float fWidth) { m_fWidth = fWidth; }
	void SetHeight(const float fHeight) { m_fHeight = fHeight; }
	void SetAngle(const float fAngle) { m_fAngle = fAngle; }
	void SetLength(const float fLength) { m_fLength = fLength; }
	void SetTexU(const float fTexU) { m_fTexU = fTexU; }
	void SetTexV(const float fTexV) { m_fTexV = fTexV; }
	void SetTexWidth(const float fTexWidth) { m_fTexWidth = fTexWidth; }
	void SetTexHeight(const float fTexHeight) { m_fTexHeight = fTexHeight; }
	void SetMoveHeight(const float fMoveHeight) { m_fMoveHeight = fMoveHeight; }

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }
	LPDIRECT3DTEXTURE9 GetTexture(void) { return m_pTexture; }
	D3DXVECTOR3 GetPos(void) { return m_Pos; }
	D3DXVECTOR3 GetRot(void) { return m_Rot; }
	D3DXMATRIX GetMtxWorld(void) { return m_MtxWorld; }
	D3DXCOLOR GetCol(void) { return m_Col; }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
	float GetAngle(void) { return m_fAngle; }
	float GetLength(void) { return m_fLength; }
	float GetTexU(void) { return m_fTexU; }
	float GetTexV(void) { return m_fTexV; }
	float GetTexWidth(void) { return m_fTexWidth; }
	float GetTexHeight(void) { return m_fTexHeight; }
	float GetMoveHeight(void) { return m_fMoveHeight; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void CalcNotScaleTransform(void);
	void SetTransformToDevice(const LPDIRECT3DDEVICE9 pDevice);

	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3				m_Pos;			// �|���S���̈ʒu
	D3DXVECTOR3				m_Rot;			// �|���S���̌���
	D3DXMATRIX				m_MtxWorld;		// ���[���h�}�g���b�N�X
	D3DXCOLOR				m_Col;			// �|���S���̐F
	float					m_fWidth;		// �|���S���̕�
	float					m_fHeight;		// �|���S���̍���
	float					m_fAngle;		// �|���S���̊p�x
	float					m_fLength;		// �|���S���̒���
	float					m_fTexU;		// �|���S���̍���e�N�X�`��U���W
	float					m_fTexV;		// �|���S���̍���e�N�X�`��V���W
	float					m_fTexWidth;	// �|���S���̃e�N�X�`�����W�̉���
	float					m_fTexHeight;	// �|���S���̃e�N�X�`�����W�̏c��
	float					m_fMoveHeight;	// �����̈ړ���
};

#endif