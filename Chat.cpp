//
// Created by xAbdoMo on 4/14/2024.
//

#include <iostream>
#include <cmath>
#include <string>
#include <chrono>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <iomanip>

#define PORT 1474

static char buff[1024 * 64];
static char rBuff[1024 * 64];

#include "Chat.h"
#include "UI.h"
#include "sec/aes256.hpp"
#include "sec/SHA256.h"


struct DaffieHellman{
    SigType Ya;
    SigType Xa;
} DHKeys;

struct Remote{
    SigType Ya1;
    SigType Ya2;
} RemoteData;

struct PublicData {
    BinaryInt Q1 , A1 , Q2 , A2;
} publicData;

static ElGamalSig signature;
static SHA256* sha;
static ByteArray key;


static SOCKET serverSocket = INVALID_SOCKET;
static SOCKET connectionSocket = INVALID_SOCKET;

static int state = 0; //0 == disconnected, 1 == connecting / server active, 2 == connected
static bool mIsServer = false;
static bool mTerminate = false;
static std::thread mThread;

static bool init_server() {
    using namespace std;

    cout << "Creating server socket .." << endl;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        cout << "Failed to create server socket : " << WSAGetLastError() << endl;
        return false;
    }
    cout << "server socket created" << endl;


    cout << "binding server socket .." << endl;
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);
    if (bind(serverSocket, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        cout << "Failed to bind server socket : " << WSAGetLastError() << endl;
        return false;
    }
    cout << "server socket bound" << endl;


    cout << "starting listening .." << endl;
    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        cout << "Failed to listen : " << WSAGetLastError() << endl;
        return false;
    }
    cout << "listening started " << endl;
    UI::info("waiting for connection");

    cout << "waiting for client on \"localhost:" << PORT << "\"" << endl;
    connectionSocket = accept(serverSocket, nullptr, nullptr);
    if (connectionSocket == INVALID_SOCKET) {
        cout << "no client connected" << endl;
        return false;
    }

    cout << "socket connected !" << endl;
    UI::info("connected!");

    return true;
}

static bool init_client() {
    using namespace std;
    cout << "Creating socket .." << endl;
    connectionSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (connectionSocket == INVALID_SOCKET) {
        cout << "Failed to create socket : " << WSAGetLastError() << endl;
        return false;
    }
    cout << "server socket created" << endl;

    UI::info("connecting ...");
    cout << "connecting socket .." << endl;
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);
    addr.sin_port = htons(PORT);
    if (connect(connectionSocket, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        cout << "Failed to connect socket : " << WSAGetLastError() << endl;
        return false;
    }

    UI::info("connected!");
    cout << "socket connected !" << endl;
    return true;
}

static bool init() {
    using namespace std;

    cout << "[Init sequance]" << endl;
    cout << "Loading Winsock .." << endl;

    WORD version = MAKEWORD(2, 2);
    WSADATA data;
    if (WSAStartup(version, &data)) {
        UI::error("Error loading the Winsock library");
        cout << "Error loading the Winsock library" << endl;
        return false;
    }

    UI::info("Winsock Loaded");
    cout << "Winsock Loaded" << endl;

    if (mIsServer) {
        if (!init_server()) return false;
    }
    else {
        if (!init_client()) return false;
    }
    return true;
}


void Chat::init(BinaryInt Q1, BinaryInt A1, BinaryInt Q2, BinaryInt A2) {
    signature.init(Q1 , A1);

    DHKeys.Xa = IntOp::generateRandInt(4) % (Q2 - 1);
    DHKeys.Ya = IntOp::powMod(A2 , DHKeys.Xa , Q2);

    publicData = {Q1 , A1 , Q2 , A2};
}

static bool sendPacket(const char* data, uint32_t size){
    memcpy(rBuff + sizeof(uint32_t) , data , size);
    memcpy(rBuff , &size , sizeof(uint32_t));
    if (send(connectionSocket, rBuff, size + sizeof(uint32_t) , 0) < 0) {
        std::cout << "Failed to send data" << std::endl;
        state = 0;
        return false;
    }
    return true;
}

static bool sendString(std::string str){
    sprintf_s(rBuff + sizeof(uint32_t) , 1024 * 60 , "%s" , str.c_str());
    uint32_t len = str.length();
    memcpy(rBuff , &len , sizeof(uint32_t));
    if (send(connectionSocket, rBuff, len + sizeof(uint32_t) , 0) < 0) {
        std::cout << "Failed to send data" << std::endl;
        state = 0;
        return false;
    }
    return true;
}

static int readPacket(){
    int c = recv(connectionSocket, rBuff, sizeof(uint32_t), 0);
    if (c < sizeof(uint32_t)){ //it should be at least sizeof(uint32_t) bytes
        return -1;
    }

    uint32_t size = 0;
    memcpy(&size , rBuff , sizeof(uint32_t));
    uint32_t offset = 0;
    while (size - offset){
        c = recv(connectionSocket , rBuff + offset , (int) (size - offset) , 0);
        if (c < 0){
            return -1;
        }
        offset += c;
    }

    return (int) size;
}

static bool readString(std::string& str){
    int size = readPacket();
    if (size < 0){
        return false;
    }
    rBuff[size] = '\0';
    str = rBuff;
    return true;
}

static void connection_loop(){
    using namespace std;

    UI::info("Starting Key Exchange");

    //send keys
    auto ElGamalPublic = signature.getPublicKey().toString(16);
    auto DHPublic = DHKeys.Ya.toString(16);
    if (!sendString(ElGamalPublic)){
        UI::error("Failed to send ElGamal public key");
        return;
    }
    if (!sendString(DHPublic)){
        UI::error("Failed to send DH public key");
        return;
    }

    auto Sig = signature.getSignature(DHPublic);
    if (!sendString(Sig.first.toString(16)) || !sendString(Sig.second.toString(16))){
        UI::error("Failed to send signature public key");
        return;
    }


    //receive keys
    string temp1 , temp2;
    if (!readString(temp1)){
        UI::error("Failed to receive ElGamal public key");
        return;
    }
    if (!readString(temp2)){
        UI::error("Failed to receive DH public key");
        return;
    }

    string message = temp2;

    RemoteData.Ya1 = BinaryInt(temp1.c_str() , 16);
    RemoteData.Ya2 = BinaryInt(temp2.c_str() , 16);

    sprintf_s(buff , "Received remote keys { EL-Gamal: %s, DH: %s }" , temp1.c_str() , temp2.c_str());
    UI::info(buff);

    if (!readString(temp1)){
        UI::error("Failed to receive signature");
        return;
    }
    if (!readString(temp2)){
        UI::error("Failed to receive signature");
        return;
    }
    sprintf_s(buff , "Validating Signature { S1: %s, S2: %s }" , temp1.c_str() , temp2.c_str());
    UI::info(buff);

    std::pair<BinaryInt , BinaryInt> rSig;
    rSig.first = BinaryInt(temp1.c_str() , 16);
    rSig.second = BinaryInt(temp2.c_str() , 16);

    if (!signature.verifySignature(rSig , RemoteData.Ya1 , message)){
        UI::error("Signature is invalid!");
        return;
    }

    UI::info("Signature Ok :)");

    sha = new SHA256();
    BinaryInt commonKey = RemoteData.Ya2;
    commonKey = IntOp::powMod(commonKey , DHKeys.Xa , publicData.Q2);
    sha->update(commonKey.toString(16));
    auto aes_key = sha->digest();
    std::string ui_message = "Common value : " + commonKey.toString(16) + ", Common Key: ";

    key.clear();

    for (int i = 0;i < 4;i++){
        std::stringstream line;
        line<< "\n\t\t";
        for (int j = 0;j < 8;j++){
            line << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(aes_key[i * 4 + j]) << "  ";
            key.push_back(aes_key[i * 4 + j]);
        }
        ui_message += line.str();
    }

    UI::info(ui_message.c_str());

    ByteArray cypher;
    ByteArray plain;

    UI::info("Connection Ready !");

    while (!mTerminate){
        int size = readPacket();
        if (size < 0){
            break;
        }

        cypher.clear();
        plain.clear();

        std::cout << "RECV_SIZE: " << size << std::endl;

        for (int i = 0;i < size;i++){
            cypher.push_back(rBuff[i]);
        }

        Aes256::decrypt(key , cypher , plain);

        for (int i = 0;i < plain.size();i++){
            buff[i] = (char) plain[i];
        }

        buff[plain.size()] = '\0';

        UI::message(buff);
    }
}

static void connection_launcher(){
    state = 1;
    if (!init()){
        state = 0;
        return;
    }

    state = 2;
    connection_loop();

    if (serverSocket != INVALID_SOCKET) {
        closesocket(serverSocket);
        serverSocket = INVALID_SOCKET;
    }
    if (connectionSocket != INVALID_SOCKET) {
        closesocket(connectionSocket);
        connectionSocket = INVALID_SOCKET;
    }
    state = 0;

}

void Chat::startConnection(bool server) {
    if (state == 0){
        mIsServer = server;
        mTerminate = false;
        std::cout << "Starting Connection Sequence" << std::endl;
        mThread = std::thread(connection_launcher);
    }
}

void Chat::disconnect() {

    mTerminate = true;
    if (serverSocket != INVALID_SOCKET) {
        closesocket(serverSocket);
        serverSocket = INVALID_SOCKET;
    }

    if (connectionSocket != INVALID_SOCKET) {
        closesocket(connectionSocket);
        connectionSocket = INVALID_SOCKET;
    }
}

void Chat::waitForChat() {
    if (mThread.joinable()){
        mThread.join();
    }
}

void Chat::send(const char * str) {
    size_t size = strlen(str);
    ByteArray cypher;
    ByteArray plain;

    cypher.clear();
    plain.clear();
    for (int i = 0;i < size;i++){
        plain.push_back(str[i]);
    }

    Aes256::encrypt(key , plain , cypher);

    for (int i = 0;i < cypher.size();i++){
        buff[i] = (char) cypher[i];
    }

    if (!sendPacket(buff , cypher.size())){
        UI::error("Failed to send message");
        disconnect();
    }

    std::cout << "SEND_SIZE: " << size << std::endl;


}

const ElGamalSig& Chat::getSig() {
    return signature;
}

std::pair<SigType, SigType> Chat::getDaffieHellman() {
    return {DHKeys.Ya,DHKeys.Xa};
}

bool Chat::isConnected() {
    return state == 2;
}

SigType Chat::getExternalDHYa() {
    return RemoteData.Ya2;
}

SigType Chat::getEternalElGamalYa() {
    return RemoteData.Ya1;
}

bool Chat::isConnecting() {
    return state == 1;
}




