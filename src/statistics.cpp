#include "statistics.h"

#include <algorithm>
#include <functional>
#include <stdexcept>

void LengthHistogram::add(std::size_t length) {
    if (length == 0) {
        throw std::invalid_argument("Stack length must be at least 1.");
    }
    if (length >= counts_.size()) {
        counts_.resize(length + 1, 0);
    }
    ++counts_[length];
    ++total_stacks_;
}

std::size_t LengthHistogram::total_stacks() const noexcept { return total_stacks_; }

std::size_t LengthHistogram::count(std::size_t length) const noexcept {
    return length < counts_.size() ? counts_[length] : 0;
}

double LengthHistogram::percentage(std::size_t length) const {
    require_stacks();
    return 100.0 * static_cast<double>(count(length)) / static_cast<double>(total_stacks_);
}

double LengthHistogram::mean() const {
    require_stacks();
    std::size_t total_cards = 0;
    for (std::size_t length = 1; length < counts_.size(); ++length) {
        total_cards += length * counts_[length];
    }
    return static_cast<double>(total_cards) / static_cast<double>(total_stacks_);
}

double LengthHistogram::median() const {
    require_stacks();
    // For an odd count both ranks are the same element.
    const std::size_t lower = length_at_rank((total_stacks_ - 1) / 2);
    const std::size_t upper = length_at_rank(total_stacks_ / 2);
    return (static_cast<double>(lower) + static_cast<double>(upper)) / 2.0;
}

std::size_t LengthHistogram::mode() const {
    require_stacks();
    // max_element returns the first maximum, i.e. the smallest length on a tie.
    const auto most_frequent = std::ranges::max_element(counts_, std::less<>{});
    return static_cast<std::size_t>(most_frequent - counts_.begin());
}

void LengthHistogram::require_stacks() const {
    if (total_stacks_ == 0) {
        throw std::logic_error("No stacks were registered.");
    }
}

/// @brief Finds the length at a 0-based position in the sorted list of all stack lengths.
std::size_t LengthHistogram::length_at_rank(std::size_t rank) const {
    std::size_t passed = 0;
    for (std::size_t length = 1; length < counts_.size(); ++length) {
        passed += counts_[length];
        if (passed > rank) {
            return length;
        }
    }
    throw std::logic_error("Rank is out of range.");
}

double expected_mean_length(int cards_per_suit) {
    if (cards_per_suit <= 0) {
        throw std::invalid_argument("Number of cards per suit must be greater than 0.");
    }
    const double c = cards_per_suit;
    return 8.0 * c * c / (4.0 * c * c + 3.0 * c + 1.0);
}
