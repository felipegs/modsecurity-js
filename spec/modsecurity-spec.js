'use strict'
const modsecurity = require('../index');

describe('Modsecurity', () => {

  describe('Modsecurity', () => {

    beforeEach(() => {
      const rules = {"clientIP": "127.0.0.1", "serverPort": 40, "headers" : {"Content-Type": "application/json"}, "payload": "{ \"ola\": 1 }" }
      console.log(modsecurity)
      console.log(modsecurity.hasThreats(rules))
      //console.log(modsecurity.analyzeThreats(rules))

    })

    it("first test", function() {

    })

  })

})
