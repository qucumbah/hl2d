//Had to rename this to level because of built-in Map type
import Wall from './Wall.js';

const Level = function(props) {
  const walls = props.source.map((wall, index) => {
    return <Wall source={wall} key={"wall" + index} />;
  });

  //console.log(walls);

  return <div className="level">{walls}</div>;
}

Level.propTypes = {
  source: PropTypes.arrayOf(
    PropTypes.shape({
      x: PropTypes.number,
      y: PropTypes.number,
      width: PropTypes.number,
      height: PropTypes.number,
    })
  ).isRequired,
};

export default Level;
