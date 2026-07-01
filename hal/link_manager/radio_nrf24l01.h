#pragma once
#include "i_link_manager.h"

#include <mtp32.h>
#include <RF24.h>

#include <cstdint>
#include <chrono>
#include <string_view>

namespace RobotRemoteController::Hal
{
/**
 * @brief This class 
 */
class RadioNrf24L01 : public ILinkManager
{
public:
    // FIXME: CONFIRM THIS PIN ASSIGNMENT IS CORRECT LATER
    static constexpr uint8_t DEFAULT_SPI_CE_GPIO = 22;
    static constexpr uint64_t DEFAULT_RADIO_PIPE_ADDRESS = 0xE8E8F0F0E1LL;

    enum class SpiOption : uint8_t
    {
        SPI_0 = 0,
        SPI_1 = 1
    };

    ~RadioNrf24L01() = default;
    RadioNrf24L01(uint8_t chip_enable_gpio = DEFAULT_SPI_CE_GPIO, 
        SpiOption spi_option = SpiOption::SPI_0,
        uint64_t radio_pipe_address = DEFAULT_RADIO_PIPE_ADDRESS,
        std::chrono::milliseconds radio_rx_timeout = MTP32::TransportManager::RX_TIMEOUT
    );

    /**
     * @brief Schedule a packet to be transmitted at the next possible opportunity.
     */
    void EnqueueTxPacket(const RobotMiddleware::Packet& packet) override;

    /**
     * @brief Set a callback function that is activated when the radio receives a packet
     */
    void SetRxPacketCallback(RxPacketCallback callback) override;

     /**
     * @brief Initializes the radio.
     * @warning If the radio is not initialized, the TX packets will be dropped and no RX packets will be received.
     * @returns Whether the radio is initialized. 
     * @note Subsequent calls after a successful initialization will have no effect on the radio and just return true.
     */
    bool InitializeRadio();

     /**
     * @brief Drives the state machine behavior of the MTP32 RF communication protocol.
     * @note Used the current_time argument to determine the passage of time between Run() calls.
     */
    void Run(std::chrono::steady_clock::time_point current_time);

    bool IsRadioInitialized() const { return m_is_radio_initialized; }

private:
    static constexpr std::string_view CLASS_NAME = "RadioNrf24L01";
    RxPacketCallback m_rx_packet_callback;
    std::chrono::milliseconds m_radio_rx_timeout;
    MTP32::TransportManager m_transport_manager;
    uint64_t m_radio_pipe_address {0};
    bool m_is_radio_initialized { false };
    RF24 m_radio;

    void HandleRxPacket(MTP32::Packet rx_packet);
    /**
    * @brief Polls the radio hardware to check if a packet was received.
    * @returns The packet if available, std::nullopt if not.
    */
    std::optional<MTP32::Packet> RequestRadioRx();
    void RequestRadioTx(const MTP32::Packet& tx_packet_bytes);
};
} // namespace RobotRemoteController::Hal