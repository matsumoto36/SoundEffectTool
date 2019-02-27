#pragma once
namespace AudioLibrary {
	class Audio;
}

using namespace AudioLibrary;

namespace SoundEffectTool {


	class SoundEffectToolSystem {

		Audio* audio;

	public:

		void GenerateAudio();

		Audio* GetAudio();

		void Draw() {}
	};
}
