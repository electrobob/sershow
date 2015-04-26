
#include "utils.h"

void set32MHzClock(const CLK_PSADIV_t divider) {

    //Start 32 Mhz clock and wait it ti stabilize
    OSC.CTRL = OSC_RC32MEN_bm;
	while (!(OSC.STATUS & OSC_RC32MRDY_bm));

	// Set divider
	CCP = CCP_IOREG_gc;
	CLK.PSCTRL = divider;

	// Switch to 32 Mhz clock
	CCP = CCP_IOREG_gc;
	CLK.CTRL = CLK_SCLKSEL_RC32M_gc;

}



uint32_t muldiv(uint32_t value, const uint32_t multiplier, const uint32_t divider, uint32_t& remainder )
{
    uint32_t q = 0;              // the quotient
    uint32_t r = 0;              // the remainder
    uint32_t qn = multiplier / divider;
    uint32_t rn = multiplier % divider;
    while(value)
    {
        if (value & 1)
        {
            q += qn;
            r += rn;
            if (r >= divider)
            {
                q++;
                r -= divider;
            }
        }
        value  >>= 1;
        qn <<= 1;
        rn <<= 1;
        if (rn >= divider)
        {
            qn++;
            rn -= divider;
        }
    }
    remainder = r;
    return q;
}
