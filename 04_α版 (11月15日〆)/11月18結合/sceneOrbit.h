//=============================================================================
//
// メッシュフィールド処理 [sceneMeshfiled.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _ORBIT_H_
#define _ORBIT_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CLoadTextOrbit;

//=============================================================================
// 軌跡クラス
//=============================================================================
class CSceneOrbit: public CScene
{
public:
	//軌跡の情報
	typedef struct
	{
		int							nTex;					//テクスチャの番号
		D3DXVECTOR3					pos[2];					//オフセット
		int							nPrimitiv;				//頂点数
		int							nPorigon;				//ポリゴン数
		D3DXCOLOR					color;					//色
		float						fUV;					//UVの分割数
		float						fAnim;					//アニメーション量
	}ORBIT_INFO;

	//奇跡の種類
	typedef enum
	{
		TYPE_PLAYER = 0,
		TYPE_STAR,
		TYPE_MAX,
	}TYPE;

	CSceneOrbit();
	~CSceneOrbit();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CSceneOrbit * Create(TYPE type, D3DXVECTOR3 pos);
	static void Load(void);
	static void Unload(void);
	void SetMtxParent(D3DXMATRIX * pMtx) { m_pMtxParent = pMtx; };			//マトリックスの始点
	void SetMtxParentEnd(D3DXMATRIX * pMtx) { m_pMtxParentEnd = pMtx; };	//マトリックスの終点
	void SetColor(D3DXCOLOR col);											//色の設定

private:
	//変数宣言
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void SetType(TYPE type) { m_type = type; };				//タイプの設定

	//メンバ変数
	static LPDIRECT3DTEXTURE9	* m_pTexture;				// テクスチャへのポインタ
	static ORBIT_INFO *			m_pOrbitInfo;				//軌跡の構造体ポインタ
	static int					m_nMaxTexture;				//テクスチャの最大数
	static CLoadTextOrbit *		m_pLoadText;				//テキストの情報
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;					//頂点バッファへのポインタ
	D3DXMATRIX					m_mtxWorld;					//ワールドマトリックス
	D3DXMATRIX *				m_pMtxParent;				//親マトリックスの情報
	D3DXMATRIX *				m_pMtxParentEnd;			//親マトリックスの情報
	D3DXVECTOR3	*				m_aPosVertex;				//頂点の計算用
	TYPE						m_type;						//タイプ
	D3DXVECTOR3					m_pos;						//位置
};
#endif
