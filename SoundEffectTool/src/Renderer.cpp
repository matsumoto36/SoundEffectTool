#include "stdafx.h"
#include "../include/Renderer.h"
#include "DxLib.h"

using namespace DxLib;

namespace SoundEffectTool {

	Renderer::Renderer() = default;
	Renderer::~Renderer() = default;

	void Renderer::Initialize(HWND hwnd, int width, int height) {

		// Dx���C�u�����������ݒ�
		SetUseDirect3DVersion(DX_DIRECT3D_9EX);				// DirectX 9EX�ɂ���
		SetUserWindow(hwnd);								// �`��E�B���h�E�̐ݒ�
		ChangeWindowMode(TRUE);								// �E�B���h�E���[�h�ɐݒ�
		SetWindowSizeChangeEnableFlag(TRUE);				// �E�B���h�E�T�C�Y���ς�

		GetDefaultState(&_maxWidth, &_maxHeight, NULL);		// �f�X�N�g�b�v�̃E�B���h�E�T�C�Y���擾
		SetGraphMode(_maxWidth, _maxHeight, 32);			// �O���t�B�b�N���[�h�̐ݒ�(�ő�)

		SetAlwaysRunFlag(TRUE);								// ��A�N�e�B�u�����������s
		SetDrawScreen(DX_SCREEN_BACK);						// �`�����o�b�N�o�b�t�@�֐ݒ�
		SetUseFPUPreserveFlag(TRUE);						// FPU�̐��x�𗎂Ƃ��Ȃ�
		SetWaitVSyncFlag(FALSE);							// VSync�����𖳌�
		SetOutApplicationLogValidFlag(FALSE);				// ���O�o�͒�~
		SetDoubleStartValidFlag(TRUE);						// ���d�N��������
		SetUseIMEFlag(TRUE);								// IME��L��

		// Dx���C�u�����̏�����
		if (DxLib_Init() == -1) return;

		// �E�B���h�E�T�C�Y�ύX
		SetWindowSize(width, height);

		// �w�i�F�̐ݒ�
		SetBackgroundColor(32, 32, 32);
	}

	void Renderer::Finalize() {

		// Dx���C�u�����̏I������
		DxLib_End();
	}

	const void* Renderer::GetBackBuffer() const {
		// D3DImage�ɕ`�悷�邽�߂�
		// �o�b�N�o�b�t�@�ւ̃|�C���^���擾�ł���悤�ɂ���
		return GetUseDirect3D9BackBufferSurface();
	}

	void Renderer::SetAudioData(const shared_ptr<AudioData> audioData) {
		//const unique_ptr<uint8_t[]>& waveData, UINT32 length, int channels
		// �Q��
		_audioData = audioData;

		// �f�t�H���g�̐ݒ�
		//SetRenderingData(1024, 100, 0, length / channels);
		SetRenderingData(1024, 100, 0, 2048);
	}

	void Renderer::SetRenderingData(int waveWidth, int waveHeight, UINT32 start, UINT32 end) {

		_waveDrawingData.clear();
		_waveDrawingData.resize(_wavePerChannel.size());
		_waveWidth = waveWidth;
		_waveHeight = waveHeight;
		_waveLength = end - start;


		_wavePerChannel.clear();
		_wavePerChannel.resize(channels);

		_waveLength = length;

		// �`�����l�����Ƃ̈ʒu�i�[�p
		int* currents = new int[channels];

		for (size_t i = 0; i < (size_t)channels; i++) {
			// ����؂��͂�
			_wavePerChannel[i] = make_unique<uint8_t[]>(length / channels);
			currents[i] = 0;
		}

		UINT32 position = 0;
		auto finished = false;
		_waveMax = 255;

		// �g�`���`�����l�����Ƃɕ����ăR�s�[
		while (!finished) {
			for (size_t i = 0; i < _wavePerChannel.size(); i++) {

				auto wave = waveData[position];
				//if (wave > _waveMax) _waveMax = wave;

				// �䗦�ŃR�s�[
				_wavePerChannel[i][currents[i]++] = wave;

				if (length < ++position) {
					finished = true;
					break;
				}
			}
		}

		delete[] currents;

		
		for (size_t i = 0; i < _waveDrawingData.size(); i++) {

			//_waveDrawingData[i] = make_unique<uint8_t[]>(waveWidth);
			_waveDrawingData[i].resize(waveWidth);

			for (size_t j = 0; j < waveWidth; j++) {
				size_t position = start + (float)j / waveWidth * _waveLength;
				auto sample = (float)_wavePerChannel[i][position] / _waveMax * waveHeight;
				_waveDrawingData[i][j] = sample;
			}
		}
	}

	void Renderer::ChangeDrawSize(int width, int height) {
		SetWindowSize(width, height);
		_width = width;
		_height = height;
	}

	void Renderer::DrawWave() const {

		//��ʂ�����
		ClearDrawScreen();

		if (_waveLength <= 0) return;

		auto waveColor = GetColor(0, 0, 255);
		auto margin = 20;

		auto y = 0;
		for (auto&& channel : _waveDrawingData) {
			
			auto prev = channel[0] - 128;
			auto x = 0;

			for (size_t i = 1; i < _waveWidth; i++) {
				auto s = channel[i] - 128;
				DrawLine(x + i - 1, y + prev, x + i, y + s, waveColor);
				prev = s;
			}

			y += margin + _waveHeight;
		}

		ScreenFlip();
	}
}