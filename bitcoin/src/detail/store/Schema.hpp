#ifndef BITCOIN_DETAIL_STORE_SCHEMA_HPP
#define BITCOIN_DETAIL_STORE_SCHEMA_HPP

#include <CoinQ/CoinQ_blocks.h>
#include <CoinCore/CoinNodeData.h>
#include <CoinCore/typedefs.h> //bytes_t
#include <stdutils/uchar_vector.h> //uchar_vector

#include <common/Network.hpp>
#include <common/Seed.hpp>
#include <common/PrivateKey.hpp>
#include <common/PublicKey.hpp>
#include <common/PubKeyHash.hpp>
#include <common/P2PKHAddress.hpp>
#include <common/P2PKHScriptPubKey.hpp>

#include <common/TransactionId.hpp>
#include <common/BlockId.hpp>
#include <common/Utilities.hpp>

#include <memory>

#include <odb/core.hxx>
#include <odb/nullable.hxx>

namespace joystream {
namespace bitcoin {
namespace detail {
namespace store {

#pragma db model version(1, 1)

/*
 * Metadata
 */

//no_id - we don't intend to have more than one Metadata object in the database
#pragma db object no_id pointer(std::shared_ptr)
class Metadata : public std::enable_shared_from_this<Metadata> {
  public:
    std::shared_ptr<Metadata> get_shared_ptr() { return shared_from_this(); }

    Metadata();
    Metadata(std::string seed, Coin::Network network, uint32_t created_utc);

    std::string seed() const;
    Coin::Network network() const;
    uint32_t created() const;

  private:
    friend class odb::access;

    //TODO: store encrypted seed
    #pragma db not_null
    std::string seed_;
    #pragma db not_null
    Coin::Network network_;
    //utc unix timestamp: QDateTime::fromTime_t(created_) to convert to QDateTime local time
    uint32_t created_;
};

/*
 * Key
 */

#pragma db object pointer(std::shared_ptr) session
class Key : public std::enable_shared_from_this<Key> {
public:
    std::shared_ptr<Key> get_shared_ptr() { return shared_from_this(); }

    Key(uint32_t index, bool used = true);
    Key(bool used = true);

    uint32_t id() const;
    uint32_t generated() const;
    bool used() const;
    void used(bool);

private:
    friend class odb::access;

    #pragma db id auto
    uint32_t index_;
    //utc unix timestamp: QDateTime::fromTime_t(created_) to convert to QDateTime local time
    uint32_t generated_;
    bool used_;
};

/*
 * Address
 */

#pragma db object pointer(std::shared_ptr) session
class Address : public std::enable_shared_from_this<Address> {
public:
    std::shared_ptr<Address> get_shared_ptr() { return shared_from_this(); }

    Address() {}
    Address(const std::shared_ptr<Key> & key_, const Coin::P2PKHAddress & p2pkh_addr);
    const std::shared_ptr<Key> key() const { return key_; }
    //Coin::P2PKHAddress toP2PKHAddress() const;
    std::string scriptPubKey() const { return scriptPubKey_; }

private:
    friend class odb::access;

    #pragma db id auto
    unsigned long id_;

    std::string address_; //Base58CheckEncoded string

    #pragma db not_null unique //unique - it doesn't make sense to have two address objects for the same key
    std::shared_ptr<Key> key_; //to establish a relation with the Wallet::Key

    #pragma db not_null
    std::string scriptPubKey_;//to lookup outputs to this address
};

/*
 * Output_id
 */

#pragma db value
struct Output_id {
    #pragma db not_null
    odb::nullable<uint64_t> value_;
    #pragma db not_null
    std::string scriptPubKey_;
    friend bool operator< (const Output_id & lhs, const Output_id & rhs);
};

/*
 * Output
 */

#pragma db object pointer(std::shared_ptr) session
class Output : public std::enable_shared_from_this<Output> {
public:
    std::shared_ptr<Output> get_shared_ptr() { return shared_from_this(); }
    Output() {}

    Output(uint64_t value, std::shared_ptr<Address> & address);

    Output(const Coin::TxOut & txout);
    Coin::TxOut toCoinCore() const;

    Output_id id() const { return id_; }
    uint64_t value() const { return id_.value_.get(); }

    const std::string& script() const { return id_.scriptPubKey_; }

    void address(std::shared_ptr<Address> a) {address_ = a; }
    const std::shared_ptr<Address> address() const { return address_; }

private:
    friend class odb::access;

    #pragma db id
    Output_id id_;

    //pointer to our Address if we are controlling this output
    //set when constructing a transaction, and updated when adding a new transaction
    //from the network
    #pragma db null
    std::shared_ptr<Address> address_;
};

/*
 * Input_id
 */

#pragma db value
struct Input_id {
    std::string op_txid_;
    #pragma db not_null
    odb::nullable<uint32_t> op_index_;
    std::string scriptSig_;
    #pragma db not_null
    odb::nullable<uint32_t> sequence_;
    friend bool operator< (const Input_id & lhs, const Input_id & rhs);
};

/*
 * Input
 */

#pragma db object pointer(std::shared_ptr) session
class Input : public std::enable_shared_from_this<Input> {
public:
    std::shared_ptr<Input> get_shared_ptr() { return shared_from_this(); }

    Input() {}

    Input(const Coin::TxIn & txIn);

    Coin::TxIn toCoinCore() const;

    Input_id id() const { return id_; }

    const std::string& outpoint_hash() const { return id_.op_txid_; }
    uint32_t outpoint_index() const { return id_.op_index_.get(); }

    const std::string& script() const { return id_.scriptSig_; }

    uint32_t sequence() const { return id_.sequence_.get(); }

    std::string toString();

private:
    friend class odb::access;

    #pragma db id
    Input_id id_;
};

/*
 * Transaction
 */

#pragma db object pointer(std::shared_ptr) session
class Transaction : public std::enable_shared_from_this<Transaction> {
public:
    std::shared_ptr<Transaction> get_shared_ptr() { return shared_from_this(); }

    Transaction() {}

    Transaction(const Coin::Transaction & t);

    std::string txid() { return txid_; }
    uint32_t version() const { return version_; }
    uint32_t locktime() const { return lockTime_; }

private:
    friend class odb::access;

    #pragma db id
    std::string txid_; //reverse byte order
    #pragma db readonly
    uint32_t version_;
    #pragma db readonly
    uint32_t lockTime_;
    //uint32_t seen_;

};

/*
 * tx_ix_t
 * The intent is to have a transaction id and index to be a primary key,
 * and the transaction id to also be a foreign key. But since ODB doesn't
 * yet support having a smart pointer to a persisted object in a composite
 * value used as an object id, we can use this composite value as a data
 * member and create a unique index.
 *
 */
#pragma db value
struct tx_ix_t {
    #pragma db not_null
    std::shared_ptr<Transaction> tx;
    #pragma db not_null
    odb::nullable<uint32_t> index_;
    friend bool operator< (const tx_ix_t & lhs, const tx_ix_t & rhs);
};

/*
 * TxHasInput
 */

#pragma db object pointer(std::shared_ptr)
class TxHasInput : public std::enable_shared_from_this<TxHasInput> {
public:
    std::shared_ptr<TxHasInput> get_shared_ptr() { return shared_from_this(); }
    TxHasInput() {}
    TxHasInput(const std::shared_ptr<Transaction> & t, uint32_t index, const std::shared_ptr<Input> &input);
    std::string outpoint_hash() const { return input_->outpoint_hash(); }
    uint32_t outpoint_index() const { return input_->outpoint_index(); }
    std::string txid() const { return tx_ix_.tx->txid(); }
    uint32_t index() const { return tx_ix_.index_.get(); }
    const std::shared_ptr<Input> input() const { return input_; }
private:
    friend class odb::access;

    #pragma db id auto
    unsigned long id_;

    #pragma db index unique not_null readonly
    tx_ix_t tx_ix_;

    //FK constraint
    #pragma db not_null readonly
    std::shared_ptr<Input> input_;
};

/*
 * TxHasOutput
 */

#pragma db object pointer(std::shared_ptr)
class TxHasOutput : public std::enable_shared_from_this<TxHasOutput> {
public:
    std::shared_ptr<TxHasOutput> get_shared_ptr() { return shared_from_this(); }

    TxHasOutput() {}
    TxHasOutput(const std::shared_ptr<Transaction> & t, uint32_t index, const std::shared_ptr<Output> &output);
    std::string txid() const { return tx_ix_.tx->txid(); }
    uint32_t index() const { return tx_ix_.index_.get(); }
    const std::shared_ptr<Output> output() const { return output_; }

private:
    friend class odb::access;

    #pragma db id auto
    unsigned long id_;

    #pragma db index unique readonly
    tx_ix_t tx_ix_;

    //FK constraint
    #pragma db not_null readonly
    std::shared_ptr<Output> output_;

};

#pragma db view object(Transaction)
typedef struct {
  #pragma db column("count(" + Transaction::txid_ + ")")
  uint64_t count;
} transaction_stat_t;

#pragma db view object(Key)
typedef struct {
  #pragma db column("count(" + Key::index_ + ")")
  uint32_t count;

  #pragma db column("max(" + Key::index_ + ")")
  uint32_t max;
} key_stat_t;

#pragma db view object(Key = key) object(Address = address)
typedef struct {
    std::shared_ptr<Key> key;
} key_view_t;


/*
 * InBoundPayment
 */

#pragma db object pointer(std::shared_ptr)
class InBoundPayment : public std::enable_shared_from_this<InBoundPayment> {
public:
    std::shared_ptr<InBoundPayment> get_shared_ptr() { return shared_from_this(); }

    InBoundPayment() {}
    InBoundPayment(const std::shared_ptr<Address> & address,
        const std::string & note);

private:
    friend class odb::access;

    #pragma db id auto
    unsigned long id_;

    uint32_t created_;
    std::string note_;
    std::shared_ptr<Address> receiveAddress_;
};

/*
 * OutBoundPayment
 */

#pragma db object pointer(std::shared_ptr)
class OutBoundPayment : public std::enable_shared_from_this<OutBoundPayment> {
public:
    std::shared_ptr<OutBoundPayment> get_shared_ptr() { return shared_from_this(); }

    OutBoundPayment() {}
    OutBoundPayment(const Coin::P2PKHAddress & toAddress,
        const std::shared_ptr<Address> &changeAddress, uint64_t amount,
        uint64_t fee, const std::string & note);

private:
    friend class odb::access;

    #pragma db id auto
    unsigned long id_;

    uint64_t fee_;
    uint64_t amount_;
    std::string note_;
    uint32_t created_;
    std::string toAddress_;
    std::shared_ptr<Address> changeAddress_;
};


#pragma db value
struct block_header_t {
    uint32_t version_;
    std::string prevBlockHash_;
    std::string merkelRoot_;
    uint32_t timestamp_;
    uint32_t bits_;
    uint32_t nonce_;
};

#pragma db object pointer(std::shared_ptr)
class BlockHeader : public std::enable_shared_from_this<BlockHeader> {
public:
    std::shared_ptr<BlockHeader> get_shared_ptr() { return shared_from_this(); }
    BlockHeader() {}
    BlockHeader(const ChainHeader &header);

    void onMainChain(bool on_main_chain) { isOnMainChain = on_main_chain; }
    std::string id() { return id_; }

private:
    friend class odb::access;

    #pragma db id
    std::string id_;

    #pragma db index unique not_null readonly
    block_header_t header_;

    uint32_t transactionCount;
    #pragma db readonly
    std::string totalProofOfWork;//hex convert from BigInt
    //to compute tx confirmations (provided we get the current chain's most recent block's height)
    #pragma db readonly
    uint32_t height_;

    //updated by SPV client on re-org
    bool isOnMainChain;
};

#pragma db value
struct tx_block_t {
    #pragma db not_null
    std::shared_ptr<Transaction> tx_;
    #pragma db not_null
    std::shared_ptr<BlockHeader> block_;
};

#pragma db object pointer(std::shared_ptr)
class TransactionMinedInBlock : public std::enable_shared_from_this<TransactionMinedInBlock> {
public:
    std::shared_ptr<TransactionMinedInBlock> get_shared_ptr() { return shared_from_this(); }
    TransactionMinedInBlock() {}
    TransactionMinedInBlock(std::shared_ptr<Transaction> tx, std::shared_ptr<BlockHeader> header);
    std::string txid() const { return tx_block_.tx_->txid(); }

private:
    friend class odb::access;

    #pragma db id auto
    uint32_t id_;

    #pragma db index unique
    tx_block_t tx_block_;

    std::string merkelBranch_;
    uint32_t index_;
};

#ifdef USE_STORE_ALPHA_CODE
#pragma db object pointer(std::shared_ptr)
class Payer : public std::enable_shared_from_this<Payer> {
public:
    std::shared_ptr<Payer> get_shared_ptr() { return shared_from_this(); }
    Payer() {}
    Payer(std::shared_ptr<Transaction> tx);

private:
    friend class odb::access;

    #pragma db id auto
    uint32_t id_;

    //should it be null until payment channel is closed? or will it be continuously updated
    //as payments are made?
    std::shared_ptr<Transaction> contract_;
    uint32_t fee_;
    uint32_t created_;
    uint32_t state_;
    std::string desciption_;
};

#pragma db value
struct output_payer_t {
    std::shared_ptr<Output> output_;
    std::shared_ptr<Payer> payer_;
};

#pragma db object pointer(std::shared_ptr)
class OutputFundsPayer : public std::enable_shared_from_this<OutputFundsPayer> {
public:
    std::shared_ptr<OutputFundsPayer> get_shared_ptr() { return shared_from_this(); }
    OutputFundsPayer() {}
    OutputFundsPayer(std::shared_ptr<Output> output, std::shared_ptr<Payer> payer);

private:
    friend class odb::access;
    #pragma db id auto
    uint32_t id_;

    #pragma db index unique not_null
    output_payer_t output_payer_;
};

#pragma db value
struct payer_index_t {
    std::shared_ptr<Payer> payer;
    uint32_t index;
};

#pragma db object pointer(std::shared_ptr)
class Slot : public std::enable_shared_from_this<Slot> {
public:
    std::shared_ptr<Slot> get_shared_ptr() { return shared_from_this(); }
    Slot() {}

private:
    friend class odb::access;
    #pragma db id auto
    uint32_t id_;

    #pragma db index unique not_null
    payer_index_t payer_index_;

    uint32_t state_;
    uint32_t price_;
    uint32_t numberOfPaymentsMade_;
    uint32_t funds_;
    std::shared_ptr<Key> payerContractKey_;
    std::shared_ptr<Key> payerFinalKey_;
    std::string payeeContractPublicKey_;
    std::string payeeFinalPublicKey_;
    std::string refundSignature_;
    std::string lastPaymentSignature_;
    uint32_t refundFee_;
    uint32_t paymentFee_;
    std::shared_ptr<Transaction> refundTransaction_;// ?
    uint32_t refundLockTime_;
};

#pragma db object pointer(std::shared_ptr)
class Payee : public std::enable_shared_from_this<Payee> {
public:
    std::shared_ptr<Payee> get_shared_ptr() { return shared_from_this(); }
    Payee() {}

private:
    friend class odb::access;
    #pragma db id auto
    uint32_t id_;

    uint32_t state_;
    uint32_t numberOfPaymentsMade_;
    std::string lastValidPayerPaymentSignature_;
    uint32_t price_;
    uint32_t funds_;
    uint32_t maximumNumberOfSellers_;
    std::shared_ptr<Key> payeePaymentKey_;
    std::string contractId_;
    uint32_t contractOutput_;
    std::string payerContractPublicKey_;
    std::string payerFinalPublicKey_;
    std::shared_ptr<Transaction> paymentTransaction_;
};
#endif //store alpha code

#pragma db view object(Output) \
    object(Address) \
    object(TxHasOutput) \
    object(Transaction = output_tx) \
    object(TransactionMinedInBlock = output_tx_mined ) \
    object(BlockHeader = output_block ) \
    object(Input : TxHasOutput::tx_ix_.index_ == Input::id_.op_index_ && \
                   TxHasOutput::tx_ix_.tx ==  Input::id_.op_txid_ ) \
    object(TxHasInput: TxHasInput::input_) \
    object(Transaction = spending_tx : TxHasInput::tx_ix_.tx) \
    object(TransactionMinedInBlock = spending_tx_mined : spending_tx_mined::tx_block_.tx_ == spending_tx::txid_ ) \
    object(BlockHeader = spending_block : spending_tx_mined::tx_block_.block_ )
typedef struct {

    std::shared_ptr<Output> output;
    uint64_t value() { return output->value(); }

} outputs_view_t;

//all blocks a transaction is mined in / query by transaction = tx_in_block::tx_block_.tx_
#pragma db view object(TransactionMinedInBlock = tx_in_block) \
    object(BlockHeader : tx_in_block::tx_block_.block_)
typedef struct {
    std::shared_ptr<BlockHeader> header;
} block_headers_view_t;


}//store
}//detail
}//bitcoin
}//joystream

#endif
