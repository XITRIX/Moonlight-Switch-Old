#include <stdio.h>
#pragma once

#if defined(__SWITCH__)

#include "MbedTLSCryptoManager.hpp"
#define CryptoManager MbedTLSCryptoManager

#elif defined(USE_OPENSSL_CRYPTO)

#include "OpenSSLCryptoManager.hpp"
#define CryptoManager OpenSSLCryptoManager

#else
#error Select crypto!
#endif
