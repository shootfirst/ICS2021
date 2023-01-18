#include <nterm.h>
#include <stdarg.h>
#include <unistd.h>
#include <SDL.h>

char handle_key(SDL_Event *ev);

static void sh_printf(const char *format, ...) {
  static char buf[256] = {};
  va_list ap;
  va_start(ap, format);
  int len = vsnprintf(buf, 256, format, ap);
  va_end(ap);
  term->write(buf, len);
}

static void sh_banner() {
  sh_printf("Built-in Shell in NTerm (NJU Terminal)\n\n");
}

static void sh_prompt() {
  sh_printf("sh> ");
}

static void sh_handle_cmd(const char *cmd) {
  //*****************************************pa3********************************************
  // 注意回车符 "/bin/hello\n" 是终端输入内容，需要去除回车符，不然底层不匹配
  char path[32];
  memset(path, 0, 32);
  for (int i = 0; cmd[i] != '\n'; i++) {
    path[i] = cmd[i];
  }
  execve(path, 0, 0);
  //*****************************************pa3********************************************
}

void builtin_sh_run() {
  sh_banner();
  sh_prompt();

  while (1) {
    SDL_Event ev;
    if (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_KEYUP || ev.type == SDL_KEYDOWN) {
        
        const char *res = term->keypress(handle_key(&ev));
        if (res) {
          printf("hhhhhhhhhhhhhhhhhhhhh %d %s\n", ev.key.keysym.sym, res);
          sh_handle_cmd(res);
          sh_prompt();
        }
      }
    }
    refresh_terminal();
  }
}
