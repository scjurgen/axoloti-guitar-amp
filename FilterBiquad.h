#pragma once
#include <cmath>

#define M_SCALE 1.0
#define OLD_SCALE 1073741824.0

#define PRECALC()                                                                                                      \
    float Fc   = frequency / SAMPLERATE;                                                                              \
    float norm = 0.0;                                                                                                  \
    float V    = pow(10, fabs(gain) / 20.0);                                                                           \
    float K    = tan(PI * Fc);                                                                                       \
    float K2   = K * K;

class AudioFilterBiquad
{
public:
    AudioFilterBiquad(void)
            : z1(0.0), z2(0.0)
    {
    }

    // Compute common filter functions`
    // http://www.musicdsp.org/files/Audio-EQ-Cookbook.txt
    void setLowpass(float frequency, float q = 0.7071)
    {
        //LogTextMessage("Init LP %f %f sr=%d", frequency, q,SAMPLERATE);
        float gain = 0.0;
        PRECALC()
        norm = 1 / (1 + K / q + K2);
        a0 = K2 * norm;
        a1 = 2 * a0;
        a2 = a0;
        b1 = 2 * (K2 - 1) * norm;
        b2 = (1 - K / q + K2) * norm;
    }

    void setHighpass(float frequency, float q = 0.7071)
    {
        //LogTextMessage("Init HP %f %f sr=%d", frequency, q,SAMPLERATE);
        float gain = 0;
        PRECALC()
        norm = 1 / (1 + K / q + K2);
        a0 = 1 * norm;
        a1 = -2 * a0;
        a2 = a0;
        b1 = 2 * (K2 - 1) * norm;
        b2 = (1 - K / q + K2) * norm;
    }

    void setBandpass(float frequency, float q = 1.0)
    {
        float gain = 0;
        PRECALC()
        norm = 1 / (1 + K / q + K2);
        a0 = K / q * norm;
        a1 = 0;
        a2 = -a0;
        b1 = 2 * (K2 - 1) * norm;
        b2 = (1 - K / q + K2) * norm;
    }

    void setNotch(float frequency, float q = 1.0)
    {
        float gain = 0;
        PRECALC()
        norm = 1 / (1 + K / q + K2);
        a0 = (1 + K2) * norm;
        a1 = 2 * (K2 - 1) * norm;
        a2 = a0;
        b1 = a1;
        b2 = (1 - K / q + K2) * norm;
    }

    void setAllPass(float frequency, float q = 1.0)
    {
        float w0 = frequency * (2 * PI / SAMPLERATE);
        float sinW0 = sin(w0);
        float alpha = sinW0 / ((float) q * 2.0);
        float cosW0 = cos(w0);
        float scale = 1.0 / (1.0 + alpha);
        a0 = (1.0 - alpha) * scale;
        a1 = (-2.0 * cosW0) * scale;
        a2 = (1.0 + alpha) * scale;
        b1 = (-2.0 * cosW0) * scale;
        b2 = (1.0 - alpha) * scale;
    }

    void setPeakEQ(float frequency, float gain, float q = 1.0)
    {
        //LogTextMessage("Init PK %f %f %f sr=%d", frequency, gain, q,SAMPLERATE);

        PRECALC()
        if (q == 0.0)
        {
            a0 = 1.0;
            a1 = a2 = b1 = b2 = 0;
            return;
        }
        if (gain >= 0)
        {
            norm = 1 / (1 + 1 / q * K + K2);
            a0 = (1 + V / q * K + K2) * norm;
            a1 = 2 * (K2 - 1) * norm;
            a2 = (1 - V / q * K + K2) * norm;
            b1 = a1;
            b2 = (1 - 1 / q * K + K2) * norm;
        } else
        {
            norm = 1 / (1 + V / q * K + K2);
            a0 = (1 + 1 / q * K + K2) * norm;
            a1 = 2 * (K2 - 1) * norm;
            a2 = (1 - 1 / q * K + K2) * norm;
            b1 = a1;
            b2 = (1 - V / q * K + K2) * norm;
        }
    }

    void setLowShelf(float frequency, float gain, float slope = 1.0f)
    {
        float B21 = pow(10, (gain / 40));
        float B22 = 2 * PI * frequency / SAMPLERATE;
        float B23 = sin(B22) / (2 * slope);
        float B24 = (B21 + 1) + (B21 - 1) * cos(B22) + 2 * sqrt(B21) * B23;

        b1 = (-2 * ((B21 - 1) + (B21 + 1) * cos(B22))) / B24;
        b2 = ((B21 + 1) + (B21 - 1) * cos(B22) - 2 * sqrt(B21) * B23) / B24;
        a0 = (B21 * ((B21 + 1) - (B21 - 1) * cos(B22) + 2 * sqrt(B21) * B23)) / B24;
        a1 = (2 * B21 * ((B21 - 1) - (B21 + 1) * cos(B22))) / B24;
        a2 = (B21 * ((B21 + 1) - (B21 - 1) * cos(B22) - 2 * sqrt(B21) * B23)) / B24;
    }

    void setHighShelf(float frequency, float gain, float slope = 1.0f)
    {
        float a = pow(10.0, gain / 40.0);
        float w0 = frequency * (2 * PI / SAMPLERATE);
        float sinW0 = sin(w0);
        // float alpha = (sinW0 * sqrt((a+1/a)*(1/slope-1)+2) ) / 2.0;
        float cosW0 = cos(w0);
        // generate three helper-values (intermediate results):
        float sinsq = sinW0 * sqrt((pow(a, 2.0) + 1.0) * (1.0 / slope - 1.0) + 2.0 * a);
        float aMinus = (a - 1.0) * cosW0;
        float aPlus = (a + 1.0) * cosW0;
        float scale = 1.0 / ((a + 1.0) - aMinus + sinsq);
        a0 = a * ((a + 1.0) + aMinus + sinsq) * scale;
        a1 = -2.0 * a * ((a - 1.0) + aPlus) * scale;
        a2 = a * ((a + 1.0) + aMinus - sinsq) * scale;
        b1 = 2.0 * ((a - 1.0) - aPlus) * scale;
        b2 = ((a + 1.0) - aMinus - sinsq) * scale;
    }

    void setLinearGain(float gain)
    {
        a0 = gain;
        a1 = 0.0;
        a2 = 0.0;
        b1 = 0.0;
        b2 = 0.0;
    }

    float step(float in)
    {
        auto out = in * a0 + z1;
        z1 = in * a1 + z2 - b1 * out;
        z2 = in * a2 - b2 * out;
        return out;
    }

    void update(float *inplaceBuffer, int size)
    {
        for (auto i = 0; i < size; ++i)
        {
            auto in = inplaceBuffer[i];
            auto out = in * a0 + z1;
            z1 = in * a1 + z2 - b1 * out;
            z2 = in * a2 - b2 * out;
            inplaceBuffer[i] = out;
        }
    }

    void update(const float *inplaceBuffer, float *outBuffer, int size)
    {
        for (auto i = 0; i < size; ++i)
        {
            auto in = inplaceBuffer[i];
            auto out = in * a0 + z1;
            z1 = in * a1 + z2 - b1 * out;
            z2 = in * a2 - b2 * out;
            outBuffer[i] = out;
        }
    }

    void reset()
    {
        z1 = 0;
        z2 = 0;
    }

    float a0, a1, a2, b1, b2;
    float z1, z2;
};
