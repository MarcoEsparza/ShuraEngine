#include "shMeshCodec.h"
#include <shStringID.h>

namespace shEngineSDK {
UID
MeshCodec::getUID() const
{
  return StringID("MeshCodec").getUID();
}

bool
MeshCodec::canDecodeExtension(const String& extension) const
{
  Vector<String> supportedExtensions = getSupportedExtensions();
  for (const String& suppExt : supportedExtensions) {
    if (extension.compare(suppExt) == 0) {
      return true;
    }
  }
  return false;
}

Vector<String>
MeshCodec::getSupportedExtensions() const
{
  return { ".fbx", ".obj", ".gltf", ".glb" };
}

bool
MeshCodec::decode(const Path& filePath) const
{
  return false;
}

bool
MeshCodec::encode(const Path& filePath) const
{
  return false;
}
}