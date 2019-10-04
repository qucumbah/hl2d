//Might need later, for now default HTML buttons will do 

const Button = function(props) {
  return (
    <button
      className="connectButton"
      disabled={props.disabled}
    >{props.text?props.text:props.image}</button>
  );
}

export default Button;
