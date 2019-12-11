#include "jsonreader.h"
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>

#include <QSysInfo>

#include <cstdlib>
#include <iostream>


JSONReader::JSONReader()
{

}

Joint* ConstructBranch(const QJsonObject& iJObj, Joint* parent, Skeleton& iSkeleton)
{
    std::unique_ptr<Joint> uCurrJointPtr = std::make_unique<Joint>();
    Joint* currJointPtr = uCurrJointPtr.get();
    currJointPtr->mName = iJObj.value("name").toString();
    currJointPtr->setText(currJointPtr->mName);
    currJointPtr->mParentJoint = parent;
    currJointPtr->mBindMatrix = glm::mat4(1.f);
    //Construct Orientation
    QJsonArray quatJsonArray = iJObj.value("rot").toArray();
    //glm::quat quat;
    glm::mat4 a;
    glm::vec3 axis = glm::vec3(quatJsonArray[1].toDouble(), quatJsonArray[2].toDouble(), quatJsonArray[3].toDouble());
    a = glm::rotate(glm::mat4(1.f), float(quatJsonArray[0].toDouble()), axis);
    currJointPtr->mOrientation = glm::quat_cast(a);

    // Construct Position
    QJsonArray posJsonArray = iJObj.value("pos").toArray();
    currJointPtr->mRelativePos.x = posJsonArray[0].toDouble();
    currJointPtr->mRelativePos.y = posJsonArray[1].toDouble();
    currJointPtr->mRelativePos.z = posJsonArray[2].toDouble();

    // Construct ID:
    currJointPtr->mID = iSkeleton.mJointsList.size();

    iSkeleton.mJointsList.push_back(std::move(uCurrJointPtr));
    // Construct Children
    QJsonArray childrenJsonArray = iJObj.value("children").toArray();
    for(unsigned int i = 0; i < childrenJsonArray.size(); i++)
    {
        Joint* child = ConstructBranch(childrenJsonArray[i].toObject(), currJointPtr, iSkeleton);
        currJointPtr->mChildJointsVec.push_back(child);
    }
    return currJointPtr;
}

bool JSONReader::ParseToSkeleton(QString iFileName, Skeleton& iSkeleton)
{
    // Read File:
    QFile file(iFileName);
    if ( !file.open( QIODevice::ReadWrite ) ) {
        std::cout << "JSONReader: GetFile() cannot open this file.\n";
        return false;
    }
    else
    {
        std::cout << "JSONReader GetFile() Succssed\n";
    }

    // Parse File to Skeleton:
    QJsonParseError jsonParserError;
    QJsonDocument   jsonDocument = QJsonDocument::fromJson(file.readAll(), &jsonParserError);
    if ( !jsonDocument.isNull() && jsonParserError.error == QJsonParseError::NoError ) {
        std::cout << "ReadSuccess" << std::endl;
        if(jsonDocument.isObject())
        {
            QJsonObject jsonObject = jsonDocument.object();
            QJsonObject  jsonRootObj = jsonObject.value("root").toObject();
            ConstructBranch(jsonRootObj, nullptr, iSkeleton);
            iSkeleton.mRoot = iSkeleton.mJointsList.begin()->get();
            //iSkeleton.mRoot->mRelativePos = glm::vec3(-1.5, 0, 0);
        }
    }
    else
    {
        std::cout << "Read Failed" << std::endl;
    }
    return true;
}
