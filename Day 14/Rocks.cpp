#include <cstdio>
#include <vector>
#include <iostream>
#include <cstring>

#define PATTERN_MAX 400
#define CYCLES 1000000000

int calculateLoad(int* input, int width, int height) {
    int sum = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int c = input[x + width*y];
            if (c == 'O') {
                // count rock
                sum += height-y;
            }
        }
    }
    return sum;
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

    std::vector<int> inputVector;
    int width = 0;
    int height = 0;
    bool foundWidth = false;

    int c = fgetc(f);
    while (!feof(f)) {
        if (c == '\n') {
            foundWidth = true;
            height++;
        }
        else if (c != '\r') {
            if (!foundWidth) {
                width++;
            }
            inputVector.push_back(c);
        }
        c = fgetc(f);
    }
    fclose(f);

    // copy input into traditional array
    int* prevState = new int[width*height];
    memcpy(prevState, inputVector.data(), width*height*sizeof(int));

    // prepare space for next state
    int* nextState = new int[width*height];

    std::vector<int> loads;

    for (u_int64_t cycle = 0; cycle < 1000000000; cycle++) {
        // iterate by column to collect rocks north
        int sum = 0;
        for (int x = 0; x < width; x++) {
            // travel down column, tracking rocks
            int nextStateY = 0;
            for (int y = 0; y < height; y++) {
                int c = prevState[x + width*y];
                if (c == '#') {
                    // next rock will slide to directly below this
                    // catch up in nextState
                    while (nextStateY < y) {
                        nextState[x + width*nextStateY] = '.';
                        nextStateY++;
                    }
                    nextState[x + width*nextStateY] = '#';
                    nextStateY++;
                }
                else if (c == 'O') {
                    // count rock
                    nextState[x + width*nextStateY] = 'O';
                    nextStateY++;
                }
            }
            // catch up in nextState
            while (nextStateY < height) {
                nextState[x + width*nextStateY] = '.';
                nextStateY++;
            }
        }
        
        if (cycle == 0) {
            std::cout << "Part 1 load: " << calculateLoad(nextState, width, height) << std::endl;
        }

        std::swap(prevState, nextState);

        // iterate by row to collect rocks west
        for (int y = 0; y < height; y++) {
            // travel right on row, tracking rocks
            int nextStateX = 0;
            for (int x = 0; x < width; x++) {
                int c = prevState[x + width*y];
                if (c == '#') {
                    // next rock will slide to directly to the right of this
                    // catch up in nextState
                    while (nextStateX < x) {
                        nextState[nextStateX + width*y] = '.';
                        nextStateX++;
                    }
                    nextState[nextStateX + width*y] = '#';
                    nextStateX++;
                }
                else if (c == 'O') {
                    // count rock
                    nextState[nextStateX + width*y] = 'O';
                    nextStateX++;
                }
            }
            // catch up in nextState
            while (nextStateX < width) {
                nextState[nextStateX + width*y] = '.';
                nextStateX++;
            }
        }

        std::swap(prevState, nextState);

        // iterate by column to collect rocks south
        for (int x = 0; x < width; x++) {
            // travel up column, tracking rocks
            int nextStateY = height-1;
            for (int y = height-1; y >= 0; y--) {
                int c = prevState[x + width*y];
                if (c == '#') {
                    // next rock will slide to directly above this
                    // catch up in nextState
                    while (nextStateY > y) {
                        nextState[x + width*nextStateY] = '.';
                        nextStateY--;
                    }
                    nextState[x + width*nextStateY] = '#';
                    nextStateY--;
                }
                else if (c == 'O') {
                    // count rock
                    nextState[x + width*nextStateY] = 'O';
                    nextStateY--;
                }
            }
            // catch up in nextState
            while (nextStateY >= 0) {
                nextState[x + width*nextStateY] = '.';
                nextStateY--;
            }
        }

        std::swap(prevState, nextState);

        // iterate by row to collect rocks east
        for (int y = 0; y < height; y++) {
            // travel left on row, tracking rocks
            int nextStateX = width-1;
            for (int x = width-1; x >= 0; x--) {
                int c = prevState[x + width*y];
                if (c == '#') {
                    // next rock will slide to directly to the right of this
                    // catch up in nextState
                    while (nextStateX > x) {
                        nextState[nextStateX + width*y] = '.';
                        nextStateX--;
                    }
                    nextState[nextStateX + width*y] = '#';
                    nextStateX--;
                }
                else if (c == 'O') {
                    // count rock
                    nextState[nextStateX + width*y] = 'O';
                    nextStateX--;
                }
            }
            // catch up in nextState
            while (nextStateX >= 0) {
                nextState[nextStateX + width*y] = '.';
                nextStateX--;
            }
        }

        loads.push_back(calculateLoad(nextState, width, height));

        // check for a pattern every so often
        if (cycle % 1000 == 999) {
            bool match;
            for (u_int64_t i = 1; i < PATTERN_MAX; i++) {
                // check for cycle length i over the last PATTERN_MAX*2 spots
                match = true;
                for (int j = 1; j < PATTERN_MAX*2; j++) {
                    if (loads.at(cycle+1-j) != loads.at(cycle+1-i-j)) {
                        match = false;
                        break;
                    }
                }
                if (match) {
                    // now we can cheat
                    // i is the cycle length
                    u_int64_t cyclesToGo = CYCLES - (cycle + 1);
                    u_int64_t offset = cyclesToGo % i;
                    std::cout << "Cycle length " << i << std::endl;
                    std::cout << "Cycles to go " << cyclesToGo << std::endl;
                    std::cout << "Current cycle " << cycle + 1 << std::endl;
                    std::cout << "Offset " << offset << std::endl;
                    std::cout << "Part two: " << loads.at(cycle-i+offset) << std::endl;
                    break;
                }
            }
            if (match) {
                break;
            }
        }

        std::swap(prevState, nextState);
    }

    delete[] prevState;
    delete[] nextState;
    return 0;
}