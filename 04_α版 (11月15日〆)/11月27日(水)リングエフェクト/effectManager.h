//*****************************************************************************
//
//     �G�t�F�N�g�Ǌ��̏���[effectManager.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _EFFECTMANAGER_H_
#define _EFFECTMANAGER_H_

//*****************************************************************************
//     �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define MAX_EMMITER         (150)     // �G�~�b�^�f�[�^�̍ő吔
#define MAX_PARTICLEDATA    (150)     // �p�[�e�B�N���f�[�^�̍ő吔
#define MAX_RINGEFFECTDATA  (150)     // �����O�G�t�F�N�g�f�[�^�̍ő吔

//*****************************************************************************
//     �O���錾
//*****************************************************************************
class CFileLoader;
class CFileSaver;
class CEmitterData;
class CRingData;
class CEmitter;
class CTextureManager;

//*****************************************************************************
//     �G�t�F�N�g�}�l�[�W���[�N���X�̒�`
//*****************************************************************************
class CEffectManager
{
public:   // �N�ł��A�N�Z�X�\
	typedef enum
	{
		EFFECT_TYPE_PARTICLE = 0,  // �p�[�e�B�N��
		EFFECT_TYPE_RING,          // �����O
		EFFECT_TYPE_MAX
	}EFFECT_TYPE;

	CEffectManager();
	~CEffectManager();

	static CEffectManager *Create(char *pFileName);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	CEmitter *SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority = 3);

	void SetFileName(char *pFileName);
	void SetTextureListFileName(char *pFileName);
	void SetEmitter(CEmitter *pEmitter);
	void SetEmitterData(CEmitterData *pEmitterData, const int nIdx);
	void SetRingEffectData(CRingData *pRingData, const int nIdx);
	void SetNumEmitterData(const int nNumEmitterData);
	void SetNumRingEffectData(const int nNumRingEffectData);
	void SetTextureManager(CTextureManager *pTextureManager);

	char *GetFileName(void);
	char *GetTextureListFileName(void);
	CEmitter *GetEmitter(void);
	CEmitterData **GetEmitterData(void);
	CEmitterData *GetEmitterData(const int nIdx);
	CRingData **GetRingEffectData(void);
	CRingData *GetRingEffectData(const int nIdx);
	int GetNumEmitterData(void);
	int GetNumRingEffectData(void);
	CTextureManager *GetTextureManager;

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void CreateTextureMemory(const int nNumTexture);
	void CreateEmitterDataMemory(const int nNumEmitterData);
	void CreateRingEffectDataMemory(const int nNumRingEffectData);

	void ReleaseTextureManager(void);
	void ReleaseEmitterData(void);
	void ReleaseRingEffectData(void);

	HRESULT LoadScript(CFileLoader *pFileLoader, char *pStr);
	HRESULT LoadTexture(char *pTextureFileName, int nCntTex);
	CEmitterData *LoadEmitterData(CFileLoader *pFileLoader, char *pStr, const int nCntEmitter);
	CRingData *LoadRingEffectData(CFileLoader *pFileLoader, char *pStr, const int nCntRingData);

	void Save(void);
	void SaveTextureData(CFileSaver *pFileSaver);
	void SaveEmitterData(CFileSaver *pFileSaver);
	void SaveRingEffectData(CFileSaver *pFileSaver);

	char            m_aFileName[256];				// �X�N���v�g�t�@�C����
	char            m_aTextureListFileName[256];	// �e�N�X�`�����X�g�t�@�C����
	CEmitter        *m_pEmitter;					// �Ō�ɐ��������G�~�b�^�N���X�ւ̃|�C���^
	int             m_nNumEmitterData;				// �G�~�b�^�f�[�^�̌�
	int             m_nNumRingEffectData;			// �����O�G�t�F�N�g�f�[�^�̌�
	CEmitterData    **m_apEmitterData;				// �G�~�b�^�f�[�^�N���X�ւ̃|�C���^
	CRingData       **m_apRingData;					// �����O�G�t�F�N�g�f�[�^�N���X�ւ̃|�C���^
	CTextureManager *m_pTextureManager;				// �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
};

#endif