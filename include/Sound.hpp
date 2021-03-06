#ifndef SOUND_H
#define SOUND_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define INCLUDE_SDL_MIXER
#include "SDL_include.h"
#include "Component.hpp"
#include "GameData.hpp"
#include "GameObject.hpp"
#include <string>

class Sound : public Component {
    public:
        Sound(GameObject& associated);

        Sound(GameObject& associated, const std::string& file);

        ~Sound();

        void Open(const std::string& file);

        void Play(int times = 1);

        void Stop();

        void Update(float dt) override;

        bool Is(GameData::Types type) const override;

        void Render() override;

        bool IsOpen() const;

        const GameData::Types Type = GameData::Types::Sound;
    private:
        std::shared_ptr<Mix_Chunk> chunk;
        int channel;
};

#endif
