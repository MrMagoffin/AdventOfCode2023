#include <cstdio>
#include <cctype>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>

u_int64_t processLine(std::vector<int>& springs, std::vector<int>& groups, std::unordered_map<u_int64_t, u_int64_t>& memos, int remainingSpots, int remainingSprings, int placedGroups = 0, int start = 0) {
    u_int64_t lookupValue = placedGroups;
    lookupValue = lookupValue << 32;
    lookupValue += start;
    auto memo = memos.find(lookupValue);
    if (memo != memos.end()) {
        return memo->second;
    }
    // if all groups have been placed, verify no springs remain
    if (placedGroups == groups.size()) {
        for (int i = start; i < springs.size(); i++) {
            if (springs.at(i) == '#') {
                return 0;
            }
        }
        return 1;
    }
    // try placing first group in groups everywhere in springs, then recurse
    u_int64_t possibilities = 0;
    int groupSize = groups.at(placedGroups) + 2;
    // group takes groupSize plus two (for bordering periods) space
    for (int i = start; i <= springs.size() - groupSize; i++) {
        if (remainingSpots < remainingSprings) {
            // this chain is dead, doesn't work
            break;
        }    
        if (springs.at(i) == '#') {
            // need to have already placed a group
            break;
        }
        int tempRemainingSpots = remainingSpots;
        // does the group fit here?
        bool fits = true;
        for (int j = 1; j < groupSize; j++) {
            int spring = springs.at(i+j);
            if (j == groupSize - 1) {
                // must be . or ?
                if (spring == '#') {
                    fits = false;
                    break;
                }
            }
            else {
                // must be # or ?
                if (spring == '.') {
                    fits = false;
                    break;
                }
            }
            if ((j != groupSize - 1) && (spring != '.')) {
                tempRemainingSpots--;
            }
        }
        if (fits) {
            // fits, recurse
            possibilities += processLine(springs, groups, memos, tempRemainingSpots, remainingSprings - groups.at(placedGroups), placedGroups+1, i+groupSize-1);
        }
        if (springs.at(i) != '.') {
            remainingSpots--;
        }
    }
    memos[lookupValue] = possibilities;
    return possibilities;
}

void pentupleVector(std::vector<int>& intVector, bool isSprings) {
    if (isSprings) {
        // remove boundaries temporarily
        intVector.pop_back();
        intVector.erase(intVector.begin());
    }
    int oldSize = intVector.size();
    intVector.reserve(5 * oldSize + 4);
    for (int i = 0; i < 4; i++) {
        if (isSprings) {
            intVector.push_back('?');
        }
        std::copy_n(intVector.begin(), oldSize, std::back_inserter(intVector));
    }
    if (isSprings) {
        // re-add boundaries
        intVector.insert(intVector.begin(), '.');
        intVector.push_back('.');
    }
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

    int c = fgetc(f);
    std::vector<int> springs;
    std::vector<int> groups;
    u_int64_t totalCombos = 0;
    u_int64_t totalCombosTwo = 0;
    int spotCount = 0;
    int springCount = 0;

    int group = 0;
    springs.push_back('.'); // starting boundary
    while (!feof(f)) {
        if (c == '\n') {
            std::unordered_map<u_int64_t, u_int64_t> memos;
            groups.push_back(group);
            springs.push_back('.'); // ending boundary
            group = 0;
            u_int64_t combos = processLine(springs, groups, memos, spotCount, springCount);
            std::cout << combos << std::endl;
            totalCombos += combos;

            // part 2
            pentupleVector(springs, true);
            pentupleVector(groups, false);
            memos.clear();
            combos = processLine(springs, groups, memos, spotCount*5 + 4, springCount*5);
            std::cout << combos << std::endl;
            totalCombosTwo += combos;
            
            spotCount = 0;
            springCount = 0;
            springs.clear();
            springs.push_back('.'); // starting boundary
            groups.clear();
        }
        else if (isdigit(c)) {
            group = 10 * group + c - 48;
        }
        else if (c == ',') {
            groups.push_back(group);
            springCount += group;
            group = 0;
        }
        else if (c == '#' || c == '?') {
            spotCount++;
            springs.push_back(c);
        }
        else if (c == '.') {
            springs.push_back(c);
        }
        c = fgetc(f);
    }
    std::cout << "Total combos: " << totalCombos << std::endl;
    std::cout << "Total combos 2: " << totalCombosTwo << std::endl;
    fclose(f);
}