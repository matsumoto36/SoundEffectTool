#include "stdafx.h"
#include "Renderer.h"
#include "DxLib.h"


namespace SoundEffectTool {

	using namespace DxLib;

	Renderer::Renderer() = default;
	Renderer::~Renderer() = default;

	void Renderer::Initialize(HWND hwnd, int width, int height) {

		// Dx���C�u�����������ݒ�
		SetUseDirect3DVersion(DX_DIRECT3D_9EX);				// DirectX 9EX�ɂ���
		SetUserWindow(hwnd);								// �`��E�B���h�E�̐ݒ�
		ChangeWindowMode(TRUE);								// �E�B���h�E���[�h�ɐݒ�
		SetWindowSizeChangeEnableFlag(TRUE);				// �E�B���h�E�T�C�Y���ς�

		GetDefaultState(&_viewWidth, &_viewHeight, NULL);	// �f�X�N�g�b�v�̃E�B���h�E�T�C�Y���擾
		SetGraphMode(_viewWidth, _viewHeight, 32);			// �O���t�B�b�N���[�h�̐ݒ�(�ő�)

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
		SetBackgroundColor(0, 0, 0);
	}

	void Renderer::Finalize() {

		// Dx���C�u�����̏I������
		DxLib_End();
	}

	const void* Renderer::GetBackBuffer() const {
		// D3DImage�ɕ`�悷�邽�߂�
		// �o�b�N�o�b�t�@�ւ̃|�C���^���擾�ł���悤�ɂ���
		return  GetUseDirect3D9BackBufferSurface();
	}

	void Renderer::ChangeDrawSize(int width, int height) const {
		SetWindowSize(width, height);
	}
}