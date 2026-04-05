#include <vector>
#include <SDL3/SDL.h>

#include "SDL3Backend.h"

namespace XidiSDL3Plugin
{
    using namespace Xidi;

    SDL_JoystickID* gamepadIDs = nullptr;
    std::vector<SDL_Gamepad*> gamepads = {};
    int gamepadCount = 0;

    std::wstring_view SDL3Backend::PluginName()
    {
        return L"SDL3";
    }

    bool SDL3Backend::Initialize()
    {
        if (!SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1"))
            return false;
        if (!SDL_Init(SDL_INIT_GAMEPAD | SDL_INIT_HAPTIC))
            return false;

        if (gamepadIDs = SDL_GetGamepads(&gamepadCount); gamepadIDs == nullptr)
            return false;

        gamepads.resize(gamepadCount);
        for (auto i = 0; i < gamepadCount; i++)
        {
            SDL_Gamepad* gp = SDL_OpenGamepad(gamepadIDs[i]);
            if ( gp == nullptr )
                ; // output error when ability added;

            gamepads[i] = gp;
        }

        return true;
    }

    TPhysicalControllerIndex SDL3Backend::MaxPhysicalControllerCount()
    {
        return gamepadCount;
    }

    bool SDL3Backend::SupportsControllerByGuidAndPath(const wchar_t* guidAndPath)
    {
        bool isGamepad = false;

        Uint16 vendorID;
        Uint16 productID;

        if (swscanf_s(guidAndPath, LR"(\\?\hid#vid_%hx&pid_%hx%*s)", &vendorID, &productID) != 2)
            return false;

        for (int i = 0; i < gamepadCount; i++)
        {
            if (vendorID == SDL_GetGamepadVendorForID(gamepadIDs[i]) &&
                productID == SDL_GetGamepadProductForID(gamepadIDs[i]))
                isGamepad = true;
        }

        return isGamepad;
    }

    SPhysicalControllerCapabilities SDL3Backend::GetCapabilities()
    {
        /* right now Xidi provides no way to alter this per-controller, so we claim to support everything */
        return {
            .stick = Controller::kPhysicalCapabilitiesAllAnalogSticks,
            .trigger = Controller::kPhysicalCapabilitiesAllAnalogTriggers,
            .button = Controller::kPhysicalCapabilitiesAllButtons,
            .forceFeedbackActuator = Controller::kPhysicalCapabilitiesAllForceFeedbackActuators
        };
    }

    SPhysicalControllerState SDL3Backend::ReadInputState(TPhysicalControllerIndex physicalControllerIndex)
    {
        SDL_UpdateGamepads();
        SDL_Gamepad* gp = gamepads[physicalControllerIndex];

        if (gp == nullptr)
            return {.deviceStatus = Controller::EPhysicalDeviceStatus::Error};

        if (!SDL_GamepadConnected(gp))
            return {.deviceStatus = Controller::EPhysicalDeviceStatus::NotConnected};

        SPhysicalControllerState state = {
            .deviceStatus = Controller::EPhysicalDeviceStatus::Ok,
            .stick = {
                /* vertical axes in SDL3 are inverted compared to what Xidi expects,
                 * and all axes are not clamped to -32767) so we need to do it manually here */
                std::max<Sint16>(SDL_GetGamepadAxis(gp, SDL_GAMEPAD_AXIS_LEFTX), -32767),
                static_cast<int16_t>(-std::max<Sint16>(SDL_GetGamepadAxis(gp, SDL_GAMEPAD_AXIS_LEFTY), -32767)),
                std::max<Sint16>(SDL_GetGamepadAxis(gp, SDL_GAMEPAD_AXIS_RIGHTX), -32767),
                static_cast<int16_t>(-std::max<Sint16>(SDL_GetGamepadAxis(gp, SDL_GAMEPAD_AXIS_RIGHTY), -32767))
            },
            .trigger = {
                /* Xidi expects triggers from 0-255, where SDL expects 0-32767, so integer divide by 128 */
                static_cast<uint8_t>(SDL_GetGamepadAxis(gp, SDL_GAMEPAD_AXIS_LEFT_TRIGGER) / 128),
                static_cast<uint8_t>(SDL_GetGamepadAxis(gp, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) / 128)
            },
            .button = [&]() -> std::bitset<16>
            {
                /* this lambda is annoying but bitset doesn't support brace initialization, don't know of a better
                 * way to do this */
                std::bitset<16> button;

                button[static_cast<uint8_t>(Controller::EPhysicalButton::DpadUp)] =
                    SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_DPAD_UP);
                button[static_cast<uint8_t>(Controller::EPhysicalButton::DpadDown)] =
                    SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_DPAD_DOWN);
                button[static_cast<uint8_t>(Controller::EPhysicalButton::DpadLeft)] =
                    SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_DPAD_LEFT);
                button[static_cast<uint8_t>(Controller::EPhysicalButton::DpadRight)] =
                    SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_DPAD_RIGHT);
                button[static_cast<uint8_t>(Controller::EPhysicalButton::Start)] =
                    SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_START);
                button[static_cast<uint8_t>(Controller::EPhysicalButton::Back)] =
                    SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_BACK);
                button[static_cast<uint8_t>(Controller::EPhysicalButton::LS)] =
                    SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_LEFT_STICK);
                button[static_cast<uint8_t>(Controller::EPhysicalButton::RS)] =
                    SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_RIGHT_STICK);
                button[static_cast<uint8_t>(Controller::EPhysicalButton::LB)] =
                    SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER);
                button[static_cast<uint8_t>(Controller::EPhysicalButton::RB)] =
                    SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER);
                button[static_cast<uint8_t>(Controller::EPhysicalButton::Guide)] =
                    SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_GUIDE);
                button[static_cast<uint8_t>(Controller::EPhysicalButton::Share)] =
                    /* games usually map the same control to share and touchpad, so use touchpad if available, otherwise
                    * use misc1 (which is share on Xbox controllers) */
                    SDL_GamepadHasButton(gp, SDL_GAMEPAD_BUTTON_TOUCHPAD) ?
                        SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_TOUCHPAD) :
                        SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_MISC1);
                button[static_cast<uint8_t>(Controller::EPhysicalButton::A)] =
                    SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_SOUTH);
                button[static_cast<uint8_t>(Controller::EPhysicalButton::B)] =
                    SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_EAST);
                button[static_cast<uint8_t>(Controller::EPhysicalButton::X)] =
                    SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_WEST);
                button[static_cast<uint8_t>(Controller::EPhysicalButton::Y)] =
                    SDL_GetGamepadButton(gp, SDL_GAMEPAD_BUTTON_NORTH);

                return button;
            }()
        };

        return state;
    }

    bool SDL3Backend::WriteForceFeedbackState(TPhysicalControllerIndex physicalControllerIndex,
        SPhysicalControllerVibration vibrationState)
    {
        SDL_Gamepad* gp = gamepads[physicalControllerIndex];
        if (gp == nullptr)
            return false;

        SDL_PropertiesID properties = SDL_GetGamepadProperties(gp);

        if (SDL_GetBooleanProperty(properties, SDL_PROP_GAMEPAD_CAP_RUMBLE_BOOLEAN, false))
            SDL_RumbleGamepad(gp, vibrationState.leftMotor, vibrationState.rightMotor, 250);

        if (SDL_GetBooleanProperty(properties, SDL_PROP_GAMEPAD_CAP_TRIGGER_RUMBLE_BOOLEAN, false))
            SDL_RumbleGamepadTriggers(gp, vibrationState.leftImpulseTrigger, vibrationState.rightImpulseTrigger, 250);

        SDL_UpdateGamepads();
        return true;
    }
}
