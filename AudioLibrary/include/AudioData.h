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

		unique_ptr<uint8_t[]> _wave;	// ファイルデータ本体
		uint32_t _dataLength;			// 波形の長さ(バイト数)
		uint32_t _sampleLength;			// 波形の長さ(サンプル数)

		struct Impl;
		unique_ptr<Impl> _impl;

	public:
		AudioData(const WAVEFORMATEX& format, unique_ptr<uint8_t[]> wave, const XAUDIO2_BUFFER& buffer);
		~AudioData();

		// コピーは禁止するが、ムーブは許可する
		AudioData(const AudioData&) = delete;
		AudioData(AudioData&&) noexcept;
		AudioData& operator=(const AudioData&) = delete;

		// 波形ファイルデータを返す
		const unique_ptr<uint8_t[]>& GetWaveFile() const;

		// 波形データを返す
		const uint8_t* GetWaveData() const;

		// 波形のデータの長さを返す
		const uint32_t GetDataLength() const {
			return _dataLength;
		}

		// 波形のサンプル数を返す
		const uint32_t GetSampleLength() const {
			return _sampleLength;
		}

		// 波形内のサンプルを読み取る
		bool ReadSamples(uint32_t start, uint32_t length, char** outSamples) const;

		// チャンネルの数を返す
		const int GetChannelCount() const;

		// フォーマットを返す
		const WAVEFORMATEX& GetFormat() const;

		// バッファを返す
		const XAUDIO2_BUFFER& GetBuffer() const;
	};
}