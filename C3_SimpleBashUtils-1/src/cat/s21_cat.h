#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

typedef struct cat_flag {
    int flag_b;  // boolean true or false
    int flag_e;
    int flag_n;
    int flag_s;
    int flag_t;
    int flag_v;
} cat_flag;

typedef struct str_flag {
    int new_str;
    int empty_str;
    int empty_str_num;
} str_flag;

    cat_flag my_cat = {0};
    char str[1024] = {0};
    int cnt = 0;

void flag_initialise(char *str, int argc, char **argv, cat_flag* my_cat, int* cnt);
void open_file(const char *str, cat_flag my_cat, int cnt);
int f_s(cat_flag *my_cat, str_flag *my_str);
int f_nb(cat_flag *my_cat, str_flag *my_str, int* num);
void f_v(cat_flag *my_cat, int* char_from_file);
void f_str(cat_flag *my_cat, str_flag *my_str, int* char_from_file, int* num);

#endif  //  SRC_CAT_S21_CAT_H_
