'use strict'
const Modsecurity = require('../index').Modsecurity;

describe('Modsecurity', () => {

  describe('Modsecurity', () => {

    beforeEach(() => {
      const rules = ""
      this.modsecurity = new Modsecurity(rules)
    })

    it("first test", function() {
      console.log("passei aqui")
      // this.modesecurity.processConnection(clientIP, clientPort, serverIP, serverPort)
      // this.modesecurity.processConnection(clientIP, clientPort, serverIP, serverPort)
      // this.modesecurity.addRequestHeader("Content-Type", "application/json")
      // this.modesecurity.setRequestBody("{'body': 1}")
    })

  })

})
