#ifndef SNDFX_H
#define SNDFX_H

#include <QSoundEffect>

enum SOUND { QUACK, PLONS };

class SndFx
{
public:
    static SndFx& getInstance();
    static void playFX(SOUND snd);

private:
    SndFx();
    QSoundEffect m_sndQuack;
    QSoundEffect m_sndPlons;
    // C++ 03
    // ========
    // Don't forget to declare these two. You want to make sure they
    // are unacceptable otherwise you may accidentally get copies of
    // your singleton appearing.
    //SndFx(SndFx const&);              // Don't Implement
    //void operator=(SndFx const&); // Don't implement

    // C++ 11
    // =======
    // We can use the better technique of deleting the methods
    // we don't want.
public:
    SndFx(SndFx const&)               = delete;
    void operator=(SndFx const&)  = delete;

    // Note: Scott Meyers mentions in his Effective Modern
    //       C++ book, that deleted functions should generally
    //       be public as it results in better error messages
    //       due to the compilers behavior to check accessibility
    //       before deleted status
};

#endif // SNDFX_H
