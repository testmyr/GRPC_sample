cmake_minimum_required(VERSION 3.16)
project(UserServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Proto file
set(PROTO_FILE user.proto)

# Paths
set(GRPC_BUILD_DIR ${CMAKE_SOURCE_DIR}/grpc)
set(GRPC_DIR ${CMAKE_SOURCE_DIR}/grpc)
set(GRPC_BUILD_DIR ${GRPC_DIR}/build)
set(PROTOC ${GRPC_BUILD_DIR}/third_party/protobuf/protoc)
set(GRPC_CPP_PLUGIN ${GRPC_BUILD_DIR}/grpc_cpp_plugin)

set(ABSEIL_INCLUDE_DIR ${GRPC_DIR}/third_party/abseil-cpp/build/install/include)
set(ABSEIL_LIB_DIR ${GRPC_DIR}/third_party/abseil-cpp/build/install/lib)
set(GRPC_LIB_DIR ${GRPC_BUILD_DIR})
set(PROTOBUF_INCLUDE_DIR ${GRPC_DIR}/third_party/protobuf/src)

# Include directories
include_directories(
    ${GRPC_DIR}/include
    ${PROTOBUF_INCLUDE_DIR}
    ${ABSEIL_INCLUDE_DIR}
)

# Generate gRPC + Protobuf sources
set(GENERATED_PROTO_SRC ${CMAKE_SOURCE_DIR}/user.pb.cc)
set(GENERATED_PROTO_HDR ${CMAKE_SOURCE_DIR}/user.pb.h)
set(GENERATED_GRPC_SRC ${CMAKE_SOURCE_DIR}/user.grpc.pb.cc)
set(GENERATED_GRPC_HDR ${CMAKE_SOURCE_DIR}/user.grpc.pb.h)

add_custom_command(
    OUTPUT ${GENERATED_PROTO_SRC} ${GENERATED_PROTO_HDR}
    COMMAND ${PROTOC} -I ${CMAKE_SOURCE_DIR} --cpp_out=${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/${PROTO_FILE}
    DEPENDS ${PROTO_FILE}
)

add_custom_command(
    OUTPUT ${GENERATED_GRPC_SRC} ${GENERATED_GRPC_HDR}
    COMMAND ${PROTOC} -I ${CMAKE_SOURCE_DIR} --grpc_out=${CMAKE_SOURCE_DIR} --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN} ${CMAKE_SOURCE_DIR}/${PROTO_FILE}
    DEPENDS ${PROTO_FILE}
)

add_custom_target(gen_proto ALL DEPENDS
    ${GENERATED_PROTO_SRC} ${GENERATED_PROTO_HDR}
    ${GENERATED_GRPC_SRC} ${GENERATED_GRPC_HDR}
)

# Source files
add_executable(server
    ${GENERATED_PROTO_SRC}
    ${GENERATED_GRPC_SRC}
    user_server.cpp
)

add_dependencies(server gen_proto)

# Link directories
link_directories(
    ${GRPC_LIB_DIR}
    ${GRPC_LIB_DIR}/third_party/protobuf
    ${GRPC_LIB_DIR}/third_party/protobuf/third_party/utf8_range/
    ${GRPC_DIR}/third_party/abseil-cpp/build/install/lib
    ${GRPC_LIB_DIR}/third_party/cares/cares/lib
    ${GRPC_LIB_DIR}/third_party/zlib
)

# Gather static libs from Abseil and gRPC
file(GLOB_RECURSE ABSEIL_LIBS "${ABSEIL_LIB_DIR}/libabsl*.a")
file(GLOB_RECURSE GRPC_LIBS "${GRPC_LIB_DIR}/lib*.a")

add_subdirectory(grpc/third_party/abseil-cpp absl_build)
target_link_libraries(server
    ${GRPC_BUILD_DIR}/libgrpc++.a
    ${GRPC_BUILD_DIR}/libgrpc.a
    ${GRPC_LIBS}
	${GRPC_BUILD_DIR}/third_party/protobuf/libprotobuf.a
    ${GRPC_BUILD_DIR}/third_party/protobuf/third_party/utf8_range/libutf8_validity.a
    absl::status
    absl::statusor
    absl::core_headers
    absl::strings
    absl::time
    absl::flags
    absl::flags_internal
    absl::flags_commandlineflag
    absl::flags_commandlineflag_internal
    absl::flags_reflection
    absl::flags_marshalling
    absl::flags_parse
    absl::log
    absl::log_internal_check_op
    absl::log_internal_conditions
    absl::log_internal_globals
    absl::log_internal_message
    absl::log_internal_format
    absl::log_internal_log_sink_set
    absl::random_random
    absl::random_distributions
    absl::random_internal_randen
    absl::random_internal_platform
    absl::random_internal_pool_urbg
    absl::random_internal_seed_material
    absl::crc32c
)

# Optional: static linking
set_target_properties(server PROPERTIES
    LINK_SEARCH_START_STATIC ON
    LINK_SEARCH_END_STATIC ON
)

