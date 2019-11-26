//*****************************************************************************
//
//     キャラアイコンの処理[charaicon.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "character.h"
#include "charaicon.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"
#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define CHARAICON_DISP_LENGTH (1200.0f)		// キャラアイコンを描画する距離

//*****************************************************************************
//     静的メンバ変数宣言
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CCharaicon::CCharaicon()
{
	m_pParentChara = NULL;
	m_pDestChara = NULL;
	m_bDisp = false;
	m_nIdx = 0;
	m_nNumPlayer = 0;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CCharaicon::~CCharaicon()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CCharaicon *CCharaicon::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nIdx, int nNumPlayer, float fTexU, float fTexV, float fTexWidth, float fTexHeight, float fRot)
{
	CCharaicon *pCharaicon = NULL;
	if (pCharaicon != NULL) { return NULL; }

	// メモリを確保
	pCharaicon = new CCharaicon;
	if (pCharaicon == NULL) { return NULL; }

	// 各種値の設定
	pCharaicon->SetPos(pos);              // ポリゴンの座標
	pCharaicon->SetRot(fRot);             // ポリゴンの向き
	pCharaicon->SetCol(col);              // ポリゴンの色
	pCharaicon->SetWidth(fWidth);         // ポリゴンの幅
	pCharaicon->SetHeight(fHeight);       // ポリゴンの高さ
	pCharaicon->SetTexU(fTexU);           // ポリゴンの左上テクスチャU座標
	pCharaicon->SetTexV(fTexV);           // ポリゴンの左上テクスチャV座標
	pCharaicon->SetTexWidth(fTexWidth);   // ポリゴンのテクスチャ座標の横幅
	pCharaicon->SetTexHeight(fTexHeight); // ポリゴンのテクスチャ座標の縦幅
	pCharaicon->SetIdx(nIdx);             // どの番号の画面と紐づいているか
	pCharaicon->SetNumPlayer(nNumPlayer); // ゲームに参加しているプレイヤーの人数

	// 初期化処理
	if (FAILED(pCharaicon->Init()))
	{
		pCharaicon->Uninit();
		delete pCharaicon;
		return NULL;
	}

	return pCharaicon;	// インスタンスのアドレスを返す
}


//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CCharaicon::Init(void)
{
	if (FAILED(CIcon2D::Init()))
	{// アイコンの初期化に失敗
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CCharaicon::Uninit(void)
{
	CIcon2D::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CCharaicon::Update(void)
{

}

//=============================================================================
//    目的のキャラが描画されているか判定する処理
//=============================================================================
void CCharaicon::CheckDraw(void)
{
	if (m_pDestChara == NULL || m_pDestChara == m_pParentChara) { m_bDisp = false; return; }

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return; }

	// 親キャラの位置を取得
	D3DXVECTOR3 ParentPos = m_pParentChara->GetPos();

	// 目的のキャラの位置を取得
	D3DXVECTOR3 pos = m_pDestChara->GetPos();

	// 距離を測定し描画する距離でなければ処理終了
	float fLength = CFunctionLib::Vec2Length(ParentPos.x, ParentPos.z, pos.x, pos.z);
	if (fLength > CHARAICON_DISP_LENGTH) { m_bDisp = false; return; }

	// デバイスからビューマトリックスとプロジェクションマトリックスを取得
	D3DXMATRIX mtxWorld, mtxView, mtxProj, mtxViewport;
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);

	// ビューポート行列を作成
	D3DVIEWPORT9 Viewport;
	D3DXMatrixIdentity(&mtxViewport);
	pDevice->GetViewport(&Viewport);
	float fValue1 = (float)Viewport.Width / 2;
	float fValue2 = (float)Viewport.Height / 2;
	mtxViewport._11 = fValue1;
	mtxViewport._22 = -1.0f * fValue2;
	mtxViewport._41 = fValue1;
	mtxViewport._42 = fValue2;
	mtxWorld = mtxView * mtxProj * mtxViewport;

	// 位置をスクリーンに投影
	D3DXVec3TransformCoord(&pos, &pos, &mtxWorld);

	// スクリーン内に収まっているか判定
	if (pos.x > Viewport.X && pos.x < Viewport.X + Viewport.Width
		&& pos.y > Viewport.Y && pos.y < Viewport.Y + Viewport.Height)
	{// 収まっているならアイコンは消す
		m_bDisp = false;
	}
	else
	{// 収まっていないならアイコンを描画する
		m_bDisp = true;
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CCharaicon::Draw(void)
{
	// 描画判定
	CheckDraw();

	if (m_bDisp == false) { return; }

	// スクリーンとの交点を算出
	CalcCrossPoint_ScreenLine();

	// アイコンの描画
	CIcon2D::Draw();
}

//=============================================================================
//    スクリーン端との交点を計算する処理
//=============================================================================
void CCharaicon::CalcCrossPoint_ScreenLine(void)
{
	if (m_pDestChara == NULL || m_pParentChara == NULL) { return; }

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return; }

	// 目的のキャラ位置と親元のキャラクターの位置を取得
	D3DXVECTOR3 DestPos = m_pDestChara->GetPos();
	D3DXVECTOR3 ParentPos = m_pParentChara->GetPos();

	// デバイスからビューマトリックスとプロジェクションマトリックスを取得
	D3DXMATRIX mtxWorld, mtxView, mtxProj, mtxViewport;
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);

	// ビューポート行列を作成
	D3DVIEWPORT9 Viewport;
	D3DXMatrixIdentity(&mtxViewport);
	pDevice->GetViewport(&Viewport);
	float fValue1 = (float)Viewport.Width / 2;
	float fValue2 = (float)Viewport.Height / 2;
	mtxViewport._11 = fValue1;
	mtxViewport._22 = -1.0f * fValue2;
	mtxViewport._41 = fValue1;
	mtxViewport._42 = fValue2;
	mtxWorld = mtxView * mtxProj * mtxViewport;

	// スクリーン座標に変換
	D3DXVec3TransformCoord(&DestPos, &DestPos, &mtxWorld);
	D3DXVec3TransformCoord(&ParentPos, &ParentPos, &mtxWorld);

	// 4本のベクトルの交点を算出
	float X = (float)Viewport.X;
	float Y = (float)Viewport.Y;
	float Width = (float)Viewport.Width;
	float Height = (float)Viewport.Height;
	D3DXVECTOR3 ScreenPos[4]
	{
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(Width, 0.0f, 0.0f),
		D3DXVECTOR3(Width, Height, 0.0f),
		D3DXVECTOR3(0.0f, Height, 0.0f),
	};
	D3DXVECTOR3 InterPos = D3DXVECTOR3(10000.0f,10000.0f,0.0f);
	int nIdxInter = -1;
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		// 線分を作成
		D3DXVECTOR3 Vec1 = D3DXVECTOR3(ParentPos.x - ScreenPos[nCnt].x, ParentPos.y - ScreenPos[nCnt].y, 0.0f);
		D3DXVECTOR3 Vec2 = D3DXVECTOR3(DestPos.x - ParentPos.x, DestPos.y - ParentPos.y, 0.0f);
		D3DXVECTOR3 Vec3 = D3DXVECTOR3(ScreenPos[(nCnt + 1) % 4].x - ScreenPos[nCnt].x, ScreenPos[(nCnt + 1) % 4].y - ScreenPos[nCnt].y, 0.0f);

		// 外積を出す(0なら平行なので処理せず次のループへ)
		float fDeno = Vec2.x * Vec3.y - Vec2.y * Vec3.x;
		if (fDeno == 0) { continue; }

		// 距離の比率を算出
		float fLengthStart = (Vec1.y * Vec3.x - Vec1.x * Vec3.y) / fDeno;
		float fLengthEnd = (Vec1.y * Vec2.x - Vec1.x * Vec2.y) / fDeno;
		if (fLengthStart < 0 || 1 < fLengthStart || fLengthEnd < 0 || 1 < fLengthEnd) { continue; }

		// 交点算出
		InterPos.x = Vec2.x * fLengthStart + ParentPos.x;
		InterPos.y = Vec2.y * fLengthStart + ParentPos.y;
		nIdxInter = nCnt;
	}
	float fLength = GetLength();
	switch (nIdxInter)
	{// どの画面端と交点が結ばれているかで処理わけ
	case 0:
		InterPos.y += fLength;
		break;
	case 1:
		InterPos.x -= fLength;
		break;
	case 2:
		InterPos.y -= fLength;
		break;
	case 3:
		InterPos.x += fLength;
		break;
	}

	switch (m_nIdx)
	{
	case 0:
		break;
	case 1:
		InterPos.x += SCREEN_WIDTH / 2.0f;
		break;
	case 2:
		InterPos.y += SCREEN_HEIGHT / 2.0f;
		break;
	case 3:
		InterPos.x += SCREEN_WIDTH / 2.0f;
		InterPos.y += SCREEN_HEIGHT / 2.0f;
		break;
	}

	// 位置の設定
	SetPos(InterPos);
	SetVtxBuffPos();
}


//=============================================================================
//    画面の大きさに合わせてずらす処理
//=============================================================================
D3DXVECTOR3 CCharaicon::ShiftScreenPos(D3DXVECTOR3 pos)
{
	switch (m_nIdx)
	{
	case 0:
		break;
	case 1:
		pos.x *= 2.0f;
		CDebugProc::Print("%.1f %.1f %.1f\n", pos.x, pos.y, pos.z);
		break;
	case 2:
		pos.y *= 2.0f;
		break;
	case 3:
		pos.x *= 2.0f;
		pos.y *= 2.0f;
		break;
	}

	return pos;
}