#include <stdint.h>

#include "stm32f0xx_hal.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_spi.h"
#include "stm32f0xx_ll_usart.h"

#include <libflock.h>

unsigned flock_millis(void)
{
    return HAL_GetTick();
}

int flock_serial_write(uint8_t c)
{
    while (!LL_USART_IsActiveFlag_TXE(USART1))
    {
    }
    LL_USART_ClearFlag_TC(USART1);

    LL_USART_TransmitData8(USART1, c);
    return 0;
}

void flock_device_addr_get(flock_addr_t *addr)
{
    // Since the UI is 12 bytes and the address is 6,
    // xor the 1st 6 bytes with the rest.
    for (int ii = 0; ii < 6; ii++)
    {
        addr->data[ii] = *(volatile uint8_t *)(UID_BASE + ii);
        addr->data[ii] ^= *(volatile uint8_t *)(UID_BASE + ii + 6);
    }
}

void flock_sx127x_set_rst(unsigned val)
{
    if (val > 0)
    {
        LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_1);
    }
    else
    {
        LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_1);
    }
}

static uint8_t spi_transmit(uint8_t c)
{
    while (!LL_SPI_IsActiveFlag_TXE(SPI1))
    {
    }
    LL_SPI_TransmitData8(SPI1, c);
    while (!LL_SPI_IsActiveFlag_RXNE(SPI1))
    {
    }
    return LL_SPI_ReceiveData8(SPI1);
}

void flock_sx127x_spi_transmit(uint16_t cmd, uint32_t addr,
                               const void *tx, size_t tx_size,
                               void *rx, size_t rx_size)
{
    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_4);

    spi_transmit(cmd << 7 | addr);

    const uint8_t *ptx = tx;
    uint8_t *prx = rx;
    for (size_t ii = 0; ii < tx_size; ii++, ptx++)
    {
        uint8_t c = spi_transmit(*ptx);
        if (prx)
        {
            *prx = c;
            prx++;
        }
    }

    while (LL_SPI_IsActiveFlag_BSY(SPI1))
    {
    }

    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_4);
}
