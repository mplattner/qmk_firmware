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

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _ALTGR
};

enum custom_keycodes {
    P_PWD
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BASE] = LAYOUT(
      KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_PSCR, KC_SLCK, KC_PAUS, RESET,
      KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_DEL, KC_HOME,
      KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_END,
      OSM(MOD_LCTL), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT, LCTL(KC_V), KC_PGUP,
      KC_NUBS, OSM(MOD_LSFT), KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, OSM(MOD_RSFT), KC_UP, KC_PGDN,
      OSM(MOD_LCTL), OSM(MOD_LGUI), OSM(MOD_LALT), LT(_ALTGR, KC_SPC), LT(_ALTGR, KC_SPC), TG(_ALTGR), KC_APP, OSM(MOD_RCTL), KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [_ALTGR] = LAYOUT(
      KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_MPLY, KC_MUTE, KC_VOLD, KC_VOLU, KC_PSCR, KC_SLCK, KC_PAUS, RESET,
      KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_MPRV, KC_MNXT, KC_EQL, KC_BSPC, KC_DEL, KC_HOME,
      KC_TAB, LALT(KC_F4), KC_ESC, KC_ENT, KC_BSPC, KC_TAB, KC_Y, RALT(KC_U), KC_UP, RALT(KC_O), P_PWD, KC_LBRC, KC_RBRC, KC_END,
      KC_CAPS, RALT(KC_A), RALT(KC_S), KC_DEL, LALT(KC_SPC), RALT(KC_TAB), KC_H, KC_LEFT, KC_DOWN, KC_RIGHT, KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT, KC_INS, KC_PGUP,
      KC_LSFT, KC_NUBS, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_PGUP, KC_HOME, KC_END, KC_PGDN, KC_SLSH, KC_RSFT, KC_UP, KC_PGDN,
      KC_LCTL, KC_LGUI, KC_LALT, KC_TRNS, KC_TRNS, KC_TRNS, KC_APP, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == P_PWD && record->event.pressed) {
        SEND_STRING("Somepass1");
        return false;
    }
    return true;
}
