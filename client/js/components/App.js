import Connect from './Connect.js';
import Game from './Game.js';

class App extends React.Component {
  constructor() {
    super();

    const defaultAdress = 'ws://127.0.0.1:8000/ws';

    this.state = {
      connectionState: WebSocket.CLOSED,
      ws: null,
      lastAdress: localStorage.getItem('lastAdress') || defaultAdress,
      lastName: localStorage.getItem('lastName') || 'Player',
      lastError: '',
      map: null,
      playerId: -1,
      entities: null,
    };

    window.onbeforeunload = () => {
      localStorage.setItem('lastAdress', this.state.lastAdress);
      localStorage.setItem('lastName', this.state.lastName);
      this.handleDisconnect('clientOff');
    };
  }

  handleConnectAttempt = (adress, name) => {
    this.displayError(''); //Clear error output

    const ws = new WebSocket(adress);
    ws.onopen = () => this.handleConnect(ws, adress, name);
    ws.onerror = () => this.handleDisconnect('connectFail', adress);
    ws.onclose = event => {
      console.log(event);
      if (event.code === 1000) {
        this.handleDisconnect('serverOff');
      } else {
        this.handleDisconnect('serverError');
      }
    };

    this.setState({ connectionState: WebSocket.CONNECTING });
  }

  handleConnect = (ws, adress, name) => {
    const request = `
      getMapInfo
      name ${name}
    `;

    //In the first request we ask the server for map info and at the same time
    //send the desired name; if there is more than one player with same name at
    //the same time server adds user's id to the name of the last player who
    //connected

    const handleFirstResponse = response => {
      //First line of response has to contain this:
      //id {userId}\n
      //Later on we'll use this ID to know which of the players is us (otherwise
      //we wouldn't know that because server sends info about all the players
      //in the same format)

      const message = response.data;
      if (!message.startsWith('id ')) {
        //Wrong response; this could happen if the server sends tick info after
        //the player has been confirmed but before it forms and sends the
        //response to player's first request; skip this one
        console.log('Skipping wrong response:', message);
        return;
      }

      const id = parseInt(message.slice(
        message.indexOf(' ') + 1,
        message.indexOf('\n')
      ));

      //After the first line comes JSON string containing info about map
      const mapString = message.slice(message.indexOf('\n') + 1);
      let mapObj;
      try {
        mapObj = JSON.parse(mapString);
      } catch (exception) {
        this.handleDisconnect('mapError');
        return;
      }

      //If we got to here everything is fine, we can set usual server listener
      ws.onmessage = this.handleServerMessage;
      console.log('Successfully connected with id', id);
      this.setState({
        ws,
        lastAdress: adress,
        lastName: name,
        connectionState: WebSocket.OPEN,
        map: mapObj,
        playerId: id
      });
    }

    //This handler will be replaced with proper one after the first response
    ws.onmessage = handleFirstResponse;
    //ws.onmessage = console.log;
    ws.send(request);
  }

  handleDisconnect = (reason, info) => {
    switch (reason) {
      case 'serverOff':
        this.displayError('Server closed');
        break;
      case 'serverError':
        this.displayError('Server error');
        break;
      case 'serverWrongResponse':
        this.displayError('Server sent wrong response: ' + info);
        break;
      case 'connectFail':
        this.displayError('Failed to connect to ' + info);
        break;
      case 'mapError':
        this.displayError('Failed to parse map file');
        break;
      default:
        this.displayError('Something went wrong: ' + reason);
    }

    this.setState({ connectionState: WebSocket.CLOSED });

    if (this.state.ws === null) {
      return;
    }

    if (this.state.ws.readyState === WebSocket.OPEN) {
      this.state.ws.close();
    }
    this.setState({ ws: null });
  }

  handleServerMessage = response => {
    const message = response.data;

    let entities;
    try {
      entities = JSON.parse(message);
    } catch (exception) {
      console.log(exception);
    }

    this.setState({ entities });

    //console.log(message);
  }

  displayError = errorMessage => {
    this.setState({ lastError: errorMessage });
  }

  sendTickInfo = (crosshairPosition, playerControls) => {
    if (!this.state.ws) {
      //This cant happen, but from my experience everything that can't happen
      //will happen
      handleDisconnect('websocketDoesNotExist');
      return;
    }

    const message = (
      'inputs\n' +
      'crosshair\n' +
      crosshairPosition.x + '\n' +
      crosshairPosition.y + '\n' +
      playerControls.join('\n')
    );

    //console.log(message);

    //console.log(this.state.counter + 1);
    this.setState({ counter: this.state.counter + 1 });

    this.state.ws.send(message);
  }

  render() {
    let content;
    if (this.state.connectionState !== WebSocket.OPEN) {
      content = (
        <Connect
          onConnectAttempt={this.handleConnectAttempt}
          onError={this.displayError}
          lastAdress={this.state.lastAdress}
          lastName={this.state.lastName}
          isConnecting={this.state.connectionState === WebSocket.CONNECTING}
        />
      );
    } else {
      content = (
        <Game
          map={this.state.map}
          entities={this.state.entities}
          playerId={this.state.playerId}
          onTick={this.sendTickInfo}
        />
      );
    }

    return (
      <div className="app">
        <div className="errorOutput">
          {this.state.lastError}
        </div>
        {content}
      </div>
    );
  }
}

export default App;
