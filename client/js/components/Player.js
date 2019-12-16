import GunIcon from './GunIcon.js';

const Player = function(props) {
  const {x, y, angle, health, name, currentGun} = props.source;
  //console.log(props.source);

  const playerContainerStyle = {
    display: (health > 0) ? 'block' : 'none',
    left: x + 'px',
    top: y + 'px',
  }
  const playerSpriteStyle = {
    transform: `translate(-50%, -50%) rotate(${angle}rad)`,
  };

  return (
    <div className="player" style={playerContainerStyle}>
      <div className="info">
        <div className="name">{name}<br />HP:{health}</div>
        <div className="health"></div>
        <GunIcon gunId={currentGun} size="30px" />
      </div>
      <div className="sprite" style={playerSpriteStyle}></div>
    </div>
  );
};

Player.propTypes = {
  source: PropTypes.shape({
    renderable: PropTypes.bool.isRequired,
    x: PropTypes.number.isRequired,
    y: PropTypes.number.isRequired,
    angle: PropTypes.number.isRequired,
    health: PropTypes.number.isRequired,
    name: PropTypes.string.isRequired,
    currentGun: PropTypes.number.isRequired,
  }).isRequired
};

export default Player;
