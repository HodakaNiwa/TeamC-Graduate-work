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
#include "light.h"

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

//*****************************************************************************
//    地面シェーダークラスの定義
//*****************************************************************************
class CFieldShader : public CShader
{
public:    // 誰でもアクセス可能
	CFieldShader(char *pFileName);
	~CFieldShader();

	static CFieldShader *Create(char *pFileName);

	HRESULT Init(void);
	void Uninit(void);
	void SetParamToEffect(void);

	void SetMtxView(const D3DXMATRIX mtxView) { m_MtxView = mtxView; }
	void SetMtxProjection(const D3DXMATRIX mtxProjection) { m_MtxProjection = mtxProjection; }
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }
	void SetLightDir(const D3DXVECTOR3 LightDir, const int nIdx) { m_LightDir[nIdx] = LightDir; }
	void SetLightDiffuse(const D3DXCOLOR LightDiffuse, const int nIdx) { m_LightDiffuse[nIdx] = LightDiffuse; }

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	D3DXMATRIX m_MtxView;					// ビューマトリックス
	D3DXMATRIX m_MtxProjection;				// プロジェクションマトリックス
	LPDIRECT3DTEXTURE9 m_pTexture;			// テクスチャへのポインタ
	D3DXVECTOR3 m_LightDir[MAXLIGHT];		// ライトの方向
	D3DXCOLOR m_LightDiffuse[MAXLIGHT];		// ライトの拡散光

	// 各パラメータのハンドル
	D3DXHANDLE m_hMtxView;
	D3DXHANDLE m_hMtxProjection;
	D3DXHANDLE m_hTexture;
	D3DXHANDLE m_hLightDir[MAXLIGHT];
	D3DXHANDLE m_hLightDiffuse[MAXLIGHT];
};

//*****************************************************************************
//    アウトラインシェーダークラスの定義
//*****************************************************************************
class COutlineShader : public CShader
{
public:    // 誰でもアクセス可能
	COutlineShader(char *pFileName);
	~COutlineShader();

	static COutlineShader *Create(char *pFileName);

	HRESULT Init(void);
	void Uninit(void);
	void SetParamToEffect(void);

	void SetMtxView(const D3DXMATRIX mtxView) { m_MtxView = mtxView; }
	void SetMtxProjection(const D3DXMATRIX mtxProjection) { m_MtxProjection = mtxProjection; }
	void SetMtxWorld(const D3DXMATRIX mtxWorld) { m_MtxWorld = mtxWorld; }

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	D3DXMATRIX m_MtxView;				// ビューマトリックス
	D3DXMATRIX m_MtxProjection;			// プロジェクションマトリックス
	D3DXMATRIX m_MtxWorld;				// ワールドマトリックス

	// 各パラメータのハンドル
	D3DXHANDLE m_hMtxView;
	D3DXHANDLE m_hMtxProjection;
	D3DXHANDLE m_hMtxWorld;
};

//*****************************************************************************
//    リングシェーダークラスの定義
//*****************************************************************************
class CRingShader : public CShader
{
public:    // 誰でもアクセス可能
	CRingShader(char *pFileName);
	~CRingShader();

	static CRingShader *Create(char *pFileName);

	HRESULT Init(void);
	void Uninit(void);
	void SetParamToEffect(void);

	void SetMtxView(const D3DXMATRIX mtxView) { m_MtxView = mtxView; }
	void SetMtxProjection(const D3DXMATRIX mtxProjection) { m_MtxProjection = mtxProjection; }
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }
	void SetLightDir(const D3DXVECTOR3 LightDir, const int nIdx) { m_LightDir[nIdx] = LightDir; }
	void SetLightDiffuse(const D3DXCOLOR LightDiffuse, const int nIdx) { m_LightDiffuse[nIdx] = LightDiffuse; }

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	D3DXMATRIX m_MtxView;					// ビューマトリックス
	D3DXMATRIX m_MtxProjection;				// プロジェクションマトリックス
	LPDIRECT3DTEXTURE9 m_pTexture;			// テクスチャへのポインタ
	D3DXVECTOR3 m_LightDir[MAXLIGHT];		// ライトの方向
	D3DXCOLOR m_LightDiffuse[MAXLIGHT];		// ライトの拡散光

	// 各パラメータのハンドル
	D3DXHANDLE m_hMtxView;
	D3DXHANDLE m_hMtxProjection;
	D3DXHANDLE m_hTexture;
	D3DXHANDLE m_hLightDir[MAXLIGHT];
	D3DXHANDLE m_hLightDiffuse[MAXLIGHT];
};

//*****************************************************************************
//    テリトリーシェーダークラスの定義
//*****************************************************************************
class CTerritoryShader : public CShader
{
public:    // 誰でもアクセス可能
	CTerritoryShader(char *pFileName);
	~CTerritoryShader();

	static CTerritoryShader *Create(char *pFileName);

	HRESULT Init(void);
	void Uninit(void);
	void SetParamToEffect(void);

	void SetMtxView(const D3DXMATRIX mtxView) { m_MtxView = mtxView; }
	void SetMtxProjection(const D3DXMATRIX mtxProjection) { m_MtxProjection = mtxProjection; }
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }
	void SetLightDir(const D3DXVECTOR3 LightDir, const int nIdx) { m_LightDir[nIdx] = LightDir; }
	void SetLightDiffuse(const D3DXCOLOR LightDiffuse, const int nIdx) { m_LightDiffuse[nIdx] = LightDiffuse; }

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	D3DXMATRIX m_MtxView;					// ビューマトリックス
	D3DXMATRIX m_MtxProjection;				// プロジェクションマトリックス
	LPDIRECT3DTEXTURE9 m_pTexture;			// テクスチャへのポインタ
	D3DXVECTOR3 m_LightDir[MAXLIGHT];		// ライトの方向
	D3DXCOLOR m_LightDiffuse[MAXLIGHT];		// ライトの拡散光

	// 各パラメータのハンドル
	D3DXHANDLE m_hMtxView;
	D3DXHANDLE m_hMtxProjection;
	D3DXHANDLE m_hTexture;
	D3DXHANDLE m_hLightDir[MAXLIGHT];
	D3DXHANDLE m_hLightDiffuse[MAXLIGHT];
};

//*****************************************************************************
//    影シェーダークラスの定義(丸影)
//*****************************************************************************
class CShadowShader : public CShader
{
public:    // 誰でもアクセス可能
	CShadowShader(char *pFileName);
	~CShadowShader();

	static CShadowShader *Create(char *pFileName);

	HRESULT Init(void);
	void Uninit(void);
	void SetParamToEffect(void);

	void SetMtxView(const D3DXMATRIX mtxView) { m_MtxView = mtxView; }
	void SetMtxProjection(const D3DXMATRIX mtxProjection) { m_MtxProjection = mtxProjection; }
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }
	void SetLightDir(const D3DXVECTOR3 LightDir, const int nIdx) { m_LightDir[nIdx] = LightDir; }
	void SetLightDiffuse(const D3DXCOLOR LightDiffuse, const int nIdx) { m_LightDiffuse[nIdx] = LightDiffuse; }

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	D3DXMATRIX m_MtxView;					// ビューマトリックス
	D3DXMATRIX m_MtxProjection;				// プロジェクションマトリックス
	LPDIRECT3DTEXTURE9 m_pTexture;			// テクスチャへのポインタ
	D3DXVECTOR3 m_LightDir[MAXLIGHT];		// ライトの方向
	D3DXCOLOR m_LightDiffuse[MAXLIGHT];		// ライトの拡散光

	// 各パラメータのハンドル
	D3DXHANDLE m_hMtxView;
	D3DXHANDLE m_hMtxProjection;
	D3DXHANDLE m_hTexture;
	D3DXHANDLE m_hLightDir[MAXLIGHT];
	D3DXHANDLE m_hLightDiffuse[MAXLIGHT];
};

#endif