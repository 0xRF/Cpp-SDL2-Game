#include "Key.hpp"

#include "../Components/ImageRenderer.hpp"
#include "../Components/Collider.hpp"
void Key::OnDestroy() {

}

void Key::Start() {

    AddComponent<ImageRenderer>(ImageRenderer("assets/kagari.png"));
    AddComponent<Collider>(GetComponent<ImageRenderer>());
}
