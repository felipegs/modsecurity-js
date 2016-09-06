#ifndef _MODSECURITY_WORKER_H_
#define _MODSECURITY_WORKER_H_

#include <nan.h>
#include <string>
#include <map>

using namespace std;
using namespace v8;

class ModsecurityWorker : public Nan::AsyncWorker {
public:
  ModsecurityWorker(Nan::Callback *callback, const map<string, string> &params, const map<string, string> &headers);

  void HandleOKCallback();

  void HandleErrorCallback();

  void Execute();

  static void Log(void *data, const char *msg);

private:

  map<string, string> params;

  map<string, string> headers;

  bool hasThreats;

  string logMessage;

};



#endif
