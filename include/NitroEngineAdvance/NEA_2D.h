#ifndef NEA_2D_H
#define NEA_2D_H
#include <memory>

#include "NitroEngineAdvance/NEA_Math.h"
#include "NitroEngineAdvance/NEA_Object.h"

extern "C" {
#include <NEMain.h>
#include <nds/ndstypes.h>
}

struct Sprite3D : public NEA_Object {
    // Meta information
    Vector2<s16> size;
    Vector2<s16> widthUV;
    Vector2<s16> heightUV;
    void draw() override;
    void setMaterial(std::unique_ptr<NE_Material>& mat) override;
    Sprite3D();

   private:
    std::unique_ptr<NE_Material> material;
};

#endif
