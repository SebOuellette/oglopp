#ifndef PERLIN_H
#define PERLIN_H

/*


uniform float time;

float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453) * 100.0;
}

vec2 getblock(float res, vec2 loc) {
    vec2 block = floor((loc) / res) * res;

    return block;
}

vec2 getVector(bool animate, float res, vec2 loc) {
    float block = rand(getblock(res, loc)) * 100.0;
    float rotSpeed = time * (sin(block) / 1.0);

    if (!animate) {
        rotSpeed = 0.0;
    }

    return vec2(sin(block + rotSpeed), cos(block + rotSpeed));
}

float interpolate(float t) {
    return 6.0 * pow(t, 5.0) - 15.0 * pow(t, 4.0) + 10.0 * pow(t, 3.0);
}

float perlin(bool animate, float res, vec2 loc) {
    vec2 localCoord = (getblock(res, loc + res) - loc) / res;

    vec2 vecTL = getVector(animate, res, loc);
    vec2 vecTR = getVector(animate, res, loc + vec2(res, 0));
    vec2 vecBL = getVector(animate, res, loc + vec2(0, -res));
    vec2 vecBR = getVector(animate, res, loc + vec2(res, -res));

    vec2 interp = vec2(interpolate(localCoord.x), interpolate(localCoord.y));

    // Map range from 0 - 1 to -1 - 1
    localCoord = localCoord * 2.0 - 1.0;

    float dottl = (dot(vecTL, localCoord - vec2(1.0, -1.0)) + 1.4) / 2.3;
    float dottr = (dot(vecTR, localCoord - vec2(-1.0, -1.0)) + 1.4) / 2.3;
    float dotbl = (dot(vecBL, localCoord - vec2(1.0, 1.0)) + 1.4) / 2.3;
    float dotbr = (dot(vecBR, localCoord - vec2(-1.0, 1.0)) + 1.4) / 2.3;

    float horizTop = interp.x * dottl + (1.0 - interp.x) * dottr;
    float horizBottom = interp.x * dotbl + (1.0 - interp.x) * dotbr;

    float vertical = interp.y * horizBottom + (1.0 - interp.y) * horizTop;

    return vertical / 1.2;
}
*/

class Perlin {
public:
   // static 

private:

};

#endif