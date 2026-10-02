#include "deck_dealer.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <stdexcept>

namespace {

/// @brief Creates a Mersenne Twister seeded with 256 bits from std::random_device.
std::mt19937 make_seeded_engine() {
    std::random_device device;
    std::array<std::uint32_t, 8> seed_data{};
    std::ranges::generate(seed_data, [&device] { return device(); });
    std::seed_seq seed_sequence(seed_data.begin(), seed_data.end());
    return std::mt19937(seed_sequence);
}

}  // namespace

DeckDealer::DeckDealer(int cards_per_suit) : rng_(make_seeded_engine()), current_index_(0) {
    if (cards_per_suit <= 0) {
        throw std::invalid_argument("Number of cards per suit must be greater than 0.");
    }

    const int num_suits = 4;
    // Cast to std::size_t before multiplying to avoid int overflow.
    deck_.reserve(static_cast<std::size_t>(num_suits) * static_cast<std::size_t>(cards_per_suit));
    for (int suit = 0; suit < num_suits; ++suit) {
        for (int rank = 0; rank < cards_per_suit; ++rank) {
            deck_.push_back(Card{rank, suit});
        }
    }

    std::ranges::shuffle(deck_, rng_);
}

Card DeckDealer::operator()() {
    if (current_index_ == deck_.size()) {
        std::ranges::shuffle(deck_, rng_);
        current_index_ = 0;
    }
    return deck_[current_index_++];
}
