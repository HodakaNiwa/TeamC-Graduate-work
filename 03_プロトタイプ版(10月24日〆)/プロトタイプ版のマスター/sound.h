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
		SOUND_LABEL_BGM003,			// �Q�[���I�[�o�[
		SOUND_LABEL_BGM004,			// �����L���O
		SOUND_LABEL_BGM005,			// �`���[�g���A��
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
