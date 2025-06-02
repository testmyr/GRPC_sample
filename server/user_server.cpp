// user_server.cpp
#include <endian.h>
#include <byteswap.h>

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "user.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using user::UserRequest;
using user::UserResponse;
using user::UserService;

// Service implementation
class UserServiceImpl final : public UserService::Service {
  Status GetUser(ServerContext* context, const UserRequest* request,
                 UserResponse* reply) override {
    int user_id = request->id();
    std::cout << "Received request for user ID: " << user_id << std::endl;

    // Dummy data for demonstration
    if (user_id == 1) {
      reply->set_name("Alice Smith");
      reply->set_email("alice@example.com");
    } else {
      reply->set_name("Unknown");
      reply->set_email("unknown@example.com");
    }

    return Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  UserServiceImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();
  return 0;
}

