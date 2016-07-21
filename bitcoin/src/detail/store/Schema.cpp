#include "Schema.hpp"
#include "Schema-odb.hxx"

#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <odb/sqlite/database.hxx>

#include <CoinCore/hash.h> // ripemd160(sha256(pubkey))

#include <bitcoin/Common.hpp>

namespace joystream {
namespace bitcoin {
namespace detail {
namespace store {

/// Metadata

    Metadata::Metadata(){}

    Metadata::Metadata(std::string entropy, uint32_t created_utc, Coin::Network network) :
         entropy_(entropy)
        ,created_(created_utc)
        ,encrypted_(false)
        ,network_(network)
    {}

    std::string Metadata::entropy() const {
        return entropy_;
    }

    uint32_t Metadata::created() const {
        return created_;
    }

    bool Metadata::encrypted() const {
        return encrypted_;
    }

    void Metadata::encrypted(bool encrypted) {
        encrypted_ = encrypted;
    }

    Coin::Network Metadata::network() const {
        return network_;
    }

/// Key

    Key::Key(uint32_t coin_type, uint32_t change, uint32_t index) :
        path_({coin_type, change, index}),
        generated_(std::time(nullptr))
    {}

    Key::Key() :
        generated_(std::time(nullptr))
    {}

    uint32_t Key::index() const {
        return path_.index;
    }

    uint32_t Key::change() const {
        return path_.change;
    }

    uint32_t Key::coin_type() const {
        return path_.coin_type;
    }

    uint32_t Key::generated() const {
        return generated_;
    }

/// Address

    Address::Address(const std::shared_ptr<Key> & key, const RedeemScriptInfo &scriptInfo) {
        key_ = key;
        redeemScript_ = scriptInfo.redeemScript.getHex();
        optionalData_ = scriptInfo.optionalData.getHex();
        scriptPubKey_ = Coin::P2SHScriptPubKey(Coin::RedeemScriptHash::fromRawScript(scriptInfo.redeemScript)).serialize().getHex();
        //scriptPubKey_ = scriptPubKey.serialize().getHex();
    }

    Address::Address(const std::shared_ptr<Key> & key, const Coin::Script & scriptPubKey) {
        key_ = key;
        //scriptPubKey_ = Coin::P2PKHScriptPubKey(key->getPrivateKey().toPublicKey()).serialize().getHex();
        scriptPubKey_ = scriptPubKey.serialize().getHex();
    }

/// Transaction

    Transaction::Transaction(const Coin::Transaction & t)
    {
        version_ = t.version;
        lockTime_ = t.lockTime;
        txid_ = Coin::TransactionId::fromTx(t).toHex().toStdString();
        //seen_ = 0; //should be initialised
    }

/// Input

    Input::Input(const Coin::TxIn& txin) {
        id_.op_txid_ = txin.getOutpointHash().getHex();
        id_.op_index_ = txin.getOutpointIndex();
        id_.scriptSig_ = txin.scriptSig.getHex();
        id_.sequence_ = txin.sequence;
    }

    Coin::TxIn Input::toCoinCore() const {
        return Coin::TxIn(Coin::OutPoint(outpoint_hash(), outpoint_index()),
                          script(),
                          sequence());
    }

    bool operator< (const Input_id &lhs, const Input_id &rhs) {

        if(lhs.op_txid_ == rhs.op_txid_){
            if(lhs.op_index_.get() == rhs.op_index_.get()){
                if(lhs.scriptSig_ == rhs.scriptSig_) {
                    return lhs.sequence_.get() < rhs.sequence_.get();
                }
                return lhs.scriptSig_ < rhs.scriptSig_;
            }
            return lhs.op_index_.get() < rhs.op_index_.get();
        }
        return lhs.op_txid_ < rhs.op_txid_;
    }

    std::string Input::toString() {
        return toCoinCore().toString();
    }

/// Output

    Output::Output(uint64_t value, std::shared_ptr<Address> & address)
        : address_(address)
    {
        id_.value_ = value;
        id_.scriptPubKey_ = address->scriptPubKey();
    }

    Output::Output(const Coin::TxOut & txout)
    {
        id_.value_ = txout.value;
        id_.scriptPubKey_ = txout.scriptPubKey.getHex();
    }

    bool operator< (const Output_id &lhs, const Output_id &rhs) {
        if(lhs.scriptPubKey_ == rhs.scriptPubKey_){
            return lhs.value_ < rhs.value_;
        }
        return lhs.scriptPubKey_ < rhs.scriptPubKey_;
    }

    Coin::TxOut Output::toCoinCore() const
    {
        return Coin::TxOut(value(), script());
    }

/// TxHasInput

    TxHasInput::TxHasInput(const std::shared_ptr<Transaction> & t, uint32_t index,
                           const std::shared_ptr<Input> &input) {
        if(t == nullptr || input == nullptr) {
            throw std::runtime_error("TxHasInput nullptr arguments");
        }
        tx_ix_.tx = t;
        tx_ix_.index_ = index;
        input_ = input;
    }

/// TxHasOutput

    TxHasOutput::TxHasOutput(const std::shared_ptr<Transaction> & t, uint32_t index,
                             const std::shared_ptr<Output> &output) {
        if(t == nullptr || output == nullptr) {
            throw std::runtime_error("TxHasOutput nullptr arguments");
        }
        tx_ix_.tx = t;
        tx_ix_.index_ = index;
        output_ = output;
    }

/// tx_ix_t

    bool operator< (const tx_ix_t &lhs, const tx_ix_t &rhs) {
        if(lhs.tx->txid() == rhs.tx->txid()){
            return lhs.index_.get() < rhs.index_.get();
        }
        return lhs.tx->txid() < rhs.tx->txid();
    }

    // BlockHeader

    BlockHeader::BlockHeader(const ChainMerkleBlock &header) {
        id_ =  header.hash().getHex();
        height_ = header.height;
    }

#ifdef USE_STORE_ALPHA_CODE
/// InBoundPayment

    InBoundPayment::InBoundPayment(const std::shared_ptr<Address> &address,
        const std::string &note) :
        receiveAddress_(address),
        note_(note),
        created_(std::time(nullptr))
    {}

/// OutBoundPayment

    OutBoundPayment::OutBoundPayment(const Coin::P2PKHAddress & toAddress,
        const std::shared_ptr<Address> & changeAddress, uint64_t amount,
        uint64_t fee, const std::string &note) :
        toAddress_(toAddress.toBase58CheckEncoding().toStdString()),
        changeAddress_(changeAddress),
        amount_(amount),
        fee_(fee),
        note_(note),
        created_(std::time(nullptr))
    {}

// Payer

    Payer::Payer(std::shared_ptr<Transaction> tx)
        : contract_(tx)
    {}

// OutputFundsPayer

    OutputFundsPayer::OutputFundsPayer(std::shared_ptr<Output> output,
                                       std::shared_ptr<Payer> payer)
    {
        output_payer_.output_ = output;
        output_payer_.payer_ = payer;
    }
#endif // store alpha code

}//store
}//detail
}//bitcoin
}//joystream

