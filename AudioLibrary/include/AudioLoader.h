#pragma once

#include <string>
#include <memory>

#include "AudioDefine.h"
#include "AudioData.h"
#include "../src/WAVFileReader.h"

namespace AudioLibrary {

	using namespace std;

	// 音声ファイルをロードするクラス
	class AUDIOLIBRARY_API AudioLoader final sealed {

		AudioLoader() {}
		~AudioLoader() {}

	public:
		
		// 音声ファイルを読み込む
		static HRESULT LoadWaveFile(const IXAudio2& xAudio2, const wstring& filePath, shared_ptr<AudioData>& retAudioData);

	private:

		// フルパスに変換
		static bool ToFullPath(const wstring& filepath, wstring& retFullPath);

	};

}