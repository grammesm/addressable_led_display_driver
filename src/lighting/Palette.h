#pragma once

#include <FastLED.h>

class Palette
{

private:
    const char *_name;
    const char *_description;
    const TProgmemRGBPalette16 *_palette;

public:
    Palette(const char *name, const char *description, const TProgmemRGBPalette16 *palette) : _name(name), _description(description), _palette(palette) {}

    const char *getName() const
    {
        return _name;
    }

    const TProgmemRGBPalette16 *getPalette() const
    {
        return _palette;
    }

    const String getPaletteJson() const
    {
        String jsonStr;
        jsonStr += "{";
        jsonStr += "\"name\":";
        jsonStr += "\"";
        jsonStr += _name;
        jsonStr += "\"";
        jsonStr += ",";
        jsonStr += "\"desc\":";
        jsonStr += "\"";
        jsonStr += _description;
        jsonStr += "\"";
        jsonStr += ",";
        jsonStr += "\"colors\":";
        jsonStr += "[";
        CRGBPalette16* palette = (CRGBPalette16*) _palette;
        for (int i = 0; i < 16; i++)
        {
            // get value and convert.
            long HexRGB = ((long)palette->entries[i].r << 16) | ((long)palette->entries[i].g << 8) | ((long)palette->entries[i].b);
            // JSON doesn't support HEX 0x
            //jsonStr += "0x";
            jsonStr += String(HexRGB);
            if (i < 15)
            {
                jsonStr += ",";
            }
        }
        jsonStr += "]";
        jsonStr += "}";

        return jsonStr;
    }
};