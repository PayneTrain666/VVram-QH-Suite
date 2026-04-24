#include "holoram/core.h"
#include <iostream>
#include <vector>

int main() {
    holoram::HoloCoreConfig cfg{};
    auto substrate = holoram::create_cpu_substrate(cfg);
    std::vector<std::uint8_t> block(512, 123), out(512, 0);
    substrate->write_block(1, 0, block.data(), 512, holoram::HoloQualityMode::Exact);
    substrate->read_block(1, 0, out.data(), 512, holoram::HoloQualityMode::Exact);
    std::cout << "QH store demo complete\n";
    return 0;
}
