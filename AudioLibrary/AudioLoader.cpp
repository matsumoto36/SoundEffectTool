#include "stdafx.h"
#include "AudioLoader.h"

#include "WAVFileReader.h"

namespace AudioLibrary {

	HRESULT AudioLoader::LoadWaveFile(const unique_ptr<IXAudio2>& xAudio2, wstring& filePath, unique_ptr<AudioData>& retAudioData) {

		auto hr = S_OK;

		// フルパスに変換
		wstring fullPath;
		if (!ToFullPath(filePath, fullPath)) {
			wprintf(L"Failed to find media file: %s\n", filePath.c_str());
			return hr;
		}

		// ファイル読み込み・パース
		auto waveFile = make_unique<uint8_t[]>(0);
		DirectX::WAVData waveData;

		// DirectX::LoadWAVAudioFromFileExはサンプルを拝借
		if (FAILED(hr = DirectX::LoadWAVAudioFromFileEx(fullPath.c_str(), waveFile, waveData))) {
			wprintf(L"Failed reading WAV file: %#X (%s)\n", hr, fullPath.c_str());
			return hr;
		}

		// バッファに必要な情報をセット
		XAUDIO2_BUFFER buffer = { 0 };
		// Submit the wave sample data using an XAUDIO2_BUFFER structure
		buffer.pAudioData = waveData.startAudio;
		buffer.Flags = XAUDIO2_END_OF_STREAM;  // tell the source voice not to expect any data after this buffer
		buffer.AudioBytes = waveData.audioBytes;

		if (waveData.loopLength > 0) {
			buffer.LoopBegin = waveData.loopStart;
			buffer.LoopLength = waveData.loopLength;
			buffer.LoopCount = 1; // We'll just assume we play the loop twice
		}

		// 返却
		auto wfx = unique_ptr<WAVEFORMATEX>(const_cast<WAVEFORMATEX*>(waveData.wfx));
		auto audioData = make_unique<AudioData>(move(wfx), move(waveFile), buffer);
		retAudioData = move(audioData);

		return hr;
	}

	bool AudioLoader::ToFullPath(wstring& filePath, wstring& retFullPath) {
	
		wchar_t fullPathBuf[MAX_PATH] = { '\0' };
		wchar_t* fullPathPart = nullptr;
		auto d = GetFullPathNameW(filePath.c_str(), MAX_PATH, fullPathBuf, &fullPathPart);

		if (d == 0) return false;

		retFullPath = fullPathBuf;

		return true;
	}

}