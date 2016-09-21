# JoyStream

JoyStream is platform for decentralized distribution, discovery and monetization of digital content.
This repository contains all applications which make the platform possible, and all internal
libraries which these applications use.

NB!: This repository will be broken up at some point in the future.

## Libraries

All libraries live in the joystream namespace, ordered in terms of dependencies, so each lib only (possibly) depends on libs above. Look at tiven application folder for more information. The listed maintainer is the developer who will most likely be able to help you promptly on the given library.

 Library | Description | Maintainer
---------|-------------|-----------
common | An extension to the mSIGNA coincore library, primarily with type safe basic Bitcoin classes, but also a few other utility classes | @bedeho, @mnaamani
protocol_wire | Messages which are used in the JoyStream BitTorrent extension wire protocol, along with serialization and deserialization routines. | @bedeho
paymentchannel | A (CLTV) payment channel library, using channels as needed in the JoyStream BitTorretn extension. | @bedeho, @mnaamani
protocol_statemachine | Boost statechart state machine for a single connection using the JoyStream BitTorrent extension protocol.| @bedeho
protocol_session | A session manager for multiple connections using the JoyStream extension wire protocol. | @bedeho
extension | A libtorrent extension (http://libtorrent.org/reference-Plugins.html) used by a JoyStream node, implementing - among other things, a session. | @bedeho
core | The core JoyStream library for a node. | @bedeho
bitcoin | A SPV wallet and data store, using mSIGNA coinq as the SPV client. | @mnaamani
app_kit | Minimal library for running a (headless) JoyStream node | @mnaamani
classic::gui | A widget based Qt GUI library for the classic JoyStream desktop client. | @bedeho
classic::controller | The controller for the classic desktop client. | @bedeho
daemon-core | The core library for the gRPC based daemon node. | @mnaamani, @rllola
blockcypher | An incomplete Blockcypher API library | @bedeho
mixpanel | An incomplete Mixpanel API library | @bedeho

## Applications

All applications live in the joystream namespace, look at tiven application folder for more information. The listed maintainer is the developer who will most likely be able to help you promptly on the given library.

Application | Description | Maintainer
------------|-------------|-----------
classic::app | Desktop GUI application for the classic JoyStream client | @bedeho
joystreamd | Daemon application for the JoyStream daemon providing a HTTP based RPC API around a JoyStream client | @mnaamani, @rllola
joystream-cli | Command line application for talking with joystreamd | -
joystream | Desktop GUI application for the JoyStream client | @rllola
joystream-android | Android application for the JoyStream client (future) | -
demo:: * | Different demo applications for different libraries | -

## Building
Instructions for building from source can be found in the  [docs/](docs/) folder.

## Licensing
To be provided later.

## Contributing
Please see the [HACKING.md](HACKING.md) if you want to contribute to the project.

Copyright (C) JoyStream Inc. - All Rights Reserved
