#include "sndfx.h"

SndFx::SndFx()
{
    m_sndQuack.setSource(QUrl("qrc:/snd/quack.wav"));
    m_sndQuack.setLoopCount(1);
    m_sndQuack.setVolume(0.5f);

    m_sndPlons.setSource(QUrl("qrc:/snd/plons.wav"));
    m_sndPlons.setLoopCount(1);
    m_sndPlons.setVolume(0.5f);
}

SndFx &SndFx::getInstance()
{
    static SndFx    instance; // Guaranteed to be destroyed.
                              // Instantiated on first use.
    return instance;
}

void SndFx::playFX(SOUND snd)
{
    switch(snd)
    {
        case QUACK  : getInstance().m_sndQuack.play(); break;
        case PLONS  : getInstance().m_sndPlons.play(); break;
    }

}
