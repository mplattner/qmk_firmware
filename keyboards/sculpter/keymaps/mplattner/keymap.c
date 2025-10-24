/* Copyright 2020 Chris Paynter
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

#include "print.h"

#define TH(key, hold_key) register_code16((!shifted) ? key : hold_key); return;
#define THU(key, hold_key) unregister_code16((!shifted) ? key : hold_key); return;

// one shot modifiers (sticky keys)
#define OSM_LCTL OSM(MOD_LCTL)
#define OSM_RSFT OSM(MOD_RSFT)
#define OSM_LGUI OSM(MOD_LGUI)
#define OSM_LALT OSM(MOD_LALT)

// german umlauts and special characters
#define KC_AE  ALGR(KC_A)
#define KC_OE  ALGR(KC_O)
#define KC_UE  ALGR(KC_U)
#define KC_SS  ALGR(KC_S)
#define KC_EUR ALGR(KC_4)

#define QK_FLASH QK_BOOT

// just defined so that vscode hightlights those keys in the keymap
#define TD_Z TD_Z
#define TD_X TD_X
#define TD_C TD_C
#define TD_V TD_V
#define TD_B TD_B
#define TD_N TD_N

#define TD_A TD_A
#define TD_S TD_S
#define TD_D TD_D
#define TD_F TD_F
#define TD_G TD_G

#define TD_Q TD_Q
#define TD_W TD_W
#define TD_E TD_E
#define TD_R TD_R
#define TD_T TD_T
#define TD_Y TD_Y

#define TD_LEFT TD_LEFT
#define TD_RIGHT TD_RIGHT

#define TD_SEL_L TD_SEL_L
#define TD_SEL_R TD_SEL_R

const char git_revision[] PROGMEM = GIT_REVISION;

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _NAVIGATION,
    _SELECT,
    _CODE,
    _ALTGR,
    //_NUM
};

typedef struct {
    uint16_t kc1;
    uint16_t kc2;
    uint16_t kc3;
} tap_dance_triple_t;

typedef struct {
    uint16_t tap;
    uint16_t hold;
} tap_dance_hold_t;

typedef struct {
    uint16_t tap;
    uint16_t hold;
} tap_dance_hold1_t;

#define ACTION_TAP_DANCE_TRIPLE(kc1, kc2, kc3) \
    { .fn = {NULL, tap_dance_triple_finished, NULL, NULL}, .user_data = (void *)&((tap_dance_triple_t){kc1, kc2, kc3}), }

// doesn't support key repeat for hold key
#define ACTION_TAP_DANCE_HOLD(tap, hold) \
    { .fn = {NULL, tap_dance_hold_finished, NULL, NULL}, .user_data = (void *)&((tap_dance_hold_t){tap, hold}), }

// does support key repeat for hold key
#define ACTION_TAP_DANCE_HOLD1(tap, hold) \
    { .fn = {tap_dance_hold1_on_each_tap, tap_dance_hold1_finished, tap_dance_hold1_reset, NULL}, .user_data = (void *)&((tap_dance_hold1_t){tap, hold}), }

void tap_dance_triple_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_triple_t *triple = (tap_dance_triple_t *)user_data;

    if (state->count == 1) {
        tap_code16(triple->kc1);
    }
    else if (state->count == 2) {
        if (triple->kc2 == KC_NO) {
            tap_code16(triple->kc1);
            tap_code16(triple->kc1);
        }
        else {
            tap_code16(triple->kc2);
        }
    }
    else if (state->count == 3) {
        tap_code16(triple->kc3);
    }
}

void tap_dance_hold_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_hold_t *tuple = (tap_dance_hold_t *)user_data;

    if (state->count == 1) {
        tap_code16(tuple->tap);
    }
    else if (state->count == 2) {
        if (state->pressed) {
            tap_code16(tuple->hold);
        }
        else {
            tap_code16(tuple->tap);
            tap_code16(tuple->tap);
        }
    }
}

void tap_dance_hold1_on_each_tap(tap_dance_state_t *state, void *user_data) {
    tap_dance_hold1_t *hold1 = (tap_dance_hold1_t *)user_data;

    if (state->count == 3) {
        tap_code16(hold1->tap);
        tap_code16(hold1->tap);
        tap_code16(hold1->tap);
    }
    else if (state->count > 3) {
        tap_code16(hold1->tap);
    }
}

void tap_dance_hold1_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_hold1_t *hold1 = (tap_dance_hold1_t *)user_data;

    if (state->count == 1) {
        register_code16(hold1->tap);
    }
    else if (state->count == 2) {
        // when interruped by another key, it's likely not an intentional tap-and-hold
        if (state->pressed && !state->interrupted) {
            // when key is of type TD(KC_*), then toggle layer
            if (hold1->hold >= QK_TOGGLE_LAYER && hold1->hold <= QK_TOGGLE_LAYER_MAX) {
                layer_invert(QK_TOGGLE_LAYER_GET_LAYER(hold1->hold));
            }
            else {
                register_code16(hold1->hold);

                // special case for S(KC_LEFT)
                if (hold1->hold == S(KC_LEFT)) {
                    layer_on(_SELECT);
                }
            }
        }
        else {
            tap_code16(hold1->tap);
            tap_code16(hold1->tap);
        }
    }
}

void tap_dance_hold1_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_hold1_t *hold1 = (tap_dance_hold1_t *)user_data;

    if (state->count == 1) {
        wait_ms(TAP_CODE_DELAY);
        unregister_code16(hold1->tap);
    } else if (state->count == 2) {
        unregister_code16(hold1->hold);
    }
}

enum custom_keycodes {
    P_PWD = SAFE_RANGE,
    P_LAY,
    P_GITRV
};

// "long" version of TD keys (enum starting with 0)
enum custom_tap_dance {
    TDL_Z,
    TDL_X,
    TDL_C,
    TDL_V,
    TDL_B,
    TDL_N,

    TDL_A,
    TDL_S,
    TDL_D,
    TDL_F,
    TDL_G,

    TDL_Q,
    TDL_W,
    TDL_E,
    TDL_R,
    TDL_T,
    TDL_Y,

    TDL_LEFT,
    TDL_RIGHT,

    TDL_SEL_L,
    TDL_SEL_R
};

// "short" version of TD keys (enum starting with QK_TAP_DANCE) to be directly used in keymap
enum custom_tap_dance_short {
    TD_Z = QK_TAP_DANCE,
    TD_X,
    TD_C,
    TD_V,
    TD_B,
    TD_N,

    TD_A,
    TD_S,
    TD_D,
    TD_F,
    TD_G,

    TD_Q,
    TD_W,
    TD_E,
    TD_R,
    TD_T,
    TD_Y,

    TD_LEFT,
    TD_RIGHT,

    TD_SEL_L,
    TD_SEL_R
};

tap_dance_action_t tap_dance_actions[] = {
    [TDL_Z] = ACTION_TAP_DANCE_DOUBLE(KC_Z, C(KC_Z)),
    [TDL_X] = ACTION_TAP_DANCE_DOUBLE(KC_X, C(KC_X)),
    [TDL_C] = ACTION_TAP_DANCE_DOUBLE(KC_C, C(KC_C)),
    [TDL_V] = ACTION_TAP_DANCE_DOUBLE(KC_V, C(KC_V)),
    [TDL_B] = ACTION_TAP_DANCE_DOUBLE(KC_B, C(KC_B)),
    [TDL_N] = ACTION_TAP_DANCE_HOLD1(KC_N, TG(_NAVIGATION)),

    //[TDL_A] = ACTION_TAP_DANCE_TRIPLE(KC_A, KC_NO, C(KC_A)),
    [TDL_A] = ACTION_TAP_DANCE_HOLD(KC_A, C(KC_A)),
    [TDL_S] = ACTION_TAP_DANCE_TRIPLE(KC_S, KC_NO, C(KC_S)),
    [TDL_D] = ACTION_TAP_DANCE_HOLD1(KC_D, KC_DEL),
    [TDL_F] = ACTION_TAP_DANCE_HOLD(KC_F, C(KC_F)),
    [TDL_G] = ACTION_TAP_DANCE_DOUBLE(KC_G, KC_TAB),

    [TDL_Q] = ACTION_TAP_DANCE_DOUBLE(KC_Q, A(KC_F4)),
    [TDL_W] = ACTION_TAP_DANCE_DOUBLE(KC_W, C(KC_W)),

    //[TDL_E] = ACTION_TAP_DANCE_TRIPLE(KC_E, KC_NO, KC_ENTER),
    [TDL_E] = ACTION_TAP_DANCE_HOLD1(KC_E, KC_ENTER),

    [TDL_R] = ACTION_TAP_DANCE_HOLD1(KC_R, KC_BSPC),

    //[TDL_T] = ACTION_TAP_DANCE_TRIPLE(KC_T, KC_NO, C(KC_T)),
    [TDL_T] = ACTION_TAP_DANCE_HOLD1(KC_T, KC_TAB),

    [TDL_Y] = ACTION_TAP_DANCE_DOUBLE(KC_Y, C(KC_Y)),

    [TDL_LEFT] = ACTION_TAP_DANCE_HOLD1(KC_LEFT, S(KC_LEFT)),
    [TDL_RIGHT] = ACTION_TAP_DANCE_HOLD1(KC_RIGHT, S(KC_RIGHT)),

    [TDL_SEL_L] = ACTION_TAP_DANCE_HOLD(S(KC_LEFT), C(S(KC_LEFT))), // not used in keymap
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
      KC_ESC  , KC_F1   , KC_F2     , KC_F3     , KC_F4     , KC_F5   , KC_F6             , KC_F7   , KC_F8   , KC_F9    , KC_F10    , KC_F11    , KC_F12   , KC_PSCR , KC_SCRL , KC_PAUS          , QK_REBOOT,
      QK_REP  , KC_1    , KC_2      , KC_3      , KC_4      , KC_5    , KC_6              , KC_7              , KC_8     , KC_9      , KC_0      , KC_MINS  , KC_EQL            , KC_BSPC          , KC_DEL   , KC_HOME,
      KC_TAB            , TD_Q      , TD_W      , TD_E      , TD_R    , TD_T              , TD_Y              , KC_U     , KC_I      , KC_O      , KC_P     , KC_LBRC , KC_RBRC , KC_NUBS                     , KC_END ,
      OSM_LCTL          , TD_A      , TD_S      , TD_D      , TD_F    , TD_G              , KC_H              , KC_J     , KC_K      , KC_L      , KC_SCLN  , KC_QUOT , KC_NUHS , KC_ENT           , C(KC_V)  , KC_PGUP,
      OSM_RSFT, KC_NUBS , TD_Z      , TD_X      , TD_C      , TD_V    , TD_B              , TD_N              , KC_M     , KC_COMM   , KC_DOT    , KC_SLSH                      , KC_CAPS          , KC_UP    , KC_PGDN,
      OSM_LCTL          , OSM_LGUI  , OSM_LALT                        , KC_SPC            , LT(_ALTGR, KC_SPC), A(KC_SPC)            , TG(_CODE) , OSM_LCTL                     , KC_LEFT          , KC_DOWN  , KC_RIGHT
    ),
    [_NAVIGATION] = LAYOUT(
      _______ , _______ , _______   , _______   , _______   , _______ , _______           , _______ , _______ , _______  , _______   , _______   , _______  , _______ , _______ , _______           , _______  ,
      _______ , _______ , _______   , _______   , _______   , _______ , _______           , _______           , _______  , _______   , _______   , _______  , _______           , _______           , _______  , _______,
      _______           , _______   , _______   , _______   , _______ , _______           , _______           , _______  , KC_UP     , _______   , _______  , _______ , _______ , _______                      , _______,
      _______           , _______   , _______   , _______   , _______ , _______           , KC_HOME           , KC_LEFT  , KC_DOWN   , KC_RIGHT  , KC_END   , _______ , _______ , _______           , _______  , _______,
      _______ , _______ , _______   , _______   , _______   , _______ , _______           , _______           , KC_PGUP  , KC_PGDN   , _______   , _______                      , _______           , S(KC_UP) , _______,
      _______           , _______   , _______                         , _______           , _______           , _______              , _______   , _______                      , S(KC_LEFT)        , S(KC_DOWN), S(KC_RIGHT)
    ),
    [_SELECT] = LAYOUT(
      _______ , _______ , _______   , _______   , _______   , _______ , _______           , _______ , _______ , _______  , _______   , _______   , _______  , _______ , _______ , _______           , _______  ,
      _______ , _______ , _______   , _______   , _______   , _______ , _______           , _______           , _______  , _______   , _______   , _______  , _______           , _______           , _______  , _______,
      _______           , _______   , _______   , _______   , _______ , _______           , _______           , _______  , _______   , _______   , _______  , _______ , _______ , _______                      , _______,
      _______           , _______   , _______   , _______   , _______ , _______           , _______           , _______  , _______   , _______   , _______  , _______ , _______ , _______           , _______  , _______,
      _______ , _______ , _______   , _______   , _______   , _______ , _______           , _______           , _______  , _______   , _______   , _______                      , _______           , S(KC_UP) , _______,
      _______           , _______   , _______                         , _______           , _______           , _______              , _______   , _______                      , S(KC_LEFT)        , S(KC_DOWN), S(KC_RIGHT)
    ),
    [_CODE] = LAYOUT(
      _______ , _______ , _______   , _______   , _______   , _______ , _______           , _______ , _______ , _______  , _______   , _______   , _______  , _______ , _______ , _______           , _______  ,
      KC_GRV  , _______ , _______   , _______   , _______   , _______ , _______           , _______           , _______  , _______   , _______   , _______  , _______           , _______           , _______  , _______,
      _______           , KC_Q      , KC_W      , KC_E      , KC_R    , KC_T              , KC_Y              , _______  , _______   , _______   , _______  , _______ , _______ , _______                      , _______,
      _______           , KC_A      , KC_S      , KC_D      , KC_F    , KC_G              , _______           , _______  , _______   , _______   , _______  , _______ , _______ , _______           , _______  , _______,
      _______ , _______ , KC_Z      , KC_X      , KC_C      , KC_V    , KC_B              , _______           , _______  , _______   , _______   , _______                      , _______           , _______  , _______,
      _______           , _______   , _______                         , _______           ,MT(MOD_RSFT,KC_SPC), _______              , _______   , _______                      , _______           , _______  , _______
    ),
    [_ALTGR] = LAYOUT(
      P_GITRV , _______ , MEH(KC_F2), MEH(KC_F3), MEH(KC_F4), _______ , _______           , _______ , _______ , KC_MPLY  , KC_MUTE   , KC_VOLD   , KC_VOLU  , _______ , _______ , _______           , QK_FLASH ,
      KC_GRV  , _______ , _______   , KC_NUHS   , KC_EUR    , _______ , _______           , _______           , _______  , _______   , KC_MPRV   , KC_MNXT  , _______           , _______           , _______  , _______,
      _______           , _______   , KC_ESC    , KC_ENT    , KC_BSPC , KC_TAB            , C(KC_Y)           , KC_UE    , KC_UP     , KC_OE     , P_PWD    , _______ , _______ , _______                      , _______,
      _______           , KC_AE     , KC_SS     , KC_DEL    , _______ , KC_TAB            , C(KC_Z)           , KC_LEFT  , KC_DOWN   , KC_RIGHT  , _______  , _______ , _______ , KC_NUHS           , KC_INS   , _______,
      KC_NUBS , _______ , C(KC_Z)   , C(KC_X)   , C(KC_C)   , C(KC_V) , _______           , KC_HOME           , KC_END   , KC_PGUP   , KC_PGDN   , _______                      , _______           , _______  , _______,
      _______           , _______   , KC_LALT                         , KC_ENT            , _______           , KC_ALGR              , _______   , _______                      , _______           , _______  , _______
    ),
    /*
    [_EMPTY] = LAYOUT(
        _______ , _______ , _______   , _______   , _______   , _______ , _______           , _______ , _______ , _______  , _______   , _______   , _______  , _______ , _______ , _______           , _______  ,
        _______ , _______ , _______   , _______   , _______   , _______ , _______           , _______           , _______  , _______   , _______   , _______  , _______           , _______           , _______  , _______,
        _______           , _______   , _______   , _______   , _______ , _______           , _______           , _______  , _______   , _______   , _______  , _______ , _______ , _______                      , _______,
        _______           , _______   , _______   , _______   , _______ , _______           , _______           , _______  , _______   , _______   , _______  , _______ , _______ , _______           , _______  , _______,
        _______ , _______ , _______   , _______   , _______   , _______ , _______           , _______           , _______  , _______   , _______   , _______                      , _______           , _______  , _______,
        _______           , _______   , _______                         , _______           , _______           , _______              , _______   , _______                      , _______           , _______  , _______
    )
    */
    /*
    [_NUM] = LAYOUT(
      KC_ESC, KC_F1, MEH(KC_F2), MEH(KC_F3), MEH(KC_F4), KC_F5, KC_F6, KC_F7, KC_F8, KC_MPLY, KC_MUTE, KC_VOLD, KC_VOLU, KC_PSCR, KC_SCRL, KC_MPLY, QK_BOOT,
      KC_GRV, KC_1, KC_2, KC_NUHS, ALGR(KC_4), KC_5, KC_6, KC_7, KC_8, KC_9, KC_MPRV, KC_MNXT, KC_EQL, KC_BSPC, KC_DEL, KC_HOME,
      P_LAY, A(KC_F4), KC_PSCR, C(A(KC_PSCR)), KC_BSPC, KC_TAB, C(KC_Y), KC_MINS, KC_LBRC, KC_RBRC, KC_P, KC_LBRC, KC_RBRC, KC_NUBS, KC_END,
      KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_NUHS, KC_ENT, KC_INS, KC_PGUP,
      OSM(MOD_RSFT), KC_NUBS, C(KC_Z), C(KC_X), C(KC_C), C(KC_V), KC_B, KC_EQL, KC_PLUS, KC_LCBR, KC_RCBR, KC_SLSH, KC_RSFT, KC_UP, KC_PGDN,
      OSM(MOD_LCTL), KC_LGUI, KC_LALT, KC_NO, _______, OSL(_NUM), _______, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),
    */
};

bool get_custom_auto_shifted_key(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // disable auto shift for tap dance on arrow keys
        case TD(TD_LEFT):
        case TD(TD_RIGHT):
            return false;
    }

    switch (keycode) {
        // case KC_SPC:
        //     return true;

        // enable auto shift for tap dance keys
        case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
            return true;

        // enable auto shift for german umlauts and ß
        case KC_AE:
        case KC_OE:
        case KC_UE:
        case KC_SS:
            return true;

        default:
            return false;
    }
}

void oneshot_locked_mods_changed_user(uint8_t mods) {
    clear_oneshot_locked_mods();
    unregister_mods(mods);

    // if OSM_LCTL was pressed ONESHOT_TAP_TOGGLE times, do this instead
    if (mods == MOD_BIT_LCTRL) {
        set_oneshot_mods(MOD_BIT_LSHIFT);
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uprintf("keycode: %d\n", keycode);
    uprintf("keycode: %d\n\n", P_LAY);

    bool win_mod = (get_mods() == MOD_BIT(KC_LGUI)) || (get_oneshot_mods() == MOD_BIT(KC_LGUI));

    if (record->event.pressed && IS_LAYER_ON(_SELECT)) {
        // when in _SELECT layer, stay in there until a non-arrow key is pressed
        switch (keycode) {
            case KC_LEFT:
            case KC_RIGHT:
            case KC_UP:
            case KC_DOWN:
            case S(KC_LEFT):
            case S(KC_RIGHT):
            case S(KC_UP):
            case S(KC_DOWN):
            case TD_LEFT:
                break;

            default:
                layer_off(_SELECT);
        }
    }

    if (keycode == P_PWD && record->event.pressed) {
        SEND_STRING("Somepass1");
        return false;
    }
    else if (keycode == P_GITRV && record->event.pressed) {
        send_string_P(git_revision);
        return false;
    }
    else if (win_mod) {
        switch (keycode) {
            case KC_F:
            case TD_F:
                tap_code16(LWIN(S(KC_F)));
                return false;
        }
    }
    /*
    else if (keycode == P_LAY && record->event.pressed) {
        if (IS_LAYER_ON(_CODE)) {
            tap_code16(MEH(KC_F12));
        } else {
            tap_code16(MEH(KC_F11));
        }
        return false;
    }
    */
    return true;
}

void autoshift_press_user(uint16_t keycode, bool shifted, keyrecord_t *record) {

    print("\n");
    uprintf("keycode: %d\n", keycode);

    bool l_shift_held = get_mods() & MOD_BIT(KC_LSFT);
    bool l_shift_held_osm = get_oneshot_mods() & MOD_BIT(KC_LSFT);
    bool r_shift_held = get_mods() & MOD_BIT(KC_RSFT);
    bool r_shift_held_osm = get_oneshot_mods() & MOD_BIT(KC_RSFT);

    /*
    bool l_ctrl_held = get_mods() & MOD_BIT(KC_LCTL);
    bool l_ctrl_held_osm = get_oneshot_mods() & MOD_BIT(KC_LCTL);

    uprintf("l_ctrl_held: %d\n", l_ctrl_held);
    uprintf("l_ctrl_held_osm: %d\n", l_ctrl_held_osm);
    */

    bool shift_held = l_shift_held || r_shift_held || l_shift_held_osm || r_shift_held_osm;
    if (IS_LAYER_ON(_BASE)) {
        if (shift_held) {
            //add_weak_mods(MOD_BIT(KC_LSFT));
        }
        switch (keycode) {
            //case KC_SPC: TH(KC_SPC, KC_H);
        }
    }
    else if (IS_LAYER_ON(_CODE)) {

        uprintf("shifted: %d\n", shifted);
        uprintf("shift_held: %d\n", shift_held);

        switch(keycode) {
            case KC_W: TH(KC_W, C(KC_W));
            case KC_E: TH(KC_E, KC_ENT);
            //case TD_E: TH(KC_E, KC_ENT); // does not work - outputs e after ENTER, here and via TD processing (?)
            case KC_R: TH(KC_R, KC_BSPC);
            case KC_T: TH(KC_T, KC_TAB);

            case KC_Y: TH(KC_Y, C(KC_Y));

            case KC_A: TH(KC_A, KC_ESC);
            case KC_D: TH(KC_D, KC_DEL);
            case KC_G: TH(KC_G, KC_GRV);

            case KC_Z: TH(KC_Z, C(KC_Z));
            // case TD_Z: TH(KC_Z, C(KC_Z)); // does not work - outputs zz two times, here and via TD processing (?)
            case KC_X: TH(KC_X, C(KC_X));
            case KC_C: TH(KC_C, C(KC_C));
            case KC_V: TH(KC_V, C(KC_V));

            // arroy keys
            case KC_I: TH(KC_I, KC_UP);
            case KC_J: TH(KC_J, KC_LEFT);
            case KC_K: TH(KC_K, KC_DOWN);
            case KC_L: TH(KC_L, KC_RGHT);

            case KC_H: TH(KC_H, KC_MINS);

            case KC_N: TH(KC_N, KC_HOME);
            case KC_M: TH(KC_M, KC_END);
            case KC_COMM: TH(KC_COMM, KC_PGUP);
            case KC_DOT: TH(KC_DOT, KC_PGDN);

            case KC_SPC: TH(KC_SPC, KC_RSFT);
        }
    }

    if (shifted && keycode != QK_REP) { // tried to fix that holding repeat key sends shifted version of last key
        add_weak_mods(MOD_BIT(KC_LSFT));
    }

    if ( ! IS_QK_TAP_DANCE(keycode)) {
        // & 0xFF gets the Tap key for Tap Holds, required when using Retro Shift
        register_code16((IS_RETRO(keycode)) ? keycode & 0xFF : keycode);
    }
}

void autoshift_release_user(uint16_t keycode, bool shifted, keyrecord_t *record) {
    // bool l_shift_held = get_mods() & MOD_BIT(KC_LSFT);
    // bool l_shift_held_osm = get_oneshot_mods() & MOD_BIT(KC_LSFT);
    // bool r_shift_held = get_mods() & MOD_BIT(KC_RSFT);
    // bool r_shift_held_osm = get_oneshot_mods() & MOD_BIT(KC_RSFT);

    // bool shift_held = l_shift_held || r_shift_held || l_shift_held_osm || r_shift_held_osm;

    if (IS_LAYER_ON(_BASE)) {
        switch (keycode) {
            //case KC_SPC: THU(KC_SPC, KC_H);
        }
    }
    else if (IS_LAYER_ON(_CODE)) {
        switch(keycode) {
            case KC_W: THU(KC_W, C(KC_W));
            case KC_E: THU(KC_E, KC_ENT);
            //case TD_E: THU(KC_E, KC_ENT);
            case KC_R: THU(KC_R, KC_BSPC);
            case KC_T: THU(KC_T, KC_TAB);

            case KC_Y: THU(KC_Y, C(KC_Y));

            case KC_A: THU(KC_A, KC_ESC);
            case KC_D: THU(KC_D, KC_DEL);
            case KC_G: THU(KC_G, KC_GRV);

            case KC_Z: THU(KC_Z, C(KC_Z));
            //case TD_Z: THU(KC_Z, C(KC_Z));
            case KC_X: THU(KC_X, C(KC_X));
            case KC_C: THU(KC_C, C(KC_C));
            case KC_V: THU(KC_V, C(KC_V));

            // arroy keys
            case KC_I: THU(KC_I, KC_UP);
            case KC_J: THU(KC_J, KC_LEFT);
            case KC_K: THU(KC_K, KC_DOWN);
            case KC_L: THU(KC_L, KC_RGHT);

            case KC_H: THU(KC_H, KC_MINS);

            case KC_N: THU(KC_N, KC_HOME);
            case KC_M: THU(KC_M, KC_END);
            case KC_COMM: THU(KC_COMM, KC_PGUP);
            case KC_DOT: THU(KC_DOT, KC_PGDN);

            case KC_SPC: THU(KC_SPC, KC_RSFT);
        }
    }

    // & 0xFF gets the Tap key for Tap Holds, required when using Retro Shift
    // The IS_RETRO check isn't really necessary here, always using
    // keycode & 0xFF would be fine.
    if ( ! IS_QK_TAP_DANCE(keycode)) {
        unregister_code16((IS_RETRO(keycode)) ? keycode & 0xFF : keycode);
    }
}

// needed to make sure the left OSM ctrl is registered as OSM and not a regular key press (fixes AUTO_SHIFT_MODIFIERS issue #19671)
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case OSM(MOD_LCTL):
        case KC_LCTL:
        case MOD_LCTL:
        //    return 400;
        default:
            return TAPPING_TERM;
    }
}

// store last layer to global variable
static uint32_t last_layer = _BASE;

// send MEH()ed F-Keys when switching between layers
layer_state_t layer_state_set_user(layer_state_t state) {
    if (get_highest_layer(state) == _BASE && last_layer == _CODE) {
        tap_code16(MEH(KC_F11));
    }
    else if (get_highest_layer(state) == _CODE && last_layer == _BASE) {
        tap_code16(MEH(KC_F12));
    }
    last_layer = get_highest_layer(state);
    return state;
}
