//=============================================================================
//
// 地面処理 [field.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#include "field.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "fieldmanager.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define ADD_ROT	(0.05f)
#define MOVE_Z_TIME	(12)
//#define FIELDDRAW
//=============================================================================
// 生成処理
//=============================================================================
CField * CField::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, TYPE type)
{
	//インスタンスの生成
	CField * pField;
	pField = new CField;

	//初期化処理
	pField->Init();

	//設定処理
	pField->Set(pos, size);
	pField->SetType(type);

	return pField;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CField::CField() : CScene3D(3, OBJTYPE_FLOR){}

//=============================================================================
// コンストラクタ
//=============================================================================
CField::CField(int nPriority, CScene::OBJTYPE obj) : CScene3D(nPriority, obj){}

//=============================================================================
// デストラクタ
//=============================================================================
CField::~CField(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CField::Init(void)
{
	CScene3D::Init();

	//変数の初期化
	m_state = STATE_NOT_UPDATE;
	m_bLight = false;
	m_nCountTime = 0;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CField::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CField::Update(void)
{
	if (m_state == STATE_NOT_UPDATE) { return; }

	CScene3D::Update();

	//カメラの描画判定
	if (m_type == TYPE_BLOCK) { ChackDraw(); }

	//状態ごとの更新処理
	switch (m_state)
	{
	case STATE_X_TURN: UpdateXturn(); break;
	case STATE_X_TURNBACK: UpdateXturnBack(); break;
	case STATE_MOVE_Z: UpdateMoveZ(); break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CField::Draw(void)
{

}

//=============================================================================
// X軸の回転処理
//=============================================================================
void CField::UpdateXturn(void)
{
	D3DXVECTOR3 Rot = CScene3D::GetRot();

	//X軸に回転値の加算
	Rot.x += ADD_ROT;

	if (Rot.x > D3DX_PI)
	{
		Rot.x = 0.0f;
		m_state = STATE_NORMAL;
		SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	}

	//回転値の設定
	CScene3D::SetRot(Rot);
}

//=============================================================================
// X軸の回転処理　裏面
//=============================================================================
void CField::UpdateXturnBack(void)
{
	D3DXVECTOR3 Rot = CScene3D::GetRot();

	//X軸に回転値の加算
	Rot.x += ADD_ROT;

	if (Rot.x > D3DX_PI * 2.0f)
	{
		Rot.x = 0.0f;
		m_state = STATE_NORMAL;
	}

	//回転値の設定
	CScene3D::SetRot(Rot);
}

//=============================================================================
// Z軸の移動処理
//=============================================================================
void CField::UpdateMoveZ(void)
{
	//時間の加算
	m_nCountTime++;
	if ((m_nCountTime % MOVE_Z_TIME) != 0) { return; }

	//位置を取得
	D3DXVECTOR3 Pos = CScene3D::GetPos();

	//移動処理
	Pos.z += (BLOCK_SIZE.z * 2.0f);

	//位置を設定
	CScene3D::SetPos(Pos);
}

//=============================================================================
// カメラの描画判定処理
//=============================================================================
void CField::ChackDraw(void)
{
#ifdef FIELDDRAW
	//変数宣言
	CGame * pGame = CManager::GetGame();
	D3DXVECTOR3 Pos = CScene3D::GetPos();
	bool bDraw[MAX_NUMPLAYER] = {};
	bool bDrawChack = true;

	//カメラごとの描画判定
	if (pGame == NULL) { return; }
	for (int nCnt = 0; nCnt < MAX_NUMPLAYER; nCnt++)
	{
		//NULLチェック
		CGameCamera * pGameCamera = pGame->GetGameCamera(nCnt);
		if (pGameCamera == NULL) { continue; }

		bDraw[nCnt] = pGame->GetGameCamera(nCnt)->ReflectObject(Pos);

		if (bDraw[nCnt])
		{
			bDrawChack = false;	//描画フラグ
			SetDraw(true);
			break;
		}
	}

	if (!bDrawChack) { return; }
	SetDraw(false);		//描画しない状態にする
#endif
}