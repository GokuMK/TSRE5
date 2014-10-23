#ifndef VECTOR2I_H
#define	VECTOR2I_H

class Vector2i {
public:
    int x;
    int z;
    Vector2i();
    Vector2i(int x, int z);
    Vector2i(const Vector2i& orig);
    virtual ~Vector2i();
    bool equals(Vector2i other);
    int hashCode();
private:
    
};

#endif	/* VECTOR2I_H */

