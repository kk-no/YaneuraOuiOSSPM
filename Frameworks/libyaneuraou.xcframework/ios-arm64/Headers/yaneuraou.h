#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef int (*yaneuraou_usi_read_cb)();
typedef void (*yaneuraou_usi_write_cb)(int ch);
int yaneuraou_ios_main(yaneuraou_usi_read_cb usi_read, yaneuraou_usi_write_cb usi_write, const char* nnue_file_path);

#ifdef __cplusplus
}
#endif /* __cplusplus */