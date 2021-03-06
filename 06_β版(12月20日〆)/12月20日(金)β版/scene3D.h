//=============================================================================
//
// 3Dポリゴン処理 [scene3D.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// 3Dポリゴンクラス
//=============================================================================
class CScene3D : public CScene
{
public:
	CScene3D();
	CScene3D(int nPriority, CScene::OBJTYPE obj);
	~CScene3D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CScene3D * Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; };	//テクスチャの割当て

	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };									//位置の更新
	void SetColor(D3DXCOLOR col);
	void SetMoveTex(float fMoveX, float fMoveY);
	void SetDivision(D3DXVECTOR2 Division);											//テクスチャの分割数
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };									//向きの設定
	void SetTexUV(float fWhidth, float fHight, float fWhidth2, float fHight2);		//UVの分割数 安
	D3DXVECTOR3 GetPos(void) { return m_pos; };										//位置の取得
	D3DXVECTOR3 GetRot(void) { return m_rot; };										//位置の取得
	D3DXVECTOR3 GetSize(void) { return m_size; };									//大きさの取得
	D3DXCOLOR GetCol(void) { return m_col; }										//頂点カラーの取得
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }								//ワールドマトリックスの取得
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }					//バッファの取得
private:
	//メンバ変数
	LPDIRECT3DTEXTURE9		m_pTexture;												// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;												//頂点バッファへのポインタ
	D3DXVECTOR3				m_pos;													//中心座標
	D3DXVECTOR3				m_rot;													//向き
	D3DXVECTOR3				m_size;													//ポリゴンのサイズ
	D3DXMATRIX				m_mtxWorld;												//ワールドマトリックス
	D3DXCOLOR				m_col;													// 頂点カラー
	D3DXVECTOR2				m_Division;												//分割数

	D3DXVECTOR3 m_nor[2];
};
#endif
