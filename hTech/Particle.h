#pragma once
#include "Entity.h"

struct ParticleGenDetails;

class Particle :
    public Entity
{
private:
    float mLifespan;

public:
    Particle();
    ~Particle();

    void Reset();
    void Render(SDL_Renderer& renderer, const ParticleGenDetails& details);
    void Update(float deltaTime, const ParticleGenDetails& details);
};

