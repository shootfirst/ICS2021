#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  //****************************************pa2*******************************************
  size_t i = 0;
  for (; s[i] != 0; i++);
  return i;
  //****************************************pa2*******************************************
  panic("Not implemented");
}

char *strcpy(char *dst, const char *src) {
  //****************************************pa2*******************************************
  size_t i = 0;
  for (; src[i] != 0; i++) {
    dst[i] = src[i];
  }
  dst[i] = 0;
  return dst;
  //****************************************pa2*******************************************
  panic("Not implemented");
}

char *strncpy(char *dst, const char *src, size_t n) {
  //****************************************pa2*******************************************
  size_t i = 0;
  for (; i < n && src[i] != 0; i++) {
    dst[i] = src[i];
  }
  for (; i < n; i++) {
    dst[i] = 0;
  }
  return dst;
  //****************************************pa2*******************************************
  panic("Not implemented");
}

char *strcat(char *dst, const char *src) {
  //****************************************pa2*******************************************
  size_t i = 0;
  for (; dst[i] != 0; i++);
  size_t len_dst = i;
  for (; src[i - len_dst] != 0; i++) {
    dst[i] = src[i - len_dst];
  }
  dst[i] = 0;
  return dst;
  //****************************************pa2*******************************************
  panic("Not implemented");
}

int strcmp(const char *s1, const char *s2) {
  //****************************************pa2*******************************************
  size_t i = 0;
  for (; s1[i] != 0 && s2[i] != 0; i++) {
    if (s1[i] != s2[i]) {
      return s1[i] - s2[i];
    }
  }
  return s1[i] - s2[i];
  //****************************************pa2*******************************************
  panic("Not implemented");
}

int strncmp(const char *s1, const char *s2, size_t n) {
  //****************************************pa2*******************************************
  size_t i = 0;
  for (; s1[i] != 0 && s2[i] != 0 && i < n; i++) {
    if (s1[i] != s2[i]) {
      return s1[i] - s2[i];
    }
  }

  if (i == n) {
    return 0;
  }
  return s1[i] - s2[i];
  //****************************************pa2*******************************************
  panic("Not implemented");
}

void *memset(void *s, int c, size_t n) {
   //****************************************pa2*******************************************
  for (size_t i = 0; i < n; i++) {
    ((char*)s)[i] = (char)c;
  }
  return s;
  //****************************************pa2*******************************************
  panic("Not implemented");
}

void *memmove(void *dst, const void *src, size_t n) {
  //****************************************pa2*******************************************
  char res[n];
  for (size_t i = 0; i < n; i++) {
    res[i] = *((char*)src + i);
  }
  for (size_t i = 0; i < n; i++) {
    *((char*)dst + i) = res[i];
  }
  return dst;
  //****************************************pa2*******************************************
  panic("Not implemented");
}

void *memcpy(void *out, const void *in, size_t n) {
  //****************************************pa2*******************************************
  printf("%p\n", out);
  for (size_t i = 0; i < n; i++) {
    *((char*)out + i) = *((char*)in + i);
  }
  return out;
  //****************************************pa2*******************************************
  panic("Not implemented");
}

int memcmp(const void *s1, const void *s2, size_t n) {
  //****************************************pa2*******************************************
  for(int i = 0; i < n; ++i){
    char val1 = *((char*)s1 + i);
    char val2 = *((char*)s2 + i);
    if(val1 != val2) {
      return val1 - val2;
    }
  }
  return 0;
  //****************************************pa2*******************************************
  panic("Not implemented");
}

#endif
