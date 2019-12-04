//=============================================================================
//
// ビルボード処理 [scenebillboard.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _SCENEBILLBORD_H_
#define _SCENEBILLBORD_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// ビルボードクラス
//=============================================================================
class CSceneBillborad : public CScene
{
public:
	CSceneBillborad();
	CSceneBillborad(int nPriority, OBJTYPE ObjType);
	~CSceneBillborad();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CSceneBillborad * Create(const D3DXVECTOR3 pos);
	D3DXMATRIX * GetMtxWorld(void) { return &m_mtxWorld; };		//ワールドマトリックスの取得
	void SetSize(D3DXVECTOR3 size);
	void SetPos(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetSize(void) { return m_size; }				//サイズの設定
	D3DXVECTOR3 GetPos(void) { return m_pos; }
protected:
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void SetColor(D3DXCOLOR color);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	void SetUV(D3DXVECTOR2 uvMIn, D3DXVECTOR2 uvMax);
	void SetColorABillborad(float colA);
	D3DXVECTOR3		m_pos;		//中心座標
	D3DXVECTOR3		m_size;		//サイズ
	D3DXVECTOR3		m_rot;		//向きの設定

private:

	//メンバ変数
	LPDIRECT3DTEXTURE9		m_pTexture;			// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			//頂点バッファへのポインタ
	D3DXMATRIX				m_mtxWorld;			//ワールドマトリックス
	D3DXCOLOR				m_Color;	//色
};
#endif