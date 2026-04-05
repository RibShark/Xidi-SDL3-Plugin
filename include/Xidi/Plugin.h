/***************************************************************************************************
 * Xidi
 *   DirectInput interface for XInput controllers.
 ***************************************************************************************************
 * Authored by Samuel Grossman
 * Copyright (c) 2016-2026
 ***********************************************************************************************//**
 * @file Plugin.h
 *   Main header file for the developers of Xidi plugins.
 **************************************************************************************************/

#pragma once

#include "PluginTypes.h"

/// Retrieves and returns the number of plugin interfaces that are implemented in this Xidi plugin
/// DLL. Each plugin DLL is identified in the configuration file as a "Plugin" entry and itself
/// may implement several plugin interfaces of the same or different types. Immediately after Xidi
/// loads this plugin DLL it will invoke this function to determine how many plugin interfaces it
/// offers. All such plugin interfaces must implement #IPlugin.
/// @return Number of plugin interfaces offered by this DLL.
extern "C" __declspec(dllexport) int __fastcall XidiPluginGetCount(void);

/// Retrieves and returns an #IPlugin interface pointer for the plugin interface of the specified
/// index. Plugin interfaces are identified by index, from 0 up to one less than the number
/// returned by #XidiPluginGetCount. The returned pointer is owned by Xidi and generally should be
/// heap-allocated. Xidi will not try to free it, but it will be cleaned up automatically when the
/// application terminates.
/// @param [in] index Zero-based index that identifies the plugin interface whose interface
/// pointer is desired.
/// @return Interface pointer to the specified interface. A return value of `nullptr` indicates an
/// error retrieving the interface pointer.
extern "C" __declspec(dllexport) ::Xidi::IPlugin* __fastcall XidiPluginGetInterface(int index);
