#include "lib/yaku.h"

#include <bitset>
#include <iostream>

#include "lib/xrand.h"

xrand rng;

int main() {
    std::uniform_int_distribution<> dist(0, 2);
    int score_dist[60]{};
    for (int k = 0; k < 10000000; k++) {
        Gain hand;
        for (int i = 0; i < 48; i++) {
            hand.set(i, dist(rng) == 0);
        }
        score_dist[hand.score()]++;
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
