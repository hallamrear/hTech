#pragma once
#include "Component.h"
#include "Colour.h"
#include "Particle.h"

#define PARTICLE_COUNT 50

class Colour;

struct ParticleGenDetails
{
    unsigned int duration = 0.0f;
    unsigned int weight = 1;
    Colour colour = Colour(255, 255, 255, 255);
    bool looping = true;
    bool playOnStart = true;
    int width = 2;
    int height = 2;

    ParticleGenDetails()
    {

    }
};

class ParticleSystemComponent :
    public Component
{
private:
    Particle* mParticles[PARTICLE_COUNT];
    std::vector<Particle*> mLivingParticles;
    std::vector<Particle*> mDeadParticles;

    ParticleGenDetails mParticleSystemDetails;
    bool mIsRunning;

public:
    ParticleSystemComponent(Entity& entity, ParticleGenDetails variables = ParticleGenDetails());
    ~ParticleSystemComponent();

    void SetDetails(ParticleGenDetails details);
    ParticleGenDetails& GetDetails();

    void Update(float deltaTime);
    void Render(SDL_Renderer& renderer);

    void Play();
    void Pause();
};

