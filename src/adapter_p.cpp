#include "adapter_p.h"
#include "adapter.h"

using namespace QBluez;

AdapterPrivate::AdapterPrivate(Adapter *parent)
    : QObject(parent)
    , q(parent)
{
}
