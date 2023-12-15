#include <cstdio>
#include <iostream>
#include <list>
#include <string>

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
    bool nextIsLens = false;
    bool remove = false;
    std::list<std::pair<std::string, int>> boxes[256];
    std::string label;
    while (!feof(f)) {
        if (c == ',') {
            curValue = 0;
            label.clear();
        }
        else if (nextIsLens) {
            // look up box
            std::list<std::pair<std::string, int>>& box = boxes[curValue];
            // look for element with matching label
            bool replaced = false;
            for (auto it = box.begin(); it != box.end(); it++) {
                if (it->first == label) {
                    // replace lens
                    it->second = c - 48;
                    replaced = true;
                    break;
                }
            }
            if (!replaced) {
                // add to the back
                box.emplace_back(label, c - 48);
            }
            nextIsLens = false;
        }
        else if (c == '-') {
            // look up box
            std::list<std::pair<std::string, int>>& box = boxes[curValue];
            // look for element with matching label
            for (auto it = box.begin(); it != box.end(); it++) {
                if (it->first == label) {
                    // remove labelled lens
                    box.erase(it);
                    break;
                }
            }
        }
        else if (c == '=') {
            nextIsLens = true;
        }
        else {
            curValue += c;
            curValue = curValue * 17;
            label.push_back(c);
        }
        c = fgetc(f);
    }
    fclose(f);

    // do final calculations
    u_int64_t sum = 0;
    for (int i = 0; i < 256; i++) {
        int slot = 1;
        for (auto it = boxes[i].begin(); it != boxes[i].end(); it++) {
            sum += (i + 1)*slot*it->second;
            slot++;
        }
    }

    std::cout << "Total power: " << sum << std::endl;

    return 0;
}