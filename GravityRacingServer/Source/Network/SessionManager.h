#pragma once
class FSession;

class FSessionManager
{
public:
    void AddSession(int32_t sessionId, std::shared_ptr<FSession> session);
    void RemoveSession(int32_t sessionId);
    std::shared_ptr<FSession> GetSession(int32_t sessionId);

private:
    std::unordered_map<int32_t, std::shared_ptr<FSession>> _sessions;
};