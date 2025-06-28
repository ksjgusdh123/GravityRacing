#pragma once
class FSession
{
public:
    void OnRecv(const char* data, int32_t len);
    void Send(const char* data, int32_t len);
    void Disconnect();
};