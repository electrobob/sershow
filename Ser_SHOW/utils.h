#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <avr/io.h>

void set32MHzClock(const CLK_PSADIV_t divider);

uint32_t muldiv(uint32_t value, const uint32_t multiplier, const uint32_t divider, uint32_t& remainder );


template <class T, uint8_t elements>
class mean {
public:
    T calc(T new_value)
    {
        prev_values[pos] = new_value;
        pos++;
        if(pos >= elements) pos = 0;

        int32_t ret = 0;
        for (uint8_t i = 0; i < elements; i++)
        {
            ret += prev_values[i];
        }

        return ret / elements;
    }

private:
    T prev_values[elements];
    uint8_t pos;

};

#endif // UTILS_H_INCLUDED
