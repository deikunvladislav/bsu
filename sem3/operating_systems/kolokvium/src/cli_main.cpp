#include <iostream>
#include <sstream>
#include <string>
#include "factorial.hpp"
#include "dedup.hpp"
#include "singly_list.hpp"

namespace {
    void print_factorials(const std::vector<app::math::BigInt>& facts) {
        for (size_t i = 0; i < facts.size(); ++i)
            std::cout << (i + 1) << "! = " << facts[i].to_string() << "\n";
    }

    template <typename T>
    void print_vector(const std::vector<T>& v, const char* label) {
        std::cout << label << ": ";
        for (const auto& x : v) std::cout << x << " ";
        std::cout << "\n";
    }

    std::vector<int> read_int_vector(const std::string& prompt) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        std::istringstream iss(line);
        std::vector<int> v;
        int x;
        while (iss >> x) v.push_back(x);
        return v;
    }
}

int main() {
    try {
        std::cout << "Enter n (number of factorials to compute): ";
        size_t n;
        std::cin >> n;
        std::cin.ignore();
        auto facts = app::math::first_n_factorials(n);
        print_factorials(facts);

        auto dedup_input = read_int_vector("Enter integers for deduplication (space-separated): ");
        print_vector(app::algo::stable_unique(dedup_input), "Dedup");

        auto list_input = read_int_vector("Enter integers for linked list reversal (space-separated): ");
        auto head = app::list::from_vector(list_input);
        auto rev = app::list::reverse_recursive<int>(std::move(head));
        print_vector(app::list::to_vector(rev), "Reversed");
    }
    catch (const app::AppError& e) {
        std::cerr << "Application error: " << e.what() << "\n";
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << "\n";
        return 2;
    }
    std::cout << "Press Enter to exit...";
    std::cin.get();
    return 0;
}