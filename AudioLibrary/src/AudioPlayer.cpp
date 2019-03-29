#include "stdafx.h"
#include "../include/AudioPlayer.h"

namespace {

	// 0~1のボリュームからデシベルの比に変換
	float ToDecibelRatio(float volume) {
		if (volume == 0) return 0;

		return XAudio2DecibelsToAmplitudeRatio(volume * -40.0f);
	}
}

namespace AudioLibrary {

	struct AudioPlayer::Impl {
		IXAudio2& _xAudio2;					// XAudio2
		IXAudio2SourceVoice* _sourceVoice;	// ソースボイス
		shared_ptr<AudioData> _audioData;	// 再生する音声データ
		XAUDIO2_VOICE_STATE state;			// 再生中のシステムの情報

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
		// 0~1に収める
		if (volume > 1.0f) volume = 1.0f;
		else if (volume < 0.0f) volume = 0.0f;

		// セット
		if (_volume != volume) return;

		_volume = volume;

		if (_impl->_sourceVoice == nullptr) return;
		_impl->_sourceVoice->SetVolume(ToDecibelRatio(_volume));
	}

	void AudioPlayer::Update() {
		if (_impl->_sourceVoice == nullptr) return;

		// Stateの取得
		XAUDIO2_VOICE_STATE state;
		_impl->_sourceVoice->GetState(&state);

		_isPlay = (state.BuffersQueued > 0) != 0;
	}

	HRESULT AudioPlayer::SetAudioData(const shared_ptr<AudioData>& audioData) {

		// ソースボイスがnull出ない場合は先に開放しておく
		if (_impl->_sourceVoice != nullptr)
			UnSetAudioData();

		_impl->_audioData = audioData;

		// ソースボイスの作成
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

		//ソースボイスの削除
		_impl->_sourceVoice->DestroyVoice();

		//音声データを参照しない
		_impl->_audioData = nullptr;
	}

	HRESULT AudioPlayer::Play() const {
		
		if (_impl->_sourceVoice == nullptr) return E_FAIL;
		if (_impl->_audioData == nullptr) {
			wprintf(L"Not set AudioData.\n");
			return E_FAIL;
		}

		// バッファの設定
		auto hr = S_OK;
		auto buffer = _impl->_audioData->GetBuffer();
		if (FAILED(hr = _impl->_sourceVoice->SubmitSourceBuffer(&buffer))) {
			wprintf(L"Error %#X submitting source buffer\n", hr);
			return hr;
		}

		// 音量の設定
		_impl->_sourceVoice->SetVolume(ToDecibelRatio(_volume));

		// 再生
		if (FAILED(hr = _impl->_sourceVoice->Start(0))) {
			wprintf(L"Error %#X failed play audio\n", hr);
			return hr;
		}

		return hr;
	}

	// 音を止める
	HRESULT AudioPlayer::Stop() const {

		if (_impl->_audioData == nullptr) {
			wprintf(L"Not set AudioData.\n");
			return E_FAIL;
		}

		// 止める
		auto hr = S_OK;
		if (FAILED(hr = _impl->_sourceVoice->Stop())) {
			wprintf(L"Error %#X failed play audio\n", hr);
			return hr;
		}

		return hr;
	}

	// 音をポーズする
	HRESULT AudioPlayer::Pause() const {

		// ポーズ
		auto hr = S_OK;
		//if (FAILED(hr = _sourceVoice->Stop())) {
		//	wprintf(L"Error %#X failed play audio\n", hr);
		//	return;
		//}

		return hr;
	}

}