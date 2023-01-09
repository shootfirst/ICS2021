#ifndef __SDB_H__
#define __SDB_H__

#include <common.h>
//************************************pa1******************************************
#include <memory/vaddr.h>
#include <isa.h>
//************************************pa1******************************************

word_t expr(char *e, bool *success);

//************************************pa1******************************************

enum watchpointtype {
  ADDR, REG,
};

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  // ***********************************pa1**********************************
  int type;
  vaddr_t addr;
  char reg_name[4];
  int pre_val;
  // ***********************************pa1**********************************
} WP;

WP* new_wp();

void free_wp(int to_del);

void view_pointer_display();

int update_and_stop();
//************************************pa1******************************************

#endif
