#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

typedef struct grep_flag {
    int flag_e;
    int flag_i;
    int flag_v;
    int flag_n;
    int flag_c;
    int flag_l;
    int flag_h;
    int flag_s;
} grep_flag;

void flag_initialize(char* str, int argc, char** argv, grep_flag* my_grep,
                     int* cnt, int* flag_count);
void open_file(grep_flag my_grep, char** argv, const int* flag_count, int argc);

void init_struct_flags(grep_flag* res);

#endif  //  SRC_GREP_S21_GREP_H_
