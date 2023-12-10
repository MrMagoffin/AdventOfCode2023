#include <cstdio>
#include <cstring>
#include <chrono>
#include <iostream>

typedef enum {
    UP = 0,
    DOWN,
    LEFT,
    RIGHT
} MoveType;

int main(int argc, char* argv[]) {
    FILE * f;
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

    // measure size
    int width = 0;
    int height = 1;
    int c = fgetc(f);
    while (!feof(f) && c != '\n') {
        width++;
        c = fgetc(f);
    }
    c = fgetc(f);
    while (!feof(f)) {
        if (c == '\n') {
            height++;
        }
        c = fgetc(f);
    }

    printf("Height %d width %d\n", height, width);

    // prepare maps
    int* charMap = new int[width*height];
    void** nodeMap = new void*[width*height]; // array of pointers pointing to other locations in the array
    for (int i = 0; i < width*height; i++) {
        nodeMap[i] = nullptr;
    }
    fseek(f, 0, SEEK_SET);

    // load character map and find start
    int start = -1;

    int* charMapCur = charMap;
    int cur = 0;

    c = fgetc(f);
    while (!feof(f)) {
        if (c != '\n') {
            *charMapCur = c;
            if (c == 'S') {
                start = cur;
            }
            charMapCur++;
            cur++;
        }
        c = fgetc(f);
    }
    fclose(f);

    if (start == -1) {
        printf("Could not find starting location\n");
        return -1;
    }

    int startX = start % width;
    int startY = start / width;

    printf("Start (%d,%d)\n", startX, startY);

    auto startTime = std::chrono::steady_clock::now();

    // part 1
    // populate pipe path
    charMapCur = charMap + start;
    void** nodeMapCur = nodeMap + start; // points to location in nodeMap containing a pointer to next location
    MoveType lastMove = UP;
    
    // determine character that should be at start
    int tempChar;
    bool up = false;
    bool down = false;
    bool left = false;
    if (startY > 0) {
        // can go up
        tempChar = *(charMapCur - width);
        if (tempChar == '|' || tempChar == '7' || tempChar == 'F') {
            up = true;
        }
    }
    if (startY < height-1) {
        // can go down
        tempChar = *(charMapCur + width);
        if (tempChar == '|' || tempChar == 'L' || tempChar == 'J') {
            down = true;
        }
    }
    if (startX > 0) {
        // can go left
        tempChar = *(charMapCur - 1);
        if (tempChar == '-' || tempChar == 'L' || tempChar == 'F') {
            left = true;
        }
    }
    // no need to check right, can infer
    if (up) {
        if (down) {
            *charMapCur = '|';
        }
        else if (left) {
            *charMapCur = 'J';
        }
        else {
            *charMapCur = 'L';
        }
    }
    else if (down) {
        if (left) {
            *charMapCur = '7';
        }
        else {
            *charMapCur = 'F';
        }
    }
    else {
        *charMapCur = '-';
    }

    //printf("At (%d,%d)\n", curX, curY);
    int stepCount = 0;
    while (*nodeMapCur == nullptr) {
        // find another node connected to nodeMapCur via charMapCur
        switch (*charMapCur) {
        case '|':
            if (lastMove == UP) {
                // go up again
                *nodeMapCur = nodeMapCur - width;
                charMapCur -= width;
                //curY--;
            }
            else {
                // go down (last move was down)
                *nodeMapCur = nodeMapCur + width;
                charMapCur += width;
                //curY++;
            }
            break;
        case '-':
            if (lastMove == LEFT) {
                // go left again
                *nodeMapCur = nodeMapCur - 1;
                charMapCur--;
                //curX--;
            }
            else {
                // go right (last move was right)
                *nodeMapCur = nodeMapCur + 1;
                charMapCur++;
                //curX++;
            }
            break;
        case 'L':
            if (lastMove == LEFT) {
                // go up
                *nodeMapCur = nodeMapCur - width;
                charMapCur -= width;
                lastMove = UP;
                //curY--;
            }
            else {
                // go right
                *nodeMapCur = nodeMapCur + 1;
                charMapCur++;
                lastMove = RIGHT;
                //curX++;
            }
            break;
        case 'J':
            if (lastMove == RIGHT) {
                // go up
                *nodeMapCur = nodeMapCur - width;
                charMapCur -= width;
                lastMove = UP;
                //curY--;
            }
            else {
                // go left
                *nodeMapCur = nodeMapCur - 1;
                charMapCur--;
                lastMove = LEFT;
                //curX--;
            }
            break;
        case '7':
            if (lastMove == RIGHT) {
                // go down
                *nodeMapCur = nodeMapCur + width;
                charMapCur += width;
                lastMove = DOWN;
                //curY++;
            }
            else {
                // go left
                *nodeMapCur = nodeMapCur - 1;
                charMapCur--;
                lastMove = LEFT;
                //curX--;
            }
            break;
        case 'F':
            if (lastMove == LEFT) {
                // go down
                *nodeMapCur = nodeMapCur + width;
                charMapCur += width;
                lastMove = DOWN;
                //curY++;
            }
            else {
                // go right
                *nodeMapCur = nodeMapCur + 1;
                charMapCur++;
                lastMove = RIGHT;
                //curX++;
            }
            break;
        case '.':
            printf("Arrived at ground tile, check input\n");
            return -1;
        case 'S':
            printf("Arrived at start but no pointer assigned, check input\n");
            return -1;
        default:
            printf("Unknown character %c\n", *charMapCur);
            return -1;
        }
        nodeMapCur = (void**)*nodeMapCur;
        stepCount++;
        //printf("At (%d,%d)\n", curX, curY);
    }

    // made it back to start
    auto midTime = std::chrono::steady_clock::now();

    // part 2
    // scan line-by-line, counting tiles inside
    int insideTiles = 0;
    nodeMapCur = nodeMap;
    charMapCur = charMap;
    bool inside = false;
    bool cameFromBottom = false;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (*nodeMapCur != nullptr) {
                // currently on the pipe
                c = *charMapCur;
                if (c == '|') {
                    // special case, immediately flip inside
                    inside = !inside;
                }
                else if (c == 'F') {
                    cameFromBottom = true;
                }
                else if (c == 'L') {
                    cameFromBottom = false;
                }
                else if (c == '7') {
                    if (!cameFromBottom) {
                        inside = !inside;
                    }
                }
                else if (c == 'J') {
                    if (cameFromBottom) {
                        inside = !inside;
                    }
                }
                // ignore '-'
            }
            if (*nodeMapCur == nullptr) {
                // not part of main loop
                if (inside) {
                    insideTiles++;
                }
            }
            charMapCur++;
            nodeMapCur++;
        }
    }

    auto endTime = std::chrono::steady_clock::now();

    printf("Pipe took %d steps.\n", stepCount);
    printf("Farthest is %d away.\n", stepCount / 2);
    printf("%d inside tiles.\n", insideTiles);
    std::cout << "Part 1 took " << std::chrono::duration_cast<std::chrono::nanoseconds>(midTime - startTime).count() << " ns." << std::endl;
    std::cout << "Part 2 took " << std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - midTime).count() << " ns." << std::endl;

    // cleanup memory
    delete[] charMap;
    delete[] nodeMap;

    return 0;
}