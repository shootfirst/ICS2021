#include "sdb.h"

#define NR_WP 32

// ***********************************pa1**********************************
// typedef struct watchpoint {
//   int NO;
//   struct watchpoint *next;

//   /* TODO: Add more members if necessary */
//   // ***********************************pa1**********************************
//   int type;
//   vaddr_t addr;
//   char reg_name[4];
//   int pre_val;
//   // ***********************************pa1**********************************
// } WP;
// ***********************************pa1**********************************

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

// ***********************************pa1**********************************

WP* new_wp() {
  if (free_ != NULL) {
    WP *res = free_;
    // update free_
    free_ = free_->next;
    // head inset
    res->next = head;
    head = res;
    return res;
  }
  return NULL;
}

void free_wp(int to_del) {
  WP *it = head;
  WP *pre = it;
  while (it != NULL) {
    if (it->NO != to_del) {
      pre = it;
      it = it->next;
    } else {
      // find wp
      if (pre == it) {
        // it is head
        head = head->next;
      } else {
        // it is not head
        pre->next = it->next;
      }
      // head insert
      it->next = free_;
      free_ = it;
      break;
    }
  }
}

void view_pointer_display() {
  for (WP *it = head; it != NULL; it = it->next) {
    if (it->type == ADDR) {
      printf("%d th: address  0x%08lx\n", it->NO + 1, (unsigned long)it->addr);
    } else if (it->type == REG) {
      printf("%d th: register %s\n", it->NO + 1, it->reg_name);
    }
  }
}

int update_and_stop() {
  int ans = 0;
  for (WP *it = head; it != NULL; it = it->next) {
    if (it->type == ADDR) {
      int now = vaddr_read(it->addr, 4);
      if (now != it->pre_val) {
        it->pre_val = now;
        ans = 1;
      }
    } else if (it->type == REG) {
      bool success = true;
      int now = isa_reg_str2val(it->reg_name, &success);
      if (now != it->pre_val) {
        it->pre_val = now;
        ans = 1;
      }
    }
  }
  return ans;
}

//*************************************pa1**********************************

