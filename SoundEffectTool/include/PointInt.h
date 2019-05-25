#pragma once
#include "SoundEffectToolDefine.h"

namespace SoundEffectTool {
	// “_‚ð•\‚·
	struct SOUNDEFFECTTOOL_API PointInt {
	public:
		int X, Y;
		PointInt() : X(0), Y(0) {}
		PointInt(int x, int y) : X(x), Y(y) {}
		PointInt operator+(PointInt p) {
			return PointInt(X + p.X, Y + p.Y);
		}
		PointInt operator-(PointInt p) {
			return PointInt(X - p.X, Y - p.Y);
		}
	};
}