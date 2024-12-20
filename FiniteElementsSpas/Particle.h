#pragma once

struct Particle {
    glm::vec2 Position, Velocity;
    glm::vec4 Color;
    float Life;

    Particle(){
        Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f)
    };
};

unsigned int nr_particles = 500;
std::vector<Particle> particles;

for (unsigned int i = 0; i < nr_particles; ++i)
    particles.push_back(Particle());

unsigned int nr_new_particles = 2;
// add new particles
for (unsigned int i = 0; i < nr_new_particles; ++i)
{
    int unusedParticle = FirstUnusedParticle();
    RespawnParticle(particles[unusedParticle], object, offset);
}
// update all particles
for (unsigned int i = 0; i < nr_particles; ++i)
{
    Particle& p = particles[i];
    p.Life -= dt; // reduce life
    if (p.Life > 0.0f)
    {	// particle is alive, thus update
        p.Position -= p.Velocity * dt;
        p.Color.a -= dt * 2.5f;
    }
}