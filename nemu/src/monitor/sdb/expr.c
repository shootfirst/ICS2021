#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

//***************************************pa1***************************************
#include <memory/vaddr.h>
//***************************************pa1***************************************

enum {
  TK_NOTYPE = 256, TK_EQ,

  /* TODO: Add more token types */
  //***************************************pa1***************************************
  TK_NUM, // number
  TK_HNUM,  // hexadecimal num
  TK_NEQ,   // not equal
  TK_AND,   // and
  TK_REG,   // register name
  TK_DEREF, // deref *
  //***************************************pa1***************************************

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"==", TK_EQ},        // equal

  //***************************************pa1***************************************
  {"\\+", '+'},         // plus
  {"\\-", '-'},         // sub
  {"\\*", '*'},         // mul
  {"/", '/'},         // div
  {"\\(", '('},         // left brace
  {"\\)", ')'},         // right brace
  {"!=", TK_NEQ},        // not equal
  {"&&", TK_AND},        // and
  
  {"0[Xx][0-9a-fA-F]+", TK_HNUM},  // hexadecimal num
  {"[0-9a-fA-F]+[hH]", TK_HNUM},  // hexadecimal num
  {"[0-9]+", TK_NUM},  // num

  {"\\$[0-9a-zA-Z]+", TK_REG},   // register name
  //***************************************pa1***************************************
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        
        //***************************************pa1***************************************
        switch (rules[i].token_type) {
          case TK_NOTYPE :
            break;
          case '+' :
            tokens[nr_token].type = '+';
            nr_token++;
            break;
          case '-' :
            tokens[nr_token].type = '-';
            nr_token++;
            break;
          case '*' :
            tokens[nr_token].type = '*';
            nr_token++;
            break;
          case '/' :
            tokens[nr_token].type = '/';
            nr_token++;
            break;
          case '(' :
            tokens[nr_token].type = '(';
            nr_token++;
            break;
          case ')' :
            tokens[nr_token].type = ')';
            nr_token++;
            break;
          case TK_EQ :
            tokens[nr_token].type = TK_EQ;
            nr_token++;
            break;
          case TK_NUM :
            tokens[nr_token].type = TK_NUM;
            for (int j = 0; j < substr_len; j++) {
              tokens[nr_token].str[j] = *(substr_start + j);
            }
            tokens[nr_token].str[substr_len] = 0;
            nr_token++;
            break;

          case TK_HNUM :
            tokens[nr_token].type = TK_HNUM;
            for (int j = 0; j < substr_len; j++) {
              tokens[nr_token].str[j] = *(substr_start + j);
            }
            tokens[nr_token].str[substr_len] = 0;
            nr_token++;
            break;
          case TK_NEQ :
            tokens[nr_token].type = TK_NEQ;
            nr_token++;
            break;
          case TK_AND :
            tokens[nr_token].type = TK_AND;
            nr_token++;
            break;
          case TK_REG :
            tokens[nr_token].type = TK_REG;
            for (int j = 0; j < substr_len; j++) {
              tokens[nr_token].str[j] = *(substr_start + j);
            }
            tokens[nr_token].str[substr_len] = 0;
            nr_token++;
            break;
          
          default: 
            printf("!!!! panic\n");
        }
        break;
        //***************************************pa1***************************************
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

//***************************************pa1***************************************

int check_operator(int pos) {
  return tokens[pos].type == '+' || tokens[pos].type == '-' || tokens[pos].type == '*' || tokens[pos].type == '/' || 
          tokens[pos].type == TK_EQ || tokens[pos].type == TK_NEQ || tokens[pos].type == TK_AND || tokens[pos].type == TK_DEREF;
}

int find_master_token(int p, int q) {
  int ans = -1;
  int brace = 0;
  /* priority:
   * 0: &&
   * 1: == !=
   * 2: + -
   * 3: * /
   * 4: * (deref)
   */
  int priority = 4;
  for (int i = p; i <= q; i++) {
    if (brace == 0 && check_operator(i)) {
      if ((tokens[i].type == TK_AND) && priority >= 0) {
        priority = 0;
        ans = i;
      } else if ((tokens[i].type == TK_EQ || tokens[i].type == TK_NEQ) && priority >= 1) {
        priority = 1;
        ans = i;
      } else if ((tokens[i].type == '+' || tokens[i].type == '-') && priority >= 2) {
        priority = 2;
        ans = i;
      } else if ((tokens[i].type == '*' || tokens[i].type == '/') && priority >= 3) {
        priority = 3;
        ans = i;
      } else if (tokens[i].type == TK_DEREF && priority >= 4) {
        priority = 4;
        ans = i;
      } 
    } else if (tokens[i].type == '(') {
      brace++;
    } else if (tokens[i].type == ')') {
      brace--;
    }
  }

  if (ans == -1 || brace != 0) {
    return -1;
  }

  return ans;
}

int check_brace_correct(int p, int q) {
  int brace = 0;
  for (int i = p; i <= q; i++) {
    if (tokens[i].type == '(') {
      brace++;
    } 
    if (tokens[i].type == ')') {
      if (brace <= 0) {
        return 1;
      }
      brace--;
    }
  }
  return 0;
}

bool check_parentheses(int p, int q) {
  return tokens[p].type == '(' && tokens[q].type == ')' && check_brace_correct(p + 1, q - 1) == 0;
}

bool is_deref_prefix(int pos) {
  return tokens[pos].type != TK_NUM && tokens[pos].type != TK_HNUM && tokens[pos].type != TK_REG;
}

word_t atohx(const char *nums, bool *success) {
  int ans = 0;
  // 0x....
  if (nums[0] == '0') {
    for (int i = 2; nums[i] != 0; i++) {
      if ('0' <= nums[i] && nums[i] <= '9') {
        ans *= 16;
        ans += nums[i] - '0';
      } else if ('a' <= nums[i] && nums[i] <= 'f') {
        ans *= 16;
        ans += nums[i] - 'a' + 10;
      } else if ('A' <= nums[i] && nums[i] <= 'F') {
        ans *= 16;
        ans += nums[i] - 'A' + 10;
      } else {
        *success = false;
        return -1;
      }
    }
  // ...H
  } else {
    for (int i = 0; nums[i] != 'H' && nums[i] != 'h'; i++) {
      if ('0' <= nums[i] && nums[i] <= '9') {
        ans *= 16;
        ans += nums[i] - '0';
      } else if ('a' <= nums[i] && nums[i] <= 'f') {
        ans *= 16;
        ans += nums[i] - 'a' + 10;
      } else if ('A' <= nums[i] && nums[i] <= 'F') {
        ans *= 16;
        ans += nums[i] - 'A' + 10;
      } else {
        *success = false;
        return -1;
      }
    }
  }
  *success = true;
  return ans;
}

word_t evaluate(int p, int q, bool *success) {
  if (p > q) {
    *success = false;
    return 0;
  } else if (p == q) {
    /* Single token.
     * For now this token should be a number.
     * Return the value of the number.
     */
    if (tokens[p].type == TK_NUM) {
      return atoi(tokens[p].str);
    } else if (tokens[p].type == TK_HNUM) {
      return atohx(tokens[p].str, success);
    } else if (tokens[p].type == TK_REG) {
      return isa_reg_str2val(tokens[p].str + 1, success);
    }
  } 
  int res = check_brace_correct(p, q);
  if (res) {
    *success = false;
    return 0;
  } else if (check_parentheses(p, q) == true) {
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    return evaluate(p + 1, q - 1, success);
  } 
  
  int op = find_master_token(p, q);
  // word_t val1 = evaluate(p, op - 1, success);
  // word_t val2 = evaluate(op + 1, q, success);

  switch (tokens[op].type) {
    case TK_AND :
      return evaluate(p, op - 1, success) && evaluate(op + 1, q, success);
      break;
    case TK_EQ :
      return evaluate(p, op - 1, success) == evaluate(op + 1, q, success);
      break;
    case TK_NEQ :
      return evaluate(p, op - 1, success) != evaluate(op + 1, q, success);
      break;
    case '+': 
      return evaluate(p, op - 1, success) + evaluate(op + 1, q, success);
      break;
    case '-': 
      return evaluate(p, op - 1, success) - evaluate(op + 1, q, success);
      break;
    case '*': 
      return evaluate(p, op - 1, success) * evaluate(op + 1, q, success);
      break;
    case '/': 
      return evaluate(p, op - 1, success) / evaluate(op + 1, q, success);
      break;
    case TK_DEREF :
      word_t addr = evaluate(op + 1, q, success);
      return vaddr_read(addr, 4);
      break;
    default: 
      *success = false;
      return 0;
      break;
  }
}
//***************************************pa1***************************************

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  //***************************************pa1***************************************
  if (nr_token == 1 && (tokens[0].type != TK_NUM && tokens[0].type != TK_HNUM && tokens[0].type != TK_REG)) {
    *success = false;
    return 0;
  }
  for (int i = 0; i < nr_token; i++) {
    if (tokens[0].type == '*' && (i == 0 || is_deref_prefix(i))) {
      tokens[i].type = TK_DEREF;
    }
  }
  return evaluate(0, nr_token - 1, success);
  //***************************************pa1***************************************

}


