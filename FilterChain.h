#pragma once
#include "FilterSet.h"
#include "FilterBiquad.h"
#include "FeedbackRing.h"
#include "PitchDelay.h"
typedef float FloatBuffer[BUFSIZE];

#define A 0.0776509570923569
#define B -0.287434475393028
#define C (M_PI_4 - A - B)
#ifndef M_PI_4
#define M_PI_4 (3.1415926535897932384626433832795/4.0)
#endif

#define ALLPASSMAX 300

class AllPassFilter
{
public:
    AllPassFilter() : head(0), gain(0.0), currentWidth(10)
    {

        buffer = (float *)sdram_malloc(ALLPASSMAX*sizeof(float));
        reset();
        feeding = false;
    }

    ~AllPassFilter()
    {
        ax_free(buffer);
    }

    void setFrequency(float f)
    {
        currentWidth = (int) (SAMPLERATE / f + 0.5);
        if (currentWidth >= ALLPASSMAX)
            currentWidth = ALLPASSMAX;
        if (currentWidth < 2)
            currentWidth = 2;
        reset();
    }

    void setGain(float value)
    {
        if (value > 0.9)
            value = 0.9;
        if (value < -0.9)
            value = -0.9;
        gain = value;
    }

    float step(float in)
    {
        float value = in + buffer[head];
        buffer[head++] = value * gain;
        if (head >= currentWidth)
            head = 0;
        return value;
    }
    void reset()
    {
        head = 0;
        for (int i=0; i < ALLPASSMAX; ++i)
            buffer[i] = 0;
    }

    void update(FloatBuffer &inPlace)
    {
        for (int s=0; s < 16; ++s)
        {
            inPlace[s] = step(inPlace[s]);
        }
    }

    void update(const FloatBuffer &in, FloatBuffer &out)
    {
        if (!feeding)
        {
            feeding = true;
        }
        for (int s=0; s < 16; ++s)
        {
            out[s] = step(in[s]);
        }
    }

    float *buffer;
    uint32_t head;
    float gain;
    int currentWidth;
    bool feeding;
};

template<int __MAXFILTERS__, int __MAXALLPASS__>
class FilterChain
{
public:
    FilterChain()
            :m_filterItems(0)
    {
        for (int f=0; f < __MAXFILTERS__; f++)
            for (int c=0; c < 2; c++)
            {
                filters[c][f] = (AudioFilterBiquad*)sdram_malloc(sizeof(AudioFilterBiquad));
            }
    }

    FilterChain(FilterSet<__MAXFILTERS__> &initial)
            : m_filterItems()
    {
        setParams(initial);
    }

    void dispose()
    {
        for (int f=0; f < __MAXFILTERS__; f++)
            for (int c=0; c < 2; c++)
            {
                ax_free(filters[c][f]);
            }
    }

    void setParams(FilterSet<__MAXFILTERS__> &newValues)
    {
        filterParam = newValues;
        int fIndex = 0, aIndex = 0;
        parallelCount = 0;
        straightCount = 0;
        allPassCount = 0;
        for (int i = 0; i < newValues.filterCount; ++i)
        {
            if (filterParam.fSet[i].equalizer != EQ_NONE)
            {
                parallelList[parallelCount++] = fIndex;
                if (filterParam.fSet[i].equalizer == EQ_LOW)
                    eqIndex[0] = i;
                if (filterParam.fSet[i].equalizer == EQ_MID)
                    eqIndex[1] = i;
                if (filterParam.fSet[i].equalizer == EQ_HIGH)
                    eqIndex[2] = i;
            } else
            {
                if (filterParam.fSet[i].type==FilterType::AllPass)
                {
                    allPassList[allPassCount++] = aIndex;
                }
                else
                {
                    straightList[straightCount++] = fIndex;
                }
            }
            switch (filterParam.fSet[i].type)
            {
                case FilterType::None:
                    for (int stage = 0; stage < 2; ++stage) filters[stage][fIndex]->setLinearGain(1.0);
                    break;
                case FilterType::AllPass:
                    allPass[aIndex].setFrequency(filterParam.fSet[i].f);
                    allPass[aIndex].setGain(filterParam.fSet[i].gain);
                    aIndex++;
                    break;
                case FilterType::HiPass:
                    for (int stage = 0; stage < 2; ++stage)
                        filters[stage][fIndex]->setHighpass(filterParam.fSet[i].f, filterParam.fSet[i].q);
                    fIndex++;
                    break;
                case FilterType::LoPass:
                    for (int stage = 0; stage < 2; ++stage)
                        filters[stage][fIndex]->setLowpass(filterParam.fSet[i].f, filterParam.fSet[i].q);
                    fIndex++;
                    break;
                case FilterType::HiShelf:
                    for (int stage = 0; stage < 2; ++stage)
                        filters[stage][fIndex]->setHighShelf(filterParam.fSet[i].f, filterParam.fSet[i].gain,
                                                            filterParam.fSet[i].q);
                    fIndex++;
                    break;
                case FilterType::LoShelf:
                    for (int stage = 0; stage < 2; ++stage)
                        filters[stage][fIndex]->setLowShelf(filterParam.fSet[i].f, filterParam.fSet[i].gain,
                                                           filterParam.fSet[i].q);
                    fIndex++;
                    break;
                case FilterType::Peak:
                    for (int stage = 0; stage < 2; ++stage)
                        filters[stage][fIndex]->setPeakEQ(filterParam.fSet[i].f, filterParam.fSet[i].gain,
                                                         filterParam.fSet[i].q);
                    fIndex++;
                    break;
                case FilterType::Notch:
                    for (int stage = 0; stage < 2; ++stage)
                        filters[stage][fIndex]->setNotch(filterParam.fSet[i].f, filterParam.fSet[i].q);
                    fIndex++;
                    break;
            }
        }
        m_filterItems = fIndex;
        reset();
    }

    void reset()
    {
        for (int stage = 0; stage < 2; ++stage)
        {
            for (int i = 0; i < __MAXFILTERS__; ++i)
                filters[stage][i]->reset();
        }
        for (int i = 0; i < __MAXALLPASS__; ++i)
            allPass[i].reset();
    }

    void update(FloatBuffer &inplace, int idx)
    {
        filters[0][idx]->update(inplace, BUFSIZE);
        if (filterParam.fSet[idx].order)
        {
            filters[1][idx]->update(inplace, BUFSIZE);
        }
    }
    void update(const FloatBuffer &in, FloatBuffer &out, int idx)
    {
        filters[0][idx]->update(in, out, BUFSIZE);
        if (filterParam.fSet[idx].order)
        {
            filters[1][idx]->update(out, BUFSIZE);
        }
    }

    void clearBuffer(FloatBuffer &buffer)
    {
        for (int s=0; s < BUFSIZE; ++s)
           buffer[s] = 0.0;
    }

    void addBuffer(const FloatBuffer &source, FloatBuffer &target)
    {
        for (int s=0; s < BUFSIZE; ++s)
        {
            target[s]+=source[s];
        }
    }

    void scaleBuffer(const FloatBuffer &buffer,  FloatBuffer &target, float gain)
    {
        for (int s=0; s < BUFSIZE; ++s)
        {
            target[s] = buffer[s] * gain;
        }
    }

    void setGain(float low, float mid, float high)
    {
        float gains[3] = {low,mid, high};
        for (int c=0; c < 3; ++c)
        {
            if (lastGains[c] != gains[c])
            {
                for (int stage = 0; stage < 2; ++stage)
                    filters[stage][eqIndex[c]]->setPeakEQ(filterParam.fSet[eqIndex[c]].f,
                                                         filterParam.fSet[eqIndex[c]].gain + gains[c] * 18,
                                                         filterParam.fSet[eqIndex[c]].q);
                lastGains[c] = gains[c];
            }
        }
    }

    void update(FloatBuffer &inplace)
    {
        FloatBuffer result;
        if (allPassCount)
        {
            FloatBuffer sum;
            FloatBuffer tmp;
            clearBuffer(sum);
            for (int i = 0; i < allPassCount; ++i)
            {
                allPass[allPassList[i]].update(inplace, tmp);
                addBuffer(tmp, sum);
            }
            scaleBuffer(sum,result, 1.0f/allPassCount);
        } else{
            memcpy(result, inplace, sizeof(FloatBuffer));
        }
        if (parallelCount)
        {
            FloatBuffer sum;
            FloatBuffer tmp;
            memcpy(sum, result, sizeof(FloatBuffer));
            for (int i = 0; i < parallelCount; ++i)
            {
                filters[0][parallelList[i]]->update(inplace, tmp, BUFSIZE);
                addBuffer(tmp, sum);
            }
            scaleBuffer(sum,result, 1.0f/parallelCount);
        }
        for (int i = 0; i < straightCount; ++i)
        {
            filters[0][straightList[i]]->update(result, BUFSIZE);
        }
        scaleBuffer(result, inplace, filterParam.masterGain);
    }

    void dsp(const int32buffer in, int32buffer &out)
    {
        float buffer[BUFSIZE];
        float tbuffer[BUFSIZE];
        for (int i = 0; i < BUFSIZE; ++i)
        {
            buffer[i] = q27_to_float(in[i]);
        }
        update(buffer);
        for (int i = 0; i < BUFSIZE; ++i)
        {
            out[i] = float_to_q27(buffer[i]);
        }
    }

    int m_filterItems;
    AllPassFilter allPass[__MAXALLPASS__];
    AudioFilterBiquad *filters[2][__MAXFILTERS__];
    FilterSet<__MAXFILTERS__> filterParam;
    int allPassList[__MAXFILTERS__];
    int allPassCount;
    int parallelList[__MAXFILTERS__];
    int parallelCount;
    int straightList[__MAXFILTERS__];
    int straightCount;
    int eqIndex[3];
    float lastGains[3];
};
