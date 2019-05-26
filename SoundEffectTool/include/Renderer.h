#pragma once

// unique_ptr�g�p�ɂ�
#pragma warning(disable:4251)

#include <vector>
#include <memory>

#include "PointInt.h"
#include "SoundEffectToolDefine.h"

using namespace AudioLibrary;
using namespace std;

namespace SoundEffectTool {

	// ��ɔg�`��\������N���X
	class SOUNDEFFECTTOOL_API Renderer {

		bool _isInitialized = false;
		PointInt _maxSize;		// �o�͂�����ʂ̍ő�T�C�Y
		PointInt _drawSize;

		int _waveMarginY = 30;
		int _drawMarginY = 10;
		uint32_t _waveLength = 0;
		PointInt _waveSize;
		shared_ptr<AudioData> _audioData = nullptr;
		vector<vector<PointInt>> _waveDrawingPoints;

	public:

		// �R�s�[�͋֎~���邪�A���[�u�͋�����
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		Renderer();
		~Renderer();

		void Initialize(HWND hwnd, PointInt size);
		void Finalize();

		// �o�b�N�o�b�t�@��Ԃ�
		const void* GetBackBuffer() const;

		// �g�`�̏���ϊ����ăZ�b�g����
		void SetAudioData(const shared_ptr<AudioData> audioData);

		// �s�N�Z���̈ʒu�ƍ�������A�g�`�̕`������O�v�Z������(sampleStart, sampleLength�̓`�����l���{�̒l�ɂȂ�)
		void CalcRenderingData(PointInt pixelSize, uint32_t sampleStart, uint32_t sampleLength);

		// �`��̃T�C�Y��ύX����
		void ChangeDrawSize(PointInt size);

		// �`��̃T�C�Y��Ԃ�
		PointInt GetDrawSize() const {
			return _drawSize;
		}

		// �`����X�V����
		void DrawWave(float playRatio) const;

	};
}
