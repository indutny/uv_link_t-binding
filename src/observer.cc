#include "src/observer.h"

namespace node {
namespace link {

using namespace v8;

void Observer::Init(Handle<Object> target) {
  Local<FunctionTemplate> t = Nan::New<FunctionTemplate>(New);
  t->SetClassName(Nan::New("Observer").ToLocalChecked());
  t->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(t, "close", Close);
  Nan::SetPrototypeMethod(t, "readStart", ReadStart);
  Nan::SetPrototypeMethod(t, "readStop", ReadStop);
  Nan::SetPrototypeMethod(t, "shutdown", Shutdown);
  Nan::SetPrototypeMethod(t, "writev", Shutdown);

  Nan::SetPrototypeMethod(t, "writeAsciiString", WriteString<ASCII>);
  Nan::SetPrototypeMethod(t, "writeUtf8String", WriteString<UTF8>);
  Nan::SetPrototypeMethod(t, "writeUCS2String", WriteString<UCS2>);
  Nan::SetPrototypeMethod(t, "writeLatin1String", WriteString<BINARY>);

  Nan::SetPrototypeMethod(t, "setStreaming", SetStreaming);

  SetLinkAccessor<Observer>(t);

  Nan::Set(target, Nan::New("Observer").ToLocalChecked(), t->GetFunction());
}


NAN_METHOD(Observer::New) {
  Observer* o = new Observer();
  o->Wrap(info.Holder());
  info.GetReturnValue().Set(info.Holder());
}


NAN_METHOD(Observer::Close) {
  Observer* o = Nan::ObjectWrap::Unwrap<Observer>(info.Holder());

  // TODO(indutny): cache `onclose`
  if (info[0]->IsFunction())
    Nan::Set(info.Holder(), Nan::New("onclose").ToLocalChecked(), info[0]);

  uv_link_close(o->link(), OnClose);
}


NAN_METHOD(Observer::ReadStart) {
  Observer* o = Nan::ObjectWrap::Unwrap<Observer>(info.Holder());
  info.GetReturnValue().Set(uv_link_read_start(o->link()));
}


NAN_METHOD(Observer::ReadStop) {
  Observer* o = Nan::ObjectWrap::Unwrap<Observer>(info.Holder());
  info.GetReturnValue().Set(uv_link_read_stop(o->link()));
}


NAN_METHOD(Observer::Shutdown) {
  Observer* o = Nan::ObjectWrap::Unwrap<Observer>(info.Holder());
  info.GetReturnValue().Set(uv_link_shutdown(o->link(), 0, 0));
}


NAN_METHOD(Observer::Writev) {
  Observer* o = Nan::ObjectWrap::Unwrap<Observer>(info.Holder());
}


template <enum encoding enc>
NAN_METHOD(Observer::WriteString) {
  Observer* o = Nan::ObjectWrap::Unwrap<Observer>(info.Holder());
}


NAN_METHOD(Observer::SetStreaming) {
  Observer* o = Nan::ObjectWrap::Unwrap<Observer>(info.Holder());

  o->streaming_ = info[0]->IsTrue();
}


Observer::Observer() : Base(reinterpret_cast<uv_link_t*>(&observer_)),
                       streaming_(false) {
  int err;

  err = uv_link_observer_init(&observer_);
  assert(err == 0);

  observer_.data = this;

  observer_.observer_read_cb = OnRead;
}


void Observer::OnRead(uv_link_observer_t* observer,
                      ssize_t nread,
                      const uv_buf_t* buf) {
  Nan::HandleScope scope;

  Observer* o = reinterpret_cast<Observer*>(observer->data);

  if (!o->streaming_)
    return;

  Local<Value> buffer;
  if (buf != nullptr && nread >= 0)
    buffer = Nan::CopyBuffer(buf->base, nread).ToLocalChecked();
  else
    buffer = Nan::Undefined();

  Local<Value> argv[] = {
    Nan::New(static_cast<int32_t>(nread)),
    buffer
  };
  Nan::MakeCallback(o->handle(), "onread", 2, argv);
}


void Observer::OnClose(uv_link_t* source) {
  Nan::HandleScope scope;
  Observer* o = reinterpret_cast<Observer*>(source->data);

  Nan::MakeCallback(o->handle(), "onclose", 0, nullptr);
}

}  // namespace link
}  // namespace node
