const Connect = function(props) {
  const adressRef = React.createRef();
  const nameRef = React.createRef();

  const handleConnectAttempt = () => {
    const adress = adressRef.current.value;
    const name = nameRef.current.value;

    props.onConnectAttempt(adress, name);
  }

  return (
    <div className="connect">
      <div className="label">Server adress:</div>
      <input defaultValue={props.lastAdress} ref={adressRef}></input>
      <div className="label">Your nickname:</div>
      <input defaultValue={props.lastName} ref={nameRef}></input>
      <button
        className="connectButton"
        onClick={handleConnectAttempt}
        disabled={props.isConnecting}
      >
        Connect
      </button>
    </div>
  );
}

export default Connect;
