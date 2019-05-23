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

		return;
		// �Q��
		_audioData = audioData;

		// �f�t�H���g�̐ݒ�ŕ`��f�[�^����
		SetRenderingData(256, 100, 0, 256);
	}

	void Renderer::SetRenderingData(uint32_t waveWidth, uint32_t waveHeight, uint32_t waveStart, uint32_t waveLength) {

		return;

		_waveDrawingData.clear();
		auto channlCount = _audioData->GetChannelCount();
		_waveDrawingData.resize(channlCount);

		// �T�C�Y���m��
		for (size_t i = 0; i < channlCount; i++) {
			_waveDrawingData[i].clear();
			_waveDrawingData[i].resize(waveWidth);
		};

		_waveWidth = waveWidth;
		_waveHeight = waveHeight;
		
		if (waveWidth == 0 || waveHeight == 0) return;

		_waveMax = 255;
		auto&& waveData = _audioData->GetWave();
		auto&& l = _audioData->GetLength();
		auto start = waveStart * channlCount;
		auto delta = (float)waveLength / waveWidth * channlCount;
		for (size_t i = 0; i < waveWidth; i++) {

			auto base = start + (uint32_t)(delta * i);
			for (size_t j = 0; j < channlCount; j++) {
				// �����𒼂��ăR�s�[
				auto sample = (float)waveData[base + j] / _waveMax * waveHeight;
				_waveDrawingData[j][i] = sample;
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
		ScreenFlip();
		return;


		auto waveColor = GetColor(0, 0, 255);
		auto margin = 20;

		auto y = 0;
		for (auto&& channel : _waveDrawingData) {
			
			auto prev = channel[0];
			auto x = 0;

			for (size_t i = 1; i < _waveWidth; i++) {
				auto s = channel[i];
				DrawLine(x + i - 1, y + prev, x + i, y + s, waveColor);
				prev = s;
			}

			y += margin + _waveHeight;
		}

		ScreenFlip();
	}
}