# Switch debounce for Arduino

All mechanical switches suffer from switch bounce. The way to solve this is to low-pass-filter the signal and pass it through a Schmitt-trigger (hysteresis) before sampling.

## Circuit 1

Simply put a 100 nF capacitor across the switch. This will work for the rising edge of the pulse, but not for the falling edge. But this is a good 1-component solution in the vast majority of use cases.

## Circuit 2

This implements a full debounce on both the leading and trailing edges of the switch press, using a proper series-R / shunt-C LPF (low-pass filter.)

```
                      5V
                      |
                  Rpull = 10 kΩ
                      |
                      +------[button]------ GND
                      |
                     Rseries = 10 kΩ
                      |
                      +----→ D2 (INPUT)
                      |
                     C = 100 nF
                      |
                     GND

```

## Notes

1. Arduino ATmega digital inputs all have Schmitt-triggers. If this is for another general-purpose circuit, then use a Schmitt-trigger buffer or buffer inverter such as the 74HC14 or 40106.
2. We are using pinMode(INPUT), not INPUT_PULLUP, because the pullup resistor is now external.
3. The debounce will work for any digital input pin. But, our test code must use a pin capable of triggering IRQ, which restricts us to D2 or D3 for the test code itself.
