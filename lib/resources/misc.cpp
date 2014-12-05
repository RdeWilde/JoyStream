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
