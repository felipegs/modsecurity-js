#include "node_modsecurity.hpp"
#include <modsecurity/modsecurity.h>
#include <modsecurity/transaction.h>
#include <iostream>
using namespace std;

using modsecurity::ModSecurity;
using modsecurity::Rules;
using modsecurity::Transaction;

NAN_MODULE_INIT(InitAll) {
  Nan::SetMethod(target,"hasThreats", HasThreats);
}

void Log(void *fn, const char *msg) {
  const unsigned argc = 1;
  v8::Local<v8::Value> argv[argc] = { Nan::New(msg).ToLocalChecked() };
  Nan::MakeCallback(Nan::GetCurrentContext()->Global(), *(v8::Local<v8::Function> *)fn, argc, argv);
}

NAN_METHOD(HasThreats) {
  ModSecurity *modsec;
  Rules *rules;
  modsec = new ModSecurity();
  rules = new Rules();
  modsecurity::ModSecurityIntervention intervention;

  Nan::MaybeLocal<v8::Object> config = Nan::To<v8::Object>(info[0]);
  Nan::MaybeLocal<v8::String> rulesConfigV8 = Nan::To<v8::String>(info[1]);
  Nan::Utf8String rulesConfig(rulesConfigV8.ToLocalChecked());

  rules->load(*rulesConfig);
  v8::Local<v8::Function> logCb = info[2].As<v8::Function>();
  Transaction *modsecTransaction = new Transaction(modsec, rules, &logCb);
  modsec->setServerLogCb((LogCb)&Log);

  Nan::MaybeLocal<v8::String> clientIPV8 = Nan::To<v8::String>(Nan::Get(config.ToLocalChecked(), Nan::New("clientIP").ToLocalChecked()).ToLocalChecked());
  Nan::MaybeLocal<v8::Uint32> clientPortv8 = Nan::To<v8::Uint32>(Nan::Get(config.ToLocalChecked(), Nan::New("clientPort").ToLocalChecked()).ToLocalChecked());
  Nan::MaybeLocal<v8::String> serverIPV8 =  Nan::To<v8::String>(Nan::Get(config.ToLocalChecked(), Nan::New("serverIP").ToLocalChecked()).ToLocalChecked());
  Nan::MaybeLocal<v8::Uint32> serverPortV8 = Nan::To<v8::Uint32>(Nan::Get(config.ToLocalChecked(), Nan::New("serverPort").ToLocalChecked()).ToLocalChecked());

  Nan::Utf8String clientIP(clientIPV8.ToLocalChecked());
  Nan::Utf8String serverIP(serverIPV8.ToLocalChecked());

  modsecTransaction->processConnection(*clientIP, serverPortV8.ToLocalChecked()->Uint32Value(), *serverIP, clientPortv8.ToLocalChecked()->Uint32Value());
  if(modsecTransaction->intervention(&intervention)){
    return info.GetReturnValue().Set(true);
  }

  Nan::MaybeLocal<v8::String> uriV8 = Nan::To<v8::String>(Nan::Get(config.ToLocalChecked(), Nan::New("uri").ToLocalChecked()).ToLocalChecked());
  Nan::MaybeLocal<v8::String> httpVersionV8 = Nan::To<v8::String>(Nan::Get(config.ToLocalChecked(), Nan::New("httpVersion").ToLocalChecked()).ToLocalChecked());
  Nan::MaybeLocal<v8::String> httpMethodV8 = Nan::To<v8::String>(Nan::Get(config.ToLocalChecked(), Nan::New("httpMethod").ToLocalChecked()).ToLocalChecked());

  Nan::Utf8String uri(uriV8.ToLocalChecked());
  Nan::Utf8String httpVersion(httpVersionV8.ToLocalChecked());
  Nan::Utf8String httpMethod(httpMethodV8.ToLocalChecked());

  modsecTransaction->processURI(*uri, *httpMethod, *httpVersion);
  if(modsecTransaction->intervention(&intervention)){
    return info.GetReturnValue().Set(true);
  }

  Nan::MaybeLocal<v8::Object> headers = Nan::To<v8::Object>(Nan::Get(config.ToLocalChecked(), Nan::New("headers").ToLocalChecked()).ToLocalChecked());
  v8::Local<v8::Array> headersNames = headers.ToLocalChecked()->GetOwnPropertyNames();

  for (unsigned int i = 0; i < headersNames->Length(); ++i) {
    v8::Local<v8::Value> key = headersNames->Get(i);
    v8::Local<v8::Value> value = headers.ToLocalChecked()->Get(key);

    if (key->IsString() && value->IsString()) {
      Nan::Utf8String utf8_key(key);
      Nan::Utf8String utf8_value(value);
      modsecTransaction->addRequestHeader(*utf8_key, *utf8_value);
    }
  }

  modsecTransaction->processRequestHeaders();

  if(modsecTransaction->intervention(&intervention)){
    return info.GetReturnValue().Set(true);
  }

  Nan::MaybeLocal<v8::String> payloadV8 = Nan::To<v8::String>(Nan::Get(config.ToLocalChecked(), Nan::New("payload").ToLocalChecked()).ToLocalChecked());
  Nan::Utf8String payload(payloadV8.ToLocalChecked());

  modsecTransaction->appendRequestBody((const unsigned char *)*payload, payload.length());
  modsecTransaction->processRequestBody();

  if(modsecTransaction->intervention(&intervention)){
    return info.GetReturnValue().Set(true);
  }

  return info.GetReturnValue().Set(false);
}

NODE_MODULE(node_modsecurity, InitAll)
