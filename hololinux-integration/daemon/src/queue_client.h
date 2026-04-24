#pragma once

#include <string>

class QueueClient {
public:
    bool open_device(const std::string& path = "/dev/holoram_kq");
    void close_device();
private:
    int fd_ = -1;
};
