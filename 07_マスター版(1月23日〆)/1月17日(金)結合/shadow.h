//=============================================================================
//
// âeÇÃèàóù [shadow.h]
// Author : ç≤ì°à¿èÉ Sato_Asumi
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"
#include "scene3D.h"

//=============================================================================
// âeÉNÉâÉX
//=============================================================================
class CShadow : public CScene3D
{
public:
	CShadow();
	CShadow(int nPriority, CScene::OBJTYPE obj);
	~CShadow();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CShadow * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	static void LoadTex(void);
	static void UnloadTex(void);
	static LPDIRECT3DTEXTURE9 GetTexture(void);

protected:
	static LPDIRECT3DTEXTURE9 m_pTexture;

private:
};
#endif
