/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef PLUGIN_INSTALLED
#define PLUGIN_INSTALLED

/**
 * @brief Enumeration of installable plugins
 */
enum class PluginInstalled {
    None,
    Observer,
    Buyer,
    Seller
};

#endif // PLUGIN_INSTALLED
