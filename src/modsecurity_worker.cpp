#include <modsecurity/modsecurity.h>
#include <modsecurity/transaction.h>
#include "modsecurity_worker.hpp"
#include <iostream>

using modsecurity::ModSecurity;
using modsecurity::Rules;
using modsecurity::Transaction;
using namespace std;

ModsecurityWorker::ModsecurityWorker(Nan::Callback *callback, const map<string, string> &params, const map<string, string> &headers):
 Nan::AsyncWorker(callback), params(params), headers(headers), hasThreats(false), logMessage("")  {
}

void ModsecurityWorker::Log(void *fn, const char *msg) {
  ((ModsecurityWorker *)fn)->logMessage.append(msg);
}

void ModsecurityWorker::Execute() {
  try {
    ModSecurity *modsec;
    Rules *rules;
    modsec = new ModSecurity();
    rules = new Rules();
    modsecurity::ModSecurityIntervention intervention;

    rules->load(params["rules"].c_str());
    Transaction *modsecTransaction = new Transaction(modsec, rules, this);
    modsec->setServerLogCb((LogCb)&ModsecurityWorker::Log);

    modsecTransaction->processConnection(params["clientIP"].c_str(), std::stoi(params["clientPort"]), params["serverIP"].c_str(), std::stoi(params["serverPort"]));
    if(modsecTransaction->intervention(&intervention)) {
      hasThreats = true;
      return;
    }

    modsecTransaction->processURI(params["uri"].c_str(), params["httpMethod"].c_str(), params["httpVersion"].c_str());
    if(modsecTransaction->intervention(&intervention)) {
      hasThreats = true;
      return;
    }

    for(pair<string, string> h : headers) {
      modsecTransaction->addRequestHeader(h.first, h.second);
    }

    modsecTransaction->processRequestHeaders();

    if(modsecTransaction->intervention(&intervention)){
      hasThreats = true;
      return;
    }

    modsecTransaction->appendRequestBody(reinterpret_cast<const unsigned char*>(params["payload"].c_str()), params["payload"].length());
    modsecTransaction->processRequestBody();

    if(modsecTransaction->intervention(&intervention)) {
      hasThreats = true;
      return;
    }

  } catch (exception& e) {
    SetErrorMessage(e.what());
  } catch(...) {
    SetErrorMessage("Unknown exception");
  }
}

void ModsecurityWorker::HandleOKCallback() {
  Nan::HandleScope scope;

  v8::Local<v8::Object> obj = Nan::New<v8::Object>();
  obj->Set(Nan::New("log").ToLocalChecked(), Nan::New(logMessage.c_str()).ToLocalChecked());
  obj->Set(Nan::New("hasThreats").ToLocalChecked(), Nan::New(hasThreats));

  Local<Value> argv[] = {
    Nan::Null(),obj
  };

  callback->Call(2, argv);
}

void ModsecurityWorker::HandleErrorCallback() {
  Nan::HandleScope scope;
  Local <Value> argv[] = {
    Nan::Error(ErrorMessage()), Nan::Null()
  };
  callback->Call(2, argv);
}
