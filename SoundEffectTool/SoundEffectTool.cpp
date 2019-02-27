// SoundEffectToolModel.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"

#include "SoundEffectTool.h"
#include "AudioLibrary.h"
#include "DxLib.h"

using namespace AudioLibrary;
using namespace DxLib;

namespace SoundEffectTool {

	void SoundEffectToolSystem::GenerateAudio() {
		audio = new Audio;
	}

	Audio* SoundEffectToolSystem::GetAudio() {
		return audio;
	}
}