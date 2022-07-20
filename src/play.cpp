#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <vector>

#include "lib/xrand.h"
#include "lib/yaku.h"

xrand rng;

constexpr int inf = 100;

int score(const std::array<Gain, 2> &gain, int turn) {
    int res = gain[turn].score();
    if (res == 0) return -inf;
    return res;
}

int play(std::array<Gain, 2> &gain, std::array<Gain, 2> &hand, Gain &board,
         const std::array<int, 48> &deck, int seen, int alpha, int beta) {
    if (seen == 40) {
        if (gain[0].score() == 0 and gain[1].score() == 0) {
            return 6;  // 親権
        }
        return 0;
    }
    const int turn = seen & 1, card_deck = deck[seen];
    int res = alpha;
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
                res = std::max(
                    res, -play(gain, hand, board, deck, seen + 1, -beta, -res));
                board.set(card_deck, false);
            } else {
                for (auto &&take_deck : match_deck) {
                    board.set(take_deck, false);
                    gain[turn].set(card_deck);
                    gain[turn].set(take_deck);
                    res = std::max(res, score(gain, turn));
                    if (res < beta) {
                        res = std::max(res, -play(gain, hand, board, deck,
                                                  seen + 1, -beta, -res));
                    }
                    board.set(take_deck);
                    gain[turn].set(card_deck, false);
                    gain[turn].set(take_deck, false);
                    if (res >= beta) break;
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
                    res = std::max(res, score(gain, turn));
                    if (res < beta) {
                        res = std::max(res, -play(gain, hand, board, deck,
                                                  seen + 1, -beta, -res));
                    }
                    board.set(card_deck, false);
                } else {
                    for (auto &&take_deck : match_deck) {
                        board.set(take_deck, false);
                        gain[turn].set(card_deck);
                        gain[turn].set(take_deck);
                        res = std::max(res, score(gain, turn));
                        if (res < beta) {
                            res = std::max(res, -play(gain, hand, board, deck,
                                                      seen + 1, -beta, -res));
                        }
                        board.set(take_deck);
                        gain[turn].set(card_deck, false);
                        gain[turn].set(take_deck, false);
                        if (res >= beta) break;
                    }
                }
                board.set(take);
                gain[turn].set(card, false);
                gain[turn].set(take, false);
                if (res >= beta) break;
            }
        }
        hand[turn].set(card);
        if (res >= beta) break;
    }

    return res;
}

int main() {
    int s = 0;
    for (int _ = 0; _ < 100; _++) {
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
                std::cout << res << std::endl;
                s += res;
                break;
            }
        }
    }
    std::cout << s << std::endl;
    return 0;
}
