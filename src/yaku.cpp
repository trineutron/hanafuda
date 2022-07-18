#include <bitset>
#include <iostream>

#include "lib/xrand.h"

xrand rng;

using Gain = std::bitset<48>;

int score(Gain gain) {
    constexpr Gain mask_hikari{0x100010000101};
    constexpr Gain mask_akatan{0x000000000222};
    constexpr Gain mask_aotan{0x002200200000};
    constexpr Gain mask_inoshika{0x001001100000};
    constexpr Gain mask_tane{0x021121111010};
    constexpr Gain mask_tan{0x042202222222};
    constexpr Gain mask_kasu{0xE8CDCCCCCCCC};

    const int hikari = 2 * (gain & mask_hikari).count() + gain[40];
    const int akatan = (gain & mask_akatan).count();
    const int aotan = (gain & mask_aotan).count();
    const int inoshika = (gain & mask_inoshika).count();
    const int tane = (gain & mask_tane).count();
    const int tan = (gain & mask_tan).count();
    const int kasu = (gain & mask_kasu).count();

    int result = 0;
    switch (hikari) {
        case 9:
            result += 15;  // 五光
            break;
        case 8:
            result += 10;  // 四光
            break;
        case 7:
            result += 8;  // 雨四光
            break;
        case 6:
            result += 6;  // 三光
            break;
    }
    if (akatan == 3) result += 6;
    if (aotan == 3) result += 6;
    if (inoshika == 3) result += 5;
    if (tane >= 5) result += tane - 4;
    if (tan >= 5) result += tan - 4;
    if (kasu >= 10) result += kasu - 9;

    return result;
}

int main() {
    std::uniform_int_distribution<> dist(0, 2);
    int score_dist[60]{};
    for (int k = 0; k < 10000000; k++) {
        Gain hand;
        for (int i = 0; i < 48; i++) {
            hand[i] = dist(rng) == 0;
        }
        score_dist[score(hand)]++;
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
