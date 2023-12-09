#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

class Node {
public:
    Node(Node* _left = nullptr) : left(_left), upLeft(nullptr), upRight(nullptr), downLeft(nullptr), valueIsSet(false) {}
    ~Node() {
        // delete down-left children
        if (downLeft != nullptr) {
            delete downLeft;
        }
    }
    void setValue(int64_t _value) {
        value = _value;
        valueIsSet = true;
    }
    int64_t getValue() {
        if (valueIsSet) {
            return value;
        }
        else {
            if (upLeft == nullptr) {
                upLeft = upRight->getLeft();
            }
            value = upRight->getValue() - upLeft->getValue();
            valueIsSet = true;
            return value;
        }
    }
    Node* getLeft() {
        if (left != nullptr) {
            return left;
        }
        else if (upLeft != nullptr) {
            left = upLeft->getDownLeft();
            return left;
        }
        else {
            upLeft = upRight->getLeft();
            left = upLeft->getDownLeft();
            return left;
        }
    }
    Node* getDownLeft() {
        if (downLeft != nullptr) {
            return downLeft;
        }
        // if we don't have a downLeft, we're responsible for creating it
        downLeft = new Node(); // assigns our downLeft
        downLeft->upRight = this;
        return downLeft;
    }
protected:
    Node* left;
    Node* upLeft;
    Node* upRight;
    Node* downLeft;
    bool valueIsSet;
    int64_t value;
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

    std::vector<std::vector<u_int64_t>> values;
    std::string line;
    while (std::getline(ifs, line)) {
        values.emplace_back();
        std::istringstream iss(line);
        int64_t value;
        while (iss >> value) {
            values.back().push_back(value);
        }
    }

    int64_t bigSum = 0;
    for (auto it = values.begin(); it != values.end(); it++) {
        // iterator is current list of values
        // build initial graph
        std::vector<Node*> topRow;
        Node* cur = nullptr;
        for (auto jt = it->begin(); jt != it->end(); jt++) {
            cur = new Node(cur);
            topRow.push_back(cur);
            cur->setValue(*jt);
        }
        // descend until the triangle is filled, summing values
        int64_t sum = cur->getValue();
       for (int i = 1; i < topRow.size(); i++) {
            cur = cur->getDownLeft();
            sum += cur->getValue();
        }
        bigSum += sum;
        // delete graph
        for (auto it = topRow.begin(); it != topRow.end(); it++) {
            delete *it;
        }
    }
    std::cout << "Final sum: " << bigSum << std::endl;

    // part 2
    bigSum = 0;
    for (auto it = values.begin(); it != values.end(); it++) {
        // iterator is current list of values
        // build initial graph
        std::vector<Node*> topRow;
        Node* cur = nullptr;
        for (auto jt = it->rbegin(); jt != it->rend(); jt++) {
            cur = new Node(cur);
            topRow.push_back(cur);
            cur->setValue(*jt);
        }
        // descend until the triangle is filled, summing values
        int64_t sum = cur->getValue();
       for (int i = 1; i < topRow.size(); i++) {
            cur = cur->getDownLeft();
            sum += cur->getValue();
        }
        bigSum += sum;
        // delete graph
        for (auto it = topRow.begin(); it != topRow.end(); it++) {
            delete *it;
        }
    }
    std::cout << "Final sum: " << bigSum << std::endl;

    return 0;
}