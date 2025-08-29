#ifndef AUDIO_DEBUGGER_H
#define AUDIO_DEBUGGER_H

#include <vector>
#include <cstdint>

#include <sys/socket.h>
#include <netinet/in.h>
#ifdef CONFIG_CONNECTION_TYPE_NERTC
#include <web_socket.h>
#endif

class AudioDebugger {
public:
    AudioDebugger();
    ~AudioDebugger();

    void Feed(const std::vector<int16_t>& data);

#ifdef CONFIG_CONNECTION_TYPE_NERTC
    void SendAudioInfo(int sample_rate, int channels);
#endif

private:
    int udp_sockfd_ = -1;
    struct sockaddr_in udp_server_addr_;
#ifdef CONFIG_CONNECTION_TYPE_NERTC
    WebSocket* websocket_ = nullptr;
#endif
};

#endif 