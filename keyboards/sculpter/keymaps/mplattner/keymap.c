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

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _ALTGR,
    _NUM,
    _CODE
};

enum custom_keycodes {
    P_PWD
};

/*
enum tap_dances {
    TD_LSPC
};

qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_LSPC] = ACTION_TAP_DANCE_DOUBLE(KC_SPC, KC_ENT),
};
*/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BASE] = LAYOUT(
      KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_PSCR, KC_SCRL, KC_PAUS, QK_RBT,
      KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_DEL, KC_HOME,
      KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_NUBS, KC_END,
      OSM(MOD_LCTL), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT, C(KC_V), KC_PGUP,
      OSM(MOD_LSFT), KC_NUBS, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, OSM(MOD_RSFT), KC_UP, KC_PGDN,
      OSM(MOD_LCTL), OSM(MOD_LGUI), OSM(MOD_LALT), LT(_NUM, KC_SPC), LT(_ALTGR, KC_SPC), TG(_CODE), TG(_ALTGR), A(KC_SPC), KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [_ALTGR] = LAYOUT(
      KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_MPLY, KC_MUTE, KC_VOLD, KC_VOLU, KC_PSCR, KC_SCRL, KC_MPLY, QK_BOOT,
      KC_GRV, KC_1, KC_2, KC_NUHS, ALGR(KC_4), KC_5, KC_6, KC_7, KC_8, KC_9, KC_MPRV, KC_MNXT, KC_EQL, KC_BSPC, KC_DEL, KC_HOME,
      KC_TAB, A(KC_F4), KC_ESC, KC_ENT, KC_BSPC, KC_TAB, C(KC_Y), ALGR(KC_U), KC_UP, ALGR(KC_O), P_PWD, KC_LBRC, KC_RBRC, KC_NUBS, KC_END,
      OSM(MOD_LCTL), ALGR(KC_A), ALGR(KC_S), KC_DEL, C(S(KC_SPC)), KC_TAB, C(KC_Z), KC_LEFT, KC_DOWN, KC_RIGHT, KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT, KC_INS, KC_PGUP,
      OSM(MOD_LSFT), KC_NUBS, C(KC_Z), C(KC_X), C(KC_C), C(KC_V), KC_B, KC_HOME, KC_END, KC_PGUP, KC_PGDN, KC_SLSH, KC_RSFT, KC_UP, KC_PGDN,
      OSM(MOD_LCTL), KC_LGUI, KC_LALT, KC_ENT, KC_TRNS, KC_ALGR, KC_TRNS, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [_NUM] = LAYOUT(
      KC_ESC, KC_F1, MEH(KC_F2), MEH(KC_F3), MEH(KC_F4), KC_F5, KC_F6, KC_F7, KC_F8, KC_MPLY, KC_MUTE, KC_VOLD, KC_VOLU, KC_PSCR, KC_SCRL, KC_MPLY, QK_BOOT,
      KC_GRV, KC_1, KC_2, KC_NUHS, ALGR(KC_4), KC_5, KC_6, KC_7, KC_8, KC_9, KC_MPRV, KC_MNXT, KC_EQL, KC_BSPC, KC_DEL, KC_HOME,
      KC_TAB, A(KC_F4), KC_ESC, KC_ENT, KC_BSPC, KC_TAB, C(KC_Y), KC_MINS, KC_LBRC, KC_RBRC, KC_P, KC_LBRC, KC_RBRC, KC_NUBS, KC_END,
      KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_NUHS, KC_ENT, KC_INS, KC_PGUP,
      OSM(MOD_LSFT), KC_NUBS, C(KC_Z), C(KC_X), C(KC_C), C(KC_V), KC_B, KC_EQL, KC_PLUS, KC_LCBR, KC_RCBR, KC_SLSH, KC_RSFT, KC_UP, KC_PGDN,
      OSM(MOD_LCTL), KC_LGUI, KC_LALT, KC_ENT, KC_TRNS, OSL(_NUM), KC_TRNS, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [_CODE] = LAYOUT(
      KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_PSCR, KC_SCRL, KC_PAUS, QK_RBT,
      KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_DEL, KC_HOME,
      KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_NUBS, KC_END,
      OSM(MOD_LCTL), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT, C(KC_V), KC_PGUP,
      OSM(MOD_RSFT), KC_NUBS, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, OSM(MOD_RSFT), KC_UP, KC_PGDN,
      OSM(MOD_LCTL), OSM(MOD_LGUI), OSM(MOD_LALT), LT(_NUM, KC_SPC), MT(MOD_RSFT, KC_SPC), TG(_CODE), TG(_ALTGR), A(KC_SPC), KC_LEFT, KC_DOWN, KC_RGHT
    ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == P_PWD && record->event.pressed) {
        SEND_STRING("Somepass1");
        return false;
    }
    return true;
}

void autoshift_press_user(uint16_t keycode, bool shifted, keyrecord_t *record) {

    print("\n");
    uprintf("keycode: %d\n", keycode);

    bool l_shift_held = get_mods() & MOD_BIT(KC_LSFT);
    bool l_shift_held_osm = get_oneshot_mods() & MOD_BIT(KC_LSFT);
    bool r_shift_held = get_mods() & MOD_BIT(KC_RSFT);
    bool r_shift_held_osm = get_oneshot_mods() & MOD_BIT(KC_RSFT);

    bool l_ctrl_held = get_mods() & MOD_BIT(KC_LCTL);
    bool l_ctrl_held_osm = get_oneshot_mods() & MOD_BIT(KC_LCTL);

    uprintf("l_ctrl_held: %d\n", l_ctrl_held);
    uprintf("l_ctrl_held_osm: %d\n", l_ctrl_held_osm);

    bool shift_held = l_shift_held || r_shift_held || l_shift_held_osm || r_shift_held_osm;
    if (IS_LAYER_ON(_CODE)) {

        uprintf("shifted: %d\n", shifted);
        uprintf("shift_held: %d\n", shift_held);

        switch(keycode) {
            case KC_Q: TH(KC_Q, C(KC_W));
            case KC_W: TH(KC_W, KC_ESC);
            case KC_E: TH(KC_E, KC_ENT);
            case KC_R: TH(KC_R, KC_BSPC);
            case KC_T: TH(KC_T, KC_TAB);

            case KC_Y: TH(KC_Y, C(KC_Y));

            case KC_D: TH(KC_D, KC_DEL);
            case KC_G: TH(KC_G, KC_GRV);

            case KC_Z: TH(KC_Z, C(KC_Z));
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

    if (shifted) {
        add_weak_mods(MOD_BIT(KC_LSFT));
    }
    // & 0xFF gets the Tap key for Tap Holds, required when using Retro Shift
    register_code16((IS_RETRO(keycode)) ? keycode & 0xFF : keycode);
}

void autoshift_release_user(uint16_t keycode, bool shifted, keyrecord_t *record) {
    // bool l_shift_held = get_mods() & MOD_BIT(KC_LSFT);
    // bool l_shift_held_osm = get_oneshot_mods() & MOD_BIT(KC_LSFT);
    // bool r_shift_held = get_mods() & MOD_BIT(KC_RSFT);
    // bool r_shift_held_osm = get_oneshot_mods() & MOD_BIT(KC_RSFT);

    // bool shift_held = l_shift_held || r_shift_held || l_shift_held_osm || r_shift_held_osm;

    if (IS_LAYER_ON(_CODE)) {
        switch(keycode) {
            case KC_Q: THU(KC_Q, C(KC_W));
            case KC_W: THU(KC_W, KC_ESC);
            case KC_E: THU(KC_E, KC_ENT);
            case KC_R: THU(KC_R, KC_BSPC);
            case KC_T: THU(KC_T, KC_TAB);

            case KC_Y: THU(KC_Y, C(KC_Y));

            case KC_D: THU(KC_D, KC_DEL);
            case KC_G: THU(KC_G, KC_GRV);

            case KC_Z: THU(KC_Z, C(KC_Z));
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
    unregister_code16((IS_RETRO(keycode)) ? keycode & 0xFF : keycode);
}

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  //debug_enable=true;
  //debug_matrix=true;
  //debug_keyboard=true;
}

// needed to make sure the left OSM ctrl is registered as OSM and not a regular key press (fixes AUTO_SHIFT_MODIFIERS issue #19671)
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case OSM(MOD_LCTL):
        case KC_LCTL:
            return 300;
        default:
            return TAPPING_TERM;
    }
}
