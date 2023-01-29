//  by July
#include "s21_cat.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    cat_flag my_cat = {0};
    char str[1024] = {0};
    int cnt = 0;

    flag_initialise(str, argc, argv, &my_cat, &cnt);
    open_file(str, my_cat, cnt);

    return 0;
}

void flag_initialise(char *str, int argc, char **argv, cat_flag *my_cat,
                     int *cnt) {
    for (int i = 1; i < argc; i++) {
        //         printf("%s\n", argv[i]);
        if (argv[i][0] == '-') {  // 1 строка  2 столбец
            if (argv[i][1] == '-') {
                if (0 == strcmp("--number-nonblank", argv[i])) {  //  or !strcmp
                    my_cat->flag_b = 1;
                }
                if (0 == strcmp("--number", argv[i])) {
                    my_cat->flag_n = 1;
                }
                if (0 == strcmp("--squeeze-blank", argv[i])) {
                    my_cat->flag_s = 1;
                }
            } else {
                for (int j = 1; j < (int)strlen(argv[i]);
                     j++) {  // j = 1 второй столбец матрицы
                    switch (argv[i][j]) {
                        case 'b':
                            my_cat->flag_b = 1;
                            break;  // true
                        case 'e':
                            my_cat->flag_e = 1;
                            my_cat->flag_v = 1;
                            break;
                        case 'n':
                            my_cat->flag_n = 1;
                            break;
                        case 's':
                            my_cat->flag_s = 1;
                            break;
                        case 't':
                            my_cat->flag_t = 1;
                            my_cat->flag_v = 1;
                            break;
                        case 'E':
                            my_cat->flag_e = 1;
                            break;
                        case 'T':
                            my_cat->flag_t = 1;
                            break;
                        case 'v':
                            my_cat->flag_v = 1;
                            break;
                    }
                }
            }
        } else {
            strcpy((str + *cnt), argv[i]);
            // printf("%s\n", str[cnt]);
            *cnt = *cnt + 1;
        }
    }
}

void open_file(const char *str, cat_flag my_cat, int cnt) {
    FILE *my_file;
    int char_from_file;
    int num = 1;  //   нумерация строк
    str_flag my_str = {0};
    my_str.new_str = 1;
    for (int i = 0; i < cnt; i++) {
        my_file = fopen((str + i), "r");
        if (my_file != NULL) {
            while ((char_from_file = fgetc(my_file)) != EOF) {
                if (my_str.new_str) {
                    if (char_from_file == '\n') {
                        my_str.empty_str = 1;
                    } else {
                        my_str.empty_str = 0;
                    }
                }
                if (f_s(&my_cat, &my_str)) continue;
                if (f_nb(&my_cat, &my_str, &num)) continue;
                f_str(&my_cat, &my_str, &char_from_file, &num);
                if (my_cat.flag_e) {
                    if (char_from_file == '\n') {
                        printf("$\n");
                        continue;
                    }
                }
                if (my_cat.flag_t) {
                    if (char_from_file == '\t') {
                        printf("^I");
                        continue;
                    }
                }
                f_v(&my_cat, &char_from_file);
                printf("%c", char_from_file);
            }
            fclose(my_file);
        } else {
            printf("s21_cat: %s: %s\n", (str + i), strerror(errno));
        }
    }
}

int f_s(cat_flag *my_cat, str_flag *my_str) {
    int t = 0;
    if (my_cat->flag_s) {
        if (my_str->new_str && my_str->empty_str) {
            my_str->empty_str_num++;
        } else {
            my_str->empty_str_num = 0;
        }
        if (my_str->empty_str_num == 2) {
            my_str->empty_str_num = 1;
            t = 1;
        }
    }
    return t;
}

int f_nb(cat_flag *my_cat, str_flag *my_str, int *num) {
    int nb = 0;
    if (my_cat->flag_n || my_cat->flag_b) {
        if (my_str->new_str) {
            if (!(my_str->empty_str && my_cat->flag_b)) {
                printf("%6d\t", *num);
                my_str->new_str = 0;
            } else {
                if (my_cat->flag_e) {
                    printf("$\n");
                } else {
                    printf("\n");
                }
                nb = 1;
            }
        }
    }
    return nb;
}

void f_v(cat_flag *my_cat, int *char_from_file) {
    if (my_cat->flag_v) {
        if (*char_from_file < 32 && *char_from_file != 9 &&
            *char_from_file != 10) {
            *char_from_file = *char_from_file + 64;
            printf("^");
        }
        if (*char_from_file == 127) {
            *char_from_file = *char_from_file - 64;
            printf("^");
        }
    }
}

void f_str(cat_flag *my_cat, str_flag *my_str, int *char_from_file, int *num) {
    if (*char_from_file == '\n') {
        my_str->new_str = 1;
        if (!(my_str->empty_str &&
              my_cat->flag_b)) {  // empty_str =0; flag_b =0;
            *num = *num + 1;
        }
    } else {
        my_str->new_str = 0;
        my_str->empty_str = 0;
    }
}
