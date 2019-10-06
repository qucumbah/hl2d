const Wall = function(props) {
  const style = {
    left: props.source.x + 'px',
    top: props.source.y + 'px',
    width: props.source.width + 'px',
    height: props.source.height + 'px'
  };

  return <div className="wall" style={style}></div>;
}

export default Wall;
