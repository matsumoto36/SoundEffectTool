#pragma once

using namespace System;

namespace SoundEffectTool {

	class SoundEffectToolSystem;
	public ref class SoundEffectToolHelper {

		SoundEffectToolSystem* instance;

	public :
		
		void Generate();

		SoundEffectToolSystem* GetInstance();

	};
}
