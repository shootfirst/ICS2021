#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

// //*******************************pa3*******************************
// size_t ramdisk_read(void *buf, size_t offset, size_t len);
// static void load_seg(uint32_t p_vaddr, uint32_t p_offset, uint32_t p_filesz, uint32_t p_memsz) {
//   // 读取到内存指定位置
//   void *addr = (void*)p_vaddr;
//   size_t read_len;
//   read_len = ramdisk_read(addr, p_offset, p_filesz);
//   assert(read_len == p_filesz);
//   memset((char*)p_vaddr + p_filesz, 0, p_memsz - p_filesz);
// }

// //*******************************pa3*******************************

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
size_t ramdisk_read(void *buf, size_t offset, size_t len);
static uintptr_t loader(PCB *pcb, const char *filename) {
  
  Elf32_Ehdr elf_header;
  ramdisk_read(&elf_header, 0, sizeof(elf_header));
  //根据小端法 0x7F E L F
  assert(*(uint32_t *)elf_header.e_ident == 0x464c457f);
  
  Elf32_Off program_header_offset = elf_header.e_phoff;
  size_t headers_entry_size = elf_header.e_phentsize;
  int headers_entry_num = elf_header.e_phnum;

  for (int i = 0; i < headers_entry_num; ++i){
    Elf_Phdr section_entry;
    ramdisk_read(&section_entry, i * headers_entry_size + program_header_offset, sizeof(elf_header));
    void * virt_addr;
    switch (section_entry.p_type) {
    case PT_LOAD:
      virt_addr = (void *)section_entry.p_vaddr; 
      ramdisk_read(virt_addr, section_entry.p_offset, section_entry.p_filesz);
      memset(virt_addr + section_entry.p_filesz, 0, 
        section_entry.p_memsz - section_entry.p_filesz);
      break;
    
    default:
      break;
    }

  }
  
  return elf_header.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  //*******************************pa3*********************************
  // 直接把entry当成函数地址，调用之。。。
  //*******************************pa3*********************************
  ((void(*)())entry) ();
}

