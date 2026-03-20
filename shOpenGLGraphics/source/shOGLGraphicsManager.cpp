/*************************************************************/
/*
*  @file    shOGLGraphicsManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2024/10/29
*  @brief   Graphics Manager for Open GL.
*
*  Graphics Manager for Open GL.
*
*  @bug     No bug known.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shOGLGraphicsManager.h"
#include "shScreen.h"
#include "shException.h"

using std::reinterpret_pointer_cast;

#if SH_PLATFORM == SH_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#endif

#if SH_PLATFORM == SH_PLATFORM_LINUX
#include "X11/Xlib.h"
#endif

// TODO: Delete this, make a wrapper.
#include "fstream"
#include "sstream"


namespace shEngineSDK {
using std::fstream;
using std::ios;
using std::stringstream;

namespace OPENGL_SHADER_TYPE {
enum E
{
  VERTEX_SHADER = GL_VERTEX_SHADER,
  PIXEL_SHADER = GL_FRAGMENT_SHADER,
  GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
  COMPUTE_SHADER = GL_COMPUTE_SHADER
};
}

String
resolveIncludes(const String& shaderCode, const String& basePath)
{
  String resolvedCode;
  size_t pos = 0;
  while (pos < shaderCode.length()) {
    size_t includePos = shaderCode.find("#include", pos);
    if (includePos == String::npos) {
      resolvedCode += shaderCode.substr(pos);
      break;
    }
    resolvedCode += shaderCode.substr(pos, includePos - pos);
    size_t startQuote = shaderCode.find('"', includePos);
    size_t endQuote = shaderCode.find('"', startQuote + 1);
    if (startQuote == String::npos || endQuote == String::npos) {
      SH_ASSERT(false && "Invalid #include directive");
      return resolvedCode;
    }
    String includeFileName = shaderCode.substr(startQuote + 1, endQuote - startQuote - 1);
    String includeFilePath = basePath + "/" + includeFileName;
    fstream includeFile(includeFilePath, ios::in);
    if (!includeFile.is_open()) {
      SH_ASSERT(false && "Failed to open included shader file");
      return resolvedCode;
    }
    stringstream includeStream;
    includeStream << includeFile.rdbuf();
    String includeShaderCode = resolveIncludes(includeStream.str(), basePath);
    resolvedCode += includeShaderCode;
    pos = endQuote + 1;
  }
  return resolvedCode;
}

uint32
compileShader(const String& fileName,
              const OPENGL_SHADER_TYPE::E shaderType,
              const Vector<ShaderMacro>& macros)
{
  // Read shader code from file.
  fstream shaderFile(fileName, ios::in);
  if (!shaderFile.is_open()) {
    SH_ASSERT(false && "Failed to open shader file");
    return 0;
  }

  stringstream shaderStream;
  shaderStream << shaderFile.rdbuf();
  String shaderCode = shaderStream.str();

  // Resolve #include directives.
  String basePath = fileName.substr(0, fileName.find_last_of("/\\"));
  shaderCode = resolveIncludes(shaderCode, basePath);

  // Insert macros into shader code.
  String macroDefinitions;
  for (const auto& macro : macros) {
    macroDefinitions += "#define " + macro.name + " " + macro.definition + "\n";
  }

  SIZE_T macroInsertPos = shaderCode.find("#version");
  if (macroInsertPos != String::npos) {
    SIZE_T lineEndPos = shaderCode.find('\n', macroInsertPos);
    shaderCode.insert(lineEndPos + 1, macroDefinitions);
  }
  else {
    shaderCode = macroDefinitions + shaderCode;
  }

  const char* shaderCodeCStr = shaderCode.c_str();

  // Create shader object.
  GLenum glShaderType = cast::st<GLenum>(shaderType);
  uint32 shader = glCreateShader(glShaderType);
  glShaderSource(shader, 1, &shaderCodeCStr, nullptr);
  glCompileShader(shader);

  // Check for compilation errors.
  int32 success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    SH_ASSERT(false && "Shader compilation failed");
    //SH_LOG("Shader compilation error in {}: {}", fileName, infoLog);
    glDeleteShader(shader);

    return 0;
  }

  return shader;
}

void
enableOpenGL(const WPtr<Screen> screen, const bool bAntiliasing)
{
#if SH_PLATFORM == SH_PLATFORM_WIN32
  
  HWND hWnd = cast::re<HWND>(screen.lock()->getPlatformHandler());
  HDC hDC = GetDC(hWnd);

  // Set the pixel format for the DC
  PIXELFORMATDESCRIPTOR pfd = {};
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 24;
  pfd.cStencilBits = 8;

  int32 pixelFormat = ChoosePixelFormat(hDC, &pfd);
  if (pixelFormat == 0) {
    SH_ASSERT(false && "Failed to choose pixel format");
    return;
  }
  SetPixelFormat(hDC, pixelFormat, &pfd);

  // Create an OpenGL rendering context
  HGLRC hRC = wglCreateContext(hDC);
  if (hRC == nullptr) {
    SH_ASSERT(false && "Failed to create OpenGL context");
    return;
  }
  wglMakeCurrent(hDC, hRC);

#elif SH_PLATFORM == SH_PLATFORM_LINUX
  /*Display* display = XOpenDisplay(NULL);
  if (display == nullptr) {
    SH_ASSERT(false && "Failed to open X display");
    return;
  }*/

#endif

  // Initialize GLEW
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)  {
    SH_ASSERT(false && "Failed to initialize GLEW");
    return;
  }
}

OGLGraphicsManager::~OGLGraphicsManager()
{

}

void
OGLGraphicsManager::initManager(const WPtr<Screen> screen,
                                const bool bAntiliasing,
                                const SampleDesc& sample)
{
  if (screen.expired()) {
    SH_ASSERT(false && "Screen expired!");
    return;
  }

  auto pScreen = screen.lock();

  m_graphicAPI = GRAPHIC_API::kOGL;
  m_bFullScreen = pScreen->isFullscreen();

  enableOpenGL(screen, bAntiliasing);

  m_multiSampleConfig.count = 1;
  m_multiSampleConfig.quality = 0;

  if (bAntiliasing) {
    m_multiSampleConfig = sample;
    glEnable(GL_MULTISAMPLE);
  }

  m_defaultFBO = 0;

  glGenFramebuffers(1, &m_backBufferFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, m_backBufferFBO);

  uint32 width = pScreen->getClientSize().x;
  uint32 height = pScreen->getClientSize().y;

  auto pBackbuffer = sh_makeShared<OGLTexture2D>();

  glGenTextures(1, &pBackbuffer->m_texture);
  glBindTexture(GL_TEXTURE_2D, pBackbuffer->m_texture);

  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGBA8,
               width,
               height,
               0,
               GL_RGBA,
               GL_UNSIGNED_BYTE,
               nullptr);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glFramebufferTexture2D(GL_FRAMEBUFFER,
                         GL_COLOR_ATTACHMENT0,
                         GL_TEXTURE_2D,
                         pBackbuffer->m_texture,
                         0);

  // Create depth stencil buffer
  glGenRenderbuffers(1, &m_depthStencilRBO);
  glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilRBO);

  glRenderbufferStorage(GL_RENDERBUFFER,
                        GL_DEPTH24_STENCIL8,
                        width,
                        height);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                            GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER,
                            m_depthStencilRBO);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    SH_ASSERT(false && "Framebuffer not complete!");
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glViewport(0, 0, width, height);

  Viewport viewPort;
  viewPort.width = static_cast<float>(width);
  viewPort.height = static_cast<float>(height);
  viewPort.minDepth = 0.0f;
  viewPort.maxDepth = 1.0f;
  viewPort.topLeftX = 0.0f;
  viewPort.topLeftY = 0.0f;

  setViewport(viewPort);

  m_pBackBuffer = pBackbuffer;
  pBackbuffer.reset();

#if defined(SH_DEBUG_MODE)
  m_debugEnabled = true;

  if (glDebugMessageCallback) {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glDebugMessageCallback([](GLenum source,
      GLenum type,
      GLuint id,
      GLenum severity,
      GLsizei length,
      const GLchar* message,
      const void* userParam)
      {
        //SH_LOG("OpenGL Debug: {}", message);
      }, nullptr);
  }
#endif
}

void
OGLGraphicsManager::clearRenderTarget(const WPtr<Texture2D> pTarget,
                                      const LinearColor& color)
{

}

void
OGLGraphicsManager::clearDepthStencil(const WPtr<Texture2D> pDepthSV,
                                      const uint32 flags,
                                      const float depth,
                                      const uint8 stencil)
{
}

void
OGLGraphicsManager::present(uint32 syncInterval, uint32 flags)
{
}

void OGLGraphicsManager::unbindAll()
{
}

void
OGLGraphicsManager::deleteProgram(uint32 programID)
{
  glDeleteProgram(programID);
}

WPtr<Texture2D>
OGLGraphicsManager::getMainRenderTargetView() const
{
  return m_pBackBuffer;
}

WPtr<Texture2D>
OGLGraphicsManager::getMainDepthStencil() const
{
  return m_pDepthStencil;
}

SPtr<InputLayout>
OGLGraphicsManager::createInputLayout(const Vector<InputDesc>& desc,
                                      const WPtr<VertexShader> pVShader)
{
  return SPtr<InputLayout>();
}

SPtr<InputLayout>
OGLGraphicsManager::createInputLayoutFromShader(const WPtr<VertexShader> pPShader)
{
  return SPtr<InputLayout>();
}

SPtr<VertexShader>
OGLGraphicsManager::createVertexShader(const String& fileName,
                                       const String& entryPoint,
                                       const String& shaderModel,
                                       const Vector<ShaderMacro>& macros)
{
  SH_UNREFERENCED_PARAMETER(entryPoint);
  SH_UNREFERENCED_PARAMETER(shaderModel);

  uint32 shaderID = compileShader(fileName, OPENGL_SHADER_TYPE::VERTEX_SHADER, macros);
  auto pVertexShader = sh_makeShared<OGLVertexShader>();
  pVertexShader->m_vertexShaderID = shaderID;

  return pVertexShader;
}

SPtr<PixelShader>
OGLGraphicsManager::createPixelShader(const String& fileName,
                                      const String& entryPoint,
                                      const String& shaderModel,
                                      const Vector<ShaderMacro>& macros)
{
  SH_UNREFERENCED_PARAMETER(entryPoint);
  SH_UNREFERENCED_PARAMETER(shaderModel);

  uint32 shaderID = compileShader(fileName, OPENGL_SHADER_TYPE::PIXEL_SHADER, macros);
  auto pPixelShader = sh_makeShared<OGLPixelShader>();
  pPixelShader->m_pixelShaderID = shaderID;

  return pPixelShader;
}

SPtr<GeometryShader>
OGLGraphicsManager::createGeometryShader(const String& fileName,
                                         const String& entryPoint,
                                         const String& shaderModel,
                                         const Vector<ShaderMacro>& macros)
{
  SH_UNREFERENCED_PARAMETER(entryPoint);
  SH_UNREFERENCED_PARAMETER(shaderModel);

  uint32 shaderID = compileShader(fileName, OPENGL_SHADER_TYPE::GEOMETRY_SHADER, macros);
  auto pGeometryShader = sh_makeShared<OGLGeometryShader>();
  pGeometryShader->m_geometryShaderID = shaderID;
  return pGeometryShader;
}

SPtr<ComputeShader>
OGLGraphicsManager::createComputeShader(const String& fileName,
                                        const String& entryPoint,
                                        const String& shaderModel,
                                        const Vector<ShaderMacro>& macros)
{
  SH_UNREFERENCED_PARAMETER(entryPoint);
  SH_UNREFERENCED_PARAMETER(shaderModel);

  uint32 shaderID = compileShader(fileName, OPENGL_SHADER_TYPE::COMPUTE_SHADER, macros);
  auto pComputeShader = sh_makeShared<OGLComputeShader>();
  pComputeShader->m_computeShaderID = shaderID;
  return pComputeShader;
}

SPtr<VertexBuffer>
OGLGraphicsManager::internalCreateVertexBuffer(const void* pData,
                                               const uint32 bufferSize,
                                               const uint32 stride,
                                               const uint32 usage)
{
  return SPtr<VertexBuffer>();
}

SPtr<IndexBuffer>
OGLGraphicsManager::createIndexBuffer(const Vector<uint32>& indices, const uint32 usage)
{
  return SPtr<IndexBuffer>();
}

SPtr<ConstantBuffer>
OGLGraphicsManager::createConstantBuffer(const uint32 bufferSize,
                                         const uint32 usage,
                                         const void* pData)
{
  return SPtr<ConstantBuffer>();
}

SPtr<SamplerState>
OGLGraphicsManager::createSamplerState(const uint32 filter, const uint32 textAddress)
{
  return SPtr<SamplerState>();
}

SPtr<Texture2D>
OGLGraphicsManager::createTextureFromFile(const String& fileName,
                                          const void* pData,
                                          const uint32 width,
                                          const uint32 height,
                                          const uint32 bpp)
{
  return SPtr<Texture2D>();
}

SPtr<Texture2D> OGLGraphicsManager::createTextureFromDDS(const String& fileName)
{
  return SPtr<Texture2D>();
}

SPtr<Texture2D>
OGLGraphicsManager::createTexture2D(const uint32 width,
                                    const uint32 height,
                                    const uint32 format,
                                    const uint32 usage,
                                    const uint32 bindFlags,
                                    const uint32 mipLevels,
                                    const uint32 arraySize)
{
  return SPtr<Texture2D>();
}

SPtr<Texture3D>
OGLGraphicsManager::createTexture3D(const Vector3 size,
                                    const uint32 format,
                                    const Vector<LinearColor>* data,
                                    const uint32 usage)
{
  return SPtr<Texture3D>();
}

SPtr<Texture2D>
OGLGraphicsManager::createErrorTexture()
{
  return SPtr<Texture2D>();
}

SPtr<Texture2D>
OGLGraphicsManager::createDefaultNormalTexture()
{
  return SPtr<Texture2D>();
}

SPtr<Texture2D>
OGLGraphicsManager::createBlackTexture()
{
  return SPtr<Texture2D>();
}

SPtr<BlendState>
OGLGraphicsManager::createBlendState(const BlendDesc& blendDesc,
                                     const LinearColor& blendFactor)
{
  return SPtr<BlendState>();
}

SPtr<RasterizerState>
OGLGraphicsManager::createRasterizerState(const RasterizerDesc& rasterizerDesc)
{
  return SPtr<RasterizerState>();
}

SPtr<DepthStencilState>
OGLGraphicsManager::createDepthStencilState(const DepthStencilDesc& depthSDesc)
{
  return SPtr<DepthStencilState>();
}

uint32
OGLGraphicsManager::createProgram(const WPtr<VertexShader>& pVShader,
                                  const WPtr<PixelShader>& pPShader,
                                  const WPtr<GeometryShader>& pGShader)
{
  if (pVShader.expired() || pPShader.expired() || pGShader.expired()) {
    SH_ASSERT(false && "Vertex and Pixel shaders are required to create a program");
    return 0;
  }

  auto pVertexShader = cast::re_ptr<OGLVertexShader>(pVShader.lock());
  auto pPixelShader = cast::re_ptr<OGLPixelShader>(pPShader.lock());
  auto pGeometryShader = cast::re_ptr<OGLGeometryShader>(pGShader.lock());

  uint32 program = glCreateProgram();
  glAttachShader(program, pVertexShader->m_vertexShaderID);
  glAttachShader(program, pPixelShader->m_pixelShaderID);
  glAttachShader(program, pGeometryShader->m_geometryShaderID);
 
  glLinkProgram(program);
 
  int32 success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(program, 512, nullptr, infoLog);
    SH_ASSERT(false && "Program linking failed");
    //SH_LOG("Program linking error: {}", infoLog);
    glDeleteProgram(program);
    return 0;
  }
 
  return program;
}

uint32
OGLGraphicsManager::createProgram(const WPtr<VertexShader>& pVShader,
                                  const WPtr<PixelShader>& pPShader)
{
  if (pVShader.expired() || pPShader.expired()) {
    SH_ASSERT(false && "Vertex and Pixel shaders are required to create a program");
    return 0;
  }

  auto pVertexShader = cast::re_ptr<OGLVertexShader>(pVShader.lock());
  auto pPixelShader = cast::re_ptr<OGLPixelShader>(pPShader.lock());

  uint32 program = glCreateProgram();
  glAttachShader(program, pVertexShader->m_vertexShaderID);
  glAttachShader(program, pPixelShader->m_pixelShaderID);

  glLinkProgram(program);

  int32 success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(program, 512, nullptr, infoLog);
    SH_ASSERT(false && "Program linking failed");
    //SH_LOG("Program linking error: {}", infoLog);
    glDeleteProgram(program);
    return 0;
  }

  return program;
}

uint32
OGLGraphicsManager::createProgram(const WPtr<ComputeShader>& pCShader)
{
  if (pCShader.expired()) {
    SH_ASSERT(false && "Vertex and Pixel shaders are required to create a program");
    return 0;
  }

  auto pComputeShader = cast::re_ptr<OGLComputeShader>(pCShader.lock());

  uint32 program = glCreateProgram();
  glAttachShader(program, pComputeShader->m_computeShaderID);
  glLinkProgram(program);
  int32 success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(program, 512, nullptr, infoLog);
    SH_ASSERT(false && "Program linking failed");
    //SH_LOG("Program linking error: {}", infoLog);
    glDeleteProgram(program);
    return 0;
  }

  return program;
}

void
OGLGraphicsManager::generateMips(const WPtr<Texture2D> pTexture)
{
}

void
OGLGraphicsManager::updateConstantBuffer(const WPtr<ConstantBuffer> pCBuffer,
                                         const void* pData,
                                         const uint32 dataSize)
{
}

void
OGLGraphicsManager::updateTexture2D(WPtr<Texture2D> pTexture,
                                    uint8* pData,
                                    uint32 width,
                                    uint32 bpp)
{
}

void
OGLGraphicsManager::updateScreenSize(const Vector2& size)
{
}
void
OGLGraphicsManager::updateVertexBuffer(const WPtr<VertexBuffer> pVBuffer,
                                       const void* pData,
                                       const uint32 dataSize)
{
}

void
OGLGraphicsManager::updateIndexBuffer(const WPtr<IndexBuffer> pIBuffer,
                                      const void* pData,
                                      const uint32 dataSize)
{
}

void
OGLGraphicsManager::saveTextureToDDS(const WPtr<Texture2D> pTexture,
                                     const String& filePath)
{
}

void
OGLGraphicsManager::setViewport(const Viewport& vp)
{
}

void
OGLGraphicsManager::setRenderTargets(const Vector<RenderTarget>& pRenderTVs,
                                     const WPtr<Texture2D> pDepthSV)
{
}

void
OGLGraphicsManager::setInputLayout(const WPtr<InputLayout> pInput)
{
}

void
OGLGraphicsManager::setVertexBuffers(const WPtr<VertexBuffer> pVBuffer,
                                     const uint32 startSlot,
                                     const uint32 numBuffers,
                                     const uint32 offset)
{
}

void
OGLGraphicsManager::setIndexBuffers(const WPtr<IndexBuffer> pIBuffer, const uint32 offset)
{
}

void
OGLGraphicsManager::vsSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                                         const uint32 startSlot,
                                         const uint32 numBuffers)
{
}

void
OGLGraphicsManager::psSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                                         const uint32 startSlot,
                                         const uint32 numBuffers)
{
}

void
OGLGraphicsManager::gsSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                                         const uint32 startSlot,
                                         const uint32 numBuffers)
{
}

void
OGLGraphicsManager::csSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                                         const uint32 startSlot,
                                         const uint32 numBuffers)
{
}

void
OGLGraphicsManager::setPrimitiveTopology(const uint32 primitive)
{
}

void
OGLGraphicsManager::setVertexShader(const WPtr<VertexShader> pVShader,
                                    const void* ppClassInstances,
                                    const uint32 numClassInstances)
{
}

void
OGLGraphicsManager::setPixelShader(const WPtr<PixelShader> pPShader,
                                   const void* ppClassInstances,
                                   const uint32 numClassInstances)
{
}

void
OGLGraphicsManager::setGeometryShader(const WPtr<GeometryShader> pGShader,
                                      const void* ppClassInstances,
                                      const uint32 numClassInstances)
{
}

void
OGLGraphicsManager::setComputeShader(const WPtr<ComputeShader> pCShader,
                                     const void* ppClassInstances,
                                     const uint32 numClassInstances)
{
}

void
OGLGraphicsManager::psSetShaderResourceView(const WPtr<Texture2D> pShaderRV,
                                            const uint32 startSlot,
                                            const uint32 numViews)
{
}

void
OGLGraphicsManager::csSetShaderResourceView(const WPtr<Texture2D> pShaderRV,
                                            const uint32 startSlot,
                                            const uint32 numViews)
{
}

void
OGLGraphicsManager::psSetShaderResourceView(const WPtr<Texture3D> pShaderRV,
                                            const uint32 startSlot,
                                            const uint32 numViews)
{
}

void
OGLGraphicsManager::csSetShaderResourceView(const WPtr<Texture3D> pShaderRV,
                                            const uint32 startSlot,
                                            const uint32 numViews)
{
}

void
OGLGraphicsManager::setUnorderedAccessView(const UnorderedAccess& pUAVs,
                                           const uint32 startSlot)
{
}

void
OGLGraphicsManager::psSetSamplerState(const WPtr<SamplerState> pSamplerLinear,
                                      const uint32 startSlot,
                                      const uint32 numSamplers)
{
}

void
OGLGraphicsManager::csSetSamplerState(const WPtr<SamplerState> pSamplerLinear,
                                      const uint32 startSlot,
                                      const uint32 numSamplers)
{
}

void
OGLGraphicsManager::setBlendState(const WPtr<BlendState> pBlendState)
{
}

void
OGLGraphicsManager::setRasterizerState(const WPtr<RasterizerState> pRasterizerState)
{
}

void
OGLGraphicsManager::setDepthStencilState(const WPtr<DepthStencilState> pDepthSState,
                                         const uint8 stencilRef)
{
}

void
OGLGraphicsManager::setScissorRects(const Rect& scissorClip)
{
}

void
OGLGraphicsManager::useProgram(uint32 programID)
{
  glUseProgram(programID);
}

void
OGLGraphicsManager::draw(const uint32 vertexCount, const uint32 startVertexLocation)
{
}

void
OGLGraphicsManager::drawIndexed(const uint32 indexCount,
                                const uint32 startIndexLocation,
                                const uint32 baseVertexLocation)
{
}

void
OGLGraphicsManager::dispatch(const uint32 threadGroupCountX,
                             const uint32 threadGroupCountY,
                             const uint32 threadGroupCountZ)
{
}
}
