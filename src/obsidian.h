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

#ifndef OBSIDIAN_OBSIDIAN_H
#define OBSIDIAN_OBSIDIAN_H

#include <pebble.h>

// I don't know how to pass parameters to the compiler, so I'm using this file
// for various configurations
#include "config.h"

#include "settings.h"
#include "drawing.h"
#include "geometry.h"

//#define DEBUG_DATE_POSITION
//#define DEBUG_WEATHER_POSITION

////////////////////////////////////////////
//// Configuration constants
////////////////////////////////////////////

// config keys are also duplicated in src/obsidian.h, appinfo.json, src/js/pebble-js-app.js and config/index.html
#define MESSAGE_KEY_WeatherLatitude 1
#define MESSAGE_KEY_WeatherLongitude 2
//
#define MESSAGE_KEY_WeatherRefresh 4
#define MESSAGE_KEY_WeatherRefreshRain 5
#define MESSAGE_KEY_LayoutBattery 6
#define MESSAGE_KEY_LayoutTicks 7
#define MESSAGE_KEY_WeatherExpire 8
#define MESSAGE_KEY_LocationAge 9
#define MESSAGE_KEY_WeatherRain 10
#define MESSAGE_KEY_WeatherCity 11
#define MESSAGE_KEY_ColorTheme 12
#define MESSAGE_KEY_LayoutLugs 13
#define MESSAGE_KEY_LayoutHandsWidth 14
#define MESSAGE_KEY_WeatherApiKey 15
#define MESSAGE_KEY_WeatherProvider 16

// message keys (also duplicated in appinfo.json)
#define MSG_KEY_WEATHER_TEMP 100
#define MSG_KEY_WEATHER_ICON 101
#define MSG_KEY_FETCH_WEATHER 102
#define MSG_KEY_JS_READY 103
#define MSG_KEY_WEATHER_FAILED 104
#define MSG_KEY_WEATHER_CITY 105
#define MSG_KEY_WEATHER_RAIN 106
#define MSG_KEY_WEATHER_T_RAIN 107
#define MSG_KEY_WEATHER_GEOCITY 108
#define MSG_KEY_FETCH_DATA 109

// persitant storage keys (in additiona to config keys above)
#define PERSIST_KEY_WEATHER 201
#define PERSIST_KEY_CONFIG 202
#define PERSIST_KEY_RADAR 203

#define THEMECOLORbackground 0
#define THEMECOLORlugs 1
#define THEMECOLORhourhand 2
#define THEMECOLORminutehand 3 
#define THEMECOLORticks 4
#define THEMECOLORweather 5
#define THEMECOLORbatteryicon 6
#define THEMECOLORcentercircle 7
#define THEMECOLORweathercity 8
#define THEMECOLORdate 9
#define THEMECOLORweekday 10

////////////////////////////////////////////
//// Configuration values
////////////////////////////////////////////

extern uint8_t config_vibrate_disconnect;
extern uint8_t config_vibrate_reconnect;
extern uint8_t config_message_disconnect;
extern uint8_t config_message_reconnect;
//hier beginnen de nieuwe

////////////////////////////////////////////
//// Global variables
////////////////////////////////////////////

extern Window *window;
extern Layer *layer_background;
extern char buffer_1[30];
extern char buffer_2[30];
extern GPoint center;
extern int16_t height;
extern int16_t width;
extern bool norain;

#ifdef OBSIDIAN_SHOW_NUMBERS
/** Open Sans font. */
extern GFont font_open_sans;
#endif
extern GFont font_system_14px_bold;
extern GFont font_system_18px_bold;
extern GFont font_nupe;
#ifdef PBL_ROUND
extern GFont font_system_24px_bold;
extern GFont font_system_bitham_42px_bold;
extern GFont font_system_bitham_30px_black;
#endif
extern bool show_bluetooth_popup;
extern AppTimer *timer_bluetooth_popup;

#if defined(DEBUG_WEATHER_POSITION) || defined(DEBUG_DATE_POSITION)
#define DEBUG_ITER_COUNTER
#endif
#ifdef DEBUG_ITER_COUNTER
extern int debug_iter;
#endif

typedef struct {
    time_t timestamp;
    bool failed;
    int8_t icon;
    int8_t temperature;
    char city[20];
} __attribute__((__packed__)) Weather;

typedef struct {
    time_t timestamp;
    bool failed;
    char rain[25];
    char geocity[20];
    int16_t t_rain;
    bool norain;
} __attribute__((__packed__)) Radar;

typedef struct {
uint8_t weather_refresh;
uint8_t weather_refreshrain;
uint8_t layout_ticks;
uint8_t weather_expire;
uint8_t layout_battery;
uint8_t weather_rain;
uint8_t layout_lugs;
uint8_t layout_handswidth;
uint8_t location_age;
uint8_t theme;
} __attribute__((__packed__)) Config;

extern Weather weather;
extern Radar radar;
extern Config config;
extern bool js_ready;
extern AppTimer * weather_request_timer;
extern AppTimer * data_request_timer;

  
////////////////////////////////////////////
//// Static configuration and useful macros
////////////////////////////////////////////

#define COLOR(c) ((GColor8) { .argb = (c) })

//#define OBSIDIAN_SHOW_NUMBERS
#define OBSIDIAN_LONG_TICKS
#define OBSIDIAN_FAT_TICKS
//#define OBSIDIAN_ONLY_RELEVANT_NUMBER
//#define OBSIDIAN_BATTERY_USE_TEXT

#define OBSIDIAN_BLUETOOTH_POPUP_MS 5000

// 100 should be plenty
#define OBSIDIAN_OUTBOX_SIZE 256
#define OBSIDIAN_N_CONFIG CONFIG_END_MARKER
// 100 + an upper bound for all the configuration items we have
#define OBSIDIAN_INBOX_SIZE 256


#endif //OBSIDIAN_OBSIDIAN_H
