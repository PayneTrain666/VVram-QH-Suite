#include "holoram/qh_substrate.h"
#include <cassert>
#include <vector>

int main() {
    holoram::HoloCoreConfig cfg{};
    auto sub = holoram::create_cpu_substrate(cfg);
    std::vector<std::uint8_t> input(512, 7), output(512, 0);
    auto err = sub->write_block(1, 0, input.data(), static_cast<std::uint32_t>(input.size()), holoram::HoloQualityMode::Exact);
    assert(err == holoram::HoloError::Ok);
    err = sub->read_block(1, 0, output.data(), static_cast<std::uint32_t>(output.size()), holoram::HoloQualityMode::Exact);
    assert(err == holoram::HoloError::Ok);
    assert(input == output);
    return 0;
}
