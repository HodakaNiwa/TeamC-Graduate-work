//=============================================================================
//
// �����_�����O���� [renderer.h]
// Author : ���� �����@[ SATO ASUMI ]
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"

//=============================================================================
// �O���錾
//=============================================================================
class CDebugProc;
class CDebugWindow;

//=============================================================================
// �����_�����O�N���X
//=============================================================================
class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	//�����o�֐�
	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; };

	static CDebugProc * GetDebugLeft(void);								//�f�o�b�N�\���|�C���^�̎擾
	static CDebugProc * GetDebugRight(void) { return m_pDebugRight;};	//�f�o�b�N�\���|�C���^�̎擾
	static bool GetDrawDebugRight(void) { return m_bDrawDebugRight;};	//�\����Ԃ��擾

private:
	//�����o�ϐ�
	LPDIRECT3D9				m_pD3D;				// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9		m_pD3DDevice;		// Device�I�u�W�F�N�g(�`��ɕK�v)

	static CDebugProc * m_pDebugLeft;
	static CDebugProc * m_pDebugRight;
	static bool m_bDrawDebugLeft;				//�f�o�b�N��\�����Ă��邩�ǂ���
	static bool m_bDrawDebugRight;				//�f�o�b�N��\�����Ă��邩�ǂ���
	static bool m_bWireFram;					//���C���[�t���[����\������
	CDebugWindow *	m_pDebugWindow;				// �f�o�b�O�E�C���h�E
};
#endif