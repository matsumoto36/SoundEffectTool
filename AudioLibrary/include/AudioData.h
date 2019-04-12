#pragma once

// unique_ptr使用につき
#pragma warning(disable:4251)

#include <vector>
#include <memory>
#include "AudioDefine.h"

#include "../src/WAVFileReader.h"

using namespace std;

namespace AudioLibrary {

	// 読み込んだサウンドのデータ
	class AUDIOLIBRARY_API AudioData {

		unique_ptr<uint8_t[]> _wave;	// 波形本体

		struct Impl;
		unique_ptr<Impl> _impl;

	public:
		AudioData(const WAVEFORMATEX& format, unique_ptr<uint8_t[]> wave, const XAUDIO2_BUFFER& buffer);
		~AudioData();

		// コピーは禁止するが、ムーブは許可する
		AudioData(const AudioData&) = delete;
		AudioData(AudioData&&) noexcept;
		AudioData& operator=(const AudioData&) = delete;

		// 指定した数のデータを取得
		const bool ReadData(size_t start, size_t count, const uint8_t* bytes) const;

		// 波形を返す
		const unique_ptr<uint8_t[]>& GetWave() const;

		// チャンネルの数を返す
		const int GetChannelCount() const;

		// フォーマットを返す
		const WAVEFORMATEX& GetFormat() const;

		// バッファを返す
		const XAUDIO2_BUFFER& GetBuffer() const;
	};
}