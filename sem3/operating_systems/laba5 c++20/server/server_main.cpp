#include <cstdlib>

int run_server();

int main() {
    const int code = run_server();
    std::exit(code);
}