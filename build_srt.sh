INSTALL_DIR=$PWD/install_static_srt
mkdir -p $INSTALL_DIR
if [ ! -d "$INSTALL_DIR" ]; then
  mkdir $INSTALL_DIR
else
  rm -rf $INSTALL_DIR/*	  
fi

OPEN_SSL_DIR=/home/afzal/ingenic/work-space/ingenic-app/apps/srt_ingenic/_openssl/open_ssl_static_lib

#/home/afzal/ingenic/work-space/ingenic-app/apps/keo-cam/lib/thirdparty-lib/openssl/openssl-1.1.1w/install-mips
#export TOOL_CHAIN_DIR=/home/afzal/ingenic/Ingenic-T23-SDK/T23-PIKE/T23_PIKE_20240325/SDK_20240204/ISVP-T23-1.1.2-20240204/Ingenic-SDK-T23-1.1.2-20240204-en/resource/toolchain/gcc_540/mips-gcc540-glibc222-64bit-r3.3.0.smaller
export PATH=$PATH:$OPEN_SSL_DIR      
export CXXFLAGS="-DSRT_SYNC_ATOMIC"

if [ ! -d "srt" ]; then
  git clone https://github.com/Haivision/srt.git
fi

cd srt
if [ ! -d "build" ]; then
  mkdir build
fi
cd build

cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=/home/afzal/ingenic/work-space/ingenic-app/apps/srt_ingenic/mips-toolchain.cmake \
  -DBUILD_SHARED_LIBS=OFF \
  -DENABLE_STATIC=ON \
  -DENABLE_SHARED=OFF \
  -DENABLE_APPS=OFF \
  -DENABLE_CXX11=ON \
  -DCMAKE_CXX_FLAGS="-DSRT_SYNC_ATOMIC" \
  -DENABLE_LOGGING=OFF \
  -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
  -DSRT_USE_OPENSSL_STATIC_LIBS=ON \
  -DCMAKE_PREFIX_PATH=$OPEN_SSL_DIR \
  -DOPENSSL_ROOT_DIR=$OPEN_SSL_DIR \
  -DOPENSSL_INCLUDE_DIR=$OPEN_SSL_DIR/include \
  -DOPENSSL_CRYPTO_LIBRARY=$OPEN_SSL_DIR/lib/libcrypto.a \
  -DOPENSSL_SSL_LIBRARY=$OPEN_SSL_DIR/lib/libssl.a \
  -DSRT_USE_OPENSSL_STATIC_LIBS=ON \
  -DUSE_ENCLIB=openssl-evp \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_EXE_LINKER_FLAGS="-latomic" \
  -DCMAKE_SHARED_LINKER_FLAGS="-latomic"

  
make -j$(nproc)
make install
#  -DOPENSSL_USE_STATIC_LIBS=ON \

