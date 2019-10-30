//=============================================================================
//
// GUI���� [GUI.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#include "GUI.h"
#include "manager.h"
#include "renderer.h"
//#include <string>
#include <atlconv.h>

//=============================================================================
//	�ÓI�����o�ϐ��錾
//=============================================================================
bool CGui::m_bDrawGui = true;
bool CGui::m_bUseGui = false;

//=============================================================================
//	�R���X�g���N�^
//=============================================================================
CGui::CGui(){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGui::~CGui(){}

//=============================================================================
//	Imgui�̃Z�b�g�A�b�v
//=============================================================================
void CGui::SetUp(HWND hWnd)
{
	//�����_���[�̎擾
	CRenderer * pRenderer = CManager::GetRenderer();

	// Imgui�̃Z�b�g�A�b�v
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// �X�^�C���̐ݒ�
	//ImGui::StyleColorsDark();						//�o�b�N�O���E���h���Z��
	ImGui::StyleColorsClassic();					//�o�b�O�O���E���h��������

	// �����_���[�̃u�����f�B���O�ݒ�
	ImGui_ImplWin32_Init(hWnd);						//Window��ݒ肷��
	ImGui_ImplDX9_Init(pRenderer->GetDevice());		//DirectX��ݒ肷��

	//�t�H���g�̎擾
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\HGRSGU.TTC", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
}

//=============================================================================
//	Imgui�̔j��
//=============================================================================
void CGui::Teardown(void)
{
	ImGui_ImplWin32_Shutdown();		//ImGui��Windows�̊J��
	ImGui_ImplDX9_Shutdown();		//ImGui��DirectX�̊J��
	ImGui::DestroyContext();		//ImGui��j������
}

//=============================================================================
//	Imgui�̕`��
//=============================================================================
void CGui::Renderer(void)
{
	if (!m_bUseGui) { return; }		//�g�p��Ԃ��ǂ���
	if (!m_bDrawGui) { return; }	//GUI��`�悷�邩�ǂ���

	ImGui::EndFrame();										//�t���[���̏I��
	ImGui::Render();										//�`�揈��
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());		//�f�[�^�̎擾
}

#if 0
//=============================================================================
// ����������
//=============================================================================
void CGui::Init(void)
{
}

//=============================================================================
// �I������
//=============================================================================
void CGui::Uninit(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void CGui::Update(void)
{
	// imgui���t���[������
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin(u8"�f�o�b�O�E�C���h�E");		//�E�C���h�E��
	ImGui::Text("FPS : %d", GetFPS());			//FPS�̎擾

	ImGui::End();	//�E�C���h�E�̏������݂̏I��

	//�E�C���h�E�̐ݒ菈��
	bool m_bDemoWindow = true;					//�f���E�C���h�E�̃t���O
	ImGui::ShowDemoWindow(&m_bDemoWindow);		//�f���E�C���h�E�̐���
}
#endif

//=============================================================================
//	int�^�̍ŏ��l�C��
//=============================================================================
void CGui::RetouchMinimumInt(int * nRetouch, int nMinimum)
{
	if (*nRetouch < nMinimum)
	{
		*nRetouch = nMinimum;
	}
}

//=============================================================================
//	float�^�̍ŏ��l�C��
//=============================================================================
void CGui::RetouchMinimumFloat(float * fRetouch, float fMinimum)
{
	if (*fRetouch < fMinimum)
	{
		*fRetouch = fMinimum;
	}
}