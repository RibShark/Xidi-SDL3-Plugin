/***************************************************************************************************
 * Xidi
 *   DirectInput interface for XInput controllers.
 ***************************************************************************************************
 * Authored by Samuel Grossman
 * Copyright (c) 2016-2026
 ***********************************************************************************************//**
 * @file PhysicalControllerBackend.h
 *   Interface declaration for a physical controller backend.
 **************************************************************************************************/

#pragma once

#include "PhysicalControllerTypes.h"
#include "PluginTypes.h"

namespace Xidi
{
  using SPhysicalControllerCapabilities = ::Xidi::Controller::SPhysicalCapabilities;
  using SPhysicalControllerState = ::Xidi::Controller::SPhysicalState;
  using SPhysicalControllerVibration = ::Xidi::Controller::SForceFeedbackState;
  using TPhysicalControllerIndex = uint16_t;

  /// Interface declaration for all plugins that implement a backend for communication with
  /// physical controllers.
  class IPhysicalControllerBackend : public IPlugin
  {
  public:

    EPluginType PluginType(void) override final
    {
      return EPluginType::PhysicalControllerBackend;
    }

    /// Retrieves and returns the maximum number of physical controllers supported by this
    /// backend. Xidi will use this number to determine how many virtual controllers to present to
    /// the application. Physical controllers are identified by index from 0 to one less than the
    /// value returned from this method.
    /// @return Maximum number of physical controllers supported by this backend. Returning 0
    /// effectively disables this backend.
    virtual TPhysicalControllerIndex MaxPhysicalControllerCount(void) = 0;

    /// Determines if the backend supports communication with the specified controller, identified
    /// by its GUID and path together in a string. If this function returns `true` then Xidi will
    /// hide it from the application during controller enumeration and instead assume that a Xidi
    /// Virtual Controller will be used to communicate with it, via this backend.
    /// @param [in] guidAndPath GUID and path string that together identify the controller.
    /// Retrieved from DirectInput using the device property `DIPROP_GUIDANDPATH`.
    /// @return `true` if the backend supports communication with the specified controller,
    /// `false` otherwise.
    virtual bool SupportsControllerByGuidAndPath(const wchar_t* guidAndPath) = 0;

    /// Retrieves and returns the physical capabilities of this backend. In this context,
    /// "physical capabilities" refers to which specific physical components of the controller
    /// this backend supports, and hence which parts of the physical controller state data
    /// structure it will actually use.
    /// @return Filled-out physical controller capability data structure. This is expected to be
    /// fixed per backend.
    virtual SPhysicalControllerCapabilities GetCapabilities(void) = 0;

    /// Reads and returns the instantaneous input state of the physical controller identified by
    /// index. The required return format is Xidi's data structure for a physical controller
    /// state. Xidi calls this method very frequently.
    /// @param [in] physicalControllerIndex Index of the physical controller whose state is to be
    /// read.
    /// @return Filled-out data structure indicating the state of the physical controller. This
    /// must include the hardware's status.
    virtual SPhysicalControllerState ReadInputState(
        TPhysicalControllerIndex physicalControllerIndex) = 0;

    /// Writes the force feedback state to the physical controller identified by index. Input
    /// format to this method is Xidi's internal force feedback state data structure.
    /// @param [in] physicalControllerIndex Index of the physical controller whose state is to be
    /// read. Xidi calls this method very frequently.
    /// @param [in] vibrationState Filled-out data structure indicating the magnitudes of the
    /// forces to be actuated to each physical force feedback actuator.
    /// @return `true` if the setting of force feedback state was successful, `false` otherwise.
    virtual bool WriteForceFeedbackState(
        TPhysicalControllerIndex physicalControllerIndex,
        SPhysicalControllerVibration vibrationState) = 0;
  };
} // namespace Xidi
