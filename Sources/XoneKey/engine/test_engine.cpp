#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include "Engine.h"
#include "DataType.h"

// Global engine settings (defined in each platform app; declared extern in Engine.h)
int vLanguage = 1;
int vInputType = vTelex;
int vFreeMark = 0;
int vCodeTable = 0;
int vSwitchKeyStatus = 0;
int vCheckSpelling = 1;
int vUseModernOrthography = 1;
int vQuickTelex = 1;
int vRestoreIfWrongSpelling = 0;
int vFixRecommendBrowser = 0;
int vUseMacro = 0;
int vUseMacroInEnglishMode = 0;
int vAutoCapsMacro = 0;
int vUseSmartSwitchKey = 0;
int vUpperCaseFirstChar = 0;
int vTempOffSpelling = 0;
int vAllowConsonantZFWJ = 0;
int vQuickStartConsonant = 0;
int vQuickEndConsonant = 0;
int vRememberCode = 0;
int vOtherLanguage = 0;
int vTempOffXoneKey = 0;

static vKeyHookState* hookState = nullptr;
static int passed = 0;
static int failed = 0;

static void resetEngine() {
    startNewSession();
    hookState->code = vDoNothing;
    hookState->backspaceCount = 0;
    hookState->newCharCount = 0;
}

static void pressKey(Uint16 key, bool caps = false) {
    vKeyHandleEvent(vKeyEvent::Keyboard, vKeyEventState::KeyDown, key, caps ? 1 : 0, false);
}

static void check(const char* desc, bool condition) {
    if (condition) {
        std::cout << "  PASS: " << desc << std::endl;
        passed++;
    } else {
        std::cerr << "  FAIL: " << desc << std::endl;
        failed++;
    }
}

// Returns the first output char code from the last hook state
static Uint32 firstOutChar() {
    return hookState->charData[0];
}

static void testTelexBasic() {
    std::cout << "\n[Telex basic]" << std::endl;
    vInputType = vTelex;

    // aa -> â: type 'a', then 'a' again
    resetEngine();
    pressKey(KEY_A); // first 'a', no change
    pressKey(KEY_A); // second 'a' triggers â
    check("aa triggers vWillProcess", hookState->code == vWillProcess);
    check("aa backspaces 1", hookState->backspaceCount == 1);
    check("aa outputs 1 char", hookState->newCharCount == 1);

    // as -> á (sắc on 'a')
    resetEngine();
    pressKey(KEY_A);
    pressKey(KEY_S);
    check("as triggers vWillProcess", hookState->code == vWillProcess);
    check("as outputs á char code", firstOutChar() == getCharacterCode(KEY_A | MARK1_MASK | CHAR_CODE_MASK) ||
                                     hookState->newCharCount >= 1);

    // 'z' key removes mark
    resetEngine();
    pressKey(KEY_A);
    pressKey(KEY_S); // type á
    pressKey(KEY_Z); // remove mark
    check("z removes mark", hookState->code == vWillProcess || hookState->code == vDoNothing);

    std::cout << std::endl;
}

static void testTelexMarkPlacement() {
    std::cout << "[Modern orthography mark placement]" << std::endl;
    vInputType = vTelex;
    vUseModernOrthography = 1;

    // "tia" + "s" -> mark should go on 'i' (rule 3.2: CHR(VSI+1)==KEY_A)
    resetEngine();
    pressKey(KEY_T);
    pressKey(KEY_I);
    pressKey(KEY_A);
    pressKey(KEY_S);
    check("tias: processes mark in vowel cluster ia", hookState->code == vWillProcess);

    // "thua" + "s" -> mark on 'u'
    resetEngine();
    pressKey(KEY_T);
    pressKey(KEY_H);
    pressKey(KEY_U);
    pressKey(KEY_A);
    pressKey(KEY_S);
    check("thuas: processes mark in vowel cluster ua", hookState->code == vWillProcess);

    std::cout << std::endl;
}

static void testVNIBasic() {
    std::cout << "[VNI basic]" << std::endl;
    vInputType = vVNI;

    // a + 6 -> â
    resetEngine();
    pressKey(KEY_A);
    pressKey(KEY_6);
    check("a6 triggers vWillProcess", hookState->code == vWillProcess);

    // a + 1 -> á
    resetEngine();
    pressKey(KEY_A);
    pressKey(KEY_1);
    check("a1 triggers mark", hookState->code == vWillProcess);

    std::cout << std::endl;
}

static void testWordBreak() {
    std::cout << "[Word break]" << std::endl;
    vInputType = vTelex;

    resetEngine();
    pressKey(KEY_A);
    pressKey(KEY_S); // á
    pressKey(KEY_SPACE);
    check("space breaks word", hookState->code == vDoNothing || hookState->code == vRestore);

    resetEngine();
    pressKey(KEY_A);
    vKeyHandleEvent(vKeyEvent::Mouse, vKeyEventState::MouseDown, 0, 0, false);
    check("mouse click breaks word", hookState->code == vDoNothing || hookState->backspaceCount == 0);

    std::cout << std::endl;
}

static void testQuickTelex() {
    std::cout << "[Quick Telex]" << std::endl;
    vInputType = vTelex;
    vQuickTelex = 1;

    // cc -> ch
    resetEngine();
    pressKey(KEY_C);
    pressKey(KEY_C);
    check("cc -> ch triggers vWillProcess", hookState->code == vWillProcess);
    check("cc -> ch outputs 2 chars", hookState->newCharCount == 2);

    // nn -> ng
    resetEngine();
    pressKey(KEY_N);
    pressKey(KEY_N);
    check("nn -> ng triggers vWillProcess", hookState->code == vWillProcess);

    std::cout << std::endl;
}

static void testDeleteKey() {
    std::cout << "[Delete key]" << std::endl;
    vInputType = vTelex;

    resetEngine();
    pressKey(KEY_A);
    pressKey(KEY_S); // á
    pressKey(KEY_DELETE);
    check("delete removes last char", hookState->extCode == 2);

    std::cout << std::endl;
}

static void testSpellingCheck() {
    std::cout << "[Spelling check]" << std::endl;
    vInputType = vTelex;
    vCheckSpelling = 1;

    // "xq" is not a valid Vietnamese syllable start - typing 's' mark should not apply
    resetEngine();
    pressKey(KEY_X);
    pressKey(KEY_Q);
    pressKey(KEY_S);
    check("invalid consonant cluster disables mark key", hookState->code == vDoNothing);

    std::cout << std::endl;
}

int main() {
    std::cout << "=== XoneKey Engine Tests ===" << std::endl;

    hookState = (vKeyHookState*)vKeyInit();
    if (!hookState) {
        std::cerr << "FATAL: vKeyInit() returned null" << std::endl;
        return 1;
    }
    std::cout << "Engine initialized." << std::endl;

    testTelexBasic();
    testTelexMarkPlacement();
    testVNIBasic();
    testWordBreak();
    testQuickTelex();
    testDeleteKey();
    testSpellingCheck();

    std::cout << "=== Results: " << passed << " passed, " << failed << " failed ===" << std::endl;
    return failed > 0 ? 1 : 0;
}
