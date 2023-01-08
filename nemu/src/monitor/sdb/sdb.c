#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"

//***************************************pa1***************************************
#include <memory/vaddr.h>
//***************************************pa1***************************************

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  //***************************************pa1***************************************
  nemu_state.state = NEMU_QUIT;
  //***************************************pa1***************************************
  return -1;
}

//***************************************pa1***************************************
static int cmd_si(char *args);

static int cmd_info(char *args);

static int cmd_x(char *args);

static int cmd_p(char *args);

static int cmd_w(char *args);

static int cmd_d(char *args);
//***************************************pa1***************************************

static int cmd_help(char *args);

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  /* TODO: Add more commands */
  //***************************************pa1***************************************
  { "si", "Single step, use like \"si [N]\", N means step numbers", cmd_si },
  { "info", "Print register or view pointer info, use like \"info r\" or \"info w\"", cmd_info },
  { "x", "Print memory content, use like \"x N EXPR\"", cmd_x },
  { "p", "Print expression value, use like \"p EXPR\"", cmd_p },
  { "w", "Set view pointer, use like \"w EXPR\"", cmd_w },
  { "d", "Delete view pointer, use like \"d N\"", cmd_d },
  //***************************************pa1*************************************** 
};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}



//***************************************pa1***************************************
static int cmd_si(char *args) {
  int step_num = 1;

  if (args != NULL) {
    step_num = atoi(args);
  } else {
    printf("please input args!!!\n");
    return 0;
  }
  
  if (step_num <= 0 || step_num >= 10000) {
    printf("si:: invalid args %s, the args should be number and > 0 and < 10000\n", args);
    return 0;
  }
  cpu_exec(step_num);
  return 0;
}

static int cmd_info(char *args) {
  if (args == NULL) {
    printf("please input args!!!\n");
    return 0;
  }

  if (strcmp(args, "r") == 0) {
    isa_reg_display();
  } else if (strcmp(args, "w") == 0) {
    view_pointer_display();
  }

  return 0;
  
}

static int cmd_x(char *args) {
  if (args != NULL) {
    char *n = strtok(args, " ");
    if (n == NULL) {
      return 0;
    }
    char *exp = n + strlen(n) + 1;
    if (exp == NULL) {
      return 0;
    }
    
    bool success;
    unsigned long exp_ans = expr(exp, &success);
    printf("%ld\n", exp_ans);
    if (success == true) {
      int N = atoi(n);
      printf("addr              value\n");
      for (int j = 0; j < N; j++) {
        printf("0x%lx        0x%08lx\n", (exp_ans + j * 4), (unsigned long)vaddr_read(exp_ans + j * 4, 4));
      }
    }
  }
  return 0;
}

static int cmd_p(char *args) {
  if (args == NULL) {
    return 0;
  }

  bool success = true;
  int exp_ans = expr(args, &success);

  if (success == true) {
    printf("%d\n", exp_ans);
  } else {
    printf("analyse %s wrong \n", args);
  }
  
  return 0;
}

static int cmd_w(char *args) {
  if (args == NULL) {
    return 0;
  }

  bool success = true;
  // args is like *exp
  if (args[0] == '*') {
    vaddr_t addr = expr(args + 1, &success);
    if (success == false) {
      printf("w:: invalid address %s!!!\n", args + 1);
      return 0;
    }
    WP* wp = new_wp();
    if (wp == NULL) {
      printf("wp is run out!!!\n");
      return 0;
    }
    wp->type = ADDR;
    wp->addr = addr;
    wp->pre_val = vaddr_read(addr, 4);
  // args is like $reg_name 
  } else if (args[0] == '$') {
    int reg_val = isa_reg_str2val(args + 1, &success);
    if (success == false) {
      printf("w:: invalid register name %s!!!\n", args + 1);
      return 0;
    }
    WP* wp = new_wp();
    if (wp == NULL) {
      printf("wp is run out!!!\n");
      return 0;
    }
    wp->type = REG;
    int i = 1;
    for (; args[i] != 0; i++) {
      wp->reg_name[i - 1] = args[i];
    }
    wp->reg_name[i - 1] = 0;
    wp->pre_val = reg_val;
  } else {
    printf("w:: invalid expression %s!!!\n", args);
  }

  return 0;
}

static int cmd_d(char *args) {
  if (args == NULL) {
    return 0;
  }
  int to_del = atoi(args);
  free_wp(to_del);
  return 0;
}
//***************************************pa1***************************************



void sdb_set_batch_mode() {
  is_batch_mode = true;
}

void sdb_mainloop() {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { 
          return; 
        }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
