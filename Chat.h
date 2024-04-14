//
// Created by xAbdoMo on 4/14/2024.
//

#ifndef SECURECHAT_CHAT_H
#define SECURECHAT_CHAT_H

#include "util/BinaryInt.h"
#include "util/BigInt.h"
#include "sec/ElGamalSignature.h"

namespace Chat {
    void init(SigType Q1,SigType A1,SigType Q2,SigType A2);
    void startConnection(bool);
    void disconnect();
    void waitForChat();
    void send(const char*);
    const ElGamalSig& getSig();
    std::pair<SigType,SigType> getDaffieHellman();
    bool isConnected();
    bool isConnecting();
    SigType getExternalDHYa();
    SigType getEternalElGamalYa();
};


#endif //SECURECHAT_CHAT_H
