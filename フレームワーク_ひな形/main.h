//=============================================================================
//
// ���C������ [main.h]
// Author : ���� �����@[ SATO ASUMI ]
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include <windows.h>
#include "d3dx9.h"										//�`�揈���ɕK�v
#include <crtdbg.h>										//���������[�N�`�F�b�N�ɕK�v
#define _CRTDBG_MAP_ALLOC
//#define new ::new(_NORMAL_BLOCK, __FILE__,__LINE__)

#define DIRECTINPUT_VERSION (0x0800)					//�r���h���̌x���Ώ��p�}�N��
#include "dinput.h"										//���͏����ɕK�v
#include "xaudio2.h"									//�T�E���h�����ɕK�p

//=============================================================================
// ���C�u�����̃����N
//=============================================================================
#pragma comment(lib,"d3d9.lib")		//�`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")	//[d3d9.lib]�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")	//Directx�R���|�[�l���g(���i)�g�p�ɕK�v
#pragma comment(lib,"winmm.lib")	//�V�X�e�������擾�ɕK�v
#pragma comment(lib,"dinput8.lib")	//���͏����ɕK�v

//=============================================================================
// �}�N����`
//=============================================================================
#define SCREEN_WIDTH	(1280)		// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(720)		// �E�C���h�E�̍���
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// 2�c�������_�t�H�[�}�b�g( ���_���W[2D] / ���_�J���[)
#define FVF_PRIMITIVE_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

// �R�c�|���S�����_�t�H�[�}�b�g( ���_���W[3D] / �@���x�N�g�� / ���_�J���[ / �e�N�X�`�����W)
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// �R�c�������_�t�H�[�}�b�g( ���_���W[3D] / �@���x�N�g�� / ���_�J���[)
#define FVF_PRIMITIVE_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE)

//=============================================================================
// �\���̂̒�`
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	float		rhw;	//1.0f�ŌŒ�
	D3DCOLOR	col;	//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`�����W
}VERTEX_2D;

typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	float		rhw;	//1.0f�ŌŒ�
	D3DCOLOR	col;	//���_�J���[
}PRIMIRIVE_2D;

//=============================================================================
// �R�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	D3DXVECTOR3 nor;	// �@���x�N�g��
	D3DCOLOR	col;	// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
}VERTEX_3D;

typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	D3DXVECTOR3 nor;	// �@���x�N�g��
	D3DCOLOR	col;	// ���_�J���[
}PRIMITIVE_3D;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
int GetFPS(void);
#endif