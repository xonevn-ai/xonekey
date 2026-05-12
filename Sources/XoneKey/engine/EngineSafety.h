//
//  EngineSafety.h
//  XoneKey
//
//  Bounds-checking helpers shared by the engine.
//  Platform-agnostic: standard C++ only.
//

#ifndef EngineSafety_h
#define EngineSafety_h

#include "DataType.h"
#include <cassert>

inline bool IsValidIndex(Byte index) {
    return index < MAX_BUFF;
}

inline bool IsIndexInWord(Byte index, Byte currentIndex) {
    return index < currentIndex && index < MAX_BUFF;
}

inline bool IsValidCharacterCode(Uint32 code) {
    return code != 0 && code < 0x10FFFF;
}

inline bool SafeIncrementIndex(Byte* index) {
    if (!index) return false;
    if (*index < MAX_BUFF - 1) {
        (*index)++;
        return true;
    }
    return false;
}

inline bool SafeDecrementIndex(Byte* index) {
    if (!index) return false;
    if (*index > 0) {
        (*index)--;
        return true;
    }
    return false;
}

inline bool SafeSetKeyState(Byte index, Uint32 value, Uint32* keyStates, Byte* currentStateIndex) {
    if (!IsValidIndex(index) || !keyStates || !currentStateIndex) {
        return false;
    }
    if (index >= *currentStateIndex && *currentStateIndex < MAX_BUFF) {
        (*currentStateIndex)++;
    }
    keyStates[index] = value;
    return true;
}

#ifdef IS_DEBUG
#define ENGINE_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            assert(false && message); \
        } \
    } while(0)
#else
#define ENGINE_ASSERT(condition, message) ((void)0)
#endif

#endif /* EngineSafety_h */
