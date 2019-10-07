const Player = function(props) {
  const x = props.source.x;
  const y = props.source.y;
  const angle = props.source.angle;

  const hp = props.source.health;

  const style = {
    display: (hp>0)?'block':'none',
    transform: `translate(-50%, -50%) rotate(${angle}rad)`,
    left: x + 'px',
    top: y + 'px',
  };

  return (
    <div className="player" style={style}></div>
  );
};

export default Player;
