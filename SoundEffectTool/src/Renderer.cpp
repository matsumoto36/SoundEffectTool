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

	void Renderer::CalcRenderingData(PointInt pixelSize, uint32_t sampleStart, uint32_t sampleLength) {

		if (!_audioData) return;
		if (pixelSize.X == 0 || pixelSize.Y == 0) return;

		auto channlCount = _audioData->GetChannelCount();
		auto pointList = vector<vector<vector<int>>>();	// �`�����l���E�s�N�Z���ʒu���Ƃ̓_�̃��X�g
		_waveDrawingPoints.clear();
		_waveDrawingPoints.resize(channlCount);
		pointList.resize(channlCount);

		_waveSize = pixelSize;
		_drawSize = PointInt(_waveSize.X, _drawMarginY * 2 + (_waveSize.Y + _waveMarginY) * channlCount - _waveMarginY);

		auto start = sampleStart * channlCount;
		auto length = sampleLength * channlCount;
		auto maxHeight = 0;

		// �T���v���f�[�^���擾
		auto sampleData = new int[length];
		_audioData->ReadSamples(start, length, &sampleData);

		// �ő�l�����߂�
		for (size_t i = 0; i < length; i++) {
			auto sample = abs(sampleData[i]);
			if (sample > maxHeight) {
				maxHeight = sample;
			}
		}

		// �T�C�Y���m��
		for (size_t i = 0; i < (size_t)channlCount; i++) {
			_waveDrawingPoints[i].clear();
			pointList[i].resize(pixelSize.X);
		};

		auto delta = (float)pixelSize.X / sampleLength;
		// �s�N�Z���ʒu���Ƃ̍����̃��X�g�ɕϊ�
		for (size_t i = 0; i < sampleLength; i++) {
			// ���̃s�N�Z���ʒu���v�Z
			auto x = (size_t)(delta * i);
			for (size_t j = 0; j < (size_t)channlCount; j++) {
				auto sample = sampleData[i * channlCount + j];
				pointList[j][x].push_back(sample);
			}
		}

		delete[] sampleData;

		// ���k���Ēǉ�����
		for (size_t i = 0; i < (size_t)pixelSize.X; i++) {
			for (size_t j = 0; j < (size_t)channlCount; j++) {
				auto dataCount = pointList[j][i].size();
				if (dataCount == 0U) continue;	// �Ȃ��ꍇ�͒ǉ����Ȃ�
				if (dataCount == 1U) {
					// �����𒼂��Ēǉ�����
					auto drawData = (float)pointList[j][i][0] / maxHeight * (pixelSize.Y / 2.0f);
					_waveDrawingPoints[j].push_back(PointInt(i, (int)drawData));
				}
				else {
					// �ŏ��ƍő���擾
					auto sampleMinMax = PointInt(pointList[j][i][0], pointList[j][i][0]);
					for (size_t k = 1; k < dataCount; k++) {
						auto p = pointList[j][i][k];
						if (p < sampleMinMax.X) sampleMinMax.X = p;
						if (p > sampleMinMax.Y) sampleMinMax.Y = p;
					}

					// �����𒼂��Ēǉ�����
					auto drawData = (float)sampleMinMax.X / maxHeight * (pixelSize.Y / 2.0f);
					_waveDrawingPoints[j].push_back(PointInt(i, (int)drawData));
					drawData = (float)sampleMinMax.Y / maxHeight * (pixelSize.Y / 2.0f);
					_waveDrawingPoints[j].push_back(PointInt(i, (int)drawData));
				}
			}

		}
		
	}

	void Renderer::ChangeDrawSize(PointInt size) {
		SetWindowSize(size.X, size.Y);
	}

	void Renderer::DrawWave(float playRatio) const {

		//��ʂ�����
		ClearDrawScreen();

		const auto waveColor = GetColor(0, 0, 255);
		const auto waveBackGroundColor = GetColor(220, 220, 220);

		const auto zeroLineColor = GetColor(32, 32, 32);
		const auto playRatioColor = GetColor(4, 4, 4);

		auto waveSizeYHalf = _waveSize.Y / 2;
		auto offsetX = 0;
		auto offsetY = _drawMarginY + waveSizeYHalf;
		for (auto&& channel : _waveDrawingPoints) {
			
			// �g�`�̒��S������
			DrawLine(0, offsetY, _waveSize.X, offsetY, zeroLineColor);

			// �g�`�̔w�i��`��
			DrawFillBox(0, offsetY - waveSizeYHalf, _waveSize.X, offsetY + waveSizeYHalf, waveBackGroundColor);

			// �_�̃��X�g��`�悷��
			auto prevSample = channel[0];
			for (size_t i = 1; i < channel.size(); i++) {
				auto currentSample = channel[i];
				DrawLine(offsetX + prevSample.X, offsetY + prevSample.Y, offsetX + currentSample.X, offsetY + currentSample.Y, waveColor);
				prevSample = currentSample;
			}

			offsetY += _waveSize.Y + _waveMarginY;
		}

		// �Đ��ʒu�ɏc��������
		auto positionX = int(_waveSize.X * playRatio + offsetX);
		DrawLine(positionX, 0, positionX, _drawSize.Y, playRatioColor);


		ScreenFlip();
	}
}