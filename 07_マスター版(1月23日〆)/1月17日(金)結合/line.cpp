//=============================================================================
//
// ビルボード処理 [scenebillbord.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//===========================================================================
#include "line.h"
#include "manager.h"
#include "renderer.h"
#include "Player.h"
#include "library.h"
#include "collision.h"
#include "tutorial.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define LINE_LIGHTING
#define LINE_HIGHT			(15.0f)
#define WIDTH				(15.0f)
#define COLLISION_WIDTH		(30.0f)
#define UPDATE_LENGTH		(1300.0f)

//=============================================================================
// 生成処理
//=============================================================================
CLine * CLine::Create(D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos, int nNumPlayer, int nId)
{
	//インスタンスの生成
	CLine * pLine;
	pLine = new CLine;

	//初期化処理
	pLine->SetNumPlayer(nNumPlayer);
	pLine->SetTerritoryPos(StartPos, EndPos);
	pLine->SetID(nId);
	pLine->Init();

	return pLine;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CLine::CLine() : CScene3D(3, OBJTYPE_LINE)
{
	m_nNumPlayer = 0;
	m_nID = 0;
	m_StartPos = INITIALIZE_VECTOR3;
	m_EndPos = INITIALIZE_VECTOR3;
	m_col = INITIALIZE_VECTOR4;
	m_bCompleteShape = false;

	for (int nCnt = 0; nCnt < MAX_POS; nCnt++)
	{
		m_VecA[nCnt] = INITIALIZE_VECTOR3;
		m_WorldPos[nCnt] = INITIALIZE_VECTOR3;
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CLine::~CLine(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLine::Init(void)
{
	CScene3D::Init();

	//変数の初期化
	m_bCompleteShape = false;

	int nNumPlayer = m_nNumPlayer;
	m_col = CPlayer::m_CountryColor[nNumPlayer];

	//色の設定
	CScene3D::SetColor(m_col);

	//中心点を求める
	D3DXVECTOR3 Pos = (m_StartPos + m_EndPos) / 2.0f;
	Pos.y = LINE_HIGHT;

	//サイズを求める
	D3DXVECTOR3 Size = INITIALIZE_VECTOR3;
	Size.z = (CFunctionLib::Vec2Length(m_StartPos.x, m_StartPos.z, m_EndPos.x, m_EndPos.z)) / 2.0f;
	Size.x = WIDTH;

	//位置、サイズを設定する
	CScene3D::Set(Pos, Size);

	//向きの設定
	float fRot = CFunctionLib::CalcAngleToDest(m_StartPos.x, m_StartPos.z, m_EndPos.x, m_EndPos.z);
	CScene3D::SetRot(D3DXVECTOR3(0.0f, fRot, 0.0f));

	//VecAを求める
	RequestVecA();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLine::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CLine::Update(void)
{
	if (m_bCompleteShape) { return; }	//図形が完成しているかどうか

	CManager::MODE mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{
		CGame *pGame = CManager::GetGame();

		int nNumPlayer = pGame->GetNumPlay();
		nNumPlayer += pGame->GetEnemyNum();

		for (int nCntPlayer = 0; nCntPlayer < nNumPlayer; nCntPlayer++)
		{
			if (nCntPlayer == m_nNumPlayer) { continue; }
			D3DXVECTOR3 Pos = CScene3D::GetPos();
			CCharacter * pOtherPlayer = CManager::GetGame()->GetChara(nCntPlayer);
			D3DXVECTOR3 PlayerPos = pOtherPlayer->GetPos();

			float fLength = CFunctionLib::Vec2Length(Pos.x, Pos.z, PlayerPos.x, PlayerPos.z);	//距離を求める

			if (fLength > UPDATE_LENGTH) { continue; }
			bool bCutLine = CollsionLine(PlayerPos);	//当たり判定をする

			if (!bCutLine) { continue; }
			CManager::GetGame()->GetChara(m_nNumPlayer)->CutLine(m_nID);	//ライン切断処理	
		}
	}
	else if(mode == CManager::MODE_TUTORIAL)
	{
		if (0 == m_nNumPlayer) { return; }
		//ラインの切断処理 
		D3DXVECTOR3 Pos = CScene3D::GetPos();
		D3DXVECTOR3 PlayerPos = CManager::GetTutorial()->GetTutorialPlayer()->GetPos();

		float fLength = CFunctionLib::Vec2Length(Pos.x, Pos.z, PlayerPos.x, PlayerPos.z);	//距離を求める

		if (fLength < UPDATE_LENGTH)
		{
			bool bCutLine = CollsionLine(PlayerPos);	//当たり判定をする
			
			//ラインの破棄
			if (bCutLine) { Uninit(); }
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CLine::Draw(void)
{
	
#ifdef LINE_LIGHTING
	// レンダリングクラスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer == NULL) { return; }

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == NULL) { return; }

	// ライティングをしない設定に
	DWORD Lighting;
	pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zバッファに書き込みしない設定に
	DWORD Zwrite;
	pDevice->GetRenderState(D3DRS_ZWRITEENABLE, &Zwrite);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// 共通の描画処理
	CScene3D::Draw();

	// ライティングの設定を戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);

	// Zバッファに書き込む設定に
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, Zwrite);
#else
	// 共通の描画処理
	CScene3D::Draw();
#endif
}

//=============================================================================
// 線との当たり判定
//=============================================================================
bool CLine::CollsionLine(D3DXVECTOR3 PlayerPos)
{
	//変数宣言
	bool bIn = false;
	int nCountEnter = 0;

	for (int nCnt = 0; nCnt < MAX_POS; nCnt++)
	{
		D3DXVECTOR3 VecB = PlayerPos - m_WorldPos[nCnt];

		//外積を求める
		float fCross = CFunctionLib::Vec2Cross(&D3DXVECTOR2(m_VecA[nCnt].x, m_VecA[nCnt].z), &D3DXVECTOR2(VecB.x, VecB.z));

		//ベクトルより大きいかどうか
		if (fCross >= 0) 
		{ 
			nCountEnter++; 
		}
		else
		{
			break;
		}
	}

	//入っているか判定する
	if (nCountEnter >= MAX_POS)
	{ 
		bIn = true; 
	}

	return bIn;
}

//=============================================================================
// 色の変更処理
//=============================================================================
bool CLine::ColChange(void)
{
	//変数宣言
	bool bDeth = false;	//死亡フラグ

	m_col.a -= 0.05f;

	if (m_col.a < 0.0f)
	{
		m_col.a = 0.0f;
		bDeth = true;	//死亡フラグを有効にする
	}

	//色を設定
	SetColor(m_col);
	return bDeth;
}


//=============================================================================
// VecAを求める
//=============================================================================
void CLine::RequestVecA(void)
{
	//変数宣言
	D3DXVECTOR3 PrimitivPos[MAX_POS] = {};

	//----------------------------
	//	頂点の位置を取得
	//----------------------------
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CScene3D::GetVtxBuff();

	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	PrimitivPos[0] = pVtx[0].pos;
	PrimitivPos[1] = pVtx[1].pos;
	PrimitivPos[2] = pVtx[2].pos;
	PrimitivPos[3] = pVtx[3].pos;

	//頂点バッファをアンロックする
	pVtxBuff->Unlock();

	//----------------------------
	//	ワールド座標を求める
	//----------------------------
	D3DXMATRIX	mtxLineWorld, mtxRot, mtxTrans;			// 計算用マトリックス
	D3DXVECTOR3 Rot = CScene3D::GetRot();			//向きの取得
	D3DXVECTOR3 Pos = CScene3D::GetPos();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxLineWorld);
	RequestWorldMatrix(Pos, Rot, &mtxLineWorld);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxLineWorld);

	// 計算用マトリックス
	D3DXMATRIX	mtxPriWorld, mtxPriRot, mtxPriTrans;			

	for (int nCnt = 0; nCnt < MAX_POS; nCnt++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxPriWorld);
		RequestWorldMatrix(PrimitivPos[nCnt], INITIALIZE_VECTOR3, &mtxPriWorld);

		// ワールドマトリックスの設定
		D3DXMatrixMultiply(&mtxPriWorld, &mtxPriWorld, &mtxLineWorld);

		//ワールド座標の保存
		m_WorldPos[nCnt] = D3DXVECTOR3(mtxPriWorld._41, mtxPriWorld._42, mtxPriWorld._43);
	}

	//----------------------------
	//	数字を入れ替える
	//----------------------------
	D3DXVECTOR3 WorldPos = m_WorldPos[3];
	m_WorldPos[3] = m_WorldPos[1];
	m_WorldPos[1] = m_WorldPos[2];
	m_WorldPos[2] = Pos;

	//----------------------------
	//	VecAを求める
	//----------------------------
	for (int nCntVec = 0; nCntVec < MAX_POS; nCntVec++)
	{
		int nCnt = nCntVec + 1;
		if ((nCnt % MAX_POS) == 0) { nCnt = 0; }

		m_VecA[nCntVec] = m_WorldPos[nCnt] - m_WorldPos[nCntVec];
	}
}

//=============================================================================
// ワールドマトリックスの計算処理
//=============================================================================
void CLine::RequestWorldMatrix(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXMATRIX * mtxWorld)
{
	D3DXMATRIX mtxRot, mtxParent; // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(mtxWorld);

	// 回転行列を作成(D3DXMatrixRotationYawPitchRoll参照)
	float fSinPitch = sinf(rot.x);
	float fCosPitch = cosf(rot.x);
	float fSinYaw = sinf(rot.y);
	float fCosYaw = cosf(rot.y);
	float fSinRoll = sinf(rot.z);
	float fCosRoll = cosf(rot.z);
	mtxRot._11 = fSinRoll * fSinPitch * fSinYaw + fCosRoll * fCosYaw;
	mtxRot._12 = fSinRoll * fCosPitch;
	mtxRot._13 = fSinRoll * fSinPitch * fCosYaw - fCosRoll * fSinYaw;
	mtxRot._21 = fCosRoll * fSinPitch * fSinYaw - fSinRoll * fCosYaw;
	mtxRot._22 = fCosRoll * fCosPitch;
	mtxRot._23 = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fSinYaw;
	mtxRot._31 = fCosPitch * fSinYaw;
	mtxRot._32 = -fSinPitch;
	mtxRot._33 = fCosPitch * fCosYaw;

	// 回転を反映する
	mtxWorld->_11 = mtxRot._11;
	mtxWorld->_12 = mtxRot._12;
	mtxWorld->_13 = mtxRot._13;
	mtxWorld->_21 = mtxRot._21;
	mtxWorld->_22 = mtxRot._22;
	mtxWorld->_23 = mtxRot._23;
	mtxWorld->_31 = mtxRot._31;
	mtxWorld->_32 = mtxRot._32;
	mtxWorld->_33 = mtxRot._33;

	// オフセット位置を反映
	mtxWorld->_41 = pos.x;
	mtxWorld->_42 = pos.y;
	mtxWorld->_43 = pos.z;
}