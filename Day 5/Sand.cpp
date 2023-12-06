#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

struct Range {
    int64_t begin; // source range start
    int64_t end; // source range end
    int64_t offset; // difference from source to destination

    Range(int64_t _begin, int64_t _end, int64_t _offset) : begin(_begin), end(_end), offset(_offset) {}
};

bool operator<(const Range& lhs, const Range& rhs) {
    return lhs.begin < rhs.begin;
}

bool operator<(const Range& lhs, const int64_t& rhs) {
    return rhs >= lhs.end;
}

bool operator<(const int64_t& lhs, const Range& rhs) {
    return lhs < rhs.begin;
}

std::ostream& operator<<(std::ostream& out, const Range& range) {
    out << '(' << range.begin << ',' << range.end << ',' << range.offset << ')';
    return out;
}

int64_t recursiveFindMinLocation(std::vector<Range>* maps, int mapIteration, int64_t start, int64_t end) {
    if (mapIteration == 7) {
        // already converted to location
        return start;
    }
    // find start of range in maps
    // binary search
    int lowBound = 0;
    int highBound = maps[mapIteration].size() - 1;
    bool betweenRanges = false;
    int testVal;
    while (true) {
        testVal = (highBound + lowBound) / 2;
        if (start < maps[mapIteration][testVal]) {
            highBound = testVal - 1;
        }
        else if (maps[mapIteration][testVal] < start) {
            lowBound = testVal + 1;
        }
        else {
            // found range containing start
            break;
        }
        if (lowBound > highBound) {
            // start is between ranges highBound and lowBound (lowBound being the higher index here)
            testVal = lowBound;
            betweenRanges = true;
            break;
        }
    }
    // now find subranges for the next level down
    int64_t subRangeStart = start;
    int64_t lowestLocation = 0x7FFFFFFFFFFFFFFF;
    while (true) {
        if (betweenRanges) {
            // subRangeStart is just before range with index testVal
            if (testVal >= maps[mapIteration].size() || end < maps[mapIteration][testVal].begin) {
                // current subrange is contained entirely between (or after) ranges, pass it verbatim
                int64_t tempVal = recursiveFindMinLocation(maps, mapIteration + 1, subRangeStart, end);
                return std::min(lowestLocation, tempVal);
            }
            else {
                // pass the part of the search range that is between ranges
                int64_t tempVal = recursiveFindMinLocation(maps, mapIteration + 1, subRangeStart, maps[mapIteration][testVal].begin - 1);
                lowestLocation = std::min(lowestLocation, tempVal);
                subRangeStart = maps[mapIteration][testVal].begin;
                betweenRanges = false;
            }
        }
        else {
            // found range containing start, dig deeper
            if (end < maps[mapIteration][testVal].end) {
                // search range is contained entirely in the found range, so pass it verbatim + offset
                int64_t tempVal = recursiveFindMinLocation(maps, mapIteration + 1, subRangeStart + maps[mapIteration][testVal].offset, end + maps[mapIteration][testVal].offset);
                return std::min(lowestLocation, tempVal);
            }
            else {
                // pass the part of the search range that is within the found range
                int64_t tempVal = recursiveFindMinLocation(maps, mapIteration + 1, subRangeStart + maps[mapIteration][testVal].offset, maps[mapIteration][testVal].end + maps[mapIteration][testVal].offset - 1);
                lowestLocation = std::min(lowestLocation, tempVal);
                // prepare to evaluate the rest of the search range
                subRangeStart = maps[mapIteration][testVal].end;
                testVal++;
                if (testVal >= maps[mapIteration].size() || subRangeStart < maps[mapIteration][testVal].begin) {
                    betweenRanges = true;
                }
            }
        }

    }
}

int main(int argc, char* argv[]) {
    std::ifstream ifs;
    if (argc > 1) {
        ifs.open(argv[1]);
    }
    else {
        ifs.open("input.txt");
    }
    if (ifs.fail()) {
        std::cout << "Could not open input file" << std::endl;
        return -1;
    }

    std::string buf;
    ifs >> buf; // "seeds:"

    std::vector<int64_t> seeds;
    int64_t seed;
    ifs >> seed;
    while (!ifs.eof() && !ifs.fail()) {
        std::cout << "Found seed " << seed << std::endl;
        seeds.push_back(seed);
        ifs >> seed;
    }
    ifs.clear();

    std::vector<Range> maps[7];

    for (int i = 0; i < 7; i++) {
        ifs >> buf; // "x-to-y"
        ifs >> buf; // "map:"

        // load map
        int64_t destStart, sourceStart, rangeLength;
        ifs >> destStart;
        ifs >> sourceStart;
        ifs >> rangeLength;
        while (!ifs.eof() && !ifs.fail()) {
            maps[i].emplace_back(sourceStart, sourceStart + rangeLength, destStart - sourceStart);
            ifs >> destStart;
            ifs >> sourceStart;
            ifs >> rangeLength;
        }
        ifs.clear();

        // sort map
        std::sort(maps[i].begin(), maps[i].end());
    }

    // find seeds in maps (part 1)
    int64_t lowestLocation = 0x7FFFFFFFFFFFFFFF;
    for (auto it = seeds.begin(); it != seeds.end(); it++) {
        int64_t lookupVal = *it;
        std::cout << "Mapping " << lookupVal << "...";
        for (int i = 0; i < 7; i++) {
            // binary search
            int lowBound = 0;
            int highBound = maps[i].size() - 1;
            while (true) {
                int testVal = (highBound + lowBound) / 2;
                if (lookupVal < maps[i][testVal]) {
                    highBound = testVal - 1;
                }
                else if (maps[i][testVal] < lookupVal) {
                    lowBound = testVal + 1;
                }
                else {
                    // found range, map value
                    lookupVal += maps[i][testVal].offset;
                    break;
                }
                if (lowBound > highBound) {
                    // between ranges, keep value
                    break;
                }
            }
        }
        std::cout << " to " << lookupVal << std::endl;
        lowestLocation = std::min(lowestLocation, lookupVal);
    }

    std::cout << "Lowest location is " << lowestLocation << std::endl;

    // find ranges in maps (part 2)
    lowestLocation = 0x7FFFFFFFFFFFFFFF;
    for (auto it = seeds.begin(); it != seeds.end(); it++) {
        int64_t start = *it;
        it++;
        int64_t end = start + *it - (int64_t)1;
        lowestLocation = std::min(lowestLocation, recursiveFindMinLocation(maps, 0, start, end));
    }

    std::cout << "Lowest range location is " << lowestLocation << std::endl;

    return 0;
}