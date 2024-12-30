"""
WatchOS用、水匠5評価関数を用いるにあたってのやねうら王ソースコードの書き換えを行う
"""

import re
from pathlib import Path

def get_encoding(path: Path):
    if path.name == "halfkp_vm_256x2-32-32.h":
        return "cp932"
    else:
        return "utf-8-sig" # BOMを削除して読む

def special_patch(source: str, path: Path):
    if path.name == "yaneuraou-search.cpp":
        # FV_SCALEを水匠5に合わせた24にする
        source = source.replace('o["FV_SCALE"] << Option(16, 1, 128);', 'o["FV_SCALE"] << Option(24, 1, 128);')
    elif path.name == "evaluate_nnue.cpp":
        # FV_SCALEを水匠5に合わせた24にする
        source = source.replace("int FV_SCALE = 16;", "int FV_SCALE = 24;")
        source = source.replace("namespace Eval {", "extern std::string nnue_file_path;\nnamespace Eval {")
        source = source.replace("const std::string file_path = Path::Combine(dir_name, file_name);", "const std::string file_path = nnue_file_path;")
    elif path.name == "YaneuraOu_dlshogi_bridge.cpp":
        # DNN_Model1を空欄にする
        source = source.replace('R"(model.mlmodel)"', '""')
    elif path.name == "usi_option.cpp":
        # デフォルトハッシュサイズを64MBにする
        # SuishoPetite評価関数なら128MBで行けるが、水匠5では64MBにしないとメモリ不足で起動できない。(Apple Watch Series 9の場合)
        # source = source.replace('o["USI_Hash"] << Option(1024, 1, MaxHashMB, [](const Option& o) { /* on_hash_size(o); */ });', 'o["USI_Hash"] << Option(64, 1, MaxHashMB, [](const Option& o) { /* on_hash_size(o); */ });')
        # 通信遅延が大きめなので切れ負けに対して5000msの余裕を持つ
        source = source.replace('o["NetworkDelay2"] << Option(time_margin + 1000, 0, 10000);', 'o["NetworkDelay2"] << Option(time_margin + 5000, 0, 10000);')
        # コア数を2に設定(Apple Watch Series 9のコア数)
        source = source.replace('o["Threads"] << Option(4, 1, 1024, [](const Option& o) { /* on_threads(o); */ });', 'o["Threads"] << Option(2, 1, 1024, [](const Option& o) { /* on_threads(o); */ });')

    return source

def modify_source(path: Path):
    dst_source = path.read_text(encoding=get_encoding(path))
    dst_source = special_patch(dst_source, path)
    path.write_text(dst_source, encoding="utf-8")

def main():
    source_dir = Path('YaneuraOu/source')
    source_paths = []
    # YaneuraOu/source/eval/deep/nn_coreml.mm は使用しない
    for pattern in ["**/*.cpp", "**/*.h", "**/*.hpp"]:
        source_paths.extend(source_dir.glob(pattern))
    for source_path in source_paths:
        print(str(source_path))
        modify_source(source_path)

if __name__ == '__main__':
    main()