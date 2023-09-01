#include "CommonIncludes.hpp"

mem::MemoryPatch saveModdedPatch;
void entryPoint() {
    std::this_thread::sleep_for(8s); // ensure everything has loaded

    static auto isSaveModded = mem::scanForPtr("40 53 48 83 EC ? C6 44 24 ? ? 48 8B D9 84 D2");

    saveModdedPatch.init(isSaveModded, BYTE_ARRAY("B0 00 C3")).applyPatch();
}

BOOL APIENTRY DllMain(const HMODULE hModule, const DWORD reasonForCall, [[maybe_unused]] LPCVOID lpReserved) {
    if (reasonForCall == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);

        if (const auto handle = CreateThread(
                nullptr, 0,
                [](LPVOID mod) -> DWORD {
                    try {
                        entryPoint();
                    } catch (...) {
                        MessageBoxA(nullptr, "An exception occured while enabling achievements", "starfield-achievementsenabler", MB_OK);
                    }

                    return 1;
                },
                hModule, 0, nullptr);
            handle != nullptr) {
            CloseHandle(handle);
        }
    }

    return TRUE;
}