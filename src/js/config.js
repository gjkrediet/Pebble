module.exports = [
  {
    "type": "heading",
    "defaultValue": "App Configuration"
  },
  {
    "type": "text",
    "defaultValue": "Instellingen Buienradar Pebble Wijzerplaat Gertjan Krediet"
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Kleuren"
      },
      {
      "type": "select",
      "messageKey": "MESSAGE_KEY_ColorTheme",
      "defaultValue": 0,
      "label": "Kleur thema",
      "options": [
      { 
        "label": "zwart/wit",
        "value": 0
      },
      { 
        "label": "bruin/blauw",
        "value": 1
      },
      { 
        "label": "paars/blauw",
        "value": 2
      }
      ]
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Layout"
      },
      {
      "type": "select",
      "messageKey": "MESSAGE_KEY_LayoutTicks",
      "defaultValue": 3,
      "label": "Layout Ticks",
      "options": [
      { 
        "label": "geen",
        "value": 0
      },
      { 
        "label": "uren 3,6,9,12",
        "value": 1
      },
      { 
        "label": "alle uren",
        "value": 2
      },
      { 
        "label": "relevante minuten",
        "value": 3
      },
      { 
        "label": "alle uren + relevante minuten",
        "value": 4
      },
      { 
        "label": "alle uren + alle minuten",
        "value": 5
      }
      ]
      },
      {
      "type": "select",
      "messageKey": "MESSAGE_KEY_LayoutLugs",
      "defaultValue": 3,
      "label": "Layout Lugs",
      "options": [
      { 
        "label": "geen",
        "value": 0
      },
      { 
        "label": "rechthoekig",
        "value": 1
      },
      { 
        "label": "rond",
        "value": 2
      }
      ]
      },
    {
      "type": "slider",
      "messageKey": "MESSAGE_KEY_LayoutHandsWidth",
      "defaultValue": "4",
      "min": "1",
      "max": "6",
      "step": "1",
      "label": "Dikte wijzers"
    },
      {
      "type": "select",
      "messageKey": "MESSAGE_KEY_LayoutBattery",
      "defaultValue": 1,
      "label": "Batterij Icoon",
      "options": [
      { 
        "label": "nooit tonen",
        "value": 0
      },
      { 
        "label": "alleen als de batterij bijna leeg is",
        "value": 1
      },
      { 
        "label": "altijd tonen",
        "value": 2
      }
      ]
      },
    {
      "type": "select",
      "messageKey": "MESSAGE_KEY_WeatherRain",
      "defaultValue": "1",
      "label": "Buienradar",
      "options": [
        { 
        "label": "Toon niets",
        "value": "0" 
      },
        { 
        "label": "Toon grafiek",
        "value": "1" 
      },
        { 
        "label": "Toon lint",
        "value": "2" 
      }
      ]
    },
    ]
  },
  {
    "type": "section",
    "items": [
    {
      "type": "heading",
      "defaultValue": "Weer"
    },
    {
      "type": "input",
      "messageKey": "MESSAGE_KEY_WeatherCity",
      "defaultValue": "Amsterdam,nl",
      "label": "Plaats station (Voor buienradar kijk op www.buienradar.nl/nederland/weerbericht/weerstations)"
    },
      {
      "type": "select",
      "messageKey": "MESSAGE_KEY_WeatherProvider",
      "defaultValue": 1,
      "label": "Weer Provider",
      "options": [
      { 
        "label": "Buienradar (altijd plaats opgeven)",
        "value": 1
      },
      { 
        "label": "OpenWeather",
        "value": 2
      }
      ]
    },
    {
      "type": "input",
      "messageKey": "MESSAGE_KEY_WeatherApiKey",
      "defaultValue": "",
      "label": "de API Key van de provider"
    },
    {
      "type": "slider",
      "messageKey": "MESSAGE_KEY_WeatherRefresh",
      "defaultValue": "10",
      "min": "1",
      "max": "60",
      "step": "1",
      "label": "ververs frequentie zonder regen"
    },
    {
      "type": "slider",
      "messageKey": "MESSAGE_KEY_WeatherRefreshRain",
      "defaultValue": "5",
      "min": "1",
      "max": "60",
      "step": "2",
      "label": "ververs frequentie met regen"
    },
    {
      "type": "slider",
      "messageKey": "MESSAGE_KEY_WeatherExpire",
      "defaultValue": "0",
      "min": "0",
      "max": "240",
      "step": "10",
      "label": "Laatste update tijd tonen na aantal minuten"
    },
    {
      "type": "slider",
      "messageKey": "MESSAGE_KEY_LocationAge",
      "defaultValue": "0",
      "min": "1",
      "max": "60",
      "label": "Vervaltijd locatiebepaling telefoon"
    },
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Opslaan"
  }
];
