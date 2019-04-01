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
		IXAudio2& _xAudio2;								// XAudio2
		IXAudio2SourceVoice* _sourceVoice;				// �\�[�X�{�C�X
		unique_ptr<IXAudio2VoiceCallback> _callBack;	// �\�[�X�{�C�X�̃R�[���o�b�N
		XAUDIO2_VOICE_STATE state;						// �Đ����̃V�X�e���̏��
		shared_ptr<AudioData> _audioData;				// �Đ����鉹���f�[�^

		Impl(IXAudio2& xAudio2) :
			_xAudio2(xAudio2),
			_sourceVoice(nullptr),
			_callBack(nullptr),
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
		if (FAILED(hr = _impl->_xAudio2.CreateSourceVoice(&_impl->_sourceVoice, &audioData->GetFormat()))) {
			wprintf(L"Error %#X creating source voice\n", hr);
			return hr;
		}

		return hr;
	}

	void AudioPlayer::UnSetAudioData() {

		if (_impl->_sourceVoice == nullptr) return;

		//�\�[�X�{�C�X�̍폜
		_impl->_sourceVoice->DestroyVoice();

		//�����f�[�^���Q�Ƃ��Ȃ�
		_impl->_audioData = nullptr;
	}

	HRESULT AudioPlayer::Play() {
		
		if (_impl->_sourceVoice == nullptr) return E_FAIL;
		if (_impl->_audioData == nullptr) {
			wprintf(L"Not set AudioData.\n");
			return E_FAIL;
		}

		Update();

		if (_isPlay) return E_FAIL;

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

		_isPlay = true;

		return hr;
	}

	HRESULT AudioPlayer::Stop() {

		if (_impl->_sourceVoice == nullptr) return E_FAIL;
		if (_impl->_audioData == nullptr) {
			wprintf(L"Not set AudioData.\n");
			return E_FAIL;
		}

		Update();

		if (!_isPlay) return E_FAIL;

		// �~�߂�
		auto hr = S_OK;
		if (FAILED(hr = _impl->_sourceVoice->Stop())) {
			wprintf(L"Error %#X failed stop audio\n", hr);
			return hr;
		}

		_isPlay = false;

		return hr;
	}

	HRESULT AudioPlayer::Pause() {

		if (_impl->_sourceVoice == nullptr) return E_FAIL;
		if (_impl->_audioData == nullptr) {
			wprintf(L"Not set AudioData.\n");
			return E_FAIL;
		}

		Update();

		if (!_isPlay) return E_FAIL;

		// �~�߂�
		auto hr = S_OK;
		if (FAILED(hr = _impl->_sourceVoice->Stop())) {
			wprintf(L"Error %#X failed stop audio\n", hr);
			return hr;
		}

		_isPlay = false;

		return hr;
	}

}