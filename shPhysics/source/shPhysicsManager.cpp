/*************************************************************/
/*
*  @file    shPhysicsManager.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/06/27
*  @brief   Physics manager class for handling physics simulation.
*
*  Physics manager class for handling physics simulation.
*
* @bug      No bug known
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPhysicsManager.h"

// Temporary include for sorting
using std::sort;

namespace shEngineSDK
{
Vector<Pair<Rigidbody*, Rigidbody*>>
PhysicsManager::sweepAndPrune(const Vector<Rigidbody*>& rigidbodies)
{
  sort(rigidbodies.begin(), rigidbodies.end(),
     [](const Rigidbody* a, const Rigidbody* b) {
       return a->m_position.x < b->m_position.x;
  });

  Vector<Pair<Rigidbody*, Rigidbody*>> possiblePairs;

  for (size_t i = 0; i < rigidbodies.size(); ++i) {
    Rigidbody* rb1 = rigidbodies[i];
    for (size_t j = i + 1; j < rigidbodies.size(); ++j) {
      Rigidbody* rb2 = rigidbodies[j];
      // Check if the bounding boxes overlap on the x-axis
      if (rb1->m_position.x + rb1->m_linearVelocity.x >
          rb2->m_position.x - rb2->m_linearVelocity.x &&
          rb1->m_position.x - rb1->m_linearVelocity.x <
          rb2->m_position.x + rb2->m_linearVelocity.x) {
        possiblePairs.push_back({rb1, rb2});
      }
    }
  }

  return possiblePairs;
}

SH_PHYSICS_EXPORT PhysicsManager&
g_physicsMan()
{
  return PhysicsManager::instance();
}
}
