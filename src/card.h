#pragma once

#include <compare>

/// @brief Playing card. Cards are compared by rank only; the suit is ignored.
struct Card {
    int rank;  ///< Value from 0 to C-1, where C is the number of cards per suit.
    int suit;  ///< Suit from 0 to 3.

    /// @brief Three-way comparison by rank.
    /// @param other Card to compare with.
    /// @return Ordering of the two ranks.
    [[nodiscard]] std::strong_ordering operator<=>(const Card& other) const noexcept {
        return rank <=> other.rank;
    }
};
