#ifndef FlickerTouch_h
#define FlickerTouch_h

#include "Arduino.h"

// =============================================================================
// Flicker Unified Touch Abstraction
// Supports Teensy, ESP32, ESP32-S2/S3, AVR (Uno, Nano, Mega, Leonardo), and more
// =============================================================================

#if defined(ARDUINO_ARCH_ESP32)

  inline int flickerTouchRead(uint8_t pin) {
    int val = (int)touchRead(pin);
    #if (defined(CONFIG_IDF_TARGET_ESP32) && CONFIG_IDF_TARGET_ESP32) && !defined(CONFIG_IDF_TARGET_ESP32S2) && !defined(CONFIG_IDF_TARGET_ESP32S3)
      // Classic ESP32 only: touchRead() returns decreasing values on touch (~80 idle, ~20 touched).
      // Invert reading so higher value indicates higher capacitance/touch.
      if (val > 0 && val <= 1024) {
        val = 1024 - val;
      }
    #endif
    return val;
  }

  inline void flickerTouchInit() {
    // No ADC priming needed for ESP32
  }

#elif defined(CORE_TEENSY) || defined(TEENSYDUINO)

  // Teensy 3.0/3.1/3.2/3.5/3.6/LC: Native hardware TSI touchRead()
  inline int flickerTouchRead(uint8_t pin) {
    return touchRead(pin);
  }

  inline void flickerTouchInit() {
    #if defined(A0)
    analogRead(A0); // ADC priming for Teensy TSI multiplexer
    #endif
  }

#elif defined(__AVR__)

  // AVR (Uno, Nano, Mega, Leonardo, Pro Mini, etc.)
  // Single-pin capacitive sensing using ADC sample & hold capacitor charge redistribution.
  inline int avrTouchRead(uint8_t pin, uint8_t samples = 4) {
    uint8_t analogChannel = pin;
    #if defined(A0)
    if (analogChannel >= A0) {
      analogChannel -= A0;
    }
    #endif

    long total = 0;
    for (uint8_t i = 0; i < samples; ++i) {
      // 1. Charge external pin/conductor by enabling pullup
      #if defined(A0)
      pinMode(A0 + analogChannel, INPUT_PULLUP);
      #else
      pinMode(pin, INPUT_PULLUP);
      #endif

      // 2. Discharge internal ADC S/H capacitor to ground
      #if defined(ADMUX)
        #if defined(__AVR_ATmega32U4__)
          ADMUX = (ADMUX & 0xE0) | 0x1F;
        #elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
          ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((0x1F >> 3) & 0x01) << MUX5);
          ADMUX = (ADMUX & 0xE0) | (0x1F & 0x07);
        #else
          ADMUX = (ADMUX & 0xE0) | 0x0F;
        #endif

        ADCSRA |= (1 << ADSC);
        while (ADCSRA & (1 << ADSC));
        #if defined(ADIF)
        ADCSRA |= (1 << ADIF);
        #endif
      #endif

      // 3. Connect pin to discharged ADC internal cap and read voltage
      #if defined(A0)
      pinMode(A0 + analogChannel, INPUT);
      total += analogRead(A0 + analogChannel);
      #else
      pinMode(pin, INPUT);
      total += analogRead(pin);
      #endif
    }
    return (int)(total / samples);
  }

  inline int flickerTouchRead(uint8_t pin) {
    return avrTouchRead(pin);
  }

  inline void flickerTouchInit() {
    #if defined(A0)
    analogRead(A0);
    #endif
  }

#else

  // Generic fallback: if touchRead() exists or fallback to analogRead
  #if defined(touchRead)
  inline int flickerTouchRead(uint8_t pin) {
    return touchRead(pin);
  }
  #else
  inline int flickerTouchRead(uint8_t pin) {
    return analogRead(pin);
  }
  #endif

  inline void flickerTouchInit() {
    #if defined(A0)
    analogRead(A0);
    #endif
  }

#endif

#endif // FlickerTouch_h
