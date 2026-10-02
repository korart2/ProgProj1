#pragma once

#include <cstddef>
#include <random>
#include <vector>

#include "card.h"

/// @brief Function object that deals cards from a deck of 4 suits.
/// The deck is shuffled on creation and again each time it runs out.
class DeckDealer {
public:
    /// @brief Creates and shuffles the deck.
    /// @param cards_per_suit Number of cards in each suit (C).
    /// @throws std::invalid_argument if cards_per_suit <= 0.
    explicit DeckDealer(int cards_per_suit);

    /// @brief Deals the next card, reshuffling the deck if it is exhausted.
    /// @return The dealt card.
    [[nodiscard]] Card operator()();

private:
    std::vector<Card> deck_;
    std::mt19937 rng_;
    std::size_t current_index_;
};
