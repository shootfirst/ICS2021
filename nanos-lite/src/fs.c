#include <fs.h>

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
  //********************pa3*****************
  size_t lseek_offset;
  int is_open;
  //********************pa3*****************
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

//**************************************pa3******************************************
size_t serial_write(const void *buf, size_t offset, size_t len);
size_t events_read(void *buf, size_t offset, size_t len);
size_t dispinfo_read(void *buf, size_t offset, size_t len);
size_t fb_write(const void *buf, size_t offset, size_t len);
//**************************************pa3******************************************

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  //******************************************pa3******************************************
  // 标准输入输出
  [FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, serial_write},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, serial_write},

  // 设备（键盘，显示器）
  {"/dev/events", 0, 0, events_read, NULL}, //键盘，只支持读
  {"/proc/dispinfo", 0, 0, dispinfo_read, NULL}, //显示器尺寸信息
  {"/dev/fb", 0, 0, NULL, fb_write}, //显示器
  
  
  // 文件
  {"/bin/nslider", 0, 1000000000},
//   {"/bin/hello", 33424, 400143},
// {"/bin/event-test", 57916, 433567},
// {"/bin/time-test", 57916, 491483},
// {"/bin/file-test", 48328, 549399},
// {"/bin/dummy", 29068, 597727},
// {"/bin/bmp-test", 58104, 626795},

  //******************************************pa3******************************************
#include "files.h"
};

void init_fs() {
  // TODO: initialize the size of /dev/fb
  //******************************************pa3******************************************
  AM_GPU_CONFIG_T gpu_config = io_read(AM_GPU_CONFIG);
  int gpu_fd = fs_open("/dev/fb", 0, 0);
  file_table[gpu_fd].size = gpu_config.width * gpu_config.height * sizeof(uint32_t);
  fs_close(gpu_fd);
  //******************************************pa3******************************************
}

//******************************************pa3******************************************
int fs_open(const char *pathname, int flags, int mode) {
  for (int i = 0; i < sizeof(file_table) / sizeof(Finfo); i++) {
    if (strcmp(file_table[i].name, pathname) == 0) {
      file_table[i].lseek_offset = 0;
      file_table[i].is_open = 1;
      return i;
    }
  }
  assert(0);
}

size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);

size_t fs_read(int fd, void *buf, size_t len) {
  Finfo *finfo = &file_table[fd];
  size_t read_len = 0;
  if (finfo->read == NULL) {
    read_len = finfo->size - finfo->lseek_offset < len ? finfo->size - finfo->lseek_offset : len;
    printf("wwwwwwwwwwwwwwwww %d\n", finfo->disk_offset + finfo->lseek_offset);
    assert(ramdisk_read(buf, finfo->disk_offset + finfo->lseek_offset, read_len) == read_len);
    finfo->lseek_offset += read_len;
  } else {
    // 意味着往标准输入读取的结果readlen是0？
    read_len = finfo->read(buf, finfo->lseek_offset, len);
    finfo->lseek_offset += read_len;
  }
  return read_len;
}

size_t fs_write(int fd, const void *buf, size_t len) {
  Finfo *finfo = &file_table[fd];
  size_t write_len = 0;
  if (finfo->write == NULL) {
    write_len = finfo->size - finfo->lseek_offset < len ? finfo->size - finfo->lseek_offset : len;
    assert(ramdisk_write(buf, finfo->disk_offset + finfo->lseek_offset, write_len) == write_len);
    finfo->lseek_offset += write_len;
  } else {
    // 意味着往标准输出写入的结果writelen是0？
    write_len = finfo->write(buf, finfo->lseek_offset, len);
    finfo->lseek_offset += write_len;
  }
  return write_len;
}

size_t fs_lseek(int fd, size_t offset, int whence) {
  Finfo *finfo = &file_table[fd];

  switch (whence)
  {
  case SEEK_SET :
    finfo->lseek_offset = offset;
    break;
  case SEEK_CUR :
    finfo->lseek_offset += offset;
    break;
  case SEEK_END :
    finfo->lseek_offset = finfo->size + offset;
    break;
  default:
    assert(0);
  }
  return finfo->lseek_offset;
}

int fs_close(int fd) {
  file_table[fd].lseek_offset = 0;
  file_table[fd].is_open = 0;
  return 0;
}
//******************************************pa3******************************************
