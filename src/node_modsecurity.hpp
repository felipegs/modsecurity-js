#ifndef NODE_MODSECURITY_HPP_
#define NODE_MODSECURITY_HPP_

#include <nan.h>

NAN_METHOD(HasThreats);
void Log(void *data, const char *msg);

#endif
