#pragma once

typedef enum
{
    None,
    HiPass,
    Peak,
    LoPass,
    AllPass,
    HiShelf,
    LoShelf,
    Notch
} FilterType;

typedef enum
{
    EQ_NONE,
    EQ_SUB,
    EQ_LOW,
    EQ_MID,
    EQ_HIGH,
    EQ_BRIGHT,
}EQUALIZERCHANNEL;

typedef struct
{
    FilterType type;
    int order;
    EQUALIZERCHANNEL equalizer;
    float f;
    float gain;
    float q;
} FilterParam;


template <int MAXFILTERS>
struct FilterSet
{
    float masterGain;
    uint32_t filterCount;
    FilterParam fSet[MAXFILTERS];
} ;

