#include "stdafx.h"
#include "../include/AudioPlayer.h"

namespace {

	// 0~1のボリュームからデシベルの比に変換
	float ToDecibelRatio(float volume) {
		if (volume == 0) return 0;

		return XAudio2DecibelsToAmplitudeRatio(1 - volume * -40.0f);
	}
}

namespace AudioLibrary {

	// ソースボイスのコールバック
	class AudioPlayer::VoiceCallback : public IXAudio2VoiceCallback {
	public:
		HANDLE Event;
		const AudioPlayer& Player;	// 転送先のインスタンス

		VoiceCallback(const AudioPlayer& player) :
			Event(CreateEvent(NULL, FALSE, FALSE, NULL)),
			Player(player) {}
		~VoiceCallback() { CloseHandle(Event); }

		void STDMETHODCALLTYPE OnStreamEnd();
		void STDMETHODCALLTYPE OnVoiceProcessingPassEnd();
		void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 samplesRequired);
		void STDMETHODCALLTYPE OnBufferEnd(void * pBufferContext);
		void STDMETHODCALLTYPE OnBufferStart(void * pBufferContext);
		void STDMETHODCALLTYPE OnLoopEnd(void * pBufferContext);
		void STDMETHODCALLTYPE OnVoiceError(void * pBufferContext, HRESULT error);
	};

	struct AudioPlayer::Impl {
		IXAudio2& _xAudio2;								// XAudio2
		IXAudio2SourceVoice* _sourceVoice;				// ソースボイス
		unique_ptr<IXAudio2VoiceCallback> _callback;	// ソースボイスのコールバック
		XAUDIO2_VOICE_STATE state;						// 再生中のシステムの情報
		shared_ptr<AudioData> _audioData;				// 再生する音声データ

		// XAudio2のコールバックをラップする
		function<void()> OnXAudio2StreamEnd;
		function<void()> OnXAudio2VoiceProcessingPassEnd;
		function<void(UINT32)> OnXAudio2VoiceProcessingPassStart;
		function<void(void*)> OnXAudio2BufferEnd;
		function<void(void*)> OnXAudio2BufferStart;
		function<void(void*)> OnXAudio2LoopEnd;
		function<void(void*, HRESULT)> OnXAudio2VoiceError;

		Impl(IXAudio2& xAudio2, const AudioPlayer& parent) :
			_xAudio2(xAudio2),
			_sourceVoice(nullptr),
			_callback(make_unique<VoiceCallback>(parent)),
			_audioData(nullptr) { }
	};

	// XAudio2のコールバックをAudioPlayerに転送する
	#pragma region WrappingCallback
	void STDMETHODCALLTYPE AudioPlayer::VoiceCallback::OnStreamEnd() {
		if (!Player._impl->OnXAudio2StreamEnd) return;
		Player._impl->OnXAudio2StreamEnd();
	}
	void STDMETHODCALLTYPE AudioPlayer::VoiceCallback::OnVoiceProcessingPassEnd() { 
		if (!Player._impl->OnXAudio2VoiceProcessingPassEnd) return;
		Player._impl->OnXAudio2VoiceProcessingPassEnd();
	}
	void STDMETHODCALLTYPE AudioPlayer::VoiceCallback::OnVoiceProcessingPassStart(UINT32 samplesRequired) {
		if (!Player._impl->OnXAudio2VoiceProcessingPassStart) return;
		Player._impl->OnXAudio2VoiceProcessingPassStart(samplesRequired);
	}
	void STDMETHODCALLTYPE AudioPlayer::VoiceCallback::OnBufferEnd(void * pBufferContext) {
		if (!Player._impl->OnXAudio2BufferEnd) return;
		Player._impl->OnXAudio2BufferEnd(pBufferContext);
	}
	void STDMETHODCALLTYPE AudioPlayer::VoiceCallback::OnBufferStart(void * pBufferContext) { 
		if (!Player._impl->OnXAudio2BufferStart) return;
		Player._impl->OnXAudio2BufferStart(pBufferContext);
	}
	void STDMETHODCALLTYPE AudioPlayer::VoiceCallback::OnLoopEnd(void * pBufferContext) {
		if (!Player._impl->OnXAudio2LoopEnd) return;
		Player._impl->OnXAudio2LoopEnd(pBufferContext);
	}
	void STDMETHODCALLTYPE AudioPlayer::VoiceCallback::OnVoiceError(void * pBufferContext, HRESULT error) {
		if (!Player._impl->OnXAudio2VoiceError) return;
		Player._impl->OnXAudio2VoiceError(pBufferContext, error);
	}
	#pragma endregion


	AudioPlayer::AudioPlayer(IXAudio2& xAudio2, float volume) :
		_impl(make_unique<Impl>(xAudio2, *this)),
		_status(AudioPlayerStatus::Stop),
		_volume(volume) { 
	
		// XAudio2のコールバックの動作を設定
		_impl->OnXAudio2BufferEnd = [&](void* pBufferContext) {
			// 今回はバッファの終了=再生の終了
			SetPlayerStatus(AudioPlayerStatus::Stop);
		};
	}

	AudioPlayer::~AudioPlayer() { }

	void AudioPlayer::SetVolume(float volume) {
		// 0~1に収める
		if (volume > 1.0f) volume = 1.0f;
		else if (volume < 0.0f) volume = 0.0f;

		// 同じ値はセットしない
		if (_volume == volume) return;

		_volume = volume;

		if (_impl->_sourceVoice == nullptr) return;
		_impl->_sourceVoice->SetVolume(ToDecibelRatio(_volume));

		// 通知
		if (OnVolumeChanged) OnVolumeChanged(_volume);
	}

	void AudioPlayer::Update() {
		if (_impl->_sourceVoice == nullptr) return;

		/* コールバックで処理をしているため、現在は意味がない */
	}

	HRESULT AudioPlayer::SetAudioData(const shared_ptr<AudioData>& audioData) {

		// ソースボイスがnull出ない場合は先に開放しておく
		if (_impl->_sourceVoice != nullptr)
			UnSetAudioData();

		_impl->_audioData = audioData;
		// ソースボイスの作成
		auto hr = _impl->_xAudio2.CreateSourceVoice(
			&_impl->_sourceVoice, &audioData->GetFormat(),
			0,
			XAUDIO2_DEFAULT_FREQ_RATIO,
			_impl->_callback.get());

		if (FAILED(hr)) {

			wprintf(L"Error %#X creating source voice\n", hr);
			return hr;
		}

		return hr;
	}

	void AudioPlayer::UnSetAudioData() {

		if (_impl->_sourceVoice == nullptr) return;

		//ソースボイスの削除
		_impl->_sourceVoice->DestroyVoice();

		//音声データを参照しない
		_impl->_audioData = nullptr;
	}

	HRESULT AudioPlayer::Play() {
		
		auto hr = S_OK;
		if (FAILED(hr = CheckValidData())) return hr;

		Update();

		switch (_status) {
			case AudioPlayerStatus::Play:
				return E_FAIL;
			case AudioPlayerStatus::Stop:
				// 停止状態からはバッファの補給が必要
				auto buffer = _impl->_audioData->GetBuffer();
				if (FAILED(hr = _impl->_sourceVoice->SubmitSourceBuffer(&buffer))) {
					wprintf(L"Error %#X submitting source buffer\n", hr);
					return hr;
				}
				break;
			case AudioPlayerStatus::Pause:
				/* ポーズからの復帰はバッファは既に存在している */
				break;
			default:
				break;
		}

		// 音量の設定
		_impl->_sourceVoice->SetVolume(ToDecibelRatio(_volume));

		// 再生
		if (FAILED(hr = _impl->_sourceVoice->Start(0))) {
			wprintf(L"Error %#X failed play audio\n", hr);
			return hr;
		}

		SetPlayerStatus(AudioPlayerStatus::Play);

		return hr;
	}

	HRESULT AudioPlayer::PlayAtPosition(size_t samples) {

		Stop();

		// 指定した位置からのバッファを指定
		auto buffer = _impl->_audioData->GetBuffer();
		buffer.PlayBegin = samples;
		auto hr = _impl->_sourceVoice->SubmitSourceBuffer(&buffer);
		if (FAILED(hr)) {
			wprintf(L"Error %#X submitting source buffer\n", hr);
			return hr;
		}

		return hr;
	}

	HRESULT AudioPlayer::Stop() {

		auto hr = S_OK;
		if (FAILED(hr = CheckValidData())) return hr;

		Update();

		switch (_status) {
			case AudioPlayerStatus::Play:
				break;
			case AudioPlayerStatus::Stop:
				return E_FAIL;
			case AudioPlayerStatus::Pause:
				break;
			default:
				break;
		}

		if (FAILED(hr = _impl->_sourceVoice->Stop())) {
			wprintf(L"Error %#X failed stop audio\n", hr);
			return hr;
		}

		// 止めて最初から再生したい場合はFlushSourceBuffersを実行する
		_impl->_sourceVoice->FlushSourceBuffers();
		SetPlayerStatus(AudioPlayerStatus::Stop);

		return hr;
	}

	HRESULT AudioPlayer::Pause() {

		auto hr = S_OK;
		if (FAILED(hr = CheckValidData())) return hr;

		Update();

		switch (_status) {
			case AudioPlayerStatus::Play:
				break;
			case AudioPlayerStatus::Stop:
				break;
			case AudioPlayerStatus::Pause:
				return E_FAIL;
			default:
				break;
		}

		// Stopが一時停止になる
		if (FAILED(hr = _impl->_sourceVoice->Stop())) {
			wprintf(L"Error %#X failed stop audio\n", hr);
			return hr;
		}

		SetPlayerStatus(AudioPlayerStatus::Pause);

		return hr;
	}

	void AudioPlayer::SetPlayerStatus(AudioPlayerStatus status) {

		if (status == _status) return;

		auto oldIsPlay = IsPlay();
		_status = status;
		auto newIsPlay = IsPlay();

		// Play変更通知
		if(oldIsPlay != newIsPlay && OnIsPlayChanged) OnIsPlayChanged(newIsPlay);
	}

	HRESULT AudioPlayer::CheckValidData() const {

		if (_impl->_sourceVoice == nullptr) return E_FAIL;
		if (_impl->_audioData == nullptr) {
			wprintf(L"Not set AudioData.\n");
			return E_FAIL;
		}

		return S_OK;
	}
}