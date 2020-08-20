#pragma once

#include <cstddef>
#include <cmath>

enum FilterType { PassThru, LowPass, HighPass, BandPass, Notch, Peak, LowShelf, HighShelf };
// check with https://webaudio.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html


class BiquadOptimized
{
  public:
    typedef void (BiquadOptimized::*BiQuadCallBack)(const float* inBuf, float* outBuf, int numFrames);
    void calc(FilterType type, float sampleRate, float frequency, float Q, float peakGain)
    {
        float Fc   = frequency / sampleRate;
        float V    = powf(10, fabs(peakGain) / 20.0);
        float K    = tanf(M_PI * Fc);
        float Ksqr = K * K;
        float norm;

        switch (type) {
            case PassThru:
                b0 = 1;
                b1 = b2 = a1 = a2 = 0; // delay elements fed with 0
                m_cb = &BiquadOptimized::stepPassThrough;
                break;

            case LowPass:
                norm   = 1 / (1 + K / Q + Ksqr);
                b0 = Ksqr * norm;
                b1 = 2 * b0;
                b2 = b0;
                a1 = 2 * (Ksqr - 1) * norm;
                a2 = (1 - K / Q + Ksqr) * norm;
                m_cb = &BiquadOptimized::stepLowHiPass;
                break;

            case HighPass:
                norm   = 1 / (1 + K / Q + Ksqr);
                b0 = 1 * norm;
                b1 = -2 * b0;
                b2 = b0;
                a1 = 2 * (Ksqr - 1) * norm;
                a2 = (1 - K / Q + Ksqr) * norm;
                m_cb = &BiquadOptimized::stepLowHiPass;
                break;

            case BandPass:
                norm   = 1 / (1 + K / Q + Ksqr);
                b0 = K / Q * norm;
                b1 = 0;
                b2 = -b0;
                a1 = 2 * (Ksqr - 1) * norm;
                a2 = (1 - K / Q + Ksqr) * norm;
                m_cb = &BiquadOptimized::stepBandpass;
                break;

            case Notch:
                norm   = 1 / (1 + K / Q + Ksqr);
                b2 = b0 = (1 + Ksqr) * norm;
                a1 = b1 = 2 * (Ksqr - 1) * norm;
                a2 = (1 - K / Q + Ksqr) * norm;
                m_cb = &BiquadOptimized::stepNotch;
                break;

            case Peak:


                if (peakGain >= 0) {
                    norm   = 1 / (1 + 1 / Q * K + Ksqr);
                    b0 = (1 + V / Q * K + Ksqr) * norm;
                    b2 = (1 - V / Q * K + Ksqr) * norm;
                    a2 = (1 - 1 / Q * K + Ksqr) * norm;
                } else {
                    norm   = 1 / (1 + V / Q * K + Ksqr);
                    b0 = (1 + 1 / Q * K + Ksqr) * norm;
                    b2 = (1 - 1 / Q * K + Ksqr) * norm;
                    a2 = (1 - V / Q * K + Ksqr) * norm;
                }
                a1 = b1 = 2 * (Ksqr - 1) * norm;

                m_cb = &BiquadOptimized::stepPeak;
                break;

            case LowShelf:
                {
                    float V = powf(10, (peakGain / 40));
                    float w0 = 2 * M_PI * frequency / sampleRate;
                    float alpha = sinf(w0) / (2 * Q);
                    float scale = (V + 1) + (V - 1) * cosf(w0) + 2 * sqrtf(V) * alpha;

                    a1 = (-2 * ((V - 1) + (V + 1) * cosf(w0))) / scale;
                    a2 = ((V + 1) + (V - 1) * cosf(w0) - 2 * sqrtf(V) * alpha) / scale;
                    b0 = (V * ((V + 1) - (V - 1) * cosf(w0) + 2 * sqrtf(V) * alpha)) / scale;
                    b1 = (2 * V * ((V - 1) - (V + 1) * cosf(w0))) / scale;
                    b2 = (V * ((V + 1) - (V - 1) * cosf(w0) - 2 * sqrtf(V) * alpha)) / scale;
                }
                m_cb = &BiquadOptimized::step;
                break;

            case HighShelf:
            {
                float V = powf(10, (peakGain / 40));
                float w0 = 2 * M_PI * frequency / sampleRate;
                float alpha = sinf(w0) / (2 * Q);
                float scale = (V + 1) - (V - 1) * cosf(w0) + 2 * sqrtf(V) * alpha;
                a1 = (2 * ((V - 1) - (V + 1) * cosf(w0))) / scale;
                a2 = ((V + 1) - (V - 1) * cosf(w0) - 2 * sqrtf(V) * alpha) / scale;
                b0 = (V * ((V + 1) + (V - 1) * cosf(w0) + 2 * sqrtf(V) * alpha)) / scale;
                b1 = (-2 * V * ((V - 1) + (V + 1) * cosf(w0))) / scale;
                b2 = (V * ((V + 1) + (V - 1) * cosf(w0) - 2 * sqrtf(V) * alpha)) / scale;
            }

                m_cb = &BiquadOptimized::step;
                break;
        }
    }

    void step(const float *in, float *outBuffer, int size)
    {
        for (int i=0; i < size; ++i)
        {
            float out = in[i] * b0 + z1;
            z1 = in[i] * b1 + z2 - a1 * out;
            z2 = in[i] * b2 - a2 * out;
            outBuffer[i] = out;
        }
    }
    void stepBandpass(const float *in, float *outBuffer, int size)
    {
        for (int i=0; i < size; ++i)
        {
            float b0s = in[i] * b0;
            float out = b0s + z1;
            z1 = z2 - a1 * out;
            z2 = -b0s - a2 * out;
            outBuffer[i] = out;
        }
    }

    void stepNotch(const float *in, float *outBuffer, int size)
    {
        for (int i=0; i < size; ++i)
        {
            float b0s = in[i] * b0;
            float out = b0s + z1;
            z1 = b1 * (in[i] - out) + z2;
            z2 = b0s - a2 * out;
            outBuffer[i] = out;
        }
    }
    void stepLowHiPass(const float *in, float *outBuffer, int size)
    {
        for (int i=0; i < size; ++i)
        {
            float b0s = in[i] * b0;
            float out  = b0s + z1;
            z1        = in[i] * b1 + z2 - a1 * out;
            z2        = - b0s - a2 * out;
            outBuffer[i] = out;
        }
    }
    void stepPeak(const float *in, float *outBuffer, int size)
    {
        for (int i=0; i < size; ++i)
        {
            float out  = in[i] * b0 + z1;
            z1        =  b1 * (in[i] - out) + z2;
            z2        = in[i] * b2 - a2 * out;
            outBuffer[i] = out;
        }
    }

    void stepPassThrough(const float *in, float *outBuffer, int size)
    {
        for (int i=0; i < size; ++i)
        {
            outBuffer[i] = b0*in[i];
        }
    }

    BiQuadCallBack m_cb;
    float b0, b1, b2, a1, a2;
    float z1, z2;
};
