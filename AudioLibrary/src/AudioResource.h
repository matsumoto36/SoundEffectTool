#pragma once

// unique_ptr使用につき
#pragma warning(disable:4251)

#include <vector>
#include "AudioDefine.h"

namespace AudioLibrary {

	using namespace std;

	// サウンドシステムのデータを保管する
	class AUDIOLIBRARY_API AudioResource {
	public:
		AudioResource();
		~AudioResource();


	};
}
