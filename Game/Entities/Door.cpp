#include "Door.hpp"

#include "../Components/ImageRenderer.hpp"
#include "../Components/Collider.hpp"
void Door::OnDestroy() {

}

void Door::Start() {

    AddComponent<ImageRenderer>(ImageRenderer("assets/duntiles.png", {0,0, 32, 64}));
    AddComponent<Collider>(GetComponent<ImageRenderer>());
    this->scale = 2.0f;
}
