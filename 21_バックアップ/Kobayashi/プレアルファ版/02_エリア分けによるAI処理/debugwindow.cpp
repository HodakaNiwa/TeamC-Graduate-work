//=============================================================================
//
// �f�o�b�O�E�C���h�E���� [debugwindow.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#include "debugwindow.h"
#include "manager.h"
#include "renderer.h"
//#include <string>
#include <atlconv.h>
#include "main.h"
#include "camera.h"
#include "game.h"
#include "player.h"
#include "model.h"
#include "enemy.h"
//=============================================================================
// �}�N����`
//=============================================================================
#define VECTOR_ZERO (D3DXVECTOR3(0.0f, 0.0f, 0.0f))		//�x�N�g�������l

//=============================================================================
// ��������
//=============================================================================
CDebugWindow * CDebugWindow::Create(void)
{
	//�C���X�^���X�̐���
	CDebugWindow * pDebug;
	pDebug = new CDebugWindow;

	//����������
	pDebug->Init();

	return pDebug;
}

//=============================================================================
//	�R���X�g���N�^
//=============================================================================
CDebugWindow::CDebugWindow()
{
	CGui::SetUse(true);		//���̍s�̓G���[�h�~�̂��߂ɕK�{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CDebugWindow::~CDebugWindow(){}

//=============================================================================
// ����������
//=============================================================================
void CDebugWindow::Init(void){}

//=============================================================================
// �I������
//=============================================================================
void CDebugWindow::Uninit(void)
{
	delete this;
}

//=============================================================================
// �X�V����
//=============================================================================
void CDebugWindow::Update(void)
{
	if (!m_bDrawGui) { return; }	//�G���[�h�~�̂��ߕK�{

	// imgui���t���[������
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin(u8"�f�o�b�O�E�C���h�E");								//�E�C���h�E��
	ImGui::Text(u8"�y F1 �z�F �f�o�b�O�E�C���h�E�@ON / OFF");			//FPS�̎擾
	ImGui::Text("FPS : %d", GetFPS());									//FPS�̎擾

	//�J�������_�E�����_
	CManager::MODE mode = CManager::GetMode();
	D3DXVECTOR3 PosV = VECTOR_ZERO;
	D3DXVECTOR3 PosR = VECTOR_ZERO;
	int nArea = 0;// �����т��ǉ�
	if (mode == CManager::MODE_GAME)
	{
		CCamera * GameCamera = CManager::GetGame()->GetGameCamera(0);
		PosV = GameCamera->GetPosV();
		PosR = GameCamera->GetPosR();
		// ���т��ǉ���
		CEnemy* pEnemy = CManager::GetGame()->GetEnemy(0);
		nArea = pEnemy->GetAreaNum();
	}

	ImGui::Text(u8"���_ �@: X %.2f, Y %.2f, Z %.2f", PosV.x, PosV.y, PosV.z);
	ImGui::Text(u8"�����_ : X %.2f, Y %.2f, Z %.2f", PosR.x, PosR.y, PosR.z);
	ImGui::Text(u8"���݂̃G���A�ԍ� : %d", nArea);// �����т��ǉ�


	if (ImGui::Button(u8"�f�o�b�N�J����"))
	{
		bool bDebugCamera = CGame::GetDebugCamera();
		bDebugCamera = bDebugCamera ? false : true;
		CGame::SetDebugCamera(bDebugCamera);
	}


	ImGui::End();	//�E�C���h�E�̏������݂̏I��

	//�f���E�C���h�E�̐ݒ菈��
	//bool m_bDemoWindow = true;					//�f���E�C���h�E�̃t���O
	//ImGui::ShowDemoWindow(&m_bDemoWindow);		//�f���E�C���h�E�̐���
}