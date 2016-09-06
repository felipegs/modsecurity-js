#include <map>
#include <string>
#include "node_modsecurity.hpp"
#include "modsecurity_worker.hpp"

using namespace std;

NAN_MODULE_INIT(InitAll) {
  Nan::SetMethod(target,"hasThreats", HasThreats);
}

NAN_METHOD(HasThreats) {
  //TODO tratar campos obrigatorios

  Nan::MaybeLocal<v8::Object> config = Nan::To<v8::Object>(info[0]);
  Nan::MaybeLocal<v8::String> rulesConfigV8 = Nan::To<v8::String>(info[1]);
  Nan::Utf8String rulesConfig(rulesConfigV8.ToLocalChecked());

  Nan::MaybeLocal<v8::String> clientIPV8 = Nan::To<v8::String>(Nan::Get(config.ToLocalChecked(), Nan::New("clientIP").ToLocalChecked()).ToLocalChecked());
  Nan::MaybeLocal<v8::String> clientPortV8 = Nan::To<v8::String>(Nan::Get(config.ToLocalChecked(), Nan::New("clientPort").ToLocalChecked()).ToLocalChecked());
  Nan::MaybeLocal<v8::String> serverIPV8 =  Nan::To<v8::String>(Nan::Get(config.ToLocalChecked(), Nan::New("serverIP").ToLocalChecked()).ToLocalChecked());
  Nan::MaybeLocal<v8::String> serverPortV8 = Nan::To<v8::String>(Nan::Get(config.ToLocalChecked(), Nan::New("serverPort").ToLocalChecked()).ToLocalChecked());

  Nan::Utf8String clientIP(clientIPV8.ToLocalChecked());
  Nan::Utf8String clientPort(clientPortV8.ToLocalChecked());
  Nan::Utf8String serverIP(serverIPV8.ToLocalChecked());
  Nan::Utf8String serverPort(serverPortV8.ToLocalChecked());

  Nan::MaybeLocal<v8::String> uriV8 = Nan::To<v8::String>(Nan::Get(config.ToLocalChecked(), Nan::New("uri").ToLocalChecked()).ToLocalChecked());
  Nan::MaybeLocal<v8::String> httpVersionV8 = Nan::To<v8::String>(Nan::Get(config.ToLocalChecked(), Nan::New("httpVersion").ToLocalChecked()).ToLocalChecked());
  Nan::MaybeLocal<v8::String> httpMethodV8 = Nan::To<v8::String>(Nan::Get(config.ToLocalChecked(), Nan::New("httpMethod").ToLocalChecked()).ToLocalChecked());

  Nan::Utf8String uri(uriV8.ToLocalChecked());
  Nan::Utf8String httpVersion(httpVersionV8.ToLocalChecked());
  Nan::Utf8String httpMethod(httpMethodV8.ToLocalChecked());

  Nan::MaybeLocal<v8::String> payloadV8 = Nan::To<v8::String>(Nan::Get(config.ToLocalChecked(), Nan::New("payload").ToLocalChecked()).ToLocalChecked());
  Nan::Utf8String payload(payloadV8.ToLocalChecked());

  map<string, string> params;
  params["rules"] = *rulesConfig;
  params["clientIP"] = *clientIP;
  params["clientPort"] = *clientPort;
  params["serverIP"] = *serverIP;
  params["serverPort"] = *serverPort;
  params["uri"] = *uri;
  params["httpVersion"] = *httpVersion;
  params["httpMethod"] = *httpMethod;
  params["payload"] = *payload;

  map<string, string> headers;

  Nan::MaybeLocal<v8::Object> headersV8 = Nan::To<v8::Object>(Nan::Get(config.ToLocalChecked(), Nan::New("headers").ToLocalChecked()).ToLocalChecked());
  v8::Local<v8::Array> headersNames = headersV8.ToLocalChecked()->GetOwnPropertyNames();

  for (unsigned int i = 0; i < headersNames->Length(); ++i) {
    v8::Local<v8::Value> key = headersNames->Get(i);
    v8::Local<v8::Value> value = headersV8.ToLocalChecked()->Get(key);

    if (key->IsString() && value->IsString()) {
      Nan::Utf8String utf8_key(key);
      Nan::Utf8String utf8_value(value);
      headers[*utf8_key] = *utf8_value;
    }
  }

  Nan::Callback *callback = new Nan::Callback(info[2].As<Function>());
  Nan::AsyncQueueWorker(new ModsecurityWorker(callback, params, headers));
}

NODE_MODULE(node_modsecurity, InitAll)
