using System;
using System.Collections.Generic;
using System.Diagnostics;
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
		private TimeSpan _lastRender;

		public MainWindow() {
			InitializeComponent();

			_soundEffectToolVM = new SoundEffectToolVM();
			DataContext = _soundEffectToolVM;

		}

		private void Image_Loaded(object sender, RoutedEventArgs e) {

			// ウィンドウハンドルを生成
			var hwndSrc = new HwndSource(0, 0, 0, 0, 0, "DxLib", IntPtr.Zero);

			_soundEffectToolVM.CreateDxView(hwndSrc.Handle, _windowName, (int)Image.Width, (int)Image.Height);

			D3DImage = new D3DImage();
			Image.Source = D3DImage;

			// 描画の更新
			CompositionTarget.Rendering += UpdateRendering;
			D3DImage.IsFrontBufferAvailableChanged += D3DImage_IsFrontBufferAvailableChanged;

		}

		private void Image_SizeChanged(object sender, SizeChangedEventArgs e) {
			//描画のサイズも変更する
			//_soundEffectToolVM.ChangeDrawSize(_windowName, (int)e.NewSize.Width, (int)e.NewSize.Height);
		}

		/// <summary>
		/// フロントバッファの更新
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void D3DImage_IsFrontBufferAvailableChanged(object sender, DependencyPropertyChangedEventArgs e) {

			if(D3DImage.IsFrontBufferAvailable) {
				CompositionTarget.Rendering += UpdateRendering;
			}
			else {
				CompositionTarget.Rendering -= UpdateRendering;
			}
		}

		private void MenuItem_Click(object sender, RoutedEventArgs e) {
			MessageBox.Show("使用したライブラリ/フレームワーク\r\n・Dxlib\r\n・XAudio2\r\n・Extended WPF Toolkit", "情報", MessageBoxButton.OK);
		}

		private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e) {
			
		}

		private void UpdateRendering(object sender, EventArgs e) {

			var args = (RenderingEventArgs)e;
			if(D3DImage.IsFrontBufferAvailable && _lastRender != args.RenderingTime) {

				IntPtr backBuffer = _soundEffectToolVM.GetBackBuffer(_windowName);
				if(backBuffer != IntPtr.Zero) {

					D3DImage.Lock();

					// バックバッファの設定
					D3DImage.SetBackBuffer(D3DResourceType.IDirect3DSurface9, backBuffer);
					// 描画
					_soundEffectToolVM.Draw(_windowName);
					D3DImage.AddDirtyRect(new Int32Rect(0, 0, D3DImage.PixelWidth, D3DImage.PixelHeight));

					D3DImage.Unlock();

					_lastRender = args.RenderingTime;
				}
			}
		}

		private void TestPlaySound(object sender, RoutedEventArgs e) {
			_soundEffectToolVM.PlaySoundFromFile("Resource/Audio/MusicSurround.wav");
		}
	}
}
