/**
 * @file CustomStatus.h
 * @brief �Զ���grpc status
 * @author װ����ҵ������� ���� 
 * @version 0.1
 * @date 2020-09-08
 * 
 * @copyright Copyright (c) 2020  �й����ӿƼ����Ź�˾����ʮһ�о���
 * 
 */

#pragma once
#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/sync_stream.h>

const grpc::Status NODE_NOT_FOUND(grpc::NOT_FOUND, "node not found");
const grpc::Status TASK_NOT_FOUND(grpc::NOT_FOUND, "task not found");
const grpc::Status API_NOT_IMPLEMENTED(grpc::UNIMPLEMENTED, "this API is unimplemented");
const grpc::Status NO_AVAILABLE_DEVICE(grpc::RESOURCE_EXHAUSTED, "no available device");
const grpc::Status INVALID_PARAM(grpc::INVALID_ARGUMENT, "invalid argument");
const grpc::Status USER_ABORT(grpc::ABORTED, "user aborted");
const grpc::Status REQUEST_CANCEL(grpc::INTERNAL, "task can not send request");
const grpc::Status NO_RESOURCE(grpc::NOT_FOUND, "resource not found");
const grpc::Status UNAVAILABLE(grpc::UNAVAILABLE, "unavailable");