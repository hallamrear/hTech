#pragma once
#include "Entity.h"

struct ParticleGenDetails;
class ParticleSystemComponent;
class BoundingBox;

class Particle :
    public Entity
{
private:
    float mLifespan;
    ParticleSystemComponent& mEmitter;
    BoundingBox* mCollider;

public:
    Particle(ParticleSystemComponent& emitter);
    ~Particle();

    void Reset();
    void Render(SDL_Renderer& renderer, const ParticleGenDetails& details);
    void Update(float deltaTime, const ParticleGenDetails& details);
};

