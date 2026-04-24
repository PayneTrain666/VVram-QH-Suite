#pragma once

#include <cstdint>

namespace holoram {

enum class HoloError : std::uint32_t {
    Ok = 0,
    InvalidArgument,
    UnsupportedMode,
    AllocationFailed,
    DecodeFailed,
    EncodeFailed,
    GenerationMismatch,
    IntegrityCheckFailed,
    SlotFull,
    SlotNotFound,
    NotImplemented
};

inline const char* to_string(HoloError err) {
    switch (err) {
        case HoloError::Ok: return "Ok";
        case HoloError::InvalidArgument: return "InvalidArgument";
        case HoloError::UnsupportedMode: return "UnsupportedMode";
        case HoloError::AllocationFailed: return "AllocationFailed";
        case HoloError::DecodeFailed: return "DecodeFailed";
        case HoloError::EncodeFailed: return "EncodeFailed";
        case HoloError::GenerationMismatch: return "GenerationMismatch";
        case HoloError::IntegrityCheckFailed: return "IntegrityCheckFailed";
        case HoloError::SlotFull: return "SlotFull";
        case HoloError::SlotNotFound: return "SlotNotFound";
        case HoloError::NotImplemented: return "NotImplemented";
        default: return "Unknown";
    }
}

} // namespace holoram
