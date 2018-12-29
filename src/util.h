#ifndef __UTIL_H_
#define __UTIL_H_

#define max(a,b)                                \
    ({ __typeof__ (a) _a = (a);                 \
        __typeof__ (b) _b = (b);                \
        _a > _b ? _a : _b; })
#define min(a,b)                                \
    ({ __typeof__ (a) _a = (a);                 \
        __typeof__ (b) _b = (b);                \
        _a < _b ? _a : _b; })
#define abs(x)                                  \
    ({ __typeof__ (x) _x = (x);                 \
        _x < 0 ? -_x : x; })


#endif // __UTIL_H_
