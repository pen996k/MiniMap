#define STBI_NO_STDIO
#define STBI_NO_HDR
#define STBI_NO_LINEAR
#define STBI_NO_THREAD_LOCALS
#define STBI_ONLY_PNG
#define STBI_ONLY_TGA
#define STBI_MAX_DIMENSIONS 1024
#define STB_IMAGE_IMPLEMENTATION
#pragma warning( push, 0 )
#pragma warning( disable : 4146 4996 4244 4456 4457 4701 4703 )
#include "third_party/stb_image.h"
#pragma warning( pop )

BOOL WINAPI DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
        switch (GetGameVersion())
        {
#ifdef __G1
            case Engine_G1:
                break;
#endif
#ifdef __G1A
            case Engine_G1A:
                break;
#endif
#ifdef __G2
            case Engine_G2:
                break;
#endif
#ifdef __G2A
            case Engine_G2A:
                break;
#endif
            default:
                return FALSE;
        }
        break;

    case DLL_PROCESS_DETACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    }

    return TRUE;
}
