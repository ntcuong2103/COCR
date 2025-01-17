#pragma once

#include "3d/wrapper_base.h"

namespace Qt3DExtras {
    class QPhongMaterial;
}

class SingleWrapper : public BaseWrapper {
Q_OBJECT
protected:
    Qt3DExtras::QPhongMaterial *material;
public:
    SingleWrapper(Qt3DCore::QEntity *_root);

    void setColor(const QColor &_color) override;
};