#include "Contract.hpp"
#include "extension/BitCoin/PrivateKey.hpp"
#include "extension/BitCoin/BitSwaprjs.hpp"

#include <QJsonObject>

Contract::Contract() {
}

Contract::Contract(const Contract& contract)
    : _fundingOutput(contract.fundingOutput())
    , _p2shTxOuts(contract.p2shTxOuts())
    , _change(cntract.refund()) {
}

Contract & Contract::operator=(const Contract& contract) {
    _fundingOutput = contract.fundingOutput();
    _p2shTxOuts = contract.p2shTxOuts();
    _change = contract.refund();
}

Contract::Contract(const OutputPoint &fundingOutput, quint32 numberOfMultisigOutputs, const P2PKHTxOut &change)
    : _fundingOutput(fundingOutput)
    , _p2shTxOuts(numberOfMultisigOutputs)
    , _change(change){
}
/*
void Contract::sign(const PrivateKey & sk) {

}
*/
QJsonObject Contract::json() const {

    QJsonObject fundingOutput {
        {"hash", _fundingOutput.hash().toString()},
        {"index", _fundingOutput.index()},
    };

    QJsonArray p2shTxOuts;
    for(QVector<P2SHTxOut>::iterator i = _p2shTxOuts,
            end(_p2shTxOuts.end()); i != end;i++) {

        // Get output
        P2SHTxOut output = *i;

        p2shTxOuts.append(QJsonObject {
                              {"value", output.value()},
                              {"firstPk", output.firstPk().toString()},
                              {"secondPk", output.secondPk().toString()}
                          });
    }

    QJsonObject change {
        {"value", _change.value()},
        {"pk", _change.pk().toString()},
    };

    return QJsonObject {
        {"fundingOutput", fundingOutput},
        {"p2shTxOuts", p2shTxOuts},
        {"change", change}
    };
}

OutputPoint Contract::fundingOutput() const {
    return _fundingOutput;
}

void Contract::setFundingOutput(const OutputPoint &fundingOutput) {
    _fundingOutput = fundingOutput;
}

QVector<P2SHTxOut> Contract::p2shTxOuts() const {
    return _p2shTxOuts;
}

void Contract::setP2shTxOuts(const QVector<P2SHTxOut> &p2shTxOuts) {
    _p2shTxOuts = p2shTxOuts;
}

P2PKHTxOut Contract::change() const {
    return _change;
}

void Contract::setChange(const P2PKHTxOut &change) {
    _change = change;
}

void Contract::setOutput(quint32 index, const P2SHTxOut& output) {

    //if(index < _p2shTxOuts.size())
        _p2shTxOuts[index] = output;
    //else
    //    throw std::exception("invalid");
}
