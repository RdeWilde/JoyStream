
#include "controller/include/ConsoleView.hpp"

void ConsoleView::startView(Controller * controller) {

	// Save reference to controller
	controller_ = controller;

	// Start thread which runs an eval loop on this object:  http://antonym.org/2009/05/threading-with-boost---part-i-creating-threads.html
	runEvalLoopThread = boost::thread(&ConsoleView::runEvalLoop, this);
}

void ConsoleView::runEvalLoop() {

	while(1) {

		// Get user input, block for x ms
		// http://cc.byexamples.com/2007/04/08/non-blocking-user-input-in-loop-without-ncurses/

		// do we have a full command? if so, parse and process it

		// process callbacks
		processCallbacks();
	}
}

void ConsoleView::processCallbacks() {

	// Lock access to queue
	pendingCallbackFunctionsMutex.lock();
	
	int numberOfCallbacks = pendingCallbackFunctions.size(), 
		counter = 0;

	// Iterate all pending callbacks and do them
	while(counter < numberOfCallbacks) {

		// Get last
		std::function<void()> & callbackFunction = pendingCallbackFunctions.back();

		// Remove from queue
		pendingCallbackFunctions.pop_back();

		// Make call
		callbackFunction();

		// Count
		counter++;
	}

	// Unlock access to queue
	pendingCallbackFunctionsMutex.unlock();
}

void ConsoleView::addTorrentCallback(const libtorrent::add_torrent_alert * addTorrentAlert) {

	// Define callback lambda
	std::function<void()> callbackFunction = [addTorrentAlert]() { std::cout << "add_torrent_alert:" << addTorrentAlert->what() << std::endl; };

	// Synchronize adding lambda to queue of callback functions
	pendingCallbackFunctionsMutex.lock();
	pendingCallbackFunctions.push_back(callbackFunction);
	pendingCallbackFunctionsMutex.unlock();
}
