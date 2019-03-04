#pragma once

using namespace System;

namespace SoundEffectTool {

	public ref class SoundEffectToolHelper {

	public :
		
		static bool Initialize();
		static void Final();

		// ウィンドウの生成
		// 戻り値はバックバッファ
		static const IntPtr CreateDxView(IntPtr windowHandle);
	};
}
