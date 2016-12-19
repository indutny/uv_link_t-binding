#include "src/source.h"

namespace node {
namespace link {

using namespace v8;

void LinkSource::Init(Handle<Object> target) {
  Local<FunctionTemplate> t = Nan::New<FunctionTemplate>(New);
  t->SetClassName(Nan::New("Source").ToLocalChecked());
  t->InstanceTemplate()->SetInternalFieldCount(1);

  SetLinkAccessor<LinkSource>(t);

  Nan::Set(target, Nan::New("Source").ToLocalChecked(), t->GetFunction());
}


NAN_METHOD(LinkSource::New) {
  if (info.Length() != 1 || !info[0]->IsExternal())
    return Nan::ThrowError("Source::New expects External as an argument");

  LinkSource* source = new LinkSource(
      reinterpret_cast<uv_stream_t*>(info[0].As<External>()->Value()));

  source->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}


LinkSource::LinkSource(uv_stream_t* stream)
    : Base(reinterpret_cast<uv_link_t*>(&source_)) {
  int err;

  err = uv_link_source_init(&source_, reinterpret_cast<uv_stream_t*>(stream));
  assert(err == 0);

  source_.data = this;
}

}  // namespace link
}  // namespace node
