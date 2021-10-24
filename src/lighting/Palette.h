#pragma once

#include <FastLED.h>

class Palette {

private:

const char* _name;
const TProgmemRGBPalette16* _palette;
    
public:
    Palette(const char * name, const TProgmemRGBPalette16* palette) : 
        _name(name), _palette(palette) {}

    const char* getName() const {
        return _name;
    }

    const TProgmemRGBPalette16* getPalette() const {
        return _palette;
    }
};