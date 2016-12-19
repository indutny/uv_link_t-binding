#ifndef SRC_OBSERVER_H_
#define SRC_OBSERVER_H_

#include "nan.h"
#include "uv_link_t.h"

#include "src/base.h"

namespace node {
namespace link {

class Observer : public Nan::ObjectWrap, public Base {
 public:
  static void Init(v8::Handle<v8::Object> target);

 protected:
  Observer();
  ~Observer() {}

  static NAN_METHOD(New);
  static NAN_METHOD(Close);
  static NAN_METHOD(ReadStart);
  static NAN_METHOD(ReadStop);
  static NAN_METHOD(Shutdown);
  static NAN_METHOD(Writev);
  template <enum encoding enc>
  static NAN_METHOD(WriteString);
  static NAN_METHOD(SetStreaming);

  static void OnRead(uv_link_observer_t* observer,
                     ssize_t nread,
                     const uv_buf_t* buf);
  static void OnClose(uv_link_t* source);

  uv_link_observer_t observer_;
  bool streaming_;
};

}  // namespace link
}  // namespace node

#endif  // SRC_OBSERVER_H_
