//
//  SmartSwitchKey.cpp
//  XoneKey
//
//  Created by Daniel Do on 16 Apr 2025.
//  Copyright © 2025 Daniel Do. All rights reserved.
//

#include "SmartSwitchKey.h"
#include <map>
#include <iostream>
#include <memory.h>

//main data, i use `map` because it has O(Log(n))
static map<string, Int8> _smartSwitchKeyData;
static string _cacheKey = ""; //use cache for faster
static Int8 _cacheData = 0; //use cache for faster

#ifdef _WIN32
#include <windows.h>
static CRITICAL_SECTION _smartSwitchCs;
static bool _smartSwitchCsInitialized = false;

static void EnsureSmartSwitchCsInitialized() {
    if (!_smartSwitchCsInitialized) {
        InitializeCriticalSection(&_smartSwitchCs);
        _smartSwitchCsInitialized = true;
    }
}

#define SMART_SWITCH_LOCK EnsureSmartSwitchCsInitialized(); EnterCriticalSection(&_smartSwitchCs);
#define SMART_SWITCH_UNLOCK LeaveCriticalSection(&_smartSwitchCs);
#else
#define SMART_SWITCH_LOCK
#define SMART_SWITCH_UNLOCK
#endif

void initSmartSwitchKey(const Byte* pData, const int& size) {
    SMART_SWITCH_LOCK
    _smartSwitchKeyData.clear();
    if (pData != NULL) {
        Uint16 count = 0;
        Uint32 cursor = 0;
        if (size >= 2) {
            memcpy(&count, pData + cursor, 2);
            cursor+=2;
        }
        Uint8 bundleIdSize;
        Uint8 value;
        for (int i = 0; i < count; i++) {
            if (cursor >= (Uint32)size) break;
            bundleIdSize = pData[cursor++];
            if (bundleIdSize == 0 || cursor + bundleIdSize + 1 > (Uint32)size) break;
            string bundleId((char*)pData + cursor, bundleIdSize);
            cursor += bundleIdSize;
            value = pData[cursor++];
            _smartSwitchKeyData[bundleId] = value;
        }
    }
    SMART_SWITCH_UNLOCK
}

void getSmartSwitchKeySaveData(vector<Byte>& outData) {
    SMART_SWITCH_LOCK
    outData.clear();
    Uint16 count = (Uint16)_smartSwitchKeyData.size();
    outData.push_back((Byte)count);
    outData.push_back((Byte)(count>>8));
    
    for (std::map<string, Int8>::iterator it = _smartSwitchKeyData.begin(); it != _smartSwitchKeyData.end(); ++it) {
        outData.push_back((Byte)it->first.length());
        for (int j = 0; j < (int)it->first.length(); j++) {
            outData.push_back(it->first[j]);
        }
        outData.push_back(it->second);
    }
    SMART_SWITCH_UNLOCK
}

int getAppInputMethodStatus(const string& bundleId, const int& currentInputMethod) {
    SMART_SWITCH_LOCK
    if (_cacheKey.compare(bundleId) == 0) {
        int result = _cacheData;
        SMART_SWITCH_UNLOCK
        return result;
    }
    if (_smartSwitchKeyData.find(bundleId) != _smartSwitchKeyData.end()) {
        _cacheKey = bundleId;
        _cacheData = _smartSwitchKeyData[bundleId];
        int result = _cacheData;
        SMART_SWITCH_UNLOCK
        return result;
    }
    _cacheKey = bundleId;
    _cacheData = currentInputMethod;
    _smartSwitchKeyData[bundleId] = _cacheData;
    SMART_SWITCH_UNLOCK
    return -1;
}

void setAppInputMethodStatus(const string& bundleId, const int& language) {
    SMART_SWITCH_LOCK
    _smartSwitchKeyData[bundleId] = language;
    _cacheKey = bundleId;
    _cacheData = language;
    SMART_SWITCH_UNLOCK
}
