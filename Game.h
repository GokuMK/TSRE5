#ifndef GAMESETTINGS_H
#define	GAMESETTINGS_H

#include <QString>

class Game {
public:
    static QString root;
    static QString route;
    static float objectLod;
    static int allowObjLag;
    static int maxObjLag;
    Game();
    Game(const Game& orig);
    virtual ~Game();
private:
};

#endif	/* GAMESETTINGS_H */

