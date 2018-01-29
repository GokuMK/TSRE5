/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  This file is c++ conversion of glMatrix:
 *  Javascript Matrix and Vector library for High Performance WebGL apps.
 *  Copyright (c) 2015, Brandon Jones, Colin MacKenzie IV.
 * 
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "GLMatrix.h"
#include <math.h>
#include <iostream>

/**
 * Creates a new, empty vec2
 * @returns {vec2} a new 2D vector
 */
float* Vec2::create() {
    float * out = new float[2];
    out[0] = 0;
    out[1] = 0;
    return out; 
};
/**
 * Creates a new vec2 initialized with values from an existing vector
 * @param {vec2} a vector to clone
 * @returns {vec2} a new 2D vector
 */
float* Vec2::clone(float* a) {
    float * out = new float[2];
    out[0] = a[0];
    out[1] = a[1];
    return out;
};
/**
 * Creates a new vec2 initialized with the given values
 * @param {Number} x X component
 * @param {Number} y Y component
 * @returns {vec2} a new 2D vector
 */
float* Vec2::fromValues(float x, float y) {
    float* out = new float[2];
    out[0] = x;
    out[1] = y;
    return out;
};
/**
 * Copy the values from one vec2 to another
 * @param {vec2} out the receiving vector
 * @param {vec2} a the source vector
 * @returns {vec2} out
 */
float* Vec2::copy(float* out, float* a) {
    out[0] = a[0];
    out[1] = a[1];
    return out;
};
/**
 * Set the components of a vec2 to the given values
 * @param {vec2} out the receiving vector
 * @param {Number} x X component
 * @param {Number} y Y component
 * @returns {vec2} out
 */
float* Vec2::set(float* out, float x, float y) {
    out[0] = x;
    out[1] = y;
    return out;
};
/**
 * Adds two vec2's
 * @param {vec2} out the receiving vector
 * @param {vec2} a the first operand
 * @param {vec2} b the second operand
 * @returns {vec2} out
 */
float* Vec2::add(float *out, float *a, float *b) {
    out[0] = a[0] + b[0];
    out[1] = a[1] + b[1];
    return out;
}
/**
 * Subtracts vector b from vector a
 * @param {vec2} out the receiving vector
 * @param {vec2} a the first operand
 * @param {vec2} b the second operand
 * @returns {vec2} out
 */
float* Vec2::subtract(float *out, float *a, float *b) {
    out[0] = a[0] - b[0];
    out[1] = a[1] - b[1];
    return out;
}
/**
 * Alias for {@link vec2.subtract}
 * @function
 */
float* Vec2::sub(float *out, float *a, float *b) {
    return subtract(out, a, b);
}
/**
 * Multiplies two vec2's
 * @param {vec2} out the receiving vector
 * @param {vec2} a the first operand
 * @param {vec2} b the second operand
 * @returns {vec2} out
 */
float* Vec2::multiply(float *out, float *a, float *b) {
    out[0] = a[0] * b[0];
    out[1] = a[1] * b[1];
    return out;
}
/**
 * Alias for {@link vec2.multiply}
 * @function
 */
float* Vec2::mul(float *out, float *a, float *b) {
    return multiply(out, a, b);
}
/**
 * Divides two vec2's
 * @param {vec2} out the receiving vector
 * @param {vec2} a the first operand
 * @param {vec2} b the second operand
 * @returns {vec2} out
 */
float* Vec2::divide(float *out, float *a, float *b) {
    out[0] = a[0] / b[0];
    out[1] = a[1] / b[1];
    return out;
}
/**
 * Alias for {@link vec2.divide}
 * @function
 */
float* Vec2::div(float *out, float *a, float *b) {
    return divide(out, a, b);
}
/**
 * Returns the minimum of two vec2's
 * @param {vec2} out the receiving vector
 * @param {vec2} a the first operand
 * @param {vec2} b the second operand
 * @returns {vec2} out
 */
float* Vec2::min(float *out, float *a, float *b) {
    out[0] = fmin(a[0], b[0]);
    out[1] = fmin(a[1], b[1]);
    return out;
}
/**
 * Returns the maximum of two vec2's
 * @param {vec2} out the receiving vector
 * @param {vec2} a the first operand
 * @param {vec2} b the second operand
 * @returns {vec2} out
 */
float* Vec2::max(float *out, float *a, float *b) {
    out[0] = fmax(a[0], b[0]);
    out[1] = fmax(a[1], b[1]);
    return out;
}
/**
 * Scales a vec2 by a scalar number
 * @param {vec2} out the receiving vector
 * @param {vec2} a the vector to scale
 * @param {Number} b amount to scale the vector by
 * @returns {vec2} out
 */
float* Vec2::scale(float *out, float *a, float b) {
    out[0] = a[0] * b;
    out[1] = a[1] * b;
    return out;
}
/**
 * Adds two vec2's after scaling the second operand by a scalar value
 * @param {vec2} out the receiving vector
 * @param {vec2} a the first operand
 * @param {vec2} b the second operand
 * @param {Number} scale the amount to scale b by before adding
 * @returns {vec2} out
 */
float* Vec2::scaleAndAdd(float *out, float *a, float *b, float scale) {
    out[0] = a[0] + (b[0] * scale);
    out[1] = a[1] + (b[1] * scale);
    return out;
}
/**
 * Calculates the euclidian distance between two vec2's
 * @param {vec2} a the first operand
 * @param {vec2} b the second operand
 * @returns {Number} distance between a and b
 */
float Vec2::distance(float *a, float *b) {
    float x = b[0] - a[0],
        y = b[1] - a[1];
    return sqrt(x*x + y*y);
}
/**
 * Alias for {@link vec2.distance}
 * @function
 */
float Vec2::dist(float *a, float *b) {
    return distance(a, b);
}
/**
 * Calculates the squared euclidian distance between two vec2's
 * @param {vec2} a the first operand
 * @param {vec2} b the second operand
 * @returns {Number} squared distance between a and b
 */
float Vec2::squaredDistance(float *a, float *b) {
    float x = b[0] - a[0],
        y = b[1] - a[1];
    return x*x + y*y;
}
/**
 * Alias for {@link vec2.squaredDistance}
 * @function
 */
float Vec2::sqrDist(float *a, float *b) {
    return squaredDistance(a, b);
}
/**
 * Calculates the length of a vec2
 * @param {vec2} a vector to calculate length of
 * @returns {Number} length of a
 */
float Vec2::length(float *a) {
    float x = a[0],
        y = a[1];
    return sqrt(x*x + y*y);
}
/**
 * Alias for {@link vec2.length}
 * @function
 */
float Vec2::len(float *a) {
    return length(a);
}
/**
 * Calculates the squared length of a vec2
 * @param {vec2} a vector to calculate squared length of
 * @returns {Number} squared length of a
 */
float Vec2::squaredLength(float *a) {
    float x = a[0],
        y = a[1];
    return x*x + y*y;
}
/**
 * Alias for {@link vec2.squaredLength}
 * @function
 */
float Vec2::sqrLen(float *a) {
    return squaredLength(a);
}
/**
 * Negates the components of a vec2
 * @param {vec2} out the receiving vector
 * @param {vec2} a vector to negate
 * @returns {vec2} out
 */
float* Vec2::negate(float *out, float *a) {
    out[0] = -a[0];
    out[1] = -a[1];
    return out;
}
/**
 * Normalize a vec2
 * @param {vec2} out the receiving vector
 * @param {vec2} a vector to normalize
 * @returns {vec2} out
 */
float* Vec2::normalize(float *out, float *a) {
    float x = a[0],
        y = a[1];
    float len = x*x + y*y;
    if (len > 0) {
        //TODO: evaluate use of glm_invsqrt here?
        len = 1.0 / sqrt(len);
        out[0] = a[0] * len;
        out[1] = a[1] * len;
    }
    return out;
}
/**
 * Calculates the dot product of two vec2's
 * @param {vec2} a the first operand
 * @param {vec2} b the second operand
 * @returns {Number} dot product of a and b
 */
float Vec2::dot(float *a, float *b) {
    return a[0] * b[0] + a[1] * b[1];
}
/**
 * Computes the cross product of two vec2's
 * Note that the cross product must by definition produce a 3D vector
 * @param {vec3} out the receiving vector
 * @param {vec2} a the first operand
 * @param {vec2} b the second operand
 * @returns {vec3} out
 */
float* Vec2::cross(float *out, float *a, float *b) {
    float z = a[0] * b[1] - a[1] * b[0];
    out[0] = out[1] = 0;
    out[2] = z;
    return out;
}
/**
 * Performs a linear interpolation between two vec2's
 * @param {vec2} out the receiving vector
 * @param {vec2} a the first operand
 * @param {vec2} b the second operand
 * @param {Number} t interpolation amount between the two inputs
 * @returns {vec2} out
 */
float* Vec2::lerp(float *out, float *a, float *b, float t) {
    float ax = a[0],
        ay = a[1];
    out[0] = ax + t * (b[0] - ax);
    out[1] = ay + t * (b[1] - ay);
    return out;
}
/**
 * Transforms the vec2 with a mat2
 * @param {vec2} out the receiving vector
 * @param {vec2} a the vector to transform
 * @param {mat2} m matrix to transform with
 * @returns {vec2} out
 */
float* Vec2::transformMat2(float *out, float *a, float *m) {
    float x = a[0],
        y = a[1];
    out[0] = m[0] * x + m[2] * y;
    out[1] = m[1] * x + m[3] * y;
    return out;
}
/**
 * Transforms the vec2 with a mat2d
 * @param {vec2} out the receiving vector
 * @param {vec2} a the vector to transform
 * @param {mat2d} m matrix to transform with
 * @returns {vec2} out
 */
float* Vec2::transformMat2d(float *out, float *a, float *m) {
    float x = a[0],
        y = a[1];
    out[0] = m[0] * x + m[2] * y + m[4];
    out[1] = m[1] * x + m[3] * y + m[5];
    return out;
}
/**
 * Transforms the vec2 with a mat3
 * 3rd vector component is implicitly '1'
 * @param {vec2} out the receiving vector
 * @param {vec2} a the vector to transform
 * @param {mat3} m matrix to transform with
 * @returns {vec2} out
 */
float* Vec2::transformMat3(float *out, float *a, float *m) {
    float x = a[0],
        y = a[1];
    out[0] = m[0] * x + m[3] * y + m[6];
    out[1] = m[1] * x + m[4] * y + m[7];
    return out;
}
/**
 * Transforms the vec2 with a mat4
 * 3rd vector component is implicitly '0'
 * 4th vector component is implicitly '1'
 * @param {vec2} out the receiving vector
 * @param {vec2} a the vector to transform
 * @param {mat4} m matrix to transform with
 * @returns {vec2} out
 */
float* Vec2::transformMat4(float *out, float *a, float *m) {
    float x = a[0], 
        y = a[1];
    out[0] = m[0] * x + m[4] * y + m[12];
    out[1] = m[1] * x + m[5] * y + m[13];
    return out;
}

/**
 * Creates a new, empty vec3
 * @returns {vec3} a new 3D vector
 */
float* Vec3::create() {
    float* out = new float[3];
    out[0] = 0;
    out[1] = 0;
    out[2] = 0;
    return out;
}
/**
 * Creates a new vec3 initialized with values from an existing vector
 * @param {vec3} a vector to clone
 * @returns {vec3} a new 3D vector
 */
float* Vec3::clone(float* a) {
    float* out = new float[3];
    out[0] = a[0];
    out[1] = a[1];
    out[2] = a[2];
    return out;
}
/**
 * Creates a new vec3 initialized with the given values
 * @param {Number} x X component
 * @param {Number} y Y component
 * @param {Number} z Z component
 * @returns {vec3} a new 3D vector
 */
float* Vec3::fromValues(float x, float y, float z) {
    float* out = new float[3];
    out[0] = x;
    out[1] = y;
    out[2] = z;
    return out;
}
/**
 * Copy the values from one vec3 to another
 * @param {vec3} out the receiving vector
 * @param {vec3} a the source vector
 * @returns {vec3} out
 */
float* Vec3::copy(float* out, float* a) {
    out[0] = a[0];
    out[1] = a[1];
    out[2] = a[2];
    return out;
}
/**
 * Set the components of a vec3 to the given values
 * @param {vec3} out the receiving vector
 * @param {Number} x X component
 * @param {Number} y Y component
 * @param {Number} z Z component
 * @returns {vec3} out
 */
float* Vec3::set(float* out, float x, float y, float z) {
    out[0] = x;
    out[1] = y;
    out[2] = z;
    return out;
}
/*
 * Adds two vec3's
 * @param {vec3} out the receiving vector
 * @param {vec3} a the first operand
 * @param {vec3} b the second operand
 * @returns {vec3} out
 */
float* Vec3::add(float* out, float* a, float* b) {
    out[0] = a[0] + b[0];
    out[1] = a[1] + b[1];
    out[2] = a[2] + b[2];
    return out;
}

float* Vec3::add(float* out, float* a, float b) {
    out[0] = a[0] + b;
    out[1] = a[1] + b;
    out[2] = a[2] + b;
    return out;
}
/**
 * Subtracts vector b from vector a
 * @param {vec3} out the receiving vector
 * @param {vec3} a the first operand
 * @param {vec3} b the second operand
 * @returns {vec3} out
 */
float* Vec3::subtract(float* out, float* a, float* b) {
    out[0] = a[0] - b[0];
    out[1] = a[1] - b[1];
    out[2] = a[2] - b[2];
    return out;
}
/**
 * Alias for {@link float* Vec3::subtract}
 * @function
 */
float* Vec3::sub(float* out, float* a, float* b) {
    return Vec3::subtract( out, a, b);
}
/**
 * Multiplies two vec3's
 * @param {vec3} out the receiving vector
 * @param {vec3} a the first operand
 * @param {vec3} b the second operand
 * @returns {vec3} out
 */
float* Vec3::multiply(float* out, float* a, float* b) {
    out[0] = a[0] * b[0];
    out[1] = a[1] * b[1];
    out[2] = a[2] * b[2];
    return out;
}
/**
 * Alias for {@link float* Vec3::multiply}
 * @function
 */
float* Vec3::mul(float* out, float* a, float* b) {
    return Vec3::multiply( out, a, b);
}
/**
 * Divides two vec3's
 * @param {vec3} out the receiving vector
 * @param {vec3} a the first operand
 * @param {vec3} b the second operand
 * @returns {vec3} out
 */
float* Vec3::divide(float* out, float* a, float* b) {
    out[0] = a[0] / b[0];
    out[1] = a[1] / b[1];
    out[2] = a[2] / b[2];
    return out;
}
/**
 * Alias for {@link float* Vec3::divide}
 * @function
 */
float* Vec3::div(float* out, float* a, float* b) {
    return Vec3::divide( out, a, b);
}
/**
 * Returns the minimum of two vec3's
 * @param {vec3} out the receiving vector
 * @param {vec3} a the first operand
 * @param {vec3} b the second operand
 * @returns {vec3} out
 */
float* Vec3::min(float* out, float* a, float* b) {
    out[0] = fmin(a[0], b[0]);
    out[1] = fmin(a[1], b[1]);
    out[2] = fmin(a[2], b[2]);
    return out;
}
/**
 * Returns the maximum of two vec3's
 * @param {vec3} out the receiving vector
 * @param {vec3} a the first operand
 * @param {vec3} b the second operand
 * @returns {vec3} out
 */
float* Vec3::max(float* out, float* a, float* b) {
    out[0] = fmax(a[0], b[0]);
    out[1] = fmax(a[1], b[1]);
    out[2] = fmax(a[2], b[2]);
    return out;
}
/**
 * Scales a vec3 by a scalar number
 * @param {vec3} out the receiving vector
 * @param {vec3} a the vector to scale
 * @param {Number} b amount to scale the vector by
 * @returns {vec3} out
 */
float* Vec3::scale(float* out, float* a, float b) {
    out[0] = a[0] * b;
    out[1] = a[1] * b;
    out[2] = a[2] * b;
    return out;
}
/**
 * Adds two vec3's after scaling the second operand by a scalar value
 * @param {vec3} out the receiving vector
 * @param {vec3} a the first operand
 * @param {vec3} b the second operand
 * @param {Number} scale the amount to scale b by before adding
 * @returns {vec3} out
 */
float* Vec3::scaleAndAdd(float* out, float* a, float* b, float scale) {
    out[0] = a[0] + (b[0] * scale);
    out[1] = a[1] + (b[1] * scale);
    out[2] = a[2] + (b[2] * scale);
    return out;
}
/**
 * Calculates the euclidian distance between two vec3's
 * @param {vec3} a the first operand
 * @param {vec3} b the second operand
 * @returns {Number} distance between a and b
 */
float Vec3::distance(float* a, float* b) {
    float x = b[0] - a[0],
        y = b[1] - a[1],
        z = b[2] - a[2];
    return sqrt(x*x + y*y + z*z);
}
/*
 * Alias for {@link float* Vec3::distance}
 * @function
 */
float Vec3::dist(float* a, float* b) {
    return Vec3::distance(a, b);
}

/**
 * Calculates the squared euclidian distance between two vec3's
 * @param {vec3} a the first operand
 * @param {vec3} b the second operand
 * @returns {Number} squared distance between a and b
 */
float Vec3::squaredDistance(float* a, float* b) {
    float x = b[0] - a[0],
        y = b[1] - a[1],
        z = b[2] - a[2];
    return x*x + y*y + z*z;
};
/**
 * Alias for {@link float* Vec3::squaredDistance}
 * @function
 */
float Vec3::sqrDist(float* a, float* b) {
    return Vec3::squaredDistance(a, b);
}
/**
 * Calculates the length of a vec3
 * @param {vec3} a vector to calculate length of
 * @returns {Number} length of a
 */
float Vec3::length (float* a) {
    float  x = a[0],
        y = a[1],
        z = a[2];
    return sqrt(x*x + y*y + z*z);
}
/**
 * Alias for {@link float* Vec3::length}
 * @function
 */
float Vec3::len (float* a) {
    return Vec3::length (a);
}
/**
 * Calculates the squared length of a vec3
 * @param {vec3} a vector to calculate squared length of
 * @returns {Number} squared length of a
 */
float Vec3::squaredLength(float* a) {
    float x = a[0],
        y = a[1],
        z = a[2];
    return x*x + y*y + z*z;
}
/**
 * Alias for {@link float* Vec3::squaredLength}
 * @function
 */
float Vec3::sqrLen(float* a) {
    return Vec3::squaredLength(a);
}
/**
 * Negates the components of a vec3
 * @param {vec3} out the receiving vector
 * @param {vec3} a vector to negate
 * @returns {vec3} out
 */
float* Vec3::negate(float* out, float* a) {
    out[0] = -a[0];
    out[1] = -a[1];
    out[2] = -a[2];
    return out;
}
/**
 * Normalize a vec3
 * @param {vec3} out the receiving vector
 * @param {vec3} a vector to normalize
 * @returns {vec3} out
 */
float* Vec3::normalize(float* out, float* a) {
    float x = a[0],
        y = a[1],
        z = a[2];
    float len = x*x + y*y + z*z;
    if (len > 0) {
        //TODO: evaluate use of glm_invsqrt here?
        len = 1.0 / sqrt(len);
        out[0] = a[0] * len;
        out[1] = a[1] * len;
        out[2] = a[2] * len;
    }
    return out;
}
/**
 * Calculates the dot product of two vec3's
 * @param {vec3} a the first operand
 * @param {vec3} b the second operand
 * @returns {Number} dot product of a and b
 */
float Vec3::dot (float* a, float* b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
/**
 * Computes the cross product of two vec3's
 * @param {vec3} out the receiving vector
 * @param {vec3} a the first operand
 * @param {vec3} b the second operand
 * @returns {vec3} out
 */
float* Vec3::cross(float* out, float* a, float* b) {
    float  ax = a[0], ay = a[1], az = a[2],
        bx = b[0], by = b[1], bz = b[2];

    out[0] = ay * bz - az * by;
    out[1] = az * bx - ax * bz;
    out[2] = ax * by - ay * bx;
    return out;
}
/**
 * Performs a linear interpolation between two vec3's
 * @param {vec3} out the receiving vector
 * @param {vec3} a the first operand
 * @param {vec3} b the second operand
 * @param {Number} t interpolation amount between the two inputs
 * @returns {vec3} out
 */
float* Vec3::lerp (float* out, float* a, float* b, float t) {
    float  ax = a[0],
        ay = a[1],
        az = a[2];
    out[0] = ax + t * (b[0] - ax);
    out[1] = ay + t * (b[1] - ay);
    out[2] = az + t * (b[2] - az);
    return out;
}
/**
 * Transforms the vec3 with a mat3.
 * @param {vec3} out the receiving vector
 * @param {vec3} a the vector to transform
 * @param {mat4} m the 3x3 matrix to transform with
 * @returns {vec3} out
 */
float* Vec3::transformMat3(float* out, float* a, float* m) {
    float  x = a[0], y = a[1], z = a[2];
    out[0] = x * m[0] + y * m[3] + z * m[6];
    out[1] = x * m[1] + y * m[4] + z * m[7];
    out[2] = x * m[2] + y * m[5] + z * m[8];
    return out;
}

float* Vec3::transformMat4(float* out, float* a, float* m) {
    float x = a[0], y = a[1], z = a[2];
    out[0] = m[0] * x + m[4] * y + m[8] * z + m[12];
    out[1] = m[1] * x + m[5] * y + m[9] * z + m[13];
    out[2] = m[2] * x + m[6] * y + m[10] * z + m[14];
    return out;
}

float* Vec3::transformQuat(float* out, float* a, float* q) {
    float x = a[0], y = a[1], z = a[2],
        qx = q[0], qy = q[1], qz = q[2], qw = q[3],

        // calculate quat * vec
        ix = qw * x + qy * z - qz * y,
        iy = qw * y + qz * x - qx * z,
        iz = qw * z + qx * y - qy * x,
        iw = -qx * x - qy * y - qz * z;

    // calculate result * inverse quat
    out[0] = ix * qw + iw * -qx + iy * -qz - iz * -qy;
    out[1] = iy * qw + iw * -qy + iz * -qx - ix * -qz;
    out[2] = iz * qw + iw * -qz + ix * -qy - iy * -qx;
    return out;
};

/**
 * Creates a new, empty vec4
 * @returns {vec4} a new 2D vector
 */
float* Vec4::create() {
    float * out = new float[4];
    out[0] = 0;
    out[1] = 0;
    out[0] = 0;
    out[1] = 0;
    return out; 
};
/**
 * Performs a linear interpolation between two vec4's
 *
 * @param {vec4} out the receiving vector
 * @param {vec4} a the first operand
 * @param {vec4} b the second operand
 * @param {Number} t interpolation amount between the two inputs
 * @returns {vec4} out
 */
float* Vec4::lerp(float *out, float *a, float *b, float t) {
    float ax = a[0],
        ay = a[1],
        az = a[2],
        aw = a[3];
    
    out[0] = ax + t * (b[0] - ax);
    out[1] = ay + t * (b[1] - ay);
    out[2] = az + t * (b[2] - az);
    out[3] = aw + t * (b[3] - aw);

    return out;
};

/**
 * Normalize a vec4
 *
 * @param {vec4} out the receiving vector
 * @param {vec4} a vector to normalize
 * @returns {vec4} out
 */
float* Vec4::normalize(float* out, float* a) {
    float x = a[0],
        y = a[1],
        z = a[2],
        w = a[3];
    float len = x*x + y*y + z*z + w*w;
    if (len > 0) {
        len = 1.0 / sqrt(len);
        out[0] = a[0] * len;
        out[1] = a[1] * len;
        out[2] = a[2] * len;
        out[3] = a[3] * len;
    }
    return out;
};

float* Vec4::copy(float* out, float* a) {
    out[0] = a[0];
    out[1] = a[1];
    out[2] = a[2];
    out[3] = a[3];
    return out; 
};
/**
 * Creates a new, empty quaternion
 * @returns {vec4} 
 */
float* Quat::create() {
    float * out = new float[4];
    out[0] = 0;
    out[1] = 0;
    out[2] = 0;
    out[3] = 1;
    return out; 
};

float* Quat::copy(float* out, float* a) {
    out[0] = a[0];
    out[1] = a[1];
    out[2] = a[2];
    out[3] = a[3];
    return out; 
};

float* Quat::fill(float* out) {
    out[0] = 0;
    out[1] = 0;
    out[2] = 0;
    out[3] = 1;
    return out; 
};

/**
 * Creates a quaternion from the given 3x3 rotation matrix.
 *
 * NOTE: The resultant quaternion is not normalized, so you should be sure
 * to renormalize the quaternion yourself where necessary.
 *
 * @param {quat} out the receiving quaternion
 * @param {mat3} m rotation matrix
 * @returns {quat} out
 * @function
 */
float* Quat::fromMat3(float *out, float *m) {
        // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
        // article "Quaternion Calculus and Fast Animation".
        float fTrace = m[0] + m[4] + m[8];
        float fRoot;
        float s_iNext[3]{1,2,0};

        if ( fTrace > 0.0 ) {
            // |w| > 1/2, may as well choose w > 1/2
            fRoot = sqrtf(fTrace + 1.0);  // 2w
            out[3] = 0.5 * fRoot;
            fRoot = 0.5/fRoot;  // 1/(4w)
            out[0] = (m[7]-m[5])*fRoot;
            out[1] = (m[2]-m[6])*fRoot;
            out[2] = (m[3]-m[1])*fRoot;
        } else {
            // |w| <= 1/2
            int i = 0;
            if ( m[4] > m[0] )
              i = 1;
            if ( m[8] > m[i*3+i] )
              i = 2;
            int j = s_iNext[i];
            int k = s_iNext[j];
            
            fRoot = sqrtf(m[i*3+i]-m[j*3+j]-m[k*3+k] + 1.0);
            out[i] = 0.5 * fRoot;
            fRoot = 0.5 / fRoot;
            out[3] = (m[k*3+j] - m[j*3+k]) * fRoot;
            out[j] = (m[j*3+i] + m[i*3+j]) * fRoot;
            out[k] = (m[k*3+i] + m[i*3+k]) * fRoot;
        }
        
        return out;
    };

float* Quat::fromRotationXYZ(float *out, float *a){
    float sx = (float) sin(a[0]/2); 
    float sy = (float) sin(a[1]/2); 
    float sz = (float) sin(a[2]/2);
    float cx = (float) cos(a[0]/2); 
    float cy = (float) cos(a[1]/2); 
    float cz = (float) cos(a[2]/2);

    out[0] = cx*cy*cz + sx*sy*sz;
    out[1] = sx*cy*cz - cx*sy*sz;
    out[2] = cx*sy*cz + sx*cy*sz;
    out[3] = cx*cy*sz - sx*sy*cz;

    return out;
}
/**
 * Calculates the inverse of a quat
 *
 * @param {quat} out the receiving quaternion
 * @param {quat} a quat to calculate inverse of
 * @returns {quat} out
 */
float* Quat::invert(float *out, float *a){
    float a0 = a[0], a1 = a[1], a2 = a[2], a3 = a[3],
        dot = a0*a0 + a1*a1 + a2*a2 + a3*a3,
        invDot = dot ? 1.0/dot : 0;
    // TODO: Would be faster to return [0,0,0,0] immediately if dot == 0
    out[0] = -a0*invDot;
    out[1] = -a1*invDot;
    out[2] = -a2*invDot;
    out[3] = a3*invDot;
    return out;
};

float* Quat::lerp(float* out, float* a, float* b, float t){
    return Vec4::lerp(out,a,b,t);
}
/**
 * Performs a spherical linear interpolation between two quat
 *
 * @param {quat} out the receiving quaternion
 * @param {quat} a the first operand
 * @param {quat} b the second operand
 * @param {Number} t interpolation amount between the two inputs
 * @returns {quat} out
 */
float* Quat::slerp(float* out, float* a, float* b, float t){

    float ax = a[0], ay = a[1], az = a[2], aw = a[3],
        bx = b[0], by = b[1], bz = b[2], bw = b[3];
    float        omega, cosom, sinom, scale0, scale1;
    // calc cosine
    cosom = ax * bx + ay * by + az * bz + aw * bw;
    // adjust signs (if necessary)
    if ( cosom < 0.0 ) {
        cosom = -cosom;
        bx = - bx;
        by = - by;
        bz = - bz;
        bw = - bw;
    }
    // calculate coefficients
    if ( (1.0 - cosom) > 0.000001 ) {
        // standard case (slerp)
        omega  = acos(cosom);
        sinom  = sin(omega);
        scale0 = sin((1.0 - t) * omega) / sinom;
        scale1 = sin(t * omega) / sinom;
    } else {        
        // "from" and "to" quaternions are very close 
        //  ... so we can do a linear interpolation
        scale0 = 1.0 - t;
        scale1 = t;
    }
    // calculate final values
    out[0] = scale0 * ax + scale1 * bx;
    out[1] = scale0 * ay + scale1 * by;
    out[2] = scale0 * az + scale1 * bz;
    out[3] = scale0 * aw + scale1 * bw;
    
    return out;
};

/**
 * Sets a quat from the given angle and rotation axis,
 * then returns it.
 *
 * @param {quat} out the receiving quaternion
 * @param {vec3} axis the axis around which to rotate
 * @param {Number} rad the angle in radians
 * @returns {quat} out
 **/
float* Quat::setAxisAngle(float *out, float *axis, float rad) {
    rad = rad * 0.5;
    float s = sin(rad);
    out[0] = s * axis[0];
    out[1] = s * axis[1];
    out[2] = s * axis[2];
    out[3] = cos(rad);
    return out;
};
/**
 * Gets the rotation axis and angle for a given
 *  quaternion. If a quaternion is created with
 *  setAxisAngle, this method will return the same
 *  values as providied in the original parameter list
 *  OR functionally equivalent values.
 * Example: The quaternion formed by axis [0, 0, 1] and
 *  angle -90 is the same as the quaternion formed by
 *  [0, 0, 1] and 270. This method favors the latter.
 * @param  {vec3} out_axis  Vector receiving the axis of rotation
 * @param  {quat} q     Quaternion to be decomposed
 * @return {Number}     Angle, in radians, of the rotation
 */
float Quat::getAxisAngle(float *out_axis, float *q) {
    float rad = acos(q[3]) * 2.0;
    float s = sin(rad / 2.0);
    if (s != 0.0) {
        out_axis[0] = q[0] / s;
        out_axis[1] = q[1] / s;
        out_axis[2] = q[2] / s;
    } else {
        // If s is zero, return any axis (no rotation - axis does not matter)
        out_axis[0] = 1;
        out_axis[1] = 0;
        out_axis[2] = 0;
    }
    return rad;
};
/**
 * Multiplies two quat's
 *
 * @param {quat} out the receiving quaternion
 * @param {quat} a the first operand
 * @param {quat} b the second operand
 * @returns {quat} out
 */
float* Quat::multiply(float *out, float *a, float *b){
    float ax = a[0], ay = a[1], az = a[2], aw = a[3],
        bx = b[0], by = b[1], bz = b[2], bw = b[3];
    out[0] = ax * bw + aw * bx + ay * bz - az * by;
    out[1] = ay * bw + aw * by + az * bx - ax * bz;
    out[2] = az * bw + aw * bz + ax * by - ay * bx;
    out[3] = aw * bw - ax * bx - ay * by - az * bz;
    return out;
};
/**
 * Rotates a quaternion by the given angle about the X axis
 *
 * @param {quat} out quat receiving operation result
 * @param {quat} a quat to rotate
 * @param {number} rad angle (in radians) to rotate
 * @returns {quat} out
 */
float* Quat::rotateX(float *out, float *a, float rad){
    rad *= 0.5; 

    float ax = a[0], ay = a[1], az = a[2], aw = a[3],
        bx = sin(rad), bw = cos(rad);

    out[0] = ax * bw + aw * bx;
    out[1] = ay * bw + az * bx;
    out[2] = az * bw - ay * bx;
    out[3] = aw * bw - ax * bx;
    return out;
};


/**
 * Rotates a quaternion by the given angle about the Y axis
 *
 * @param {quat} out quat receiving operation result
 * @param {quat} a quat to rotate
 * @param {number} rad angle (in radians) to rotate
 * @returns {quat} out
 */
float* Quat::rotateY(float *out, float *a, float rad){
    rad *= 0.5; 

    float ax = a[0], ay = a[1], az = a[2], aw = a[3],
        by = sin(rad), bw = cos(rad);

    out[0] = ax * bw - az * by;
    out[1] = ay * bw + aw * by;
    out[2] = az * bw + ax * by;
    out[3] = aw * bw - ay * by;
    return out;
};

/**
 * Rotates a quaternion by the given angle about the Z axis
 *
 * @param {quat} out quat receiving operation result
 * @param {quat} a quat to rotate
 * @param {number} rad angle (in radians) to rotate
 * @returns {quat} out
 */
float* Quat::rotateZ(float *out, float *a, float rad){
    rad *= 0.5; 

    float ax = a[0], ay = a[1], az = a[2], aw = a[3],
        bz = sin(rad), bw = cos(rad);

    out[0] = ax * bw + ay * bz;
    out[1] = ay * bw - ax * bz;
    out[2] = az * bw + aw * bz;
    out[3] = aw * bw - az * bz;
    return out;
};

float Mat4::GLMAT_EPSILON = 0.000001f;

float* Mat4::clone(float* a) {
    float* out = new float[16];
    std::copy(a, a + 16, out);
    return out;
}

float* Mat4::copy(float* a, float *b) {
    std::copy(b, b + 16, a);
    return a;
}


float* Mat4::create() {
    float * f = new float[16]{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    return f;
}

float* Mat4::identity(float *out) {
    out[0] = 1;
    out[1] = 0;
    out[2] = 0;
    out[3] = 0;
    out[4] = 0;
    out[5] = 1;
    out[6] = 0;
    out[7] = 0;
    out[8] = 0;
    out[9] = 0;
    out[10] = 1;
    out[11] = 0;
    out[12] = 0;
    out[13] = 0;
    out[14] = 0;
    out[15] = 1;
    return out;
}

/**
 * Calculates a 4x4 matrix from the given quaternion
 *
 * @param {mat4} out mat4 receiving operation result
 * @param {quat} q Quaternion to create matrix from
 *
 * @returns {mat4} out
 */

float* Mat4::fromQuat (float *out, float *q) {
    float x = q[0], y = q[1], z = q[2], w = q[3],
        x2 = x + x,
        y2 = y + y,
        z2 = z + z,
        xx = x * x2,
        yx = y * x2,
        yy = y * y2,
        zx = z * x2,
        zy = z * y2,
        zz = z * z2,
        wx = w * x2,
        wy = w * y2,
        wz = w * z2;

    out[0] = 1 - yy - zz;
    out[1] = yx + wz;
    out[2] = zx - wy;
    out[3] = 0;
    out[4] = yx - wz;
    out[5] = 1 - xx - zz;
    out[6] = zy + wx;
    out[7] = 0;
    out[8] = zx + wy;
    out[9] = zy - wx;
    out[10] = 1 - xx - yy;
    out[11] = 0;
    out[12] = 0;
    out[13] = 0;
    out[14] = 0;
    out[15] = 1;

    return out;
};

float* Mat4::fromRotationTranslation(float* out, float* q, float* v) {
    // Quaternion math
    float x = q[0], y = q[1], z = q[2], w = q[3],
        x2 = x + x,
        y2 = y + y,
        z2 = z + z,

        xx = x * x2,
        xy = x * y2,
        xz = x * z2,
        yy = y * y2,
        yz = y * z2,
        zz = z * z2,
        wx = w * x2,
        wy = w * y2,
        wz = w * z2;

    out[0] = 1 - (yy + zz);
    out[1] = xy + wz;
    out[2] = xz - wy;
    out[3] = 0;
    out[4] = xy - wz;
    out[5] = 1 - (xx + zz);
    out[6] = yz + wx;
    out[7] = 0;
    out[8] = xz + wy;
    out[9] = yz - wx;
    out[10] = 1 - (xx + yy);
    out[11] = 0;
    out[12] = v[0];
    out[13] = v[1];
    out[14] = v[2];
    out[15] = 1;
    
    return out;
}

/**
 * Generates a orthogonal projection matrix with the given bounds
 *
 * @param {mat4} out mat4 frustum matrix will be written into
 * @param {number} left Left bound of the frustum
 * @param {number} right Right bound of the frustum
 * @param {number} bottom Bottom bound of the frustum
 * @param {number} top Top bound of the frustum
 * @param {number} near Near bound of the frustum
 * @param {number} far Far bound of the frustum
 * @returns {mat4} out
 */
float* Mat4::ortho(float *out, float left, float right, float bottom, float top, float near, float far) {
    float lr = 1.0 / (left - right),
        bt = 1.0 / (bottom - top),
        nf = 1.0 / (near - far);
    
    out[0] = -2.0 * lr;
    out[1] = 0;
    out[2] = 0;
    out[3] = 0;
    out[4] = 0;
    out[5] = -2.0 * bt;
    out[6] = 0;
    out[7] = 0;
    out[8] = 0;
    out[9] = 0;
    out[10] = 2.0 * nf;
    out[11] = 0;
    out[12] = (left + right) * lr;
    out[13] = (top + bottom) * bt;
    out[14] = (far + near) * nf;
    out[15] = 1.0;

    return out;

};

float* Mat4::lookAt(float *out, float *eye, float *center, float *up) {
    float x0, x1, x2, y0, y1, y2, z0, z1, z2, len,
        eyex = eye[0],
        eyey = eye[1],
        eyez = eye[2],
        upx = up[0],
        upy = up[1],
        upz = up[2],
        centerx = center[0],
        centery = center[1],
        centerz = center[2];

    if (fabs(eyex - centerx) < GLMAT_EPSILON &&
        fabs(eyey - centery) < GLMAT_EPSILON &&
        fabs(eyez - centerz) < GLMAT_EPSILON) {
        return Mat4::identity(out);
    }

    z0 = eyex - centerx;
    z1 = eyey - centery;
    z2 = eyez - centerz;

    len = 1.0f / sqrt(z0 * z0 + z1 * z1 + z2 * z2);
    z0 *= len;
    z1 *= len;
    z2 *= len;

    x0 = upy * z2 - upz * z1;
    x1 = upz * z0 - upx * z2;
    x2 = upx * z1 - upy * z0;
    len = sqrt(x0 * x0 + x1 * x1 + x2 * x2);
    if (!len) {
        x0 = 0;
        x1 = 0;
        x2 = 0;
    } else {
        len = 1.0f / len;
        x0 *= len;
        x1 *= len;
        x2 *= len;
    }

    y0 = z1 * x2 - z2 * x1;
    y1 = z2 * x0 - z0 * x2;
    y2 = z0 * x1 - z1 * x0;

    len = sqrt(y0 * y0 + y1 * y1 + y2 * y2);
    if (!len) {
        y0 = 0;
        y1 = 0;
        y2 = 0;
    } else {
        len = 1.0f / len;
        y0 *= len;
        y1 *= len;
        y2 *= len;
    }

    out[0] = x0;
    out[1] = y0;
    out[2] = z0;
    out[3] = 0;
    out[4] = x1;
    out[5] = y1;
    out[6] = z1;
    out[7] = 0;
    out[8] = x2;
    out[9] = y2;
    out[10] = z2;
    out[11] = 0;
    out[12] = -(x0 * eyex + x1 * eyey + x2 * eyez);
    out[13] = -(y0 * eyex + y1 * eyey + y2 * eyez);
    out[14] = -(z0 * eyex + z1 * eyey + z2 * eyez);
    out[15] = 1.0f;

    return out;
};

float * Mat4::multiply(float *out, float *a, float *b) {
    float a00 = a[0], a01 = a[1], a02 = a[2], a03 = a[3],
        a10 = a[4], a11 = a[5], a12 = a[6], a13 = a[7],
        a20 = a[8], a21 = a[9], a22 = a[10], a23 = a[11],
        a30 = a[12], a31 = a[13], a32 = a[14], a33 = a[15];

    // Cache only the current line of the second matrix
    float b0  = b[0], b1 = b[1], b2 = b[2], b3 = b[3];  
    out[0] = b0*a00 + b1*a10 + b2*a20 + b3*a30;
    out[1] = b0*a01 + b1*a11 + b2*a21 + b3*a31;
    out[2] = b0*a02 + b1*a12 + b2*a22 + b3*a32;
    out[3] = b0*a03 + b1*a13 + b2*a23 + b3*a33;

    b0 = b[4]; b1 = b[5]; b2 = b[6]; b3 = b[7];
    out[4] = b0*a00 + b1*a10 + b2*a20 + b3*a30;
    out[5] = b0*a01 + b1*a11 + b2*a21 + b3*a31;
    out[6] = b0*a02 + b1*a12 + b2*a22 + b3*a32;
    out[7] = b0*a03 + b1*a13 + b2*a23 + b3*a33;

    b0 = b[8]; b1 = b[9]; b2 = b[10]; b3 = b[11];
    out[8] = b0*a00 + b1*a10 + b2*a20 + b3*a30;
    out[9] = b0*a01 + b1*a11 + b2*a21 + b3*a31;
    out[10] = b0*a02 + b1*a12 + b2*a22 + b3*a32;
    out[11] = b0*a03 + b1*a13 + b2*a23 + b3*a33;

    b0 = b[12]; b1 = b[13]; b2 = b[14]; b3 = b[15];
    out[12] = b0*a00 + b1*a10 + b2*a20 + b3*a30;
    out[13] = b0*a01 + b1*a11 + b2*a21 + b3*a31;
    out[14] = b0*a02 + b1*a12 + b2*a22 + b3*a32;
    out[15] = b0*a03 + b1*a13 + b2*a23 + b3*a33;
    return out;
};

float* Mat4::perspective(float* out, float fovy, float aspect, float near, float far) {
    float f = 1.0 / (float) tan(fovy / 2);
    float nf = 1.0 / (near - far);
    out[0] = f / aspect;
    out[1] = 0;
    out[2] = 0;
    out[3] = 0;
    out[4] = 0;
    out[5] = f;
    out[6] = 0;
    out[7] = 0;
    out[8] = 0;
    out[9] = 0;
    out[10] = (far + near) * nf;
    out[11] = -1.0;
    out[12] = 0;
    out[13] = 0;
    out[14] = (2.0 * far * near) * nf;
    out[15] = 0;
    return out;
}
float* Mat4::rotate(float* out, float* a, float rad, float* axis) {
    return rotate(out, a, rad,  axis[0], axis[1], axis[2]);
}
float* Mat4::rotate(float* out, float* a, float rad, float x, float y, float z) {
        float len = sqrt(x * x + y * y + z * z),
        s, c, t,
        a00, a01, a02, a03,
        a10, a11, a12, a13,
        a20, a21, a22, a23,
        b00, b01, b02,
        b10, b11, b12,
        b20, b21, b22;

    if (fabs(len) < GLMAT_EPSILON) { return out; }
    
    len = 1 / len;
    x *= len;
    y *= len;
    z *= len;

    s = sin(rad);
    c = cos(rad);
    t = 1 - c;

    a00 = a[0]; a01 = a[1]; a02 = a[2]; a03 = a[3];
    a10 = a[4]; a11 = a[5]; a12 = a[6]; a13 = a[7];
    a20 = a[8]; a21 = a[9]; a22 = a[10]; a23 = a[11];

    // Construct the elements of the rotation matrix
    b00 = x * x * t + c; b01 = y * x * t + z * s; b02 = z * x * t - y * s;
    b10 = x * y * t - z * s; b11 = y * y * t + c; b12 = z * y * t + x * s;
    b20 = x * z * t + y * s; b21 = y * z * t - x * s; b22 = z * z * t + c;

    // Perform rotation-specific matrix multiplication
    out[0] = a00 * b00 + a10 * b01 + a20 * b02;
    out[1] = a01 * b00 + a11 * b01 + a21 * b02;
    out[2] = a02 * b00 + a12 * b01 + a22 * b02;
    out[3] = a03 * b00 + a13 * b01 + a23 * b02;
    out[4] = a00 * b10 + a10 * b11 + a20 * b12;
    out[5] = a01 * b10 + a11 * b11 + a21 * b12;
    out[6] = a02 * b10 + a12 * b11 + a22 * b12;
    out[7] = a03 * b10 + a13 * b11 + a23 * b12;
    out[8] = a00 * b20 + a10 * b21 + a20 * b22;
    out[9] = a01 * b20 + a11 * b21 + a21 * b22;
    out[10] = a02 * b20 + a12 * b21 + a22 * b22;
    out[11] = a03 * b20 + a13 * b21 + a23 * b22;

    if (a != out) { // If the source and destination differ, copy the unchanged last row
        out[12] = a[12];
        out[13] = a[13];
        out[14] = a[14];
        out[15] = a[15];
    }
    return out;
}
/**
 * Rotates a matrix by the given angle around the X axis
 *
 * @param {mat4} out the receiving matrix
 * @param {mat4} a the matrix to rotate
 * @param {Number} rad the angle to rotate the matrix by
 * @returns {mat4} out
 */
float* Mat4::rotateX(float* out, float* a, float rad) {
    float s = sin(rad),
        c = cos(rad),
        a10 = a[4],
        a11 = a[5],
        a12 = a[6],
        a13 = a[7],
        a20 = a[8],
        a21 = a[9],
        a22 = a[10],
        a23 = a[11];

    if (a != out) { // If the source and destination differ, copy the unchanged rows
        out[0]  = a[0];
        out[1]  = a[1];
        out[2]  = a[2];
        out[3]  = a[3];
        out[12] = a[12];
        out[13] = a[13];
        out[14] = a[14];
        out[15] = a[15];
    }

    // Perform axis-specific matrix multiplication
    out[4] = a10 * c + a20 * s;
    out[5] = a11 * c + a21 * s;
    out[6] = a12 * c + a22 * s;
    out[7] = a13 * c + a23 * s;
    out[8] = a20 * c - a10 * s;
    out[9] = a21 * c - a11 * s;
    out[10] = a22 * c - a12 * s;
    out[11] = a23 * c - a13 * s;
    return out;
}

float* Mat4::rotateY(float* out, float* a, float rad) {
    float s = sin(rad),
        c = cos(rad),
        a00 = a[0],
        a01 = a[1],
        a02 = a[2],
        a03 = a[3],
        a20 = a[8],
        a21 = a[9],
        a22 = a[10],
        a23 = a[11];

    if (a != out) { // If the source and destination differ, copy the unchanged rows
        out[4]  = a[4];
        out[5]  = a[5];
        out[6]  = a[6];
        out[7]  = a[7];
        out[12] = a[12];
        out[13] = a[13];
        out[14] = a[14];
        out[15] = a[15];
    }

    // Perform axis-specific matrix multiplication
    out[0] = a00 * c - a20 * s;
    out[1] = a01 * c - a21 * s;
    out[2] = a02 * c - a22 * s;
    out[3] = a03 * c - a23 * s;
    out[8] = a00 * s + a20 * c;
    out[9] = a01 * s + a21 * c;
    out[10] = a02 * s + a22 * c;
    out[11] = a03 * s + a23 * c;
    return out;
}

float* Mat4::translate(float* out, float* a, float* v) {
    return translate(out, a, v[0], v[1], v[2]);
}

float* Mat4::translate(float* out, float* a,float x,float y,float z) {
    float a00, a01, a02, a03,
        a10, a11, a12, a13,
        a20, a21, a22, a23;

    if (a == out) {
        out[12] = a[0] * x + a[4] * y + a[8] * z + a[12];
        out[13] = a[1] * x + a[5] * y + a[9] * z + a[13];
        out[14] = a[2] * x + a[6] * y + a[10] * z + a[14];
        out[15] = a[3] * x + a[7] * y + a[11] * z + a[15];
    } else {
        a00 = a[0]; a01 = a[1]; a02 = a[2]; a03 = a[3];
        a10 = a[4]; a11 = a[5]; a12 = a[6]; a13 = a[7];
        a20 = a[8]; a21 = a[9]; a22 = a[10]; a23 = a[11];

        out[0] = a00; out[1] = a01; out[2] = a02; out[3] = a03;
        out[4] = a10; out[5] = a11; out[6] = a12; out[7] = a13;
        out[8] = a20; out[9] = a21; out[10] = a22; out[11] = a23;

        out[12] = a00 * x + a10 * y + a20 * z + a[12];
        out[13] = a01 * x + a11 * y + a21 * z + a[13];
        out[14] = a02 * x + a12 * y + a22 * z + a[14];
        out[15] = a03 * x + a13 * y + a23 * z + a[15];
    }

    return out;
};