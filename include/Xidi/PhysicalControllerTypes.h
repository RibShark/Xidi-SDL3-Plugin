/***************************************************************************************************
 * Xidi
 *   DirectInput interface for XInput controllers.
 ***************************************************************************************************
 * Authored by Samuel Grossman
 * Copyright (c) 2016-2026
 ***********************************************************************************************//**
 * @file PhysicalControllerTypes.h
 *   Declaration of constants and types used for representing physical controllers and their state.
 **************************************************************************************************/

#pragma once

#include <array>
#include <bitset>
#include <cstdint>

namespace Xidi
{
  namespace Controller
  {
    /// Maximum possible reading from an analog stick. Value based on XInput documentation.
    inline constexpr int32_t kAnalogValueMax = 32767;

    /// Minimum possible reading from an analog stick. Value derived from the above to ensure
    /// symmetry around 0.
    inline constexpr int32_t kAnalogValueMin = -kAnalogValueMax;

    /// Neutral value for an analog stick. Value computed from extreme value constants above.
    inline constexpr int32_t kAnalogValueNeutral = (kAnalogValueMax + kAnalogValueMin) / 2;

    /// Maximum possible reading for an analog trigger. Value based on XInput documentation.
    inline constexpr int32_t kTriggerValueMax = 255;

    /// Minimum possible reading for an analog trigger. Value based on XInput documentation.
    inline constexpr int32_t kTriggerValueMin = 0;

    /// Midpoint reading for an analog trigger.
    inline constexpr int32_t kTriggerValueMid = (kTriggerValueMax + kTriggerValueMin) / 2;

    /// Enumerates possible statuses for physical controller devices.
    enum class EPhysicalDeviceStatus : uint8_t
    {
      /// Device is connected and functioning correctly
      Ok,

      /// Device is not connected and has not reported an error
      NotConnected,

      /// Device has experienced an error
      Error,

      /// Sentinel value, total number of enumerators
      Count
    };

    /// Enumerates all analog sticks that might be present on a physical controller.
    /// One enumerator exists per possible stick.
    enum class EPhysicalStick : uint8_t
    {
      LeftX,
      LeftY,
      RightX,
      RightY,

      /// Sentinel value, total number of enumerators
      Count
    };

    /// Enumerates all analog triggers that might be present on a physical controller.
    /// One enumerator exists per possible trigger.
    enum class EPhysicalTrigger : uint8_t
    {
      LT,
      RT,

      /// Sentinel value, total number of enumerators
      Count
    };

    /// Enumerates all digital buttons that might be present on a physical controller. As an
    /// implementation simplification, the order of enumerators corresponds to the ordering used in
    /// XInput. One enumerator exists per possible button. Guide and Share buttons are not actually
    /// used, but they still have space allocated for them on a speculative basis.
    enum class EPhysicalButton : uint8_t
    {
      DpadUp,
      DpadDown,
      DpadLeft,
      DpadRight,
      Start,
      Back,
      LS,
      RS,
      LB,
      RB,
      Guide,
      Share,
      A,
      B,
      X,
      Y,

      /// Sentinel value, total number of enumerators
      Count
    };

    /// Enumerates the force feedback actuators present on physical controllers.
    enum class EForceFeedbackActuator : uint8_t
    {
      /// Left motor (low-frequency rumble)
      LeftMotor,

      /// Right motor (high-frequency rumble)
      RightMotor,

      /// Left impulse trigger (embedded in LT)
      LeftImpulseTrigger,

      /// Right impulse trigger (embedded in RT)
      RightImpulseTrigger,

      /// Sentinel value, total number of enumerators
      Count
    };

    /// Data format for representing the physical controller capabilities, as received from the
    /// controller backend. Used to identify the components of the Xidi physical controller data
    /// structures that the backend actually supports for a given controller.
    struct SPhysicalCapabilities
    {
      /// Whether or not the backend supports each possible analog stick, one bit per possible
      /// enumerator.
      std::bitset<static_cast<int>(EPhysicalStick::Count)> stick;

      /// Whether or not the backend supports each possible analog trigger, one bit per possible
      /// enumerator.
      std::bitset<static_cast<int>(EPhysicalTrigger::Count)> trigger;

      /// Whether or not the backend supports each possible digital, one bit per possible
      /// enumerator.
      std::bitset<static_cast<int>(EPhysicalButton::Count)> button;

      /// Whether or not the backend supports each possible force feedback actuator, one bit per
      /// possible enumerator.
      std::bitset<static_cast<int>(EForceFeedbackActuator::Count)> forceFeedbackActuator;

      constexpr bool operator==(const SPhysicalCapabilities& other) const = default;

      constexpr bool operator[](EPhysicalStick desiredStick) const
      {
        return stick[static_cast<int>(desiredStick)];
      }

      constexpr bool operator[](EPhysicalTrigger desiredTrigger) const
      {
        return trigger[static_cast<int>(desiredTrigger)];
      }

      constexpr bool operator[](EPhysicalButton desiredButton) const
      {
        return button[static_cast<int>(desiredButton)];
      }

      constexpr bool operator[](EForceFeedbackActuator desiredActuator) const
      {
        return forceFeedbackActuator[static_cast<int>(desiredActuator)];
      }

      constexpr decltype(stick)::reference operator[](EPhysicalStick desiredStick)
      {
        return stick[static_cast<int>(desiredStick)];
      }

      constexpr decltype(trigger)::reference operator[](EPhysicalTrigger desiredTrigger)
      {
        return trigger[static_cast<int>(desiredTrigger)];
      }

      constexpr decltype(button)::reference operator[](EPhysicalButton desiredButton)
      {
        return button[static_cast<int>(desiredButton)];
      }

      constexpr decltype(forceFeedbackActuator)::reference operator[](
          EForceFeedbackActuator desiredActuator)
      {
        return forceFeedbackActuator[static_cast<int>(desiredActuator)];
      }
    };

    static_assert(sizeof(SPhysicalCapabilities) <= 16, "Data structure size constraint violation.");

    /// Data format for representing physical controller state, as received from controller devices
    /// and before being passed through a mapper.
    struct SPhysicalState
    {
      /// Whether or not the physical state represented by this object was successfully read from a
      /// controller device.
      EPhysicalDeviceStatus deviceStatus;

      /// Analog stick values read from the physical controller, one element per possible stick and
      /// axis direction.
      std::array<int16_t, static_cast<int>(EPhysicalStick::Count)> stick;

      /// Analog trigger values read from the physical controller, one element per possible trigger.
      std::array<uint8_t, static_cast<int>(EPhysicalTrigger::Count)> trigger;

      /// Digital button values read from the physical controller, one element per possible digital
      /// button.
      std::bitset<static_cast<int>(EPhysicalButton::Count)> button;

      constexpr bool operator==(const SPhysicalState& other) const = default;

      constexpr int16_t operator[](EPhysicalStick desiredStick) const
      {
        return stick[static_cast<int>(desiredStick)];
      }

      constexpr uint8_t operator[](EPhysicalTrigger desiredTrigger) const
      {
        return trigger[static_cast<int>(desiredTrigger)];
      }

      constexpr bool operator[](EPhysicalButton desiredButton) const
      {
        return button[static_cast<int>(desiredButton)];
      }

      constexpr decltype(stick)::reference operator[](EPhysicalStick desiredStick)
      {
        return stick[static_cast<int>(desiredStick)];
      }

      constexpr decltype(trigger)::reference operator[](EPhysicalTrigger desiredTrigger)
      {
        return trigger[static_cast<int>(desiredTrigger)];
      }

      constexpr decltype(button)::reference operator[](EPhysicalButton desiredButton)
      {
        return button[static_cast<int>(desiredButton)];
      }
    };

    static_assert(sizeof(SPhysicalState) <= 16, "Data structure size constraint violation.");

    /// Type used to represent a force feedback effect value that can be sent to a physical
    /// actuator.
    using TForceFeedbackActuatorValue = uint16_t;

    /// Represents the states of all possible force feedback actuators on a physical controller.
    struct SForceFeedbackState
    {
      TForceFeedbackActuatorValue leftMotor;
      TForceFeedbackActuatorValue rightMotor;
      TForceFeedbackActuatorValue leftImpulseTrigger;
      TForceFeedbackActuatorValue rightImpulseTrigger;

      constexpr bool operator==(const SForceFeedbackState& other) const = default;
    };

    /// Pre-defined constant representing all analog sticks being supported in a physical controller
    /// capabilities data structure.
    inline constexpr unsigned int kPhysicalCapabilitiesAllAnalogSticks =
        ((1u << static_cast<unsigned int>(EPhysicalStick::LeftX)) |
         (1u << static_cast<unsigned int>(EPhysicalStick::LeftY)) |
         (1u << static_cast<unsigned int>(EPhysicalStick::RightX)) |
         (1u << static_cast<unsigned int>(EPhysicalStick::RightY)));

    /// Pre-defined constant representing all analog triggers being supported in a physical
    /// controller capabilities data structure.
    inline constexpr unsigned int kPhysicalCapabilitiesAllAnalogTriggers =
        ((1u << static_cast<unsigned int>(EPhysicalTrigger::LT)) |
         (1u << static_cast<unsigned int>(EPhysicalTrigger::RT)));

    /// Pre-defined constant representing the standard set of buttons, as documented in the
    /// XInput API, supported in a physical controller capabilities data structure.
    inline constexpr unsigned int kPhysicalCapabilitiesStandardXInputButtons =
        ((1u << static_cast<unsigned int>(EPhysicalButton::DpadUp)) |
         (1u << static_cast<unsigned int>(EPhysicalButton::DpadDown)) |
         (1u << static_cast<unsigned int>(EPhysicalButton::DpadLeft)) |
         (1u << static_cast<unsigned int>(EPhysicalButton::DpadRight)) |
         (1u << static_cast<unsigned int>(EPhysicalButton::Start)) |
         (1u << static_cast<unsigned int>(EPhysicalButton::Back)) |
         (1u << static_cast<unsigned int>(EPhysicalButton::LS)) |
         (1u << static_cast<unsigned int>(EPhysicalButton::RS)) |
         (1u << static_cast<unsigned int>(EPhysicalButton::LB)) |
         (1u << static_cast<unsigned int>(EPhysicalButton::RB)) |
         (1u << static_cast<unsigned int>(EPhysicalButton::A)) |
         (1u << static_cast<unsigned int>(EPhysicalButton::B)) |
         (1u << static_cast<unsigned int>(EPhysicalButton::X)) |
         (1u << static_cast<unsigned int>(EPhysicalButton::Y)));

    /// Pre-defined constant representing all buttons, even those not documented in the
    /// XInput API, supported in a physical controller capabilities data structure.
    inline constexpr unsigned int kPhysicalCapabilitiesAllButtons =
        ((1u << static_cast<unsigned int>(EPhysicalButton::DpadUp)) |
         (1u << static_cast<unsigned int>(EPhysicalButton::DpadDown)) |
         (1u << static_cast<unsigned int>(EPhysicalButton::DpadLeft)) |
         (1u << static_cast<unsigned int>(EPhysicalButton::DpadRight)) |
         (1u << static_cast<unsigned int>(EPhysicalButton::Start)) |
         (1u << static_cast<unsigned int>(EPhysicalButton::Back)) |
         (1u << static_cast<unsigned int>(EPhysicalButton::LS)) |
         (1u << static_cast<unsigned int>(EPhysicalButton::RS)) |
         (1u << static_cast<unsigned int>(EPhysicalButton::LB)) |
         (1u << static_cast<unsigned int>(EPhysicalButton::RB)) |
         (1u << static_cast<unsigned int>(EPhysicalButton::Guide)) |
         (1u << static_cast<unsigned int>(EPhysicalButton::Share)) |
         (1u << static_cast<unsigned int>(EPhysicalButton::A)) |
         (1u << static_cast<unsigned int>(EPhysicalButton::B)) |
         (1u << static_cast<unsigned int>(EPhysicalButton::X)) |
         (1u << static_cast<unsigned int>(EPhysicalButton::Y)));

    /// Pre-defined constant representing the standard set of force feedback actuators, as
    /// documented in the XInput API, supported in a physical controller capabilities data
    /// structure.
    inline constexpr unsigned int kPhysicalCapabilitiesStandardXInputForceFeedbackActuators =
        ((1u << static_cast<unsigned int>(EForceFeedbackActuator::LeftMotor)) |
         (1u << static_cast<unsigned int>(EForceFeedbackActuator::RightMotor)));

    /// Pre-defined constant representing all force feedback actuators, even those not documented in
    /// the XInput API, supported in a physical controller capabilities data structure.
    inline constexpr unsigned int kPhysicalCapabilitiesAllForceFeedbackActuators =
        ((1u << static_cast<unsigned int>(EForceFeedbackActuator::LeftMotor)) |
         (1u << static_cast<unsigned int>(EForceFeedbackActuator::RightMotor)) |
         (1u << static_cast<unsigned int>(EForceFeedbackActuator::LeftImpulseTrigger)) |
         (1u << static_cast<unsigned int>(EForceFeedbackActuator::RightImpulseTrigger)));
  } // namespace Controller
} // namespace Xidi
