#include "stdafx.h"
#include "AudioPlayer.h"

namespace AudioLibrary {

	AudioPlayer::AudioPlayer(const unique_ptr<IXAudio2>& xAudio2) :
		_xAudio2(*xAudio2),
		_sourceVoice(nullptr),
		_audioData(nullptr) {
	}

	AudioPlayer::~AudioPlayer() {
	
		// 再生中なら止める
		Stop();
	
		// リソースの開放
		UnSetAudioData();
	}

	HRESULT AudioPlayer::SetAudioData(const shared_ptr<AudioData>& audioData) {

		// ソースボイスがnull出ない場合は先に開放しておく
		if (_sourceVoice != nullptr)
			UnSetAudioData();

		_audioData = audioData;

		// ソースボイスの作成
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

		//ソースボイスの削除
		_sourceVoice.reset();

		//音声データを参照しない
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

		// バッファの設定
		auto hr = S_OK;
		if (FAILED(hr = _sourceVoice->SubmitSourceBuffer(&_audioData->GetBuffer()))) {
			wprintf(L"Error %#X submitting source buffer\n", hr);
			return hr;
		}

		// 再生
		if (FAILED(hr = _sourceVoice->Start(0))) {
			wprintf(L"Error %#X failed play audio\n", hr);
			return hr;
		}

		return hr;
	}

	// 音を止める
	HRESULT AudioPlayer::Stop() {

		// 止める
		auto hr = S_OK;
		//if (FAILED(hr = _sourceVoice->Stop())) {
		//	wprintf(L"Error %#X failed play audio\n", hr);
		//	return;
		//}

		return hr;
	}

	// 音をポーズする
	HRESULT AudioPlayer::Pause() {

		// ポーズ
		auto hr = S_OK;
		//if (FAILED(hr = _sourceVoice->Stop())) {
		//	wprintf(L"Error %#X failed play audio\n", hr);
		//	return;
		//}

		return hr;
	}

}