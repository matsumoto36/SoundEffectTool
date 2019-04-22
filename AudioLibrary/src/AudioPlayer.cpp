#include "stdafx.h"
#include "../include/AudioPlayer.h"

namespace {

	// 0~1�̃{�����[������f�V�x���̔�ɕϊ�
	float ToDecibelRatio(float volume) {
		if (volume == 0) return 0;

		return XAudio2DecibelsToAmplitudeRatio(1 - volume * -40.0f);
	}
}

namespace AudioLibrary {

	// �\�[�X�{�C�X�̃R�[���o�b�N
	class AudioPlayer::VoiceCallback : public IXAudio2VoiceCallback {
	public:
		HANDLE Event;
		const AudioPlayer& Player;	// �]����̃C���X�^���X

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
		IXAudio2SourceVoice* _sourceVoice;				// �\�[�X�{�C�X
		unique_ptr<IXAudio2VoiceCallback> _callback;	// �\�[�X�{�C�X�̃R�[���o�b�N
		shared_ptr<AudioData> _audioData;				// �Đ����鉹���f�[�^
		
		const float FadeTime = 0.1f;					// �����t�F�[�h�̎���
		float _xaudio2Volume;							// �ڍׂȉ��� �����t�F�[�h�̐���p

		// XAudio2�̃R�[���o�b�N�����b�v����
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

	// XAudio2�̃R�[���o�b�N��AudioPlayer�ɓ]������
	#pragma region WrappingCallback
	void STDMETHODCALLTYPE AudioPlayer::VoiceCallback::OnStreamEnd() {
		if (!Player._impl->OnXAudio2StreamEnd) return;
		Player._impl->OnXAudio2StreamEnd();
	}
	void STDMETHODCALLTYPE AudioPlayer::VoiceCallback::OnVoiceProcessingPassEnd() { 
		if (!Player._impl->OnXAudio2VoiceProcessingPassEnd) return;
		Player._impl->OnXAudio2VoiceProcessingPassEnd();
	}
	void STDMETHODCALLTYPE AudioPlayer::VoiceCallback::OnVoiceProcessingPassStart(UINT32 bytesRequired) {
		if (!Player._impl->OnXAudio2VoiceProcessingPassStart) return;
		Player._impl->OnXAudio2VoiceProcessingPassStart(bytesRequired);
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
	
		// XAudio2�̃R�[���o�b�N�̓����ݒ�
		_impl->OnXAudio2VoiceProcessingPassStart = [&](UINT32 requredBytes) {

			//if (FAILED(CheckValidData())) return;

			// *�^���I�ɃX�g���[�~���O�d�l�ɂ���ƃm�C�Y������*
			if (requredBytes == 0) return;
			AddBuffer(requredBytes);

		};

		_impl->OnXAudio2BufferEnd = [&](void * pBufferContext) {
			// ����̓o�b�t�@�̏I�����Đ��̏I��
			//XAUDIO2_VOICE_STATE state;
			//_impl->_sourceVoice->GetState(&state);
			//if(state.BuffersQueued < 3)
			//	AddBuffer(1764);
		};
	}

	AudioPlayer::~AudioPlayer() { }

	void AudioPlayer::SetVolume(float volume) {
		// 0~1�Ɏ��߂�
		if (volume > 1.0f) volume = 1.0f;
		else if (volume < 0.0f) volume = 0.0f;

		// �����l�̓Z�b�g���Ȃ�
		if (_volume == volume) return;

		_volume = volume;

		if (_impl->_sourceVoice == nullptr) return;
		SetFade(_volume, _impl->FadeTime);

		// �ʒm
		if (OnVolumeChanged) OnVolumeChanged(_volume);
	}

	const shared_ptr<AudioData>& AudioPlayer::GetAudioData() const {
		return _impl->_audioData;
	}

	HRESULT AudioPlayer::SetAudioData(const shared_ptr<AudioData>& audioData) {

		// �\�[�X�{�C�X��null�o�Ȃ��ꍇ�͐�ɊJ�����Ă���
		if (_impl->_sourceVoice != nullptr)
			UnSetAudioData();

		_impl->_audioData = audioData;
		// �\�[�X�{�C�X�̍쐬
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

		//�\�[�X�{�C�X�̍폜
		_impl->_sourceVoice->DestroyVoice();

		//�����f�[�^���Q�Ƃ��Ȃ�
		_impl->_audioData = nullptr;
	}

	void AudioPlayer::Update(float deltaTime) {
		if (_impl->_sourceVoice == nullptr) return;

		// �t�F�[�h�X�V
		UpdateFade(deltaTime);

		//XAUDIO2_VOICE_STATE state;
		//_impl->_sourceVoice->GetState(&state);

		// �Đ��ʒu�̌v�Z
		auto blockAlign = _impl->_audioData->GetFormat().nBlockAlign;
		_position = _seekData / blockAlign;

	}

	HRESULT AudioPlayer::Play() {
		
		auto hr = S_OK;
		if (FAILED(hr = CheckValidData())) return hr;

		switch (_status) {
			case AudioPlayerStatus::Play:
				return E_FAIL;
			case AudioPlayerStatus::Stop:
				// ��~��Ԃ���̓o�b�t�@�̕⋋���K�v
				//auto buffer = _impl->_audioData->GetBuffer();
				//if (FAILED(hr = _impl->_sourceVoice->SubmitSourceBuffer(&buffer))) {
				//	wprintf(L"Error %#X submitting source buffer\n", hr);
				//	return hr;
				//}
				_seekData = 0;
				//AddBuffer(1764000);
				break;
			case AudioPlayerStatus::Pause:
				/* �|�[�Y����̕��A�̓o�b�t�@�͊��ɑ��݂��Ă��� */
				break;
			default:
				break;
		}

		// ���ʂ̐ݒ�
		SetFade(_volume, _impl->FadeTime);

		// �Đ�
		if (FAILED(hr = _impl->_sourceVoice->Start(0))) {
			wprintf(L"Error %#X failed play audio\n", hr);
			return hr;
		}

		SetPlayerStatus(AudioPlayerStatus::Play);

		return hr;
	}

	HRESULT AudioPlayer::PlayAtPosition(UINT32 samples) {

		Stop();

		auto hr = S_OK;
		if (FAILED(hr = CheckValidData())) return hr;

		// �w�肵���ʒu����̃o�b�t�@���w��
		//auto buffer = _impl->_audioData->GetBuffer();
		//buffer.PlayBegin = samples;
		//hr = _impl->_sourceVoice->SubmitSourceBuffer(&buffer);
		//if (FAILED(hr)) {
		//	wprintf(L"Error %#X submitting source buffer\n", hr);
		//	return hr;
		//}

		_position = samples;
		auto blockAlign = _impl->_audioData->GetFormat().nBlockAlign;
		_seekData = _position * blockAlign;

		// ���ʂ̐ݒ�
		SetFade(_volume, _impl->FadeTime);


		// �Đ�
		//AddBuffer(1764);
		if (FAILED(hr = _impl->_sourceVoice->Start(0))) {
			wprintf(L"Error %#X failed play audio\n", hr);
			return hr;
		}

		return hr;
	}

	HRESULT AudioPlayer::Stop() {

		auto hr = S_OK;
		if (FAILED(hr = CheckValidData())) return hr;

		switch (_status) {
			case AudioPlayerStatus::Play:
				break;
			case AudioPlayerStatus::Stop:
				break;
			case AudioPlayerStatus::Pause:
				break;
			default:
				break;
		}

		//SetFade(0, _impl->FadeTime, [&]() {

			if (FAILED(hr = _impl->_sourceVoice->Stop())) {
				wprintf(L"Error %#X failed stop audio\n", hr);
			}

			_seekData = 0;

			// �~�߂čŏ�����Đ��������ꍇ��FlushSourceBuffers�����s����
			_impl->_sourceVoice->FlushSourceBuffers();
		
		//});
		SetPlayerStatus(AudioPlayerStatus::Stop);


		return hr;
	}

	HRESULT AudioPlayer::Pause() {

		auto hr = S_OK;
		if (FAILED(hr = CheckValidData())) return hr;

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

		// Stop���ꎞ��~�ɂȂ�
		//SetFade(0, _impl->FadeTime, [&]() {

			if (FAILED(hr = _impl->_sourceVoice->Stop())) {
				wprintf(L"Error %#X failed stop audio\n", hr);
				return hr;
			}

		//});
		SetPlayerStatus(AudioPlayerStatus::Pause);

		return hr;
	}

	void AudioPlayer::SetPlayerStatus(AudioPlayerStatus status) {

		if (status == _status) return;

		auto oldIsPlay = IsPlay();
		_status = status;
		auto newIsPlay = IsPlay();

		// Play�ύX�ʒm
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

	void AudioPlayer::AddBuffer(UINT32 requredBytes) {

		auto&& p = _impl->_audioData->GetBuffer().pAudioData;
		XAUDIO2_BUFFER buf = { 0 };
		buf.pAudioData = &p[_seekData];
		volatile auto a = p[_seekData];

		// ���N�G�X�g�ɑ΂��đ��݂��Ă���T���v�����v�Z
		auto length = _impl->_audioData->GetBuffer().AudioBytes;
		if (length >= _seekData + requredBytes) {
			buf.AudioBytes = requredBytes;
			_seekData += requredBytes;
		}
		else {
			auto bytes = length - _seekData;
			if (bytes <= 0) {
				Stop();
				return;
			}
			buf.AudioBytes = bytes;
			// �X�g���[�~���O�Đ��I���t���O
			buf.Flags = XAUDIO2_END_OF_STREAM;
			_seekData += bytes;
		}

		_impl->_sourceVoice->SubmitSourceBuffer(&buf);
	}

	void AudioPlayer::SetFade(float targetVolume, float targetTime, function<void()> callback) {
		_fadeStartVolume = _impl->_xaudio2Volume;
		_targetVolume = targetVolume;
		_targetTime = targetTime;
		_fadeCallback = callback;
		_fading = 0;
	}

	void AudioPlayer::UpdateFade(float deltaTime) {
		if (_fading >= _targetTime) return;
		
		_fading += deltaTime;

		// �t�F�[�h����
		if (_fading >= _targetTime) {
			_fading = _targetTime;
			_impl->_sourceVoice->SetVolume(ToDecibelRatio(_impl->_xaudio2Volume = _targetVolume));
			if (_fadeCallback) _fadeCallback();
		}
		else {
			_impl->_xaudio2Volume = _fadeStartVolume + (_targetVolume - _fadeStartVolume) * (_fading / _targetTime);
			_impl->_sourceVoice->SetVolume(ToDecibelRatio(_impl->_xaudio2Volume));
		}

	}
}