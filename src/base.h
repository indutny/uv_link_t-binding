#ifndef SRC_BASE_H_
#define SRC_BASE_H_

#include "nan.h"
#include "uv_link_t.h"

namespace node {
namespace link {

class Base {
 public:
  Base(uv_link_t* link) : link_(link) {}

  inline uv_link_t* link() { return link_; }

 protected:
  template <class T>
  static void SetLinkAccessor(v8::Handle<v8::FunctionTemplate> t) {
    Nan::SetAccessor(t->InstanceTemplate(), Nan::New("link").ToLocalChecked(),
                     GetLink<T>);

  }

  template <class T>
  static NAN_GETTER(GetLink) {
    T* source = Nan::ObjectWrap::Unwrap<T>(info.Holder());
    info.GetReturnValue().Set(Nan::New<v8::External>(source->link_));
  }

  uv_link_t* link_;
};

}  // namespace link
}  // namespace node

#endif  // SRC_BASE_H_
