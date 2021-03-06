/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   Trem_png;
    const int            Trem_pngSize = 2833;

    extern const char*   Release_png;
    const int            Release_pngSize = 5025;

    extern const char*   Thresh_png;
    const int            Thresh_pngSize = 3961;

    extern const char*   Clip_png;
    const int            Clip_pngSize = 2497;

    extern const char*   Gain_png;
    const int            Gain_pngSize = 3076;

    extern const char*   Knob_png;
    const int            Knob_pngSize = 7241;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 6;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
