#include "queue_client.h"
#include <fcntl.h>
#include <unistd.h>

bool QueueClient::open_device(const std::string& path) {
    fd_ = ::open(path.c_str(), O_RDWR);
    return fd_ >= 0;
}

void QueueClient::close_device() {
    if (fd_ >= 0) {
        ::close(fd_);
        fd_ = -1;
    }
}
