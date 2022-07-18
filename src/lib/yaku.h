#include <bitset>

class Gain {
    using State = std::bitset<48>;

    State gain;

   public:
    Gain() : gain() {}
    Gain(State hand) : gain(hand) {}

    void set(int idx, bool x) { gain[idx] = x; }

    int score() {
        constexpr State mask_hikari{0x100010000101};
        constexpr State mask_akatan{0x000000000222};
        constexpr State mask_aotan{0x002200200000};
        constexpr State mask_inoshika{0x001001100000};
        constexpr State mask_tane{0x021121111010};
        constexpr State mask_tan{0x042202222222};
        constexpr State mask_kasu{0xE8CDCCCCCCCC};

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
};
