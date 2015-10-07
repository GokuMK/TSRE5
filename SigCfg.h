#ifndef SIGCFG_H
#define	SIGCFG_H

#include <QString>
#include <unordered_map>

class SignalShape;
class SignalType;

class SigCfg {
public:
    std::unordered_map<std::string, SignalShape*> signalShape;
    std::unordered_map<std::string, SignalType*> signalType;
    SigCfg();
    virtual ~SigCfg();
private:

};

#endif	/* SIGCFG_H */

