#ifndef SRC_SOURCE_H_
#define SRC_SOURCE_H_

#include "nan.h"
#include "uv_link_t.h"

#include "src/base.h"

namespace node {
namespace link {

class LinkSource : public Nan::ObjectWrap, public Base {
 public:
  static void Init(v8::Handle<v8::Object> target);

 protected:
  LinkSource(uv_stream_t* stream);
  ~LinkSource() {}

  static NAN_METHOD(New);

  uv_link_source_t source_;
};

}  // namespace link
}  // namespace node

#endif  // SRC_SOURCE_H_
