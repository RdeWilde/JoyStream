#include <joystreamd_lib/RPCSuscribeEvents.hpp>


RPCSuscribeEvents::RPCSuscribeEvents(joystream::daemon::rpc::Daemon::AsyncService* service, grpc::ServerCompletionQueue* cq, joystream::core::Node* node)
    : RPCRequestStreamServer(service, cq), node_(node)
{
    ctx_.AsyncNotifyWhenDone(this);
    service_->RequestSuscribeEvents(&ctx_, &request_, &responder_, cq_, cq_, this);

}


void RPCSuscribeEvents::publishRemovedTorrent(const libtorrent::sha1_hash & info_hash)
{
    joystream::daemon::rpc::Event response;

    std::cout << "Torrent Removed signal !" << std::endl;

    response.set_name("Removed Torrent");

    this->write(response);
}

void RPCSuscribeEvents::publishAddedTorrent(joystream::core::Torrent * torrent)
{
    joystream::daemon::rpc::Event response;

    std::cout << "Torrent Added signal !" << std::endl;

    response.set_name("Added Torrent");

    this->write(response);
}

void RPCSuscribeEvents::process()
{
    // Pop up a new instance for concurency
    new RPCSuscribeEvents(service_, cq_, node_);

    QObject::connect(node_, &joystream::core::Node::removedTorrent,
                    this, &RPCSuscribeEvents::publishRemovedTorrent);

    QObject::connect(node_, &joystream::core::Node::addedTorrent,
                    this, &RPCSuscribeEvents::publishAddedTorrent);
}
