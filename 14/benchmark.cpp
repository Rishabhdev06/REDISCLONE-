#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <chrono>
#include <string>
#include <vector>

static void die(const char *msg) {
    int err = errno;
    fprintf(stderr, "[%d] %s\n", err, msg);
    abort();
}

static int32_t write_all(int fd, const char *buf, size_t n) {
    while (n > 0) {
        ssize_t rv = write(fd, buf, n);
        if (rv <= 0) return -1;
        n -= (size_t)rv;
        buf += rv;
    }
    return 0;
}

static int32_t read_full(int fd, char *buf, size_t n) {
    while (n > 0) {
        ssize_t rv = read(fd, buf, n);
        if (rv <= 0) return -1;
        n -= (size_t)rv;
        buf += rv;
    }
    return 0;
}

const size_t k_max_msg = 4096;

// Reusing your binary protocol serialization
static int32_t send_req(int fd, const std::vector<std::string> &cmd) {
    uint32_t len = 4;
    for (const std::string &s : cmd) {
        len += 4 + s.size();
    }
    if (len > k_max_msg) return -1;

    char wbuf[4 + k_max_msg];
    memcpy(&wbuf[0], &len, 4);
    uint32_t n = (uint32_t)cmd.size();
    memcpy(&wbuf[4], &n, 4);
    size_t cur = 8;
    for (const std::string &s : cmd) {
        uint32_t p = (uint32_t)s.size();
        memcpy(&wbuf[cur], &p, 4);
        memcpy(&wbuf[cur + 4], s.data(), s.size());
        cur += 4 + s.size();
    }
    return write_all(fd, wbuf, 4 + len);
}

// Reading the response, but ignoring the payload to maximize speed
static int32_t read_res(int fd) {
    char rbuf[4 + k_max_msg];
    if (read_full(fd, rbuf, 4)) return -1; // read header
    uint32_t len = 0;
    memcpy(&len, rbuf, 4);
    if (len > k_max_msg) return -1;
    if (read_full(fd, &rbuf[4], len)) return -1; // read body
    return 0; 
}

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) die("socket()");

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(6379); // Connecting to your server
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);
    if (connect(fd, (const struct sockaddr *)&addr, sizeof(addr))) {
        die("connect");
    }

    int num_requests = 100000;
    std::vector<std::string> cmd = {"set", "benchmark_key", "benchmark_value"};

    printf("Starting benchmark... sending %d requests.\n", num_requests);
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < num_requests; ++i) {
        if (send_req(fd, cmd) != 0) die("send error");
        if (read_res(fd) != 0) die("read error");
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    
    printf("Completed in %.2f seconds.\n", diff.count());
    printf("Throughput: %.2f Operations/Second (OPS)\n", num_requests / diff.count());

    close(fd);
    return 0;
}