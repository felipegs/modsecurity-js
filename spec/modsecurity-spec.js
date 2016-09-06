'use strict'
const modsecurity = require('../index');
const fs = require('fs');
const path = require('path');


describe('Modsecurity', () => {

  describe('Modsecurity', () => {
    it("should block request with invalid json", function(done) {
      const data = {"clientIP": "127.0.0.1", "clientPort": 1234, "serverIP": "198.0.0.1", "serverPort": 40, "headers" : {"Content-Type": "application/json"}, "payload": "{ \"ola\": }" }
      const rules = fs.readFileSync(path.join(__dirname, "rules.conf"), "utf8");
      modsecurity.hasThreats(data, rules, function(err, result) {
        console.log(err)
        console.log(result)
        expect(result.hasThreats).toBe(true)
        done()
      })
    })
  })
})
