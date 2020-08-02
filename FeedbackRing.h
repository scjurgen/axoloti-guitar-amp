
#pragma once

class FeedbackRing
{
public:
    FeedbackRing()
            : m_size(0)
            , m_head(0)
            , m_gain(0)
            , m_buffer(0)
    {
    }
    
    ~FeedbackRing()
    {
        dispose();
    }
    void dispose()
    {
        if (m_buffer)
            ax_free(m_buffer);
        m_buffer = 0;       
    }

    void init(int size)
    {
        m_size = size;
        m_buffer = (int32_t *) sdram_malloc(sizeof(int32_t)*m_size);
        memset(m_buffer,0, sizeof(int32_t)*m_size);
        m_head = 0;
        m_adjustGain = false;
        biquad_clearstate(&bs);
    }

    void gain(int32_t gain_q27, bool adjustGain=false)
    {
        if (float_to_q27(0.9) <= gain_q27)
            gain_q27 = float_to_q27(0.9);
        m_gain = gain_q27<<3;
        m_outgain = 8<<27;
        if (adjustGain)
        {
            adjustOutputGain();
        }
    }

    void setLowPass(int32_t pitch, int32_t reso)
    {
        int32_t freq;
        MTOF(pitch,freq);
        biquad_lp_coefs(&bc,freq,INT_MAX - (__USAT(reso,27)<<4));
    }

    void adjustOutputGain()
    {
        const float cx=1.0, cy=0.35, cd=0.73;
        float currentAdjust = 1.0/((1.0f / (cx-fabs(q27_to_float(m_gain>>3)))) * cd + cy);
        m_outgain = float_to_q27(currentAdjust)<<3;
        m_adjustGain = true;
    }


    int32_t tap(int32_t rel_position)
    {
        int h=m_head+rel_position;
        if (h >= m_size)
            h-=m_size;
        return m_buffer[h];
    }

    int32_t update(bool lp, const int32buffer in, int32buffer &out, int32_t tap1Pos, int32buffer &tap1, int32_t tap2Pos,int32buffer &tap2)
    {
        int32buffer tmp,ret, filtered;
        int head = m_head;
        for (int i=0; i < BUFSIZE; ++i)
        {
            ret[i] = ___SMMLA(in[i]<<2, m_gain, m_buffer[head]);
            int32_t satValue = ___SMMLA(m_buffer[head]<<2, -m_gain, in[i]); // N.B. negativ feedback
            tmp[i] = satValue;
            head++;
            if (head >= m_size)
                head = 0;
        }
        if (lp)
            biquad_dsp(&bs,&bc,tmp,filtered);
        else
            memcpy(filtered, tmp, sizeof(tmp));

        for (int i=0; i < BUFSIZE; ++i)
        {
            m_buffer[m_head++] = filtered[i];
            if (m_head >= m_size)
                m_head = 0;
            if (m_adjustGain)
            {
                ret[i] = ___SMMUL(ret[i], m_outgain)<<2;
            }
            out[i] = ret[i];
            tap1[i] = tap(tap1Pos);
            tap2[i] = tap(tap2Pos);
        }
    }


private:
    int32_t m_size;
    int32_t m_head;
    int32_t m_gain;
    int32_t m_outgain;
    bool m_adjustGain;
    int32_t *m_buffer;
    biquad_state bs;
    biquad_coefficients bc;
};

