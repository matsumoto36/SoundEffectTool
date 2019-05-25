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
using System.Windows.Threading;
using SoundEffectTool;

namespace SoundEffectToolGUI {
	/// <summary>
	/// MainWindow.xaml の相互作用ロジック
	/// </summary>
	public partial class MainWindow : Window {

		const string SoundKey = "MainSound";							// 音声ファイルにアクセスするためのキー
		const string SoundFilePath = "Resource/Audio/MusicSurround.wav";	// 音声ファイルのパス
		private SoundEffectToolVM _soundEffectToolVM;
		private string _windowName = "DxLib";
		private TimeSpan _lastRender;

		private DispatcherTimer _audioTick;

		private DateTime _lastTick;
		private float _soundFileLength;
		private bool _playerPositionChanging;

		private BitmapImage _startButtonImage;
		private BitmapImage _pauseButtonImage;
		private BitmapImage _volumeButtonImage;
		private BitmapImage _volumeMuteButtonImage;

		private float _volume;
		public float Volume {
			get { return _volume; }
			set {
				_volume = value;
				if(_soundEffectToolVM == null) return;
				_soundEffectToolVM.SetVolume(_volume);
			}
		}

		public float PlayRatio { get; set; }

		public MainWindow() {

			InitializeComponent();

			_soundEffectToolVM = new SoundEffectToolVM();
			DataContext = _soundEffectToolVM;

			// 画像の読み込み
			LoadButtonImage();

			// サウンドシステムの準備
			SetupAudio();

			// タイマー準備
			SetupTimer();
		}

		#region Events

		private void Image_Loaded(object sender, RoutedEventArgs e) {
			// レンダリング準備
			SetupRendering();
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

		private void StartButton_Click(object sender, RoutedEventArgs e) {
			if(_soundEffectToolVM.IsPlay()) {
				PauseSound();
			}
			else {
				PlaySound();
			}
		}

		private void StopButton_Click(object sender, RoutedEventArgs e) {
			StopSound();
		}

		private void MuteButton_Click(object sender, RoutedEventArgs e) {
			if(_volume != 0) {
				// ミュートにする
				Volume = 0;
			}
			else {
				// 音量を戻す
				Volume = (float)VolumeSlider.Value;
			}
		}

		private void VolumeSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e) {
			// 音量を変更
			Volume = (float)e.NewValue;
		}

		private void PlayPositionSlider_PreviewMouseDown(object sender, MouseButtonEventArgs e) {
			_playerPositionChanging = true;
		}

		private void PlayPositionSlider_PreviewMouseUp(object sender, MouseButtonEventArgs e) {
			_playerPositionChanging = false;
			PlayRatio = (float)PlayPositionSlider.Value;
			if(_soundEffectToolVM.IsPlay()) {
				// 再生位置を変更して再生
				_soundEffectToolVM.PlayMainSoundAtPosition(PlayRatio * _soundFileLength);
			}
		}

		private void PlayPositionSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e) {
			PlayPositionText.Text = ToTime(PlayRatio * _soundFileLength);
		}
		#endregion

		/// <summary>
		/// ボタンの画像を読み込む
		/// </summary>
		private void LoadButtonImage() {

			// 画像を読み込む
			void LoadImage(ref BitmapImage image, string path) {
				image = new BitmapImage();
				image.BeginInit();
				image.UriSource = new Uri(path, UriKind.Relative);
				image.EndInit();
			}

			LoadImage(ref _startButtonImage, "\\Resource\\Texture\\Play.png");
			LoadImage(ref _pauseButtonImage, "\\Resource\\Texture\\Pause.png");
			LoadImage(ref _volumeButtonImage, "\\Resource\\Texture\\Volume.png");
			LoadImage(ref _volumeMuteButtonImage, "\\Resource\\Texture\\VolumeMute.png");
		}

		/// <summary>
		/// レンダリングシステムを準備する
		/// </summary>
		private void SetupRendering() {

			// ウィンドウハンドルを生成
			var hwndSrc = new HwndSource(0, 0, 0, 0, 0, "DxLib", IntPtr.Zero);

			_soundEffectToolVM.CreateDxView(hwndSrc.Handle, _windowName, (int)Image.Width, (int)Image.Height);

			D3DImage = new D3DImage();
			Image.Source = D3DImage;

			// 描画の更新
			CompositionTarget.Rendering += UpdateRendering;
			D3DImage.IsFrontBufferAvailableChanged += D3DImage_IsFrontBufferAvailableChanged;

			// 描画対象に設定
			var b = _soundEffectToolVM.SetWaveData(_windowName, SoundKey);
		}

		/// <summary>
		/// サウンドシステムを準備する
		/// </summary>
		private void SetupAudio() {

			// 音声ファイルのロード
			_soundEffectToolVM.LoadSound(SoundFilePath, SoundKey);
			// メインの音にセット
			_soundEffectToolVM.SetMainSound(SoundKey);

			// 長さを取得
			_soundFileLength = _soundEffectToolVM.GetSoundFileLength();
			SoundFileLengthText.Text = ToTime(_soundFileLength);

			// 再生状況が変化したときにアイコンを変更
			_soundEffectToolVM.OnAudioIsPlayChanged += isPlay => {
				BitmapImage image;
				if(isPlay) {
					image = _pauseButtonImage;
				}
				else {
					image = _startButtonImage;
				}

				// 別スレッドからも呼ばれるため
				try {
					Dispatcher.Invoke(() => {
						StartButtonImage.Source = image;
					});
				}
				catch { /* キャンセルされたときは握りつぶす */ }

			};

			// 再生位置を更新
			_soundEffectToolVM.OnAudioIsPlayChanged += isPlay => {
				// 別スレッドからも呼ばれるため
				try {
					Dispatcher.Invoke(() => {
						if(PlayRatio == 1.0f)
							PlayPositionSlider.Value = PlayRatio = 0.0f;
					});
				}
				catch { /* キャンセルされたときは握りつぶす */ }
			};

			// 音量が変化したときにアイコンを変更
			_soundEffectToolVM.OnAudioVolumeChanged += volume => {
				BitmapImage image;
				if(volume == 0) {
					image = _volumeMuteButtonImage;
				}
				else {
					image = _volumeButtonImage;
				}

				// 別スレッドからも呼ばれるため
				try {
					Dispatcher.Invoke(() => {
						MuteButtonImage.Source = image;
					});
				}
				catch { /* キャンセルされたときは握りつぶす */ }
			};

				//PlayPositionSlider.
		}
		/// <summary>
		/// タイマーを準備する
		/// </summary>
		private void SetupTimer() {

			// タイマー起動
			_audioTick = new DispatcherTimer();
			_audioTick.Interval = new TimeSpan(1);
			_lastTick = DateTime.Now;
			_audioTick.Tick += (s, e) => {

				if(!_soundEffectToolVM.IsPlay()) return;

				// 音楽系更新
				var deltaTime = (float)(DateTime.Now - _lastTick).TotalSeconds;
				_soundEffectToolVM.UpdateAudio(deltaTime);
				_lastTick = DateTime.Now;

				var position = _soundEffectToolVM.GetMainPlayerPosition();
				var ratio = position / _soundFileLength;
				if(_soundFileLength == 0) {
					ratio = 0;
				}

				// 再生情報
				PlayRatio = ratio;
				PlayPositionText.Text = ToTime(position);

				// 一秒ごとに再生位置の表示更新
				if (Math.Floor(position) != Math.Floor(position - deltaTime) && !_playerPositionChanging) {
					PlayPositionSlider.Value = PlayRatio;
				}

			};
			_audioTick.Start();


			Closed += (s, e) => {
				_audioTick.Stop();
			};
		}

		/// <summary>
		/// 描画を更新する
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void UpdateRendering(object sender, EventArgs e) {

			var args = (RenderingEventArgs)e;
			if(D3DImage.IsFrontBufferAvailable && _lastRender != args.RenderingTime) {

				var backBuffer = _soundEffectToolVM.GetBackBuffer(_windowName);
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

		/// <summary>
		/// セットした音声を再生する
		/// </summary>
		private void PlaySound() {
			_soundEffectToolVM.PlayMainSoundAtPosition(PlayRatio * _soundFileLength);
		}

		/// <summary>
		/// セットした音声を停止する
		/// </summary>
		private void StopSound() {
			_soundEffectToolVM.StopMainSound();
			PlayPositionSlider.Value = PlayRatio = 0.0f;
		}

		/// <summary>
		/// セットした音声をポーズする
		/// </summary>
		private void PauseSound() {
			_soundEffectToolVM.PauseMainSound();
		}

		/// <summary>
		/// 時間を表示するテキストに変換
		/// </summary>
		/// <param name="time"></param>
		/// <returns></returns>
		private string ToTime(float time) {
			var minites = (int)(time / 60.0f);
			var seconds = time - minites;
			return string.Format("{0}:{1:00.00}", minites, seconds);
		}
	}
}
