// 
// File:   Type.h
// Author: serazi
//
// Created on October 9, 2003, 5:38 PM
//

#ifndef _Type_H
#define	_Type_H

struct Type { 
    static const int UNSIGNED_INT = 0; // unsigned integer type
    static const int SIGNED_INT = 1;
    static const int UNSIGNED_DOUBLE = 2;
    static const int SIGNED_DOUBLE = 3;
    static const int SING_CAT = 4;
    static const int MULT_CAT = 5;
    static const int BIT_SET = 6;
    static const int SKIP = 7;
};

#endif	/* _Type_H */

