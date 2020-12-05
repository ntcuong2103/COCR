#ifndef _MOL3D_HPP_
#define _MOL3D_HPP_

#include "mol.hpp"

#include <Qt3DCore/QEntity>
#include <Qt3DExtras/QTorusMesh>

#include <unordered_map>

class Mol3D : public QObject {
Q_OBJECT

    Qt3DCore::QEntity *getSphereEntity(const QVector3D &_center, const float &_radius,
                                       const QColor &_color);

    Qt3DCore::QEntity *getCylinderEntity(const QVector3D &_from, const QVector3D &_to,
                                         const float &_radius, const QColor &_color);

    std::pair<Qt3DCore::QEntity *, Qt3DCore::QEntity *>
    getDoubleCylinderEntity(const QVector3D &_from, const QVector3D &_to,
                            const float &_radius, const QColor &_color);

    std::tuple<Qt3DCore::QEntity *, Qt3DCore::QEntity *, Qt3DCore::QEntity *>
    getTripleCylinderEntity(const QVector3D &_from, const QVector3D &_to,
                            const float &_radius, const QColor &_color);

    std::unordered_map<size_t, Qt3DCore::QEntity *> mAtomEntities;
    std::unordered_multimap<size_t, Qt3DCore::QEntity *> mBondEntities;

    Qt3DCore::QEntity *mRootEntity;
    std::shared_ptr<JMol> mol;

    void clear();


public:
    explicit Mol3D(Qt3DCore::QEntity *_rootEntity);

    ~Mol3D();

    void resetMol(std::shared_ptr<JMol> _mol);

public slots:

};


#endif//_MOL3D_HPP_
