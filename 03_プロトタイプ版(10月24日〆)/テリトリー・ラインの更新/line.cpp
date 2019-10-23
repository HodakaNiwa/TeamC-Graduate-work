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
#include "library.h"

//=============================================================================
// マクロ定義
//=============================================================================
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
CLine::CLine() : CScene3D(3, OBJTYPE_LINE){}

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

	//色の設定
	CPlayer::TYPE PlayerType = CManager::GetGame()->GetPlayer(m_nNumPlayer)->GetType();
	CScene3D::SetColor(CPlayer::m_CountryColor[(int)PlayerType]);

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
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUMPLAYER; nCntPlayer++)
	{
		if (nCntPlayer == m_nNumPlayer) { continue; }
		D3DXVECTOR3 Pos = CScene3D::GetPos();
		CPlayer * pOtherPlayer = CManager::GetGame()->GetPlayer(nCntPlayer);
		D3DXVECTOR3 PlayerPos = pOtherPlayer->GetPos();

		float fLength = CFunctionLib::Vec2Length(Pos.x, Pos.z, PlayerPos.x, PlayerPos.z);	//距離を求める

		if (fLength > UPDATE_LENGTH) { continue; }
		bool bCutLine = CollsionLine(PlayerPos);	//当たり判定をする

		if (!bCutLine){ continue;}
		CManager::GetGame()->GetPlayer(m_nNumPlayer)->CutLine(m_nID);	//ライン切断処理					
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CLine::Draw(void)
{
	CScene3D::Draw();
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

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, Rot.y, Rot.x, Rot.z);
	D3DXMatrixMultiply(&mtxLineWorld, &mtxLineWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, Pos.x, Pos.y, Pos.z);
	D3DXMatrixMultiply(&mtxLineWorld, &mtxLineWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxLineWorld);

	// 計算用マトリックス
	D3DXMATRIX	mtxPriWorld, mtxPriRot, mtxPriTrans;			

	for (int nCnt = 0; nCnt < MAX_POS; nCnt++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxPriWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxPriRot, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtxPriWorld, &mtxPriWorld, &mtxPriRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxPriTrans, PrimitivPos[nCnt].x, PrimitivPos[nCnt].y, PrimitivPos[nCnt].z);
		D3DXMatrixMultiply(&mtxPriWorld, &mtxPriWorld, &mtxPriTrans);

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