#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>

const int cardLookup[85] = {
    // lookup table, ASCII to card value
    // return -1 for invalid card
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1,  0,  1,  2,  3,  4,  5,  6,  7, -1, -1, -1, -1, -1, -1,
    -1, 12, -1, -1, -1, -1, -1, -1, -1, -1,  9, 11, -1, -1, -1, -1,
    -1, 10, -1, -1,  8
};

typedef enum {
    UNKNOWN_TYPE = 0,
    HIGH_CARD,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    FIVE_OF_A_KIND
} CardType;

struct Hand {
    int cards[5]; // cards converted to value
    CardType type;
    int bid;
};

CardType classify(const Hand& hand) {
    int counts[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 5; i++) {
        // count each type of card
        counts[hand.cards[i]]++;
    }
    bool hasTriple = false;
    int pairs = 0;
    for (int i = 0; i < 13; i++) {
        // look at the number of each card
        if (counts[i] == 0 || counts[i] == 1) {
            continue;
        }
        else if (counts[i] == 5) {
            return FIVE_OF_A_KIND;
        }
        else if (counts[i] == 4) {
            return FOUR_OF_A_KIND;
        }
        else if (counts[i] == 3) {
            hasTriple = true;
        }
        else if (counts[i] == 2) {
            pairs++;
        }
    }
    if (hasTriple) {
        if (pairs == 1) {
            return FULL_HOUSE;
        }
        else {
            return THREE_OF_A_KIND;
        }
    }
    else {
        if (pairs == 2) {
            return TWO_PAIR;
        }
        else if (pairs == 1) {
            return ONE_PAIR;
        }
        else {
            return HIGH_CARD;
        }
    }
}

CardType classifyPartTwo(const Hand& hand) {
    int counts[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 5; i++) {
        // count each type of card
        counts[hand.cards[i]]++;
    }
    bool hasTriple = false;
    int pairs = 0;
    for (int i = 1; i < 13; i++) {
        // look at the number of each card besides jokers (in counts[0])
        if (counts[i] == 5) {
            return FIVE_OF_A_KIND;
        }
        else if (counts[i] == 4) {
            // hand is either five of a kind with joker or four of a kind
            return (counts[0] > 0) ? FIVE_OF_A_KIND : FOUR_OF_A_KIND;
        }
        else if (counts[i] == 3) {
            // if there's at least one joker, we're done
            if (counts[0] == 2) {
                return FIVE_OF_A_KIND;
            }
            else if (counts[0] == 1) {
                return FOUR_OF_A_KIND;
            }
            hasTriple = true; // check later for a pair
        }
        else if (counts[i] == 2) {
            pairs++;
        }
    }
    if (hasTriple) {
        if (pairs == 1) {
            return FULL_HOUSE;
        }
        else {
            return THREE_OF_A_KIND;
        }
    }
    else {
        if (pairs == 2) {
            // with a joker this becomes a full house
            return (counts[0] == 1) ? FULL_HOUSE : TWO_PAIR;
        }
        else if (pairs == 1) {
            // check joker count
            if (counts[0] == 3) {
                return FIVE_OF_A_KIND;
            }
            else if (counts[0] == 2) {
                return FOUR_OF_A_KIND;
            }
            else if (counts[0] == 1) {
                return THREE_OF_A_KIND;
            }
            else {
                return ONE_PAIR;
            }
        }
        else {
            // check joker count
            if (counts[0] == 5) {
                return FIVE_OF_A_KIND;
            }
            else if (counts[0] == 4) {
                return FIVE_OF_A_KIND;
            }
            else if (counts[0] == 3) {
                return FOUR_OF_A_KIND;
            }
            else if (counts[0] == 2) {
                return THREE_OF_A_KIND;
            }
            else if (counts[0] == 1) {
                return ONE_PAIR;
            }
            else {
                return HIGH_CARD;
            }
        }
    }
}

std::ostream& operator<<(std::ostream& os, const Hand& hand) {
    os << hand.cards[0];
    for (int i = 1; i < 5; i++) {
        os << "," << hand.cards[i];
    }
    os << " " << hand.bid;
    os << " " << hand.type;
    return os;
}

bool operator<(const Hand& lhs, const Hand& rhs) {
    // true if lhs is lower rank, false otherwise
    // hands are assumed to be classified already
    if (lhs.type == rhs.type) {
        // same type, check cards
        for (int i = 0; i < 5; i++) {
            if (lhs.cards[i] != rhs.cards[i]) {
                return (lhs.cards[i] < rhs.cards[i]);
            }
        }
        // equal, just return
        return false;
    }
    return (lhs.type < rhs.type);
}

int main(int argc, char* argv[]) {
    FILE * f;
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

    std::vector<Hand> hands;
    int c = fgetc(f);
    while (!feof(f)) {
        hands.emplace_back();
        Hand& hand = hands.back();
        hand.cards[0] = cardLookup[c];
        for (int i = 1; i < 5; i++) {
            c = fgetc(f);
            hand.cards[i] = cardLookup[c];
        }
        fgetc(f); // space
        c = fgetc(f);
        hand.bid = 0;
        while (c != '\n') {
            hand.bid = 10*hand.bid + c - 48;
            c = fgetc(f);
        }

        // classify while we're here
        hand.type = classify(hand);

        c = fgetc(f);
    }

    fclose(f);

    // sort hands
    std::sort(hands.begin(), hands.end());
    int winnings = 0;
    int i = 1;
    for (auto it = hands.begin(); it != hands.end(); it++) {
        winnings += it->bid*i;
        i++;
    }

    std::cout << "Winnings: " << winnings << std::endl;

    // part two
    // convert hand values and re-classify
    for (auto it = hands.begin(); it != hands.end(); it++) {
        for (int i = 0; i < 5; i++) {
            if (it->cards[i] < 9) {
                it->cards[i]++;
            }
            else if (it->cards[i] > 9) {
                continue;
            }
            else {
                it->cards[i] = 0;
            }
        }
        it->type = classifyPartTwo(*it);
    }

    // sort hands
    std::sort(hands.begin(), hands.end());
    winnings = 0;
    i = 1;
    for (auto it = hands.begin(); it != hands.end(); it++) {
        winnings += it->bid*i;
        i++;
    }

    std::cout << "Winnings: " << winnings << std::endl;
}