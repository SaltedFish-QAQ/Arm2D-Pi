#include "board_peripherals.h"

#include "bsp_cfg.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"

#include "drv_QMI8658.h"
#include "drv_buzzer.h"

static uint32_t s_wBuzzerStopMS;
static bool s_bBuzzerTimed;
static bool s_bImuReady;
static app_alert_handler_t s_fnAlertHandler;
static void *s_pAlertHandlerTarget;

/* Standard 88-key piano range: A0 through C8, rounded to integer Hz. */
#define APP_BUZZER_NOTE_A0_HZ                            28u
#define APP_BUZZER_NOTE_AS0_HZ                           29u
#define APP_BUZZER_NOTE_B0_HZ                            31u
#define APP_BUZZER_NOTE_C1_HZ                            33u
#define APP_BUZZER_NOTE_CS1_HZ                           35u
#define APP_BUZZER_NOTE_D1_HZ                            37u
#define APP_BUZZER_NOTE_DS1_HZ                           39u
#define APP_BUZZER_NOTE_E1_HZ                            41u
#define APP_BUZZER_NOTE_F1_HZ                            44u
#define APP_BUZZER_NOTE_FS1_HZ                           46u
#define APP_BUZZER_NOTE_G1_HZ                            49u
#define APP_BUZZER_NOTE_GS1_HZ                           52u
#define APP_BUZZER_NOTE_A1_HZ                            55u
#define APP_BUZZER_NOTE_AS1_HZ                           58u
#define APP_BUZZER_NOTE_B1_HZ                            62u
#define APP_BUZZER_NOTE_C2_HZ                            65u
#define APP_BUZZER_NOTE_CS2_HZ                           69u
#define APP_BUZZER_NOTE_D2_HZ                            73u
#define APP_BUZZER_NOTE_DS2_HZ                           78u
#define APP_BUZZER_NOTE_E2_HZ                            82u
#define APP_BUZZER_NOTE_F2_HZ                            87u
#define APP_BUZZER_NOTE_FS2_HZ                           93u
#define APP_BUZZER_NOTE_G2_HZ                            98u
#define APP_BUZZER_NOTE_GS2_HZ                          104u
#define APP_BUZZER_NOTE_A2_HZ                           110u
#define APP_BUZZER_NOTE_AS2_HZ                          117u
#define APP_BUZZER_NOTE_B2_HZ                           123u
#define APP_BUZZER_NOTE_C3_HZ                           131u
#define APP_BUZZER_NOTE_CS3_HZ                          139u
#define APP_BUZZER_NOTE_D3_HZ                           147u
#define APP_BUZZER_NOTE_DS3_HZ                          156u
#define APP_BUZZER_NOTE_E3_HZ                           165u
#define APP_BUZZER_NOTE_F3_HZ                           175u
#define APP_BUZZER_NOTE_FS3_HZ                          185u
#define APP_BUZZER_NOTE_G3_HZ                           196u
#define APP_BUZZER_NOTE_GS3_HZ                          208u
#define APP_BUZZER_NOTE_A3_HZ                           220u
#define APP_BUZZER_NOTE_AS3_HZ                          233u
#define APP_BUZZER_NOTE_B3_HZ                           247u
#define APP_BUZZER_NOTE_C4_HZ                           262u
#define APP_BUZZER_NOTE_CS4_HZ                          277u
#define APP_BUZZER_NOTE_D4_HZ                           294u
#define APP_BUZZER_NOTE_DS4_HZ                          311u
#define APP_BUZZER_NOTE_E4_HZ                           330u
#define APP_BUZZER_NOTE_F4_HZ                           349u
#define APP_BUZZER_NOTE_FS4_HZ                          370u
#define APP_BUZZER_NOTE_G4_HZ                           392u
#define APP_BUZZER_NOTE_GS4_HZ                          415u
#define APP_BUZZER_NOTE_A4_HZ                           440u
#define APP_BUZZER_NOTE_AS4_HZ                          466u
#define APP_BUZZER_NOTE_B4_HZ                           494u
#define APP_BUZZER_NOTE_C5_HZ                           523u
#define APP_BUZZER_NOTE_CS5_HZ                          554u
#define APP_BUZZER_NOTE_D5_HZ                           587u
#define APP_BUZZER_NOTE_DS5_HZ                          622u
#define APP_BUZZER_NOTE_E5_HZ                           659u
#define APP_BUZZER_NOTE_F5_HZ                           698u
#define APP_BUZZER_NOTE_FS5_HZ                          740u
#define APP_BUZZER_NOTE_G5_HZ                           784u
#define APP_BUZZER_NOTE_GS5_HZ                          831u
#define APP_BUZZER_NOTE_A5_HZ                           880u
#define APP_BUZZER_NOTE_AS5_HZ                          932u
#define APP_BUZZER_NOTE_B5_HZ                           988u
#define APP_BUZZER_NOTE_C6_HZ                          1047u
#define APP_BUZZER_NOTE_CS6_HZ                         1109u
#define APP_BUZZER_NOTE_D6_HZ                          1175u
#define APP_BUZZER_NOTE_DS6_HZ                         1245u
#define APP_BUZZER_NOTE_E6_HZ                          1319u
#define APP_BUZZER_NOTE_F6_HZ                          1397u
#define APP_BUZZER_NOTE_FS6_HZ                         1480u
#define APP_BUZZER_NOTE_G6_HZ                          1568u
#define APP_BUZZER_NOTE_GS6_HZ                         1661u
#define APP_BUZZER_NOTE_A6_HZ                          1760u
#define APP_BUZZER_NOTE_AS6_HZ                         1865u
#define APP_BUZZER_NOTE_B6_HZ                          1976u
#define APP_BUZZER_NOTE_C7_HZ                          2093u
#define APP_BUZZER_NOTE_CS7_HZ                         2217u
#define APP_BUZZER_NOTE_D7_HZ                          2349u
#define APP_BUZZER_NOTE_DS7_HZ                         2489u
#define APP_BUZZER_NOTE_E7_HZ                          2637u
#define APP_BUZZER_NOTE_F7_HZ                          2794u
#define APP_BUZZER_NOTE_FS7_HZ                         2960u
#define APP_BUZZER_NOTE_G7_HZ                          3136u
#define APP_BUZZER_NOTE_GS7_HZ                         3322u
#define APP_BUZZER_NOTE_A7_HZ                          3520u
#define APP_BUZZER_NOTE_AS7_HZ                         3729u
#define APP_BUZZER_NOTE_B7_HZ                          3951u
#define APP_BUZZER_NOTE_C8_HZ                          4186u

static const drv_buzzer_note_t c_tCountdownFinishedNotes[] = {
    {APP_BUZZER_NOTE_C6_HZ, 120u},
    {DRV_BUZZER_REST,        60u},
    {APP_BUZZER_NOTE_E6_HZ, 180u},
    {DRV_BUZZER_REST,       130u},
    {APP_BUZZER_NOTE_G6_HZ, 120u},
    {DRV_BUZZER_REST,        60u},
    {APP_BUZZER_NOTE_C7_HZ, 260u},
};

static const drv_buzzer_score_t c_tCountdownFinishedScore = {
    "Countdown finished",
    c_tCountdownFinishedNotes,
    (uint16_t)(sizeof(c_tCountdownFinishedNotes) /
               sizeof(c_tCountdownFinishedNotes[0])),
};

/*
 * Melody source supplied by the user:
 * https://blog.csdn.net/breaksoftware/article/details/147964335
 * Original article license: CC BY-SA 4.0.
 */
static const drv_buzzer_note_t c_tHappyBirthdayNotes[] = {
    {APP_BUZZER_NOTE_C4_HZ,  300u},
    {APP_BUZZER_NOTE_C4_HZ,  300u},
    {APP_BUZZER_NOTE_D4_HZ,  600u},
    {APP_BUZZER_NOTE_C4_HZ,  600u},
    {APP_BUZZER_NOTE_F4_HZ,  600u},
    {APP_BUZZER_NOTE_E4_HZ, 1200u},
    {APP_BUZZER_NOTE_C4_HZ,  300u},
    {APP_BUZZER_NOTE_C4_HZ,  300u},
    {APP_BUZZER_NOTE_D4_HZ,  600u},
    {APP_BUZZER_NOTE_C4_HZ,  600u},
    {APP_BUZZER_NOTE_G4_HZ,  600u},
    {APP_BUZZER_NOTE_F4_HZ, 1200u},
    {APP_BUZZER_NOTE_C4_HZ,  300u},
    {APP_BUZZER_NOTE_C4_HZ,  300u},
    {APP_BUZZER_NOTE_C5_HZ,  600u},
    {APP_BUZZER_NOTE_A4_HZ,  600u},
    {APP_BUZZER_NOTE_F4_HZ,  600u},
    {APP_BUZZER_NOTE_E4_HZ,  600u},
    {APP_BUZZER_NOTE_D4_HZ, 1200u},
    {APP_BUZZER_NOTE_B4_HZ,  300u},
    {APP_BUZZER_NOTE_B4_HZ,  300u},
    {APP_BUZZER_NOTE_A4_HZ,  600u},
    {APP_BUZZER_NOTE_F4_HZ,  600u},
    {APP_BUZZER_NOTE_G4_HZ,  600u},
    {APP_BUZZER_NOTE_F4_HZ, 1200u},
};

static const drv_buzzer_score_t c_tHappyBirthdayScore = {
    "Happy Birthday",
    c_tHappyBirthdayNotes,
    (uint16_t)(sizeof(c_tHappyBirthdayNotes) /
               sizeof(c_tHappyBirthdayNotes[0])),
};

bool app_peripherals_init(void)
{
    app_buzzer_init();
    s_bImuReady = app_imu_init();
    return s_bImuReady;
}

void app_peripherals_task(uint32_t wNowMS)
{
    if (s_bBuzzerTimed && ((int32_t)(wNowMS - s_wBuzzerStopMS) >= 0)) {
        drv_buzzer_stop();
        s_bBuzzerTimed = false;
    }

    if (drv_buzzer_score_is_active()) {
        (void)drv_buzzer_score_task();
    }
}

void app_buzzer_init(void)
{
    drv_buzzer_init();
    s_bBuzzerTimed = false;
}

void app_buzzer_set_tone(uint16_t hwFrequencyHz)
{
    s_bBuzzerTimed = false;
    drv_buzzer_set_tone(hwFrequencyHz, DRV_BUZZER_DUTY_PERMILLE);
}

void app_buzzer_beep(uint16_t hwFrequencyHz, uint16_t hwDurationMS)
{
    if ((0u == hwFrequencyHz) || (0u == hwDurationMS)) {
        drv_buzzer_stop();
        s_bBuzzerTimed = false;
        return;
    }

    drv_buzzer_set_tone(hwFrequencyHz, DRV_BUZZER_DUTY_PERMILLE);
    s_wBuzzerStopMS = to_ms_since_boot(get_absolute_time()) + hwDurationMS;
    s_bBuzzerTimed = true;
}

bool app_buzzer_play_countdown_finished_chime(void)
{
    s_bBuzzerTimed = false;
    return drv_buzzer_score_start(&c_tCountdownFinishedScore);
}

bool app_buzzer_play_happy_birthday(void)
{
    s_bBuzzerTimed = false;
    return drv_buzzer_score_start(&c_tHappyBirthdayScore);
}

bool app_buzzer_is_playing(void)
{
    return drv_buzzer_score_is_active();
}

void app_alert_set_handler(app_alert_handler_t fnHandler, void *pTarget)
{
    s_fnAlertHandler = fnHandler;
    s_pAlertHandlerTarget = pTarget;
}

void app_alert_play(app_alert_t tAlert)
{
    if (NULL != s_fnAlertHandler) {
        s_fnAlertHandler(tAlert, s_pAlertHandlerTarget);
        return;
    }

    if (APP_ALERT_COUNTDOWN_FINISHED_DEFAULT == tAlert) {
        (void)app_buzzer_play_countdown_finished_chime();
    } else if (APP_ALERT_COUNTDOWN_FINISHED_BIRTHDAY == tAlert) {
        (void)app_buzzer_play_happy_birthday();
    }
}

bool app_imu_init(void)
{
    i2c_init(I2C_PORT, QMI8658_I2C_BAUD_HZ);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    s_bImuReady = QMI8658A_Init() != 0;
    return s_bImuReady;
}

bool app_imu_read(float fData[6])
{
    int16_t hwRawData[6];

    if ((NULL == fData) || !s_bImuReady || !QMI8658A_ReadData(hwRawData)) {
        return false;
    }

    QMI8658A_ConvertData(hwRawData, fData, ACCRANGE, GYRRANGE);
    return true;
}

void app_button_init(app_button_t *ptButton,
                     uint8_t chPin,
                     bool bActiveLow,
                     uint32_t wDebounceMS)
{
    bool bPressed;

    if (NULL == ptButton) {
        return;
    }

    gpio_init(chPin);
    gpio_set_dir(chPin, GPIO_IN);
    if (bActiveLow) {
        gpio_pull_up(chPin);
    } else {
        gpio_pull_down(chPin);
    }

    bPressed = (gpio_get(chPin) != 0u);
    if (bActiveLow) {
        bPressed = !bPressed;
    }

    ptButton->chPin = chPin;
    ptButton->bActiveLow = bActiveLow;
    ptButton->bPressed = bPressed;
    ptButton->bCandidatePressed = bPressed;
    ptButton->bPressLatched = false;
    ptButton->wCandidateSinceMS = to_ms_since_boot(get_absolute_time());
    ptButton->wDebounceMS = wDebounceMS;
}

void app_button_poll(app_button_t *ptButton, uint32_t wNowMS)
{
    bool bPressed;

    if (NULL == ptButton) {
        return;
    }

    bPressed = (gpio_get(ptButton->chPin) != 0u);
    if (ptButton->bActiveLow) {
        bPressed = !bPressed;
    }

    if (bPressed != ptButton->bCandidatePressed) {
        ptButton->bCandidatePressed = bPressed;
        ptButton->wCandidateSinceMS = wNowMS;
        return;
    }

    if ((bPressed != ptButton->bPressed) &&
        ((uint32_t)(wNowMS - ptButton->wCandidateSinceMS) >= ptButton->wDebounceMS)) {
        ptButton->bPressed = bPressed;
        if (bPressed) {
            ptButton->bPressLatched = true;
        }
    }
}

bool app_button_is_pressed(const app_button_t *ptButton)
{
    return (NULL != ptButton) && ptButton->bPressed;
}

bool app_button_was_pressed(app_button_t *ptButton)
{
    bool bPressed;

    if (NULL == ptButton) {
        return false;
    }

    bPressed = ptButton->bPressLatched;
    ptButton->bPressLatched = false;
    return bPressed;
}