// Copyright 2015-16 Stefan Heule
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#include "obsidian.h"


////////////////////////////////////////////
//// Default values for the configuration
////////////////////////////////////////////

// defaults are also in src/obsidian.c, src/js/pebble-js-app.js and config/index.html
uint8_t config_battery_logo = 1;
uint8_t config_vibrate_disconnect = true;
uint8_t config_vibrate_reconnect = true;
uint8_t config_message_disconnect = true;
uint8_t config_message_reconnect = true;
//hier beginnen de nieuwe
Config config;

////////////////////////////////////////////
//// Global variables
////////////////////////////////////////////

/** A pointer to our window, for later deallocation. */
Window *window;

/** All layers */
Layer *layer_background;

/** Buffers for strings */
char buffer_1[30];
char buffer_2[30];

/** The center of the watch */
GPoint center;

/** The height and width of the watch */
int16_t height;
int16_t width;

#ifdef OBSIDIAN_SHOW_NUMBERS
/** Open Sans font. */
GFont font_open_sans;
#endif

/** Fonts. */
GFont font_system_14px_bold;
GFont font_system_18px_bold;
GFont font_nupe;
#ifdef PBL_ROUND
GFont font_system_24px_bold;
GFont font_system_bitham_42px_bold;
GFont font_system_bitham_30px_black;
#endif

/** Is the bluetooth popup current supposed to be shown? */
bool show_bluetooth_popup;

/** The timer for the bluetooth popup */
AppTimer *timer_bluetooth_popup;

/** The current weather information. */
Weather weather;
Radar radar;

/** Is the JS runtime ready? */
bool js_ready;

/** A timer used to schedule weather updates. */
AppTimer * weather_request_timer;
AppTimer * data_request_timer;



////////////////////////////////////////////
//// Implementation
////////////////////////////////////////////


/**
 * Handler for time ticks.
 */
void handle_second_tick(struct tm *tick_time, TimeUnits units_changed) {
    layer_mark_dirty(layer_background);
}

void timer_callback_bluetooth_popup(void *data) {
    show_bluetooth_popup = false;
    timer_bluetooth_popup = NULL;
    layer_mark_dirty(layer_background);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "bluetooth change callback");
}

void handle_bluetooth(bool connected) {
    // redraw background (to turn on/off the logo)
    layer_mark_dirty(layer_background);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "bluetooth change");

    bool show_popup = false;
    bool vibrate = false;
    if ((config_message_reconnect && connected) || (config_message_disconnect && !connected)) {
        show_popup = true;
    }
    if ((config_vibrate_reconnect && connected) || (config_vibrate_disconnect && !connected)) {
        vibrate = true;
    }

    // vibrate
    if (vibrate) {
        vibes_double_pulse();
    }

    // turn light on
    if (show_popup) {
        light_enable_interaction();
    }

    // show popup
    if (show_popup) {
        show_bluetooth_popup = true;
        if (timer_bluetooth_popup) {
            app_timer_reschedule(timer_bluetooth_popup, OBSIDIAN_BLUETOOTH_POPUP_MS);
        } else {
            timer_bluetooth_popup = app_timer_register(OBSIDIAN_BLUETOOTH_POPUP_MS, timer_callback_bluetooth_popup,
                                                       NULL);
        }
    }
}

/**
 * Window load callback.
 */
void window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    // create layer
    layer_background = layer_create(bounds);
    layer_set_update_proc(layer_background, background_update_proc);
    layer_add_child(window_layer, layer_background);

    // load fonts
#ifdef OBSIDIAN_SHOW_NUMBERS
    font_open_sans = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_OPEN_SANS_12));
#endif
    font_system_14px_bold = fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD);
    font_system_18px_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
    font_system_24px_bold = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
    font_system_bitham_42px_bold = fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD);
    font_system_bitham_30px_black = fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK);
    font_nupe = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_NUPE_23));

    // initialize
    show_bluetooth_popup = false;
}

/**
 * Window unload callback.
 */
void window_unload(Window *window) {
    layer_destroy(layer_background);
#ifdef OBSIDIAN_SHOW_NUMBERS
    fonts_unload_custom_font(font_open_sans);
#endif
    fonts_unload_custom_font(font_nupe);
}

/**
 * Initialization.
 */
void init() {
    read_config_all();
    window = window_create();
    window_set_window_handlers(window, (WindowHandlers) {
            .load = window_load,
            .unload = window_unload,
    });
    window_stack_push(window, true);

    GRect bounds = layer_get_bounds(window_get_root_layer(window));
    center = grect_center_point(&bounds);
    height = bounds.size.h;
    width = bounds.size.w;

    TimeUnits unit = MINUTE_UNIT;
  #ifdef DEBUG_ITER_COUNTER
    unit = SECOND_UNIT;
#endif
    tick_timer_service_subscribe(unit, handle_second_tick);
    // we can actually just ignore this to not waste battery.  it's fine if battery updates are 1 minute delayed
//    battery_state_service_subscribe(handle_battery);
    bluetooth_connection_service_subscribe(handle_bluetooth);

    app_message_open(OBSIDIAN_INBOX_SIZE, OBSIDIAN_OUTBOX_SIZE);
//    app_message_register_inbox_received(prv_inbox_received_handler);
    app_message_register_inbox_received(inbox_received_handler);

//    window_set_click_config_provider(window, (ClickConfigProvider) config_provider);
}

/**
 * De-initialisation.
 */
void deinit() {
    tick_timer_service_unsubscribe();
    battery_state_service_unsubscribe();
    bluetooth_connection_service_unsubscribe();

    window_destroy(window);

    app_message_deregister_callbacks();
}

/**
 * Main entry point.
 */
int main() {
    init();
    app_event_loop();
    deinit();
}
