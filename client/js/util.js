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
    value = this.convertNumbers(value);
    value = this.convertBooleans(value);
    return value;
  }
}

const util = new Util();
export default util;