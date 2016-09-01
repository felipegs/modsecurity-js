'use strict'
const modsecurity = require('../index');
const fs = require('fs');
const path = require('path');


describe('Modsecurity', () => {

  describe('Modsecurity', () => {
    it("should block request invalid json", function() {
      const data = {"clientIP": "127.0.0.1", "serverPort": 40, "headers" : {"Content-Type": "application/json"}, "payload": "{ \"ola\": }"  }
      const rules = fs.readFileSync(path.join(__dirname,"rules.conf"), "utf8");
      expect(modsecurity.hasThreats(data, rules, console.log)).toBe(true)
    })
  })
})
