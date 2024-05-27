#ifndef NEA_OBJECT_H
#define NEA_OBJECT_H

#include <memory>
#include <vector>

#include "NEA_Math.h"
#include "nds/bios.h"

extern "C" {
#include "NEMain.h"
}

struct NEA_Object {
    // Transform
    Vector3<s16> transform;
    // Rotation
    Vector3<s16> rotation;
    // Scale
    Vector3<s16> scale;
    std::unique_ptr<NE_Model> model;
    std::vector<std::unique_ptr<NEA_Object>> child;
    virtual void draw() { NE_ModelDraw(model.get()); }
    virtual void setMaterial(std::unique_ptr<NE_Material>& mat);
};

#endif