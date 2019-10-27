const Particle = function({source}) {
  const imagePath = './img/' + source.name + '.png';
  const style = {
    left: source.x + 'px',
    top: source.y + 'px',
  };
  return (
    <div className="particle" style={style}>
      <img src={imagePath} alt="" />
    </div>
  );
}

export default Particle;
