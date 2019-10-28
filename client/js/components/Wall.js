const Wall = function(props) {
  const style = {
    left: props.source.x + 'px',
    top: props.source.y + 'px',
    width: props.source.width + 'px',
    height: props.source.height + 'px'
  };

  return <div className="wall" style={style}></div>;
}

Wall.propTypes = {
  source: PropTypes.shape({
    x: PropTypes.number,
    y: PropTypes.number,
    width: PropTypes.number,
    height: PropTypes.number,
  }).isRequired,
};

export default Wall;
