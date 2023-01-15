#include <proc.h>
#include <elf.h>
//*********************************pa3***************************************
#include <fs.h>
//*********************************pa3***************************************

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

//*******************************pa3*******************************
static void load_seg(int fd, uint32_t p_vaddr, uint32_t p_offset, uint32_t p_filesz, uint32_t p_memsz) {
  // 读取到内存指定位置
  void *addr = (void*)p_vaddr;
  size_t read_len;

  fs_lseek(fd, p_offset, SEEK_SET);
  read_len = fs_read(fd, addr, p_filesz);
  
  assert(read_len == p_filesz);
  memset((char*)p_vaddr + p_filesz, 0, p_memsz - p_filesz);
}
//*******************************pa3*******************************

static uintptr_t loader(PCB *pcb, const char *filename) {
  //*******************************pa3*********************************
  assert(filename != NULL);
  int fd = fs_open(filename, 0, 0);

  Elf32_Ehdr elf_header;
  Elf32_Phdr pgm_header;
  // 读取elf header
  size_t read_len;
  // 从偏移量为0，读取长度为sizeof(Elf32_Ehdr)字节到elf_header中
  fs_lseek(fd, 0, SEEK_SET);
  read_len = fs_read(fd, (void*)(&elf_header), sizeof(Elf32_Ehdr));
  printf("wwwwwwwwwwwwwwwww\n");
  assert(read_len == sizeof(Elf32_Ehdr));

  // 魔数检查，前4字节是魔数，注意riscv为小端序
  assert(*(uint32_t*)(elf_header.e_ident) == 0x464C457FU);
  // 文件类型检查，应该是可执行文件
  assert(elf_header.e_type == ET_EXEC);
  // isa检查，应该是riscv
  assert(elf_header.e_machine == EM_RISCV);
  // elf version 检查，应该大于1
  assert(elf_header.e_version >= 1);
  
  // 将可以加载的seg从elf文件加载进内存的特定位置
  int i, off;
  for (i = 0, off = elf_header.e_phoff; i < elf_header.e_phnum; i++, off += sizeof(Elf32_Phdr)) {

    // 读取pro header信息
    fs_lseek(fd, off, SEEK_SET);
    read_len = fs_read(fd, (void*)(&pgm_header), sizeof(Elf32_Phdr));

    assert(read_len == sizeof(Elf32_Phdr));

    // 如果不能加载进入内存，继续
    if (pgm_header.p_type != PT_LOAD) {
      continue;
    }

    assert(pgm_header.p_memsz >= pgm_header.p_filesz);

    assert(pgm_header.p_memsz >= 0);

    // 加载入内存
    load_seg(fd, pgm_header.p_vaddr, pgm_header.p_offset, pgm_header.p_filesz, pgm_header.p_memsz);
  }
  
  fs_close(fd);
  return elf_header.e_entry;
  //*******************************pa3*********************************
}

// //*******************************pa3*******************************
// static void load_seg(uint32_t p_vaddr, uint32_t p_offset, uint32_t p_filesz, uint32_t p_memsz) {
//   // 读取到内存指定位置
//   void *addr = (void*)p_vaddr;
//   size_t read_len;
//   read_len = ramdisk_read(addr, p_offset, p_filesz);
//   assert(read_len == p_filesz);
//   memset((char*)p_vaddr + p_filesz, 0, p_memsz - p_filesz);
// }

// static uintptr_t loader(PCB *pcb, const char *filename) {
//   //*******************************pa3*********************************
//   Elf32_Ehdr elf_header;
//   Elf32_Phdr pgm_header;
//   // 读取elf header
//   size_t read_len;
//   // 从偏移量为0，读取长度为sizeof(Elf32_Ehdr)字节到elf_header中
//   read_len = ramdisk_read((void*)(&elf_header), 0, sizeof(Elf32_Ehdr));
//   assert(read_len == sizeof(Elf32_Ehdr));

//   // 魔数检查，前4字节是魔数，注意riscv为小端序
//   assert(*(uint32_t*)(elf_header.e_ident) == 0x464C457FU);
//   // 文件类型检查，应该是可执行文件
//   assert(elf_header.e_type == ET_EXEC);
//   // isa检查，应该是riscv
//   assert(elf_header.e_machine == EM_RISCV);
//   // elf version 检查，应该大于1
//   assert(elf_header.e_version >= 1);

//   // 将可以加载的seg从elf文件加载进内存的特定位置
//   int i, off;
//   for (i = 0, off = elf_header.e_phoff; i < elf_header.e_phnum; i++, off += sizeof(Elf32_Phdr)) {
//     // 读取pro header信息
//     read_len = ramdisk_read((void*)(&pgm_header), off, sizeof(Elf32_Phdr));
//     assert(read_len == sizeof(Elf32_Phdr));

//     // 如果不能加载进入内存，继续
//     if (pgm_header.p_type != PT_LOAD) {
//       continue;
//     }

//     assert(pgm_header.p_memsz >= pgm_header.p_filesz);

//     assert(pgm_header.p_memsz >= 0);

//     // 加载入内存
//     load_seg(pgm_header.p_vaddr, pgm_header.p_offset, pgm_header.p_filesz, pgm_header.p_memsz);
//   }
//   return elf_header.e_entry;
//   //*******************************pa3*********************************

//   TODO();
//   return 0;
// }

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

