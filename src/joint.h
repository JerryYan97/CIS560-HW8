#ifndef JOINT_H
#define JOINT_H
#include "la.h"
#include <memory>
#include "drawable.h"
#include "polygon.h"
#include <QListWidget>

class Joint : public QListWidgetItem
{
public:
    Joint();
    Joint(glm::vec3 iRelativePos);
    ~Joint(){}
    QString mName;
    Joint* mParentJoint;
    std::vector<Joint*> mChildJointsVec;
    glm::vec3 mRelativePos;
    glm::quat mOrientation;
    glm::mat4 mBindMatrix;
    void UpdateOrientation();

    // returns a mat4 that represents the concatenation of a joint's position and rotation.
    glm::mat4 getLocalTransformation();
    // returns a mat4 that represents the concatentation of this joint's local transformation
    // with the transformations of its chain of parent joints.
    glm::mat4 getOverallTransformation();

    int mID;
    bool mSelected;
    double mXAxisDegree;
    double mYAxisDegree;
    double mZAxisDegree;
};


class Skeleton : public Drawable
{
public:
    std::vector<std::unique_ptr<Joint>> mJointsList;
    Joint* mRoot;

    Skeleton(OpenGLContext* context);
    ~Skeleton(){}
    void create() override;
    GLenum drawMode();

    void ClearSelected();
    glm::mat4 GetModelMat();
    bool SkeletonReady();

    std::vector<glm::mat4> getBindMatVec();
    std::vector<glm::mat4> getJointToWorldMatVec();
};


class FrameWireSphere
{
public:
    FrameWireSphere();
    std::vector<glm::vec4> verticalZVec;
    std::vector<glm::vec4> verticalXVec;
    std::vector<glm::vec4> horizontalVec;
};
#endif // JOINT_H
