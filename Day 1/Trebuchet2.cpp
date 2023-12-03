#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>

void updateDigits(char* digits, char newDigit) {
    if (digits[0] == -1) {
        digits[0] = newDigit;
        digits[1] = newDigit;
    }
    else {
        digits[1] = newDigit;
    }
}

int main(int argc, char * argv[]) {
    FILE * file;
    if (argc > 1) {
        file = fopen(argv[1], "r");
    }
    else {
        file = fopen("input.txt", "r");
    }

    int finalResult = 0; // final sum

    // array of five letters to find numbers as words
    char letters[5];
    memset(letters, 0, 5);

    // first and last digits (as actual number, not ASCII)
    char digits[2];
    digits[0] = -1;

    int c = fgetc(file); // get first character
    while (!feof(file))
    {
        // shift in new letter
        for (int i = 0; i < 4; i++) {
            letters[i] = letters[i + 1];
        }
        letters[4] = c;
        if (isdigit(c)){
            updateDigits(digits, c-48);
        }
        else if (c == '\n') {
            char* end;
            int result = 10*digits[0] + digits[1];
            finalResult += result;
            digits[0] = -1;
            memset(letters, 0, 5);
        }
        else {
            if (strncmp(letters+2, "one", 3) == 0) {
                updateDigits(digits, 1);
            }
            else if (strncmp(letters+2, "two", 3) == 0) {
                updateDigits(digits, 2);
            }
            else if (strncmp(letters, "three", 5) == 0) {
                updateDigits(digits, 3);
            }
            else if (strncmp(letters+1, "four", 4) == 0) {
                updateDigits(digits, 4);
            }
            else if (strncmp(letters+1, "five", 4) == 0) {
                updateDigits(digits, 5);
            }
            else if (strncmp(letters+2, "six", 3) == 0) {
                updateDigits(digits, 6);
            }
            else if (strncmp(letters, "seven", 5) == 0) {
                updateDigits(digits, 7);
            }
            else if (strncmp(letters, "eight", 5) == 0) {
                updateDigits(digits, 8);
            }
            else if (strncmp(letters+1, "nine", 4) == 0) {
                updateDigits(digits, 9);
            }
        }
        c = fgetc(file);
    }
    if (digits[0] != -1) {
        char* end;
        int result = 10*digits[0] + digits[1];
        finalResult += result;
    }

    printf("Sum: %d\n", finalResult);

    return 0;
}