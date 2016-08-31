#ifndef _NODE_MODSECURITY_H_
#define _NODE_MODSECURITY_H_

#include <nan.h>
#include <string>

using namespace std;

class NodeModsecurity : public Nan::ObjectWrap {
public:
  static NAN_MODULE_INIT(Init);

  static Nan::Persistent<v8::Function> & constructor();

private:
  explicit NodeModsecurity(string rules);

  ~NodeModsecurity();

  static void New(const Nan::FunctionCallbackInfo<v8::Value> &info);
  static void Log(void *data, const char *msg);

  // static void Target(const Nan::FunctionCallbackInfo<v8::Value> &info);
  //
  // static void ByRegex(const Nan::FunctionCallbackInfo<v8::Value> &info);
  //
  // static void ByCss(const Nan::FunctionCallbackInfo<v8::Value> &info);
  //
  // static void ByXPath(const Nan::FunctionCallbackInfo<v8::Value> &info);
  //
  // static void ByJsonPath(const Nan::FunctionCallbackInfo<v8::Value> &info);
  //
  // static void ParseXml(const Nan::FunctionCallbackInfo<v8::Value> &info);
  //

};

#endif
