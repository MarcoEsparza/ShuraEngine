/*************************************************************/
/*
*  @file    sh.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/03/26
*  @brief
*
*
*
*  @bug     No bug known.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shAudioManager.h"
#include "shSound.h"

#define MAX_CHANNELS            512

namespace shEngineSDK {
void
AudioManager::initSystem()
{
  m_system = nullptr;
  FMOD_RESULT result = FMOD::System_Create(&m_system);
  if (result != FMOD_OK) {
    SH_ASSERT("FMOD error at system creation");
  }

  result = m_system->init(MAX_CHANNELS, FMOD_INIT_NORMAL, nullptr);
  if (result != FMOD_OK) {
    SH_ASSERT("FMOD error at system initialization");
  }
}

void
AudioManager::update()
{
  m_system->update();
}

void
AudioManager::close()
{
  m_channel->stop();
  m_channel = nullptr;
  m_system->release();
  //m_system = nullptr;
}

SPtr<Sound>
AudioManager::createSound(Path& filePath)
{
  auto pSound = make_shared<Sound>();

  FMOD_RESULT res = FMOD_OK;
  res = m_system->createSound("resources/cat.wav", FMOD_DEFAULT, nullptr, &pSound->m_sound);
  if (res != FMOD_OK) {
    SH_ASSERT("Error on sound creation");
  }

  return pSound;
}

void
AudioManager::playSound(const SPtr<Sound>& pSound)
{
  FMOD_RESULT res = FMOD_OK;
  res = m_system->playSound(pSound->m_sound, nullptr, false, &m_channel);
  if (res != FMOD_OK) {
    SH_ASSERT("Error on sound play");
  }
  pSound->bIsPlaying = true;
}
}
