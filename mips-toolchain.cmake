set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR mips)

set(CROSS_PREFIX /home/afzal/ingenic/Ingenic-T23-SDK/T23-PIKE/T23_PIKE_20240325/SDK_20240204/ISVP-T23-1.1.2-20240204/Ingenic-SDK-T23-1.1.2-20240204-en/resource/toolchain/gcc_540/mips-gcc540-glibc222-64bit-r3.3.0.smaller/bin/mips-linux-gnu)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSRT_SYNC_ATOMIC")
set(CMAKE_C_COMPILER ${CROSS_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${CROSS_PREFIX}-g++)
set(CMAKE_AR ${CROSS_PREFIX}-ar)
set(CMAKE_RANLIB ${CROSS_PREFIX}-ranlib)
set(CMAKE_LINKER ${CROSS_PREFIX}-ld)
set(CMAKE_STRIP ${CROSS_PREFIX}-strip)

set(CMAKE_FIND_ROOT_PATH /home/afzal/ingenic/work-space/ingenic-app/Ingenic-SDK-T23-1.1.2-20240204-en/resource/toolchain/gcc_540/mips-gcc540-glibc222-64bit-r3.3.0.smaller/mips-linux-gnu/libc)
set(CMAKE_SYSROOT /home/afzal/ingenic/work-space/ingenic-app/Ingenic-SDK-T23-1.1.2-20240204-en/resource/toolchain/gcc_540/mips-gcc540-glibc222-64bit-r3.3.0.smaller/mips-linux-gnu/libc)

set(OPENSSL_ROOT_DIR /home/afzal/ingenic/work-space/ingenic-app/apps/srt_ingenic/_openssl/open_ssl_static_lib)

