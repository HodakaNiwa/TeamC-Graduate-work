//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : SATO ASUMI
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//=============================================================================
// �T�E���h�N���X
//=============================================================================
class CSound
{
public:
	typedef enum
	{
		SOUND_LABEL_BGM000 = 0,		// �^�C�g��
		SOUND_LABEL_BGM001,			// �Q�[��
		SOUND_LABEL_BGM002,			// ���U���g
		SOUND_LABEL_BGM003,			// ���U���g
		SOUND_LABEL_BGM004,			// �����L���O
		SOUND_LABEL_BGM005,			// �`���[�g���A��
		SOUND_LABEL_BGM006,			// �Z���N�g
		SOUND_LABEL_SE007,			// ���艹
		SOUND_LABEL_SE008,			// �L�����Z��
		SOUND_LABEL_SE009,			// �Z���N�g
		SOUND_LABEL_SE010,			// �J�E���g�_�E���i�J�n���j
		SOUND_LABEL_SE011,			// �Q�[���X�^�[�g
		SOUND_LABEL_SE012,			// �^�C���A�i�E���X
		SOUND_LABEL_SE013,			// �J�E���g�_�E���i�I�����j
		SOUND_LABEL_SE014,			// �Q�[���I��
		SOUND_LABEL_SE015,			// ���U���g�F���ʔ��\
		SOUND_LABEL_SE016,			// �|�[�Y�J�n
		SOUND_LABEL_SE017,			// �|�C���g�l��
		SOUND_LABEL_SE018,			// ����
		SOUND_LABEL_SE019,			// �_�E��
		SOUND_LABEL_SE020,			// ���x�^�U��
		SOUND_LABEL_SE021,			// ���x�^�U������
		SOUND_LABEL_SE022,			// �p���[�^�U��
		SOUND_LABEL_SE023,			// �p���[�^�U������
		SOUND_LABEL_SE024,			// �n���^�U��
		SOUND_LABEL_SE025,			// �n���^�U������
		SOUND_LABEL_SE026,			// �̒n�m��
		SOUND_LABEL_SE027,			// ����1�i�����[�I�j
		SOUND_LABEL_SE028,			// ����2�i��������j
		SOUND_LABEL_SE029,			// �̒n�m�ۖW�Q
		SOUND_LABEL_SE030,			// �X�s�[�h�A�b�v
		SOUND_LABEL_SE031,			// �Z���N�g��ʂ̊���		 
		SOUND_LABEL_SE032,			// ���f�B�[�g�D�t�@�C�g�̉��@
		SOUND_LABEL_SE033,			// �X�L���A�C�R�������܂�����	���@
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	CSound();
	~CSound();
	HRESULT InitSound(HWND hWnd);
	void UninitSound(void);
	static HRESULT PlaySound(SOUND_LABEL label);
	static void StopSound(SOUND_LABEL label);
	static void StopSound(void);
	static void SetVolume(SOUND_LABEL label, float fVolume);

private:
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	static IXAudio2 *m_pXAudio2;									// XAudio2�I�u�W�F�N�g�ւ̃|�C���^
	static IXAudio2MasteringVoice *m_pMasteringVoice;				// �}�X�^�[�{�C�X�ւ̃|�C���^
	static IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X�ւ̃|�C���^
	static BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�ւ̃|�C���^
	static DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�T�C�Y
};
#endif
