#include "pico/stdlib.h"
#include <stdio.h>
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"

const uint PIN_LED_RED = 13;
const uint PIN_LED_GREEN = 11;
const uint PIN_LED_BLUE = 15;


void on_pwm_wrap() {
    static int fade = 0;
    static bool going_up = true;
    // Clear the interrupt flag that brought us here
    pwm_clear_irq(pwm_gpio_to_slice_num(PIN_LED_RED));
    pwm_clear_irq(pwm_gpio_to_slice_num(PIN_LED_GREEN));
    pwm_clear_irq(pwm_gpio_to_slice_num(PIN_LED_BLUE));

    if (going_up) {
        ++fade;
        if (fade > 255) {
            fade = 255;
            going_up = false;
        }
    } else {
        --fade;
        if (fade < 0) {
            fade = 0;
            going_up = true;
        }
    }
    // Square the fade value to make the LED's brightness appear more linear
    // Note this range matches with the wrap value
   // pwm_set_gpio_level(led_red, fade * fade);;
    pwm_set_gpio_level(PIN_LED_RED, fade * fade);;
    pwm_set_gpio_level(PIN_LED_GREEN, fade * fade);;
    pwm_set_gpio_level(PIN_LED_BLUE, fade * fade);;
}
//#endif

int main() {
    //#ifndef PICO_DEFAULT_LED_PIN
    //#warning pwm/led_fade example requires a board with a regular LED
    //#else
    
    // Tell the LED pin that the PWM is in charge of its value.
    // gpio_set_function(led_red, GPIO_FUNC_PWM);
    gpio_set_function(PIN_LED_RED, GPIO_FUNC_PWM);
    gpio_set_function(PIN_LED_GREEN, GPIO_FUNC_PWM);
    gpio_set_function(PIN_LED_BLUE, GPIO_FUNC_PWM);
    
    // Figure out which slice we just connected to the LED pin
    
    uint slice_num1 = pwm_gpio_to_slice_num(PIN_LED_RED);
    uint slice_num2 = pwm_gpio_to_slice_num(PIN_LED_GREEN);
    uint slice_num3 = pwm_gpio_to_slice_num(PIN_LED_BLUE);
    
    
    // Mask our slice's IRQ output into the PWM block's single interrupt line,
    // and register our interrupt handler
    pwm_clear_irq(slice_num1);
    pwm_set_irq_enabled(slice_num1, true);
    pwm_clear_irq(slice_num2);
    pwm_set_irq_enabled(slice_num2, true);
    pwm_clear_irq(slice_num3);
    pwm_set_irq_enabled(slice_num3, true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);
    irq_set_enabled(PWM_IRQ_WRAP, true);
    
    // Get some sensible defaults for the slice configuration. By default, the
    // counter is allowed to wrap over its maximum range (0 to 2**16-1)
    pwm_config config = pwm_get_default_config();
    // Set divider, reduces counter clock to sysclock/this value
    pwm_config_set_clkdiv(&config, 9.f);
    
    // Load the configuration into our PWM slice, and set it running.
    pwm_init(slice_num1, &config, true);
    // sleep_ms(1000);
    pwm_init(slice_num2, &config, true);
    // sleep_ms(1000);
    pwm_init(slice_num3, &config, true);
    //sleep_ms(1000);
    
    // Everything after this point happens in the PWM interrupt handler, so we
    // can twiddle our thumbs
    while (1){
        tight_loop_contents();
        
        //#endif
    }
}
