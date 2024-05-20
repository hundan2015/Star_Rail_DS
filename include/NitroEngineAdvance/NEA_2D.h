#ifndef NEA_2D_H
#define NEA_2D_H
#include <memory>

#include "NitroEngineAdvance/NEA_Math.h"

extern "C" {
#include <NEMain.h>
#include <nds/ndstypes.h>
}

struct Sprite3D {
    // Transform
    Vecter3<s16> transform;
    // Rotation
    Vecter3<s16> rotation;
    // Scale
    Vecter3<s16> scale;
    // Meta information
    Vecter2<s16> size;
    Vecter2<s16> widthUV;
    Vecter2<s16> heightUV;
    void draw();
    void setMaterial(std::unique_ptr<NE_Material>& mat);

    Sprite3D() {
        scale.x = 1;
        scale.y = 1;
        scale.z = 1;
    }

   private:
    std::unique_ptr<NE_Material> material;
};

#endif
