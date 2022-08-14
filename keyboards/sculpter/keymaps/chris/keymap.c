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
    _QWERTY,
    _NUMERAL,
};

enum custom_keys {
  BASE_LCTL,
  BASE_LGUI,
};

static bool shift_held = false;


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BASE] = LAYOUT(
      KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, XXXXXXX, KC_SLCK, KC_PAUS, RESET,
      KC_DLR, KC_AMPR, KC_LBRC, KC_LCBR, KC_RCBR, KC_LPRN, KC_EQUAL, KC_ASTR, KC_RPRN, KC_PLUS, KC_RBRC, KC_EXLM, KC_HASH, KC_BSPC, KC_DEL, KC__VOLUP,
      KC_TAB, KC_SCOLON, KC_COMMA, KC_DOT, KC_P, KC_Y, KC_F, KC_G, KC_C, KC_R, KC_L, KC_SLASH, KC_AT, KC_BSLASH, KC__VOLDOWN,
      TT(_NUMERAL), KC_A, KC_O, KC_E, KC_U, KC_I, KC_D, KC_H, KC_T, KC_N, KC_S, KC_MINUS, KC_ENT, XXXXXXX, XXXXXXX,
      KC_LSFT, KC_QUOT, KC_Q, KC_J, KC_K, KC_X, KC_B, KC_M, KC_W, KC_V, KC_Z, KC_RSFT, KC_UP, XXXXXXX,
      BASE_LCTL, KC_LALT, BASE_LGUI, KC_SPC, KC_SPC, KC_RALT, KC_APP, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [_QWERTY] = LAYOUT(
      KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_PSCR, KC_SLCK, KC_PAUS, XXXXXXX,
      KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_DEL, KC_HOME,
      KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS, KC_END,
      KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT, KC_INS, KC_PGUP,
      KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_UP, KC_PGDN,
      KC_LCTL, KC_LALT, KC_LGUI, KC_SPC, KC_SPC, KC_RALT, KC_APP, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [_NUMERAL] = LAYOUT(
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_8, KC_BSPC,
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_7, KC_9,
      _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_4, KC_5, KC_6,
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_1, KC_2, KC_3,
      XXXXXXX, XXXXXXX, XXXXXXX, KC_SPC, KC_SPC, XXXXXXX, XXXXXXX, KC_KP_PLUS, KC_KP_MINUS, KC_KP_SLASH, KC_KP_ASTERISK
    ),
};

void base_lctl_handler(keyrecord_t *record) {
    if(record->event.pressed) {
        register_mods(MOD_LCTL);
        layer_on(_QWERTY);
    } else {
        unregister_mods(MOD_LCTL);
        layer_off(_QWERTY);
    }
}

void base_lgui_handler(keyrecord_t *record) {
    // static uint16_t timer;
    if(record->event.pressed) {
        // timer = timer_read();
        register_mods(MOD_LGUI);
        layer_on(_QWERTY);
    } else {
        unregister_mods(MOD_LGUI);
        layer_off(_QWERTY);
        // if (timer_elapsed(timer) < TAPPING_TERM) {
          // Can make the command button a tappable key also
        // }
    }
}

void handle_custom_shift_key(uint16_t keycode, keyrecord_t *record, bool unshift) {
  // Because we have shift key pressed to activate this layer, we can't simply
  // just send the keycode, because it may be a shiftable key.
  // For example, if we press shift to activate the Programmer Dvorak numbers,
  // and send the scancode for 5, the OS will still have the shift flag activated.
  // Therefore, for some scan codes, such as 5, we must temporarily unregister
  // the shift modifier whilst we send the code, so that the shifted value is not
  // mapped by the OS, in the case of the scan code for 5, the shifted variant (in ANSI US
  // layout at least) will be percentage.
  
  if (record->event.pressed) {
      if(unshift) {
            unregister_mods(MOD_LSFT);
      }
      register_code(keycode);
  } else {
      if(unshift) {
          register_mods(MOD_LSFT);
      }
      unregister_code(keycode);
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    if (shift_held) {
      if(layer_state == _BASE) {
        // Handle the special shifted keys in Programmer Dvorak
        switch(keycode)
        {
          case KC_DLR:
            handle_custom_shift_key(KC_TILDE, record, false);
            return false;
          case KC_AMPR:
            handle_custom_shift_key(KC_PERC, record, false);
            return false;
          case KC_LBRC:
            handle_custom_shift_key(KC_7, record, true);
            return false;
          case KC_LCBR:
            handle_custom_shift_key(KC_5, record, true);
            return false;
          case KC_RCBR:
            handle_custom_shift_key(KC_3, record, true);
            return false;
          case KC_LPRN:
            handle_custom_shift_key(KC_1, record, true);
            return false;
          case KC_EQUAL:
            handle_custom_shift_key(KC_9, record, true);
            return false;
          case KC_ASTR:
            handle_custom_shift_key(KC_0, record, true);
            return false;
          case KC_RPRN:
            handle_custom_shift_key(KC_2, record, true);
            return false;
          case KC_PLUS:
            handle_custom_shift_key(KC_4, record, true);
            return false;
          case KC_RBRC:
            handle_custom_shift_key(KC_6, record, true);
            return false;
          case KC_EXLM:
            handle_custom_shift_key(KC_8, record, true);
            return false;
          case KC_HASH:
            handle_custom_shift_key(KC_GRAVE, record, true);
            return false;
          case KC_AT:
            handle_custom_shift_key(KC_CIRC, record, false);
            return false;
        }
      }
    }


    switch (keycode) {
      case KC_LSFT:
        shift_held = record->event.pressed;
        return true;
      case BASE_LCTL:
        base_lctl_handler(record);
        return false;
      case BASE_LGUI:
        base_lgui_handler(record);
        return false;
    }
    return true;
}


    // [_QWERTY] = LAYOUT(
    //   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    //   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    //   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    //   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    //   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    //   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
    // )