//=============================================================================
//
// Xファイル処理 [sceneX.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _SCENEX_H_
#define _SCENEX_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// Xファイルクラス
//=============================================================================
class CSceneX : public CScene
{
public:

	typedef enum
	{
		TYPE_HART = 0,
		TYPE_OBJ,
		TYPE_ARROW,
	}TYPE;

	CSceneX();
	CSceneX(int nPriority, OBJTYPE objtype);
	~CSceneX();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CSceneX * Create(const D3DXVECTOR3 pos, D3DXVECTOR3 rot, char FileName[80], LPDIRECT3DTEXTURE9 pTexture, TYPE type);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };					//中心座標の設定
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };					//中心座標の向き
	void SetMat(float Mat) { m_Mat_a = Mat; };					//中心座標の向き
	D3DXVECTOR3 GetPos(void) { return m_pos; };
	D3DXVECTOR3 GetRot(void) { return m_rot; };
	float GetMat_a(void) { return m_Mat_a; };
	D3DXVECTOR3 GetvtxMin(void) { return m_vtxMin; };
	D3DXVECTOR3 GetvtxMax(void) { return m_vtxMax; };

protected:
	void Load(char FileName[80], LPDIRECT3DTEXTURE9 pTexture);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	D3DXVECTOR3			m_vtxMin;		//頂点の最小値
	D3DXVECTOR3			m_vtxMax;		//頂点の最大値

private:
	//変数宣言

	LPDIRECT3DTEXTURE9 * m_ppTexture;		// テクスチャへのポインタ
	LPD3DXMESH			 m_pMesh;			//メッシュ情報へのポインタ
	LPD3DXBUFFER		 m_pBuffMat;		//マテリアル情報へのポインタ
	DWORD				 m_nNumMat;			//マテリアル情報の数

	//メンバ変数
	D3DXVECTOR3				m_pos;				//中心座標
	D3DXVECTOR3				m_rot;				//向き
	D3DXMATRIX				m_mtxWorld;			//ワールドマトリックス
	float					m_Mat_a;			//マテリアルのα値
};
#endif