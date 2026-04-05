# Xidi SDL3 Plugin
This is a plugin for [Xidi](https://github.com/samuelgr/Xidi) (versions 5.0.0 and above) that allows the usage of any controller that is supported by the [SDL3 Gamepad API](https://wiki.libsdl.org/SDL3/CategoryGamepad). This includes most popular controllers, including:
* Sony DualShock 3
* Sony DualShock 4
* Sony DualSense
* Microsoft Xbox 360 Controller
* Microsoft Xbox One Controller
* Nintendo Switch Joy-Con
* Nintendo Switch Pro Controller
* Nintendo Switch 2 Joy-Con
* Nintendo Switch 2 Pro Controller
* Steam Controller
* Google Stadia Controller
* Amazon Luna Controller
* Various 8BitDo Gamepads
* Various Logitech Gamepads
* Various Razer Gamepads

A full list of supported controllers can be found in the [SDL3 source code](https://github.com/libsdl-org/SDL/blob/main/src/joystick/SDL_gamepad_db.h).

## Usage
To use the plugin, [set up Xidi as per normal](https://github.com/samuelgr/Xidi/wiki/Getting-Started), download the [latest release of the SDL3 Plugin](https://github.com/RibShark/Xidi-SDL3-Plugin/releases/latest), and place either the "SDL.XidiPlugin.32.dll" file (for a 32-bit application), or the "SDL.XidiPlugin.64.dll" (for a 64-bit application) into the same directory as the application executable.

Xidi must then be configured to use the plugin instead of its own implementation, to do this, create a [configuration file](https://github.com/samuelgr/Xidi/wiki/Configuration) in the same directory as the application named "Xidi.ini" and add the following text to the top of the file:
````
Plugin = SDL
ControllerBackend = SDL3
````

Note that there is no INI section for this, it must be added before any INI section headers (text in \[angled brackets\] are section headers). You can leave the rest of the configuration file blank, or configure it as described in the [Xidi wiki](https://github.com/samuelgr/Xidi/wiki/Configuration).
