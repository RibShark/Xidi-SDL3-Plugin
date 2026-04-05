/***************************************************************************************************
 * Xidi
 *   DirectInput interface for XInput controllers.
 ***************************************************************************************************
 * Authored by Samuel Grossman
 * Copyright (c) 2016-2026
 ***********************************************************************************************//**
 * @file PluginTypes.h
 *   Declaration of constants, types, and interfaces for Xidi plugins.
 **************************************************************************************************/

#pragma once

#include <string_view>

namespace Xidi
{
  /// Enumerates all supported plugin types.
  enum class EPluginType : unsigned int
  {
    /// Physical controller backend, implements #IPhysicalControllerBackend.
    PhysicalControllerBackend,

    /// Sentinel value, total number of enumerators.
    Count
  };

  /// Base class for all Xidi plugins.
  class IPlugin
  {
  public:

    /// Retrieves and returns the type of the plugin. Xidi uses the result of this invocation to
    /// determine the interface type for the plugin. Implemented internally by the various
    /// sub-interfaces and should not be overridden manually.
    /// @return Plugin type enumerator.
    virtual EPluginType PluginType(void) = 0;

    /// Retrieves and returns the name of this plugin, which is how users will identify it in
    /// configuration files and how Xidi will identify it in the logs. This method needs to be
    /// overridden by each concrete plugin interface implementation. A plugin's name is expected to
    /// be constant and hence the returned string must remain valid throughout the lifetime of the
    /// application.
    /// @return Name of the plugin.
    virtual std::wstring_view PluginName(void) = 0;

    /// Invoked to give this plugin an opportunity to initialize. All plugin types are allowed to
    /// initialize before they are used. As a matter of best practice, this is where any expensive
    /// initialization should occur, rather than in the constructor or the DLL entry point. That is
    /// because Xidi will unconditionally load all plugin DLL files that are requested in the
    /// configuration file and subsequently request interface pointers to all available plugin
    /// interfaces, but it will only initialize the specific plugin interfaces that it intends to
    /// use.
    /// @return `true` if initialization was successful and the plugin can be used, `false`
    /// otherwise.
    virtual bool Initialize(void) = 0;
  };

} // namespace Xidi
