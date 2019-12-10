//*****************************************************************************
//
//     エフェクト管轄の処理[effectManager.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _EFFECTMANAGER_H_
#define _EFFECTMANAGER_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define MAX_EMMITER         (150)     // エミッタデータの最大数
#define MAX_PARTICLEDATA    (150)     // パーティクルデータの最大数
#define MAX_RINGEFFECTDATA  (150)     // リングエフェクトデータの最大数

//*****************************************************************************
//     前方宣言
//*****************************************************************************
class CFileLoader;
class CFileSaver;
class CEmitterData;
class CRingData;
class CEmitter;
class CTextureManager;

//*****************************************************************************
//     エフェクトマネージャークラスの定義
//*****************************************************************************
class CEffectManager
{
public:   // 誰でもアクセス可能
	typedef enum
	{
		EFFECT_TYPE_PARTICLE = 0,  // パーティクル
		EFFECT_TYPE_RING,          // リング
		EFFECT_TYPE_MAX
	}EFFECT_TYPE;

	CEffectManager();
	~CEffectManager();

	static CEffectManager *Create(char *pFileName);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	CEmitter *SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority = 3);

	void SetFileName(char *pFileName);
	void SetTextureListFileName(char *pFileName);
	void SetEmitter(CEmitter *pEmitter);
	void SetEmitterData(CEmitterData *pEmitterData, const int nIdx);
	void SetRingEffectData(CRingData *pRingData, const int nIdx);
	void SetNumEmitterData(const int nNumEmitterData);
	void SetNumRingEffectData(const int nNumRingEffectData);
	void SetTextureManager(CTextureManager *pTextureManager);

	char *GetFileName(void);
	char *GetTextureListFileName(void);
	CEmitter *GetEmitter(void);
	CEmitterData **GetEmitterData(void);
	CEmitterData *GetEmitterData(const int nIdx);
	CRingData **GetRingEffectData(void);
	CRingData *GetRingEffectData(const int nIdx);
	int GetNumEmitterData(void);
	int GetNumRingEffectData(void);
	CTextureManager *GetTextureManager;

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void CreateTextureMemory(const int nNumTexture);
	void CreateEmitterDataMemory(const int nNumEmitterData);
	void CreateRingEffectDataMemory(const int nNumRingEffectData);

	void ReleaseTextureManager(void);
	void ReleaseEmitterData(void);
	void ReleaseRingEffectData(void);

	HRESULT LoadScript(CFileLoader *pFileLoader, char *pStr);
	HRESULT LoadTexture(char *pTextureFileName, int nCntTex);
	CEmitterData *LoadEmitterData(CFileLoader *pFileLoader, char *pStr, const int nCntEmitter);
	CRingData *LoadRingEffectData(CFileLoader *pFileLoader, char *pStr, const int nCntRingData);

	void Save(void);
	void SaveTextureData(CFileSaver *pFileSaver);
	void SaveEmitterData(CFileSaver *pFileSaver);
	void SaveRingEffectData(CFileSaver *pFileSaver);

	char            m_aFileName[256];				// スクリプトファイル名
	char            m_aTextureListFileName[256];	// テクスチャリストファイル名
	CEmitter        *m_pEmitter;					// 最後に生成したエミッタクラスへのポインタ
	int             m_nNumEmitterData;				// エミッタデータの個数
	int             m_nNumRingEffectData;			// リングエフェクトデータの個数
	CEmitterData    **m_apEmitterData;				// エミッタデータクラスへのポインタ
	CRingData       **m_apRingData;					// リングエフェクトデータクラスへのポインタ
	CTextureManager *m_pTextureManager;				// テクスチャ管轄クラスへのポインタ
};

#endif