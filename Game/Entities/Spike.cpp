//
// Created by xrf on 5/10/18.
//

#include "Spike.hpp"
#include "../Components/ImageRenderer.hpp"
#include "../Components/Collider.hpp"
#include "../../Engine/Engine.hpp"
void Spike::OnDestroy() {

}

void Spike::Start() {
    AddComponent<ImageRenderer>(ImageRenderer("assets/spike.png"));
    AddComponent<Collider>(Collider(GetComponent<ImageRenderer>()));
}
