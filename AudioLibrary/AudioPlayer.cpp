#include "stdafx.h"
#include "AudioPlayer.h"

namespace AudioLibrary {

	AudioPlayer::AudioPlayer(const unique_ptr<IXAudio2>& xAudio2) :
		_xAudio2(*xAudio2),
		_sourceVoice(nullptr),
		_audioData(nullptr) {
	}

	AudioPlayer::~AudioPlayer() {
	
		// �Đ����Ȃ�~�߂�
		Stop();
	
		// ���\�[�X�̊J��
		UnSetAudioData();
	}

	HRESULT AudioPlayer::SetAudioData(const shared_ptr<AudioData>& audioData) {

		// �\�[�X�{�C�X��null�o�Ȃ��ꍇ�͐�ɊJ�����Ă���
		if (_sourceVoice != nullptr)
			UnSetAudioData();

		_audioData = audioData;

		// �\�[�X�{�C�X�̍쐬
		auto hr = S_OK;
		auto sourceVoice = _sourceVoice.get();
		
		if (FAILED(hr = _xAudio2.CreateSourceVoice(&sourceVoice, &_audioData->GetFormat()))) {
			wprintf(L"Error %#X creating source voice\n", hr);
			return hr;
		}

		return hr;
	}

	void AudioPlayer::UnSetAudioData() {

		if (_sourceVoice == nullptr) return;

		//�\�[�X�{�C�X�̍폜
		_sourceVoice.reset();

		//�����f�[�^���Q�Ƃ��Ȃ�
		_audioData.reset();
	}

	bool AudioPlayer::IsPlay() {

		XAUDIO2_VOICE_STATE state;
		_sourceVoice->GetState(&state);
		return (state.BuffersQueued > 0) != 0;
	}

	HRESULT AudioPlayer::Play() {
		
		if (_audioData == nullptr) {
			wprintf(L"Not set AudioData.\n");
			return E_FAIL;
		}

		// �o�b�t�@�̐ݒ�
		auto hr = S_OK;
		if (FAILED(hr = _sourceVoice->SubmitSourceBuffer(&_audioData->GetBuffer()))) {
			wprintf(L"Error %#X submitting source buffer\n", hr);
			return hr;
		}

		// �Đ�
		if (FAILED(hr = _sourceVoice->Start(0))) {
			wprintf(L"Error %#X failed play audio\n", hr);
			return hr;
		}

		return hr;
	}

	// �����~�߂�
	HRESULT AudioPlayer::Stop() {

		// �~�߂�
		auto hr = S_OK;
		//if (FAILED(hr = _sourceVoice->Stop())) {
		//	wprintf(L"Error %#X failed play audio\n", hr);
		//	return;
		//}

		return hr;
	}

	// �����|�[�Y����
	HRESULT AudioPlayer::Pause() {

		// �|�[�Y
		auto hr = S_OK;
		//if (FAILED(hr = _sourceVoice->Stop())) {
		//	wprintf(L"Error %#X failed play audio\n", hr);
		//	return;
		//}

		return hr;
	}

}