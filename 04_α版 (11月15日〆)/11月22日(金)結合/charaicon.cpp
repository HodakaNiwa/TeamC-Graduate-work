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
#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************


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
CCharaicon *CCharaicon::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fTexU, float fTexV, float fTexWidth, float fTexHeight, float fRot)
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

	// 目的のキャラの位置を取得
	D3DXVECTOR3 pos = m_pDestChara->GetPos();

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
	mtxViewport._33 = Viewport.MaxZ - Viewport.MinZ;
	mtxViewport._41 = (float)(Viewport.X + fValue1);
	mtxViewport._42 = (float)(Viewport.Y + fValue2);
	mtxViewport._43 = Viewport.MinZ;
	mtxWorld = mtxView * mtxProj * mtxViewport;

	// 位置をスクリーンに投影
	D3DXVec3TransformCoord(&pos, &pos, &mtxWorld);

	// スクリーン内に収まっているか判定
	if (pos.x <= Viewport.X || pos.x >= Viewport.X + Viewport.Width
		|| pos.y <= Viewport.Y || pos.y >= Viewport.Y + Viewport.Height)
	{// 収まっていないならアイコンを描画する
		m_bDisp = true;
	}
	else
	{// 収まっているならアイコンは消す
		m_bDisp = false;
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
	mtxWorld = mtxView * mtxProj;

	// ビューポート行列を作成
	D3DVIEWPORT9 Viewport;
	D3DXMatrixIdentity(&mtxViewport);
	pDevice->GetViewport(&Viewport);
	float fValue1 = (float)Viewport.Width / 2;
	float fValue2 = (float)Viewport.Height / 2;
	mtxViewport._11 = fValue1;
	mtxViewport._22 = -1.0f * fValue2;
	mtxViewport._33 = Viewport.MaxZ - Viewport.MinZ;
	mtxViewport._41 = (float)(Viewport.X + fValue1);
	mtxViewport._42 = (float)(Viewport.Y + fValue2);
	mtxViewport._43 = Viewport.MinZ;
	mtxWorld *= mtxViewport;

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
		D3DXVECTOR3(X, Y, 0.0f),
		D3DXVECTOR3(X + Width, Y, 0.0f),
		D3DXVECTOR3(X + Width, Y + Height, 0.0f),
		D3DXVECTOR3(X, Y + Height, 0.0f),
	};
	D3DXVECTOR3 InterPos = INITIALIZE_VECTOR3;
	int nIdxInter = -1;
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		// 線分を作成
		D3DXVECTOR3 Vec1 = D3DXVECTOR3(DestPos.x - ScreenPos[nCnt].x, DestPos.y - ScreenPos[nCnt].y, 0.0f);
		D3DXVECTOR3 Vec2 = D3DXVECTOR3(ParentPos.x - DestPos.x, ParentPos.y - DestPos.y, 0.0f);
		D3DXVECTOR3 Vec3 = D3DXVECTOR3(ScreenPos[(nCnt + 1) % 4].x - ScreenPos[nCnt].x, ScreenPos[(nCnt + 1) % 4].y - ScreenPos[nCnt].y, 0.0f);

		// 外積を出す(0なら平行なので処理せず次のループへ)
		float fDeno = Vec2.x * Vec3.y - Vec2.y * Vec3.x;
		if (fDeno == 0) { continue; }

		// 距離の比率を算出
		float fLengthStart = (Vec1.y * Vec3.x - Vec1.x * Vec3.y) / fDeno;
		float fLengthEnd = (Vec1.y * Vec2.x - Vec1.x * Vec2.y) / fDeno;
		if (fLengthStart < 0 || 1 < fLengthStart || fLengthEnd < 0 || 1 < fLengthEnd) { continue; }

		// 交点算出
		InterPos.x = Vec2.x * fLengthStart + DestPos.x;
		InterPos.y = Vec2.y * fLengthStart + DestPos.y;
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

	// 位置の設定
	SetPos(InterPos);
	SetVtxBuffPos();
}