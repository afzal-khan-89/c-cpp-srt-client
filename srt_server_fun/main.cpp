#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <string>
#include <srt/srt.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

// Live555 headers
#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "CustomH264Subsession.hh"
#include "CustomAACSubsession.hh"

constexpr unsigned char TAG_HEADER = 0xE0;
constexpr unsigned char TAG_AAC    = 0xE1;
constexpr unsigned char TAG_H264   = 0xE2;

std::atomic<bool> running(true);
std::mutex clients_mutex;

struct ClientContext {
    int srtSocket;
    std::string clientIP;
    std::thread workerThread;
};

std::vector<ClientContext*> clients;

void registerLive555Stream(RTSPServer* rtspServer, UsageEnvironment* env,
                           const std::string& streamName,
                           const std::string& h264File, const std::string& aacFile) {
    ServerMediaSession* sms = ServerMediaSession::createNew(*env, streamName.c_str(),
                                                            streamName.c_str(),
                                                            "AAC + H264 live growing file stream");
    sms->addSubsession(CustomH264Subsession::createNew(*env, h264File.c_str(), True));
    sms->addSubsession(CustomAACSubsession::createNew(*env, aacFile.c_str(), True));
    rtspServer->addServerMediaSession(sms);

    std::string url = rtspServer->rtspURL(sms);
    std::cout << "Stream ready: " << url << std::endl;
}

void clientHandler(int srtSocket, const std::string& clientIP, RTSPServer* rtspServer, UsageEnvironment* env) {
    char buffer[1316];
    std::string aacFile = clientIP + ".aac";
    std::string h264File = clientIP + ".h264";

    FILE* aac = fopen(aacFile.c_str(), "wb");
    FILE* h264 = fopen(h264File.c_str(), "wb");
    if (!aac || !h264) {
        std::cerr << "Failed to open output files for client " << clientIP << std::endl;
        return;
    }

    std::string streamName = "cam_" + clientIP;

    env->taskScheduler().scheduleDelayedTask(0,
        [](void* data) {
            auto* args = static_cast<std::tuple<RTSPServer*, UsageEnvironment*, std::string, std::string, std::string>*>(data);
            registerLive555Stream(std::get<0>(*args), std::get<1>(*args), std::get<2>(*args),
                                  std::get<3>(*args), std::get<4>(*args));
            delete args;
        },
        new std::tuple<RTSPServer*, UsageEnvironment*, std::string, std::string, std::string>(
            rtspServer, env, streamName, h264File, aacFile));

    std::cout << "[+] Receiving from SRT IP Camera: " << clientIP << std::endl;

    int bytesRead = srt_recvmsg(srtSocket, buffer, 1316);
    if (bytesRead > 0 && static_cast<unsigned char>(buffer[0]) != TAG_HEADER) {
        std::cout << "[+]rcv invalid command pkt ... ...\n";
        goto cleanup;
    }

    while (running.load()) {
        int bytesRead = srt_recvmsg(srtSocket, buffer, 1316);
        if (bytesRead > 0) {
            switch (static_cast<unsigned char>(buffer[0])) {
                case TAG_AAC:
                    fwrite(buffer + 1, 1, bytesRead - 1, aac);
                    fflush(aac);
                    break;
                case TAG_H264:
                    fwrite(buffer + 1, 1, bytesRead - 1, h264);
                    fflush(h264);
                    break;
                default:
                    std::cout << "Invalid tag: 0x" << std::hex << (int)(unsigned char)buffer[0] << std::dec << std::endl;
                    break;
            }
        } else {
            goto cleanup;
        }
    }

cleanup:
    fclose(aac);
    fclose(h264);
    srt_close(srtSocket);
    std::cout << "[-] SRT client " << clientIP << " disconnected.\n";
}

int main() {
    srt_startup();
    OutPacketBuffer::maxSize = 330000;  // Avoid frame truncation

    int srtServer = srt_create_socket();
    sockaddr_in sa{};
    sa.sin_family = AF_INET;
    sa.sin_port = htons(9000);
    sa.sin_addr.s_addr = INADDR_ANY;
    srt_bind(srtServer, (sockaddr*)&sa, sizeof sa);
    srt_listen(srtServer, 5);

    TaskScheduler* scheduler = BasicTaskScheduler::createNew();
    UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);
    RTSPServer* rtspServer = RTSPServer::createNew(*env, 8554);
    if (!rtspServer) {
        std::cerr << "Failed to create RTSP server: " << env->getResultMsg() << "\n";
        return 1;
    }

    std::thread live555Thread([&]() {
        env->taskScheduler().doEventLoop(); // blocks
    });

    std::cout << "[+] SRT server listening on port 9000\n";

    while (running.load()) {
        sockaddr_in clientAddr{};
        int addrLen = sizeof clientAddr;
        int clientSock = srt_accept(srtServer, (sockaddr*)&clientAddr, &addrLen);
        if (clientSock == SRT_INVALID_SOCK) continue;

        char ipStr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, ipStr, sizeof(ipStr));

        auto ctx = new ClientContext{clientSock, ipStr};
        ctx->workerThread = std::thread(clientHandler, clientSock, std::string(ipStr), rtspServer, env);

        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients.push_back(ctx);
        }
    }

    for (auto ctx : clients) {
        if (ctx->workerThread.joinable()) ctx->workerThread.join();
        delete ctx;
    }

    srt_close(srtServer);
    srt_cleanup();
    return 0;
}

