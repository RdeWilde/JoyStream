
#ifndef CONSOLE_VIEW_HPP
#define CONSOLE_VIEW_HPP

#include "controller/include/Controller.hpp"
#include "controller/include/ViewRequestCallbackHandler.hpp"

#include <boost/thread.hpp>

class ConsoleView : public ViewRequestCallbackHandler {

private:

	// Pointer to controller
	Controller * controller_;
	
	// Thread running runEvalLoop method, is started by startView()
	boost::thread runEvalLoopThread;

	// Process any pending callbacks
	void processCallbacks();

	// Queue of callback functions, they are lambdas wrapped in function pointer wrapper std::function
	// nice tutorial: http://oopscenities.net/2012/02/24/c11-stdfunction-and-stdbind/
	std::vector<std::function<void()> > pendingCallbackFunctions;

	// Synchronizes access to pendingCallbackFunctions
	boost::mutex pendingCallbackFunctionsMutex;

public:

	// Starts ui thread
	void startView(Controller * const controller);

	// Run eval loop used to run gui and take input
	void runEvalLoop();

	// Implementing virtual callback functions in ViewRequestCallback
	void addTorrentCallback(const libtorrent::add_torrent_alert * addTorrentAlert);
};

#endif
