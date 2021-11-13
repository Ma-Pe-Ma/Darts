#ifndef THROWRESULT_H
#define THROWRESULT_H

enum ThrowType {
    normal,
    busted,
    winning
};

struct ThrowResult {
    ThrowType throwType;
};

#endif