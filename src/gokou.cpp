#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>

#include "lib/xrand.h"
#include "lib/yaku.h"

xrand rng;

constexpr int inf = 1000000, v = 10;

int score(const std::array<Gain, 2> &gain, int turn) {
    int res = gain[turn].score();
    res *= v;
    if (gain[turn].score_hikari() == 15) res++;
    if (res == 0) return -inf;
    return res;
}

int play(std::array<Gain, 2> &gain, std::array<Gain, 2> &hand, Gain &board,
         const std::array<int, 48> &deck, int seen, int alpha, int beta) {
    if (seen == 40) {
        if (score(gain, 0) == -inf and score(gain, 1) == -inf) {
            return 6 * v;  // 親権
        }
        return 0;
    }
    const int turn = seen & 1, card_deck = deck[seen];
    std::vector<int> cards;
    for (int i = 0; i < 48; i++) {
        if (not hand[turn].get(i)) continue;
        cards.emplace_back(i);
    }
    std::sort(cards.begin(), cards.end(), [&](int a, int b) {
        return (board.match(a).size() ^ 3) < (board.match(b).size() ^ 3);
    });
    for (auto &&card : cards) {
        hand[turn].set(card, false);
        const auto match{board.match(card)};
        if (match.empty()) {
            board.set(card);
            const auto match_deck{board.match(card_deck)};
            if (match_deck.empty()) {
                board.set(card_deck);
                alpha = std::max(alpha, -play(gain, hand, board, deck, seen + 1,
                                              -beta, -alpha));
                board.set(card_deck, false);
            } else {
                for (auto &&take_deck : match_deck) {
                    board.set(take_deck, false);
                    gain[turn].set(card_deck);
                    gain[turn].set(take_deck);
                    alpha = std::max(alpha, score(gain, turn));
                    if (alpha < beta) {
                        alpha = std::max(alpha, -play(gain, hand, board, deck,
                                                      seen + 1, -beta, -alpha));
                    }
                    board.set(take_deck);
                    gain[turn].set(card_deck, false);
                    gain[turn].set(take_deck, false);
                    if (alpha >= beta) break;
                }
            }
            board.set(card, false);
        } else {
            for (auto &&take : match) {
                board.set(take, false);
                gain[turn].set(card);
                gain[turn].set(take);
                const auto match_deck{board.match(card_deck)};
                if (match_deck.empty()) {
                    board.set(card_deck);
                    alpha = std::max(alpha, score(gain, turn));
                    if (alpha < beta) {
                        alpha = std::max(alpha, -play(gain, hand, board, deck,
                                                      seen + 1, -beta, -alpha));
                    }
                    board.set(card_deck, false);
                } else {
                    for (auto &&take_deck : match_deck) {
                        board.set(take_deck, false);
                        gain[turn].set(card_deck);
                        gain[turn].set(take_deck);
                        alpha = std::max(alpha, score(gain, turn));
                        if (alpha < beta) {
                            alpha =
                                std::max(alpha, -play(gain, hand, board, deck,
                                                      seen + 1, -beta, -alpha));
                        }
                        board.set(take_deck);
                        gain[turn].set(card_deck, false);
                        gain[turn].set(take_deck, false);
                        if (alpha >= beta) break;
                    }
                }
                board.set(take);
                gain[turn].set(card, false);
                gain[turn].set(take, false);
                if (alpha >= beta) break;
            }
        }
        hand[turn].set(card);
        if (alpha >= beta) break;
    }

    return alpha;
}

int main() {
    constexpr int iter = 100000;

    std::map<int, int> result;
    for (int _ = 0; _ < iter; _++) {
        for (;;) {
            std::array<Gain, 2> gain{};
            std::array<Gain, 2> hand{};
            Gain board{};
            std::array<int, 48> deck;
            std::iota(deck.begin(), deck.end(), 0);
            std::shuffle(deck.begin(), deck.end(), rng);
            for (int i = 0; i < 8; i++) {
                hand[0].set(deck[i]);
            }
            for (int i = 0; i < 8; i++) {
                hand[1].set(deck[i + 8]);
            }
            for (int i = 0; i < 8; i++) {
                board.set(deck[i + 16]);
            }

            bool good = true;
            for (int i = 0; i < 12; i++) {
                if (hand[0].match(4 * i).size() > 2) good = false;
                if (hand[1].match(4 * i).size() > 2) good = false;
                if (board.match(4 * i).size() > 2) good = false;
            }

            if (good) {
                int res = play(gain, hand, board, deck, 24, -1, 1);
                if (res > 0) {
                    res = play(gain, hand, board, deck, 24, res, inf);
                } else {
                    res = play(gain, hand, board, deck, 24, -inf, res);
                }
                result[res]++;
                break;
            }
        }
    }

    int s = 0, s_abs = 0, win_count = 0, gokou = 0, gokou_d = 0;
    for (auto &&[res_raw, count] : result) {
        int res = res_raw / v, r = res_raw % v;
        std::cout << res_raw << '\t' << count << std::endl;
        s += res * count;
        s_abs += abs(res) * count;
        if (res > 0) win_count += count;
        gokou += abs(r) * count;
        gokou_d += r * count;
    }
    std::cout << std::endl;
    const double avg = double(s) / iter, win_rate = double(win_count) / iter;
    std::cout << "average\t" << avg << std::endl;
    std::cout << "avg_win\t" << double(s_abs) / iter << std::endl;
    std::cout << "winrate\t" << win_rate << std::endl;
    std::cout << "expect\t" << avg / (1 - win_rate) << std::endl;
    std::cout << "gokou\t" << double(gokou) / iter << std::endl;
    std::cout << "gokou_d\t" << double(gokou_d) / iter << std::endl;
    return 0;
}
