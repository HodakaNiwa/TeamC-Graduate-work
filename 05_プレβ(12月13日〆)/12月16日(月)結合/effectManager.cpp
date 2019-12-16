//*****************************************************************************
//
//     エフェクト管轄の処理[effectManager.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "effectManager.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"
#include "emitter.h"
#include "particle.h"
#include "ringEffect.h"
#include "textureManager.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
//#define EFFECT_AUTO_SAVE                                    // 宣言時 : オートセーブ
#define EFFECT_SAVE_FILENAME "data/TEXT/save_ringEffect.txt"  // エフェクトデータを保存する外部ファイルパス名

// 値読み込みをする際の目印となる文字列
// テクスチャ
#define NUM_TEXTURE "NUM_TEXTURE = "
#define TEXTURE_FILENAME "TEXTURE_FILENAME = "

// エミッタデータ
#define NUM_EMITTER       "NUM_EMITTER = "         // エミッタデータの個数
#define EMITTERSET        "EMITTERSET"             // エミッタデータ読み込み開始の合図
#define END_EMITTERSET    "END_EMITTERSET"         // エミッタデータ読み込み終了の合図
#define TYPE              "TYPE = "                // 使用するエフェクトの種類
#define TEX_TYPE          "TEX_TYPE = "            // 使用するテクスチャの番号
#define EFFECT_TYPE       "EFFECT_TYPE = "         // 使用するエフェクトの番号
#define RANGE_MIN         "RANGE_MIN = "           // エフェクトを生成する範囲の最小値
#define RANGE_MAX         "RANGE_MAX = "           // エフェクトを生成する範囲の最大値
#define LIFE              "LIFE = "                // エミッタの寿命
#define LAP               "LAP = "                 // １回の放出で出すエフェクトの個数
#define APPEAR            "APPEAR = "              // エフェクトを出す間隔
#define LOOP              "LOOP = "                // ループするかしないか

// パーティクルデータ
#define NUM_PARTICLE      "NUM_PARTICLE = "        // パーティクルデータの個数
#define PARTICLESET       "PARTICLESET"            // パーティクルデータ読み込み開始の合図
#define END_PARTICLESET   "END_PARTICLESET"        // パーティクルデータ読み込み終了の合図
#define ROT               "ROT = "                 // 向き
#define MAX_MOVE          "MAX_MOVE = "            // 移動量の最大値
#define MIN_MOVE          "MIN_MOVE = "            // 移動量の最小値
#define CHANGE_MOVE       "CHANGE_MOVE = "         // 移動量の変化量
#define INIT_COL          "INIT_COL = "            // 生成時の色
#define CHANGE_COL        "CHANGE_COL = "          // 毎フレームの色の変化量
#define MAX_SIZE          "MAX_SIZE = "            // 大きさの最大値
#define MIN_SIZE          "MIN_SIZE = "            // 大きさの最小値
#define SPREAD            "SPREAD = "              // 大きさの変化量
#define MAX_LIFE          "MAX_LIFE = "            // 寿命の最大値
#define MIN_LIFE          "MIN_LIFE = "            // 寿命の最小値
#define GRAVITY           "GRAVITY = "             // 毎フレームかける重力
#define BOUNCING          "BOUNCING = "            // バウンド量
#define ROT_PATTERN       "ROT_PATTERN = "         // 回転の種類[ 0:時計回り 1:反時計回り 2:ランダム ]
#define ROT_SPEED         "ROT_SPEED = "           // 回転するスピード
#define COLLISION         "COLLISION = "           // 当たり判定するかしないか[ 0:なし 1:あり ]
#define DRAW_ADDTIVE      "DRAW_ADDTIVE = "        // 加算合成するかしないか[ 0:なし 1:あり ]
#define LIGHTING          "LIGHTING = "            // ライティングするかしないか[ 0:なし 1:あり ]

// リングエフェクトデータ
#define NUM_RING          "NUM_RING = "            // リングエフェクトデータの個数
#define RINGSET           "RINGSET"                // リングエフェクトデータ読み込み開始の合図
#define MAX_ROT           "MAX_ROT = "             // 向きの最大値
#define MIN_ROT           "MIN_ROT = "             // 向きの最大値
#define HEIGHT            "HEIGHT = "              // 高さ
#define RADIUS            "RADIUS = "              // 半径
#define DIFFUSION         "DIFFUSION = "           // 幅
#define CHANGE_HEIGHT     "CHANGE_HEIGHT = "       // 高さを毎フレームどれくらい変化させるか
#define CHANGE_RADIUS     "CHANGE_RADIUS = "       // 半径を毎フレームどれくらい変化させるか
#define CHANGE_DIFFUSION  "CHANGE_DIFFUSION = "    // 幅を毎フレームどれくらい変化させるか
#define XBLOCK            "XBLOCK = "              // 横の分割数
#define YBLOCK            "YBLOCK = "              // 縦の分割数
#define CULLING           "CULLING = "             // カリングするかしないか[ 0:なし 1:あり ]
#define END_RINGSET       "END_RINGSET"            // リングエフェクトデータ読み込み終了の合図

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CEffectManager::CEffectManager()
{
	// 各種値のクリア
	strcpy(m_aFileName, "\0");				// 読み込むスクリプトファイル名
	strcpy(m_aTextureListFileName, "\0");	// テクスチャリストファイル名
	m_apEmitterData = NULL;					// エミッタデータクラスへのポインタ
	m_apRingData = NULL;					// リングエフェクトデータクラスへのポインタ
	m_nNumEmitterData = 0;					// エミッタデータの個数
	m_nNumRingEffectData = 0;				// リングエフェクトデータの個数
	m_pTextureManager = NULL;				// テクスチャ管轄クラスへのポインタ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CEffectManager::~CEffectManager()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CEffectManager *CEffectManager::Create(char *pFileName)
{
	CEffectManager *pEffectManager = NULL;   // エフェクト管轄クラス型のポインタ
	if (pEffectManager == NULL)
	{// メモリが空になっている
		pEffectManager = new CEffectManager;
		if (pEffectManager != NULL)
		{// インスタンスを生成できた
			pEffectManager->SetFileName(pFileName);
			if (FAILED(pEffectManager->Init()))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}

	return pEffectManager;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CEffectManager::Init(void)
{
	// スクリプトファイルが読み込めるかチェック
	CFileLoader *pFileLoader = CFileLoader::Create(m_aFileName);
	if (pFileLoader == NULL) { return E_FAIL; }

	// スクリプトファイルを読み込む
	char aStr[256] = "\0";
	strcpy(aStr, pFileLoader->GetString(aStr));
	if (CFunctionLib::Memcmp(aStr, SCRIPT) == 0)
	{// スクリプト読み込み開始の合図だった
		if (FAILED(LoadScript(pFileLoader, aStr)))
		{
			return E_FAIL;
		}
	}

	// メモリの開放
	if (pFileLoader != NULL)
	{
		pFileLoader->Uninit();
		delete pFileLoader;
		pFileLoader = NULL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CEffectManager::Uninit(void)
{
#ifdef EFFECT_AUTO_SAVE
	// エフェクトデータを保存する
	Save();
#endif

	// テクスチャ管轄クラスの開放
	ReleaseTextureManager();

	// エミッタデータクラスの開放
	ReleaseEmitterData();

	// リングエフェクトデータクラスの開放
	ReleaseRingEffectData();
}

//=============================================================================
//    更新処理
//=============================================================================
void CEffectManager::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CEffectManager::Draw(void)
{

}

//=============================================================================
//    エフェクトを設定する処理
//=============================================================================
CEmitter *CEffectManager::SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority)
{
	// エミッタデータクラスからデータを取得
	CEmitter *pEmitter = NULL;
	if (m_apEmitterData[nType] != NULL)
	{// データクラスが作成されている
		int nEffectType = m_apEmitterData[nType]->GetType();
		int nTexIdx = m_apEmitterData[nType]->GetTexIdx();
		int nEffectIdx = m_apEmitterData[nType]->GetEffectIdx();
		int nLife = m_apEmitterData[nType]->GetLife();
		D3DXVECTOR3 RangeMax = m_apEmitterData[nType]->GetRangeMax();
		D3DXVECTOR3 RangeMin = m_apEmitterData[nType]->GetRangeMin();
		int nLap = m_apEmitterData[nType]->GetLap();
		int nAppear = m_apEmitterData[nType]->GetAppear();
		bool bLoop = m_apEmitterData[nType]->GetLoop();

		// エミッタクラスを作成する
		pEmitter = CRingEmitter::Create(pos, rot, nTexIdx, nEffectIdx, RangeMax, RangeMin, nLife, nLap, nAppear, bLoop, m_apRingData[nEffectIdx], m_pTextureManager->GetTexture(nTexIdx), nType, nPriority);
	}

	m_pEmitter = pEmitter;
	return pEmitter;
}

//=============================================================================
//    スクリプトファイルを読み込む
//=============================================================================
HRESULT CEffectManager::LoadScript(CFileLoader *pFileLoader, char *pStr)
{
	int nCntTex = 0;
	int nCntEmitter = 0;
	int nCntParticle = 0;
	int nCntRing = 0;

	while (1)
	{// ループ開始
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_TEXTURE) == 0)
		{// テクスチャリストファイル名だった
			CreateTextureMemory(CFunctionLib::ReadInt(pStr, NUM_TEXTURE));
		}
		else if (CFunctionLib::Memcmp(pStr, TEXTURE_FILENAME) == 0)
		{// テクスチャリストファイル名だった
			LoadTexture(CFunctionLib::ReadString(pStr, pStr, TEXTURE_FILENAME), nCntTex);
			nCntTex++;
		}
		else if (CFunctionLib::Memcmp(pStr, NUM_EMITTER) == 0)
		{// エミッタデータの個数情報がある
			CreateEmitterDataMemory(CFunctionLib::ReadInt(pStr, NUM_EMITTER));
		}
		else if (CFunctionLib::Memcmp(pStr, EMITTERSET) == 0)
		{// エミッタデータ読み込み開始の合図だった
			m_apEmitterData[nCntEmitter] = LoadEmitterData(pFileLoader, pStr, nCntEmitter);
			nCntEmitter++;
		}
		else if (CFunctionLib::Memcmp(pStr, NUM_RING) == 0)
		{// リングエフェクトデータの個数情報がある
			CreateRingEffectDataMemory(CFunctionLib::ReadInt(pStr, NUM_RING));
		}
		else if (CFunctionLib::Memcmp(pStr, RINGSET) == 0)
		{// リングエフェクトデータ読み込み開始の合図だった
			m_apRingData[nCntRing] = LoadRingEffectData(pFileLoader, pStr, nCntRing);
			nCntRing++;
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// スクリプト読み込み終了の合図だった
			break;  // ループ終了
		}
	}

	return S_OK;
}

//=============================================================================
//    テクスチャを読み込む処理
//=============================================================================
HRESULT CEffectManager::LoadTexture(char *pTextureFileName, int nCntTex)
{
	if (m_pTextureManager == NULL) { return E_FAIL; }

	// テクスチャの読み込み
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	if (FAILED(D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), pTextureFileName, &pTexture)))
	{
		return E_FAIL;
	}

	// テクスチャ管轄クラスにセット
	m_pTextureManager->BindTexture(pTexture, nCntTex);

	return S_OK;
}

//=============================================================================
//    エミッタデータを読み込む処理
//=============================================================================
CEmitterData *CEffectManager::LoadEmitterData(CFileLoader *pFileLoader, char *pStr, const int nCntEmitter)
{
	// エミッタデータ用のメモリを確保する
	CEmitterData *pEmitterData = NULL;  // エミッタデータクラスへのポインタ
	pEmitterData = new CEmitterData;
	if (pEmitterData == NULL) { return NULL; }

	while (1)
	{// ループ開始
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TYPE) == 0)
		{// エフェクトの種類情報がある
			pEmitterData->SetType(CFunctionLib::ReadInt(pStr, TYPE));
		}
		else if (CFunctionLib::Memcmp(pStr, TEX_TYPE) == 0)
		{// テクスチャ番号情報がある
			pEmitterData->SetTexIdx(CFunctionLib::ReadInt(pStr, TEX_TYPE));
		}
		else if (CFunctionLib::Memcmp(pStr, EFFECT_TYPE) == 0)
		{// 種類情報がある
			pEmitterData->SetEffectIdx(CFunctionLib::ReadInt(pStr, EFFECT_TYPE));
		}
		else if (CFunctionLib::Memcmp(pStr, RANGE_MIN) == 0)
		{// エフェクトを生成する範囲の最小値情報がある
			pEmitterData->SetRangeMin(CFunctionLib::ReadVector3(pStr, RANGE_MIN));
		}
		else if (CFunctionLib::Memcmp(pStr, RANGE_MAX) == 0)
		{// エフェクトを生成する範囲の最大値情報がある
			pEmitterData->SetRangeMax(CFunctionLib::ReadVector3(pStr, RANGE_MAX));
		}
		else if (CFunctionLib::Memcmp(pStr, LIFE) == 0)
		{// 寿命情報がある
			pEmitterData->SetLife(CFunctionLib::ReadInt(pStr, LIFE));
		}
		else if (CFunctionLib::Memcmp(pStr, LAP) == 0)
		{// 1回の放出で生成する個数情報がある
			pEmitterData->SetLap(CFunctionLib::ReadInt(pStr, LAP));
		}
		else if (CFunctionLib::Memcmp(pStr, APPEAR) == 0)
		{// 放出する間隔情報がある
			pEmitterData->SetAppear(CFunctionLib::ReadInt(pStr, APPEAR));
		}
		else if (CFunctionLib::Memcmp(pStr, LOOP) == 0)
		{// ループするかしないか情報がある
			pEmitterData->SetLoop(CFunctionLib::ReadBool(pStr, LOOP));
		}
		else if (CFunctionLib::Memcmp(pStr, END_EMITTERSET) == 0)
		{// エミッタデータ読み込み終了の合図だった
			break;  // ループ終了
		}
	}

	return pEmitterData;
}

//=============================================================================
//    リングエフェクトデータを読み込む処理
//=============================================================================
CRingData *CEffectManager::LoadRingEffectData(CFileLoader *pFileLoader, char *pStr, const int nCntRingEffectData)
{
	// リングエフェクトデータ用のメモリを確保する
	CRingData *pRingData = NULL;
	pRingData = new CRingData;
	if (pRingData == NULL) { return NULL; }

	while (1)
	{// ループ開始
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, MAX_ROT) == 0)
		{// 向きの最大値情報がある
			pRingData->SetMaxRot(CFunctionLib::ReadVector3(pStr, MAX_ROT));
		}
		else if (CFunctionLib::Memcmp(pStr, MIN_ROT) == 0)
		{// 向きの最小値情報がある
			pRingData->SetMinRot(CFunctionLib::ReadVector3(pStr, MIN_ROT));
		}
		else if (CFunctionLib::Memcmp(pStr, MAX_MOVE) == 0)
		{// 移動量の最大値情報がある
			pRingData->SetMaxMove(CFunctionLib::ReadVector3(pStr, MAX_MOVE));
		}
		else if (CFunctionLib::Memcmp(pStr, MIN_MOVE) == 0)
		{// 移動量の最小値情報がある
			pRingData->SetMinMove(CFunctionLib::ReadVector3(pStr, MIN_MOVE));
		}
		else if (CFunctionLib::Memcmp(pStr, CHANGE_MOVE) == 0)
		{// 移動量の変化量情報がある
			pRingData->SetChangeMove(CFunctionLib::ReadVector3(pStr, CHANGE_MOVE));
		}
		else if (CFunctionLib::Memcmp(pStr, INIT_COL) == 0)
		{// 生成時の色情報がある
			pRingData->SetInitCol(CFunctionLib::ReadVector4(pStr, INIT_COL));
		}
		else if (CFunctionLib::Memcmp(pStr, CHANGE_COL) == 0)
		{// フレーム毎の色の変化量情報がある
			pRingData->SetChangeCol(CFunctionLib::ReadVector4(pStr, CHANGE_COL));
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// 高さ情報がある
			pRingData->SetHeight(CFunctionLib::ReadFloat(pStr, HEIGHT));
		}
		else if (CFunctionLib::Memcmp(pStr, RADIUS) == 0)
		{// 半径情報がある
			pRingData->SetRadius(CFunctionLib::ReadFloat(pStr, RADIUS));
		}
		else if (CFunctionLib::Memcmp(pStr, DIFFUSION) == 0)
		{// 幅情報がある
			pRingData->SetDiffusion(CFunctionLib::ReadFloat(pStr, DIFFUSION));
		}
		else if (CFunctionLib::Memcmp(pStr, CHANGE_HEIGHT) == 0)
		{// 高さを毎フレームどれくらい変化させる情報がある
			pRingData->SetChangeHeight(CFunctionLib::ReadFloat(pStr, CHANGE_HEIGHT));
		}
		else if (CFunctionLib::Memcmp(pStr, CHANGE_RADIUS) == 0)
		{// 半径を毎フレームどれくらい変化させる情報がある
			pRingData->SetChangeRadius(CFunctionLib::ReadFloat(pStr, CHANGE_RADIUS));
		}
		else if (CFunctionLib::Memcmp(pStr, CHANGE_DIFFUSION) == 0)
		{// 幅を毎フレームどれくらい変化させる情報がある
			pRingData->SetChangeDiffusion(CFunctionLib::ReadFloat(pStr, CHANGE_DIFFUSION));
		}
		else if (CFunctionLib::Memcmp(pStr, MAX_LIFE) == 0)
		{// 寿命の最大値情報がある
			pRingData->SetMaxLife(CFunctionLib::ReadInt(pStr, MAX_LIFE));
		}
		else if (CFunctionLib::Memcmp(pStr, MIN_LIFE) == 0)
		{// 寿命の最小値情報がある
			pRingData->SetMinLife(CFunctionLib::ReadInt(pStr, MIN_LIFE));
		}
		else if (CFunctionLib::Memcmp(pStr, XBLOCK) == 0)
		{// 横の分割数情報がある
			pRingData->SetXBlock(CFunctionLib::ReadInt(pStr, XBLOCK));
		}
		else if (CFunctionLib::Memcmp(pStr, YBLOCK) == 0)
		{// 縦の分割数情報がある
			pRingData->SetYBlock(CFunctionLib::ReadInt(pStr, YBLOCK));
		}
		else if (CFunctionLib::Memcmp(pStr, ROT_PATTERN) == 0)
		{// 回転の種類情報がある
			pRingData->SetRotPattern(CFunctionLib::ReadInt(pStr, ROT_PATTERN));
		}
		else if (CFunctionLib::Memcmp(pStr, ROT_SPEED) == 0)
		{// 回転のスピード情報がある
			pRingData->SetRotSpeed(CFunctionLib::ReadVector3(pStr, ROT_SPEED));
		}
		else if (CFunctionLib::Memcmp(pStr, CULLING) == 0)
		{// カリングをするかしないか情報がある
			pRingData->SetCulling(CFunctionLib::ReadBool(pStr, CULLING));
		}
		else if (CFunctionLib::Memcmp(pStr, DRAW_ADDTIVE) == 0)
		{// 加算合成するかしないか情報がある
			pRingData->SetDrawAddtive(CFunctionLib::ReadBool(pStr, DRAW_ADDTIVE));
		}
		else if (CFunctionLib::Memcmp(pStr, END_RINGSET) == 0)
		{// リングエフェクトデータ読み込み終了の合図だった
			break;  // ループ終了
		}
	}

	return pRingData;
}

//=============================================================================
//    エフェクトデータを保存する処理
//=============================================================================
void CEffectManager::Save(void)
{
	CFileSaver *pFileSaver = CFileSaver::Create(EFFECT_SAVE_FILENAME);
	if (pFileSaver != NULL)
	{// ファイルポインタが確保できた
	    // ファイルの冒頭部分を作成
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("# エフェクトデータ [%s]\n", EFFECT_SAVE_FILENAME);
		pFileSaver->Print("# Author : HODAKA NIWA\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("%s			# この行は絶対に消さないこと！\n\n", SCRIPT);

		// テクスチャデータの保存
		SaveTextureData(pFileSaver);

		// エミッタデータの保存
		SaveEmitterData(pFileSaver);

		// リングエフェクトデータの保存
		SaveRingEffectData(pFileSaver);

		// コメント部分を作成
		pFileSaver->Print("%s			# この行は絶対に消さないこと！\n\n", END_SCRIPT);

		// メモリの開放
		if (pFileSaver != NULL)
		{
			pFileSaver->Uninit();
			delete pFileSaver;
			pFileSaver = NULL;
		}
	}
}

//=============================================================================
//    テクスチャリストファイル名を保存する処理
//=============================================================================
void CEffectManager::SaveTextureData(CFileSaver *pFileSaver)
{
	// コメント部分を作成
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  使用するテクスチャの数\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// 使用するテクスチャの数を保存
	pFileSaver->Print("%s%d\n\n", NUM_TEXTURE, m_aTextureListFileName);

	// コメント部分を作成
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  使用するテクスチャのファイル名\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// ファイル名を保存
	for (int nCntTex = 0; nCntTex < m_pTextureManager->GetNumTexture(); nCntTex++)
	{
		pFileSaver->Print("%s%s\n", TEXTURE_FILENAME, m_pTextureManager->GetFileName(nCntTex));
	}
	pFileSaver->Print("\n");
}

//=============================================================================
//    エミッタデータを保存する処理
//=============================================================================
void CEffectManager::SaveEmitterData(CFileSaver *pFileSaver)
{
	// コメント部分を作成
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  エミッタ数\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// エミッタ数を保存
	pFileSaver->Print("%s%d\n\n", NUM_EMITTER, m_nNumEmitterData);

	// コメント部分を作成
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  エミッタのデータ\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// エミッタのデータを作成した数だけ保存
	if (m_apEmitterData == NULL) { return; }
	for (int nCntEmitter = 0; nCntEmitter < m_nNumEmitterData; nCntEmitter++)
	{// 作成したエミッタの数だけ繰り返し
		if (m_apEmitterData[nCntEmitter] != NULL)
		{// エミッタデータが作成されている
			D3DXVECTOR3 RangeMin = m_apEmitterData[nCntEmitter]->GetRangeMin();
			D3DXVECTOR3 RangeMax = m_apEmitterData[nCntEmitter]->GetRangeMax();

			pFileSaver->Print("%s\n", EMITTERSET);
			pFileSaver->Print("	%s%d					# 使用するエフェクトの種類\n", TYPE, m_apEmitterData[nCntEmitter]->GetType());
			pFileSaver->Print("	%s%d				# テクスチャの番号\n", TEX_TYPE, m_apEmitterData[nCntEmitter]->GetTexIdx());
			pFileSaver->Print("	%s%d				# 使用するエフェクトの番号\n", EFFECT_TYPE, m_apEmitterData[nCntEmitter]->GetEffectIdx());
			pFileSaver->Print("	%s%.1f %.1f %.1f		# エフェクトを放出する範囲の最小値\n", RANGE_MIN, RangeMin.x, RangeMin.y, RangeMin.z);
			pFileSaver->Print("	%s%.1f %.1f %.1f		# エフェクトを放出する範囲の最大値\n", RANGE_MAX, RangeMax.x, RangeMax.y, RangeMax.z);
			pFileSaver->Print("	%s%d					# エミッタの寿命\n", LIFE, m_apEmitterData[nCntEmitter]->GetLife());
			pFileSaver->Print("	%s%d						# １回の放出で出すエフェクトの個数\n", LAP, m_apEmitterData[nCntEmitter]->GetLap());
			pFileSaver->Print("	%s%d					# エフェクトを出す間隔\n", APPEAR, m_apEmitterData[nCntEmitter]->GetAppear());
			pFileSaver->Print("	%s%d					# ループするかしないか[ 0:なし 1:あり ]\n", LOOP, (int)m_apEmitterData[nCntEmitter]->GetLoop());
			pFileSaver->Print("%s\n\n", END_EMITTERSET);
		}
	}
}

//=============================================================================
//    リングエフェクトデータを保存する処理
//=============================================================================
void CEffectManager::SaveRingEffectData(CFileSaver *pFileSaver)
{
	// コメント部分を作成
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  リングエフェクトデータの数\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// リングエフェクトデータ数を保存
	pFileSaver->Print("%s%d\n\n", NUM_RING, m_nNumRingEffectData);

	// コメント部分を作成
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  リングエフェクトのデータ\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// リングエフェクトのデータを作成した数だけ保存
	if (m_apRingData == NULL) { return; }
	for (int nCntRing = 0; nCntRing < m_nNumRingEffectData; nCntRing++)
	{// 作成したリングエフェクトのデータの数だけ繰り返し
		pFileSaver->Print("%s\n", RINGSET);
		pFileSaver->Print("	%s%.4f %.4f %.4f			# 向きの最大値\n", MAX_ROT, m_apRingData[nCntRing]->GetMaxRot().x, m_apRingData[nCntRing]->GetMaxRot().y, m_apRingData[nCntRing]->GetMaxRot().z);
		pFileSaver->Print("	%s%.4f %.4f %.4f			# 向きの最小値\n", MIN_ROT, m_apRingData[nCntRing]->GetMinRot().x, m_apRingData[nCntRing]->GetMinRot().y, m_apRingData[nCntRing]->GetMinRot().z);
		pFileSaver->Print("	%s%.4f %.4f %.4f			# 移動量の最大値\n", MAX_MOVE, m_apRingData[nCntRing]->GetMaxMove().x, m_apRingData[nCntRing]->GetMaxMove().y, m_apRingData[nCntRing]->GetMaxMove().z);
		pFileSaver->Print("	%s%.4f %.4f %.4f				# 移動量の最小値\n", MIN_MOVE, m_apRingData[nCntRing]->GetMinMove().x, m_apRingData[nCntRing]->GetMinMove().y, m_apRingData[nCntRing]->GetMinMove().z);
		pFileSaver->Print("	%s%.4f %.4f %.4f			# 移動量の変化量\n", CHANGE_MOVE, m_apRingData[nCntRing]->GetChangeMove().x, m_apRingData[nCntRing]->GetChangeMove().y, m_apRingData[nCntRing]->GetChangeMove().z);
		pFileSaver->Print("	%s%.4f %.4f %.4f %.4f			# 生成時の色\n", INIT_COL, m_apRingData[nCntRing]->GetInitCol().r, m_apRingData[nCntRing]->GetInitCol().g, m_apRingData[nCntRing]->GetInitCol().b, m_apRingData[nCntRing]->GetInitCol().a);
		pFileSaver->Print("	%s%.4f %.4f %.4f %.4f		# フレーム毎の色の変化量\n", CHANGE_COL, m_apRingData[nCntRing]->GetChangeCol().r, m_apRingData[nCntRing]->GetChangeCol().g, m_apRingData[nCntRing]->GetChangeCol().b, m_apRingData[nCntRing]->GetChangeCol().a);
		pFileSaver->Print("	%s%f						# 高さ\n", HEIGHT, m_apRingData[nCntRing]->GetHeight());
		pFileSaver->Print("	%s%f						# 半径\n", RADIUS, m_apRingData[nCntRing]->GetRadius());
		pFileSaver->Print("	%s%f						# 幅\n", DIFFUSION, m_apRingData[nCntRing]->GetDiffusion());
		pFileSaver->Print("	%s%f						# 高さを毎フレームどれくらい変化させる\n", CHANGE_HEIGHT, m_apRingData[nCntRing]->GetChangeHeight());
		pFileSaver->Print("	%s%f						# 半径を毎フレームどれくらい変化させる\n", CHANGE_RADIUS, m_apRingData[nCntRing]->GetChangeRadius());
		pFileSaver->Print("	%s%f						# 幅を毎フレームどれくらい変化させる\n", CHANGE_DIFFUSION, m_apRingData[nCntRing]->GetChangeDiffusion());
		pFileSaver->Print("	%s%d						# 寿命の最大値\n", MAX_LIFE, m_apRingData[nCntRing]->GetMaxLife());
		pFileSaver->Print("	%s%d						# 寿命の最小値\n", MIN_LIFE, m_apRingData[nCntRing]->GetMinLife());
		pFileSaver->Print("	%s%d						# 横の分割数\n", XBLOCK, m_apRingData[nCntRing]->GetXBlock());
		pFileSaver->Print("	%s%d						# 縦の分割数\n", YBLOCK, m_apRingData[nCntRing]->GetYBlock());
		pFileSaver->Print("	%s%d						# 回転のパターン[ 0:時計回り 1:反時計回り 2:ランダム ]\n", ROT_PATTERN, m_apRingData[nCntRing]->GetRotPattern());
		pFileSaver->Print("	%s%.4f %.4f %.4f			# 回転のスピード\n", ROT_SPEED, m_apRingData[nCntRing]->GetRotSpeed().x, m_apRingData[nCntRing]->GetRotSpeed().y, m_apRingData[nCntRing]->GetRotSpeed().z);
		pFileSaver->Print("	%s%d						# カリングするかしないか[ 0:なし 1:あり ]\n", CULLING, (int)m_apRingData[nCntRing]->GetCulling());
		pFileSaver->Print("	%s%d					# 加算合成するかしないか[ 0:なし 1:あり ]\n", DRAW_ADDTIVE, (int)m_apRingData[nCntRing]->GetDrawAddtive());
		pFileSaver->Print("%s\n\n", END_RINGSET);
	}
}

//=============================================================================
//    テクスチャ用のメモリを生成する処理
//=============================================================================
void CEffectManager::CreateTextureMemory(const int nNumTexture)
{
	if (nNumTexture <= 0 || m_pTextureManager != NULL) { return; }
	m_pTextureManager = CTextureManager::Create(nNumTexture);
}

//=============================================================================
//    エミッタデータ用のメモリを生成する処理
//=============================================================================
void CEffectManager::CreateEmitterDataMemory(const int nNumEmitterData)
{
	// 生成数を保存
	m_nNumEmitterData = nNumEmitterData;

	// メモリを確保する
	if (nNumEmitterData == 0 || m_apEmitterData != NULL) { return; }
	m_apEmitterData = new CEmitterData*[nNumEmitterData];

	// 確保したメモリを初期化しておく
	if (m_apEmitterData == NULL) { return; }
	for (int nCntEmitter = 0; nCntEmitter < nNumEmitterData; nCntEmitter++)
	{// エミッタデータ数だけ繰り返し
		m_apEmitterData[nCntEmitter] = NULL;
	}
}

//=============================================================================
//    リングエフェクトデータ用のメモリを生成する処理
//=============================================================================
void CEffectManager::CreateRingEffectDataMemory(const int nNumRingEffectData)
{
	// 生成数を保存
	m_nNumRingEffectData = nNumRingEffectData;

	// メモリを確保する
	if (nNumRingEffectData == 0 || m_apRingData != NULL) { return; }
	m_apRingData = new CRingData*[nNumRingEffectData];

	// 確保したメモリを初期化しておく
	if (m_apRingData == NULL) { return; }
	for (int nCntRingData = 0; nCntRingData < nNumRingEffectData; nCntRingData++)
	{// リングエフェクトデータ数だけ繰り返し
		m_apRingData[nCntRingData] = NULL;
	}
}

//=============================================================================
//    テクスチャ管轄クラスを開放する処理
//=============================================================================
void CEffectManager::ReleaseTextureManager(void)
{
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}
}

//=============================================================================
//    エミッタデータを開放する処理
//=============================================================================
void CEffectManager::ReleaseEmitterData(void)
{
	// メモリが確保されていなければ処理しない
	if (m_apEmitterData == NULL) { return; }

	for (int nCntEmitter = 0; nCntEmitter < m_nNumEmitterData; nCntEmitter++)
	{// エミッタデータ数だけ繰り返し
		if (m_apEmitterData[nCntEmitter] != NULL)
		{// メモリが確保されている
			delete m_apEmitterData[nCntEmitter];
			m_apEmitterData[nCntEmitter] = NULL;
		}
	}
	delete[] m_apEmitterData;
	m_apEmitterData = NULL;
}

//=============================================================================
//    リングエフェクトデータを開放する処理
//=============================================================================
void CEffectManager::ReleaseRingEffectData(void)
{
	// メモリが確保されていなければ処理しない
	if (m_apRingData == NULL) { return; }

	for (int nCntRingData = 0; nCntRingData < m_nNumRingEffectData; nCntRingData++)
	{// リングエフェクトデータ数だけ繰り返し
		if (m_apRingData[nCntRingData] != NULL)
		{// メモリが確保されている
			delete m_apRingData[nCntRingData];
			m_apRingData[nCntRingData] = NULL;
		}
	}
	delete[] m_apRingData;
	m_apRingData = NULL;
}

//=============================================================================
//    スクリプトファイル名を設定する処理
//=============================================================================
void CEffectManager::SetFileName(char *pFileName)
{
	strcpy(m_aFileName, pFileName);
}

//=============================================================================
//    テクスチャリストファイル名を設定する処理
//=============================================================================
void CEffectManager::SetTextureListFileName(char *pFileName)
{
	strcpy(m_aTextureListFileName, pFileName);
}

//=============================================================================
//    最後に生成したエミッタクラスへのポインタを設定する処理
//=============================================================================
void CEffectManager::SetEmitter(CEmitter *pEmitter)
{
	m_pEmitter = pEmitter;
}

//=============================================================================
//    エミッタデータを設定する処理
//=============================================================================
void CEffectManager::SetEmitterData(CEmitterData *pEmitterData, const int nIdx)
{
	m_apEmitterData[nIdx] = pEmitterData;
}

//=============================================================================
//    リングエフェクトデータを設定する処理
//=============================================================================
void CEffectManager::SetRingEffectData(CRingData *pRingData, const int nIdx)
{
	m_apRingData[nIdx] = pRingData;
}

//=============================================================================
//    エミッタデータの数を設定する処理
//=============================================================================
void CEffectManager::SetNumEmitterData(const int nNumEmitterData)
{
	m_nNumEmitterData = nNumEmitterData;
}

//=============================================================================
//    リングエフェクトデータの数を設定する処理
//=============================================================================
void CEffectManager::SetNumRingEffectData(const int nNumRingEffectData)
{
	m_nNumRingEffectData = nNumRingEffectData;
}

//=============================================================================
//    スクリプトファイル名を取得する処理
//=============================================================================
char *CEffectManager::GetFileName(void)
{
	return m_aFileName;
}

//=============================================================================
//    テクスチャリストファイル名を取得する処理
//=============================================================================
char *CEffectManager::GetTextureListFileName(void)
{
	return m_aTextureListFileName;
}

//=============================================================================
//    最後に生成したエミッタクラスへのポインタを取得する処理
//=============================================================================
CEmitter *CEffectManager::GetEmitter(void)
{
	return m_pEmitter;
}

//=============================================================================
//    エミッタデータを取得する処理
//=============================================================================
CEmitterData **CEffectManager::GetEmitterData(void)
{
	return m_apEmitterData;
}
CEmitterData *CEffectManager::GetEmitterData(const int nIdx)
{
	return m_apEmitterData[nIdx];
}

//=============================================================================
//    リングエフェクトデータを取得する処理
//=============================================================================
CRingData **CEffectManager::GetRingEffectData(void)
{
	return m_apRingData;
}
CRingData *CEffectManager::GetRingEffectData(const int nIdx)
{
	return m_apRingData[nIdx];
}

//=============================================================================
//    エミッタデータの数を取得する処理
//=============================================================================
int CEffectManager::GetNumEmitterData(void)
{
	return m_nNumEmitterData;
}

//=============================================================================
//    リングエフェクトデータの数を取得する処理
//=============================================================================
int CEffectManager::GetNumRingEffectData(void)
{
	return m_nNumRingEffectData;
}