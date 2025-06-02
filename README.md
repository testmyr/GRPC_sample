# GRPC_sample

This is a step-by-step guide to building gRPC from source on Linux without installing it system-wide.

1. Go to the server directory
   ```
    cd GRPC_sample/server
   ```
3. Clone gRPC source code
   ```
   git clone --recurse-submodules -b v1.60.0 https://github.com/grpc/grpc
   ```
5. Build Abseil with proper export settings:
   ```
    cd grpc/third_party/abseil-cpp
    rm -rf build
    mkdir build
    cd build
    
    # Configure with all necessary flags for proper exports
    cmake .. \
      -DCMAKE_INSTALL_PREFIX=$(pwd)/install \
      -DABSL_PROPAGATE_CXX_STD=ON \
      -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
      -DABSL_ENABLE_INSTALL=ON \
      -DCMAKE_BUILD_TYPE=Release \
      -DABSL_USE_EXTERNAL_GOOGLETEST=OFF \
      -DABSL_FIND_GOOGLETEST=OFF
      -DCMAKE_CXX_STANDARD=17
    
    make -j$(nproc)
    make install
    ```
6. Build gRPC
   ```
    cd 'your path'/GRPC_sample/server/grpc
    rm -rf build
    mkdir build
    cd build
    
    # Use CMAKE_PREFIX_PATH instead of ABSL_ROOT_DIR
    cmake .. \
      -DgRPC_INSTALL=ON \
      -DgRPC_BUILD_TESTS=OFF \
      -DCMAKE_INSTALL_PREFIX=$(pwd)/install \
      -DCMAKE_PREFIX_PATH=$(pwd)/../third_party/abseil-cpp/build/install \
      -DgRPC_ABSL_PROVIDER=package \
      -DABSL_PROPAGATE_CXX_STD=ON \
      -DCMAKE_CXX_STANDARD=17 \
      -DCMAKE_CXX_STANDARD_REQUIRED=ON
      -DgRPC_BUILD_SHARED_LIBS=OFF \
      -DProtobuf_USE_STATIC_LIBS=ON \
      -DZLIB_USE_STATIC_LIBS=ON \
      -DCARES_STATIC=ON \
      -DOPENSSL_USE_STATIC_LIBS=TRUE
    
    make -j$(nproc)
   ```
7. Finally build the gRPC server sample written in C++:
   ```
    cd 'your path'/GRPC_sample/server
    rm -rf build
    mkdir build
    cd build
    cmake ..
    make -j$(nproc)
   ```
8. Now you can launch it:
   ```
    ./server
   ```
Then any client, including the attached Android project, can be used to connect to the server.





