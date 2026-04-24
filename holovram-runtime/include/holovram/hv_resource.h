#pragma once

#include <cstdint>

namespace holovram {

using ResourceId = std::uint64_t;
using TileId = std::uint32_t;

enum class ResourceKind : std::uint16_t { Texture2D = 1, Texture3D = 2, Buffer = 3, Meshlet = 4, EmbeddingBlock = 5, ModelWeightTile = 6, VideoTile = 7, GaussianSplatChunk = 8 };
enum class HoloFormat : std::uint16_t { Unknown = 0, Rgba8 = 1, Rgba16f = 2, R32f = 3, VectorF32 = 4, CompressedOpaque = 5 };

struct ResourceDesc {
    ResourceKind kind = ResourceKind::Texture2D;
    HoloFormat format = HoloFormat::Unknown;
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    std::uint32_t depth = 1;
    std::uint32_t mips = 1;
    std::uint32_t tile_width = 128;
    std::uint32_t tile_height = 128;
    std::uint32_t tile_depth = 1;
};

struct ResourceHandle { ResourceId id = 0; };

} // namespace holovram
