//
//  EngineSafety.h
//  XoneKey
//
//  Safety improvements for Engine
//  Added bounds checking, validation, and error handling
//  Cross-platform compatible (Windows, macOS, Linux)
//

#ifndef EngineSafety_h
#define EngineSafety_h

#include "DataType.h"
#include <cassert>

// Cross-platform compatibility check
// All functions use only standard C++ features, no platform-specific code

// Safe array access with bounds checking
inline bool IsValidIndex(Byte index) {
    return index >= 0 && index < MAX_BUFF;
}

// Safe TypingWord access
inline Uint32 SafeGetTypingWord(Byte index, Uint32* typingWord, Byte currentIndex) {
    if (!IsValidIndex(index) || index >= currentIndex || !typingWord) {
        return 0; // Return safe default
    }
    return typingWord[index];
}

// Safe TypingWord write
inline bool SafeSetTypingWord(Byte index, Uint32 value, Uint32* typingWord, Byte currentIndex) {
    if (!IsValidIndex(index) || index >= currentIndex || !typingWord) {
        return false;
    }
    typingWord[index] = value;
    return true;
}

// Safe KeyStates access
inline Uint32 SafeGetKeyState(Byte index, Uint32* keyStates, Byte currentStateIndex) {
    if (!IsValidIndex(index) || index >= currentStateIndex || !keyStates) {
        return 0;
    }
    return keyStates[index];
}

// Safe KeyStates write
inline bool SafeSetKeyState(Byte index, Uint32 value, Uint32* keyStates, Byte* currentStateIndex) {
    if (!IsValidIndex(index) || !keyStates || !currentStateIndex) {
        return false;
    }
    if (index >= *currentStateIndex && *currentStateIndex < MAX_BUFF) {
        (*currentStateIndex)++;
    }
    if (index < MAX_BUFF) {
        keyStates[index] = value;
        return true;
    }
    return false;
}

// Validate character code before processing
inline bool IsValidCharacterCode(Uint32 code) {
    // Basic validation - can be extended
    return code != 0 && code < 0x10FFFF; // Valid Unicode range
}

// Check if index is within safe bounds for current word
inline bool IsIndexInWord(Byte index, Byte currentIndex) {
    return index >= 0 && index < currentIndex && index < MAX_BUFF;
}

// Safe increment with bounds check
inline bool SafeIncrementIndex(Byte* index) {
    if (!index) return false;
    if (*index < MAX_BUFF - 1) {
        (*index)++;
        return true;
    }
    return false; // Would overflow
}

// Safe decrement with bounds check
inline bool SafeDecrementIndex(Byte* index) {
    if (!index) return false;
    if (*index > 0) {
        (*index)--;
        return true;
    }
    return false; // Would underflow
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

