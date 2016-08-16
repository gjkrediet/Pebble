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

#include "drawing.h"
#include "obsidian.h"

GColor colortheme[3][11] = 
{
  {
    (GColor)GColorWhiteARGB8, //background
    (GColor)GColorBlackARGB8, //lugs
    (GColor)GColorBlackARGB8, //hourhand
    (GColor)GColorBlackARGB8, //minutehand 
    (GColor)GColorLightGrayARGB8, //ticks
    (GColor)GColorBlackARGB8, //weather icon en temp
    (GColor)GColorBlackARGB8, //batteryicon
    (GColor)GColorLightGrayARGB8, //circle center
    (GColor)GColorDarkGrayARGB8, //weather city
    (GColor)GColorBlackARGB8, //date
    (GColor)GColorDarkGrayARGB8, //weekday
  },
  {
    (GColor)GColorWhiteARGB8, //background
    (GColor)GColorWindsorTanARGB8, //lugs
    (GColor)GColorBlueARGB8, //hourhand
    (GColor)GColorBlueARGB8, //minutehand 
    (GColor)GColorRajahARGB8, //ticks
    (GColor)GColorBlueARGB8, //weather icon en temp
    (GColor)GColorBulgarianRoseARGB8, //batteryicond
    (GColor)GColorWindsorTanARGB8, //circle center
    (GColor)GColorBulgarianRoseARGB8, //weather city
    (GColor)GColorBlueARGB8, //date
    (GColor)GColorBulgarianRoseARGB8, //weekday
  },
  {
    (GColor)GColorRichBrilliantLavenderARGB8, //background
    (GColor)GColorVividVioletARGB8, //lugs
    (GColor)GColorBlueARGB8, //hourhand
    (GColor)GColorBlueARGB8, //minutehand 
    (GColor)GColorBrilliantRoseARGB8, //ticks
    (GColor)GColorBlueARGB8, //weather icon en temp
    (GColor)GColorBlueARGB8, //batteryicond
    (GColor)GColorVividVioletARGB8, //circle center
    (GColor)GColorBlueMoonARGB8, //weather city
    (GColor)GColorBlueARGB8, //date
    (GColor)GColorBlueMoonARGB8, //weekday
  }
};

/**
 * Draw a line with a given width.
 */
void graphics_draw_line_with_width(GContext *ctx, GPoint p0, GPoint p1, uint8_t width) {
    graphics_context_set_stroke_width(ctx, width);
    graphics_draw_line(ctx, p0, p1);
}

/**
 * Draws a bluetooth logo at a given position (top-left).
 */
void draw_bluetooth_logo(GContext *ctx, GPoint origin) {
#define BLUETOOTH_LOGO_STEP 3
    // background
    graphics_context_set_fill_color(ctx, colortheme[config.theme][THEMECOLORweather]);
    graphics_fill_rect(ctx, GRect(origin.x - 2, origin.y - 2, BLUETOOTH_LOGO_STEP * 2 + 5, BLUETOOTH_LOGO_STEP * 4 + 5),
                       2, GCornersAll);

    // logo on the inside
    graphics_context_set_antialiased(ctx, false);

    graphics_context_set_stroke_color(ctx, colortheme[config.theme][THEMECOLORweather]);
    graphics_draw_line_with_width(ctx, GPoint(origin.x + BLUETOOTH_LOGO_STEP, origin.y + 0),
                                  GPoint(origin.x + BLUETOOTH_LOGO_STEP, origin.y + 4 * BLUETOOTH_LOGO_STEP), 1);
    graphics_draw_line_with_width(ctx, GPoint(origin.x + 0, origin.y + BLUETOOTH_LOGO_STEP),
                                  GPoint(origin.x + 2 * BLUETOOTH_LOGO_STEP, origin.y + 3 * BLUETOOTH_LOGO_STEP), 1);
    graphics_draw_line_with_width(ctx, GPoint(origin.x + 0, origin.y + 3 * BLUETOOTH_LOGO_STEP),
                                  GPoint(origin.x + 2 * BLUETOOTH_LOGO_STEP, origin.y + BLUETOOTH_LOGO_STEP), 1);
    graphics_draw_line_with_width(ctx, GPoint(origin.x + BLUETOOTH_LOGO_STEP, origin.y + 0),
                                  GPoint(origin.x + 2 * BLUETOOTH_LOGO_STEP, origin.y + BLUETOOTH_LOGO_STEP), 1);
    graphics_draw_line_with_width(ctx, GPoint(origin.x + BLUETOOTH_LOGO_STEP, origin.y + 4 * BLUETOOTH_LOGO_STEP),
                                  GPoint(origin.x + 2 * BLUETOOTH_LOGO_STEP, origin.y + 3 * BLUETOOTH_LOGO_STEP), 1);
    graphics_context_set_antialiased(ctx, true);

}

/**
 * Draws a popup about the bluetooth connection
 */
void bluetooth_popup(GContext *ctx, bool connected) {

    if (!show_bluetooth_popup) return;

    int16_t yoffset = 70;
    graphics_context_set_fill_color(ctx, GColorBlack);
    GRect notification_rect = GRect(-10, height - yoffset - 7, width + 20, 50);
    graphics_fill_rect(ctx, notification_rect, 0, GCornersAll);
    graphics_context_set_fill_color(ctx, GColorWhite);
    graphics_fill_rect(ctx, GRect(-10, height - yoffset - 3, width + 20, 50 - 8), 0, GCornersAll);
    graphics_context_set_text_color(ctx, GColorBlack);
    graphics_draw_text(ctx, connected ? "Bluetooth Connected" : "Bluetooth Disconnected", font_system_18px_bold,
                       GRect(22, notification_rect.origin.y + 4, 105, 40),
                       GTextOverflowModeWordWrap, GTextAlignmentCenter,
                       NULL);
    if (connected) {
        graphics_context_set_fill_color(ctx, COLOR_FALLBACK(GColorGreen, GColorBlack));
    } else {
        graphics_context_set_fill_color(ctx, COLOR_FALLBACK(GColorRed, GColorBlack));
    }

    GPoint icon_center = GPoint(138, notification_rect.origin.y + notification_rect.size.h - 26);
    graphics_fill_circle(ctx, icon_center, 9);
    graphics_context_set_stroke_color(ctx, GColorWhite);

    if (connected) {
        graphics_draw_line_with_width(ctx,
                                      GPoint(icon_center.x + 4, icon_center.y - 3),
                                      GPoint(icon_center.x - 2, icon_center.y + 3), 2);
        graphics_draw_line_with_width(ctx,
                                      GPoint(icon_center.x - 4, icon_center.y + 0),
                                      GPoint(icon_center.x - 2, icon_center.y + 3), 2);
    } else {
        graphics_draw_line_with_width(ctx,
                                      GPoint(icon_center.x + 3, icon_center.y - 3),
                                      GPoint(icon_center.x - 3, icon_center.y + 3), 2);
        graphics_draw_line_with_width(ctx,
                                      GPoint(icon_center.x - 3, icon_center.y - 3),
                                      GPoint(icon_center.x + 3, icon_center.y + 3), 2);
    }
}

/**
 * Update procedure for the background
 */
    void background_update_proc(Layer *layer, GContext *ctx) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "drawing...");
      //bepaal vast of de buiengrafiek getoond gaat worden.
    uint8_t i;
    GRect bounds = layer_get_bounds(layer);
    int16_t radius = bounds.size.w / 2;
    bool bluetooth = bluetooth_connection_service_peek();
    BatteryChargeState battery_state = battery_state_service_peek();
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    graphics_context_set_fill_color(ctx,colortheme[config.theme][THEMECOLORbackground]);
    graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
    graphics_context_set_fill_color(ctx, colortheme[config.theme][THEMECOLORlugs]);
    if (config.layout_lugs == 1) {
      graphics_fill_rect(ctx, GRect(0, 0, width, 25), 0, GCornerNone);
      graphics_fill_rect(ctx, GRect(0, height-23, width, height), 0, GCornerNone);
    }
    else if (config.layout_lugs == 2){
      graphics_fill_circle(ctx,GPoint(width/2,-200),225);
      graphics_fill_circle(ctx,GPoint(width/2,height+200),225);
    }

      

    // compute angles
    int32_t minute_angle = t->tm_min * TRIG_MAX_ANGLE / 60;
    GPoint minute_hand = get_radial_point(radius * 65/100, minute_angle);
    int hour_tick = ((t->tm_hour % 12) * 6) + (t->tm_min / 10);
    int32_t hour_angle = hour_tick * TRIG_MAX_ANGLE / (12 * 6);
    GPoint hour_hand = get_radial_point(radius * 50/100, hour_angle);

    // format date strings 
    static const char *dagnaam[] = {
            "Zondag",
            "Maandag",
            "Dinsdag",
            "Woensdag",
            "Donderdag",
            "Vrijdag",
            "Zaterdag",
    };

   static const char *maandnaam[] = {
            "jan",
            "feb",
            "mrt",
            "apr",
            "mei",
            "juni",
            "juli",
            "aug",
            "sept",
            "okt",
            "nov",
            "dec",
    };
/*
      static const char *maandnaam[] = {
            "januari",
            "februari",
            "maart",
            "april",
            "mei",
            "juni",
            "juli",
            "augustus",
            "september",
            "oktober",
            "november",
            "december",
    };
*/
    snprintf (buffer_1,sizeof(buffer_1),"%d %s",t->tm_mday,maandnaam[t->tm_mon]);
    snprintf (buffer_2,sizeof(buffer_2),"%d",t->tm_mday);

    // determine where we can draw the date without overlap
    const int d_height = 20;
    const int d_y_start = 20;
    bool found = false;
    int d_center;
    const int border = 2;
// loop through all points and use the first one that doesn't overlap with the watch hands
//GJK deze loop aangepast zodat alleen de numerieke datum niet achter een wijzer komt
    GRect date_pos;
    GRect day_pos;

     for (i = 0; i < 30; i++) {
        if (i % 2 == 0) {
          d_center = -i/2;
        }
        else {
          d_center = (i+1)/2;
        }
        date_pos = GRect(d_center, d_y_start, width, d_height);
        GSize date_size = graphics_text_layout_get_content_size(buffer_1, font_system_18px_bold, date_pos,
                                                                GTextOverflowModeWordWrap, GTextAlignmentCenter);
        day_pos = GRect(d_center, d_y_start, width, d_height);
        if (! line2_rect_intersect(center, hour_hand, center, minute_hand,
                                   GPoint(width / 2 + d_center - date_size.w / 2 - border,
                                          d_y_start - border),
                                   GPoint(width / 2 + d_center + date_size.w / 2 + border,
                                          d_y_start + d_height + border))) {
            found = true;
            break;
        }
      }
    if (!found){
      for (i = 0; i < 80; i++) {
        if (i % 2 == 0) {
          d_center = -i/2;
        }
        else {
          d_center = (i+1)/2;
        }
        date_pos = GRect(d_center, d_y_start, width, d_height);
        GSize date_size = graphics_text_layout_get_content_size(buffer_1, font_system_24px_bold, date_pos,
                                                                GTextOverflowModeWordWrap, GTextAlignmentCenter);
        GSize dnum_size = graphics_text_layout_get_content_size(buffer_2, font_system_24px_bold, date_pos,
                                                                GTextOverflowModeWordWrap, GTextAlignmentCenter);
        day_pos = GRect(d_center, d_y_start, width, d_height);
        if (! line2_rect_intersect(center, hour_hand, center, minute_hand,
                                   GPoint(width / 2 + d_center - date_size.w / 2 - border,
                                          d_y_start - border),
                                   GPoint(width / 2 + d_center - date_size.w / 2 + dnum_size.w + border,
                                          d_y_start + d_height + border))) {
            found = true;
            break;
        }
      }
    }
    // this should not happen, but if it does, then use the default position
    if (!found) {
        d_center = 0;
    }
    date_pos = GRect(d_center, d_y_start, width, d_height);
     
    // actuallyl draw the date text
    graphics_context_set_text_color(ctx, colortheme[config.theme][THEMECOLORweekday]);
      day_pos = GRect(d_center, d_y_start+20, width, 12);
      graphics_draw_text(ctx, dagnaam[t->tm_wday], font_system_18px_bold, day_pos,
                       GTextOverflowModeWordWrap, GTextAlignmentCenter,NULL);
    graphics_context_set_text_color(ctx, colortheme[config.theme][THEMECOLORdate]);
    graphics_draw_text(ctx, buffer_1, font_system_24px_bold, date_pos,
                       GTextOverflowModeWordWrap, GTextAlignmentCenter,
                       NULL);

    // weather information
    bool weather_is_on = config.weather_refresh > 0;
    bool weather_is_available = weather.timestamp > 0;
//    APP_LOG(APP_LOG_LEVEL_DEBUG, "weather update time: %s", buffer_1);

      bool show_weather = weather_is_on && weather_is_available;
//      show_weather = true;
    found=false;
    if (show_weather || !bluetooth) {
        int temp = weather.temperature;
        if (weather.failed) {
            if (!show_weather) {
                snprintf(buffer_1, 10, "z");
            } else if (!bluetooth) {
                snprintf(buffer_1, 10, "z%c%d", weather.icon, temp);
            } else {
                snprintf(buffer_1, 10, "%c%d", weather.icon, temp);
            }
        } else {
            if (!show_weather) {
                snprintf(buffer_1, 10, "z");
            } else if (!bluetooth) {
                snprintf(buffer_1, 10, "z%c%d°", weather.icon, temp);
            } else {
            snprintf(buffer_1, 10, "%c%d°", weather.icon, temp);
            }
        }
        GRect pos;
        const int w_border = 2;
        const int w_height = 21;
        const int c_height = 9;
        const int w_x = width / 2;
        int w_y = height/2 + 17;
        int w_center;
        GSize weather_size = graphics_text_layout_get_content_size(buffer_1, font_nupe, GRect(0, 0, 300, 300),
                                                                   GTextOverflowModeWordWrap, GTextAlignmentCenter);
        // loop through all points and use the first one that doesn't overlap with the watch hands
        for (i = 0 ; i < 80; i++) {
            if (i % 2 == 0) {
              w_center = -i/2;
            }
            else {
              w_center = (i+1)/2;
            }
            if (!line2_rect_intersect(center, hour_hand, center, minute_hand,
                                      GPoint(w_x + w_center - weather_size.w / 2 - w_border,
                                             w_y - w_border),
                                      GPoint(w_x + w_center + weather_size.w / 2 + w_border,
                                             w_y + w_height + w_border))) {
                found = true;
                break;
            }
        }

        // this should not happen, but if it does, then use the default position
        if (!found) {
            w_center = 0;
        }
        pos = GRect(w_center, w_y, width, 23);
        graphics_context_set_text_color(ctx, colortheme[config.theme][THEMECOLORweather]);
        graphics_draw_text(ctx, buffer_1, font_nupe, pos, GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
        pos = GRect(w_center, w_y-c_height, width, w_height);
        graphics_context_set_text_color(ctx, colortheme[config.theme][THEMECOLORweather]);
        graphics_draw_text(ctx, weather.city, font_system_14px_bold, pos, GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
    }


    // second hand
//    GPoint second_hand = get_radial_point_basic(radius, t->tm_sec, 60);
//    graphics_context_set_stroke_width(ctx, 4);
//    graphics_context_set_stroke_color(ctx, GColorBlack);
//    graphics_draw_line(ctx, second_hand, center);
//    graphics_context_set_stroke_width(ctx, 3);
//    graphics_context_set_stroke_color(ctx, GColorWhite);
//    graphics_draw_line(ctx, second_hand, center);

    // minute hand
    graphics_context_set_stroke_color(ctx, colortheme[config.theme][THEMECOLORminutehand]);
    graphics_draw_line_with_width(ctx, minute_hand, center, config.layout_handswidth);

    // hour hand
    graphics_context_set_stroke_color(ctx, colortheme[config.theme][THEMECOLORhourhand]);
    graphics_draw_line_with_width(ctx, hour_hand, center, config.layout_handswidth);

    // circle in the center
    graphics_context_set_fill_color(ctx, colortheme[config.theme][THEMECOLORcentercircle]);
    graphics_fill_circle(ctx, center, 5);
    graphics_context_set_fill_color(ctx, colortheme[config.theme][THEMECOLORbackground]);
    graphics_fill_circle(ctx, center, 1);


    // battery status
    if (config.layout_battery == 2 ||
        (config.layout_battery == 1 && battery_state.charge_percent <= 30 && !battery_state.is_charging &&
         !battery_state.is_plugged)) {
        GRect battery = GRect((width-14)/2, 21, 14, 8);
        // determine where we can draw the bluetooth logo without overlap
        int b_center;
        const int b_x = width - 15 - battery.size.w;
        const int b_y = height/2 - battery.size.h/2;
        const int b_border = 3;
        // loop through all points and use the first one that doesn't overlap with the watch hands
        for (i = 0; i < 40; i++) {
            if (i % 2 == 0) {
              b_center=-i/2;
            }
            else {
              b_center=(i+1)/2;
            }
            if (!line2_rect_intersect(center, hour_hand, center, minute_hand,
                                      GPoint(b_x - battery.size.w/2 - b_border, b_y + b_center - b_border),
                                      GPoint(b_x + battery.size.w/2 + b_border, b_y + b_center + battery.size.h + b_border))) {
                break;
            }
        }
        battery = GRect(b_x - battery.size.w/2, b_y + b_center, battery.size.w, battery.size.h);
        GColor battery_color = colortheme[config.theme][THEMECOLORbatteryicon];
        if (battery_state.charge_percent <= 20 && !battery_state.is_charging && !battery_state.is_plugged) {
            battery_color = GColorRed;
        }
        graphics_context_set_stroke_color(ctx, colortheme[config.theme][THEMECOLORbatteryicon]);
        graphics_context_set_stroke_width(ctx, 1);
        graphics_context_set_fill_color(ctx, battery_color);
        graphics_draw_rect(ctx, battery);
        graphics_fill_rect(ctx, GRect(battery.origin.x + 2, battery.origin.y + 2, battery_state.charge_percent / 10, 4),
                           0, GCornerNone);
        graphics_draw_line_with_width(ctx, GPoint(battery.origin.x + battery.size.w, battery.origin.y + 2),
                                      GPoint(battery.origin.x + battery.size.w, battery.origin.y + 5), 1);
    }

        //buienradar
    if (config.weather_rain>0) {
          const int r_x = width/2 - 44;
          const int r_y = height-23;
          int rain_now = 60*t->tm_hour+t->tm_min;
          int rv;
          GRect gc_pos;
          gc_pos = GRect(width*2/10, r_y-17, width*6/10, 12);
          graphics_context_set_text_color(ctx, colortheme[config.theme][THEMECOLORweathercity]);
          if ((time(NULL) - radar.timestamp) >= (config.weather_expire * 60)) {
            struct tm * time_info = localtime(&weather.timestamp);
            strftime(buffer_2,7, "%H:%M " , time_info);
            strncat(buffer_2,radar.geocity,sizeof(buffer_2)-6);
            graphics_context_set_text_color(ctx, colortheme[config.theme][THEMECOLORweathercity]);
            graphics_draw_text(ctx, buffer_2, font_system_14px_bold, gc_pos, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
          }

          if (!radar.norain) {
            graphics_context_set_fill_color(ctx, GColorWhite);
            graphics_fill_rect(ctx, GRect(0, r_y+1, width, 20), 0, GCornerNone);
            graphics_context_set_fill_color(ctx, GColorDarkGray);
            graphics_fill_rect(ctx, GRect(0, r_y, width, 1), 0, GCornerNone);
//            APP_LOG(APP_LOG_LEVEL_DEBUG, "rain string/time/now: %s %d %d", radar.rain, radar.t_rain, rain_now);
            if (config.weather_rain==1) {
              for (i =0; i < 24 ; ++i){
                rv=(radar.rain[i]-97);
                if (rv>20) rv=20;
                graphics_context_set_fill_color(ctx, GColorBlue);
                graphics_fill_rect(ctx, GRect(r_x+i*4, r_y+1, 4, rv), 0, GCornerNone);
                //zet de tijdlijn
                graphics_context_set_fill_color(ctx, GColorRed);
                if (((radar.t_rain + i*5) <= rain_now &&(radar.t_rain + i*5 +5) > rain_now) || 
                   ((radar.t_rain-1440 + i*5) <= rain_now && (radar.t_rain-1435 + i*5) > rain_now)) {
                  graphics_fill_rect(ctx, GRect(r_x+i*4, r_y+3, 2, 17), 0, GCornerNone);
                }
                if (((radar.t_rain + (i-12)*5) <= rain_now &&(radar.t_rain + (i-12)*5 +5) > rain_now) || 
                   ((radar.t_rain-1440 + (i-12)*5) <= rain_now && (radar.t_rain-1435 + (i-12)*5) > rain_now)) {
                  graphics_fill_rect(ctx, GRect(r_x+i*4, r_y+3, 2, 17), 0, GCornerNone);
                }
              }
            }
            else {
              GColor rain_color;
              for (i =0; i < 24 ; ++i){
                rain_color=GColorWhite;
                rv=(radar.rain[i]-97);
  
                if (rv>0) rain_color=GColorLightGray;
//                if (rv>7) rain_color=GColorDarkGray;
                if (rv>9) rain_color=GColorBlueMoon;
                if (rv>11) rain_color=GColorBlue;
                if (rv>12) rain_color=GColorDukeBlue;
                if (rv>13) rain_color=GColorOxfordBlue;
                if (rv>15) rain_color=GColorJazzberryJam;

                graphics_context_set_fill_color(ctx, rain_color);
                graphics_fill_rect(ctx, GRect(r_x+i*4, r_y+1, 4, 20), 0, GCornerNone);
                //zet de tijdlijn
                graphics_context_set_fill_color(ctx, GColorRed);
                if (((radar.t_rain + i*5) <= rain_now &&(radar.t_rain + i*5 +5) > rain_now) || 
                   ((radar.t_rain-1440 + i*5) <= rain_now && (radar.t_rain-1435 + i*5) > rain_now)) {
                  graphics_fill_rect(ctx, GRect(r_x+i*4, r_y+3, 2, 17), 0, GCornerNone);
                }
                if (((radar.t_rain + (i-12)*5) <= rain_now &&(radar.t_rain + (i-12)*5 +5) > rain_now) || 
                   ((radar.t_rain-1440 + (i-12)*5) <= rain_now && (radar.t_rain-1435 + (i-12)*5) > rain_now)) {
                  graphics_fill_rect(ctx, GRect(r_x+i*4, r_y+3, 2, 17), 0, GCornerNone);
                }
              } 
            }
          }
      }
        // hour and minute ticks
    uint8_t tick_width = 3;
    uint8_t tick_length = 5;
    graphics_context_set_stroke_color(ctx, colortheme[config.theme][THEMECOLORticks]);
    if (config.layout_ticks != 0 && config.layout_ticks != 3) {
        for (int i = 0; i < 12; ++i) {
            if (config.layout_ticks == 1 && (i % 3) != 0) continue;

            int32_t angle = i * TRIG_MAX_ANGLE / 12;
            graphics_draw_line_with_width(ctx, get_radial_point(radius - 2, angle),
                                          get_radial_point(radius - tick_length, angle),
                                          tick_width);
        }
    }


    if (config.layout_ticks == 3 || config.layout_ticks == 4) {
        // only relevant minute ticks
        int start_min_tick = (t->tm_min / 5) * 5 + 1;
        for (int i = start_min_tick; i < start_min_tick + 4; ++i) {
            int32_t angle = i * TRIG_MAX_ANGLE / 60;
            graphics_draw_line_with_width(ctx, get_radial_point(radius - 2, angle),
                                          get_radial_point(radius - tick_length, angle), tick_width);
        }
    } else if (config.layout_ticks == 5) {
        // all minute ticks
        for (int i = 0; i < 60; ++i) {
            int32_t angle = i * TRIG_MAX_ANGLE / 60;
            graphics_draw_line_with_width(ctx, get_radial_point(radius - 2, angle),
                                          get_radial_point(radius - 3, angle), 3);
        }
    }

      
      // draw the bluetooth popup
    bluetooth_popup(ctx, bluetooth);
}
