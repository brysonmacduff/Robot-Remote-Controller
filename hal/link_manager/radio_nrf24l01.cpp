#include "radio_nrf24l01.h"

namespace RobotRemoteController::Hal
{

RadioNrf24l01::RadioNrf24l01(uint8_t chip_enable_gpio, SpiOption spi_option, uint64_t radio_pipe_address, std::chrono::milliseconds radio_rx_timeout)
: m_radio_pipe_address(radio_pipe_address)
, m_radio_rx_timeout(radio_rx_timeout) 
, m_transport_manager(MTP32::TransportManager(MTP32::Role::MASTER
    , [&](MTP32::Packet tx_packet) { RequestRadioTx(tx_packet); }
    , [&]() { return RequestRadioRx(); }
    , [&](MTP32::Packet rx_packet) { HandleRxPacket(rx_packet); })
    )
, m_radio(RF24(chip_enable_gpio, static_cast<uint8_t>(spi_option)))
{
}

void RadioNrf24l01::EnqueueTxPacket(const RobotMiddleware::Packet &packet)
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

void RadioNrf24l01::SetRxPacketCallback(RxPacketCallback callback)
{
    m_rx_packet_callback = std::move(callback);
}

bool RadioNrf24l01::InitializeRadio()
{
    if(IsRadioInitialized())
    {
        return true;
    }

    // Initialize the radio hardware

    if (not m_radio.begin()) 
    {
        printf("{%s}::{%s}() -> Failed to initialize radio hardware. This is a fatal error.\n",CLASS_NAME,__func__);
        return false;
    }

    // These could be made configurable but it is unclear at this time whether they should be.
    m_radio.setPALevel(RF24_PA_LOW);
    m_radio.setDataRate(RF24_1MBPS);

    // Open pipes for both TX and RX
    m_radio.openWritingPipe(m_radio_pipe_address);
    m_radio.openReadingPipe(1, m_radio_pipe_address);

    // Enter RX mode to start
    m_radio.startListening();

    m_is_radio_initialized = true;

    return IsRadioInitialized();
}

void RadioNrf24l01::Run(std::chrono::steady_clock::time_point current_time)
{
    if(not IsRadioInitialized())
    {
        printf("{%s}::{%s}() -> Failed to run radio worker task. Radio is not initialized!\n",CLASS_NAME,__func__);
        return;
    }

    m_transport_manager.Run(current_time);
}

void RadioNrf24l01::HandleRxPacket(MTP32::Packet mtp32_rx_packet)
{
    RobotMiddleware::Packet rm_rx_packet {};

    memcpy(&rm_rx_packet, mtp32_rx_packet.data(), sizeof(rm_rx_packet));
    
    m_rx_packet_callback(rm_rx_packet);
}

std::optional<MTP32::Packet> RadioNrf24l01::RequestRadioRx()
{
    if(not m_radio.available())
    {
        return std::nullopt;
    }

    MTP32::Packet packet {};

    m_radio.read(packet.data(), packet.size());

    return packet;
}

void RadioNrf24l01::RequestRadioTx(const MTP32::Packet &tx_packet_bytes)
{
    m_radio.stopListening(); // Enter TX mode
    m_radio.write(tx_packet_bytes.data(), tx_packet_bytes.size());
    m_radio.startListening(); // Enter RX mode
}

} // RobotRemoteController::Hal