/**
 *
 * Audio driver
 *
 * @date 6/21/2019
 * @author hamster
 *
 */

#ifndef MODULE_AUDIO_H
#define MODULE_AUDIO_H

class Audio {

    public:
        Audio();

        void enable(bool enable);
        bool isEnabled();

        bool headphonesInserted();

        void beep(int duration, int frequency);

    private:
        bool enabled;
        bool headphones;

};


#endif //MODULE_AUDIO_H
