#include <cstdio>
#include <iostream>
#include <list>

int main(int argc, char* argv[]) {
    FILE* f;
    if (argc > 1) {
        f = fopen(argv[1], "rb");
    }
    else {
        f = fopen("input.txt", "rb");
    }
    if (!f) {
        printf("Could not open file.\n");
        return -1;
    }

    int c = fgetc(f);
    unsigned char curValue = 0;
    u_int64_t sum = 0;
    while (!feof(f)) {
        if (c == ',') {
            sum += curValue;
            curValue = 0;
        }
        else {
            curValue += c;
            curValue = curValue * 17;
        }
        c = fgetc(f);
    }
    fclose(f);

    // final
    sum += curValue;

    std::cout << "Sum: " << sum << std::endl;

    return 0;
}