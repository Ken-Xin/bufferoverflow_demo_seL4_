# seL4マイクロカーネル セキュリティデモンストレーションプロジェクト


seL4マイクロカーネルのセキュリティ機能を包括的に実証するプロジェクトです。IPC通信、コンポーネント隔離、フォールトトレランス、バッファオーバーフロー保護をケイパビリティベースの隔離機能により実現します。

## 目次

- [プロジェクト概要](#プロジェクト概要)
- [主な機能](#主な機能)
- [システム要件](#システム要件)
- [インストール](#インストール)
- [プロジェクトタスク](#プロジェクトタスク)
- [ビルドと実行](#ビルドと実行)
- [プロジェクト構造](#プロジェクト構造)
- [重要な概念](#重要な概念)
- [Linuxとの比較](#linuxとの比較)
- [トラブルシューティング](#トラブルシューティング)

## プロジェクト概要

このプロジェクトは、ARM64アーキテクチャ上でMicrokitを使用したPC/MacベースのseL4マイクロカーネルシステムをQEMU経由（オプションでRaspberry Pi 4上）で実装します。以下を実証します：

- エンドポイントと共有メモリによるプロセス間通信（IPC）
- ケイパビリティベースの隔離（VSpaceとCSpace）
- フォールトトレランスと封じ込め
- 数学的証明に基づく隔離によるバッファオーバーフロー保護
- モノリシックなLinuxアーキテクチャとのパフォーマンス比較

##  主な機能

### タスク2: IPC クライアント・サーバー実装
- クライアントとサーバー間のエンドポイントベース通信
- 共有メモリ（ページマッピング）のデモンストレーション
- バッジとリプライオブジェクトを使用したメッセージパッシング

### タスク3: ロガー付きコンポーネント隔離
- **VSpace隔離**: 各Protection Domainが独立した仮想アドレス空間を持つ
- **CSpace隔離**: ケイパビリティベースのアクセス制御
- **ケイパビリティマッピング**: 最小権限の原則の実施
- 制御されたアクセスを持つ第3のコンポーネント（ロガー）

### タスク4: フォールトトレランスのデモンストレーション
- 意図的なコンポーネントクラッシュのデモ
- フォールト封じ込めの検証
- 障害時のシステム整合性の維持
- フォールトハンドラーの実装

### タスク5: バッファオーバーフローセキュリティ分析
- 従来のセキュリティ機能を無効化したバッファオーバーフロー攻撃
- seL4と従来のOS保護の比較
- seL4の数学的証明ベースのセキュリティのデモンストレーション
- seL4の基本的な保護機能を示すため、ASLR、スタックカナリア、DEPを無効化

##  システム要件

### ハードウェア
- Mac（M1/M2/M3 Apple Silicon または Intel）
- 推奨4GB以上のRAM
- （オプション）ハードウェア展開用のRaspberry Pi 4

### ソフトウェア
- macOS 11.0以降
- Homebrewパッケージマネージャー
- QEMU（ARM64サポート）
- AArch64クロスコンパイラツールチェーン
- seL4 Microkit SDK v2.0.1

## インストール

### 1. 依存関係のインストール

Homebrewのインストール（まだインストールしていない場合）
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

QEMUのインストール
brew install qemu

AArch64ツールチェーンのインストール
brew install aarch64-elf-gcc

または:
brew tap messense/macos-cross-toolchains
brew install aarch64-unknown-linux-gnu



### 2. Microkit SDKのダウンロード

プロジェクトディレクトリの作成
mkdir ~/seL4_project
cd ~/seL4_project

Microkit SDKのダウンロード（M1/M2/M3 Mac用のmacOS ARM64版）
curl -L https://github.com/seL4/microkit/releases/download/2.0.1/microkit-sdk-2.0.1-macos-aarch64.tar.gz -o sdk.tar.gz

Intel Mac用:
curl -L https://github.com/seL4/microkit/releases/download/2.0.1/microkit-sdk-2.0.1-macos-x86-64.tar.gz -o sdk.tar.gz
展開
tar xf sdk.tar.gz
mv microkit-sdk-2.0.1 microkit-sdk



### 3. プロジェクトファイルのクローン/コピー

すべてのプロジェクトファイルを`~/seL4_project`ディレクトリに配置：
- `Makefile`
- `util.h`
- `*.c`ファイル（server.c、client.c、logger.cなど）
- `*.xml`ファイル（system.xml、system_with_logger.xmlなど）

##  プロジェクトタスク

### タスク2: IPC クライアント・サーバー実装

make clean
make task2
make run



**期待される出力**: クライアントとサーバーが共有メモリを介してメッセージを交換し、IPC機能を実証します。

### タスク3: ロガー付きコンポーネント隔離

make clean
make task3
make run



**期待される出力**: 3つの独立したコンポーネント（クライアント、サーバー、ロガー）が独立して動作し、VSpaceとCSpaceの隔離を実証します。

### タスク4: フォールトトレランス

make clean
make task4
make run



**期待される出力**: 1つのコンポーネントが意図的にクラッシュしますが、他のコンポーネントは正常に動作し続けます。

### タスク5: バッファオーバーフローセキュリティ

make clean
make task5
make run



**期待される出力**: 脆弱なコンポーネントでバッファオーバーフローが発生しますが、隔離により他のコンポーネントへの伝播を防ぎます。

### QEMUの終了

`Ctrl+A`を押してから`X`を押す

## 📁 プロジェクト構造

seL4_project/
├── Makefile # ビルド自動化
├── util.h # 文字列/メモリユーティリティ関数
│
├── server.c # サーバーコンポーネント
├── client.c # クライアントコンポーネント
├── logger.c # ロガーコンポーネント
├── fault_handler.c # フォールトハンドリングコンポーネント
├── fault_test.c # フォールトテストコンポーネント
├── safe_server.c # セキュアサーバーコンポーネント
├── vulnerable_pd.c # 脆弱なコンポーネント（タスク5用）
├── attacker.c # 攻撃シミュレーター（タスク5用）
│
├── system.xml # タスク2システム記述
├── system_with_logger.xml # タスク3システム記述
├── system_fault.xml # タスク4システム記述
├── system_buffer_overflow.xml # タスク5システム記述
│
├── microkit-sdk/ # Microkit SDK
└── README.md # このファイル


##  重要な概念

### VSpace隔離
各Protection Domainは独自の隔離された仮想アドレス空間を持ちます。たとえコンポーネントが他のコンポーネントのメモリの仮想アドレスを知っていても、明示的なケイパビリティマッピングなしではアクセスできません。

### CSpace隔離
各Protection Domainは独自のケイパビリティ空間を持ち、システム記述XMLファイルで明示的に付与されたケイパビリティのみを含みます。

### ケイパビリティマッピング
アクセス権はケイパビリティを通じて制御されます。ケイパビリティは特定のリソースに対する特定の権限を付与する偽造不可能なトークンです。

### 数学的証明
seL4は完全な形式検証を持つ最初のオペレーティングシステムカーネルであり、カーネル内でバッファオーバーフローが不可能であることを数学的に証明しています。

**重要な洞察**: 従来のOSセキュリティは「攻撃を困難にする」こと（確率的）に依存していますが、seL4は特定の攻撃を「数学的に不可能」にします（証明可能）。

##  トラブルシューティング

### ビルドエラー

SDKが正しくインストールされているか確認
make check-sdk

クリーンして再ビルド
make clean
make task2


### QEMUの問題

QEMUのインストールを確認
qemu-system-aarch64 --version

QEMUがARM CPUにアクセスできるか確認
qemu-system-aarch64 -cpu help | grep cortex-a53


##  追加リソース

- [seL4公式ドキュメント](https://docs.sel4.systems/)
- [Microkitユーザーマニュアル](https://docs.sel4.systems/projects/microkit/)
- [seL4形式検証](https://sel4.systems/Info/FAQ/proof.pml)

