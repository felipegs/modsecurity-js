#include "node_modsecurity.hpp"
#include <modsecurity/modsecurity.h>
#include <modsecurity/transaction.h>
#include <iostream>
using namespace std;

using modsecurity::ModSecurity;
using modsecurity::Rules;
using modsecurity::Transaction;

NAN_MODULE_INIT(NodeModsecurity::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Modsecurity").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1); //precisa disso?

  // Nan::SetPrototypeMethod(tpl, "target", Target);
  // Nan::SetPrototypeMethod(tpl, "byRegex", ByRegex);
  // Nan::SetPrototypeMethod(tpl, "byXPath", ByXPath);
  // Nan::SetPrototypeMethod(tpl, "byCss", ByCss);
  // Nan::SetPrototypeMethod(tpl, "byJsonPath", ByJsonPath);
  // Nan::SetPrototypeMethod(tpl, "parseXml", ParseXml);

  constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("Modsecurity").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

NodeModsecurity::NodeModsecurity(string r)  {

}


NodeModsecurity::~NodeModsecurity() {
}

void NodeModsecurity::Log(void *data, const char *msg) {
  string m(msg);

  cout << "LOOOOOOOOOG=" << m << endl;
}

void NodeModsecurity::New(const Nan::FunctionCallbackInfo<v8::Value> &info) {
  cout << "hahahahahahahahah" << endl;
  ModSecurity *modsec;
  Rules *rules;
  modsec = new ModSecurity();
  rules = new Rules();
  modsecurity::ModSecurityIntervention bla;

  //rules->load(r.c_str());
  rules->loadFromUri("/root/rules.conf");
  Transaction *modsecTransaction = new Transaction(modsec, rules, NULL);
  modsec->setServerLogCb((LogCb)&NodeModsecurity::Log);
  modsecTransaction->processConnection("127.0.1", 12345, "www.google.com", 80);
  if(modsecTransaction->intervention(&bla)){
      cout << "ocorreu intervencao" << endl;
  }

  modsecTransaction->processURI("http://www.google.com/?abc=../../ ", "GET", "1.2");
  if(modsecTransaction->intervention(&bla)){
      cout << "ocorreu intervencao" << endl;
  }

  string str = "{\"ola\":}";
  modsecTransaction->addRequestHeader("content-type", "application/json");
  int requestHeaders = modsecTransaction->processRequestHeaders();
  cout << "Headers=" << requestHeaders << endl;

  if(modsecTransaction->intervention(&bla)){
      cout << "ocorreu intervencao" << endl;
  }

  modsecTransaction->appendRequestBody((const unsigned char *)str.c_str(), 8);
  int payload = modsecTransaction->processRequestBody();
  cout << "Payload=" << payload << endl;

  if(modsecTransaction->intervention(&bla)){
      cout << "ocorreu intervencao" << endl;
  }
  cout << "bla=" << bla.status << endl;
  cout << "disruptive=" << bla.disruptive << endl;
  cout << "log=" << bla.log << endl;
  cout << "pause=" << bla.pause << endl;

  NodeModsecurity *obj = new NodeModsecurity("ola");
  obj->Wrap(info.This());
  return info.GetReturnValue().Set(info.This());

}

Nan::Persistent<v8::Function> & NodeModsecurity::constructor() {
  static Nan::Persistent<v8::Function> constructor;
  return constructor;
}

NODE_MODULE(node_modsecurity, NodeModsecurity::Init)
