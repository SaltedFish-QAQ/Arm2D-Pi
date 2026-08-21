#ifndef BOARD_PERIPHERALS_H
#define BOARD_PERIPHERALS_H

#include <stdbool.h>
#include <stdint.h>

typedef enum app_alert_t {
    APP_ALERT_COUNTDOWN_FINISHED_DEFAULT,
    APP_ALERT_COUNTDOWN_FINISHED_BIRTHDAY,
} app_alert_t;

typedef void (*app_alert_handler_t)(app_alert_t tAlert, void *pTarget);

typedef struct app_button_t {
    uint8_t chPin;
    bool bActiveLow;
    bool bPressed;
    bool bCandidatePressed;
    bool bPressLatched;
    uint32_t wCandidateSinceMS;
    uint32_t wDebounceMS;
} app_button_t;

/** Initialise the built-in passive buzzer and QMI8658 sensor bus. */
bool app_peripherals_init(void);

/** Advance non-blocking application-layer peripherals. Call once per main-loop pass. */
void app_peripherals_task(uint32_t wNowMS);

/** Reserve the buzzer GPIO and leave it silent. */
void app_buzzer_init(void);

/** Start a continuous tone. Pass 0 Hz to silence it. */
void app_buzzer_set_tone(uint16_t hwFrequencyHz);

/** Play a tone and stop it automatically after hwDurationMS. */
void app_buzzer_beep(uint16_t hwFrequencyHz, uint16_t hwDurationMS);

/** Start the built-in short countdown-finished chime. */
bool app_buzzer_play_countdown_finished_chime(void);

/** Start the built-in Happy Birthday melody. Call app_peripherals_task() to advance it. */
bool app_buzzer_play_happy_birthday(void);

/** Return true while a score-based buzzer melody is playing. */
bool app_buzzer_is_playing(void);

/** Set a custom alert strategy. Pass NULL to restore the default buzzer alert. */
void app_alert_set_handler(app_alert_handler_t fnHandler, void *pTarget);

/** Play an alert through the configured strategy. */
void app_alert_play(app_alert_t tAlert);

/** Configure I2C0 and initialise the QMI8658. */
bool app_imu_init(void);

/** Read acceleration [0..2] in g and angular rate [3..5] in dps. */
bool app_imu_read(float fData[6]);

/** Configure a GPIO input with its internal pull resistor for a push button. */
void app_button_init(app_button_t *ptButton,
                     uint8_t chPin,
                     bool bActiveLow,
                     uint32_t wDebounceMS);

/** Sample and debounce a button. Call once per main-loop pass. */
void app_button_poll(app_button_t *ptButton, uint32_t wNowMS);

/** Return the stable button state. */
bool app_button_is_pressed(const app_button_t *ptButton);

/** Return and clear one press event. */
bool app_button_was_pressed(app_button_t *ptButton);

#endif