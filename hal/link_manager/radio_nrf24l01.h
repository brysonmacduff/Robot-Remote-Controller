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
 * @brief This class functions as the sole point of control over the RF communication link.
 * @note In the context of MTP32 communication roles, this class assumes the role of "master".
 */
class RadioNrf24l01 : public ILinkManager
{
public:
    static constexpr uint8_t DEFAULT_SPI_CE_GPIO = 22;
    static constexpr uint8_t DEFAULT_SPI_CSN_GPIO = 17;
    static constexpr uint8_t DEFAULT_SPI_CLK_GPIO = 2;
    static constexpr uint8_t DEFAULT_SPI_MOSI_GPIO = 3;
    static constexpr uint8_t DEFAULT_SPI_MISO_GPIO = 4;
    static constexpr uint DEFAULT_SPI_BAUD_RATE_HERTZ = 4'000'000;
    static constexpr uint64_t DEFAULT_RADIO_PIPE_ADDRESS = 0xE8E8F0F0E1LL;

    ~RadioNrf24l01() = default;
    /**
     * @brief Specifies the configuration that the NRF24L01 radio will use.
     * @note The pico version of RF24 only uses SPI0.
     * @param radio The RF24 instance is reference-injected because RF24 instantiation on a Pico is bug prone. Therefore, the 
     * lifetime of an RF24 instance is managed externally to less painfully incorporate future updates to the RF24 library.
     * @todo Consider refactoring this API if the RF24 library receives bug fix improvements for the Pico.
     */
    RadioNrf24l01(RF24& radio,
        uint8_t chip_enable_gpio = DEFAULT_SPI_CE_GPIO, 
        uint8_t chip_select_gpio = DEFAULT_SPI_CSN_GPIO,
        uint64_t radio_pipe_address = DEFAULT_RADIO_PIPE_ADDRESS,
        uint8_t clock_gpio = DEFAULT_SPI_CLK_GPIO,
        uint8_t mosi_gpio = DEFAULT_SPI_MOSI_GPIO,
        uint8_t miso_gpio = DEFAULT_SPI_MISO_GPIO,
        uint spi_baud_rate_hertz = DEFAULT_SPI_BAUD_RATE_HERTZ,
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
     * @brief Initializes GPIO pins for SPI and starts the radio
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

    size_t GetTxMessageQueueCount() const { return m_transport_manager.GetPendingTxMessageCount(); }

private:
    static constexpr std::string_view CLASS_NAME = "RadioNrf24l01";
    RxPacketCallback m_rx_packet_callback = [](const RobotMiddleware::Packet& packet){ (void)packet;};
    std::chrono::milliseconds m_radio_rx_timeout;
    MTP32::TransportManager m_transport_manager;
    uint64_t m_radio_pipe_address {0};
    bool m_is_radio_initialized { false };
    uint8_t m_chip_enable_gpio { 0 };
    uint8_t m_chip_select_gpio { 0 };
    uint8_t m_clock_gpio { 0 };
    uint8_t m_mosi_gpio { 0 };
    uint8_t m_miso_gpio { 0 };
    uint m_spi_baud_rate_hertz { 0 };
    RF24& m_radio;

    /**
     * @brief Called when a data packet is received by the radio.
     * @note This function is called by MTP32::TransportManager after successfully receiving a Packet after calling RequestRadioRx.
     */
    void HandleRxPacket(MTP32::Packet mtp32_rx_packet);

    /**
    * @brief Polls the radio hardware to check if a packet was received.
    * @note This function is called by MTP32::TransportManager when it wants to read from the radio.
    * @returns The packet if available, std::nullopt if not.
    */
    std::optional<MTP32::Packet> RequestRadioRx();

    /**
     * @brief Attempts to use the radio to send a packet.
     * @warning Message delivery is not guaranteed.
     * @note This function called by MTP32::TransportManager when it attempts to send a message.
     */
    void RequestRadioTx(const MTP32::Packet& tx_packet_bytes);
};
} // namespace RobotRemoteController::Hal