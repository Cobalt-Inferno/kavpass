#include "generator.h"

#define ARR_S(arr) (sizeof(arr) / sizeof((arr)[0])) 

const size_t BUF_SIZE = 2096;

void init(Password *pass) {
  pass->symb = "!@#$%^&*()-=+_][{}";
  pass->l_let = "qwertyuiopasdfghjklzxcvbnm";
  pass->u_let = "QWERTYUIOPASDFGHJKLZXCVBNM";
  pass->ints = "1234567890";
  pass->test_symb = "™€‰—®©☺⚛";
  pass->Pass = (char *) malloc(BUF_SIZE * sizeof(char));
  pass->file = (char *) malloc(BUF_SIZE * sizeof(char));
}

int pull_rand() {
  FILE * file = fopen("/dev/random", "r");            
  unsigned char buff[10];
  if (!file) {
    perror("fopen");
    return EXIT_FAILURE;
  }
  size_t retv = fread(buff, sizeof(*buff), ARR_S(buff), file); /* I have to add some error checking */
  if (retv != ARR_S(buff)) {
    perror("fread(): failed.");
    return EXIT_FAILURE;
  }
  int temp = buff[0];
  fclose(file);
  return temp;
}

void safe_return_ran(int line, Password *pass) {
  if (line >=6) {
    pass->failed = true;
  }else {
    switch(line) {
    case 1:
      pass->tmp_c = pass->symb[pull_rand() % (int) strlen(pass->symb)];
      break;
    case 2:
      pass->tmp_c = pass->l_let[pull_rand() % (int) strlen(pass->l_let)];
      break;
    case 3:
      pass->tmp_c = pass->u_let[pull_rand() % (int) strlen(pass->u_let)];
      break;
    case 4:
      pass->tmp_c = pass->ints[pull_rand() % (int) strlen(pass->ints)];
      break;
    case 5:
      pass->tmp_c = pass->test_symb[pull_rand() % (int) strlen(pass->test_symb)];
      break;
    default:
      pass->failed = true;
      break;
    }
  }
}

void safe_set_pass(Password *pass) {
 LOOP:
  if (pass->prefix) {
    for(size_t i = 0; (int) i < pass->len + (int) strlen(pass->pref); i++) {
      if (i >= strlen(pass->pref)) {
        if (pass->test_symb_b) {
          safe_return_ran(pull_rand() % 5 + 1, pass);
        } else {
          safe_return_ran(pull_rand() % 4 + 1, pass);
        }
        pass->Pass[i] = pass->tmp_c;
        if (pass->failed) {
          return;
        }
      }
      else {
        pass->Pass[i] = pass->pref[i];
      }
    }
    if (strlen(pass->Pass) != (size_t)pass->len + strlen(pass->pref)) {
      goto LOOP;
      printf("not");
    }
  } 
  else {
    for(int i = 0; i < pass->len; i++) {
      if (pass->test_symb_b) {
        safe_return_ran(pull_rand() % 5 + 1, pass);
      } else {
        safe_return_ran(pull_rand() % 4 + 1, pass);
      }
      pass->Pass[i] = pass->tmp_c;
      if (pass->failed) {
        return;
      }
    }
    if (strlen(pass->Pass) !=(size_t)pass->len) {
      goto LOOP;
    }
  }
}

void unsafe_return_ran(int line, Password *pass) {
  if (line >=6) {
    pass->failed = true;
  }else {
    switch(line) {
    case 1:
      pass->tmp_c = pass->symb[rand() % (int)  strlen(pass->symb)];
      break;
    case 2:
      pass->tmp_c = pass->l_let[rand() % (int)  strlen(pass->l_let)];
      break;
    case 3:
      pass->tmp_c = pass->u_let[rand() % (int)  strlen(pass->u_let)];
      break;
    case 4:
      pass->tmp_c = pass->ints[rand() % (int)  strlen(pass->ints)];
      break;
    case 5:
      pass->tmp_c = pass->test_symb[rand() % (int)  strlen(pass->test_symb)];
      break;
    default:
      pass->failed = true;
      break;
    }
  }
}

void unsafe_set_pass(Password *pass) {
 LOOP:
  srand((unsigned)time(NULL));
  for(int i = 0; i < pass->len; i++) {
    if (pass->test_symb_b) {
      unsafe_return_ran(rand() % 5 + 1, pass);
    } else {
      unsafe_return_ran(rand() % 4 + 1, pass);
    }
    pass->Pass[i] = pass->tmp_c;
    if(pass->failed) {
      return;
    }
  }
  if (strlen(pass->Pass) != (size_t)pass->len) {
    goto LOOP;
  }
}
