#include "s21_grep.h"

#include <errno.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    grep_flag my_grep;
    init_struct_flags(&my_grep);
    char str[1024];
    int cnt = 0;

    int flag_count = 0;

    flag_initialize(str, argc, argv, &my_grep, &cnt, &flag_count);
    open_file(my_grep, argv, &flag_count, argc);

    return 0;
}

void flag_initialize(char *str, int argc, char **argv, grep_flag *my_grep,
                     int *cnt, int *flag_count) {
    for (int i = 1; i < argc; i++) {
        //         printf("%s\n", argv[i]);
        if (argv[i][0] == '-') {  // 1 строка  2 столбец
            *flag_count = *flag_count + 1;
            for (int j = 1; j < (int)strlen(argv[i]);
                 j++) {  // j = 1 второй столбец матрицы
                switch (argv[i][j]) {
                    case 'e':
                        my_grep->flag_e = 1;
                        break;  // true
                    case 'i':
                        my_grep->flag_i = 1;
                        break;
                    case 'v':
                        my_grep->flag_v = 1;
                        break;
                    case 'n':
                        my_grep->flag_n = 1;
                        break;
                    case 'c':
                        my_grep->flag_c = 1;
                        break;
                    case 'l':
                        my_grep->flag_l = 1;
                        break;
                    case 'h':
                        my_grep->flag_h = 1;
                        break;
                    case 's':
                        my_grep->flag_s = 1;
                        break;
                }
            }
        } else {
            strcpy((str + *cnt), argv[i]);
            *cnt = *cnt + 1;
        }
    }
}

void open_file(grep_flag my_grep, char **argv, const int *flag_count,
               int argc) {
    FILE *my_file;
    char stroke_from_file[1024] = "\0";

    regex_t regex;
    int flag = 0;
    typedef struct count_lnc {
        int count_l;
        int count_n;
        int count_c;
    } count_lnc;
    count_lnc countlnc = {0, 0, 0};
    for (int i = 2 + *flag_count, reti = 0; i < argc; i++) {
        if (argv[i][0] != '-') {
            my_file = fopen((argv[i]), "r");
            if (my_file != NULL) {
                /* Compile regular expression */

                //  flag i
                if (my_grep.flag_i) {
                    flag = REG_ICASE;
                }

                reti = regcomp(&regex, argv[1 + *flag_count], flag);
                while (fgets(stroke_from_file, 500, my_file) != NULL) {
                    if (stroke_from_file[strlen(stroke_from_file) - 1] !=
                        '\n') {
                        strcat(stroke_from_file, "\n");
                    }
                    // flag n
                    if (my_grep.flag_n) {
                        countlnc.count_n++;
                    }

                    reti = regexec(&regex, stroke_from_file, 0, NULL, 0);
                    //  flag v
                    if (my_grep.flag_v) reti = !reti;
                    if (!reti) {
                        //  flag c
                        if (my_grep.flag_c) {
                            countlnc.count_c++;
                            if (!my_grep.flag_l) continue;
                        }

                        //  flag l
                        if (my_grep.flag_l) {
                            countlnc.count_l++;
                            if (!(my_grep.flag_c) && (countlnc.count_l == 1))
                                printf("%s\n", argv[i]);
                            continue;
                        }

                        //  flag n
                        if ((argc - *flag_count) == 3) {
                            if (my_grep.flag_n == 1) {
                                printf("%d:%s", countlnc.count_n,
                                       stroke_from_file);
                            } else {
                                printf("%s", stroke_from_file);
                            }
                        } else {
                            if (my_grep.flag_n) {
                                if (!my_grep.flag_h) {
                                    printf("%s:%d:%s", argv[i],
                                           countlnc.count_n, stroke_from_file);
                                } else {
                                    printf("%d:%s", countlnc.count_n,
                                           stroke_from_file);
                                }
                            } else {
                                if (!my_grep.flag_h) {
                                    printf("%s:%s", argv[i], stroke_from_file);
                                } else {
                                    printf("%s", stroke_from_file);
                                }
                            }
                        }
                    }
                }
                countlnc.count_l = 0;
                countlnc.count_n = 0;
                //  to flag c
                if (my_grep.flag_c) {
                    if (my_grep.flag_l) {
                        if (countlnc.count_c != 0) {
                            if ((argc - *flag_count) == 3) {
                                printf("1\n");
                            } else {
                                if (!my_grep.flag_h) {
                                    printf("%s:1\n", argv[i]);
                                } else {
                                    printf("1\n");
                                }
                            }
                            printf("%s\n", argv[i]);
                        } else {
                            if ((argc - *flag_count) == 3) {
                                printf("0\n");
                            } else {
                                if (!my_grep.flag_h) {
                                    printf("%s:0\n", argv[i]);
                                } else {
                                    printf("0\n");
                                }
                            }
                        }
                    } else {
                        int rargc = argc;
                        if ((rargc - flag_count[0]) == 3) {
                            printf("%d\n", countlnc.count_c);
                        } else {
                            if (!my_grep.flag_h) {
                                printf("%s:%d\n", argv[i], countlnc.count_c);
                            } else {
                                printf("%d\n", countlnc.count_c);
                            }
                        }
                    }
                    countlnc.count_c = 0;
                }

                regfree(&regex);
                // printf("%s", argv[1 + *flag_count]);
                fclose(my_file);
            } else {
                if (!my_grep.flag_s)
                    printf("s21_grep: %s: %s\n", (argv[i]), strerror(errno));
            }
        }
    }
}
void init_struct_flags(grep_flag *res) {
    res->flag_c = 0;
    res->flag_e = 0;
    res->flag_i = 0;
    res->flag_l = 0;
    res->flag_n = 0;
    res->flag_v = 0;
    res->flag_h = 0;
    res->flag_s = 0;
}
