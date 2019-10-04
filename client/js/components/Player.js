const Player = function(props) {
  const x = props.source.x;
  const y = props.source.y;
  const angle = props.source.angle;

  const hp = props.source.health;

  const style = {
    display: (hp>0)?'block':'none',
    transform: `translate(${x}px, ${y}px) rotate(${angle}rad)`,
  };

  return (
    <div className="player" style={style}></div>
  );
};

export default Player;
