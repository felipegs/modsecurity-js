'use strict'
const modsecurity = require('../index');

describe('Modsecurity', () => {

  describe('Modsecurity', () => {
    it("first test", function() {
      const data = {"clientIP": "127.0.0.1", "serverPort": 40, "headers" : {"Content-Type": "application/json"}, "payload": "{ \"ola\": }"  }
      const rules = {}
      expect(modsecurity.hasThreats(data, rules, console.log)).toBe(true)
    })
  })
})
