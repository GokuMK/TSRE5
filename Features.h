#ifndef FEATURES_H
#define	FEATURES_H

#include <unordered_map>
#include <vector>

class Features {
public:
    static std::unordered_map<std::string, int> LIST;
    static std::vector<int> LAYER;
    Features();
    virtual ~Features();
private:

};

#endif	/* FEATURES_H */

