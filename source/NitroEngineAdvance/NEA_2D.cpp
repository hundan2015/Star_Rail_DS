#include "NitroEngineAdvance/NEA_2D.h"

#include "nds/arm9/math.h"

extern "C" {
#include "nds/arm9/video.h"
}

void Sprite3D::draw() {
    NE_MaterialUse(material.get());
    MATRIX_PUSH = 0;

    MATRIX_TRANSLATE = inttof32((int)transform.x);
    MATRIX_TRANSLATE = inttof32((int)transform.y);
    MATRIX_TRANSLATE = inttof32((int)transform.z);

    MATRIX_SCALE = inttof32((int)scale.x);
    MATRIX_SCALE = inttof32((int)scale.y);
    MATRIX_SCALE = inttof32((int)scale.z);

    if (rotation.x != 0) glRotateXi((int)(rotation.x) << 6);
    if (rotation.y != 0) glRotateYi((int)(rotation.x) << 6);
    if (rotation.z != 0) glRotateZi((int)(rotation.z) << 6);

    // Begin drawing
    NE_PolyBegin(GL_QUAD);
    // Top left
    NE_PolyColor(NE_White);   // Set next vertices color
    NE_PolyTexCoord(0, 0);    // Texture coordinates
    NE_PolyVertex(-1, 1, 0);  // Send new vertex
    // Top right
    NE_PolyColor(NE_White);
    NE_PolyTexCoord(0, 64);
    NE_PolyVertex(-1, -1, 0);
    // Bottom right
    NE_PolyColor(NE_White);
    NE_PolyTexCoord(64, 64);
    NE_PolyVertex(1, -1, 0);
    // Bottom left
    NE_PolyColor(NE_White);
    NE_PolyTexCoord(64, 0);
    NE_PolyVertex(1, 1, 0);

    NE_PolyEnd();
    MATRIX_POP = 1;
}

void Sprite3D::setMaterial(std::unique_ptr<NE_Material>& mat) {
    material = std::move(mat);
}

Sprite3D::Sprite3D() {
    scale.x = 1;
    scale.y = 1;
    scale.z = 1;
}
