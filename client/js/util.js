class Util {
  convertNumbers = value => {
    if ( !isNaN(value) ) {
      return parseFloat(value);
    }
    return value;
  }

  convertBooleans = value => {
    if (value === 'true') {
      return true;
    } else if (value === 'false') {
      return false;
    }

    return value;
  }

  jsonReviewer = (key, value) => {
    if (value instanceof Array) {
      //This is not too beautiful but...
      //isNaN([]) = false
      //isNaN([1]) = false
      //isNaN([1, 2]) = true
      //And there is no better fix that I can think of
      return value;
    }

    value = this.convertNumbers(value);
    value = this.convertBooleans(value);
    return value;
  }
}

const util = new Util();
export default util;