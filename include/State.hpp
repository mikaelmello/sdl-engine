#ifndef STATE_H
#define STATE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "GameObject.hpp"
#include "Sprite.hpp"
#include "Music.hpp"
#include <string>
#include <vector>
#include <memory>

class State {
    public:
        State();

        ~State();

        bool QuitRequested() const;

        void LoadAssets();

        void Update(float dt);

        void Render();
    private:
        void Input();

        void AddObject(int mouseX, int mouseY);

        std::vector<std::shared_ptr<GameObject>> objects;
        Music music;
        bool quitRequested;
};

#endif
