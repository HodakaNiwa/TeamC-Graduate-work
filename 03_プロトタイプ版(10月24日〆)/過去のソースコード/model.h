//=============================================================================
//
// モデル専用の処理 [model.h]
// Author : 有馬武志
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=====================
//  CModel 単独クラス
//=====================
class CModel
{
public://誰でも扱える
	CModel();
	~CModel();
	HRESULT Init(char FileName[40]);
	void Uninit(void);
	void Update(void);
	void Draw(float fAlpha);
	static CModel * Create(const D3DXVECTOR3 pos, char FileName[40], D3DXVECTOR3 Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }

	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_Rot; }

	void SetFirstPos(D3DXVECTOR3 FirstPos) { m_FirstPos = FirstPos; };
	void SetParent(CModel *pModel) { m_pParent = pModel; }
	void AddPos(D3DXVECTOR3 Pos) { m_Pos = Pos + m_FirstPos; };
	void BindModel(LPD3DXMESH mesh, LPD3DXBUFFER buff, DWORD nummat) { m_pMesh = mesh; m_pBuffMat = buff; m_nNumMat = nummat; }
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTextures = pTexture; }
	D3DXMATRIX &GetMtxWorld(void) { return m_mtxWorld; }
	D3DXVECTOR3 GetPos(void) { return m_Pos; }
	D3DXVECTOR3 GetVtxMax(void) { return m_VtxMax; }
	D3DXVECTOR3 GetVtxMin(void) { return m_VtxMin; }
	D3DXVECTOR3 GetScale(void) { return m_Scale; }
	void SetColor(D3DXCOLOR col);

protected://派生クラスでも使える
	void CreateXFile(char FileName[40]);						//Xファイルの生成
	LPD3DXMESH					m_pMesh;						//メッシュ情報へのポインタ
	LPD3DXBUFFER				m_pBuffMat;						//マテリアルの情報へのポインタ
	DWORD						m_nNumMat;						//マテリアルの情報数
	LPDIRECT3DTEXTURE9			m_pTextures;					//テクスチャ
	D3DXMATRIX					m_mtxWorld;						//ワールドマトリックス
	D3DXVECTOR3					m_VtxMin,m_VtxMax;				//モデルの最小値・最大値
	D3DXVECTOR3					m_Pos;							//位置
	D3DXVECTOR3					m_Rot;							//向き
	D3DXVECTOR3					m_FirstPos;						//初期位置
	D3DXVECTOR3					m_Scale;						//拡大、縮小率
	CModel						*m_pParent;						//親モデルへのポインタ

private://個人のみ使える
	void SetScale(D3DXVECTOR3 Scale) { m_Scale = Scale; };
};
#endif