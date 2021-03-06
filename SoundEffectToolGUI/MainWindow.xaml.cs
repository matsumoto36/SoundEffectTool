﻿using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Threading;
using SoundEffectTool;

namespace SoundEffectToolGUI {
	/// <summary>
	/// MainWindow.xaml の相互作用ロジック
	/// </summary>
	public partial class MainWindow : Window {

		const string SoundKey = "MainSound";							// 音声ファイルにアクセスするためのキー
		const string SoundFilePath = "Resource/Audio/MusicSurround.wav";// 音声ファイルのパス

		private SoundEffectToolVM _soundEffectToolVM;					// 内部システム
		private string _windowName = "WaveRenderer";					// 描画のウィンドウにアクセスするためのキー
		private TimeSpan _lastRender;									// 最後に描画した時間
		private Point _waveOffset;										// 波形を描画するときのオフセット
		private float _wavePixelsPerSec;								// 波形の拡大率

		private DispatcherTimer _audioTick;								// サウンド関係の更新タイマー
		private DateTime _lastAudioTickTime;							// 最後にサウンドを更新した時間
		private float _soundFileLength;									// 音楽ファイルの再生時間
		private bool _currentPositionChanging;							// 再生位置を変更している途中か

		private BitmapImage _startButtonImage;
		private BitmapImage _pauseButtonImage;
		private BitmapImage _volumeButtonImage;
		private BitmapImage _volumeMuteButtonImage;

		// 音量
		private float _volume;
		public float Volume {
			get { return _volume; }
			set {
				_volume = value;
				if(_soundEffectToolVM == null) return;
				_soundEffectToolVM.SetVolume(_volume);
			}
		}

		// 再生位置の比率
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

		private void Image_PreviewMouseWheel(object sender, MouseWheelEventArgs e) {

			if(Keyboard.Modifiers != ModifierKeys.Control) return;
			e.Handled = true;

			// 倍率を設定
			var delta = 2;
			var min = 4;
			var max = 1024;

			var current = _wavePixelsPerSec;

			if(e.Delta > 0)
				current = Math.Min(current * delta, max);
			else if(e.Delta < 0)
				current = Math.Max(current / delta, min);

			if(current == _wavePixelsPerSec) return;

			// 波形の横幅を変更する
			_soundEffectToolVM.CalcWaveRenderingScale(_windowName, _wavePixelsPerSec = current);

			// サイズ変更
			var size = _soundEffectToolVM.GetDrawSize(_windowName);
			ImageRect.Width = size.Width;
			ImageRect.Height = size.Height;
		}

		private void ScrollViewer_ScrollChanged(object sender, ScrollChangedEventArgs e) {
			// スクロールしてもImageの位置が相対的に変わらないようにする
			var margin = Image.Margin;
			margin.Left = e.HorizontalOffset;
			Image.Margin = margin;
			_waveOffset.X = -e.HorizontalOffset;
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

		private void MenuItem_Exit_Click(object sender, RoutedEventArgs e) {
			Close();
		}

		private void MenuItem_Info_Click(object sender, RoutedEventArgs e) {
			var message = "使用したライブラリ/フレームワーク\r\n・Dxlib\r\n・XAudio2\r\n・Extended WPF Toolkit";
			MessageBox.Show(message, "情報", MessageBoxButton.OK);
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
				Volume = 0;
			}
			else {
				Volume = (float)VolumeSlider.Value;
			}
		}

		private void VolumeSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e) {
			Volume = (float)e.NewValue;
		}

		private void PlayPositionSlider_PreviewMouseDown(object sender, MouseButtonEventArgs e) {
			_currentPositionChanging = true;
		}

		private void PlayPositionSlider_PreviewMouseUp(object sender, MouseButtonEventArgs e) {
			_currentPositionChanging = false;

			// スライダーの位置を再生位置にする
			PlayRatio = (float)PlayPositionSlider.Value;
			PlayPositionLabel.Content = ToTime(PlayRatio * _soundFileLength);

			// 再生している場合は止めずに再生
			if(_soundEffectToolVM.IsPlay()) {
				_soundEffectToolVM.PlayMainSoundAtPosition(PlayRatio * _soundFileLength);
			}
		}

		private void PlayPositionSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e) {
			// スライダーの位置を時間にする
			PlayPositionLabel.Content = ToTime(PlayRatio * _soundFileLength);
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

			// くっきり表示させる
			RenderOptions.SetEdgeMode(Image, EdgeMode.Aliased);
			RenderOptions.SetBitmapScalingMode(Image, BitmapScalingMode.NearestNeighbor);


			// ウィンドウハンドルを生成
			var hwndSrc = new HwndSource(0, 0, 0, 0, 0, "DxLib", IntPtr.Zero);

			// Imageは1920*1080固定
			var imageSize = new Size(1920, 1080);
			Image.RenderSize = imageSize;
			Image.Width = imageSize.Width;
			Image.Height = imageSize.Height;
			_soundEffectToolVM.CreateDxView(hwndSrc.Handle, _windowName, Image.RenderSize);

			D3DImage = new D3DImage();
			Image.Source = D3DImage;

			// 描画の更新
			CompositionTarget.Rendering += UpdateRendering;
			D3DImage.IsFrontBufferAvailableChanged += D3DImage_IsFrontBufferAvailableChanged;

			// 描画対象に設定
			_wavePixelsPerSec = _soundEffectToolVM.GetDefaultPixelsPerSec();
			_soundEffectToolVM.SetWaveData(_windowName, SoundKey);
			var size = _soundEffectToolVM.GetDrawSize(_windowName);
			ImageRect.Width = size.Width;
			ImageRect.Height = size.Height;
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
			SoundFileLengthLabel.Content = ToTime(_soundFileLength);

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
						if(isPlay)
							PlayPositionSlider.Value = PlayRatio;
					});
				}
				catch { /* キャンセルされたときは握りつぶす */ }
			};

			// 再生し終わったら戻す
			_soundEffectToolVM.OnAudioPlayingEnd += () => {
				// 別スレッドからも呼ばれるため
				try {
					Dispatcher.Invoke(() => {
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
		}

		/// <summary>
		/// タイマーを準備する
		/// </summary>
		private void SetupTimer() {

			// タイマー起動
			_audioTick = new DispatcherTimer();
			_audioTick.Interval = new TimeSpan(1);
			_lastAudioTickTime = DateTime.Now;
			_audioTick.Tick += (s, e) => {

				// 音楽系更新
				var deltaTime = (float)(DateTime.Now - _lastAudioTickTime).TotalSeconds;
				_soundEffectToolVM.UpdateAudio(deltaTime);
				_lastAudioTickTime = DateTime.Now;

				// 再生位置を更新
				var position = _soundEffectToolVM.GetMainPlayerPosition();
				var ratio = position / _soundFileLength;
				if(_soundFileLength == 0) {
					ratio = 0;
				}

				if(!_soundEffectToolVM.IsPlay()) return;

				// 再生情報
				PlayRatio = ratio;
				PlayPositionLabel.Content = ToTime(position);

				// 1秒ごとに再生位置の表示更新
				// スライダーに触れている間は、再生位置の更新をしないようにする
				if(Math.Floor(position) != Math.Floor(position - deltaTime) && !_currentPositionChanging) {
					PlayPositionSlider.Value = PlayRatio;
				}

			};
			_audioTick.Start();

			// アプリを終了するときにタイマーも終了する
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
			if(D3DImage.IsFrontBufferAvailable && _lastRender.TotalSeconds + 0.02 < args.RenderingTime.TotalSeconds) {

				// バックバッファの取得
				var backBuffer = _soundEffectToolVM.GetBackBuffer(_windowName);
				if(backBuffer != IntPtr.Zero) {

					D3DImage.Lock();

					// バックバッファの設定
					D3DImage.SetBackBuffer(D3DResourceType.IDirect3DSurface9, backBuffer);
					// 描画
					_soundEffectToolVM.Draw(_windowName, _waveOffset, PlayRatio);
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
			PlayPositionSlider.Value = PlayRatio;
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
