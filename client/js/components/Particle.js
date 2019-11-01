const Particle = function({source}) {
  const imagePath = './img/' + source.name + '.png';

  let style;
  if (source.type === 'sprite') {
    style = {
      left: source.x + 'px',
      top: source.y + 'px',
      animationDuration: source.lifeTime + 'ms',
      transform: source.angle ? 'rotate(' + source.angle + 'rad)' : 'none',
    };
  } else if (source.type === 'line') {
    style = {
      left: source.x + 'px',
      top: source.y + 'px',
      animationDuration: source.lifeTime + 'ms',
      transform: source.angle ? 'rotate(' + source.angle + 'rad)' : 'none',
      transformOrigin: 'center left',
      width: source.width + 'px',
      height: source.height + 'px',
    };
  } else {
    style = { display: 'none' };
  }

  return (
    <div className="particle" style={style}>
      <img className={source.type} src={imagePath} alt="" />
    </div>
  );
}

Particle.propTypes = {
  source: PropTypes.shape({
    type: PropTypes.string.isRequired,
    x: PropTypes.number.isRequired,
    y: PropTypes.number.isRequired,
    name: PropTypes.string.isRequired,
    lifeTime: PropTypes.number.isRequired,
  }).isRequired,
};

export default Particle;
