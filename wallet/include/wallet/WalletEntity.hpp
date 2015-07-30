#ifndef WALLET_ENTITY_HPP
#define WALLET_ENTITY_HPP

#include <QSqlQuery>

// Common interface for all wallet entities in wallet database
class WalletEntity
{
public:

    // Query which creates table corresponding to entity
    virtual QSqlQuery createTableQuery() = 0;

    // (Unbound) Query which inserts record in table
    virtual QSqlQuery unboundInsertQuery() = 0;


private:

};

#endif // WALLET_ENTITY_HPP
