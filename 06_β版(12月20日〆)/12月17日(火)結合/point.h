//=============================================================================
//
// �|�C���g�̏��� [point.h]
// Author : ���� �����@[ SATO ASUMI ]
//
//=============================================================================
#ifndef _POINT_H_
#define _POINT_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// �O���錾
//=============================================================================
class CNumberBillborad;
class CSceneBillborad;

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_NUMBER_POINT	(2)

//=============================================================================
// �X�R�A�N���X
//=============================================================================
class CPoint : public CScene
{
public:
	//�����o�֐�
	CPoint();
	~CPoint();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPoint * Create(int nPoint, D3DXVECTOR3 pos, D3DXCOLOR Color);
	void AddScore(int nScore);
	int GetScore(void);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size) {};
	static void LoadTex(void);
	static void UnloadTex(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	int m_nPoint;
	int m_nLife;
	int m_nNumber[MAX_NUMBER_POINT];
	bool m_bDeth;
	D3DXCOLOR m_Color;
	D3DXVECTOR3 m_pos;
	CNumberBillborad * m_pNumber[MAX_NUMBER_POINT];
	CSceneBillborad * m_pPlus;
};
#endif
