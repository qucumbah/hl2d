import Player from './Player.js';
import Level from './Level.js';
import Particle from './Particle.js';

const TICKTIME = 10;

//Constants for player's actions; the release actions have the same IDs but they
//are negative, e.g. click is encoded as:
//11, -11 (attack1 pressed, attack1 released)
const UP = 1;
const RIGHT = 2;
const DOWN = 3;
const LEFT = 4;

const GUN1 = 5;
const GUN2 = 6;
const GUN3 = 7;
const GUN4 = 8;
const GUN5 = 9;
const GUN6 = 10;

const ATTACK1 = 11;
const ATTACK2 = 12;

//Player's mappings
const keyboardMappings = {
  87: UP, //w
  68: RIGHT, //d
  83: DOWN, //s
  65: LEFT, //a

  49: GUN1, //1
  50: GUN2, //2
  51: GUN3, //3
  52: GUN4, //4
  53: GUN5, //5
  54: GUN6, //6
};

const mouseMappings = {
  0: ATTACK1, //lmb
  2: ATTACK2, //rmb
};

class Game extends React.Component {
  constructor(props) {
    super(props);

    this.state = {
      interval: null,
      actions: [],
      crosshairPosition: { x: 0, y: 0 },
    };

    //Only need this to focus, autoFocus property doesn't work
    this.gameRef = React.createRef();
  }

  componentDidMount = () => {
    this.gameRef.current.focus();
    this.startTickInterval();
  }
  componentWillUnmount = () => {
    this.stopTickInterval();
  }

  startTickInterval = () => {
    const interval = setInterval(() => {
      const actions = this.state.actions;
      this.setState({ actions: [] });

      const crosshairPosition = this.state.crosshairPosition

      this.props.onTick(crosshairPosition, actions);
    }, TICKTIME);

    this.setState({ interval });
  }
  stopTickInterval = () => {
    clearInterval(this.state.interval);
    this.setState({ interval: null });
  }

  handleKeyDown = event => {
    const keyCode = event.keyCode;

    if (keyboardMappings[keyCode]) {
      this.recordAction( +keyboardMappings[keyCode] );
    }
  }
  handleKeyUp = event => {
    const keyCode = event.keyCode;

    if (keyboardMappings[keyCode]) {
      this.recordAction( -keyboardMappings[keyCode] );
    }
  }

  handleMouseDown = event => {
    const button = event.button;

    if (mouseMappings[button]) {
      this.recordAction( +mouseMappings[button] );
    }
  }
  handleMouseUp = event => {
    const button = event.button;

    if (mouseMappings[button]) {
      this.recordAction( -mouseMappings[button] );
    }
  }

  recordAction = actionCode => {
    this.state.actions.push(actionCode);
  }

  handleMouseMove = event => {
    this.recordCrosshairPosition(event.pageX, event.pageY);
    //console.log(event.pageX, event.pageY);
  }

  recordCrosshairPosition = (x, y) => {
    this.setState({ crosshairPosition: {x, y} });
  }

  mapEntities = entities => {
    return entities.map(entity => {
      if (!entity.renderable) {
        return null;
      }

      const key = entity.type + entity.id;

      switch (entity.type) {
        case 'Player':
          return <Player source={entity} key={key} />;
          break;
        case 'Projectile':
          const projectileSource = {
            type: 'sprite',
            name: 'crossbowBolt',
            lifeTime: 100000,
            x: entity.x,
            y: entity.y,
            angle: entity.angle,
          };

          return <Particle source={projectileSource} key={key} />
          break;
        case 'Rocket':
          const rocketSource = {
            type: 'sprite',
            name: 'rocket',
            lifeTime: 100000,
            x: entity.x,
            y: entity.y,
            angle: entity.angle,
          };

          return <Particle source={rocketSource} key={key} />
          break;
        case 'LaserPointer':
          const laserSource =  {
            type: 'sprite',
            name: 'laser',
            lifeTime: 100000,
            x: entity.x,
            y: entity.y,
          };

          return <Particle source={laserSource} key={key} />
          break;
        default:
          return null;
      }
    });
  }

  mapParticles = particles => {
    return particles.map(particle => (
      <Particle source={particle} key={particle.id} />
    ));
  }

  render() {
    let entities, particles;
    if (!this.props.entities) {
      entities = null;
    } else {
      entities = this.mapEntities(this.props.entities);
    }

    if (!this.props.particles) {
      particles = null;
    } else {
      particles = this.mapParticles(this.props.particles);
    }

    return (
      <div
        className="game"
        tabIndex="-1"
        onKeyDown={this.handleKeyDown}
        onKeyUp={this.handleKeyUp}
        onMouseMove={this.handleMouseMove}
        onMouseDown={this.handleMouseDown}
        onMouseUp={this.handleMouseUp}
        ref={this.gameRef}
        onContextMenu={event => event.preventDefault()} //Block rmb context menu
      >
        {entities}
        {particles}
        <Level source={this.props.map} />
      </div>
    );
  }
}

Game.propTypes = {
  map: PropTypes.arrayOf(
    PropTypes.shape({
      x: PropTypes.number,
      y: PropTypes.number,
      width: PropTypes.number,
      height: PropTypes.number,
    })
  ).isRequired,
  entities: PropTypes.arrayOf(
    PropTypes.shape({ renderable: PropTypes.bool })
  ),
  particles: PropTypes.arrayOf(
    PropTypes.shape({
      x: PropTypes.number,
      y: PropTypes.number,
      name: PropTypes.string,
      lifeTime: PropTypes.number,
    })
  ),
  playerId: PropTypes.number,
  onTick: PropTypes.func.isRequired,
};

Game.defaultProps = {
  entities: null,
  particles: null,
  playerId: -1,
};

export default Game;
