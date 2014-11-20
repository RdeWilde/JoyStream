#include <iostream>

int main()
{
    std::cout << "Hello, world!\n";

    // Listening port: 32225


    // how to add torrent?, how to spit out information?

    // Start each bitswapr in its own QThread? and then use invoke later to qmetaobject to
    // issue commands, like addTorrent, etc? But how about reading communicaton out?

    // think more thourghly through how to test this, how to look into state of both sides of a connection,
    //m etc.

    /*
     * // Create Qt application: all objects created after this point are owned by this thread
    QApplication a(argc, argv);


    // Load default state
    ControllerState controllerState;

    // Create client
    BitSwapr client(controllerState, showView);

    // Start client
    client.start();
    std::cout << "Started client thread." << std::endl;

    // Start application event loop,
    // it services events of thread which runs client.
    a.exec();

    // Notify that event loop processing was ended
    std::cout << "Application event loop exited, application closing." << std::endl;
    */
}
