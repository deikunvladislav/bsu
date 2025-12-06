#ifndef OS_LAB5_PIPE_HPP
#define OS_LAB5_PIPE_HPP

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <stdexcept>
#include <optional>
#include "handle.hpp"
#include "protocol.hpp"

struct IServerTransport {
    virtual ~IServerTransport() = default;
    virtual bool write_response(const Proto::Response& resp) = 0;
    virtual std::optional<Proto::Request> read_request() = 0;
    virtual void disconnect() = 0;
};

struct IClientTransport {
    virtual ~IClientTransport() = default;
    virtual bool send_request(const Proto::Request& req) = 0;
    virtual std::optional<Proto::Response> receive_response() = 0;
};

class NamedPipeServer : public IServerTransport {
public:
    void create() {
        Handle tmp(CreateNamedPipeW(
            Proto::PIPE_NAME,
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            Proto::MAX_CLIENTS,
            Proto::PIPE_OUT_BUFFER,
            Proto::PIPE_IN_BUFFER,
            Proto::PIPE_TIMEOUT_MS,
            nullptr
        ));
        if (!tmp.valid()) throw std::runtime_error("CreateNamedPipeW failed");
        pipe_ = std::move(tmp);
    }

    void wait_for_client() {
        if (!pipe_.valid()) throw std::runtime_error("Pipe not created");
        const BOOL ok = ConnectNamedPipe(pipe_.get(), nullptr) ? TRUE :
            (GetLastError() == ERROR_PIPE_CONNECTED ? TRUE : FALSE);
        if (!ok) throw std::runtime_error("ConnectNamedPipe failed");
    }

    bool write_response(const Proto::Response& resp) override {
        DWORD written = 0;
        const BOOL ok = WriteFile(pipe_.get(), &resp, sizeof(Proto::Response), &written, nullptr);
        return ok && written == sizeof(Proto::Response);
    }

    std::optional<Proto::Request> read_request() override {
        DWORD read = 0;
        Proto::Request req{};
        const BOOL ok = ReadFile(pipe_.get(), &req, sizeof(Proto::Request), &read, nullptr);
        if (!ok || read != sizeof(Proto::Request)) return std::nullopt;
        return req;
    }

    void disconnect() override {
        if (pipe_.valid()) {
            DisconnectNamedPipe(pipe_.get());
        }
    }

private:
    Handle pipe_;
};

class NamedPipeClient : public IClientTransport {
public:
    bool try_connect() {
        const BOOL okWait = WaitNamedPipeW(Proto::PIPE_NAME, Proto::PIPE_TIMEOUT_MS);
        if (!okWait) return false;

        Handle tmp(CreateFileW(
            Proto::PIPE_NAME,
            GENERIC_READ | GENERIC_WRITE,
            0,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        ));
        if (!tmp.valid()) return false;

        pipe_ = std::move(tmp);
        DWORD mode = PIPE_READMODE_MESSAGE;
        SetNamedPipeHandleState(pipe_.get(), &mode, nullptr, nullptr);
        return true;
    }

    bool send_request(const Proto::Request& req) override {
        DWORD written = 0;
        const BOOL ok = WriteFile(pipe_.get(), &req, sizeof(Proto::Request), &written, nullptr);
        return ok && written == sizeof(Proto::Request);
    }

    std::optional<Proto::Response> receive_response() override {
        DWORD read = 0;
        Proto::Response resp{};
        const BOOL ok = ReadFile(pipe_.get(), &resp, sizeof(Proto::Response), &read, nullptr);
        if (!ok || read != sizeof(Proto::Response)) return std::nullopt;
        return resp;
    }

private:
    Handle pipe_;
};

#endif