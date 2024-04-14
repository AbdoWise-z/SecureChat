//
// Created by xAbdoMo on 4/13/2024.
//

#ifndef SECURITY_PROJECT_ELGAMALSIGNATURE_H
#define SECURITY_PROJECT_ELGAMALSIGNATURE_H

#ifndef SigType
#define SigType BinaryInt
#define RandomSize 6
#endif

class ElGamalSig{
private:
    SigType mQ;
    SigType mA;
    SigType mYa;
    SigType mXa;
    SigType mK;
    SigType mKInverse;
public:
    ElGamalSig();
    void init(SigType q, SigType a);
    [[nodiscard]] SigType getRandomK() const;
    [[nodiscard]] SigType getRandomKInv() const;
    [[nodiscard]] SigType getPublicKey() const ;
    [[nodiscard]] SigType getPrivateKey() const ;

    [[nodiscard]] std::pair<SigType , SigType> getSignature(const std::string& message) const ;
    [[nodiscard]] bool verifySignature(std::pair<SigType , SigType>, SigType Ya , const std::string& message) const;
};

#endif //SECURITY_PROJECT_ELGAMALSIGNATURE_H
