//*****************************************************************************
//
//     シェーダーの処理[shader.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SHADER_H_
#define _SHADER_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    シェーダークラスの定義
//*****************************************************************************
class CShader
{
public:    // 誰でもアクセス可能
	CShader(char *pFileName);
	virtual ~CShader();

	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void SetParamToEffect(void) = 0;

	HRESULT Begin(void);
	HRESULT End(void);

	HRESULT BeginPass(int nIdx = 0);
	HRESULT EndPass(void);

	HRESULT CommitChanges(void);

protected: // このクラスと派生クラスだけがアクセス可能
	void SetFileName(char *pFileName);

	LPD3DXEFFECT m_pEffect;
	D3DXHANDLE m_hTechnique;
	char m_pFileName[256];

private:   // このクラスだけがアクセス可能
};


//*****************************************************************************
//    インスタンシングシェーダークラスの定義
//*****************************************************************************
class CInstancingShader : public CShader
{
public:    // 誰でもアクセス可能
	CInstancingShader(char *pFileName);
	~CInstancingShader();

	static CInstancingShader *Create(char *pFileName);

	HRESULT Init(void);
	void Uninit(void);
	void SetParamToEffect(void);

	void SetMtxView(const D3DXMATRIX mtxView) { m_MtxView = mtxView; }
	void SetMtxProjection(const D3DXMATRIX mtxProjection) { m_MtxProjection = mtxProjection; }
	void SetMtxLocal(const D3DXMATRIX mtxLocal) { m_MtxLocal = mtxLocal; }
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }
	void SetLightDir(const D3DXVECTOR3 LightDir) { m_LightDir = LightDir; }
	void SetLightDiffuse(const D3DXCOLOR LightDiffuse) { m_LightDiffuse = LightDiffuse; }
	void SetLightAmbient(const D3DXCOLOR LightAmbient) { m_LightAmbient = LightAmbient; }

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	D3DXMATRIX m_MtxView;				// ビューマトリックス
	D3DXMATRIX m_MtxProjection;			// プロジェクションマトリックス
	D3DXMATRIX m_MtxLocal;				// ローカル座標におけるワールドマトリックス
	LPDIRECT3DTEXTURE9 m_pTexture;		// テクスチャへのポインタ
	D3DXVECTOR3 m_LightDir;				// ライトの方向
	D3DXCOLOR m_LightDiffuse;			// ライトの拡散光
	D3DXCOLOR m_LightAmbient;			// ライトの環境光

	// 各パラメータのハンドル
	D3DXHANDLE m_hMtxView;
	D3DXHANDLE m_hMtxProjection;
	D3DXHANDLE m_hMtxLocal;
	D3DXHANDLE m_hTexture;
	D3DXHANDLE m_hLightDir;
	D3DXHANDLE m_hLightDiffuse;
	D3DXHANDLE m_hLightAmbient;
};

#endif