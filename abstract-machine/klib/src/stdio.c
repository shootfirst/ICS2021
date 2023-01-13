// #include <am.h>
// #include <klib.h>
// #include <klib-macros.h>
// #include <stdarg.h>

// #if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

// int printf(const char *fmt, ...) {
//   //****************************************pa2*******************************************
//   char out[512];
//   memset(out, 0, 512);
//   va_list p;
//   va_start(p,fmt);
//   int res = vsprintf(out, fmt, p);
//   va_end(p);
//   putstr(out);
//   return res;
//   //****************************************pa2*******************************************
//   panic("Not implemented");
// }

// //****************************************pa2*******************************************
// void _itoa(int num, char* buf) {
//   int len = 0;
//   for (; num != 0; len++) {
//     buf[len] = num % 10 + '0';
//     num /= 10;
//   }
//   for (int j = 0; j < len / 2; j++) {
//     char tmp = buf[j];
//     buf[j] = buf[len - j - 1];
//     buf[len - j - 1] = tmp;
//   }
// }
// //****************************************pa2*******************************************

// int vsprintf(char *out, const char *fmt, va_list ap) {
//   //****************************************pa2*******************************************
//   char* res = out;
//   while(*fmt) {
//     int n = 1;
//     if (*fmt != '%') {
//       *out = *fmt;
//     } else {
//       fmt++;
//       switch (*fmt)
//       {
//       case 's':
//         //获得一个char*无名参数，同时p指向下一个无名参数
//         char *c = va_arg(ap, char *);
//         n = strlen(c);
//         strncpy(out, c, n);
//         break;
//       case 'd':
//         //获得一个int无名参数，同时p指向下一个无名参数
//         int i = va_arg(ap, int);
//         char buf[32];
//         memset(buf, 0, 32);
//         if(i == 0){
//           *out = '0';
//           break;
//         }
// 				_itoa(i, buf);
//         n = strlen(buf);
// 				memcpy(out, buf, n);

//         break;
//       default:
//       // 暂时只支持%s和%d
//       // return -1;
//       }
//     }
//     ++fmt;
//     out += n;
//   }
//   // 必须加
//   *out = '\0';
//   return out - res;
//   //****************************************pa2*******************************************
//   panic("Not implemented");
// }


// int sprintf(char *out, const char *fmt, ...) {
//   //****************************************pa2*******************************************
//   //将有名参数fmt作为起点，p此时指向第一个无名参数
//   va_list p;
//   va_start(p,fmt);
//   int res = vsprintf(out, fmt, p);
//   va_end(p);
//   return res;
//   //****************************************pa2*******************************************
//   panic("Not implemented");
// }



// int snprintf(char *out, size_t n, const char *fmt, ...) {
//   panic("Not implemented");
// }

// int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
//   panic("Not implemented");
// }

// #endif


#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

#if defined(CONFIG_ISA64)
typedef uint64_t word_t;
#else
typedef uint32_t word_t;
#endif

#define NUM_LEN 32
#define OSTRING_LEN 5120

int printf(const char *fmt, ...)
{
  char out[OSTRING_LEN];

  va_list ap;
  va_start(ap, fmt);
  int num = vsprintf(out, fmt, ap);
  va_end(ap);

  for (int i = 0; i < num; ++i)
  {
    putch(out[i]);
  }

  return num;
}

static char *__itoa(int num, char *buff, uint16_t base)
{
  static const char sym[] = "0123456789abcdef";

  char tmp[NUM_LEN];
  bool is_neg = false;
  if (num == 0)
  {
    strcpy(buff, "0");
    return buff;
  }
  else if (num < 0)
  {
    is_neg = true;
    strcpy(buff, "-");
    buff++;
    num = -num;
  }

  uint8_t i = 0;
  while (num != 0)
  {
    tmp[i] = sym[num % base];
    num /= base;
    i++;
  }

  for (int j = i - 1; j >= 0; --j)
    buff[i - 1 - j] = tmp[j];
  buff[i] = '\0';

  return is_neg ? (buff - 1) : buff;
}

static char *__ptoa(void *p, char *buff)
{
  static const char sym[] = "0123456789abcdef";

  word_t num = (word_t)p;
  char tmp[NUM_LEN];

  if (num == 0)
  {
    buff[0] = '0';
    buff[1] = '\0';
    return (buff - 1);
  }

  uint8_t i = 0;
  while (num != 0)
  {
    tmp[i] = sym[num % 16];
    num /= 16;
    i++;
  }

  for (int j = i - 1; j >= 0; --j)
    buff[i - 1 - j] = tmp[j];
  buff[i] = '\0';

  return (buff - 1);
}

int vsprintf(char *out, const char *fmt, va_list ap)
{
  char *pout = out;
  size_t i = 0;
  while (fmt[i] != '\0')
  {
    if (fmt[i] != '%')
    {
      *(pout++) = fmt[i];
    }
    else
    {
      bool is_end = false;
      char num_leftpad_sym = ' ';
      bool left_align = false;
      int width = 0;
      while (!is_end)
      {
        i++;
        switch (fmt[i])
        {
        case '0':
        {
          if (fmt[i - 1] == '%')
          {
            num_leftpad_sym = '0';
            break;
          }
        }
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
          width = width * 10 + (fmt[i] - '0');
          break;
        }
        case '-':
        {
          if (fmt[i - 1] == '%')
          {
            left_align = true;
          }
          break;
        }
        case 's':
        {
          const char *str = va_arg(ap, char *);
          if (!left_align)
          {
            for (int j = 0; j < width - (int)strlen(str); ++j)
            {
              *(pout++) = ' ';
            }
            strcpy(pout, str);
            pout += strlen(str);
          }
          else
          {
            strcpy(pout, str);
            pout += strlen(str);
            for (int j = 0; j < width - (int)strlen(str); ++j)
            {
              *(pout++) = ' ';
            }
          }
          is_end = true;
          break;
        }
        case 'd':
        case 'i':
        case 'x':
        {
          int val = va_arg(ap, int);

          char buff[NUM_LEN];
          __itoa(val, buff, (fmt[i] == 'x') ? 16 : 10);
          if (!left_align)
          {
            for (int j = 0; j < width - (int)strlen(buff); ++j)
              *(pout++) = num_leftpad_sym;
            strcpy(pout, buff);
            pout += strlen(buff);
          }
          else
          {
            strcpy(pout, buff);
            pout += strlen(buff);
            for (int j = 0; j < width - (int)strlen(buff); ++j)
              *(pout++) = num_leftpad_sym;
          }
          is_end = true;
          break;
        }
        case 'p':
        {
          void *val = va_arg(ap, void *);

          char buff[NUM_LEN];
          __ptoa(val, buff);
          if (!left_align)
          {
            for (int j = 0; j < width - (int)strlen(buff); ++j)
              *(pout++) = num_leftpad_sym;
            strcpy(pout, buff);
            pout += strlen(buff);
          }
          else
          {
            strcpy(pout, buff);
            pout += strlen(buff);
            for (int j = 0; j < width - (int)strlen(buff); ++j)
              *(pout++) = num_leftpad_sym;
          }
          is_end = true;
          break;
        }
        default:
        {
          printf("print format is wrong!\n");
          // Wrong format!!!
          assert(0);
        }
        }
      }
    }
    ++i;
  }
  *pout = '\0';

  va_end(ap);

  return strlen(out);
}

int sprintf(char *out, const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  int num = vsprintf(out, fmt, ap);
  va_end(ap);

  return num;
}

// int snprintf(char *out, size_t n, const char *fmt, ...)
// {
//   panic("Not implemented");
// }

// int vsnprintf(char *out, size_t n, const char *fmt, va_list ap)
// {
//   panic("Not implemented");
// }

static char HEX_CHARACTERS[] = "0123456789ABCDEF";
#define BIT_WIDE_HEX 8

int snprintf(char *out, size_t n, const char *fmt, ...) {
  va_list arg;
  va_start (arg, fmt);

  int done = vsnprintf(out, n, fmt, arg);

  va_end(arg);
  return done;
}

#define append(x) {out[j++]=x; if (j >= n) {break;}}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  char buffer[128];
  char *txt, cha;
  int num, len;
  unsigned int unum;
  uint32_t pointer;
  
  
  int state = 0, i, j;//模仿一个状态机
  for (i = 0, j = 0; fmt[i] != '\0'; ++i){
    switch (state)
    {
    case 0:
      if (fmt[i] != '%'){
        append(fmt[i]);
      } else
        state = 1;
      break;
    
    case 1:
      switch (fmt[i])
      {
      case 's':
        txt = va_arg(ap, char*);
        for (int k = 0; txt[k] !='\0'; ++k)
          append(txt[k]);
        break;
      
      case 'd':
        num = va_arg(ap, int);
        if(num == 0){
          append('0');
          break;
        }
        if (num < 0){
          append('-');
          num = 0 - num;
        }
        for (len = 0; num ; num /= 10, ++len)
          //buffer[len] = num % 10 + '0';//逆序的
          buffer[len] = HEX_CHARACTERS[num % 10];//逆序的
        for (int k = len - 1; k >= 0; --k)
          append(buffer[k]);
        break;
      
      case 'c':
        cha = (char)va_arg(ap, int);
        append(cha);
        break;

      case 'p':
        pointer = va_arg(ap, uint32_t);
        for (len = 0; pointer ; pointer /= 16, ++len)
          buffer[len] = HEX_CHARACTERS[pointer % 16];//逆序的
        for (int k = 0; k < BIT_WIDE_HEX - len; ++k)
          append('0');

        for (int k = len - 1; k >= 0; --k)
          append(buffer[k]);
        break;

      case 'x':
        unum = va_arg(ap, unsigned int);
        if(unum == 0){
          append('0');
          break;
        }
        for (len = 0; unum ; unum >>= 4, ++len)
          buffer[len] = HEX_CHARACTERS[unum & 0xF];//逆序的

        for (int k = len - 1; k >= 0; --k)
          append(buffer[k]);
        break;  

      default:
        assert(0);
      }
      state = 0;
      break;
      
    }
  }

  out[j] = '\0';
  return j;
}

#endif
