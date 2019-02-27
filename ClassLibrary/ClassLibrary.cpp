#include "stdafx.h"

#include "ClassLibrary.h"
#include "SoundEffectTool.h"

namespace SoundEffectTool {

	void SoundEffectToolHelper::Generate() {
		instance = new SoundEffectToolSystem;
	}

	SoundEffectToolSystem* SoundEffectToolHelper::GetInstance() {
		return instance;
	}
}
