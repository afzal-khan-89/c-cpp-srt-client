#! /bin/sh


export TOOL_CHAIN_DIR=/home/afzal/ingenic/Ingenic-T23-SDK/T23-PIKE/T23_PIKE_20240325/SDK_20240204/ISVP-T23-1.1.2-20240204/Ingenic-SDK-T23-1.1.2-20240204-en/resource/toolchain/gcc_540/mips-gcc540-glibc222-64bit-r3.3.0.smaller

PROJ_DIR=$PWD
OPEN_SSL_SRC_DIR=_openssl
OPEN_SSL_INSTALL_DIR=$PROJ_DIR/$OPEN_SSL_SRC_DIR/open_ssl_static_lib

if [ ! -d "$OPEN_SSL_SRC_DIR" ]; then 
	mkdir  $OPEN_SSL_SRC_DIR 
fi

cd $OPEN_SSL_SRC_DIR
if [ -d "open_ssl_static_lib" ]; then
	rm -rf open_ssl_static_lib 
fi
mkdir open_ssl_static_lib



#echo "using --cross-compile-prefix   and path variable"

#export CROSS_COMPILE=mips-linux-gnu-
#export PATH=$TOOL_CHAIN_DIR/bin:$PATH

#if [ -f "openssl-1.1.1w.tar.gz" ]; then
# 	rm openssl-1.1.1w.tar.gz 
#fi

#wget https://www.openssl.org/source/openssl-1.1.1w.tar.gz
#tar xvf openssl-1.1.1w.tar.gz
#cd openssl-1.1.1w  

#./Configure linux-generic32 \
#    --prefix=$OPEN_SSL_INSTALL_DIR \
#    --openssldir=$PWD/install-mips no-shared no-async \
#    --cross-compile-prefix=$CROSS_COMPILE

#make -j$(nproc)
#make install_sw


 
 
 
 
 
 
 
 
echo "using CROSS_PREFIX "


export CROSS_PREFIX=$TOOL_CHAIN_DIR/bin/mips-linux-gnu-
if [ -f "openssl-1.1.1w.tar.gz" ]; then
 	rm openssl-1.1.1w.tar.gz 
fi


wget https://www.openssl.org/source/openssl-1.1.1w.tar.gz
tar xvf openssl-1.1.1w.tar.gz
cd openssl-1.1.1w  

./Configure linux-generic32 \
    --prefix=$OPEN_SSL_INSTALL_DIR \
    --openssldir=$PWD/install-mips no-shared no-async


make -j$(nproc)
make install_sw

echo "open ssl build ok .... .... ...."
 


cd $PROJ_DIR 

export OPEN_SSL_DIR=$OPEN_SSL_INSTALL_DIR
export PATH=$PATH:$OPEN_SSL_DIR    

SRT_SRC_DIR=_srt
SRT_INSTALL_DIR=$PROJ_DIR/$SRT_SRC_DIR/srt_static_lib


if [  -d "$SRT_SRC_DIR" ]; then 
	rm -rf $SRT_SRC_DIR
fi
mkdir $SRT_SRC_DIR

cd $SRT_SRC_DIR

if [ -d "srt_static_lib" ]; then
	rm -rf srt_static_lib
fi

mkdir srt_static_lib


  
if [ ! -d "srt" ]; then
  git clone https://github.com/Haivision/srt.git
fi

cd srt
if [ -d "build" ]; then
  rm -rf build
fi
mkdir build 
cd build
 
export CXXFLAGS="-DSRT_SYNC_ATOMIC" 

cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=/home/afzal/ingenic/work-space/ingenic-app/apps/srt_ingenic/mips-toolchain.cmake \
  -DBUILD_SHARED_LIBS=OFF \
  -DENABLE_STATIC=ON \
  -DENABLE_SHARED=OFF \
  -DENABLE_APPS=OFF \
  -DENABLE_CXX11=ON \
  -DCMAKE_CXX_FLAGS="-DSRT_SYNC_ATOMIC" \
  -DENABLE_LOGGING=OFF \
  -DCMAKE_INSTALL_PREFIX=$SRT_INSTALL_DIR \
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












