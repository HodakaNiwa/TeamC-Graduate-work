//*****************************************************************************
//
//     ���b�V�������O�̏���[meshRing.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MESHRING_H_
#define _MESHRING_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
//    ���b�V�������O�N���X�̒�`
//*****************************************************************************
class CMeshRing : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	CMeshRing(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESHRING);
	~CMeshRing();

	static CMeshRing *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, float fDiffusion, int nXBlock, int nYBlock, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size) {}
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);
	void SetVtxBuffValue(void);
	void SetVtxBuffPos(void);
	void SetVtxBuffCol(void);
	void SetVtxBuffNor(void);
	void SetVtxBuffTex(void);

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);
	void SetCol(const D3DXCOLOR col);
	void SetHeight(const float fHeight);
	void SetRadius(const float fRadius);
	void SetDiffusion(const float fDiffusion);
	void SetXBlock(const int nXBlock);
	void SetYBlock(const int nYBlock);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXMATRIX GetMtxWorld(void);
	D3DXCOLOR GetCol(void);
	float GetHeight(void);
	float GetRadius(void);
	float GetDiffusion(void);
	int GetXBlock(void);
	int GetYBlock(void);
	int GetNumVertex(void);
	int GetNumIndex(void);
	int GetNumPolygon(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\
	void ClearVariable(void);

private:   // ���̃N���X�������A�N�Z�X�\
	void CalcNotScaleTransform(void);
	void SetTransformToDevice(const LPDIRECT3DDEVICE9 pDevice);

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;		// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3             m_Pos;			// ���b�V�������O�̈ʒu
	D3DXVECTOR3             m_Rot;			// ���b�V�������O�̌���
	D3DXMATRIX              m_MtxWorld;		// ���b�V�������O�̃��[���h�}�g���b�N�X
	D3DXCOLOR               m_Col;			// ���b�V�������O�̐F
	float                   m_fHeight;		// ���b�V�������O�̍���
	float                   m_fRadius;		// ���b�V�������O�̔��a
	float                   m_fDiffusion;	// ���b�V�������O�̊g�U�l
	int                     m_nXBlock;		// ���̕�����
	int                     m_nYBlock;		// �c�̕�����
	int                     m_nNumVertex;	// ���_��
	int                     m_nNumIndex;	// �C���f�b�N�X��
	int                     m_nNumPolygon;	// �|���S����
};

#endif