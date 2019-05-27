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
		PointInt _maxSize;							// �o�͂�����ʂ̍ő�T�C�Y
		PointInt _drawSize;							// �g�`�̕`��T�C�Y

		int _waveRenderHandler = -1;				// �`�悳�ꂽ�g�`�̃n���h��
		int _waveMarginY = 30;						// �g�`�Ƃ̊Ԃ̋���
		int _drawMarginY = 10;						// �㉺�̊Ԃ̋���
		shared_ptr<AudioData> _audioData = nullptr;	// �Z�b�g���ꂽ�g�`�f�[�^

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

		// �g�`�̏���Ԃ�
		const shared_ptr<AudioData> GetAudioData() {
			return _audioData;
		}

		// �g�`�̏���ݒ肷��
		void SetAudioData(const shared_ptr<AudioData> audioData) {
			_audioData = audioData;
		}

		// �s�N�Z���̈ʒu�ƍ�������A�g�`�̕`������O�v�Z������(sampleStart, sampleLength�̓`�����l���{�̒l�ɂȂ�)
		void CalcRenderingData(PointInt pixelSize, uint32_t sampleStart, uint32_t sampleLength, uint32_t samplesPerSec = 1000);

		// �`��̃T�C�Y��ύX����
		void ChangeDrawSize(PointInt size);

		// �`��̃T�C�Y��Ԃ�
		PointInt GetDrawSize() const {
			return _drawSize;
		}

		// �`����X�V����
		void Draw(PointInt waveOffset, float playRatio) const;

	private:

		// �g�`��`�悷��
		void DrawWave(vector<vector<PointInt>> wavePoints, PointInt pixelSize) const;
	};
}
