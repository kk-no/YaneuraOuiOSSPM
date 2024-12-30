//#include <iostream>
//#include "bitboard.h"
//#include "position.h"
#include "../../YaneuraOu/source/search.h"
#include "../../YaneuraOu/source/thread.h"
#include "../../YaneuraOu/source/tt.h"
#include "../../YaneuraOu/source/usi.h"
#include "../../YaneuraOu/source/misc.h"
#include "../include/yaneuraou.h"

// cin/coutをsocketにリダイレクト
#include <iostream>
#include <unistd.h>
#include <sys/types.h>

std::string nnue_file_path;

class myoutstreambuf : public std::streambuf {
private:
	yaneuraou_usi_write_cb usi_write;

public:
    myoutstreambuf(yaneuraou_usi_write_cb _usi_write): usi_write(_usi_write) {
    }

protected:
    int overflow(int nCh = EOF) {
		usi_write(nCh);
        return nCh;
    }
};

class myinstreambuf : public std::streambuf {
private:
	yaneuraou_usi_read_cb usi_read;

public:
    myinstreambuf(yaneuraou_usi_read_cb _usi_read): usi_read(_usi_read) {
    }

protected:
    int uflow() {
		return usi_read();
    }

};

static void yaneuraou_ios_thread_main(yaneuraou_usi_read_cb usi_read, yaneuraou_usi_write_cb usi_write) {
	myoutstreambuf outs(usi_write);
	myinstreambuf ins(usi_read);
	auto default_out = std::cout.rdbuf(&outs);
	auto default_in = std::cin.rdbuf(&ins);

	// --- 全体的な初期化
	int argc = 1;
	char argv0[] = "yaneuraou";
	char *argv[] = {argv0};
	CommandLine::init(argc,argv);
	USI::init(Options);
	Bitboards::init();
	Position::init();
	Search::init();

	// エンジンオプションの"Threads"があるとは限らないので…。
	size_t thread_num = Options.count("Threads") ? (size_t)Options["Threads"] : 1;
	Threads.set(thread_num);

	//Search::clear();
	Eval::init();

#if !defined(__EMSCRIPTEN__)
	// USIコマンドの応答部

	USI::loop(argc, argv);

	// 生成して、待機させていたスレッドの停止

	Threads.set(0);
#else
	// yaneuraOu.wasm
	// ここでループしてしまうと、ブラウザのメインスレッドがブロックされてしまうため、コメントアウト
#endif

	std::cout.rdbuf(default_out);
	std::cin.rdbuf(default_in);
    
	// maybe send disconnect message
}

extern "C" int yaneuraou_ios_main(yaneuraou_usi_read_cb usi_read, yaneuraou_usi_write_cb usi_write, const char* nnue_file_path) {
    ::nnue_file_path = nnue_file_path;
	std::thread thread(yaneuraou_ios_thread_main, usi_read, usi_write);
    thread.detach();
    return 0;
}
