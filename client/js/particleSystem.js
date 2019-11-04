class ParticleSystem {
  constructor() {
    this.particles = [];
    this.prevEntities = null;
    this.nextId = 0;
  }

  addParticle = particle => {
    this.particles.push(particle);
  }

  removeParticle = removable => {
    this.particles = this.particles.filter(particle => particle !== removable);
  }

  queueParticleDeath = particle => {
    setTimeout(() => this.removeParticle(particle), particle.lifeTime);
  }

  getNewParticles = (prevEntities, newEntities) => {
    return newEntities.map(newEntity => {
      switch (newEntity.type) {
        case 'Player':
          //Player can bleed if he gets hit (if his HP decreases)
          if (!prevEntities) {
            //First tick, cant know if player got hit
            return null;
          }

          const previous = prevEntities.find(prevEntity => 
            prevEntity.type === 'Player' && prevEntity.id === newEntity.id
          );

          if (!previous) {
            //Player just connected (his first tick); cant be hit yet
            return null;
          }

          if (previous.health > newEntity.health) {
            return {
              type: 'sprite',
              name: 'blood',
              lifeTime: 2000,
              x: newEntity.x,
              y: newEntity.y,
            };
          }

          break;
        case 'AreaHitter':
          //Crowbar swing
          if (!newEntity.renderable) {
            return null;
          }
          
          return {
            type: 'sprite',
            name: 'crowbarSwing',
            lifeTime: 300,
            x: newEntity.x,
            y: newEntity.y,
            angle: newEntity.angle,
          };
          break;
        case 'LineHitter':
          if (!newEntity.renderable) {
            return null;
          }

          return {
            type: 'line',
            name: 'bulletTrajectory',
            lifeTime: 500,
            x: newEntity.x,
            y: newEntity.y,
            width: newEntity.radius,
            height: 2,
            angle: newEntity.angle,
          };
          break;
        case 'Rocket':
          if (!newEntity.exploded) {
            return null;
          }

          return {
            type: 'sprite',
            name: 'explosion',
            lifeTime: 1000,
            x: newEntity.x,
            y: newEntity.y,
            angle: newEntity.angle,
          }
        default:
          return null;
      }

      return null;
    });
  }

  update = newEntities => {
    if (!newEntities) {
      return;
    }

    const newParticles = (
      this.getNewParticles(this.prevEntities, newEntities)
      .filter(particle => particle !== null)
    );

    newParticles.forEach(particle => {
      particle.id = this.nextId++;
      this.queueParticleDeath(particle);

      this.addParticle(particle);
    });

    this.prevEntities = newEntities;
  }

  getParticles = () => {
    return this.particles;
  }
}

const particleSystem = new ParticleSystem();
export default particleSystem;