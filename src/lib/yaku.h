#include <bitset>
#include <vector>

class Gain {
    using State = std::bitset<48>;

    State gain;

   public:
    Gain() : gain() {}
    Gain(State hand) : gain(hand) {}

    void set(int idx, bool x = true) { gain[idx] = x; }

    std::vector<int> match(int card) {
        const int base = card & ~3;
        std::vector<int> res;
        for (int i = 0; i < 4; i++) {
            if (gain[base + i]) {
                res.emplace_back(base + i);
            }
        }
        return res;
    }

    int score_hikari() {
        constexpr State mask_hikari{0x100010000101};
        const int hikari = 2 * (gain & mask_hikari).count() + gain[40];
        switch (hikari) {
            case 9:
                return 15;  // 五光
            case 8:
                return 10;  // 四光
            case 7:
                return 8;  // 雨四光
            case 6:
                return 6;  // 三光
            default:
                return 0;
        }
    }

    int score_akatan() {
        constexpr State mask_akatan{0x000000000222};
        const int akatan = (gain & mask_akatan).count();
        if (akatan == 3) {
            return 6;
        } else {
            return 0;
        }
    }

    int score_aotan() {
        constexpr State mask_aotan{0x002200200000};
        const int aotan = (gain & mask_aotan).count();
        if (aotan == 3) {
            return 6;
        } else {
            return 0;
        }
    }

    int score_inoshika() {
        constexpr State mask_inoshika{0x001001100000};
        const int inoshika = (gain & mask_inoshika).count();
        if (inoshika == 3) {
            return 5;
        } else {
            return 0;
        }
    }

    int score_tane() {
        constexpr State mask_tane{0x021121111010};
        const int tane = (gain & mask_tane).count();
        if (tane >= 5) {
            return tane - 4;
        } else {
            return 0;
        }
    }

    int score_tan() {
        constexpr State mask_tan{0x042202222222};
        const int tan = (gain & mask_tan).count();
        if (tan >= 5) {
            return tan - 4;
        } else {
            return 0;
        }
    }

    int score_kasu() {
        constexpr State mask_kasu{0xE8CDCCCCCCCC};
        const int kasu = (gain & mask_kasu).count();
        if (kasu >= 10) {
            return kasu - 9;
        } else {
            return 0;
        }
    }

    int score() {
        return score_hikari() + score_akatan() + score_aotan() +
               score_inoshika() + score_tane() + score_tan() + score_kasu();
    }
};
