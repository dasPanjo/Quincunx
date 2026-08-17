
#include "Component.h"
#include "GameObject.h"

namespace Penjin {
    Transform& Component::transform() const { return gameObject_->transform(); }
}
