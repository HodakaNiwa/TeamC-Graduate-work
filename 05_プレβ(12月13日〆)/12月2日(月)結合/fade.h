//=============================================================================
//
// フェードの処理 [fade.h]
// Author : 佐藤 安純　[ SATO ASUMI ]
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"
#include "scene.h"
#include "manager.h"


//=============================================================================
// フェードクラス
//=============================================================================
class CFade : public CScene
{
public:
	typedef enum
	{
		FADE_NONE = 0,
		FADE_IN,
		FADE_OUT,
	}FADE;

	//メンバ関数
	CFade();
	~CFade();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	//static CStock * Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);

	static void SetFade(CManager::MODE ModeNext);
	static FADE GetFadeMode(void);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	static FADE				m_fade;						// フェードの状態
	static D3DXCOLOR		m_Coloer;					// 色の設定
	static CManager::MODE	m_ModeNext;				//次のモード
};
#endif