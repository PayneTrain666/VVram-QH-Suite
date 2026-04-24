#include "holoram/qh_writemap.h"
#include <cassert>

int main() {
    holoram::HoloWriteMap wm;
    holoram::HoloMapRecord rec{};
    rec.key.logical_id = 123;
    rec.key.sub_id = 4;
    rec.handle.slot_id = 9;
    rec.handle.generation = 1;
    wm.put(rec);
    auto got = wm.get(rec.key);
    assert(got.has_value());
    assert(got->handle.slot_id == 9);
    return 0;
}
