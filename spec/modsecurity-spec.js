'use strict'
const modsecurity = require('../index');
const fs = require('fs');
const path = require('path');

describe('Modsecurity', () => {

  beforeEach(function () {
    this.data = {"clientIP": "127.0.0.1", "clientPort": 1234, "serverIP": "198.0.0.1", "serverPort": 40, "headers" : {"Content-Type": "application/json"}, "payload": "{ \"ola\": }" }
    this.rules = fs.readFileSync(path.join(__dirname, "rules.conf"), "utf8")
  })

  it("should block request with invalid json", function(done) {
    modsecurity.hasThreats(this.data, this.rules, function(err, result) {
      expect(result.hasThreats).toBe(true)
      done()
    })
  })

  it("should not abort if empty data and rules", function() {
    expect(function () { modsecurity.hasThreats(null, null, null) }).toThrow(new TypeError("Wrong arguments"));
  })

  it("should not abort if empty rules", function() {
    const cb = function () {}
    expect((function () { modsecurity.hasThreats(this.data, null, cb) }).bind(this)).toThrow(new TypeError("Wrong arguments"))
  })

  it("should not abort if empty data", function() {
    const cb = function () {}
    expect((function () { modsecurity.hasThreats(null, this.rules, cb) }).bind(this)).toThrow(new TypeError("Wrong arguments"))
  })

  it("should not abort if invalid rules", function(done) {
    const cb = function (err, result) {
      expect(result.hasThreats).toBe(false)
      done()
    }
    modsecurity.hasThreats(this.data, "invalid rule", cb)
  })

  it("should not abort if invalid data", function() {
    const cb = function (err, result) { }
    expect((function () { modsecurity.hasThreats({"ola": 1}, this.rules, cb) }).bind(this)).toThrow(new TypeError("Wrong arguments"))
  })
})
