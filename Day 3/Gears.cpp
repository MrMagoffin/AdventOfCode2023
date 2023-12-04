#include <cstdio>
#include <iostream>

int expandAndClearNumber(char* charArray, int x, int y, int width, int height, bool clear = true) {
    // find start of number
    char* cur = &(charArray[width*y + x]);
    while (x > 0) {
        cur--;
        if (!isdigit(*cur)) {
            cur++;
            break;
        }
        x--;
    }
    x++;
    // extract number and clear
    int result = *cur - 48;
    if (clear) {
        *cur = '.';
    }
    cur++;
    while (isdigit(*cur) && x < width) {
        result = 10*result + *cur - 48;
        if (clear) {
            *cur = '.';
        }
        cur++;
        x++;
    }
    return result;
}

int main(int argc, char* argv[]) {
    FILE* f;
    if (argc > 1) {
        f = fopen(argv[1], "rb");
    }
    else {
        f = fopen("input.txt", "rb");
    }
    if (!f) {
        std::cout << "Could not open input file" << std::endl;
        return -1;
    }

    // determine dimensions
    int width, height;
    char c;
    width = 0;
    height = 0;
    c = fgetc(f);
    while (!feof(f)) {
        if (c == '\n') {
            height++;
        }
        else if (height == 0) {
            width++;
        }
        c = fgetc(f);
    }

    // initialize array
    char* charArray = new char[width*height + 1]; // 1 extra for guard
    char* cur = charArray;

    // load array
    fseek(f, 0, SEEK_SET);
    c = fgetc(f);
    while (!feof(f)) {
        if (c != '\n') {
            *cur = c;
            cur++;
        }
        c = fgetc(f);
    }
    *cur = '\0'; // guard byte

    // go through array looking for gear ratios (Part 2)
    cur = charArray;
    int ratioSum = 0;
    int sum = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (*cur == '*') {
                int adjacentCount = 0;
                int ratio = 1;
                // found gear symbol
                std::cout << "Gear symbol at (" << x << "," << y << ")" << std::endl;
                if (y > 0) {
                    // search for number above
                    if (isdigit(charArray[width*(y - 1) + x])) {
                        int partNumber = expandAndClearNumber(charArray, x, y - 1, width, height);
                        std::cout << "Found bordering number " << partNumber << " at (" << x << "," << y - 1 << ")" << std::endl;
                        adjacentCount++;
                        ratio *= partNumber;
                        sum += partNumber;
                    }
                    if (x > 0 && isdigit(charArray[width*(y - 1) + x - 1])) {
                        int partNumber = expandAndClearNumber(charArray, x - 1, y - 1, width, height);
                        std::cout << "Found bordering number " << partNumber << " at (" << x - 1 << "," << y - 1 << ")" << std::endl;
                        adjacentCount++;
                        ratio *= partNumber;
                        sum += partNumber;
                    }
                    if (x < width - 1 && isdigit(charArray[width*(y - 1) + x + 1])) {
                        int partNumber = expandAndClearNumber(charArray, x + 1, y - 1, width, height);
                        std::cout << "Found bordering number " << partNumber << " at (" << x + 1 << "," << y - 1 << ")" << std::endl;
                        adjacentCount++;
                        ratio *= partNumber;
                        sum += partNumber;
                    }
                }
                if (y < height - 1) {
                    // search for number below
                    if (isdigit(charArray[width*(y + 1) + x])) {
                        int partNumber = expandAndClearNumber(charArray, x, y + 1, width, height);
                        std::cout << "Found bordering number " << partNumber << " at (" << x << "," << y + 1 << ")" << std::endl;
                        adjacentCount++;
                        ratio *= partNumber;
                        sum += partNumber;
                    }
                    if (x > 0 && isdigit(charArray[width*(y + 1) + x - 1])) {
                        int partNumber = expandAndClearNumber(charArray, x - 1, y + 1, width, height);
                        std::cout << "Found bordering number " << partNumber << " at (" << x - 1 << "," << y + 1 << ")" << std::endl;
                        adjacentCount++;
                        ratio *= partNumber;
                        sum += partNumber;
                    }
                    if (x < width - 1 && isdigit(charArray[width*(y + 1) + x + 1])) {
                        int partNumber = expandAndClearNumber(charArray, x + 1, y + 1, width, height);
                        std::cout << "Found bordering number " << partNumber << " at (" << x + 1 << "," << y + 1 << ")" << std::endl;
                        adjacentCount++;
                        ratio *= partNumber;
                        sum += partNumber;
                    }
                }
                if (x > 0) {
                    // search for number left
                    if (isdigit(charArray[width*y + x - 1])) {
                        int partNumber = expandAndClearNumber(charArray, x - 1, y, width, height);
                        std::cout << "Found bordering number " << partNumber << " at (" << x - 1 << "," << y << ")" << std::endl;
                        adjacentCount++;
                        ratio *= partNumber;
                        sum += partNumber;
                    }
                }
                if (x < width - 1) {
                    // search for number right
                    if (isdigit(charArray[width*y + x + 1])) {
                        int partNumber = expandAndClearNumber(charArray, x + 1, y, width, height);
                        std::cout << "Found bordering number " << partNumber << " at (" << x + 1 << "," << y << ")" << std::endl;
                        adjacentCount++;
                        ratio *= partNumber;
                        sum += partNumber;
                    }
                }
                if (adjacentCount == 2) {
                    ratioSum += ratio;
                }
            }
            cur++;
        }
    }

    // go through array for rest of sum (Part 1)
    cur = charArray;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (*cur != '.' && !isdigit(*cur)) {
                // found symbol
                std::cout << "Symbol at (" << x << "," << y << ")" << std::endl;
                if (y > 0) {
                    // search for number above
                    if (isdigit(charArray[width*(y - 1) + x])) {
                        int partNumber = expandAndClearNumber(charArray, x, y - 1, width, height);
                        std::cout << "Found bordering number " << partNumber << " at (" << x << "," << y - 1 << ")" << std::endl;
                        sum += partNumber;
                    }
                    if (x > 0 && isdigit(charArray[width*(y - 1) + x - 1])) {
                        int partNumber = expandAndClearNumber(charArray, x - 1, y - 1, width, height);
                        std::cout << "Found bordering number " << partNumber << " at (" << x - 1 << "," << y - 1 << ")" << std::endl;
                        sum += partNumber;
                    }
                    if (x < width - 1 && isdigit(charArray[width*(y - 1) + x + 1])) {
                        int partNumber = expandAndClearNumber(charArray, x + 1, y - 1, width, height);
                        std::cout << "Found bordering number " << partNumber << " at (" << x + 1 << "," << y - 1 << ")" << std::endl;
                        sum += partNumber;
                    }
                }
                if (y < height - 1) {
                    // search for number below
                    if (isdigit(charArray[width*(y + 1) + x])) {
                        int partNumber = expandAndClearNumber(charArray, x, y + 1, width, height);
                        std::cout << "Found bordering number " << partNumber << " at (" << x << "," << y + 1 << ")" << std::endl;
                        sum += partNumber;
                    }
                    if (x > 0 && isdigit(charArray[width*(y + 1) + x - 1])) {
                        int partNumber = expandAndClearNumber(charArray, x - 1, y + 1, width, height);
                        std::cout << "Found bordering number " << partNumber << " at (" << x - 1 << "," << y + 1 << ")" << std::endl;
                        sum += partNumber;
                    }
                    if (x < width - 1 && isdigit(charArray[width*(y + 1) + x + 1])) {
                        int partNumber = expandAndClearNumber(charArray, x + 1, y + 1, width, height);
                        std::cout << "Found bordering number " << partNumber << " at (" << x + 1 << "," << y + 1 << ")" << std::endl;
                        sum += partNumber;
                    }
                }
                if (x > 0) {
                    // search for number left
                    if (isdigit(charArray[width*y + x - 1])) {
                        int partNumber = expandAndClearNumber(charArray, x - 1, y, width, height);
                        std::cout << "Found bordering number " << partNumber << " at (" << x - 1 << "," << y << ")" << std::endl;
                        sum += partNumber;
                    }
                }
                if (x < width - 1) {
                    // search for number right
                    if (isdigit(charArray[width*y + x + 1])) {
                        int partNumber = expandAndClearNumber(charArray, x + 1, y, width, height);
                        std::cout << "Found bordering number " << partNumber << " at (" << x + 1 << "," << y << ")" << std::endl;
                        sum += partNumber;
                    }
                }
            }
            cur++;
        }
    }

    std::cout << "Sum: " << sum << std::endl;

    std::cout << "Ratio sum: " << ratioSum << std::endl;

    fclose(f);

    return 0;
}