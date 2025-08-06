cmake_minimum_required(VERSION 3.10)
project(srt_server)

set(CMAKE_CXX_STANDARD 11)

set(LIVE555_DIR /home/afzal/ingenic/work-space/media-srt/live555/live)

include_directories(
    ${LIVE555_DIR}/liveMedia/include
    ${LIVE555_DIR}/groupsock/include
    ${LIVE555_DIR}/BasicUsageEnvironment/include
    ${LIVE555_DIR}/UsageEnvironment/include
)

add_executable(srt_server
       srt_serv.cpp
)

target_link_libraries(srt_server
    ${LIVE555_DIR}/liveMedia/libliveMedia.a
    ${LIVE555_DIR}/groupsock/libgroupsock.a
    ${LIVE555_DIR}/BasicUsageEnvironment/libBasicUsageEnvironment.a
    ${LIVE555_DIR}/UsageEnvironment/libUsageEnvironment.a
    srt
    ssl
    crypto
    pthread
)
