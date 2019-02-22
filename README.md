# audioio
Unity project for iOS/Android Audio Input/Output.

### 概要
オーディオの入力／出力のテストのために作成しました。入力した音をリアルタイムに出力します。iOS の CoreAudio と Android の AAudio, OpenSLES, Samsung Professional Audio の実装をしています。<br>

### 構成
Unity プロジェクトです。Projects フォルダ以下がプラグインのプロジェクトです。Samsung Professional Audio のプラグインのプロジェクトはライセンス上の理由で公開できていません。<br>

### ビルド方法
Unity のメニューより Tools -> Build -> iOS/Android を実行します。iOS は Xcode のプロジェクトが iOS フォルダに出力されます。Android は jp.co.aquariumy.audioio.apk が出力されます。<br>

### アプリ操作方法
１．イヤホン／ヘッドホンを接続するとオーディオの出力が開始されます<br>
２．バッファーのサンプル数を設定できます（CoreAudio, AAudio, OpenSLES)<br>
３．入力音量を設定できます<br>
４．出力音量を設定できます<br>

### 所感
CoreAudio のリアルタイム性能が圧倒的です。AAudio, Samsung Professional Audio も製品利用可能と思います。OpenSLES は厳しいですね。

### 動作確認環境
MacBook MacOS 10.11.6<br>
Unity 5.6.4p4<br>
Xcode 8.1 + Device Support iOS 10.2<br>
Eclipse 4.7.3<br>
Android SDK 5.0.1(21)<br>
Android NDK r10e<br>
iPhone 6 Plus iOS 10.2 + iRig Pro<br>
Android One S2 Android 8.0 + iRig HD-A<br>
Galaxy S6 edge Android 7.0 + iRig HD-A<br>
Nexus 5 Android 6.0 + iRig HD-A<br>
