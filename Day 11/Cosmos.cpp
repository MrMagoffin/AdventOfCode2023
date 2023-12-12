#include <cstdio>
#include <iostream>
#include <vector>

struct Point {
    Point(int _x, int _y) : x(_x), y(_y) {}
    int x;
    int y;
};

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
    std::vector<std::vector<int>> map;
    map.emplace_back();
    std::vector<int>* row = &(map.back());
    while (!feof(f)) {
        if (c == '\n') {
            map.emplace_back();
            row = &(map.back());
        }
        else if (c != '\r') {
            row->push_back(c);
        }
        c = fgetc(f);
    }
    fclose(f);

    // delete last row if empty
    if (row->size() == 0) {
        map.pop_back();
    }

    std::cout << "Pre-expansion: " << map.size() << " rows, " << map.back().size() << " columns." << std::endl;

    // scan columns
    std::vector<int> emptyColumns;
    for (int i = 0; i < map.back().size(); i++) {
        bool foundGalaxy = false;
        for (int j = 0; j < map.size(); j++) {
            if (map.at(j).at(i) == '#') {
                foundGalaxy = true;
                break;
            }
        }
        if (foundGalaxy) {
            continue;
        }
        // record column
        emptyColumns.emplace_back(i);
    }

    // scan rows
    std::vector<int> emptyRows;
    for (int i = 0; i < map.size(); i++) {
        bool foundGalaxy = false;
        for (auto jt = map.at(i).begin(); jt != map.at(i).end(); jt++) {
            if (*jt == '#') {
                foundGalaxy = true;
                break;
            }
        }
        if (foundGalaxy) {
            continue;
        }
        // record row
        emptyRows.emplace_back(i);
    }

    std::cout << "Post-expansion: " << map.size() + emptyRows.size() << " rows, " << map.back().size() + emptyColumns.size() << " columns." << std::endl;

    // find galaxies
    std::vector<Point> galaxies;
    // iterate through rows
    int y = 0;
    for (auto it = map.begin(); it != map.end(); it++) {
        // iterate through columns
        int x = 0;
        for (auto jt = it->begin(); jt != it->end(); jt++) {
            if (*jt == '#') {
                galaxies.emplace_back(x, y);
            }
            x++;
        }
        y++;
    }

    // iterate through galaxy pairs
    u_int64_t dist = 0;
    u_int64_t bonusDist = 0;
    for (auto it = galaxies.begin(); it != galaxies.end(); it++) {
        for (auto jt = it + 1; jt != galaxies.end(); jt++) {
            int x1, x2, y1, y2;
            x1 = it->x;
            x2 = jt->x;
            y1 = it->y;
            y2 = jt->y;
            if (x1 > x2) {
                std::swap(x1, x2);
            }
            if (y1 > y2) {
                std::swap(y1, y2);
            }
            // add baseline distance
            dist += std::abs(jt->y - it->y) + std::abs(jt->x - it->x);
            // add bonus distance
            for (auto it = emptyRows.begin(); it != emptyRows.end(); it++) {
                if (y1 < *it && y2 > *it) {
                    bonusDist++;
                }
            }
            for (auto it = emptyColumns.begin(); it != emptyColumns.end(); it++) {
                if (x1 < *it && x2 > *it) {
                    bonusDist++;
                }
            }
        }
    }
    
    std::cout << "Part 1 distance total: " << dist + bonusDist << std::endl;
    std::cout << "Part 2 distance total: " << dist + (1000000 - 1) * bonusDist << std::endl;

    return 0;
}