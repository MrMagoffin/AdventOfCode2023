#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

class Scratchcard {
public:
    Scratchcard() : score(0), matches(0), instanceCount(1) {}
    void insertWinningNumber(int winningNumber) {
        winningNumbers.insert(winningNumber);
    }
    void evaluateNum(int num) {
        if (winningNumbers.find(num) != winningNumbers.end()) {
            matches++;
            if (score == 0) {
                score = 1;
            }
            else {
                score *= 2;
            }
        }
    }
    int getScore() {
        return score;
    }
    int getMatches() {
        return matches;
    }
    void copy(int copies = 1) {
        instanceCount += copies;
    }
    int getInstanceCount() {
        return instanceCount;
    }
private:
    std::unordered_set<int> winningNumbers;
    int score;
    int matches;
    int instanceCount;
};

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

    int scoreSum = 0;

    std::string buf;
    ifs >> buf; // "Card"
    std::vector<Scratchcard> scratchList;
    while (!ifs.eof()) {
        scratchList.emplace_back();
        int cardID, num;
        ifs >> cardID;
        ifs >> buf; // colon

        ifs >> num;
        while (!ifs.fail()) {
            scratchList.back().insertWinningNumber(num);
            ifs >> num;
        }
        ifs.clear();

        ifs >> buf; // separator

        int score = 0;

        ifs >> num;
        while (!ifs.fail()) {
            scratchList.back().evaluateNum(num);
            ifs >> num;
        }
        ifs.clear();
        
        scoreSum += scratchList.back().getScore();

        ifs >> buf; // "Card"
    }

    std::cout << "Score sum: " << scoreSum << std::endl;

    // part 2
    int listCount = 0;
    for (auto it = scratchList.begin(); it != scratchList.end(); it++) {
        listCount += it->getInstanceCount();
        if (it->getMatches() > 0) {
            auto jt = it + 1;
            for (int i = 0; jt != scratchList.end() && i < it->getMatches(); i++) {
                jt->copy(it->getInstanceCount());
                jt++;
            }
        }
    }

    std::cout << "Total scratchers: " << listCount << std::endl;

    return 0;
}