//=============================================================================
//
// デバッグウインドウ処理 [debugwindow.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#include "debugwindow.h"
#include "manager.h"
#include "renderer.h"
#include <string>
#include <atlconv.h>
#include "main.h"
#include "camera.h"
#include "game.h"
#include "player.h"
#include "model.h"
//=============================================================================
// マクロ定義
//=============================================================================
#define VECTOR_ZERO (D3DXVECTOR3(0.0f, 0.0f, 0.0f))		//ベクトル初期値

//=============================================================================
// 生成処理
//=============================================================================
CDebugWindow * CDebugWindow::Create(void)
{
	//インスタンスの生成
	CDebugWindow * pDebug;
	pDebug = new CDebugWindow;

	//初期化処理
	pDebug->Init();

	return pDebug;
}

//=============================================================================
//	コンストラクタ
//=============================================================================
CDebugWindow::CDebugWindow()
{
	CGui::SetUse(true);		//この行はエラー防止のために必須
}

//=============================================================================
// デストラクタ
//=============================================================================
CDebugWindow::~CDebugWindow(){}

//=============================================================================
// 初期化処理
//=============================================================================
void CDebugWindow::Init(void){}

//=============================================================================
// 終了処理
//=============================================================================
void CDebugWindow::Uninit(void)
{
	delete this;
}

//=============================================================================
// 更新処理
//=============================================================================
void CDebugWindow::Update(void)
{
	if (!m_bDrawGui) { return; }	//エラー防止のため必須

	// imgui毎フレーム処理
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin(u8"デバッグウインドウ");								//ウインドウ名
	ImGui::Text(u8"【 F1 】： デバッグウインドウ　ON / OFF");			//FPSの取得
	ImGui::Text("FPS : %d", GetFPS());									//FPSの取得

	//カメラ視点・注視点
	CManager::MODE mode = CManager::GetMode();
	D3DXVECTOR3 PosV = VECTOR_ZERO;
	D3DXVECTOR3 PosR = VECTOR_ZERO;

	if (mode == CManager::MODE_GAME)
	{
		CCamera * GameCamera = CManager::GetGame()->GetGameCamera(0);
		PosV = GameCamera->GetPosV();
		PosR = GameCamera->GetPosR();
	}

	ImGui::Text(u8"視点 　: X %.2f, Y %.2f, Z %.2f", PosV.x, PosV.y, PosV.z);
	ImGui::Text(u8"注視点 : X %.2f, Y %.2f, Z %.2f", PosR.x, PosR.y, PosR.z);


	ImGui::End();	//ウインドウの書き込みの終了

	//デモウインドウの設定処理
	//bool m_bDemoWindow = true;					//デモウインドウのフラグ
	//ImGui::ShowDemoWindow(&m_bDemoWindow);		//デモウインドウの生成
}