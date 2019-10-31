const Particle = function({source}) {
  const imagePath = './img/' + source.name + '.png';

  const style = {
    left: source.x + 'px',
    top: source.y + 'px',
    animationDuration: source.lifeTime + 'ms',
  };

  if (source.angle) {
    style.transform = 'rotate(' + source.angle + 'rad)';
  }

  return (
    <div className="particle" style={style}>
      <img src={imagePath} alt="" />
    </div>
  );
}

Particle.propTypes = {
  source: PropTypes.shape({
    x: PropTypes.number,
    y: PropTypes.number,
    name: PropTypes.string,
    lifeTime: PropTypes.number,
  }).isRequired,
};

export default Particle;
