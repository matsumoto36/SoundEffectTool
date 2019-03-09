#pragma once

using System::ComponentModel::INotifyPropertyChanged;
using System::ComponentModel::PropertyChangedEventArgs;
using System::ComponentModel::PropertyChangedEventHandler;

using namespace System;

namespace SoundEffectTool {

	public ref class SoundEffectToolVM : public INotifyPropertyChanged {

		SoundEffectToolManager* _manager;

	public :

		// INotifyPropertyChanged を介して継承されました
		virtual event System::ComponentModel::PropertyChangedEventHandler ^PropertyChanged;

		SoundEffectToolVM();
		~SoundEffectToolVM();
		!SoundEffectToolVM();
		
		// ウィンドウの生成
		void CreateDxView(IntPtr windowHandle, String^ windowName, int width, int height);

		// 対象のウィンドウのバックバッファを取得する
		const IntPtr GetBackBuffer(String^ windowName);

		// 描画サイズを変更する
		void ChangeDrawSize(String^ windowName, int width, int height);

		// 描画する
		void Draw(String^ windowName);

	};
}
