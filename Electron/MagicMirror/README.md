
The Magic Mirror focuses on a modular plugin system and uses [Electron](https://www.electronjs.org/) as a cross-platform wrapper with nodeJS for backend operations.

This repo features a fork of the base open-source application with a number of custom made modules to suit smart home,  weather, and voice activated AI integration.

The Electron application is best suited to ARM-53, ARM/A-7 (or above) based SBCs with at least 1GB of RAM. To auto-start this application, use crontab:

Open the CLI and run the following commands:

'crontab -e'
'@reboot npm start --prefix /path/to/your/MagicMirror'

## Demonstration:

#### Magic Mirror at idle with various modules:

Open Weather API for local weather information. 
In-room monitoring for temperature and humidity. 
Brewery system levels from esp32 devices via Blynk API (via load scaled HX711 signals).
OpenAI prompt for voice activated AI interaction using the "Web-Speech" API https://developer.mozilla.org/en-US/docs/Web/API/Web_Speech_API

![image](https://github.com/matthankinson93/portfolio/assets/102736018/f75014e6-2987-4af4-9a8e-7ee973e9401e)

#### Magic Mirror with returned prompt tokens from OpenAI API ('tell me how to make a black russian cocktail'):

![image](https://github.com/matthankinson93/portfolio/assets/102736018/d9877046-fe02-42ee-9c67-cadb914e831b)
