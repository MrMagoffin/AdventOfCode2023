#include <cstdio>
#include <cstring>
#include <vector>
#include <iostream>
#include <queue>
#include <unordered_map>

#ifdef _WIN64
typedef uint64_t u_int64_t;
typedef uint32_t u_int32_t;
#endif

typedef enum {
    LEFT = 0,
    RIGHT,
    UP,
    DOWN
} Direction;

struct State {
    State(u_int64_t _cost, u_int64_t _heat, Direction _dir, int _consec, int _x, int _y) : cost(_cost), heat(_heat), dir(_dir), consec(_consec), x(_x), y(_y) {}
    u_int64_t cost;
    u_int64_t heat;
    Direction dir;
    int consec;
    int x;
    int y;
};

bool operator<(const State& lhs, const State& rhs) {
    return (lhs.cost > rhs.cost);
}

u_int32_t minHeatMapKey(int x, int y, Direction dir, int consec) {
    return x & 0xFF | (y & 0xFF) << 8 | (dir & 0xFF) << 16 | (consec & 0xFF) << 24;
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
    for (int i = 0; i < width*height; i++) {
        grid[i] = inputVector.at(i) - 48;
    }

    // map for tracking minimum heats at locations for given direction and consecutive travelled spaces
    // [7:0] x
    // [15:8] y
    // [23:16] dir
    // [31:24] consec
    std::unordered_map<u_int32_t, u_int64_t> minHeatMap;

    // processing queue
    std::priority_queue<State> processQueue;
    
    // initialize first two possibilities, right and down
    processQueue.emplace(grid[1] + width + height - 1, grid[1], RIGHT, 1, 1, 0);
    processQueue.emplace(grid[width] + width + height - 1, grid[width], DOWN, 1, 0, 1);
    minHeatMap[0x01010001] = grid[1];
    minHeatMap[0x01030100] = grid[width];

    while (processQueue.size() > 0) {
        const State cur = processQueue.top();
        processQueue.pop();
        std::cout << "(" << cur.x << "," << cur.y << ") " << cur.heat << " " << cur.cost << std::endl;
        // see if we're done
        if (cur.x == width-1 && cur.y == height-1) {
            std::cout << cur.heat << std::endl;
            break;
        }

        // check all four directions
        // calculate cost of new location plus heuristic to destination
        // if it's a new minimum, emplace

        // right
        if (cur.x < width - 1 && cur.dir != LEFT && (cur.dir != RIGHT || cur.consec < 3)) {
            u_int64_t newHeat = cur.heat;
            newHeat += grid[cur.x + 1 + width*cur.y];
            u_int32_t key = minHeatMapKey(cur.x + 1, cur.y, RIGHT, (cur.dir == RIGHT) ? (cur.consec + 1) : 1);
            if (minHeatMap.find(key) == minHeatMap.end() || newHeat < minHeatMap.at(key)) {
                u_int64_t newCost = newHeat + (width - 1) - (cur.x + 1) + (height - 1) - cur.y;
                processQueue.emplace(newCost, newHeat, RIGHT, (cur.dir == RIGHT) ? (cur.consec + 1) : 1, cur.x + 1, cur.y);
                minHeatMap[key] = newHeat;
            }
        }
        // left
        if (cur.x > 0 && cur.dir != RIGHT && (cur.dir != LEFT || cur.consec < 3)) {
            u_int64_t newHeat = cur.heat;
            newHeat += grid[cur.x - 1 + width*cur.y];
            u_int32_t key = minHeatMapKey(cur.x - 1, cur.y, LEFT, (cur.dir == LEFT) ? (cur.consec + 1) : 1);
            if (minHeatMap.find(key) == minHeatMap.end() || newHeat < minHeatMap.at(key)) {
                u_int64_t newCost = newHeat + (width - 1) - (cur.x - 1) + (height - 1) - cur.y;
                processQueue.emplace(newCost, newHeat, LEFT, (cur.dir == LEFT) ? (cur.consec + 1) : 1, cur.x - 1, cur.y);
                minHeatMap[key] = newHeat;
            }
        }
        // down
        if (cur.y < height - 1 && cur.dir != UP && (cur.dir != DOWN || cur.consec < 3)) {
            u_int64_t newHeat = cur.heat;
            newHeat += grid[cur.x + width*(cur.y + 1)];
            u_int32_t key = minHeatMapKey(cur.x, cur.y + 1, DOWN, (cur.dir == DOWN) ? (cur.consec + 1) : 1);
            if (minHeatMap.find(key) == minHeatMap.end() || newHeat < minHeatMap.at(key)) {
                u_int64_t newCost = newHeat + (width - 1) - cur.x + (height - 1) - (cur.y + 1);
                processQueue.emplace(newCost, newHeat, DOWN, (cur.dir == DOWN) ? (cur.consec + 1) : 1, cur.x, cur.y + 1);
                minHeatMap[key] = newHeat;
            }
        }
        // up
        if (cur.y > 0 && cur.dir != DOWN && (cur.dir != UP || cur.consec < 3)) {
            u_int64_t newHeat = cur.heat;
            newHeat += grid[cur.x + width*(cur.y-1)];
            u_int32_t key = minHeatMapKey(cur.x, cur.y + 1, UP, (cur.dir == UP) ? (cur.consec + 1) : 1);
            if (minHeatMap.find(key) == minHeatMap.end() || newHeat < minHeatMap.at(key)) {
                u_int64_t newCost = newHeat + (width - 1) - cur.x + (height - 1) - (cur.y - 1);
                processQueue.emplace(newCost, newHeat, UP, (cur.dir == UP) ? (cur.consec + 1) : 1, cur.x, cur.y - 1);
                minHeatMap[key] = newHeat;
            }
        }
    }

    // part 2
     // map for tracking minimum heats at locations for given direction and consecutive travelled spaces
    // [7:0] x
    // [15:8] y
    // [23:16] dir
    // [31:24] consec
    minHeatMap.clear();

    // processing queue
    while (!processQueue.empty()) processQueue.pop();

    // initialize first two possibilities, right and down
    processQueue.emplace(grid[1] + width + height - 1, grid[1], RIGHT, 1, 1, 0);
    processQueue.emplace(grid[width] + width + height - 1, grid[width], DOWN, 1, 0, 1);
    minHeatMap[0x01010001] = grid[1];
    minHeatMap[0x01030100] = grid[width];

    while (processQueue.size() > 0) {
        const State cur = processQueue.top();
        processQueue.pop();
        std::cout << "(" << cur.x << "," << cur.y << ") " << cur.heat << " " << cur.cost << std::endl;

        if (cur.consec < 4) {
            // must continue in the same direction, if possible
            // right
            if (cur.x < width - 1 && cur.dir == RIGHT) {
                u_int64_t newHeat = cur.heat;
                newHeat += grid[cur.x + 1 + width * cur.y];
                u_int32_t key = minHeatMapKey(cur.x + 1, cur.y, RIGHT, (cur.dir == RIGHT) ? (cur.consec + 1) : 1);
                if (minHeatMap.find(key) == minHeatMap.end() || newHeat < minHeatMap.at(key)) {
                    u_int64_t newCost = newHeat + (width - 1) - (cur.x + 1) + (height - 1) - cur.y;
                    processQueue.emplace(newCost, newHeat, RIGHT, (cur.dir == RIGHT) ? (cur.consec + 1) : 1, cur.x + 1, cur.y);
                    minHeatMap[key] = newHeat;
                }
            }
            // left
            if (cur.x > 0 && cur.dir == LEFT) {
                u_int64_t newHeat = cur.heat;
                newHeat += grid[cur.x - 1 + width * cur.y];
                u_int32_t key = minHeatMapKey(cur.x - 1, cur.y, LEFT, (cur.dir == LEFT) ? (cur.consec + 1) : 1);
                if (minHeatMap.find(key) == minHeatMap.end() || newHeat < minHeatMap.at(key)) {
                    u_int64_t newCost = newHeat + (width - 1) - (cur.x - 1) + (height - 1) - cur.y;
                    processQueue.emplace(newCost, newHeat, LEFT, (cur.dir == LEFT) ? (cur.consec + 1) : 1, cur.x - 1, cur.y);
                    minHeatMap[key] = newHeat;
                }
            }
            // down
            if (cur.y < height - 1 && cur.dir == DOWN) {
                u_int64_t newHeat = cur.heat;
                newHeat += grid[cur.x + width * (cur.y + 1)];
                u_int32_t key = minHeatMapKey(cur.x, cur.y + 1, DOWN, (cur.dir == DOWN) ? (cur.consec + 1) : 1);
                if (minHeatMap.find(key) == minHeatMap.end() || newHeat < minHeatMap.at(key)) {
                    u_int64_t newCost = newHeat + (width - 1) - cur.x + (height - 1) - (cur.y + 1);
                    processQueue.emplace(newCost, newHeat, DOWN, (cur.dir == DOWN) ? (cur.consec + 1) : 1, cur.x, cur.y + 1);
                    minHeatMap[key] = newHeat;
                }
            }
            // up
            if (cur.y > 0 && cur.dir == UP) {
                u_int64_t newHeat = cur.heat;
                newHeat += grid[cur.x + width * (cur.y - 1)];
                u_int32_t key = minHeatMapKey(cur.x, cur.y + 1, UP, (cur.dir == UP) ? (cur.consec + 1) : 1);
                if (minHeatMap.find(key) == minHeatMap.end() || newHeat < minHeatMap.at(key)) {
                    u_int64_t newCost = newHeat + (width - 1) - cur.x + (height - 1) - (cur.y - 1);
                    processQueue.emplace(newCost, newHeat, UP, (cur.dir == UP) ? (cur.consec + 1) : 1, cur.x, cur.y - 1);
                    minHeatMap[key] = newHeat;
                }
            }
        }
        else {
            // see if we're done
            if (cur.x == width - 1 && cur.y == height - 1) {
                std::cout << cur.heat << std::endl;
                break;
            }
            // check all four directions
            // calculate cost of new location plus heuristic to destination
            // if it's a new minimum, emplace

            // right
            if (cur.x < width - 1 && cur.dir != LEFT && (cur.dir != RIGHT || cur.consec < 10)) {
                u_int64_t newHeat = cur.heat;
                newHeat += grid[cur.x + 1 + width * cur.y];
                u_int32_t key = minHeatMapKey(cur.x + 1, cur.y, RIGHT, (cur.dir == RIGHT) ? (cur.consec + 1) : 1);
                if (minHeatMap.find(key) == minHeatMap.end() || newHeat < minHeatMap.at(key)) {
                    u_int64_t newCost = newHeat + (width - 1) - (cur.x + 1) + (height - 1) - cur.y;
                    processQueue.emplace(newCost, newHeat, RIGHT, (cur.dir == RIGHT) ? (cur.consec + 1) : 1, cur.x + 1, cur.y);
                    minHeatMap[key] = newHeat;
                }
            }
            // left
            if (cur.x > 0 && cur.dir != RIGHT && (cur.dir != LEFT || cur.consec < 10)) {
                u_int64_t newHeat = cur.heat;
                newHeat += grid[cur.x - 1 + width * cur.y];
                u_int32_t key = minHeatMapKey(cur.x - 1, cur.y, LEFT, (cur.dir == LEFT) ? (cur.consec + 1) : 1);
                if (minHeatMap.find(key) == minHeatMap.end() || newHeat < minHeatMap.at(key)) {
                    u_int64_t newCost = newHeat + (width - 1) - (cur.x - 1) + (height - 1) - cur.y;
                    processQueue.emplace(newCost, newHeat, LEFT, (cur.dir == LEFT) ? (cur.consec + 1) : 1, cur.x - 1, cur.y);
                    minHeatMap[key] = newHeat;
                }
            }
            // down
            if (cur.y < height - 1 && cur.dir != UP && (cur.dir != DOWN || cur.consec < 10)) {
                u_int64_t newHeat = cur.heat;
                newHeat += grid[cur.x + width * (cur.y + 1)];
                u_int32_t key = minHeatMapKey(cur.x, cur.y + 1, DOWN, (cur.dir == DOWN) ? (cur.consec + 1) : 1);
                if (minHeatMap.find(key) == minHeatMap.end() || newHeat < minHeatMap.at(key)) {
                    u_int64_t newCost = newHeat + (width - 1) - cur.x + (height - 1) - (cur.y + 1);
                    processQueue.emplace(newCost, newHeat, DOWN, (cur.dir == DOWN) ? (cur.consec + 1) : 1, cur.x, cur.y + 1);
                    minHeatMap[key] = newHeat;
                }
            }
            // up
            if (cur.y > 0 && cur.dir != DOWN && (cur.dir != UP || cur.consec < 10)) {
                u_int64_t newHeat = cur.heat;
                newHeat += grid[cur.x + width * (cur.y - 1)];
                u_int32_t key = minHeatMapKey(cur.x, cur.y + 1, UP, (cur.dir == UP) ? (cur.consec + 1) : 1);
                if (minHeatMap.find(key) == minHeatMap.end() || newHeat < minHeatMap.at(key)) {
                    u_int64_t newCost = newHeat + (width - 1) - cur.x + (height - 1) - (cur.y - 1);
                    processQueue.emplace(newCost, newHeat, UP, (cur.dir == UP) ? (cur.consec + 1) : 1, cur.x, cur.y - 1);
                    minHeatMap[key] = newHeat;
                }
            }
        }
    }
    
    delete[] grid;
    return 0;
}