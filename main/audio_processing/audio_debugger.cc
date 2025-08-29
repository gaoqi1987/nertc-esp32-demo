#include "audio_debugger.h"
#include "sdkconfig.h"

#if CONFIG_USE_AUDIO_DEBUGGER
#include <esp_log.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <string>
#endif
#ifdef CONFIG_CONNECTION_TYPE_NERTC
#include "settings.h"
#include "board.h"
#include <esp_log.h>
#endif

#define TAG "AudioDebugger"


AudioDebugger::AudioDebugger() {
#if CONFIG_USE_AUDIO_DEBUGGER
    udp_sockfd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_sockfd_ >= 0) {
        // 解析配置的服务器地址 "IP:PORT"
        std::string server_addr = CONFIG_AUDIO_DEBUG_UDP_SERVER;
        size_t colon_pos = server_addr.find(':');
        
        if (colon_pos != std::string::npos) {
            std::string ip = server_addr.substr(0, colon_pos);
            int port = std::stoi(server_addr.substr(colon_pos + 1));
            
            memset(&udp_server_addr_, 0, sizeof(udp_server_addr_));
            udp_server_addr_.sin_family = AF_INET;
            udp_server_addr_.sin_port = htons(port);
            inet_pton(AF_INET, ip.c_str(), &udp_server_addr_.sin_addr);
            
            ESP_LOGI(TAG, "Initialized server address: %s", CONFIG_AUDIO_DEBUG_UDP_SERVER);
        } else {
            ESP_LOGW(TAG, "Invalid server address: %s, should be IP:PORT", CONFIG_AUDIO_DEBUG_UDP_SERVER);
            close(udp_sockfd_);
            udp_sockfd_ = -1;
        }
    } else {
        ESP_LOGW(TAG, "Failed to create UDP socket: %d", errno);
    }
#endif

#ifdef CONFIG_CONNECTION_TYPE_NERTC
    std::string nertc_url = CONFIG_USE_NERTC_AUDIO_DEBUGGER_WS_URL;
    if (!nertc_url.empty()) {
        Settings settings("websocket", true);
        settings.SetString("url", nertc_url);
        websocket_ = Board::GetInstance().CreateWebSocket();
        if (!websocket_->Connect(nertc_url.c_str())) {
            return;
        }
        ESP_LOGI(TAG, "Initialized ws server url: %s", nertc_url.c_str());
    }
#endif
}

AudioDebugger::~AudioDebugger() {
#if CONFIG_USE_AUDIO_DEBUGGER
    if (udp_sockfd_ >= 0) {
        close(udp_sockfd_);
        ESP_LOGI(TAG, "Closed UDP socket");
    }
#endif

#ifdef CONFIG_CONNECTION_TYPE_NERTC
    if (websocket_ != nullptr) {
        delete websocket_;
        websocket_ = nullptr;
    }
#endif
}

void AudioDebugger::Feed(const std::vector<int16_t>& data) {
#if CONFIG_USE_AUDIO_DEBUGGER
    if (udp_sockfd_ >= 0) {
        ssize_t sent = sendto(udp_sockfd_, data.data(), data.size() * sizeof(int16_t), 0,
                             (struct sockaddr*)&udp_server_addr_, sizeof(udp_server_addr_));
        if (sent < 0) {
            ESP_LOGW(TAG, "Failed to send audio data to %s: %d", CONFIG_AUDIO_DEBUG_UDP_SERVER, errno);
        } else {
            ESP_LOGD(TAG, "Sent %d bytes audio data to %s", sent, CONFIG_AUDIO_DEBUG_UDP_SERVER);
        }
    }
#endif

#ifdef CONFIG_CONNECTION_TYPE_NERTC
    if (websocket_ && websocket_->IsConnected()) {
        std::vector<uint8_t> byte_data;
        byte_data.resize(data.size() * sizeof(int16_t));
        memcpy(byte_data.data(), data.data(), byte_data.size());
        websocket_->Send(byte_data.data(), byte_data.size(), true);
    }
#endif
}

#ifdef CONFIG_CONNECTION_TYPE_NERTC
void AudioDebugger::SendAudioInfo(int sample_rate, int channels) {
    if (websocket_ && websocket_->IsConnected()) {
        std::string json = "{\"sample_rate\":\"" + std::to_string(sample_rate) + 
                        "\",\"channels\":\"" + std::to_string(channels) + "\"}";

        websocket_->Send(json);
    }
}
#endif