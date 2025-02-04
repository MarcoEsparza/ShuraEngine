/*************************************************************/
/*
*  @file    Player.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/13
*  @brief
*
*
*
*  @bug     No bug known.
*/
/*************************************************************/
#pragma once

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shPrerequisitesCore.h"
#include "shPrerequisitesPhysics.h"
#include "shGraphicTypes.h"
#include "shVector2.h"
#include "shMatrix4.h"
#include "shSphere.h"
#include "shPath.h"

namespace shEngineSDK {
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
class Texture2D;

namespace DIRECTION {
enum E
{
  kUp = 0,
  kUpRight,
  kRight,
  kDownRight,
  kDown,
  kDownLeft,
  kLeft,
  kUpLeft
};
}

class Sprite
{
 public:
  Sprite() = default;
  Sprite(const Path& filePath, const Vector2& min, const Vector2& max);
  ~Sprite() = default;

  void
  setSprite(const Path& filePath, const Vector2& min, const Vector2& max);

  SPtr<VertexBuffer> m_pVB;
  SPtr<IndexBuffer> m_pIB;
  SPtr<Texture2D> m_pTexture;
  Vector<VertexData> m_vertices;
  Vector<uint32> m_indices;
  Matrix4 m_transform = Matrix4::IDENTITY;
};

class Arrow
{
 public:
  Arrow() = default;
  Arrow(const Path& filePath, const Vector2& min, const Vector2& max);
  ~Arrow() = default;

 public:
  SPtr<Sprite> m_sprite;
  Matrix4 m_position = Matrix4::IDENTITY;
  SPtr<ConstantBuffer> m_modelBuffer;
};

class Player
{
 public:
  Player() = default;
  Player(const Vector2& min,
         const Vector2& max,
         const Path& filePath,
         const Vector2& position,
         const float mass,
         const float speed,
         const float dragC,
         const float radius);
  ~Player() = default;

  void
  setPlayer(const Vector2& min,
            const Vector2& max,
            const Path& filePath,
            const Vector2& position,
            const float mass,
            const float speed,
            const float dragC,
            const float radius);

  void
  setArrow(const SPtr<Arrow>& arrow);

  void
  move(const Vector2& direction);

  Vector2
  calculateDrag();

  void
  eulerDrag(const Vector2& dForce, const float dt);

  void
  verletDrag(const Vector2& dForce, const float dt);

  void
  update(float deltaTime);

  void
  updateModelBuffer();

  void
  updateArrow();

  /*const Sprite&
  getSprite() const { return m_sprite; }*/

 public:
  SPtr<Sprite> m_sprite;

  Vector2 m_velocity;
  Vector2 m_position;

  Vector2 m_previousPosition = Vector2(0.0f, 0.0f);

  float m_mass;
  float m_speed;
  float m_dragC;
  float m_radius;

  SPtr<Arrow> m_pDirArrow;
  DIRECTION::E m_direction = DIRECTION::kRight;

  Matrix4 m_transform = Matrix4::IDENTITY;
  SPtr<ConstantBuffer> m_modelBuffer;
};
}
