#include "holoram/qh_metrics.h"

#include <cassert>
#include <cstdint>
#include <cstring>

int main() {
    const char* s = "123456789";
    const auto crc = holoram::crc32_bytes(s, static_cast<std::uint32_t>(std::strlen(s)));
    assert(crc == 0xCBF43926u);

    const auto a = holoram::crc32_bytes("abc", 3);
    const auto b = holoram::crc32_bytes("abd", 3);
    assert(a != b);

    return 0;
}
