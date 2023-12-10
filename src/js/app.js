// Import the Clay package
var Clay = require('pebble-clay');
// Load our Clay configuration file
var clayConfig = require('./config');

// Initialize Clay
//var clay = new Clay(clayConfig);
var clay = new Clay(clayConfig, null, { autoHandleEvents: false });

var MESSAGE_KEY_WeatherCity="11";
var MESSAGE_KEY_WeatherRain="10";
var MESSAGE_KEY_LocationAge="9";
var MESSAGE_KEY_WeatherApiKey="15";
var MESSAGE_KEY_WeatherProvider="16";

var data = {};
var timeout=15000;

Pebble.addEventListener('ready', function () {
    console.log('[ info/app ] PebbleKit JS ready!');
    var data = {
        "MSG_KEY_JS_READY": 1
    };
    Pebble.sendAppMessage(data);
});

Pebble.addEventListener('showConfiguration', function(e) {
  Pebble.openURL(clay.generateUrl());
});

Pebble.addEventListener('webviewclosed', function(e) {
  if (e && !e.response) {
    return;
  }
  var dict = clay.getSettings(e.response);
  localStorage.setItem('LocationAge', dict[MESSAGE_KEY_LocationAge]);
  localStorage.setItem('WeatherCity', dict[MESSAGE_KEY_WeatherCity]);
  localStorage.setItem('WeatherRain', dict[MESSAGE_KEY_WeatherRain]);
  localStorage.setItem('WeatherApiKey', dict[MESSAGE_KEY_WeatherApiKey]);
  localStorage.setItem('WeatherProvider', dict[MESSAGE_KEY_WeatherProvider]);

  // Send settings values to watch side
  Pebble.sendAppMessage(dict, function(e) {
    console.log('Sent config data to Pebble');
    console.log(JSON.stringify(dict));
  }, function(e) {
    console.log('Failed to send config data!');
    console.log(JSON.stringify(e));
  });
});

function iconBR(ic) {
  var BR_ICONS = [
    'a','aa', //onbewolkt
    'b','bb', //half bewolkt
    'c','cc', //bewolkt
    'd','dd', //Afwisselend bewolkt met (kans op) mist (/mistbanken)
    'e','ee', //Mist
    'f','ff', //Afwisselend bewolkt met (mogelijk) lichte regen
    'g','gg', //Afwisseling zon en buien met onweer
    'h','hh', //Afwisselend bewolkt met winterse neerslag
    'i','ii', //Afwisselend bewolkt met (mogelijk) lichte regen
    'k','kk', //Afwisselend bewolkt en regen
    'l','ll', //Zwaar bewolkt en pittige buien
    'm','mm', //Zwaar bewolkt met regen
    'n','nn', //Kans op gladheid
    'o','oo', //half bewolkt
    'p','pp', //Bewolkt
    'q','qq', //Bewolkt met regen
    's','ss', //Bewolkt met regen en onweer
    't','tt', //Bewolkt met sneeuw
    'u','uu', //Half bewolkt met sneeuw
    'v','vv', //Bewolkt met sneeuw
    'w','ww', //Bewolkt met natte sneeuw
    'x','xx', //Bewolkt met sneeuw
    'y','yy', //Bewolkt en mistig met lichte sneeuw
    'z','zz' //Bewolkt en mistig met sneeuw
  ];
  
  var PB_ICONS = [
/*
a zon
b wolk en zon
c wolk
d 2 wolken
e druppel
f 2 druppels
g bliksem
h sneeuwvlok
i mist
j glad?
*/
    'a', 'A', 
    'b', 'B',
    'c', 'C',
    'i', 'I',
    'i', 'I',
    'e', 'E',
    'g', 'G', //G
    'h', 'H',
    'e', 'E',
    'f', 'F', //J ontbreekt
    'f', 'F', //L
    'e', 'E',
    'j', 'J',
    'b', 'B', //O
    'c', 'C',
    'f', 'F', //R ontbreekt
    'g', 'G', //S
    'h', 'H',
    'h', 'H',
    'h', 'H',
    'h', 'H', //W
    'h', 'H',
    'h', 'H',
    'h', 'H'
  ];
  
  return(PB_ICONS[BR_ICONS.indexOf(ic)]);
}

function iconOWM(ic) {

  var OWM_ICONS = [
    '01d', '01n', // sun
    '02d', '02n', // cloud and sun
    '03d', '03n', // cloud
    '04d', '04n', // clouds
    '09d', '09n', // rain drops
    '10d', '10n', // rain drops
    '11d', '11n', // lightning
    '13d', '13n', // snow flake
    '50d', '50n' // mist
  ];
  
  var PB_ICONS = [
/*
a zon
b wolk en zon
c wolk
d 2 wolken
e druppel
f 2 druppels
g bliksem
h sneeuwvlok
i mist
j glad?
*/
    'a', 'A', 
    'b', 'B',
    'c', 'C',
    'd', 'D',
    'e', 'E',
    'f', 'F',
    'g', 'G',
    'h', 'H',
    'i', 'I'
  ];
  
  return(PB_ICONS[OWM_ICONS.indexOf(ic)]);
}

function fetchWeather(latitude, longitude) {
  
  data = {};
  
  var city=localStorage.getItem('WeatherCity');
  var provider = localStorage.getItem('WeatherProvider');
  if (latitude === 0 && longitude === 0) {
    latitude=localStorage.getItem('WeatherLatitude');      
    longitude=localStorage.getItem('WeatherLongitude');      
    console.log('Fetchweather met opgeslagen locatie ' + latitude + ',' + longitude);
  }
  console.log("provider:" + provider);
  if (provider==='1') fetchbrweather(city);
  else if (provider==='2') fetchowmweather(latitude, longitude, city);
  fetchgglcity(latitude, longitude);
  fetchbrrain(latitude, longitude);
}

function fetchgglcity(latitude, longitude) {
    // De plaatsnaam bij lat / lon
  var req = new XMLHttpRequest();
  var myAPIKey='YOURKEY';
  req.timeout=timeout;
  req.open ('GET', 'https://maps.googleapis.com/maps/api/geocode/json?latlng='+ latitude + ','+ longitude + '&key=' + myAPIKey);
  req.onload = function () {
    if (req.readyState === 4 && req.status === 200) {
      var response = JSON.parse(req.responseText);
      var geocity=response.results[0].address_components[2].long_name;
      console.log(geocity);
      data.MSG_KEY_WEATHER_GEOCITY=geocity;
    } else {
      console.log('Fout bij ophalen plaatsnaam');
      data.MSG_KEY_WEATHER_GEOCITY='onbekend';
    }
  };
  req.send(null);  
}

function fetchbrrain(latitude, longitude) {
  var rain='';
  var t_rain;
  //regenvoorspellng buienradar
  var url = 'http://gps.buienradar.nl/getrr.php?lat=' + latitude + '&lon=' + longitude;
  // Create the request
  var req = new XMLHttpRequest();
  req.timeout=timeout;
  // Specify the callback for when the request is completed
  req.open('GET', url, true);
  req.onload = function () {
    if (req.readyState === 4 && req.status === 200) {
      // The request was successfully completed!
      var index,char,a,b;
      rain = '';
      //zet de eerste datumtag om in minuten sinds afgelopen middernacht
      t_rain=parseInt(60*(this.responseText.substr(4,2)) + parseInt(this.responseText.substr(7,2)));
      for (index=0;index<25;++index) {
        a = parseInt(this.responseText.substr(index*11,3));
        b=a/10;
        if (b < 0.05) b = 0;
        if (a > 0 && b < 1) b = 1;
        if (b > 25) b = 25;
        b = Math.ceil(b);
        char='abcdefghijklmnopqrstuvwxyz'.charAt(b);
        rain = rain+char;
      }
      console.log(rain);
      console.log(t_rain);
      data.MSG_KEY_WEATHER_RAIN = rain;
      data.MSG_KEY_WEATHER_T_RAIN = t_rain;
    } else {
      console.log ('fout bij laden pagina regenradar');
    }
  };
  req.send(null);
}

function fetchowmweather(latitude, longitude, city) {
  // Haal het weer van openweathermap
  var req = new XMLHttpRequest();
  var url;
  var myAPIKey=localStorage.getItem('WeatherApiKey');
  if (city) url='http://api.openweathermap.org/data/2.5/weather?q='+ city + '&appid=' + myAPIKey + '&units=metric';
  else url='http://api.openweathermap.org/data/2.5/weather?lat=' + latitude + '&lon=' + longitude + '&appid=' + myAPIKey + '&units=metric';
  console.log(url);
  req.timeout=timeout;
  req.open ('GET', url);
  req.onload = function () {
    if (req.readyState === 4 && req.status === 200) {
      try {
        var response = JSON.parse(req.responseText);
        console.log(JSON.stringify(response));
        var temp = response.main.temp;
        var icon = response.weather[0].icon;
        icon=iconOWM(icon);
        console.log(icon);
        console.log(temp);
        console.log(city);
        data.MSG_KEY_WEATHER_ICON = icon;
        data.MSG_KEY_WEATHER_TEMP = Math.round(temp);
        data.MSG_KEY_WEATHER_CITY = city;
      }
      catch (err) {
        console.log('Fout bij parsen gegevens owm');        
      }
    } else {
      console.log('Fout bij ophalen gegevens owm');
    }
  };
  req.send(null);  
}

function fetchbrweather(city) {
    // info van buienradar
  var url = 'http://xml.buienradar.nl/';
  // Create the request
  var req = new XMLHttpRequest();
  var temp=0;
  var icon=1;
  req.timeout=timeout;
  // Specify the callback for when the request is completed
  req.open('GET', url);
  req.onload = function () {
    if (req.readyState === 4 && req.status === 200) {
      // The request was successfully completed!
      // parse de xml-output. Zoek eerst de sectie van het juiste meetstation obv stationsnaam
      var xml1; 
      var regex = new RegExp('<stationnaam regio="'+ city + '(.*?)weerstation>','i');
      try {
        xml1=req.responseText.match(regex)[1];
//      console.log(xml1);
      }
      catch (err) {
        console.log ('stad niet gevonden');
        city='Amsterdam';
        regex = new RegExp('<stationnaam regio="'+ city + '(.*?)weerstation>','i');
        xml1=req.responseText.match(regex)[1];
      }
      try {
        temp=parseInt(xml1.match(/<temperatuurGC>(.*?)<\/temperatuurGC>/)[1]);
        console.log(temp);
        xml1=xml1.match('<icoonactueel(.*?)/icoonactueel>')[1];
        console.log(xml1);
        icon=xml1.match('ID="(.*?)"')[1];
        console.log(icon);
        icon=iconBR(icon);
        console.log(icon);
        console.log(temp);
        console.log(city);
        data.MSG_KEY_WEATHER_ICON = icon;
        data.MSG_KEY_WEATHER_TEMP = Math.round(temp);
        data.MSG_KEY_WEATHER_CITY = city;
      }
      catch (err) {
        console.log ('parse xml weerpagina buienradar mislukt');
      }
    } else {
      console.log('fout bij laden xml weerpagina buienradar');
    }
  };
  req.send(null);
}

function fetchdata() {
    Pebble.sendAppMessage(data, function() {
    console.log('Message sent successfully: ' + JSON.stringify(data));
    }, function(e) {
      console.log('Message failed: ' + JSON.stringify(e));
    });
}

Pebble.addEventListener('appmessage', function (e) {
    console.log('[ info/app ] app message received: ' + JSON.stringify(e));
    var dict = e.payload;
    var location_age=localStorage.getItem('LocationAge');      
    var latitude,longitude;
    if (dict.MSG_KEY_FETCH_WEATHER) {
      navigator.geolocation.getCurrentPosition(
        function (pos) {
          latitude = pos.coords.latitude;
          longitude = pos.coords.longitude;
          console.log('Found location: ' + latitude + ',' + longitude);
          localStorage.setItem('WeatherLatitude', latitude);
          localStorage.setItem('WeatherLongitude', longitude);
          console.log('Fetchweather met ' + latitude + ',' + longitude);
          fetchWeather(latitude, longitude);
        },
        function (err) {
          console.log('[ info/app ] location request failed: ' + err);
          fetchWeather(0, 0);
        },
        {timeout: 15000, maximumAge: location_age*60000}
      );
    }
    if (dict.MSG_KEY_FETCH_DATA) {
      fetchdata();
    }
  }
);
