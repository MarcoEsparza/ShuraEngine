/*************************************************************/
/*
*  @file    shPhysicsApp.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/01/11
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
#include "shBaseApp.h"
#include "Player.h"

namespace shEngineSDK {
class ProgramShader;
class InputLayout;
class SamplerState;

class PhysicsApp : public BaseApp
{
 public:
   PhysicsApp(const ScreenDesc& desc,
              const GRAPHIC_API::E dllGAPI = GRAPHIC_API::kDX11,
              const SampleDesc& sample = SampleDesc(1, 1))
              : BaseApp(desc, dllGAPI, sample),
                m_desc(desc) {}

  ~PhysicsApp() = default;

 protected:
  void
  onCreate() override;

  void
  onUpdate(float deltaTime) override;

  void
  onRender() override;

 private:
  ScreenDesc m_desc;
  SPtr<ProgramShader> m_pShader;
  SPtr<InputLayout> m_pIL;
  SPtr<SamplerState> m_pSamplerLinear;
  SPtr<Player> m_player;
};
}
