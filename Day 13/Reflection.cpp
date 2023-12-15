#include <cstdio>
#include <vector>
#include <iostream>

std::pair<int, int> processInput(std::vector<int>& input, int width, int height) {
    // check for vertical lines of reflection
    std::pair<int, int> returnVal;
    for (int i = 1; i < width; i++) {
        // iterating through reflection columns
        bool reflects = true;
        bool smudge = false;
        // iterate through row pairs to see if it reflects the given pattern
        int left = i-1;
        int right = i;
        while (left >= 0 && right < width) {
            for (int j = 0; j < height; j++) {
                // check row
                if (input.at(left+width*j) != input.at(right+width*j)) {
                    if (!smudge) {
                        smudge = true;
                    }
                    else {
                        reflects = false;
                        break;
                    }
                }
            }
            if (!reflects) {
                break;
            }
            left--;
            right++;
        }
        if (!reflects) {
            continue;
        }
        // found valid reflection
        if (!smudge) {
            returnVal.first = i;
        }
        else {
            returnVal.second = i;
        }
    }

    // now horizontal lines of reflection
    for (int i = 1; i < height; i++) {
        // iterating through reflection rows
        bool reflects = true;
        bool smudge = false;
        // iterate through column pairs to see if it reflects the given pattern
        int top = i-1;
        int bottom = i;
        while (top >= 0 && bottom < height) {
            for (int j = 0; j < width; j++) {
                // check column
                if (input.at(j+width*top) != input.at(j+width*bottom)) {
                    if (!smudge) {
                        smudge = true;
                    }
                    else {
                        reflects = false;
                        break;
                    }
                }
            }
            if (!reflects) {
                break;
            }
            top--;
            bottom++;
        }
        if (!reflects) {
            continue;
        }
        // found valid reflection
        if (!smudge) {
            returnVal.first = 100*i;
        }
        else {
            returnVal.second = 100*i;
        }
    }
    return returnVal;
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
        printf("Could not open file.\n");
        return -1;
    }

    std::vector<int> input;
    int width = 0;
    int height = 0;
    bool foundWidth = false;
    bool lastWasNewline = false;

    int c = fgetc(f);
    int sum = 0;
    int sumTwo = 0;
    while (!feof(f)) {
        if (c == '\n') {
            if (lastWasNewline) {
                // complete input, process
                std::pair<int, int> results = processInput(input, width, height);
                sum += results.first;
                sumTwo += results.second;
                input.clear();
                width = 0;
                height = 0;
                foundWidth = false;
            }
            else {
                foundWidth = true;
                lastWasNewline = true;
                height++;
            }
        }
        else if (c != '\r') {
            lastWasNewline = false;
            if (!foundWidth) {
                width++;
            }
            input.push_back(c);
        }
        c = fgetc(f);
    }
    if (input.size() > 0) {
        // final input
        std::pair<int, int> results = processInput(input, width, height);
        sum += results.first;
        sumTwo += results.second;
    }
    std::cout << "Sum: " << sum << std::endl;
    std::cout << "Sum Two: " << sumTwo << std::endl;
    fclose(f);

    return 0;
}