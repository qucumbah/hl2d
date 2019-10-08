import Particle from 'Particle.js';

class ParticleSystem extends React.Component {
  constructor(props) {
    super(props);

    this.state = {
      particles: [],
    };
  }

  addAllParticles = newParticles => {
    const allParticles = this.state.particles;
    newParticles.forEach( newParticle => allParticles.push(newParticle) );
    this.setState({ particleNames: allParticles });
  }

  queueParticleDeath = particle => {
    setTimeout(() => this.removeParticle(particle), particle.lifeTime);
  }

  removeParticle = removable => {
    particles.filter(particle => particle !== removable);
  }

  mapParticles = particles => {
    return particles.map(particle => {
      //Return some particle
    });
  }

  render() {
    const newParticles = this.props.particleNames;
    if (newParticles) {
      this.addAllParticleNames(this.props.particleNames);
      newParticles.forEach(queueParticleDeath);
    }

    const particleComponents = mapParticles(this.state.particles);

    return (
      <div className="particleSystem">
        {particleComponents}
      </div>
    );
  }
}
