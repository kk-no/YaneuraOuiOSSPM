# YaneuraOuiOSSPM

やねうら王をiOS向けのSwift Package Managerパッケージとしてビルドするスクリプト。

試作版。水匠埋め込み試作中。

# ビルド

Xcode 16.0, cmake 3.24.1

Suisho petit
```
git submodule init
git submodule update
cd build
curl --create-dirs -RLo .dl/suishopetite_20211123.k_p.nnue.cpp.gz https://github.com/mizar/YaneuraOu/releases/download/resource/suishopetite_20211123.k_p.nnue.cpp.gz
gzip -cd .dl/suishopetite_20211123.k_p.nnue.cpp.gz > .dl/embedded_nnue.cpp
./build.bash
```

Suisho5
```
git submodule init
git submodule update
cd build
curl --create-dirs -RLo .dl/suisho5_20211123.halfkp.nnue.cpp.gz https://github.com/mizar/YaneuraOu/releases/download/resource/suisho5_20211123.halfkp.nnue.cpp.gz
gzip -cd .dl/suisho5_20211123.halfkp.nnue.cpp.gz > .dl/embedded_nnue.cpp
./build.bash
```

`CmakeLists.txt`(アーキテクチャ別に2個ある)の`target_compile_options`の中の`-DEVAL_NNUE_KP256`を削除。

# インターフェース変更

インターフェースとなる関数は `build/src/ios_main.cpp` に記述する。

関数シグネチャは `build/include/yaneuraou.h`及び`Sources/YaneuraOuiOSSPM/include/yaneuraou.h`に同じ内容を記述する。

ビルドするソースコードリスト、コンパイラオプションは `build/ios_{arm64,x86_64}/CMakeLists.txt` に記述する。(本当は１ファイルにまとめたいのだがcmakeの知識が足りないだけ)

# git　タグ

パッケージのバージョンはgitのタグで識別される。`1.2.3`のように3つの数字で表す。

# ライセンス

GPLv3 (やねうら王本体に従います)

ただし、 `ios.toolchain.cmake`([取得元](https://raw.githubusercontent.com/leetal/ios-cmake/master/ios.toolchain.cmake)) は修正BSDライセンス。
