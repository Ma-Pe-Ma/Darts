#ifndef THROWRESULT_H
#define THROWRESULT_H

enum ThrowType {
    normal,
    busted,
    missed,
    winning
};

struct ThrowResult {
    ThrowType throwType;
};

#endif