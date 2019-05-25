#pragma once

// unique_ptr�g�p�ɂ�
#pragma warning(disable:4251)

#include <vector>
#include <memory>

#include "SoundEffectToolDefine.h"

using namespace AudioLibrary;
using namespace std;

namespace SoundEffectTool {

	// �_��\��
	struct SOUNDEFFECTTOOL_API PointInt {
	public:
		int X, Y;
		PointInt() : X(0), Y(0) {}
		PointInt(int x, int y) : X(x), Y(y) {}
		PointInt operator+(PointInt p) {
			return PointInt(X + p.X, Y + p.Y);
		}
		PointInt operator-(PointInt p) {
			return PointInt(X - p.X, Y - p.Y);
		}
	};

	// ����\��
	struct SOUNDEFFECTTOOL_API LineInt {
	public:
		PointInt From, To;
		LineInt() : From(PointInt()), To(PointInt()) {}
		LineInt(int x1, int y1, int x2, int y2) : From(PointInt(x1, y1)), To(PointInt(x2, y2)) {}
		LineInt(PointInt from, PointInt to) : From(from), To(to) {}
	};

	// ��ɔg�`��\������N���X
	class SOUNDEFFECTTOOL_API Renderer {

		bool _isInitialized = false;
		int _maxWidth, _maxHeight;		// �o�͂�����ʂ̍ő�T�C�Y
		int _width, _height;

		uint32_t _waveLength = 0;
		uint32_t _waveWidth = 1024;
		uint32_t _waveHeight = 100;
		shared_ptr<AudioData> _audioData = nullptr;
		vector<vector<PointInt>> _waveDrawingPoints;

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

		// �s�N�Z���̈ʒu�ƍ�������A�g�`�̕`������O�v�Z������(sampleStart, sampleLength�̓`�����l���{�̒l�ɂȂ�)
		void CalcRenderingData(uint32_t pixelWidth, uint32_t pixelHeight, uint32_t sampleStart, uint32_t sampleLength);

		// �`��̃T�C�Y��ύX����
		void ChangeDrawSize(int width, int height);

		// �`����X�V����
		void DrawWave() const;

	};
}
