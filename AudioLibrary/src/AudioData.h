#pragma once

// unique_ptr使用につき
#pragma warning(disable:4251)

#include <memory>
#include "AudioDefine.h"

namespace AudioLibrary {

	using namespace std;

	// 読み込んだサウンドのデータ
	class AUDIOLIBRARY_API AudioData {

		unique_ptr<WAVEFORMATEX> _format;				// 波形の情報
		unique_ptr<uint8_t[]> _wave;					// 波形本体
		XAUDIO2_BUFFER _buffer;						// ソースボイスのバッファ

	public:
		AudioData(unique_ptr<WAVEFORMATEX> format, unique_ptr<uint8_t[]> wave, XAUDIO2_BUFFER& buffer);
		~AudioData();

		// コピーは禁止するが、ムーブは許可する
		AudioData(const AudioData&) = delete;
		AudioData(AudioData&&) noexcept = default;
		AudioData& operator=(const AudioData&) = delete;

		// フォーマットを返す
		const WAVEFORMATEX& GetFormat() const {
			return *_format;
		}

		// バッファを返す
		const XAUDIO2_BUFFER& GetBuffer() const {
			return _buffer;
		}
	};
}