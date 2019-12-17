//=============================================================================
//
// �����Տ��� [ecprosiontrace.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _EXPRISIONTRACE_H_
#define _EXPRISIONTRACE_H_

#include "main.h"
#include "scene3D.h"

//=============================================================================
// �����ՃN���X
//=============================================================================
class CExprosionTrace : public CScene3D
{
public:
	CExprosionTrace();
	CExprosionTrace(int nPriority, CScene::OBJTYPE obj);
	~CExprosionTrace();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CExprosionTrace * Create(const D3DXVECTOR3 pos);
	static void LoadTex(void);
	static void UnloadTex(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	int m_nLife;	//�\������
	bool m_bDeth;	//���S�t���O
};
#endif
