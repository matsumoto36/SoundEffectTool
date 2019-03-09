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

		private SoundEffectToolVM _soundEffectToolVM;
		private string _windowName = "DxLib";

		public MainWindow() {
			InitializeComponent();

			_soundEffectToolVM = new SoundEffectToolVM();
			DataContext = _soundEffectToolVM;

			SetUpD3DImage();
		}

		private void Image_SizeChanged(object sender, SizeChangedEventArgs e) {
			//描画のサイズも変更する
			_soundEffectToolVM.ChangeDrawSize(_windowName, (int)e.NewSize.Width, (int)e.NewSize.Height);
		}

		private void MenuItem_Click(object sender, RoutedEventArgs e) {
			MessageBox.Show("使用したライブラリ/フレームワーク\r\n・Dxlib\r\n・XAudio2\r\n・Extended WPF Toolkit", "情報", MessageBoxButton.OK);
		}

		private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e) {
			//SoundEffectToolHelper.Final();
		}

		private void SetUpD3DImage() {

			//ウィンドウハンドルを生成
			var hwndSrc = new HwndSource(0, 0, 0, 0, 0, "DxLib", IntPtr.Zero);
			D3DImage = new D3DImage();

			_soundEffectToolVM.CreateDxView(hwndSrc.Handle, _windowName, (int)Image.Width, (int)Image.Height);
			var backBuffer = _soundEffectToolVM.GetBackBuffer(_windowName);

			CompositionTarget.Rendering += (s, e) => {

				// バックバッファの設定
				D3DImage.Lock();
				D3DImage.SetBackBuffer(D3DResourceType.IDirect3DSurface9, backBuffer);
				D3DImage.Unlock();
				Image.Source = D3DImage;
			};
		}
	}
}
