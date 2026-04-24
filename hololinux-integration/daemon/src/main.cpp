#include "queue_client.h"
#include <iostream>

int main() {
    QueueClient client;
    if (!client.open_device()) {
        std::cerr << "Failed to open /dev/holoram_kq\n";
        return 1;
    }
    std::cout << "holoram daemon started\n";
    std::cout << "TODO(v0.1): process queue jobs\n";
    client.close_device();
    return 0;
}
