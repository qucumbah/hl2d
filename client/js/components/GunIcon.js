const gunNames = [
  'crowbar', 'rifle', 'shotgun', 'gauss', 'crossbow', 'rocketLauncher'
];

const GunIcon = function(props) {
  const style = {
    width: props.size,
    height: props.size,
  };

  const iconPath = './img/guns/' + gunNames[props.gunId] + '.png';
  return <img src={iconPath} style={style} />;
};

export default GunIcon;
