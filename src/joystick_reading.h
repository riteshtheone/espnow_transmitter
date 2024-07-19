#ifndef TRANSMITTER_WEB_JOYSTICK_READING_H
#define TRANSMITTER_WEB_JOYSTICK_READING_H
#define CH1PIN 7
#define CH2PIN 6
#define CH3PIN 4
#define CH4PIN 5
#define CH5PIN 8

const uint16_t cthrottle = 495;
const uint16_t cyaw      = 513;
const uint16_t cpitch    = 500;
const uint16_t croll     = 492;
const uint16_t max_input = 1023;

const uint16_t min_val    = 0;
const uint16_t max_val    = 1000;

volatile uint16_t middle_val_throttle  = 406;
volatile uint16_t middle_val_roll      = 500;
volatile uint16_t middle_val_pitch     = 500;
volatile uint16_t middle_val_yaw       = 500;

const uint8_t error = 5;

uint16_t roll, pitch, throttle, yaw;
uint16_t rroll, rpitch, rthrottle, ryaw;

void joystick_setup() { analogReadResolution(10); }

void read_joystick() {
    uint16_t roll_new     = analogRead(CH1PIN);
    uint16_t pitch_new    = analogRead(CH2PIN);
    uint16_t throttle_new = analogRead(CH3PIN);
    uint16_t yaw_new      = analogRead(CH4PIN);

    rroll     = roll_new;
    rpitch    = pitch_new;
    rthrottle = throttle_new;
    ryaw      = yaw_new;

    // throttle_new = map(throttle_new, min_val, max_input, min_val, max_val);
    throttle_new = throttle_new  <= cthrottle  ? map(throttle_new, min_val, cthrottle, min_val, middle_val_throttle)    : map(throttle_new, cthrottle, max_input, middle_val_throttle, max_val);
    roll_new     = roll_new      <= croll      ? map(roll_new, min_val, croll, min_val, middle_val_roll)                : map(roll_new, croll, max_input, middle_val_roll, max_val);
    pitch_new    = pitch_new     <= cpitch     ? map(pitch_new, min_val, cpitch, min_val, middle_val_pitch)             : map(pitch_new, cpitch, max_input, middle_val_pitch, max_val);
    yaw_new      = yaw_new       <= cyaw       ? map(yaw_new, min_val, cyaw, min_val, middle_val_yaw)                   : map(yaw_new, cyaw, max_input, middle_val_yaw, max_val);

    // throttle = throttle_new <= throttle - error || throttle_new >= throttle + error ? throttle_new : throttle;
    throttle = throttle_new <= throttle - error || throttle_new >= throttle + error ? throttle_new >= middle_val_throttle  - error && throttle_new  <= middle_val_throttle  + error ? middle_val_throttle  : throttle_new  : throttle;
    roll     = roll_new     <= roll     - error || roll_new     >= roll     + error ? roll_new     >= middle_val_roll      - error && roll_new      <= middle_val_roll      + error ? middle_val_roll      : roll_new      : roll;
    pitch    = pitch_new    <= pitch    - error || pitch_new    >= pitch    + error ? pitch_new    >= middle_val_pitch     - error && pitch_new     <= middle_val_pitch     + error ? middle_val_pitch     : pitch_new     : pitch;
    yaw      = yaw_new      <= yaw      - error || yaw_new      >= yaw      + error ? yaw_new      >= middle_val_yaw       - error && yaw_new       <= middle_val_yaw       + error ? middle_val_yaw       : yaw_new       : yaw;
}
#endif //TRANSMITTER_WEB_JOYSTICK_READING_H
