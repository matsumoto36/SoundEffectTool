#pragma once

// unique_ptr�g�p�ɂ�
#pragma warning(disable:4251)

#include <vector>
#include <memory>

#include "SoundEffectToolDefine.h"

using namespace AudioLibrary;
using namespace std;

namespace SoundEffectTool {

	class SOUNDEFFECTTOOL_API Renderer {

		bool _isInitialized = false;
		int _maxWidth, _maxHeight;		// �o�͂�����ʂ̍ő�T�C�Y
		int _width, _height;

		uint32_t _waveLength = 0;
		uint32_t _waveWidth = 1024;
		uint32_t _waveHeight = 100;
		shared_ptr<AudioData> _audioData = nullptr;
		vector<vector<int>> _waveDrawingData;

	public:

		// �R�s�[�͋֎~���邪�A���[�u�͋�����
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		Renderer();
		~Renderer();

		void Initialize(HWND, int, int);
		void Finalize();

		// �o�b�N�o�b�t�@���擾����
		const void* GetBackBuffer() const;

		// �g�`�̏���ϊ����ăZ�b�g����
		void SetAudioData(const shared_ptr<AudioData> audioData);

		// �g�`�̒����ƈʒu��ύX(start, length�̓`�����l�����Ƃ̒���)
		void SetRenderingData(uint32_t waveWidth, uint32_t waveHeight, uint32_t waveStart, uint32_t waveLength);

		// �`��̃T�C�Y��ύX����
		void ChangeDrawSize(int width, int height);

		// �`����X�V����
		void DrawWave() const;

	};
}
