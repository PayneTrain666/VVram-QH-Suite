#include "holovram/hv_prefetch.h"
#include <cassert>

int main() {
    holovram::PrefetchScheduler sched;
    holovram::TileRequest req{};
    req.resource_id = 1;
    req.target_frame = 10;
    sched.submit(req);
    auto early = sched.drain_ready(9);
    assert(early.empty());
    auto ready = sched.drain_ready(10);
    assert(ready.size() == 1);
    return 0;
}
