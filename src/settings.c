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

#include <pebble.h>
#include "settings.h"

static void update_weather_helper(void* unused);

void set_weather_timer(int timeout_min) {
    const uint32_t timeout_ms = timeout_min * 1000 * 60;
    if (weather_request_timer) {
        if (!app_timer_reschedule(weather_request_timer, timeout_ms)) {
            weather_request_timer = app_timer_register(timeout_ms, update_weather_helper, NULL);
        }
    } else {
        weather_request_timer = app_timer_register(timeout_ms, update_weather_helper, NULL);
    }
}

void request_data () {
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    dict_write_uint8(iter, MSG_KEY_FETCH_DATA, 1);
    app_message_outbox_send();
    APP_LOG(APP_LOG_LEVEL_INFO, "requesting updated data:");  
}

/**
 * Update the weather information (and schedule a periodic timer to update again)
 */
void update_weather() {
    // return if we don't want weather information
    if (config.weather_refresh == 0) return;
    const uint32_t timeout_min = (radar.norain ? config.weather_refresh : config.weather_refreshrain);
    // als er regen voorspeld is, dan vaker updaten.
//  APP_LOG(APP_LOG_LEVEL_INFO, "requesting weather update in %d minutes",timeout_min);
    set_weather_timer(timeout_min);
    data_request_timer = app_timer_register(20000, request_data, NULL); //zet data_raquest_timer op timeout (in app.js) + 5 sec.
    // actually update the weather by sending a request
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    dict_write_uint8(iter, MSG_KEY_FETCH_WEATHER, 1);
    app_message_outbox_send();
    APP_LOG(APP_LOG_LEVEL_INFO, "requesting weather update:");
}

/**
 * Utility function.
 */
static void update_weather_helper(void* unused) {
    update_weather();
}

void inbox_received_handler(DictionaryIterator *iter, void *context) {
  // Read preferences
  Tuple *tmp_tuple;
  tmp_tuple = dict_find(iter, MESSAGE_KEY_WeatherRefresh);
  if(tmp_tuple) config.weather_refresh = tmp_tuple->value->int32;
  tmp_tuple = dict_find(iter, MESSAGE_KEY_WeatherRefreshRain);
  if(tmp_tuple) config.weather_refreshrain = tmp_tuple->value->int32;
    tmp_tuple = dict_find(iter, MESSAGE_KEY_LayoutTicks);
  if(tmp_tuple) config.layout_ticks = tmp_tuple->value->int32 - 48;
  tmp_tuple = dict_find(iter, MESSAGE_KEY_WeatherExpire);
  if(tmp_tuple) config.weather_expire = tmp_tuple->value->int32;
  tmp_tuple = dict_find(iter, MESSAGE_KEY_LayoutBattery);
  if(tmp_tuple) config.layout_battery = tmp_tuple->value->int32 - 48;
  tmp_tuple = dict_find(iter, MESSAGE_KEY_LayoutLugs);
  if(tmp_tuple) config.layout_lugs = tmp_tuple->value->int32 - 48;
  tmp_tuple = dict_find(iter, MESSAGE_KEY_LayoutHandsWidth);
  if(tmp_tuple) config.layout_handswidth = tmp_tuple->value->int32;
  tmp_tuple = dict_find(iter, MESSAGE_KEY_WeatherRain);
  if(tmp_tuple) config.weather_rain = tmp_tuple->value->int32 - 48;
  tmp_tuple = dict_find(iter, MESSAGE_KEY_ColorTheme);
  if(tmp_tuple) config.theme = tmp_tuple->value->int32 - 48;
    
  persist_write_data(PERSIST_KEY_CONFIG, &config, sizeof(Config));

  bool dirty = false;
  bool ask_for_weather_update = true;


  if (dict_find(iter, MSG_KEY_WEATHER_TEMP)) {
    Tuple *icon_tuple = dict_find(iter, MSG_KEY_WEATHER_ICON);
    Tuple *temp_tuple = dict_find(iter, MSG_KEY_WEATHER_TEMP);
    Tuple *city_tuple = dict_find(iter, MSG_KEY_WEATHER_CITY);
    weather.timestamp = time(NULL);
    weather.icon = icon_tuple->value->int8;
    weather.temperature = temp_tuple->value->int8;
    memcpy(weather.city, city_tuple->value->cstring,20);
    weather.city[19]=0;
    persist_write_data(PERSIST_KEY_WEATHER, &weather, sizeof(Weather));
    dirty = true;
    ask_for_weather_update = false;
  }
  
  if (dict_find(iter, MSG_KEY_WEATHER_RAIN)) {
    Tuple *rain_tuple = dict_find(iter, MSG_KEY_WEATHER_RAIN);
    Tuple *geocity_tuple = dict_find(iter, MSG_KEY_WEATHER_GEOCITY);
    Tuple *t_rain_tuple = dict_find(iter, MSG_KEY_WEATHER_T_RAIN);
    radar.timestamp = time(NULL);
    memcpy(radar.rain, rain_tuple->value->cstring,25);
    radar.rain[24]=0;
    radar.norain = true;
    for (uint8_t i =0; i < 24 ; ++i) { if(radar.rain[i]-97 > 0) radar.norain=false;}
    memcpy(radar.geocity, geocity_tuple->value->cstring,19);
    radar.geocity[19]=0;
    radar.t_rain = t_rain_tuple->value->int16;
    radar.failed = false;
    persist_write_data(PERSIST_KEY_RADAR, &radar, sizeof(Radar));
    dirty = true;
    ask_for_weather_update = false;
  }
  
  if (dict_find(iter, MSG_KEY_WEATHER_FAILED)) {
    Tuple *failed_tuple = dict_find(iter, MSG_KEY_WEATHER_FAILED);
  // retry early when weather update failed
//    set_weather_timer(1);
    ask_for_weather_update = false;
// GJ nog te doen
  }
  if (dict_find(iter, MSG_KEY_JS_READY)) js_ready = true;
  if (dirty) layer_mark_dirty(layer_background);
  if (ask_for_weather_update) update_weather();
}

/**
 * Read all items from the configuration storage.
 */
void read_config_all() {
  config_vibrate_disconnect = true;
  config_vibrate_reconnect = true;
  config_message_disconnect = true;
  config_message_reconnect = true;
  //Hier beginnen de nieuwe
  config.layout_ticks = 3;
  config.weather_refresh = 10;
  config.weather_expire = 60;
  config.layout_battery = 1;
  config.weather_rain = 1;
  config.layout_lugs = 1;
  config.theme = 0;
  
  if (persist_exists(PERSIST_KEY_WEATHER)) {
    persist_read_data(PERSIST_KEY_WEATHER, &weather, sizeof(Weather));
  } else {
    weather.timestamp = 0;
  }
  if (persist_exists(PERSIST_KEY_RADAR)) {
    persist_read_data(PERSIST_KEY_RADAR, &radar, sizeof(Radar));
  } else {
    radar.timestamp = 0;
  }
  if (persist_exists(PERSIST_KEY_CONFIG)) {
    persist_read_data(PERSIST_KEY_CONFIG, &config, sizeof(Config));
  }
  js_ready = false;
}
