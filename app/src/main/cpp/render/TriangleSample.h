//
// Created by okunu on 2022/10/29.
//

#ifndef DEMO_TRIANGLESAMPLE_H
#define DEMO_TRIANGLESAMPLE_H

#include "GLUtil.h"
#include "LogUtil.h"
#include "BaseSample.h"

class TriangleSample: public BaseSample {

    using superClass = BaseSample;

public:
    TriangleSample();

    void init();
    void draw();
};


#endif //DEMO_TRIANGLESAMPLE_H
