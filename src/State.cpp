#include "State.hpp"
#include "Face.hpp"
#include "Sprite.hpp"
#include "TileSet.hpp"
#include "TileMap.hpp"
#include "GameObject.hpp"
#include "Component.hpp"
#include "Vec2.hpp"
#include "Sound.hpp"
#include "Music.hpp"
#include <string>
#include <cmath>
#include <algorithm>
#include <memory>

State::State() : quitRequested(false) {
    GameObject* go = new GameObject();
    Sprite* background = new Sprite(*go, "assets/img/ocean.jpg");
    go->AddComponent(background);
    objects.emplace_back(go);

    GameObject* mapGo = new GameObject();
	tileSet = new TileSet(64, 64, "assets/img/tileset.png");
    TileMap* map = new TileMap(*mapGo, "assets/map/tileMap.txt", tileSet);
    mapGo->AddComponent(map);
	objects.emplace_back(mapGo);

    music.Open("assets/audio/stageState.ogg");
	music.Play();
}

State::~State() {
	delete tileSet;
    objects.clear();
}

void State::Input() {
	SDL_Event event;
	int mouseX, mouseY;

	// Obtenha as coordenadas do mouse
	SDL_GetMouseState(&mouseX, &mouseY);

	// SDL_PollEvent retorna 1 se encontrar eventos, zero caso contrário
	while (SDL_PollEvent(&event)) {

		// Se o evento for quit, setar a flag para terminação
		if (event.type == SDL_QUIT) {
			quitRequested = true;
		}
		
		// Se o evento for clique...
		if (event.type == SDL_MOUSEBUTTONDOWN) {

			// Percorrer de trás pra frente pra sempre clicar no objeto mais de cima
			for (int i = objects.size() - 1; i >= 0; --i) {
				// Obtem o ponteiro e casta pra Face.
				GameObject* go = (GameObject*) objects[i].get();
				// Nota: Desencapsular o ponteiro é algo que devemos evitar ao máximo.
				// O propósito do shared_ptr é manter apenas uma cópia daquele ponteiro,
				// ao usar get(), violamos esse princípio e estamos menos seguros.
				// Esse código, assim como a classe Face, é provisório. Futuramente, para
				// chamar funções de GameObjects, use objects[i]->função() direto.

				if (go->box.Contains({(float)mouseX, (float)mouseY}) ) {
					auto component = go->GetComponent("Face");
					if (auto componentSp = component.lock()) {
						auto face = std::dynamic_pointer_cast<Face>(componentSp);
						// Aplica dano
						face->Damage(std::rand() % 10 + 10);
						// Sai do loop (só queremos acertar um)
						break;
					}
				}
			}
		}
		if (event.type == SDL_KEYDOWN) {
			// Se a tecla for ESC, setar a flag de quit
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				quitRequested = true;
			}
			// Se não, crie um objeto
			else {
				Vec2 objPos = Vec2(200, 0).GetRotated( -M_PI + M_PI*(rand() % 1001)/500.0 ) + Vec2(mouseX, mouseY);
				AddObject((int)objPos.x, (int)objPos.y);
			}
		}
	}
}

void State::LoadAssets() {
}

void State::Update(float dt) {
    Input();

    std::for_each(
        objects.begin(),
        objects.end(),
        [&](std::shared_ptr<GameObject>& go) { go->Update(dt); }
    );

    objects.erase(
        std::remove_if(
            objects.begin(),
            objects.end(),
            [](std::shared_ptr<GameObject>& go) { return go->IsDead(); }
        ),
        objects.end()
    );
}

void State::Render() {
    std::for_each(
        objects.begin(),
        objects.end(),
        [](std::shared_ptr<GameObject>& go) { go->Render(); }
    );
}

bool State::QuitRequested() const {
    return quitRequested;
}

void State::AddObject(int mouseX, int mouseY) {
    GameObject* go = new GameObject();
    Sprite* penguinSprite = new Sprite(*go, "assets/img/penguinface.png");
    Sound* penguinSound = new Sound(*go, "assets/audio/boom.wav");
    Face* penguinFace = new Face(*go);

	go->box = go->box.GetCentered(mouseX, mouseY);
    go->AddComponent(penguinSprite);
    go->AddComponent(penguinSound);
    go->AddComponent(penguinFace);

    objects.emplace_back(go);
}
