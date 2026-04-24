#include "holovram/hv_context.h"
#include <iostream>

int main() {
    auto ctx = holovram::create_context({});
    holovram::ResourceDesc desc{};
    desc.kind = holovram::ResourceKind::Texture2D;
    desc.width = 16384;
    desc.height = 16384;
    desc.mips = 12;
    auto res = ctx->create_resource(desc);
    std::cout << "Created virtual texture resource id=" << res.id << "\n";
    std::cout << "TODO(v0.1): simulate moving camera and tile residency\n";
    return 0;
}
