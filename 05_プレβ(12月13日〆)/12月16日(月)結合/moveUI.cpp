//=============================================================================
//
// 移動タイプのUIの処理 [MoveUI.cpp]
// Author : 佐藤安純　SATOU ASUMI
//
//=============================================================================
#include "moveUI.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
#include "logo.h"
#include "scene2D.h"
#include "library.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define VECTOR_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	//ベクトルの初期化

//=============================================================================
// 生成処理
//=============================================================================
CMoveUI * CMoveUI::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, LPDIRECT3DTEXTURE9 pTexture)
{
	CMoveUI * pMoveUI = new CMoveUI;

	pMoveUI->Init();
	pMoveUI->Set(pos, size);
	pMoveUI->BindTexture(pTexture);

	return pMoveUI;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CMoveUI::CMoveUI() : CScene2D(10, CScene::OBJTYPE_UI){}

//=============================================================================
// デストラクタ
//=============================================================================
CMoveUI::~CMoveUI(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMoveUI::Init(void)
{
	CScene2D::Init();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMoveUI::Uninit()
{
	CScene2D::Uninit();

	//死亡フラグを立てる
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMoveUI::Update(){}

//=============================================================================
// 描画処理
//=============================================================================
void CMoveUI::Draw()
{
	CScene2D::Draw();
}

//=============================================================================
// 左に移動する
//=============================================================================
void CMoveUI::UpdateMoveLeft(float fMove)
{
	//位置の取得
	D3DXVECTOR3 Pos = CScene2D::GetPos();

	//移動処理
	Pos.x -= fMove;

	//位置設定
	CScene2D::SetPos(Pos, CScene2D::GetOtherPos());
}

//=============================================================================
// 右に移動する
//=============================================================================
void CMoveUI::UpdateMoveRight(float fMove)
{
	//位置の取得
	D3DXVECTOR3 Pos = CScene2D::GetPos();

	//移動処理
	Pos.x += fMove;

	//位置設定
	CScene2D::SetPos(Pos, CScene2D::GetOtherPos());
}

//=============================================================================
// 上に移動する
//=============================================================================
void CMoveUI::UpdateMoveUp(float fMove)
{
	//位置の取得
	D3DXVECTOR3 Pos = CScene2D::GetPos();

	//移動処理
	Pos.y -= fMove;

	//位置設定
	CScene2D::SetPos(Pos, CScene2D::GetOtherPos());
}

//=============================================================================
// 下に移動する
//=============================================================================
void CMoveUI::UpdateMoveDown(float fMove)
{
	//位置の取得
	D3DXVECTOR3 Pos = CScene2D::GetPos();

	//移動処理
	Pos.y += fMove;

	//位置設定
	CScene2D::SetPos(Pos, CScene2D::GetOtherPos());
}

//=============================================================================
// 拡大処理
//=============================================================================
void CMoveUI::UpdateScaleUp(D3DXVECTOR3 AimSize, D3DXVECTOR2 Move)
{
	//サイズの取得
	D3DXVECTOR3 Size = CScene2D::GetSize();

	//Xのサイズ設定
	if (Size.x < AimSize.x)
	{
		Size.x += Move.x;
	}
	else
	{
		Size.x = AimSize.x;
	}

	//Yのサイズ設定
	if (Size.y < AimSize.y)
	{
		Size.y += Move.y;
	}
	else
	{
		Size.y = AimSize.y;
	}

	//サイズの設定
	CScene2D::SetSize(Size);
}

//=============================================================================
//	縮小処理
//=============================================================================
void CMoveUI::UpdateScaleDown(D3DXVECTOR3 AimSize, D3DXVECTOR2 Move)
{
	//サイズの取得
	D3DXVECTOR3 Size = CScene2D::GetSize();

	//Xのサイズ設定
	if (Size.x > AimSize.x)
	{
		Size.x -= Move.x;
	}
	else
	{
		Size.x = AimSize.x;
	}

	//Yのサイズ設定
	if (Size.y > AimSize.y)
	{
		Size.y -= Move.y;
	}
	else
	{
		Size.y = AimSize.y;
	}

	//サイズの設定
	CScene2D::SetSize(Size);
}

//=============================================================================
//	目的の位置まで移動する
//=============================================================================
void CMoveUI::UpdateMoveDest(D3DXVECTOR3 AimPos, float fSpeed)
{
	//変数宣言
	bool bBigerX = false;
	bool bBigerY = false;

	//目的地よりも小さいか大きいか
	D3DXVECTOR3 Pos = CScene2D::GetPos();
	if (Pos.x > AimPos.x) { bBigerX = true; }
	if (Pos.y > AimPos.y) { bBigerY = true; }

	//目的の注視点の角度を求める
	float fAngle = CFunctionLib::CalcAngleToDest(Pos.x, Pos.y, AimPos.x, AimPos.y);

	//注視点の更新
	Pos.x += sinf(fAngle) * fSpeed;
	Pos.y += cosf(fAngle) * fSpeed;

	//値の修正
	if (bBigerX)
	{//X値が目的地より大きい
		if (Pos.x < AimPos.x) { Pos.x = AimPos.x; }
	}
	else
	{//X値が目的地より小さい
		if (Pos.x > AimPos.x) { Pos.x = AimPos.x; }
	}

	if (bBigerY)
	{//Y値が目的地より大きい
		if (Pos.y < AimPos.y) { Pos.y = AimPos.y; }
	}
	else
	{//Y値が目的地より小さい
		if (Pos.y > AimPos.y) { Pos.y = AimPos.y; }
	}

	//位置を設定
	CScene2D::SetPos(Pos, CScene2D::GetOtherPos());
}

//=============================================================================
//	頂点ごとの設定で目的地までの移動
//=============================================================================
void CMoveUI::UpdateMoveDestVertex(D3DXVECTOR3 AimPos, float fSpeed)
{
	//変数宣言
	bool bBigerX = false;
	bool bBigerY = false;

	//目的地よりも小さいか大きいか
	D3DXVECTOR3 Pos = CScene2D::GetPos();
	if (Pos.x > AimPos.x) { bBigerX = true; }
	if (Pos.y > AimPos.y) { bBigerY = true; }

	//目的の注視点の角度を求める
	float fAngle = CFunctionLib::CalcAngleToDest(Pos.x, Pos.y, AimPos.x, AimPos.y);

	//注視点の更新
	Pos.x += sinf(fAngle) * fSpeed;
	Pos.y += cosf(fAngle) * fSpeed;

	//値の修正
	if (bBigerX)
	{//X値が目的地より大きい
		if (Pos.x < AimPos.x) { Pos.x = AimPos.x; }
	}
	else
	{//X値が目的地より小さい
		if (Pos.x > AimPos.x) { Pos.x = AimPos.x; }
	}

	if (bBigerY)
	{//Y値が目的地より大きい
		if (Pos.y < AimPos.y) { Pos.y = AimPos.y; }
	}
	else
	{//Y値が目的地より小さい
		if (Pos.y > AimPos.y) { Pos.y = AimPos.y; }
	}

	//位置を設定
	CScene2D::SetPosVertex(Pos);
}