#pragma once

#include <string>
#include <memory>

#include "AudioData.h"

namespace AudioLibrary {

	using namespace std;

	// 音声ファイルをロードするクラス
	class AUDIOLIBRARY_API AudioLoader final sealed {

		AudioLoader() {}
		~AudioLoader() {}

	public:
		
		// 音声ファイルを読み込む
		static HRESULT LoadWaveFile(const unique_ptr<IXAudio2>& xAudio2, wstring& filePath, unique_ptr<AudioData>& retAudioData);

		// フルパスに変換
		static bool ToFullPath(wstring& filepath, wstring& retFullPath);

	};

}