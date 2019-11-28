//=============================================================================
//
// �����_�����O���� [renderer.h]
// Author : ���� �����@[ SATO ASUMI ]
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"
#include "modebase.h"

//=============================================================================
// �O���錾
//=============================================================================
class CSky;

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
	void Draw(CModeBase *pModeBase);
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; };

private:
	//�����o�ϐ�
	LPDIRECT3D9				m_pD3D;				// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9		m_pD3DDevice;		// Device�I�u�W�F�N�g(�`��ɕK�v)

	// �w�i�`��p
	HRESULT CreateBG(void);
	void ReleaseBG(void);

	LPDIRECT3DTEXTURE9 m_pTextureBg;			// �w�i�e�N�X�`���ւ̃|�C���^
	CSky *m_pSky;								// �w�i�p���N���X�ւ̃|�C���^
};
#endif