#include "radio_nrf24l01.h"

namespace RobotRemoteController::Hal
{

RadioNrf24L01::RadioNrf24L01(uint8_t chip_enable_gpio, SpiOption spi_option, uint64_t radio_pipe_address, std::chrono::milliseconds radio_rx_timeout)
: m_radio_pipe_address(radio_pipe_address)
, m_radio_rx_timeout(radio_rx_timeout) 
, m_transport_manager(MTP32::TransportManager(MTP32::Role::MASTER
    , [&](MTP32::Packet tx_packet) { RequestRadioTx(tx_packet); }
    , [&]() { return RequestRadioRx(); }
    , [&](MTP32::Packet rx_packet) { HandleRxPacket(rx_packet); })
    )
{
}

void RadioNrf24L01::EnqueueTxPacket(const RobotMiddleware::Packet &packet)
{
    if(not IsRadioInitialized())
    {   
        printf("{%s}::{%s}() -> Cannot service TX request. Radio is not initialized!\n",CLASS_NAME,__func__);
        return;
    }

    MTP32::Packet tx_packet_bytes {};

    memcpy(tx_packet_bytes.data(), &packet, MTP32::MAXIMUM_PACKET_SIZE);

    m_transport_manager.EnqueuePacket(tx_packet_bytes);
}

void RadioNrf24L01::SetRxPacketCallback(RxPacketCallback callback)
{
    m_rx_packet_callback = std::move(callback);
}

void RadioNrf24L01::Run(std::chrono::steady_clock::time_point current_time)
{
    if(not IsRadioInitialized())
    {
        printf("{%s}::{%s}() -> Failed to run radio worker task. Radio is not initialized!\n",CLASS_NAME,__func__);
        return;
    }

    m_transport_manager.Run(current_time);
}

void RadioNrf24L01::HandleRxPacket(MTP32::Packet rx_packet)
{
    (void)rx_packet;
    // FIXME: Implement RX packet handler
}

std::optional<MTP32::Packet> RadioNrf24L01::RequestRadioRx()
{
    return std::optional<MTP32::Packet>();
    // FIXME: Implment RX packet polling logic
}

void RadioNrf24L01::RequestRadioTx(const MTP32::Packet &tx_packet_bytes)
{
    (void)tx_packet_bytes;
    // FIXME: Implement packet TX sender
}

} // RobotRemoteController::Hal