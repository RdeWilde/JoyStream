#include "End.hpp"
#include "MessageType.hpp"

End::End()
{
}

MessageType End::messageType() const {
    return MessageType::end;
}

quint32 End::length() const {
    return 0;
}

void End::write(QDataStream & stream) const {
}
