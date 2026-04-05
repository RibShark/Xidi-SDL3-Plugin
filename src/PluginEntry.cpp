#include <Xidi/Plugin.h>

#include "SDL3Backend.h"

static Xidi::IPlugin* plugin = new XidiSDL3Plugin::SDL3Backend();

extern "C" int __fastcall XidiPluginGetCount(void)
{
    return 1;
}

extern "C" Xidi::IPlugin* __fastcall XidiPluginGetInterface(int index)
{
    return plugin;
}