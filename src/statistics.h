#pragma once

#include <cstddef>
#include <vector>

/// @brief Counts of stacks by length, with statistics computed from them.
/// A stack is never longer than C, so memory depends on C, not on the number of stacks.
class LengthHistogram {
public:
    /// @brief Registers one stack.
    /// @param length Stack length, at least 1.
    /// @throws std::invalid_argument if length is 0.
    void add(std::size_t length);

    /// @return Total number of registered stacks.
    [[nodiscard]] std::size_t total_stacks() const noexcept;

    /// @param length Stack length.
    /// @return Number of stacks of exactly this length (0 if there are none).
    [[nodiscard]] std::size_t count(std::size_t length) const noexcept;

    /// @param length Stack length.
    /// @return Percentage of stacks of exactly this length.
    /// @throws std::logic_error if no stacks were registered.
    [[nodiscard]] double percentage(std::size_t length) const;

    /// @return Mean stack length.
    /// @throws std::logic_error if no stacks were registered.
    [[nodiscard]] double mean() const;

    /// @return Median stack length (average of the two middle values for an even count).
    /// @throws std::logic_error if no stacks were registered.
    [[nodiscard]] double median() const;

    /// @return Most frequent stack length; the smallest one if several are tied.
    /// @throws std::logic_error if no stacks were registered.
    [[nodiscard]] std::size_t mode() const;

private:
    void require_stacks() const;
    [[nodiscard]] std::size_t length_at_rank(std::size_t rank) const;

    std::vector<std::size_t> counts_;  // counts_[len] = number of stacks of length len
    std::size_t total_stacks_ = 0;
};

/// @brief Expected mean stack length for a long deal.
/// A new stack starts when two neighbouring cards are not an ascent. Inside a deck
/// two cards share a rank with probability 3/(4C-1); across a reshuffle, with 1/C.
/// This gives P(new stack) = (4C^2 + 3C + 1) / (8C^2), and the mean is 1 / P.
/// @param cards_per_suit Number of cards in each suit (C).
/// @return Expected mean stack length.
/// @throws std::invalid_argument if cards_per_suit <= 0.
[[nodiscard]] double expected_mean_length(int cards_per_suit);
