#include "stdafx.h"
#include "../include/AudioPlayer.h"

namespace {

	// 0~1�̃{�����[������f�V�x���̔�ɕϊ�
	float ToDecibelRatio(float volume) {
		if (volume == 0) return 0;

		return XAudio2DecibelsToAmplitudeRatio(volume * -40.0f);
	}
}

namespace AudioLibrary {

	struct AudioPlayer::Impl {
		IXAudio2& _xAudio2;					// XAudio2
		IXAudio2SourceVoice* _sourceVoice;	// �\�[�X�{�C�X
		shared_ptr<AudioData> _audioData;	// �Đ����鉹���f�[�^
		XAUDIO2_VOICE_STATE state;			// �Đ����̃V�X�e���̏��

		Impl(IXAudio2& xAudio2) :
			_xAudio2(xAudio2),
			_sourceVoice(nullptr),
			_audioData(nullptr) {}
	};

	AudioPlayer::AudioPlayer(IXAudio2& xAudio2, float volume) :
		_impl(make_unique<Impl>(xAudio2)),
		_volume(volume),
		_isPlay(false) { }

	AudioPlayer::AudioPlayer(AudioPlayer&&) noexcept = default;

	AudioPlayer::~AudioPlayer() { }

	void AudioPlayer::SetVolume(float volume) {
		// 0~1�Ɏ��߂�
		if (volume > 1.0f) volume = 1.0f;
		else if (volume < 0.0f) volume = 0.0f;

		// �Z�b�g
		if (_volume != volume) return;

		_volume = volume;

		if (_impl->_sourceVoice == nullptr) return;
		_impl->_sourceVoice->SetVolume(ToDecibelRatio(_volume));
	}

	void AudioPlayer::Update() {
		if (_impl->_sourceVoice == nullptr) return;

		// State�̎擾
		XAUDIO2_VOICE_STATE state;
		_impl->_sourceVoice->GetState(&state);

		_isPlay = (state.BuffersQueued > 0) != 0;
	}

	HRESULT AudioPlayer::SetAudioData(const shared_ptr<AudioData>& audioData) {

		// �\�[�X�{�C�X��null�o�Ȃ��ꍇ�͐�ɊJ�����Ă���
		if (_impl->_sourceVoice != nullptr)
			UnSetAudioData();

		_impl->_audioData = audioData;

		// �\�[�X�{�C�X�̍쐬
		auto hr = S_OK;
		IXAudio2SourceVoice* sourceVoice;

		auto a = &audioData->GetFormat();
		const tWAVEFORMATEX* b = new tWAVEFORMATEX(audioData->GetFormat());
		WAVEFORMATEX c = audioData->GetFormat();
		if (FAILED(hr = _impl->_xAudio2.CreateSourceVoice(&sourceVoice, &audioData->GetFormat()))) {
			wprintf(L"Error %#X creating source voice\n", hr);
			return hr;
		}

		_impl->_sourceVoice = sourceVoice;
		return hr;
	}

	void AudioPlayer::UnSetAudioData() {

		if (_impl->_sourceVoice == nullptr) return;

		//�\�[�X�{�C�X�̍폜
		_impl->_sourceVoice->DestroyVoice();

		//�����f�[�^���Q�Ƃ��Ȃ�
		_impl->_audioData = nullptr;
	}

	HRESULT AudioPlayer::Play() const {
		
		if (_impl->_sourceVoice == nullptr) return E_FAIL;
		if (_impl->_audioData == nullptr) {
			wprintf(L"Not set AudioData.\n");
			return E_FAIL;
		}

		// �o�b�t�@�̐ݒ�
		auto hr = S_OK;
		auto buffer = _impl->_audioData->GetBuffer();
		if (FAILED(hr = _impl->_sourceVoice->SubmitSourceBuffer(&buffer))) {
			wprintf(L"Error %#X submitting source buffer\n", hr);
			return hr;
		}

		// ���ʂ̐ݒ�
		_impl->_sourceVoice->SetVolume(ToDecibelRatio(_volume));

		// �Đ�
		if (FAILED(hr = _impl->_sourceVoice->Start(0))) {
			wprintf(L"Error %#X failed play audio\n", hr);
			return hr;
		}

		return hr;
	}

	// �����~�߂�
	HRESULT AudioPlayer::Stop() const {

		if (_impl->_audioData == nullptr) {
			wprintf(L"Not set AudioData.\n");
			return E_FAIL;
		}

		// �~�߂�
		auto hr = S_OK;
		if (FAILED(hr = _impl->_sourceVoice->Stop())) {
			wprintf(L"Error %#X failed play audio\n", hr);
			return hr;
		}

		return hr;
	}

	// �����|�[�Y����
	HRESULT AudioPlayer::Pause() const {

		// �|�[�Y
		auto hr = S_OK;
		//if (FAILED(hr = _sourceVoice->Stop())) {
		//	wprintf(L"Error %#X failed play audio\n", hr);
		//	return;
		//}

		return hr;
	}

}