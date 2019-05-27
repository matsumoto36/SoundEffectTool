#include "stdafx.h"
#include "../include/Renderer.h"
#include "DxLib.h"

using namespace DxLib;

namespace SoundEffectTool {

	Renderer::Renderer() = default;
	Renderer::~Renderer() = default;

	void Renderer::Initialize(HWND hwnd, PointInt size) {

		// Dx���C�u�����������ݒ�
		SetUseDirect3DVersion(DX_DIRECT3D_9EX);				// DirectX 9EX�ɂ���
		SetUserWindow(hwnd);								// �`��E�B���h�E�̐ݒ�
		ChangeWindowMode(TRUE);								// �E�B���h�E���[�h�ɐݒ�
		SetWindowSizeChangeEnableFlag(TRUE);				// �E�B���h�E�T�C�Y���ς�

		GetDefaultState(&_maxSize.X, &_maxSize.Y, NULL);	// �f�X�N�g�b�v�̃E�B���h�E�T�C�Y���擾
		SetGraphMode(_maxSize.X, _maxSize.Y, 32);			// �O���t�B�b�N���[�h�̐ݒ�(�ő�)

		SetAlwaysRunFlag(TRUE);								// ��A�N�e�B�u�����������s
		SetDrawScreen(DX_SCREEN_BACK);						// �`�����o�b�N�o�b�t�@�֐ݒ�
		SetUseFPUPreserveFlag(TRUE);						// FPU�̐��x�𗎂Ƃ��Ȃ�
		SetWaitVSyncFlag(FALSE);							// VSync�����𖳌�
		SetOutApplicationLogValidFlag(FALSE);				// ���O�o�͒�~
		SetDoubleStartValidFlag(TRUE);						// ���d�N��������
		SetUseIMEFlag(TRUE);								// IME��L��
			
		SetDrawValidGraphCreateFlag(TRUE);					// �`���ɐݒ�ł���摜���쐬�ł���

		// Dx���C�u�����̏�����
		if (DxLib_Init() == -1) return;

		// �E�B���h�E�T�C�Y�ύX
		SetWindowSize(size.X, size.Y);

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

	void Renderer::CalcRenderingData(PointInt pixelSize, uint32_t sampleStart, uint32_t sampleLength, uint32_t toSamplingRate) {

		if (!_audioData) return;
		if (pixelSize.X == 0 || pixelSize.Y == 0) return;

		auto channlCount = _audioData->GetChannelCount();
		auto wavePoints = vector<vector<PointInt>>(channlCount);

		// �`��̃T�C�Y���v�Z(WPF��Image�̃T�C�Y�Ɏg�p�����)
		_drawSize = PointInt(pixelSize.X, _drawMarginY * 2 + (pixelSize.Y + _waveMarginY) * channlCount - _waveMarginY);

		auto start = sampleStart * channlCount;
		auto length = sampleLength * channlCount;
		auto maxHeight = 0;

		// �T���v���f�[�^���擾
		auto sampleData = new int[length];
		_audioData->ReadSamples(start, length, &sampleData);

		// �ő�l�����߂�
		for (size_t i = 0; i < length; i++) {
			auto sample = sampleData[i];
			if (abs(sample) > maxHeight) {
				maxHeight = sample;
			}
		}

		// �f�[�^���Ԉ������Ēǉ� 44100 => toSamplingRate
		auto samplesPerSec = _audioData->GetFormat().nSamplesPerSec;
		auto sampleMag = float(samplesPerSec / toSamplingRate);
		auto dataCount = size_t(sampleLength / sampleMag);

		// �T�C�Y���m��
		for (size_t i = 0; i < (size_t)channlCount; i++) {
			wavePoints[i].resize(dataCount);
		};

		auto deltaI = (float)1 / dataCount;
		for (size_t i = 0; i < dataCount; i++) {

			auto base = size_t(i * sampleMag);
			for (size_t j = 0; j < (size_t)channlCount; j++) {
				auto sample = sampleData[base * channlCount + j];

				// �����𒼂��Ēǉ�����
				auto drawData = (float)sample / maxHeight * (pixelSize.Y / 2.0f);
				wavePoints[j][i] = PointInt(int(pixelSize.X * i * deltaI), (int)drawData);
			}
		}

		delete[] sampleData;

		// ���炩���ߕ`�悷��
		if (_waveRenderHandler != -1) DeleteGraph(_waveRenderHandler);
		_waveRenderHandler = MakeGraph(_drawSize.X, _drawSize.Y);
		DrawWave(wavePoints, pixelSize);
	}

	void Renderer::ChangeDrawSize(PointInt size) {
		SetWindowSize(size.X, size.Y);
	}

	void Renderer::DrawWave(vector<vector<PointInt>> wavePoints, PointInt pixelSize) const {

		// �^�[�Q�b�g���w��
		SetDrawScreen(_waveRenderHandler);

		//��ʂ�����
		ClearDrawScreen();

		const auto waveColor = GetColor(0, 0, 255);
		const auto waveBackGroundColor = GetColor(220, 220, 220);

		const auto zeroLineColor = GetColor(32, 32, 32);

		auto waveSizeYHalf = pixelSize.Y / 2;
		auto offsetX = 0;
		auto offsetY = _drawMarginY + waveSizeYHalf;
		for (auto&& channel : wavePoints) {
			
			// �g�`�̒��S������
			DrawLine(0, offsetY, pixelSize.X, offsetY, zeroLineColor);

			// �g�`�̔w�i��`��
			DrawFillBox(0, offsetY - waveSizeYHalf, pixelSize.X, offsetY + waveSizeYHalf, waveBackGroundColor);

			// �_�̃��X�g��`�悷��
			auto prevSample = channel[0];
			for (size_t i = 1; i < channel.size(); i++) {
				auto currentSample = channel[i];
				DrawLine(offsetX + prevSample.X, offsetY + prevSample.Y, offsetX + currentSample.X, offsetY + currentSample.Y, waveColor);
				prevSample = currentSample;
			}

			offsetY += pixelSize.Y + _waveMarginY;
		}
	}

	void Renderer::Draw(PointInt waveOffset, float playRatio) const {

		// �^�[�Q�b�g���w��
		SetDrawScreen(DX_SCREEN_BACK);

		//��ʂ�����
		ClearDrawScreen();

		// �g�`��`��
		DrawGraph(waveOffset.X, waveOffset.Y, _waveRenderHandler, FALSE);
		
		const auto playRatioColor = GetColor(4, 4, 4);

		// �Đ��ʒu�ɏc��������
		auto positionX = int(_drawSize.X * playRatio + waveOffset.X);
		DrawLine(positionX, 0, positionX, _drawSize.Y, playRatioColor);

		ScreenFlip();
	}

}