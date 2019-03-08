using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using SoundEffectTool;

namespace SoundEffectToolGUI {
	/// <summary>
	/// MainWindow.xaml の相互作用ロジック
	/// </summary>
	public partial class MainWindow : Window {

		IntPtr _backBuffer;
		IntPtr _renderer;

		public MainWindow() {
			InitializeComponent();

			//システムセットアップ
			SoundEffectToolHelper.Initialize();

			//ウィンドウハンドルを生成
			var hwndSrc = new HwndSource(0, 0, 0, 0, 0, "DxLib", IntPtr.Zero);
			D3DImage = new D3DImage();

			_renderer = SoundEffectToolHelper.CreateDxView(hwndSrc.Handle, ref _backBuffer, (int)Image.Width, (int)Image.Height);

			//Dxlib黒画面表示
			CompositionTarget.Rendering += CompositionTarget_Rendering;
		}

		private void CompositionTarget_Rendering(object sender, EventArgs e) {

			// バックバッファの設定
			D3DImage.Lock();
			D3DImage.SetBackBuffer(D3DResourceType.IDirect3DSurface9, _backBuffer);
			D3DImage.Unlock();
			Image.Source = D3DImage;
		}

		private void Image_SizeChanged(object sender, SizeChangedEventArgs e) {
			//描画のサイズも変更する
			SoundEffectToolHelper.ChangeDrawSize(_renderer, (int)e.NewSize.Width, (int)e.NewSize.Height);
		}

		private void MenuItem_Click(object sender, RoutedEventArgs e) {
			MessageBox.Show("使用したライブラリ/フレームワーク\r\n・Dxlib\r\n・XAudio2\r\n・Extended WPF Toolkit", "情報", MessageBoxButton.OK);
		}

		private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e) {

			SoundEffectToolHelper.Final();
		}
	}
}
