#pragma once
#include "Entity.h"

struct ParticleGenDetails;
class ParticleSystemComponent;

class Particle :
    public Entity
{
private:
    float mLifespan;
    ParticleSystemComponent& mEmitter;

public:
    Particle(ParticleSystemComponent& emitter);
    ~Particle();

    void Reset();
    void Render(SDL_Renderer& renderer, const ParticleGenDetails& details);
    void Update(float deltaTime, const ParticleGenDetails& details);
};

