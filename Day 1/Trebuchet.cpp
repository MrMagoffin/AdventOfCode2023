#include <cstdio>
#include <cstdlib>
#include <cctype>

int main(int argc, char * argv[]) {
    FILE * file;
    if (argc > 1) {
        file = fopen(argv[1], "r");
    }
    else {
        file = fopen("input.txt", "r");
    }

    int finalResult = 0;
    int c = fgetc(file);
    char digits[3];
    digits[0] = -1;
    digits[2] = 0;
    while (!feof(file))
    {
        if (isdigit(c)){
            if (digits[0] == -1) {
                digits[0] = c;
                digits[1] = c;
            }
            else {
                digits[1] = c;
            }
        }
        else if (c == '\n') {
            char* end;
            int result = strtol(digits, &end, 10);
            finalResult += result;
            digits[0] = -1;
        }
        c = fgetc(file);
    }
    if (digits[0] != -1) {
        char* end;
        int result = strtol(digits, &end, 10);
        finalResult += result;
    }

    printf("Sum: %d\n", finalResult);

    return 0;
}