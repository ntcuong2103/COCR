#ifndef _MOL3D_BUILDER_HPP_
#define _MOL3D_BUILDER_HPP_

#include "mol3d.hpp"
#include <Qt3DCore/QEntity>
#include <Qt3DExtras/QTorusMesh>
#include <optional>
#include <unordered_map>

class Mol3DBuilder : public QObject {
Q_OBJECT
    std::shared_ptr<Mol3D> mol3d;
    Qt3DCore::QEntity *mRootEntity;
    int sphereRings, sphereSlices, cylinderRings, cylinderSlices;
    float doubleBondSpaceScale, tripleBondSpaceScale;
    std::unordered_map<size_t, Qt3DCore::QEntity *> mAtomEntities;
    std::unordered_multimap<size_t, Qt3DCore::QEntity *> mBondEntities;

    Qt3DCore::QEntity *getSphereEntity(const QVector3D &_center, const float &_radius,
                                       const QColor &_color);

    Qt3DCore::QEntity *getSingleCylinderEntity(const QVector3D &_from, const QVector3D &_to,
                                               const float &_radius, const QColor &_color);

    Qt3DCore::QEntity *getDoubleCylinderEntity(const QVector3D &_from, const QVector3D &_to,
                                               const float &_radius, const QColor &_color,
                                               const std::optional<QVector3D> &_normVec);

    Qt3DCore::QEntity *getTripleCylinderEntity(const QVector3D &_from, const QVector3D &_to,
                                               const float &_radius, const QColor &_color,
                                               const std::optional<QVector3D> &_normVec);

    Qt3DCore::QEntity *getMultiCylinderEntities(
            const std::vector<QVector3D> &translations, const float &_radius,
            const QColor &_color, const QVector3D &_from, const QVector3D &_to,
            const std::optional<QVector3D> &_normVec);

    void clear();

public:
    explicit Mol3DBuilder(Qt3DCore::QEntity *_rootEntity, std::shared_ptr<JMol> _mol);

    ~Mol3DBuilder();

    void reset(std::shared_ptr<JMol> _mol);

    float getDoubleBondSpaceScale() const;

    void setDoubleBondSpaceScale(float doubleBondSpaceScale);

    float getTripleBondSpaceScale() const;

    void setTripleBondSpaceScale(float tripleBondSpaceScale);

    int getSphereRings() const;

    void setSphereRings(int sphereRings);

    int getSphereSlices() const;

    void setSphereSlices(int sphereSlices);

    int getCylinderRings() const;

    void setCylinderRings(int cylinderRings);

    int getCylinderSlices() const;

    void setCylinderSlices(int cylinderSlices);

    bool build();
};

#endif
