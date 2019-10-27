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

          const previousTickHealth = parseFloat(previous.health);
          const currentTickHealth = parseFloat(newEntity.health);

          if (previousTickHealth > currentTickHealth) {
            return {
              name: 'blood',
              lifeTime: 2000,
              x: newEntity.x,
              y: newEntity.y,
            };
          }

          break;
        case 'AreaHitter':
          //Crowbar swing
          return {
            name: 'crowbarSwing',
            lifeTime: 500,
            x: newEntity.x,
            y: newEntity.y,
          };
          break;
        default:
          return null;
      }

      return null;
    });
  }

  update = newEntities => {
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