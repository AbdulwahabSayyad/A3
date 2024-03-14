#ifndef SPEAKER_H
#define SPEAKER_H

#include <QString>

using namespace std;

class Speaker
{

public:
    Speaker(int speakerID);
    ~Speaker();

    void playText(const QString &text);
    void controlMic();
    void ringBell();

private:
    int speakerID;
};

#endif // SPEAKER_H
