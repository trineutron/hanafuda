#include "lib/yaku.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

#include "lib/xrand.h"

xrand rng;

int main() {
    int score_dist[60]{};
    for (int k = 0; k < 1000000; k++) {
        std::array<int, 48> deck;
        std::iota(deck.begin(), deck.end(), 0);
        std::shuffle(deck.begin(), deck.end(), rng);

        std::array<int, 12> board;
        for (int i = 0; i < 12; i++) {
            board[i] = -1;
        }

        std::array<Gain, 2> hand{};
        std::array<int, 2> score{0, 0};
        int winner = 0;
        for (int i = 0; i < 40; i++) {
            const int card = deck[i];
            if (board[card >> 2] == -1) {
                board[card >> 2] = card;
            } else {
                hand[i & 1].set(board[card >> 2], true);
                hand[i & 1].set(card, true);
                board[card >> 2] = -1;
                if (hand[i & 1].score()) {
                    if (score[1 - (i & 1)]) break;
                    score[i & 1] = hand[i & 1].score();
                    winner = i & 1;
                }
            }
        }
        score_dist[hand[winner].score_kasu()]++;
    }

    int maximum = 0, total = 0;
    for (int i = 0; i < 60; i++) {
        if (score_dist[i]) maximum = i;
        total += i * score_dist[i];
    }
    for (int i = 0; i <= maximum; i++) {
        std::cout << i << '\t' << score_dist[i] << std::endl;
    }
    std::cout << '\t' << total << std::endl;
    return 0;
}
