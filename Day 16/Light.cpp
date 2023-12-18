#include <cstdio>
#include <cstring>
#include <vector>
#include <iostream>
#include <queue>

#define UP 1
#define DOWN 2
#define LEFT 4
#define RIGHT 8

struct Beam {
    Beam(int _x, int _y, char _dir) : x(_x), y(_y), dir(_dir) {}
    int x;
    int y;
    char dir; // one-hot: 0 up 1 down 2 left 3 right
};


inline void checkAndAddBeam(int x, int y, char dir, std::queue<Beam>& processQueue, char* rays, int width, int height) {
    if (!(rays[x + y*width] & dir)) {
        rays[x + y*width] |= dir;
        processQueue.emplace(x, y, dir);
    }
}

int evaluateAtStart(int x, int y, char dir, int* grid, int width, int height) {
    // create array for light rays approaching a given point
    char* rays = new char[width*height];
    memset(rays, 0, width*height*sizeof(char));
    // bits from 0->3 are up, down, left, right

    std::queue<Beam> processQueue;
    processQueue.emplace(x, y, dir);
    rays[x + width*y] = dir;

    while (!processQueue.empty()) {
        Beam& cur = processQueue.front();
        if (cur.dir == RIGHT) {
            switch (grid[cur.x + width*cur.y]) {
                case '.':
                case '-':
                    if (cur.x < width - 1) {
                        checkAndAddBeam(cur.x + 1, cur.y, RIGHT, processQueue, rays, width, height);
                    }
                    break;
                case '|':
                    if (cur.y > 0) {
                        checkAndAddBeam(cur.x, cur.y - 1, UP, processQueue, rays, width, height);
                    }
                    if (cur.y < height - 1) {
                        checkAndAddBeam(cur.x, cur.y + 1, DOWN, processQueue, rays, width, height);
                    }
                    break;
                case '/':
                    if (cur.y > 0) {
                        checkAndAddBeam(cur.x, cur.y - 1, UP, processQueue, rays, width, height);
                    }
                    break;
                case '\\':
                    if (cur.y < height - 1) {
                        checkAndAddBeam(cur.x, cur.y + 1, DOWN, processQueue, rays, width, height);
                    }
                    break;
                default:
                    std::cout << "Unknown character " << grid[cur.x + width*cur.y] << std::endl;
                    return -1;
            }
        }
        else if (cur.dir == LEFT) {
            switch (grid[cur.x + width*cur.y]) {
                case '.':
                case '-':
                    if (cur.x > 0) {
                        checkAndAddBeam(cur.x - 1, cur.y, LEFT, processQueue, rays, width, height);
                    }
                    break;
                case '|':
                    if (cur.y > 0) {
                        checkAndAddBeam(cur.x, cur.y - 1, UP, processQueue, rays, width, height);
                    }
                    if (cur.y < height - 1) {
                        checkAndAddBeam(cur.x, cur.y + 1, DOWN, processQueue, rays, width, height);
                    }
                    break;
                case '/':
                    if (cur.y < height - 1) {
                        checkAndAddBeam(cur.x, cur.y + 1, DOWN, processQueue, rays, width, height);
                    }
                    break;
                case '\\': 
                    if (cur.y > 0) {
                        checkAndAddBeam(cur.x, cur.y - 1, UP, processQueue, rays, width, height);
                    }
                    break;
                default:
                    std::cout << "Unknown character " << grid[cur.x + width*cur.y] << std::endl;
                    return -1;
            }
        }
        else if (cur.dir == UP) {
            switch (grid[cur.x + width*cur.y]) {
                case '.':
                case '|':
                    if (cur.y > 0) {
                        checkAndAddBeam(cur.x, cur.y - 1, UP, processQueue, rays, width, height);
                    }
                    break;
                case '-':
                    if (cur.x > 0) {
                        checkAndAddBeam(cur.x - 1, cur.y, LEFT, processQueue, rays, width, height);
                    }
                    if (cur.x < width - 1) {
                        checkAndAddBeam(cur.x + 1, cur.y, RIGHT, processQueue, rays, width, height);
                    }
                    break;
                case '/':
                    if (cur.x < width - 1) {
                        checkAndAddBeam(cur.x + 1, cur.y, RIGHT, processQueue, rays, width, height);
                    }
                    break;
                case '\\': 
                    if (cur.x > 0) {
                        checkAndAddBeam(cur.x - 1, cur.y, LEFT, processQueue, rays, width, height);
                    }
                    break;
                default:
                    std::cout << "Unknown character " << grid[cur.x + width*cur.y] << std::endl;
                    return -1;
            }
        }
        else if (cur.dir == DOWN) {
            switch (grid[cur.x + width*cur.y]) {
                case '.':
                case '|':
                    if (cur.y < height - 1) {
                        checkAndAddBeam(cur.x, cur.y + 1, DOWN, processQueue, rays, width, height);
                    }
                    break;
                case '-':
                    if (cur.x > 0) {
                        checkAndAddBeam(cur.x - 1, cur.y, LEFT, processQueue, rays, width, height);
                    }
                    if (cur.x < width - 1) {
                        checkAndAddBeam(cur.x + 1, cur.y, RIGHT, processQueue, rays, width, height);
                    }
                    break;
                case '/':
                    if (cur.x > 0) {
                        checkAndAddBeam(cur.x - 1, cur.y, LEFT, processQueue, rays, width, height);
                    }
                    break;
                case '\\': 
                    if (cur.x < width - 1) {
                        checkAndAddBeam(cur.x + 1, cur.y, RIGHT, processQueue, rays, width, height);
                    }
                    break;
                default:
                    std::cout << "Unknown character " << grid[cur.x + width*cur.y] << std::endl;
                    return -1;
            }
        }
        else {
            std::cout << "Unknown direction " << cur.dir << std::endl;
            return -1;
        }
        processQueue.pop();
    }

    int sum = 0;
    char* cur = rays;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (*(cur++)) {
                sum++;
            }
        }
    }
    delete[] rays;
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
    int* grid = new int[width*height];
    memcpy(grid, inputVector.data(), width*height*sizeof(int));

    // Part 1
    int partOne = evaluateAtStart(0, 0, RIGHT, grid, width, height);
    std::cout << partOne << std::endl;

    // Part 2
    int max = partOne;
    // entries from left (besides part one)
    for (int y = 1; y < height; y++) {
        int result = evaluateAtStart(0, y, RIGHT, grid, width, height);
        max = std::max(result, max);
    }
    // entries from right
    for (int y = 0; y < height; y++) {
        int result = evaluateAtStart(width-1, y, LEFT, grid, width, height);
        max = std::max(result, max);
    }
    // entries from top
    for (int x = 0; x < width; x++) {
        int result = evaluateAtStart(x, 0, DOWN, grid, width, height);
        max = std::max(result, max);
    }
    // entries from bottom
    for (int x = 0; x < width; x++) {
        int result = evaluateAtStart(x, height-1, UP, grid, width, height);
        max = std::max(result, max);
    }
    std::cout << max << std::endl;

    delete[] grid;
    return 0;
}