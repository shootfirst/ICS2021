#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  //****************************************pa2*******************************************
  char out[512];
  memset(out, 0, 512);
  va_list p;
  va_start(p,fmt);
  int res = vsprintf(out, fmt, p);
  va_end(p);
  putstr(out);
  return res;
  //****************************************pa2*******************************************
  panic("Not implemented");
}

//****************************************pa2*******************************************
void _itoa(int num, char* buf) {
  int len = 0;
  for (; num != 0; len++) {
    buf[len] = num % 10 + '0';
    num /= 10;
  }
  for (int j = 0; j < len / 2; j++) {
    char tmp = buf[j];
    buf[j] = buf[len - j - 1];
    buf[len - j - 1] = tmp;
  }
}
//****************************************pa2*******************************************

int vsprintf(char *out, const char *fmt, va_list ap) {
  //****************************************pa2*******************************************
  char* res = out;
  while(*fmt) {
    int n = 1;
    if (*fmt != '%') {
      *out = *fmt;
    } else {
      fmt++;
      switch (*fmt)
      {
      case 's':
        //获得一个char*无名参数，同时p指向下一个无名参数
        char *c = va_arg(ap, char *);
        n = strlen(c);
        strncpy(out, c, n);
        break;
      case 'd':
        //获得一个int无名参数，同时p指向下一个无名参数
        int i = va_arg(ap, int);
        char buf[32];
        memset(buf, 0, 32);
        if(i == 0){
          *out = '0';
          break;
        }
				_itoa(i, buf);
        n = strlen(buf);
				memcpy(out, buf, n);

        break;
      default:
      // 暂时只支持%s和%d
      // return -1;
      }
    }
    ++fmt;
    out += n;
  }
  // 必须加
  *out = '\0';
  return out - res;
  //****************************************pa2*******************************************
  panic("Not implemented");
}


int sprintf(char *out, const char *fmt, ...) {
  //****************************************pa2*******************************************
  //将有名参数fmt作为起点，p此时指向第一个无名参数
  va_list p;
  va_start(p,fmt);
  int res = vsprintf(out, fmt, p);
  va_end(p);
  return res;
  //****************************************pa2*******************************************
  panic("Not implemented");
}



int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
