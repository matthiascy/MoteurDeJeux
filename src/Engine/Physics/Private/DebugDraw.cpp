#include "Physics/DebugDraw.hpp"

void DebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {

}

void DebugDraw::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime,
                                 const btVector3& color) {

}

void DebugDraw::reportErrorWarning(const char *warningString) {

}

void DebugDraw::draw3dText(const btVector3& location, const char *textString) {

}

void DebugDraw::setDebugMode(int debugMode) {

}

int DebugDraw::getDebugMode() const {
  return 0;
}
