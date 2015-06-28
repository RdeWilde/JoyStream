    /**
     * @brief Configuration of seller peer plugin.
     
    class Configuration : public PeerPlugin::Configuration {

    public:

        // Constructor
        Configuration();

        // Constructor from members
        Configuration(const ExtendedMessageIdMapping & clientMapping,
                      const ExtendedMessageIdMapping & peerMapping,
                      BEPSupportStatus peerBEP10SupportStatus,
                      BEPSupportStatus peerBitSwaprBEPSupportStatus,
                      const PeerState & peerState,
                      ClientState clientState);

        // Getters and setters
        PeerState peerState() const;
        void setPeerState(const PeerState & peerState);

        ClientState clientState() const;
        void setClientState(ClientState clientState);

    private:

        // State of peer
        PeerState _peerState;

        // State of client
        ClientState _clientState;

        //Payee::Configuration _payee;
    };
    */	


/**
 * BuyerPeerPlugin::Configuration


BuyerPeerPlugin::Configuration::Configuration() {

}

BuyerPeerPlugin::Configuration::Configuration(const ExtendedMessageIdMapping & clientMapping,
                                              const ExtendedMessageIdMapping & peerMapping,
                                              BEPSupportStatus peerBEP10SupportStatus,
                                              BEPSupportStatus peerBitSwaprBEPSupportStatus,
                                              const PeerState & peerState,
                                              ClientState clientState,
                                              quint32 payorSlot)
    : PeerPlugin::Configuration(clientMapping, peerMapping, peerBEP10SupportStatus, peerBitSwaprBEPSupportStatus)
    , _peerState(peerState)
    , _clientState(clientState)
    , _payorSlot(payorSlot) {
}

BuyerPeerPlugin::PeerState BuyerPeerPlugin::Configuration::peerState() const {
    return _peerState;
}

void BuyerPeerPlugin::Configuration::setPeerState(const PeerState & peerState) {
    _peerState = peerState;
}

BuyerPeerPlugin::ClientState BuyerPeerPlugin::Configuration::clientState() const {
    return _clientState;
}

void BuyerPeerPlugin::Configuration::setClientState(ClientState clientState) {
    _clientState = clientState;
}
*/

    /**
 * PeerPlugin::Configuration
 */

PeerPlugin::Configuration::Configuration() {
}

PeerPlugin::Configuration::Configuration(const ExtendedMessageIdMapping & clientMapping,
                                         const ExtendedMessageIdMapping & peerMapping,
                                         BEPSupportStatus peerBEP10SupportStatus,
                                         BEPSupportStatus peerBitSwaprBEPSupportStatus)
    : _clientMapping(clientMapping)
    , _peerMapping(peerMapping)
    , _peerBEP10SupportStatus(peerBEP10SupportStatus)
    , _peerBitSwaprBEPSupportStatus(peerBitSwaprBEPSupportStatus) {
}

ExtendedMessageIdMapping PeerPlugin::Configuration::clientMapping() const {
    return _clientMapping;
}

void PeerPlugin::Configuration::setClientMapping(const ExtendedMessageIdMapping & clientMapping) {
    _clientMapping = clientMapping;
}

ExtendedMessageIdMapping PeerPlugin::Configuration::peerMapping() const {
    return _peerMapping;
}

void PeerPlugin::Configuration::setPeerMapping(const ExtendedMessageIdMapping & peerMapping) {
    _peerMapping = peerMapping;
}

BEPSupportStatus PeerPlugin::Configuration::peerBEP10SupportStatus() const {
    return _peerBEP10SupportStatus;
}

void PeerPlugin::Configuration::setPeerBEP10SupportStatus(BEPSupportStatus peerBEP10SupportedStatus) {
    _peerBEP10SupportStatus = peerBEP10SupportedStatus;
}

BEPSupportStatus PeerPlugin::Configuration::peerBitSwaprBEPSupportStatus() const {
    return _peerBitSwaprBEPSupportStatus;
}

void PeerPlugin::Configuration::setPeerBitSwaprBEPSupportStatus(BEPSupportStatus peerBEP43SupportedStatus) {
    _peerBitSwaprBEPSupportStatus = peerBEP43SupportedStatus;
}

	
	/**
     * PeerPlugin
	 * @brief
     */
    class Configuration {

    public:

        // Default constructor
        Configuration();

        // Constructor from members
        Configuration(const ExtendedMessageIdMapping & clientMapping,
                      const ExtendedMessageIdMapping & peerMapping,
                      BEPSupportStatus peerBEP10SupportStatus,
                      BEPSupportStatus peerBitSwaprBEPSupportStatus);

        // Getters and Setters
        ExtendedMessageIdMapping clientMapping() const;
        void setClientMapping(const ExtendedMessageIdMapping & clientMapping);

        ExtendedMessageIdMapping peerMapping() const;
        void setPeerMapping(const ExtendedMessageIdMapping & peerMapping);

        BEPSupportStatus peerBEP10SupportStatus() const;
        void setPeerBEP10SupportStatus(BEPSupportStatus peerBEP10SupportStatus);

        BEPSupportStatus peerBitSwaprBEPSupportStatus() const;
        void setPeerBitSwaprBEPSupportStatus(BEPSupportStatus  peerBitSwaprBEPSupportStatus);

    private:

        // Mapping from messages to BEP10 ID of client
        ExtendedMessageIdMapping _clientMapping;

        // Mapping from messages to BEP10 ID of peer
        ExtendedMessageIdMapping _peerMapping;

        // Indicates whether peer supports BEP10
        BEPSupportStatus _peerBEP10SupportStatus;

        // Indicates whether peer supports BitSwapr BEP
        BEPSupportStatus _peerBitSwaprBEPSupportStatus;
    };



/**
     * @brief Configuration of buyer peer plugin.
     *
     * DOES NOT REPRESNT PERSISTENT STATE OF BUER PEER PLUGIN,
     * RATHER JUST SETTINGS NEEDED TO START A FRESH PLUGIN.
     * WE DO NOT SUPPORT PERSISINT PEER LEVEL PLUGINS AS OF YET.

    class Configuration : public PeerPlugin::Configuration {

    public:

        // Constructor
        Configuration();

        // Constructor from members
        Configuration(const ExtendedMessageIdMapping & clientMapping,
                      const ExtendedMessageIdMapping & peerMapping,
                      BEPSupportStatus peerBEP10SupportStatus,
                      BEPSupportStatus peerBitSwaprBEPSupportStatus,
                      const PeerState & peerState,
                      ClientState clientState,
                      quint32 payorSlot);

        // Getters and setters
        PeerState peerState() const;
        void setPeerState(const PeerState & peerState);

        ClientState clientState() const;
        void setClientState(ClientState clientState);

    private:

        // State of peer
        PeerState _peerState;

        // State of client
        ClientState _clientState;

        // Payor slot: payment channel output slot
        quint32 _payorSlot;
    };
    */

/*
Payor::Payor(const OutPoint& fundingOutput, const KeyPair& fundingOutputKeyPair)
    : _state(State::waiting_for_full_set_of_sellers)
    , _fundingOutput(fundingOutput)
    , _fundingOutputKeyPair(fundingOutputKeyPair)
    , _numberOfSignatures(0) {

    // Check that _fundingOutput
    // *exists
    // *is unspent
    // *has correct output script with correct sighash
    // *is controlled by _fundingOutputKeyPair
    // *has enough value
}

Payor::Payor(const QSet<Channel::PayorConfiguration> & configurations, const OutPoint& fundingOutput, const KeyPair& fundingOutputKeyPair)
    : _state(State::waiting_for_full_set_of_sellers)
    , _fundingOutput(fundingOutput)
    , _fundingOutputKeyPair(fundingOutputKeyPair)
    , _numberOfSignedSlots(0) {

    // Check that _fundingOutput
    // *exists
    // *is unspent
    // *has correct output script with correct sighash
    // *is controlled by _fundingOutputKeyPair
    // *has enough value

    quint32 index = 0;
    for(QSet<Channel::PayorConfiguration>::iterator i = configurations.begin(),
            end(configurations.end()); i != end;i++) {

        // Get configuration
        Channel::PayorConfiguration payorConfiguration = *i;

        // Add slot
        _channels.append(s);

        // Increment counter
        index++;
    }
}

quint32 Payor::addChannel(const Channel::PayorSettings & configuration) {

    // Check state
    if(_state != State::waiting_for_full_set_of_sellers)
        throw std::exception("State incompatile request, must be in waiting_for_full_set_of_sellers state.");

    // Get channel size
    quint32 index = _channels.size();

    // Create channel
    Channel c;
    c.index(index);
    c.state(Channel::State::unassigned);
    c.numberOfPaymentsMade(0);
    c.payorContractKeyPair(contractKeyPair);
    c.payorFinalKeyPair(finalKeyPair);
    c.refundFee(refundFee);
    c.funds(funds);

    // Add channel to vector of channels.
    _channels.p(c);

    // Return size of channel
    return index + 1;

    return 0;
}
*/









Payor::Payor(const Payor::Configuration & configuration)
    : _state(State::waiting_for_full_set_of_sellers)
    , _fundingOutPoint(c.fundingOutPoint())
    , _fundingOutputKeyPair(c.fundingOutPointPk(), _wallet->getSk(c.fundingOutPointPk()))
    , _changeOutputKeyPair(c.changeOutPointPk(), _wallet->getSk(c.changeOutPointPk()))
    , _changeValue(c.changeValue())
    , _maxPrice(c.maxPrice())
    , _maxLock(c.maxLock())
    , _numberOfSignatures(0){

    // Create channels
    quint32 index = 0;
    const QVector<Channel::PayorSettings> & settings = c.channels();
    for(QVector<Channel::PayorSettings>::const_iterator i = settings.constBegin();
        i != settings.constEnd();i++) {

        // Get settings for channel
        const Channel::PayorSettings & setting = *i;

        // Create channel
        Channel channel(index,
                        Channel::State::unassigned,
                        0,
                        0,
                        setting.funds(),
                        KeyPair(setting.contractPk(), _wallet->getSk(setting.contractPk())),
                        KeyPair(setting.finalPk(), _wallet->getSk(setting.finalPk())),
                        PublicKey(),
                        PublicKey(),
                        Signature(),
                        Signature(),
                        0,
                        0,
                        0);

        _channels.append(channel);

        index++;
    }
}
    
	
	/**
    // Count number of sellers
    quint32 numberOfChannels = funds.count();

    // Generate two key pairs for each channel, the payors contract and final keys
    QList<KeyPair> payorContractKeyPairs = BitSwaprjs::generate_fresh_key_pairs(numberOfChannels);
    QList<KeyPair> payorFinalKeyPairs = BitSwaprjs::generate_fresh_key_pairs(numberOfChannels);

    // Create payor channel configurations, and add to vector
    for(quint32 index = 0; index < numberOfChannels;index++)
        _channels.push_back(Channel::Configuration(index,
                                                   funds[index],
                                                   payorContractKeyPairs[index],
                                                   payorFinalKeyPairs[index]));

    */
		


/**
Payor::Channel::PayorSettings
         * @brief Payor provided settings in channel, as through wire message.
         

        class PayorSettings {

        public:

            // Default constructor
            PayorSettings();

            // Member constructor
            PayorSettings(quint64 funds, const PublicKey & contractPk, const PublicKey & finalPk);

            // Copy constructor
            PayorSettings(const PayorSettings & o);

            // Assignment operator
            PayorSettings & PayorSettings::operator=(const PayorSettings & o);

            // Getters and setters
            quint64 funds() const;
            void setFunds(quint64 funds);

            PublicKey contractPk() const;
            void setContractPk(const PublicKey & contractPk);

            PublicKey finalPk() const;
            void setFinalPk(const PublicKey & finalPk);

        private:

            // Funds allocated to output
            quint64 _funds;

            // Controls payour output of multisig
            PublicKey _contractPk;

            // Controls final payment to payor
            PublicKey _finalPk;
        };
        
        */




/*
std::set<libtorrent::sha1_hash> ControllerConfiguration::getTorrentInfoHashes() const {

    // Create vector for keeping keys
    std::set<libtorrent::sha1_hash> keys;

    // Iterate map and populate keys vector
    for(std::map<libtorrent::sha1_hash, TorrentConfiguration>::const_iterator i = _torrentConfigurations.begin(),
            end(_torrentConfigurations.end()); i != end;i++)
        keys.insert(i->first);

    // Return set
    return keys;
}
*/

/*
TorrentConfiguration * ControllerConfiguration::getTorrentConfiguration(const libtorrent::sha1_hash & info_hash) {

    // Look up configuration for torrrent with given info hash
    std::map<libtorrent::sha1_hash, TorrentConfiguration>::iterator & mapIterator = _torrentConfigurations.find(info_hash);

    // Return the configuration pointer if present
    if(mapIterator == _torrentConfigurations.end())
        throw NULL;
    else
        return mapIterator->second;
}
*/

/*
bool ControllerConfiguration::eraseTorrentConfiguration(const libtorrent::sha1_hash & info_hash) {

    // Try to find iterator reference to mathch
    std::map<libtorrent::sha1_hash, TorrentConfiguration>::iterator & mapIterator = _torrentConfigurations.find(info_hash);

    // Did we find match
    if(mapIterator == _torrentConfigurations.end())
         return false;
    else {

        // Erase
        _torrentConfigurations.erase(mapIterator);

        // Indicate it worked
        return true;
    }
}
*/

/*
bool Controller::addTorrent(const TorrentConfiguration & torrentConfiguration) {

    // Attempt to add to session
    if(addTorrentToSession(torrentConfiguration)) {

        // and add to controller if this was possible
        if(!_controllerConfiguration.addTorrentConfiguration(torrentConfiguration)) {

            // Could add to session, but not controller!!!
            return false;
        }

        // Could add to both session and controller
        return true;

    } else // Could not add to session
        return false;
}
*/

/*
const TorrentPluginConfiguration * Controller::getTorrentPluginConfiguration(const libtorrent::sha1_hash & info_hash) {

    const TorrentPluginConfiguration * torrentPluginConfiguration = NULL;

    try{

        // Get torrent plugin configuration
        torrentPluginConfiguration = _controllerConfiguration.getTorrentConfiguration(info_hash).getTorrentPluginConfiguration();

    } catch (std::exception & e) {

        // Write critial warning
        qCCritical(_category) << "exception caught: " << e.what() << '\n';

        return NULL;
    }

    // Return pointer
    return torrentPluginConfiguration;
}
*/

boost::shared_ptr<libtorrent::peer_plugin> TorrentPlugin::new_connection(libtorrent::peer_connection * peerConnection) {

    /**
     * Libtorrent docs (http://libtorrent.org/reference-Plugins.html#peer_plugin):
     * ===========================================================================
     * The peer_connection will be valid as long as the shared_ptr is being held by the
     * torrent object. So, it is generally a good idea to not keep a shared_ptr to
     * your own peer_plugin. If you want to keep references to it, use weak_ptr.
     */

    // Get end point to look up sets
    const libtorrent::tcp::endpoint & endPoint = peerConnection->remote();
    std::string endPointString = libtorrent::print_endpoint(endPoint);

    qCDebug(category_) << "New connection from" << endPointString.c_str();

    // Check if this peer should be accepted, if not
    // a null is returned, hence plugin is not installed
    if(!TorrentPlugin::installPluginOnNewConnection(peerConnection)) {
        qCDebug(category_) << "Rejected connection from peer, peer plugin not installed.";
        return boost::shared_ptr<libtorrent::peer_plugin>();
    }

    // Create seller peer
    libtorrent::bt_peer_connection * bittorrentPeerConnection = static_cast<libtorrent::bt_peer_connection*>(peerConnection);
    PeerPlugin * peerPlugin = new SellerPeerPlugin(this, bittorrentPeerConnection, category_);

    // Add to collection
    peerPlugins_.insert(std::make_pair(endPoint, peerPlugin));

    // Connect peer plugin signal to controller slot
    //Controller * controller = plugin_->getController();
    //
    //QObject::connect(peerPlugin,
    //                 SIGNAL(peerPluginStatusUpdated(const PeerPluginStatus&)),
    //                 controller,
    //                 SLOT(updatePeerPluginStatus(const PeerPluginStatus&)));

    qCDebug(category_) << "Seller #" << peerPlugins_.size() << endPointString.c_str() << "added to " << this->torrent_->name().c_str();

    // Emit peer added signal
    // Should not be here, should be when a payment channel actually starts
    //emit peerAdded(peerPlugin->getPeerPluginId());

    // Return pointer to plugin as required
    return boost::shared_ptr<libtorrent::peer_plugin>(peerPlugin);
}


boost::shared_ptr<libtorrent::torrent_plugin> Plugin::new_torrent(libtorrent::torrent * newTorrent, void * userData) {

    
    // Check what sort of plugin, if any, should be installed on this torrent
    TorrentPluginConfiguration * torrentPluginConfiguration = _controller->getTorrentPluginConfiguration(newTorrent->info_hash());

    if(!torrentPluginConfiguration) {
        qCDebug(_category) << "Plugin not installed on new torrent.";
        return boost::shared_ptr<libtorrent::torrent_plugin>();
    }

    // Create the appropriate torrent plugin depending on if we have full file
    TorrentPlugin * torrentPlugin;

    const BuyerTorrentPluginConfiguration * potentialBuyerTorrentPluginConfiguration = dynamic_cast<BuyerTorrentPluginConfiguration*>(torrentPluginConfiguration);
    const SellerTorrentPluginConfiguration * potentialSellerTorrentPluginConfiguration = dynamic_cast<SellerTorrentPluginConfiguration*>(torrentPluginConfiguration);

    if(potentialBuyerTorrentPluginConfiguration) {
        torrentPlugin = new BuyerTorrentPlugin(this, newTorrent, _category, true, potentialBuyerTorrentPluginConfiguration);
    else if(potentialSellerTorrentPluginConfiguration)
        torrentPlugin = new SellerTorrentPlugin(this, newTorrent, _category, true, potentialSellerTorrentPluginConfiguration);
    else {

        qCDebug(_category) << "Type disaster!!!!. Plugin not installed on new torrent.";
        return boost::shared_ptr<libtorrent::torrent_plugin>();
    }


    // Add to collection
    _torrentPlugins.insert(std::make_pair(newTorrent->info_hash(), torrentPlugin));

    // Connect torrent plugin signal
    /*
    //qRegisterMetaType<TorrentPluginStatus>();
    QObject::connect(torrentPlugin,
                     SIGNAL(updateTorrentPluginStatus(TorrentPluginStatus)),
                     controller_,
                     SLOT(updateTorrentPluginStatus(TorrentPluginStatus)));

    //qRegisterMetaType<libtorrent::tcp::endpoint>();

    QObject::connect(torrentPlugin,
                     SIGNAL(peerAdded(libtorrent::tcp::endpoint)),
                     controller_,
                     SLOT(extensionPeerAdded(libtorrent::tcp::endpoint)));

    QObject::connect(torrentPlugin,
                     SIGNAL(peerRemoved(libtorrent::tcp::endpoint)),
                     controller_,
                     SLOT(removePeer(libtorrent::tcp::endpoint)));
*/
    // Diagnostic
    qCDebug(_category) << "Torrent #" << _torrentPlugins.size() << " added.";

    // Return
    return boost::shared_ptr<libtorrent::torrent_plugin>(torrentPlugin);
}


/*
bool Controller::saveResumeDataForTorrent(QString const & save_path, QString const & file_name, std::vector<char> const & resume_data) const {

    // Check that save_path still exists, if not create it,
    // because it could have been deleted by user since addTorrent()
    // If you cant create it, then return false
    if(!(QDir().exists(save_path) || QDir().mkpath(save_path))) {

        qCWarning(category_) << "Could not create save_path: " << save_path.toStdString().c_str();
        return false;
    }

    // Open file
    QString resumeFile = save_path + QDir::separator() + file_name;
    QFile file(resumeFile);

    if(!file.open(QIODevice::WriteOnly)) {

        qCCritical(category_) << "Could not open : " << resumeFile.toStdString().c_str();
        return false;
    }

    // Write to file
    file.write(&(resume_data[0]), resume_data.size());

    // Close file
    file.close();

    return true;
}

QString Controller::resumeFileNameForTorrent(libtorrent::sha1_hash & info_hash) const {
    return QString((libtorrent::to_hex(info_hash.to_string()) + ".resume").c_str());
}

// QString const & save_path, QString const & file_name, std::vector<char> * resume_data
bool Controller::loadResumeDataForTorrent(libtorrent::add_torrent_params & params) const {

    // Check that file exists
    QString save_path(params.save_path.c_str());
    QString file_name = resumeFileNameForTorrent(params.info_hash);
    QString resumeFile = save_path + QDir::separator() + file_name;

    QFile file(resumeFile);

    if(file.exists()) {

        // Open file
        if(!file.open(QIODevice::ReadOnly)) {
            qCWarning(category_) << "Could not open : " << resumeFile.toStdString().c_str();
            return false;
        }

        // Read entire file
        QByteArray fullFile = file.readAll();

        // Close file
        file.close();

        // Populate resume_data vector
        params.resume_data = new std::vector<char>();
        for(QByteArray::iterator i = fullFile.begin(), end(fullFile.end()); i != end; i++)
            params.resume_data->push_back(*i);

        return true;
    } else
        return false;
}


// Assignment operator required to put in std::map
PeerPluginViewModel & operator=(const PeerPluginViewModel& rhs);

// Assignment operator required to put in std::map
PeerPluginViewModel & PeerPluginViewModel::operator=(const PeerPluginViewModel& rhs) {

    // Copy fields
    endPoint_ = rhs.getEndPoint();
    peerPluginsTableViewModel_ = rhs.getPeerPluginsTableViewModel();

    hostItem = rhs.getHostItem();
    stateItem = rhs.getStateItem();
    balanceItem = rhs.getBalanceItem();
    progressItem = rhs.getProgressItem();

    // Change item data field to this object
    setItemData();

    return *this;
}

    // Remove corresponding row from torrentTableViewModel_
    QModelIndex index = torrentTableViewModel_->indexFromItem(nameItem);

    torrentTableViewModel_->removeRows(index.row(), 1);

    // Delete view model for peer plugins table,
    // this also will automatically delete all items in model
    delete peerPluginsTableViewModel_;
	
	
	
	
TorrentViewModel & TorrentViewModel::operator=(const TorrentViewModel& rhs) {

    // Copy fields
    info_hash_ = rhs.getInfoHash();

    torrentTableViewModel_ = rhs.getTorrentTableViewModel();
    peerPluginsTableViewModel_ = rhs.getPeerPluginsTableViewModel();
    //CATEGORY = rhs.getCategory(); // Cannot be reassigned

    nameItem = rhs.getNameItem();
    sizeItem = rhs.getSizeItem();
    stateItem = rhs.getStateItem();
    speedItem = rhs.getSpeedItem();
    peersItem = rhs.getPeersItem();
    modeItem = rhs.getModeItem();
    peerPluginsItem = rhs.getPeerPluginsItem();
    balanceItem = rhs.getBalanceItem();

    // Change item data field to this object
    setItemData();

    return *this;
}





























*/
