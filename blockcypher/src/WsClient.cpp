
#include <blockcypher/TX.hpp>
#include <blockcypher/WsClient.hpp>


#include <QDebug>
#include <QAbstractSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>


//Suggestions for future expansion:
// - Should there be a way to restore filters if connection is dropped?
//   In that case, there would need to be a storage structure for filters that are applied, also a timer
//   that monitors the connection, and restores the filters as needed. A downside to this could be that
//   a notification is lost during downtime.


namespace BlockCypher {

    WSClient::WSClient(QObject *parent) : QObject(parent), client_socket(), client_url(BLOCKCYPHER_WEBSOCKET_TESTNET3_ENDPOINT) {

        //Signals connected to lambda functions.
        connect(&_timer, &QTimer::timeout, [this](){keepAlive();});
        connect(&client_socket,&QWebSocket::connected,[this](){wsConnected();});
        connect(&client_socket,&QWebSocket::disconnected,[this](){wsDisconnected();});
        connect(&client_socket,&QWebSocket::textMessageReceived,[this](QString s){msgIncoming(s);});
        connect(&client_socket, static_cast<void (QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error),
                [this]( QAbstractSocket::SocketError e ){wsError(e);});
    }

    void WSClient::wsConnected(void) {
        qDebug() << "WSClient::wsConnected -> Connected!";
        _timer.start(TIMERINTERVALL);
        emit connected();
    }

    void WSClient::wsDisconnected(void) {
        _timer.stop();
        emit disconnected();
    }

    void WSClient::wsError(QAbstractSocket::SocketError error) {
        qDebug() << "WsError" << error;
    }

    //Add events to the active filter.
    void WSClient::addEvent(const Event & e) {

        if(isConnected()) {
            //Does serialization and sends to blockcypher.
            qDebug() << "DEBUG: WSClient::monitor :Client is connected ";
            QJsonObject t = e.toJson();
            QJsonDocument msg(t);

            //Debug code
            //QString strJson(msg.toJson(QJsonDocument::Compact));
            //qDebug() << "DEBUG: WSClient::monitor : Sending: " << strJson;

            client_socket.sendBinaryMessage(msg.toJson());
        }else {
            //TODO: Write to error log here? Notify user?
            qDebug() << "Not connected!";
        }

    }

    void WSClient::openConnection(void) {
        if (!isConnected()) {
            qDebug() << "Client socket opened!";
            client_socket.open(client_url);
        }
    }

    void WSClient::closeConnection(void) {
        if (isConnected()) {
            qDebug() << "Socket close";
            client_socket.close();
        }
    }

    //Handles incoming message from websocket and 
    //emits relevant signal with relevant object as payload
    //Notice: Even if we use sendBinaryMessage over the active
    //websocket, we get a textMessageReceived in return fom BlockCypher.
    void WSClient::msgIncoming(QString msg) {

        //Each of the diferent event types will likely result in a different type of json response string
        //Note: 15th Jan 2016: Only tx-confirmation event with TX payload is detected.


        TX TXob; //TX object based on notification.
        QList<TXInput> inputs;
        QList<TXOutput> outputs;
        QStringList addresses;
        QStringList block_key_list;
        QStringList tx_key_list;

        //TODO: Suggested improvement, this should ideally not be hardcoded here!
        QString tx_toplevel_keys = "addresses,block_height,block_index,confidence,confirmations,double_spend,"
            "fees,hash,inputs,lock_time,outputs,preference,received,relayed_by,size,total,ver,vin_sz,vout_sz";
        QString block_toplevel_keys = "bits,chain,depth,fees,hash,height,mrkl_root,n_tx,nonce,prev_block,"
            "prev_block_url,received_time,relayed_by,size,time,total,tx_url,txids,ver";

        tx_key_list = tx_toplevel_keys.split(",");
        block_key_list = block_toplevel_keys.split(",");

        //Get the responseObject
        QJsonDocument jsonResponse = QJsonDocument::fromJson(msg.toUtf8());
        QJsonObject responseObject = jsonResponse.object();



        //Check if we have tx notification
        if(probeNotification(tx_key_list, responseObject)) {

            qDebug() << "\nDEBUG: Inserting to TX object";

            //_hash
            TXob.setHash(responseObject.value("hash").toString());

            //_blockheight
            TXob.setBlockHeight((qint32)responseObject.value("block_height").toDouble());

            //_addresses
            //QJsonArray adr_arr = ;

            foreach (const QJsonValue & value, (responseObject.value("addresses")).toArray()) {
                addresses.append(value.toString());
            }
            TXob.setAddresses(addresses);

            //_total
            TXob.setTotal((quint64)responseObject.value("total").toDouble());

            //_fees
            TXob.setFees((quint64)responseObject.value("fees").toDouble());

            //_received (Example incoming value: 2016-01-06T16:22:59Z)
            TXob.setReceived(QDateTime::fromString((responseObject.value("received").toString()).left(23), "yyyy-MM-ddThh:mm:ss.zzz"));

            //_ver
            TXob.setVersion((quint32)responseObject.value("ver").toDouble());

            //_lock_time
            TXob.setLockTime((quint32)responseObject.value("lock_time").toDouble());

            //_double_spend
            TXob.setDoubleSpend(responseObject.value("double_spend").toBool());

            //_vin_sz : tot number of inputs in the tx
            TXob.setNumInputs(responseObject.value("vin_sz").toInt());

            //_vout_sz : tot number of outputs in the tx
            TXob.setNumOutputs(responseObject.value("vout_sz").toInt());

            //_confirmations
            int confirmations = (responseObject.value("confirmations")).toInt();
            TXob.setConfirmations(confirmations);

            //Both inputs and outputs will always be an array, with at least 1 element.

            //_inputs
            getelements(inputs,responseObject.value("inputs"));
            TXob.setInputs(inputs);

            /* Example JSON:
            "inputs": [{ "prev_hash": "4822fbc9f90f2f1a26941e1c1f97aefdd4cd3b31c898b366a00a33699d9d998f",
                  "output_index": 0,
                  "script": "473044022070a48a7ede9706a1d7823690c.....2f7fd66fb87bd967dd83d58fd53",
                  "output_value": 33844495, "sequence": 4294967294,
                  "addresses": [ "mgwdZ6W5vxBNe1TXqaqHtkCG7hMmCQDqu4" ],
                  "script_type": "pay-to-pubkey-hash", "age": 7}]
            */

            //_outputs
            getelements(outputs,responseObject.value("outputs"));
            TXob.setOutputs(outputs);

            if(confirmations > 0) { //Confirmed tx
                //Let's get block_hash and confirmed

                TXob.setBlockHash(responseObject.value("block_hash").toString());
                TXob.setConfirmed(QDateTime::fromString((responseObject.value("confirmed").toString()).left(23), "yyyy-MM-ddThh:mm:ss.zzz"));
            }

            emit(txArrived(TXob));

        //Check if we have a new block notification
        } else if(probeNotification(block_key_list, responseObject)){

            // /////////////////////////////////////////////////////////////////////////////////////////////

            /*
             * Example new-block notification:
            {
               "bits":486604799,
               "chain":"BTC.test3",
               "depth":0,
               "fees":10059562,
               "hash":"00000000001c72812c81341f3514d89920c4069387f3961e8a5eda717bd1b5d0",
               "height":633862,
               "mrkl_root":"c7f769f786ba95e99ee284068d05f8e9797afa6e52359fad1dfbca4b77d423cc",
               "n_tx":239,
               //OPTIONAL FIELD:// "next_txids":"https://api.blockcypher.com/v1/btc/test3/blocks/chain?txstart=100&limit=100",
               "nonce":2664042151,
               "prev_block":"00000000000008fabb9275aa424c2c3e28c9878f57c657e9acd3ec3a39932420",
               "prev_block_url":"https://api.blockcypher.com/v1/btc/test3/blocks/chain",
               "received_time":"2016-01-15T22:42:41Z",
               "relayed_by":"",
               "size":203286,
               "time":"2016-01-15T22:42:41Z",
               "total":61486721031,
               "tx_url":"https://api.blockcypher.com/v1/btc/test3/txs/",
               "txids":[ ],
               "ver":4
            }
        */
            // ////////////////////////////////////////////////////////////////////////////////////////////////
            qDebug() << "\nDEBUG: Inserting to Block object";
            qDebug() << "\nTODO: Not implemented.";

            //emit(newBlock(Block));

        } else {

            qDebug() << "We have have something else than a TX or New Block coming in.";

            //TODO: This is only temporary. Replace with relevant routine(s) like txArrived and newBlock.
            emit ws_msgReceived(QJsonDocument::fromJson(msg.toUtf8()));
        }
    }

    void WSClient::keepAlive(void) {

        if(isConnected()) {
            qDebug() << "PING!";
            client_socket.ping();
            /*
             * Orginal ping code:
             * Let's keep the orginal code, until we are sure the new shorter
             * version works ok.
             *
                      QJsonObject obdata;
                      obdata["event"] = QString("ping");
                      QJsonDocument doc(obdata);

                      QString strJson(doc.toJson(QJsonDocument::Compact));
                      qDebug() << "DEBUG: WSClient::keepAlive : Sending: " << strJson;

                      client_socket.sendBinaryMessage(doc.toJson());
            */

        }else {
            //TODO: What to do here? Reconnect, write error log, notify user?
            qDebug() << "Not connected!";
        }
    }

    template<class T>  // Get all elements from array
    void WSClient::getelements(QList<T> l, const QJsonValue & val) {
        QJsonArray arr1 = val.toArray();
        foreach (const QJsonValue & value, arr1) {
            T element(value.toObject());
            l.append(element);
        }
    }

    bool WSClient::probeNotification(QStringList list,const QJsonObject &ob) {

        for (QStringList::iterator it = list.begin(); it != list.end(); ++it) {

            QString current = *it;
            qDebug() << "Debug: [[" << current << "]] Value: "<< ob.value(current);
            if(ob.value(current) == QJsonValue::Undefined) {
                return false;
            }
        }
        return true;
    }

} // end namespace BlockCypher
