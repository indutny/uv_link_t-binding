#include "nan.h"
#include "uv.h"
#include "uv_link_t.h"

#include "src/observer.h"
#include "src/source.h"

namespace node {
namespace link {

using namespace v8;

enum AuxType {
  kChainType,
  kUnchainType
};

template <AuxType type>
static NAN_METHOD(AuxMethod) {
  if (info.Length() != 2 || !info[0]->IsExternal() || !info[1]->IsExternal())
    return Nan::ThrowError("Link::Chain expects External as both arguments");

  uv_link_t* from =
      reinterpret_cast<uv_link_t*>(info[0].As<External>()->Value());
  uv_link_t* to =
      reinterpret_cast<uv_link_t*>(info[1].As<External>()->Value());

  int err;
  if (type == kChainType)
    err = uv_link_chain(from, to);
  else
    err = uv_link_unchain(from, to);

  info.GetReturnValue().Set(err);
}

static NAN_MODULE_INIT(Init) {
  Observer::Init(target);
  LinkSource::Init(target);

  Nan::SetMethod(target, "chain", AuxMethod<kChainType>);
  Nan::SetMethod(target, "unchain", AuxMethod<kUnchainType>);
}

}  // namespace link
}  // namespace node

NODE_MODULE(binding, node::link::Init);
