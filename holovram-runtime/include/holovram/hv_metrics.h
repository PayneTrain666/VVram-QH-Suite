#pragma once

#include "holovram/hv_residency.h"

namespace holovram {

struct FrameMetrics { double frame_ms = 0.0; double decode_ms = 0.0; double upload_ms = 0.0; ResidencyStats residency{}; };

} // namespace holovram
