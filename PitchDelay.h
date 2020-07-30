//#pragma once

#define SHIFTADVANCE 12
typedef int32_t Q16;

/*
 * octave play out delay
 */
class PitchDelay
{
public:
    PitchDelay()
            : m_size(0), m_head(0), m_buffer(nullptr),
              m_fadeTime(256), m_fadeStep(1 << (27 - 8)), m_fade(false)
    {
    }

    ~PitchDelay()
    {
        // sdram_free(m_buffer);
    }

    // with axoloti core RAII not possible, therefor you need to call init
    void init_delay(uint32_t size)
    {
        if (size & 1)
            size++;
        if (m_buffer)
        {
            LogTextMessage("WARNING! Buffer was already allocated, don't reuse objects");
            // sdram_free(m_buffer);
        }
        m_buffer = (int32_t *) sdram_malloc(sizeof(int32_t) * size);
        m_size = size;
        memset(m_buffer, 0, sizeof(int32_t) * m_size);
        m_head = 0;
        LogTextMessage("Octave Delay Buffer size: %d", m_size);
        m_fadeInPos = (m_size / 4) << SHIFTADVANCE;
        m_fadeOutPos = (size * 3 / 4) << SHIFTADVANCE;
        m_fadeTime = 256;
        m_fadeStep = float_to_q27(1.0 / (m_fadeTime / 2));
        m_fadeStep <<= 3;
        m_fdbkGain = float_to_q27(0.1);
    }

    void setFadeTime(int32_t bits)
    {
        m_fadeTime = 1 << bits;
        m_fadeStep = float_to_q27(1.0 / (m_fadeTime));
        m_fadeStep <<= 3;
        resetFadeTimeAdjusted();
    }

    void resetFadeTimeAdjusted()
    {
        if (m_advance >= 1<<SHIFTADVANCE)
        {
            m_fadeTimePitchAdjusted = ((m_fadeTime * m_advance)>>SHIFTADVANCE)-m_fadeTime+1;
//            m_fadeTimePitchAdjusted = (m_fadeTime * (m_advance - (1 << SHIFTADVANCE))) >> SHIFTADVANCE;
        }
        else
        {
            m_fadeTimePitchAdjusted = m_fadeTime-((m_fadeTime * m_advance)>>SHIFTADVANCE)+1;
        }
    }


    void setFeedback(int32_t value)
    {
        m_fdbkGain = value;
    }

    void triggerFade()
    {
        m_fade = true;
        m_fadeOutPos = m_fadeInPos;
        m_fadeCount = m_fadeTime;
        m_fadeInGain = 0;
        m_fadeOutGain = 1 << 27;
        m_fadeOutGain <<= 3;
        //LogTextMessage("fadeinpos: %d head=%d, ft=%d s=%f",m_fadeInPos, m_head, m_fadeTime, q27_to_float(m_fadeStep>>3));
    }


    void inline advanceFade(Q16 &fadePos )
    {
        if (m_reverse)
        {
            fadePos -= m_advance;
            if (fadePos >> SHIFTADVANCE < 0)
            {
                fadePos += m_size << SHIFTADVANCE;
            }

        } else{
            fadePos += m_advance;
            if (fadePos >> SHIFTADVANCE >= m_size)
            {
                fadePos -= m_size << SHIFTADVANCE;
            }
        }
    }


    int32_t inline step(int32_t in)
    {
        int32_t valout;
        if (!m_fade)
        {
            valout = m_buffer[m_fadeInPos >> SHIFTADVANCE];
            if (m_advance >= (1 << SHIFTADVANCE)) // pitch up
            {
                int delta = m_head - (m_fadeInPos >> SHIFTADVANCE);
                if (delta < 0)
                    delta += m_size;
                if (m_reverse)
                {
                    if (delta<3)
                    {
                        triggerFade();
                        int newMaterialIn = m_head + m_size / 2;
                        if (newMaterialIn >= m_size)
                            newMaterialIn -= m_size;
                        m_fadeInPos = newMaterialIn << SHIFTADVANCE;
                    }
                }
                else
                {
                    if (delta <= m_fadeTimePitchAdjusted)
                    {
                        triggerFade();
                        int newMaterialIn = m_head + m_size / 2;
                        if (newMaterialIn >= m_size)
                            newMaterialIn -= m_size;
                        m_fadeInPos = newMaterialIn << SHIFTADVANCE;
                        //LogTextMessage("Up: hd %d fin %d fout %d", m_head, m_fadeInPos>>SHIFTADVANCE, m_fadeOutPos>>SHIFTADVANCE);

                    }
                }
            } else // pitching down
            {
                int delta = (m_fadeInPos >> SHIFTADVANCE)-m_head;
                if (delta < 0)
                    delta += m_size;
                if (m_reverse)
                {
                    if (delta<3)
                    {
                        triggerFade();
                        int newMaterialIn = m_head + m_size / 2;
                        if (newMaterialIn >= m_size)
                            newMaterialIn -= m_size;
                        m_fadeInPos = newMaterialIn << SHIFTADVANCE;
                    }
                }
                else
                {
                    if (delta <= m_fadeTimePitchAdjusted)
                    {
                        triggerFade();
                        int newMaterialIn = m_head + m_size / 2;
                        if (newMaterialIn >= m_size)
                            newMaterialIn -= m_size;
                        m_fadeInPos = newMaterialIn << SHIFTADVANCE;
                        //LogTextMessage("Down: hd %d fin %d fout %d", m_head, m_fadeInPos>>SHIFTADVANCE, m_fadeOutPos>>SHIFTADVANCE);
                    }
                }
            }


        } else
        {
            // LogTextMessage("Fading fout:%4d fin:%4d head:%4d %f %f", m_fadeOutPos>> SHIFTADVANCE, m_fadeInPos>> SHIFTADVANCE, m_head, q27_to_float(m_fadeInGain),q27_to_float(m_fadeOutGain));
            int32_t tmp = ___SMMUL(m_buffer[m_fadeInPos >> SHIFTADVANCE] << 2, m_fadeInGain);
            valout = ___SMMLA(m_buffer[m_fadeOutPos >> SHIFTADVANCE] << 2, m_fadeOutGain, tmp);
            m_fadeInGain += m_fadeStep;
            m_fadeOutGain -= m_fadeStep;
            advanceFade(m_fadeOutPos);
            m_fadeCount--;
            if (m_fadeCount <= 0)
            {
                //LogTextMessage("Done hd %d fin %d fout %d", m_head, m_fadeInPos>>SHIFTADVANCE, m_fadeOutPos>>SHIFTADVANCE);
                m_fade = false;
            }
        }
        advanceFade(m_fadeInPos);
        m_buffer[m_head++] = ___SMMLA(valout << 2, m_fdbkGain << 3, in);;
        if (m_head >= m_size)
        {
            m_head = 0;
        }
        return valout;
    }

    void setPitch(int32_t ratio)
    {
        if (ratio == 0)
            return;
        if (ratio < 0)
        {
            m_reverse = true;
            ratio = -ratio;
        }
        else
            m_reverse = false;
        m_advance = ratio >> (27 - SHIFTADVANCE-1);
        resetFadeTimeAdjusted();
    }

    void setPitch(float ratio)
    {
        if (ratio == 0.0)
            return;
        if (ratio < 0.0)
        {
            m_reverse = true;
            ratio = -ratio;
        }
        else
            m_reverse = false;
        m_advance = (int32_t)(ratio*(1<<SHIFTADVANCE));
        resetFadeTimeAdjusted();
    }

    Q16 m_fadeInPos; // q16 (for now)
    Q16 m_fadeOutPos;
    Q16 m_advance;
    uint32_t m_head;
    uint32_t m_size;
    uint32_t m_fadeTimePitchAdjusted;

protected:
    int32_t *m_buffer;
    uint32_t m_fdbkGain;
    uint32_t m_fadeInGain;
    uint32_t m_fadeOutGain;
    uint32_t m_fadeTime;
    uint32_t m_fadeCount;
    uint32_t m_fadeStep;
    uint32_t m_mix;
    bool m_fade;
    bool m_reverse;
};
