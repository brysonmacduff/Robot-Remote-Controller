#pragma once
#include <remote_control_protocol.h>

#include <functional>

namespace RobotRemoteController::Hal
{
class ILinkManager
{
public:
    using RxPacketCallback = std::function<void(const RobotMiddleware::Packet& packet)>;

    virtual ~ILinkManager() = default;

    virtual void EnqueueTxPacket(const RobotMiddleware::Packet& packet) = 0;
    virtual void SetRxPacketCallback(RxPacketCallback callback) = 0;
};
} // namespace RemoteRobotController::Hal