/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef OSMFEATURES_H
#define	OSMFEATURES_H

#include <unordered_map>
#include <vector>

class OSMFeatures {
public:
    static std::unordered_map<std::string, int> LIST;
    static std::vector<int> LAYER;
    OSMFeatures();
    virtual ~OSMFeatures();
private:

};

#endif	/* OSMFEATURES_H */

