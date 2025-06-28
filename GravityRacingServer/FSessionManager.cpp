#include "PCH.h"
#include "FSessionManager.h"

void FSessionManager::AddSession(int32_t sessionId, std::shared_ptr<FSession> session)
{
    _sessions[sessionId] = session;
}

void FSessionManager::RemoveSession(int32_t sessionId)
{
    _sessions.erase(sessionId);
}

std::shared_ptr<FSession> FSessionManager::GetSession(int32_t sessionId)
{
    auto it = _sessions.find(sessionId);
    return (it != _sessions.end()) ? it->second : nullptr;
}