// Compiler used: MSVC 19.51.36260 (Visual Studio 2026 / Build Tools v18.10.3), Standard: C++23

#include <charconv>
#include <cstddef>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

#include "deck_dealer.h"
#include "simulation.h"
#include "statistics.h"

namespace {

constexpr int MAX_C_LIMIT = 10'000;
constexpr std::size_t MAX_N_LIMIT = 50'000'000;

/// @brief Program parameters.
struct Parameters {
    int cards_per_suit;
    std::size_t deal_count;
};

/// @brief Parses an integer and checks that it lies in [min, max].
/// @param text Text to parse.
/// @param name Parameter name used in error messages.
/// @param min Smallest allowed value.
/// @param max Largest allowed value.
/// @return Parsed value.
/// @throws std::invalid_argument if the text is not an integer.
/// @throws std::out_of_range if the value is outside [min, max].
long long parse_in_range(std::string_view text, const std::string& name, long long min, long long max) {
    long long value = 0;
    const char* begin = text.data();
    const char* end = begin + text.size();
    const auto [ptr, error_code] = std::from_chars(begin, end, value);

    if (error_code == std::errc::invalid_argument || (error_code == std::errc{} && ptr != end)) {
        throw std::invalid_argument("Invalid integer format for " + name + ".");
    }
    if (error_code == std::errc::result_out_of_range || value < min || value > max) {
        throw std::out_of_range(name + " must be in [" + std::to_string(min) + ", " + std::to_string(max) + "].");
    }
    return value;
}

/// @brief Converts the text values of C and n into validated parameters.
Parameters make_parameters(std::string_view c_text, std::string_view n_text) {
    return Parameters{
        static_cast<int>(parse_in_range(c_text, "C", 1, MAX_C_LIMIT)),
        static_cast<std::size_t>(parse_in_range(n_text, "n", 1, static_cast<long long>(MAX_N_LIMIT)))};
}

/// @brief Asks the user to enter C and n.
Parameters read_parameters_interactively() {
    std::string c_input;
    std::string n_input;

    std::cout << "Enter number of cards per suit (C): ";
    if (!(std::cin >> c_input)) {
        throw std::invalid_argument("Invalid input for C.");
    }
    std::cout << "Enter total number of cards to deal (n): ";
    if (!(std::cin >> n_input)) {
        throw std::invalid_argument("Invalid input for n.");
    }
    return make_parameters(c_input, n_input);
}

/// @brief Prints the percentage of stacks for every length from 1 to C, then mean, median and mode.
void print_results(const LengthHistogram& stacks, int cards_per_suit) {
    const int index_width = static_cast<int>(std::to_string(cards_per_suit).size());

    std::cout << "\n=== Results ===\n";
    std::cout << "Total stacks generated: " << stacks.total_stacks() << '\n';

    std::cout << "\n--- Stack Length Percentages ---\n";
    std::cout << std::fixed << std::setprecision(2);
    for (int length = 1; length <= cards_per_suit; ++length) {
        const auto len = static_cast<std::size_t>(length);
        std::cout << "Length " << std::setw(index_width) << length << ": "
                  << std::setw(6) << stacks.percentage(len) << "% ("
                  << stacks.count(len) << " stacks)\n";
    }

    std::cout << std::setprecision(4);
    std::cout << "\nMean length:   " << stacks.mean() << '\n';
    std::cout << "Median length: " << stacks.median() << '\n';
    std::cout << "Mode length:   " << stacks.mode() << '\n';
}

/// @brief Runs the experiment: mean and median stack length for several values of C.
void run_experiment() {
    constexpr std::size_t n_experiment = 1'000'000;
    const std::vector<int> c_values = {1, 2, 4, 8, 13, 20, 50, 100};

    std::cout << "=== Running Experiment (n = " << n_experiment << ") ===\n";
    std::cout << std::left << std::setw(15) << "Cards/Suit (C)"
              << std::setw(15) << "Mean Length"
              << std::setw(15) << "Expected Mean"
              << std::setw(15) << "Median Length"
              << "Mode\n";
    std::cout << "-------------------------------------------------------------\n";
    std::cout << std::fixed << std::setprecision(4);

    for (int c : c_values) {
        DeckDealer dealer(c);
        const LengthHistogram stacks = count_stacks(dealer, n_experiment);

        std::cout << std::setw(15) << c
                  << std::setw(15) << stacks.mean()
                  << std::setw(15) << expected_mean_length(c)
                  << std::setw(15) << stacks.median()
                  << stacks.mode() << '\n';
    }
}

}  // namespace

int main(int argc, char* argv[]) {
    try {
        if (argc == 2 && std::string_view(argv[1]) == "--experiment") {
            run_experiment();
            return EXIT_SUCCESS;
        }

        Parameters parameters{};
        if (argc == 1) {
            parameters = read_parameters_interactively();
        } else if (argc == 3) {
            parameters = make_parameters(argv[1], argv[2]);
        } else {
            throw std::invalid_argument("Usage: program [C n] OR program --experiment");
        }

        DeckDealer dealer(parameters.cards_per_suit);
        const LengthHistogram stacks = count_stacks(dealer, parameters.deal_count);
        print_results(stacks, parameters.cards_per_suit);
    } catch (const std::exception& e) {
        std::cerr << "\n[Error] " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
