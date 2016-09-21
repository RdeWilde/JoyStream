# JoyStream

JoyStream is platform for decentralized distribution, discovery and monetization of digital content.
This repository contains all applications which make the platform possible, and all internal
libraries which these applications use.

NB!: This repository will be broken up at some point in the future.

## Libraries

All libraries live in the joystream namespace, ordered in terms of dependencies, so each lib
only (possibly) depends on libs above.

 Library | Description 
---------|-------------
common | 
protocol_wire | Messages which are used in the JoyStream BitTorrent extension wire protocol, along with serialization and deseriazliation routines.
paymentchannel | 
protocol_statemachine | Boost statechart state machine for a single connection using the JoyStream BitTorrent extension protocol.
protocol_session | 
extension |
core |
bitcoin |
classic::gui |
classic::controller |
daemon-core |
blockcypher |
mixpanel |

## Applications

Application | Description
------------|------------
classic::application | Desktop GUI application for the classic JoyStream client
joystreamd | Daemon application for the JoyStream daemon providing a HTTP based RPC API around a JoyStream client
joystream-cli | Command line application for talking with joystreamd
joystream | Desktop GUI application for the JoyStream client
joystream-android | Android application for the JoyStream client (future)

## Building
Instructions for building from source can be found in the  [docs/](docs/) folder.

## Licensing
To be provided later.

## Contributing
Please see the [HACKING.md](HACKING.md) if you want to contribute to the project.

---
Copyright (C) JoyStream Inc. - All Rights Reserved
