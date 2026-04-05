#ifndef XIDI_SDL3_PLUGIN_SDL3BACKEND_H
#define XIDI_SDL3_PLUGIN_SDL3BACKEND_H

#include <Xidi/PhysicalControllerBackend.h>

#define STRING_SIZE(s) ((sizeof(s) / sizeof(L"")) - 1)

namespace XidiSDL3Plugin
{
    using namespace Xidi;

    constexpr wchar_t VENDORID_STR[] = L"vid_";
    constexpr wchar_t PRODUCTID_STR[] = L"pid_";

    // ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
    class SDL3Backend : public IPhysicalControllerBackend
    {
        // IPlugin
        std::wstring_view PluginName() override;
        bool Initialize() override;

        // IPhysicalControllerBackend
        TPhysicalControllerIndex MaxPhysicalControllerCount() override;
        bool SupportsControllerByGuidAndPath(const wchar_t* guidAndPath) override;
        SPhysicalControllerCapabilities GetCapabilities() override;
        SPhysicalControllerState ReadInputState(TPhysicalControllerIndex physicalControllerIndex) override;
        bool WriteForceFeedbackState(TPhysicalControllerIndex physicalControllerIndex,
            SPhysicalControllerVibration vibrationState) override;
    };
}

#endif //XIDI_SDL3_PLUGIN_SDL3BACKEND_H