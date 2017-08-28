/*
 * SX1276wrapper.h
 *
 *  Created on: Aug 10, 2017
 *      Author: Bora
 */

#ifndef MICROFRAMEWORKPK_V4_3_DEVICECODE_TARGETS_NATIVE_STM32F10X_DEVICECODE_DRIVERS_RADIO_SX1276_SX1276WRAPPER_H_
#define MICROFRAMEWORKPK_V4_3_DEVICECODE_TARGETS_NATIVE_STM32F10X_DEVICECODE_DRIVERS_RADIO_SX1276_SX1276WRAPPER_H_


#include "EmoteLoraHat.h"

#include "sx1276driver/sx1276/sx1276.h"
#include "sx1276wrapper_definitions.h"


class SX1276M1BxASWrapper : public SX1276_Semtech::SX1276, private LoraHat::Emote_Lora_Hat {
public :
	static void SX1276_Radio_Interrupt_Handler0(GPIO_PIN Pin, BOOL PinState, void* Param);
	static void SX1276_Radio_Interrupt_Handler1(GPIO_PIN Pin, BOOL PinState, void* Param);
	static void SX1276_Radio_Interrupt_Handler2(GPIO_PIN Pin, BOOL PinState, void* Param);
	static void SX1276_Radio_Interrupt_Handler3(GPIO_PIN Pin, BOOL PinState, void* Param);
	static void SX1276_Radio_Interrupt_Handler4(GPIO_PIN Pin, BOOL PinState, void* Param);
	static void SX1276_Radio_Interrupt_Handler5(GPIO_PIN Pin, BOOL PinState, void* Param);

	static void SX1276_Radio_OnTimeoutIrq(void* param);

private:
	RadioRegisters_t RadioRegsInit[16];
public:
	SX1276M1BxASWrapper();
	void Initialize(SX1276_Semtech::RadioEvents_t *events);
//	SX1276M1BxASWrapper(RadioEvents_t *events );
	virtual ~SX1276M1BxASWrapper();

	virtual void SetOpMode_public( uint8_t opMode );

    /*!
     * @brief Checks if the given RF frequency is supported by the hardware
     *
     * @param [IN] frequency RF frequency to be checked
     * @retval isSupported [true: supported, false: unsupported]
     */
	virtual bool CheckRfFrequency( uint32_t frequency );
    /*!
     * @brief Writes the radio register at the specified address
     *
     * @param [IN]: addr Register address
     * @param [IN]: data New register value
     */
    virtual void Write ( uint8_t addr, uint8_t data );
    /*!
     * @brief Reads the radio register at the specified address
     *
     * @param [IN]: addr Register address
     * @retval data Register value
     */
    virtual uint8_t Read ( uint8_t addr );
    /*!
     * @brief Writes multiple radio registers starting at address
     *
     * @param [IN] addr   First Radio register address
     * @param [IN] buffer Buffer containing the new register's values
     * @param [IN] size   Number of registers to be written
     */
    virtual void Write( uint8_t addr, uint8_t *buffer, uint8_t size );
    /*!
     * @brief Reads multiple radio registers starting at address
     *
     * @param [IN] addr First Radio register address
     * @param [OUT] buffer Buffer where to copy the registers data
     * @param [IN] size Number of registers to be read
     */
    virtual void Read ( uint8_t addr, uint8_t *buffer, uint8_t size );
    /*!
     * @brief Writes the buffer contents to the SX1276 FIFO
     *
     * @param [IN] buffer Buffer containing data to be put on the FIFO.
     * @param [IN] size Number of bytes to be written to the FIFO
     */
    virtual void WriteFifo( uint8_t *buffer, uint8_t size );
    /*!
     * @brief Reads the contents of the SX1276 FIFO
     *
     * @param [OUT] buffer Buffer where to copy the FIFO read data.
     * @param [IN] size Number of bytes to be read from the FIFO
     */
    virtual void ReadFifo( uint8_t *buffer, uint8_t size );
    /*!
     * @brief Resets the SX1276
     */
    virtual void Reset( void );

    friend class Samraksh_SX1276_hal;

protected:
    /*!
     * @brief Initializes the radio I/Os pins interface
     */
    virtual void IoInit( void );

    virtual void InitializeTimers();

    /*!
     *    @brief Initializes the radio registers
     */
    virtual void RadioRegistersInit( );

    /*!
     * @brief Initializes the radio SPI
     */
    virtual void SpiInit( void );

    /*!
     * @brief Initializes DIO IRQ handlers
     *
     * @param [IN] irqHandlers Array containing the IRQ callback functions
     */
    virtual void IoIrqInit( );

    /*!
     * @brief De-initializes the radio I/Os pins interface.
     *
     * \remark Useful when going in MCU lowpower modes
     */
    virtual void IoDeInit( void );

    /*!
     * @brief Sets the radio output power.
     *
     * @param [IN] power Sets the RF output power
     */
    virtual void SetRfTxPower( int8_t power );

    /*!
     * @brief Gets the board PA selection configuration
     *
     * @param [IN] channel Channel frequency in Hz
     * @retval PaSelect RegPaConfig PaSelect value
     */
    virtual uint8_t GetPaSelect( uint32_t channel );

    /*!
     * @brief Set the RF Switch I/Os pins in Low Power mode
     *
     * @param [IN] status enable or disable
     */
    virtual void SetAntSwLowPower( bool status );

    /*!
     * @brief Initializes the RF Switch I/Os pins interface
     */
    virtual void AntSwInit( void );

    /*!
     * @brief De-initializes the RF Switch I/Os pins interface
     *
     * \remark Needed to decrease the power consumption in MCU lowpower modes
     */
    virtual void AntSwDeInit( void );

    /*!
     * @brief Controls the antenna switch if necessary.
     *
     * \remark see errata note
     *
     * @param [IN] opMode Current radio operating mode
     */
    virtual void SetAntSw( uint8_t opMode );


    virtual void SetTimeoutTimer(TimeoutName_t ton, float delay) ;
    virtual void CancelTimeoutTimer(TimeoutName_t ton);


private:
    virtual uint8_t GetTimerID(TimeoutName_t ton) ;

};




#endif /* MICROFRAMEWORKPK_V4_3_DEVICECODE_TARGETS_NATIVE_STM32F10X_DEVICECODE_DRIVERS_RADIO_SX1276_SX1276WRAPPER_H_ */
