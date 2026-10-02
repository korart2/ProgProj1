#pragma once

#include <compare>
#include <cstddef>
#include <stdexcept>

#include "card.h"
#include "statistics.h"

/// @brief Deals n cards and counts the lengths of the stacks they form.
/// A card strictly greater than the previous one extends the stack; otherwise the
/// stack ends and a new one starts. The last stack (cut off by n) is counted too,
/// so the stack lengths always add up to n.
/// @tparam Dealer Callable returning the next Card, e.g. DeckDealer.
/// @param deal Source of cards.
/// @param n Number of cards to deal.
/// @return Histogram of stack lengths.
/// @throws std::invalid_argument if n == 0.
template <typename Dealer>
[[nodiscard]] LengthHistogram count_stacks(Dealer& deal, std::size_t n) {
    if (n == 0) {
        throw std::invalid_argument("Number of cards to deal must be greater than 0.");
    }

    LengthHistogram stacks;
    Card previous = deal();
    std::size_t length = 1;

    for (std::size_t i = 1; i < n; ++i) {
        const Card current = deal();
        if (std::is_gt(current <=> previous)) {
            ++length;
        } else {
            stacks.add(length);
            length = 1;
        }
        previous = current;
    }

    stacks.add(length);
    return stacks;
}
