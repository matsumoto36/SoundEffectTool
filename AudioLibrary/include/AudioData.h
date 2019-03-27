#pragma once

// unique_ptr使用につき
#pragma warning(disable:4251)

#include <memory>
#include "AudioDefine.h"

namespace AudioLibrary {

	using namespace std;

	// 読み込んだサウンドのデータ
	class AUDIOLIBRARY_API AudioData {
	public:
		~AudioData();

		// コピーは禁止するが、ムーブは許可する
		AudioData(const AudioData&) = delete;
		AudioData(AudioData&&) noexcept = default;
		AudioData& operator=(const AudioData&) = delete;
	};
}