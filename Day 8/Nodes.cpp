#include <cstdio>
#include <iostream>
#include <vector>
#include <utility>
#include <unordered_map>
#include <string>
#include <cstring>

u_int64_t euclid(u_int64_t x, u_int64_t y) {
    if (x < y) {
        std::swap(x, y);
    }
    do {
        u_int64_t rem = x % y;
        x = y;
        y = rem;
    } while (y != 0);
    return x;
}

struct Node {
    std::string name;
    std::string leftName;
    std::string rightName;
    Node* left;
    Node* right;
    u_int64_t cycleLength;
};

int main(int argc, char* argv[]) {
    FILE* f;
    if (argc > 1) {
        f = fopen(argv[1], "r");
    }
    else {
        f = fopen("input.txt", "r");
    }
    if (!f) {
        std::cout << "Could not open input file" << std::endl;
        return -1;
    }

    std::vector<int> instructions;
    int c;
    c = fgetc(f);
    while (!feof(f) && c != '\n') {
        instructions.emplace_back(c);
        c = fgetc(f);
    }

    // skip newlines
    do {
        c = fgetc(f);
    } while (!feof(f) && c == '\n');

    // load nodes
    std::unordered_map<std::string, Node*> nodeMap;
    while (!feof(f)) {
        Node* node = new Node;
        do {
            node->name.push_back(c);
            c = fgetc(f);
        } while (c != ' ');
        do {
            c = fgetc(f);
        } while (c != '(');
        c = fgetc(f);
        do {
            node->leftName.push_back(c);
            c = fgetc(f);
        } while (c != ',');
        c = fgetc(f); // space
        c = fgetc(f);
        do {
            node->rightName.push_back(c);
            c = fgetc(f);
        } while (c != ')');
        nodeMap.insert(std::make_pair(node->name, node));
        c = fgetc(f);
        c = fgetc(f);
    }

    // connect nodes
    for (auto it = nodeMap.begin(); it != nodeMap.end(); it++) {
        it->second->left = nodeMap[it->second->leftName];
        it->second->right = nodeMap[it->second->rightName];
    }

    Node* cur = nullptr;
    u_int64_t steps = 0;
    auto it = instructions.begin();
    try {
        cur = nodeMap.at("AAA");
    }
    catch (std::out_of_range &e) {
        std::cout << "Skipping part 1" << std::endl;
    }
    if (cur != nullptr) {
        while (cur->name != "ZZZ") {
            if (*it == 'L') {
                cur = cur->left;
            }
            else {
                cur = cur->right;
            }
            steps++;
            it++;
            if (it == instructions.end()) {
                it = instructions.begin();
            }
        }

        std::cout << "Took " << steps << " steps." << std::endl;
    }

    // part 2
    // find all A nodes
    std::vector<Node*> curNodes;
    for (auto it = nodeMap.begin(); it != nodeMap.end(); it++) {
        if (it->second->name[2] == 'A') {
            curNodes.push_back(it->second);
        }
    }
    std::cout << "Found " << curNodes.size() << " A nodes." << std::endl;

    // calculate cycle lengths
    for (auto curNode = curNodes.begin(); curNode != curNodes.end(); curNode++) {
        Node* start = *curNode;
        it = instructions.begin();
        steps = 1;
        if (*it == 'L') {
            (*curNode) = (*curNode)->left;
        }
        else {
            (*curNode) = (*curNode)->right;
        }
        it++;
        while ((*curNode)->name[2] != 'Z') {
            if (*it == 'L') {
                (*curNode) = (*curNode)->left;
            }
            else {
                (*curNode) = (*curNode)->right;
            }
            steps++;
            it++;
            if (it == instructions.end()) {
                it = instructions.begin();
            }
        }
        start->cycleLength = steps;
        *curNode = start;
    }

    // calculate LCM
    auto nodeIt = curNodes.begin();
    u_int64_t lcm = (*nodeIt)->cycleLength;
    nodeIt++;
    for (; nodeIt != curNodes.end(); nodeIt++) {
        u_int64_t cycleLength = (*nodeIt)->cycleLength;
        lcm = (lcm*cycleLength)/euclid(lcm, cycleLength);
    }
    std::cout << "LCM: " << lcm << std::endl;
}