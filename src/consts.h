#ifndef CONSTS_H
#define CONSTS_H

enum class Shade {  PHONG,
                    GI,     // Global Ilumination: Direct + Indirect Light
                    GI_D,   // Global Ilumination: Direct Light only
                    GI_I    // Global Ilumination: Indirect Light only
                 };


#endif // CONSTS_H
