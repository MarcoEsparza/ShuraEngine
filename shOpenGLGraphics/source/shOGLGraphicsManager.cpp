/*************************************************************/
/*
*  @file    shOGLGraphicsManager.h
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2026/03/23
*  @brief   Graphics Manager for Open GL.
*
*  Graphics Manager for Open GL.
*
*  @bug     Not currently working.
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
#include <shMath.h>

using std::reinterpret_pointer_cast;

#if SH_PLATFORM == SH_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#elif SH_PLATFORM == SH_PLATFORM_LINUX
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
  SIZE_T pos = 0;
  while (pos < shaderCode.length()) {
    SIZE_T includePos = shaderCode.find("#include", pos);
    if (includePos == String::npos) {
      resolvedCode += shaderCode.substr(pos);
      break;
    }
    resolvedCode += shaderCode.substr(pos, includePos - pos);
    SIZE_T startQuote = shaderCode.find('"', includePos);
    SIZE_T endQuote = shaderCode.find('"', startQuote + 1);
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
    StringStream includeStream;
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
  SystemPath path(fileName);
  path.replace_extension(".glsl");
  String newFileName = path.string();
  fstream shaderFile(newFileName, ios::in);
  if (!shaderFile.is_open()) {
    SH_ASSERT(false && "Failed to open shader file");
    return 0;
  }

  stringstream shaderStream;
  shaderStream << shaderFile.rdbuf();
  String shaderCode = shaderStream.str();

  // Resolve #include directives.
  String basePath = newFileName.substr(0, newFileName.find_last_of("/\\"));
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
enableOpenGL(const WPtr<Screen> screen)
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
  
  auto handle = screen.lock()->getPlatformHandler();
  Display* display = handle.display;
  Window window = handle.window;
  
  int32 fbAttribs[] = {
    GLX_X_RENDERABLE, True,
    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
    GLX_RENDER_TYPE, GLX_RGBA_BIT,
    GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
    GLX_RED_SIZE, 8,
    GLX_GREEN_SIZE, 8,
    GLX_BLUE_SIZE, 8,
    GLX_ALPHA_SIZE, 8,
    GLX_DEPTH_SIZE, 24,
    GLX_STENCIL_SIZE, 8,
    None
  };

  int32 fbCount;
  GLXFBConfig* fbConfigs = glXChooseFBConfig(display, DefaultScreen(display),
                                             fbAttribs, &fbCount);
  if (fbConfigs == nullptr || fbCount == 0) {
    SH_ASSERT(false && "Failed to choose FB config");
    return;
  }

  XVisualInfo* vi = glXGetVisualFromFBConfig(display, fbConfigs[0]);

  GLXContext context = glXCreateContext(display, vi, nullptr, GL_TRUE);
  if (context == nullptr) {
    SH_ASSERT(false && "Failed to create OpenGL context");
    return;
  }

  glXMakeCurrent(display, window, context);

  XFree(vi);
  XFree(fbConfigs);

#endif

  // Initialize GLEW
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)  {
    SH_ASSERT(false && "Failed to initialize GLEW");
    return;
  }
}

GLenum
translateBlend(uint32 blend)
{
  switch (blend)
  {
  case BLEND::kZero: return GL_ZERO;
  case BLEND::kOne: return GL_ONE;
  case BLEND::kSrcColor: return GL_SRC_COLOR;
  case BLEND::kInvSrcColor: return GL_ONE_MINUS_SRC_COLOR;
  case BLEND::kSrcAlpha: return GL_SRC_ALPHA;
  case BLEND::kInvSrcAlpha: return GL_ONE_MINUS_SRC_ALPHA;
  case BLEND::kDestAlpha: return GL_DST_ALPHA;
  case BLEND::kInvDestAlpha: return GL_ONE_MINUS_DST_ALPHA;
  case BLEND::kDestColor: return GL_DST_COLOR;
  case BLEND::kInvDestColor: return GL_ONE_MINUS_DST_COLOR;
  case BLEND::kSrcAlphaSat: return GL_SRC_ALPHA_SATURATE;
  case BLEND::kBlendFactor: return GL_CONSTANT_COLOR;
  case BLEND::kInvBlendFactor: return GL_ONE_MINUS_CONSTANT_COLOR;
  default: return GL_ONE;
  }
}

GLenum
translateBlendOp(uint32 op)
{
  switch (op)
  {
  case BLEND_OP::kAdd: return GL_FUNC_ADD;
  case BLEND_OP::kSubtract: return GL_FUNC_SUBTRACT;
  case BLEND_OP::kRevSubtract: return GL_FUNC_REVERSE_SUBTRACT;
  case BLEND_OP::kMin: return GL_MIN;
  case BLEND_OP::kMax: return GL_MAX;
  default: return GL_FUNC_ADD;
  }
}

GLenum
translateComparison(uint32 func)
{
  switch (func)
  {
  case COMPARISON_FUNC::kNever: return GL_NEVER;
  case COMPARISON_FUNC::kLess: return GL_LESS;
  case COMPARISON_FUNC::kEqual: return GL_EQUAL;
  case COMPARISON_FUNC::kLessEqual: return GL_LEQUAL;
  case COMPARISON_FUNC::kGreater: return GL_GREATER;
  case COMPARISON_FUNC::kNotEqual: return GL_NOTEQUAL;
  case COMPARISON_FUNC::kGreaterEqual: return GL_GEQUAL;
  case COMPARISON_FUNC::kAlways: return GL_ALWAYS;
  default: return GL_LESS;
  }
}

GLenum
translateStencilOp(uint32 op)
{
  switch (op)
  {
  case STENCIL_OP::kKeep: return GL_KEEP;
  case STENCIL_OP::kZero: return GL_ZERO;
  case STENCIL_OP::kReplace: return GL_REPLACE;
  case STENCIL_OP::kIncr: return GL_INCR;
  case STENCIL_OP::kIncrSat: return GL_INCR_WRAP;
  case STENCIL_OP::kDecr: return GL_DECR;
  case STENCIL_OP::kDecrSat: return GL_DECR_WRAP;
  case STENCIL_OP::kInvert: return GL_INVERT;
  default: return GL_KEEP;
  }
}

GLenum
translateFillMode(uint32 mode)
{
  switch (mode)
  {
  case FILL_MODE::kSolid: return GL_FILL;
  case FILL_MODE::kWireframe: return GL_LINE;
  default: return GL_FILL;
  }
}

GLenum
translateCullMode(uint32 mode)
{
  switch (mode)
  {
  case CULL_MODE::kNone: return GL_NONE;
  case CULL_MODE::kFront: return GL_FRONT;
  case CULL_MODE::kBack: return GL_BACK;
  default: return GL_BACK;
  }
}

GLenum
translateInternalTextureFormat(uint32 format)
{
  switch (format)
  {
  case TEXTURE_FORMAT::kR32G32B32A32_FLOAT: return GL_RGBA32F;
  case TEXTURE_FORMAT::kR32G32B32A32_UINT:  return GL_RGBA32UI;
  case TEXTURE_FORMAT::kR32G32B32A32_SINT:  return GL_RGBA32I;

  case TEXTURE_FORMAT::kR32G32B32_FLOAT: return GL_RGB32F;
  case TEXTURE_FORMAT::kR32G32B32_UINT:  return GL_RGB32UI;
  case TEXTURE_FORMAT::kR32G32B32_SINT:  return GL_RGB32I;

  case TEXTURE_FORMAT::kR16G16B16A16_FLOAT: return GL_RGBA16F;
  case TEXTURE_FORMAT::kR16G16B16A16_UNORM: return GL_RGBA16;
  case TEXTURE_FORMAT::kR16G16B16A16_UINT:  return GL_RGBA16UI;
  case TEXTURE_FORMAT::kR16G16B16A16_SNORM: return GL_RGBA16_SNORM;
  case TEXTURE_FORMAT::kR16G16B16A16_SINT:  return GL_RGBA16I;

  case TEXTURE_FORMAT::kR32G32_FLOAT: return GL_RG32F;
  case TEXTURE_FORMAT::kR32G32_UINT:  return GL_RG32UI;
  case TEXTURE_FORMAT::kR32G32_SINT:  return GL_RG32I;

  case TEXTURE_FORMAT::kR10G10B10A2_UNORM: return GL_RGB10_A2;
  case TEXTURE_FORMAT::kR10G10B10A2_UINT:  return GL_RGB10_A2UI;

  case TEXTURE_FORMAT::kR11G11B10_FLOAT: return GL_R11F_G11F_B10F;

  case TEXTURE_FORMAT::kR8G8B8A8_UNORM:      return GL_RGBA8;
  case TEXTURE_FORMAT::kR8G8B8A8_UNORM_SRGB: return GL_SRGB8_ALPHA8;
  case TEXTURE_FORMAT::kR8G8B8A8_UINT:       return GL_RGBA8UI;
  case TEXTURE_FORMAT::kR8G8B8A8_SNORM:      return GL_RGBA8_SNORM;
  case TEXTURE_FORMAT::kR8G8B8A8_SINT:       return GL_RGBA8I;

  case TEXTURE_FORMAT::kR16G16_FLOAT: return GL_RG16F;
  case TEXTURE_FORMAT::kR16G16_UNORM: return GL_RG16;
  case TEXTURE_FORMAT::kR16G16_UINT:  return GL_RG16UI;
  case TEXTURE_FORMAT::kR16G16_SNORM: return GL_RG16_SNORM;
  case TEXTURE_FORMAT::kR16G16_SINT:  return GL_RG16I;

  case TEXTURE_FORMAT::kR32_FLOAT: return GL_R32F;
  case TEXTURE_FORMAT::kR32_UINT:  return GL_R32UI;
  case TEXTURE_FORMAT::kR32_SINT:  return GL_R32I;

  case TEXTURE_FORMAT::kR8G8_UNORM: return GL_RG8;
  case TEXTURE_FORMAT::kR8G8_UINT:  return GL_RG8UI;
  case TEXTURE_FORMAT::kR8G8_SNORM: return GL_RG8_SNORM;
  case TEXTURE_FORMAT::kR8G8_SINT:  return GL_RG8I;

  case TEXTURE_FORMAT::kR16_FLOAT: return GL_R16F;
  case TEXTURE_FORMAT::kR16_UNORM: return GL_R16;
  case TEXTURE_FORMAT::kR16_UINT:  return GL_R16UI;
  case TEXTURE_FORMAT::kR16_SNORM: return GL_R16_SNORM;
  case TEXTURE_FORMAT::kR16_SINT:  return GL_R16I;

  case TEXTURE_FORMAT::kD32_FLOAT:       return GL_DEPTH_COMPONENT32F;
  case TEXTURE_FORMAT::kD24_UNORM_S8_UINT: return GL_DEPTH24_STENCIL8;
  case TEXTURE_FORMAT::kD16_UNORM:       return GL_DEPTH_COMPONENT16;

  default: return GL_RGBA8;
  }
}

GLenum
translateTextureFormat(uint32 format)
{
  switch (format)
  {
  case TEXTURE_FORMAT::kR32G32B32A32_FLOAT:
  case TEXTURE_FORMAT::kR32G32B32A32_UINT:
  case TEXTURE_FORMAT::kR32G32B32A32_SINT:
  case TEXTURE_FORMAT::kR16G16B16A16_FLOAT:
  case TEXTURE_FORMAT::kR16G16B16A16_UNORM:
  case TEXTURE_FORMAT::kR16G16B16A16_UINT:
  case TEXTURE_FORMAT::kR16G16B16A16_SNORM:
  case TEXTURE_FORMAT::kR16G16B16A16_SINT:
  case TEXTURE_FORMAT::kR8G8B8A8_UNORM:
  case TEXTURE_FORMAT::kR8G8B8A8_UNORM_SRGB:
    return GL_RGBA;

  case TEXTURE_FORMAT::kR32G32B32_FLOAT:
    return GL_RGB;

  case TEXTURE_FORMAT::kR32G32_FLOAT:
  case TEXTURE_FORMAT::kR16G16_FLOAT:
  case TEXTURE_FORMAT::kR8G8_UNORM:
    return GL_RG;

  case TEXTURE_FORMAT::kR32_FLOAT:
  case TEXTURE_FORMAT::kR16_FLOAT:
  case TEXTURE_FORMAT::kR16_UNORM:
    return GL_RED;

  case TEXTURE_FORMAT::kD32_FLOAT:
  case TEXTURE_FORMAT::kD16_UNORM:
    return GL_DEPTH_COMPONENT;

  case TEXTURE_FORMAT::kD24_UNORM_S8_UINT:
    return GL_DEPTH_STENCIL;

  default:
    return GL_RGBA;
  }
}

GLenum
translateTextureType(uint32 format)
{
  switch (format)
  {
  case TEXTURE_FORMAT::kR32G32B32A32_FLOAT:
  case TEXTURE_FORMAT::kR32G32B32_FLOAT:
  case TEXTURE_FORMAT::kR32G32_FLOAT:
  case TEXTURE_FORMAT::kR32_FLOAT:
    return GL_FLOAT;

  case TEXTURE_FORMAT::kR16G16B16A16_FLOAT:
  case TEXTURE_FORMAT::kR16G16_FLOAT:
  case TEXTURE_FORMAT::kR16_FLOAT:
    return GL_HALF_FLOAT;

  case TEXTURE_FORMAT::kR8G8B8A8_UNORM:
  case TEXTURE_FORMAT::kR8G8_UNORM:
    return GL_UNSIGNED_BYTE;

  case TEXTURE_FORMAT::kR16_UNORM:
    return GL_UNSIGNED_SHORT;

  case TEXTURE_FORMAT::kR32_UINT:
  case TEXTURE_FORMAT::kR32G32_UINT:
  case TEXTURE_FORMAT::kR32G32B32A32_UINT:
    return GL_UNSIGNED_INT;

  case TEXTURE_FORMAT::kR32_SINT:
  case TEXTURE_FORMAT::kR32G32_SINT:
  case TEXTURE_FORMAT::kR32G32B32A32_SINT:
    return GL_INT;

  case TEXTURE_FORMAT::kD24_UNORM_S8_UINT:
    return GL_UNSIGNED_INT_24_8;

  default:
    return GL_UNSIGNED_BYTE;
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

  enableOpenGL(screen);

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

  glGenTextures(1, &pBackbuffer->m_textureID);
  glBindTexture(GL_TEXTURE_2D, pBackbuffer->m_textureID);

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
                         pBackbuffer->m_textureID,
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

  //glViewport(0, 0, width, height);
  m_frameBufferHeight = height;

  Viewport viewPort;
  viewPort.width = cast::st<float>(width);
  viewPort.height = cast::st<float>(height);
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
  if (pTarget.expired()) {
    SH_ASSERT(false && "Render target expired!");
    return;
  }

  auto pTexture = cast::re_ptr<OGLTexture2D>(pTarget.lock());
  glBindFramebuffer(GL_FRAMEBUFFER, pTexture->m_textureID);
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void
OGLGraphicsManager::clearDepthStencil(const WPtr<Texture2D> pDepthSV,
                                      const uint32 flags,
                                      const float depth,
                                      const uint8 stencil)
{
  if (pDepthSV.expired()) {
    SH_ASSERT(false && "Depth stencil view expired!");
    return;
  }

  auto pTexture = cast::re_ptr<OGLTexture2D>(pDepthSV.lock());

  glBindFramebuffer(GL_FRAMEBUFFER, pTexture->m_textureID);

  GLbitfield glFlags = 0;

  if (flags & CLEAR_FLAGS::kDepth) {
    glFlags |= GL_DEPTH_BUFFER_BIT;
    glClearDepth(depth);
  }

  if (flags & CLEAR_FLAGS::kStencil) {
    glFlags |= GL_STENCIL_BUFFER_BIT;
    glClearStencil(stencil);
  }

  glClear(glFlags);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void
OGLGraphicsManager::present(uint32 syncInterval, uint32 flags)
{
  SH_UNREFERENCED_PARAMETER(syncInterval);
  SH_UNREFERENCED_PARAMETER(flags);

  auto pBackBuffer = cast::re_ptr<OGLTexture2D>(m_pBackBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, pBackBuffer->m_textureID);
}

void
OGLGraphicsManager::unbindAll()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  
  for (uint32 i = 0; i < 16; ++i) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  glUseProgram(0);
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
  SH_UNREFERENCED_PARAMETER(desc);
  SH_UNREFERENCED_PARAMETER(pVShader);
  return SPtr<InputLayout>();
}

SPtr<InputLayout>
OGLGraphicsManager::createInputLayoutFromShader(const WPtr<VertexShader> pPShader)
{
  SH_UNREFERENCED_PARAMETER(pPShader);
  return SPtr<InputLayout>();
}

SPtr<InputLayout>
OGLGraphicsManager::createInputLayoutFromVertexBuffer(const Vector<InputDesc>& desc,
                                                      const WPtr<VertexBuffer> pVBuffer)
{
  if (pVBuffer.expired()) {
    SH_ASSERT(false && "Vertex buffer expired!");
    return SPtr<InputLayout>();
  }

  auto pInputLayout = sh_makeShared<OGLInputLayout>();
  uint32 vbo = cast::re_ptr<OGLVertexBuffer>(pVBuffer.lock())->m_bufferID;

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  uint32 offset = 0;
  uint32 stride = 0;

  for (const auto& element : desc) {
    stride += element.size;
  }

  for (uint32 i = 0; i < desc.size(); ++i) {
    const auto& element = desc[i];
    
    GLint componentCount = 0;
    GLenum glType = GL_FLOAT;
    GLboolean normalized = GL_FALSE;

    switch (element.type) {
      case INPUT_LAYOUT_TYPES::kPosition:
      componentCount = 3;
      glType = GL_FLOAT;
      break;

    
      break;
    }

    glEnableVertexAttribArray(i);

    glVertexAttribPointer(i,
                          componentCount,
                          glType,
                          normalized,
                          stride,
                          cast::re<void*>(offset));

    offset += element.size;
  }

  glBindVertexArray(0);

  pInputLayout->m_vao = vao;

  return pInputLayout;
}

SPtr<VertexShader>
OGLGraphicsManager::createVertexShader(const String& fileName,
                                       const String& entryPoint,
                                       const String& shaderModel,
                                       const Vector<ShaderMacro>& macros)
{
  //SH_UNREFERENCED_PARAMETER(entryPoint);
  SH_UNREFERENCED_PARAMETER(shaderModel);

  SystemPath path(fileName);
  String newFileName = path.filename().string() + entryPoint + "VS";

  uint32 shaderID = compileShader(newFileName, OPENGL_SHADER_TYPE::VERTEX_SHADER, macros);
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
  //SH_UNREFERENCED_PARAMETER(entryPoint);
  SH_UNREFERENCED_PARAMETER(shaderModel);

  SystemPath path(fileName);
  String newFileName = path.filename().string() + entryPoint + "FS";

  uint32 shaderID = compileShader(newFileName, OPENGL_SHADER_TYPE::PIXEL_SHADER, macros);
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
  //SH_UNREFERENCED_PARAMETER(entryPoint);
  SH_UNREFERENCED_PARAMETER(shaderModel);

  SystemPath path(fileName);
  String newFileName = path.filename().string() + entryPoint + "GS";

  uint32 shaderID = compileShader(newFileName, OPENGL_SHADER_TYPE::GEOMETRY_SHADER, macros);
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
  //SH_UNREFERENCED_PARAMETER(entryPoint);
  SH_UNREFERENCED_PARAMETER(shaderModel);

  SystemPath path(fileName);
  String newFileName = path.filename().string() + entryPoint + "CS";

  uint32 shaderID = compileShader(newFileName, OPENGL_SHADER_TYPE::COMPUTE_SHADER, macros);
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
  auto pVBuffer = sh_makeShared<OGLVertexBuffer>();

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  GLenum glUsage = GL_STATIC_DRAW;

  switch (usage) {
  case USAGE::kImmutable:
    glUsage = GL_STATIC_DRAW;
    break;
  case USAGE::kDynamic:
    glUsage = GL_DYNAMIC_DRAW;
    break;
  case USAGE::kStaging:
    glUsage = GL_STREAM_DRAW;
    break;
  default:
    glUsage = GL_STATIC_DRAW;
    break;
  }

  glBufferData(GL_ARRAY_BUFFER, bufferSize * stride, pData, glUsage);

  pVBuffer->m_bufferID = vbo;
  pVBuffer->m_stride = stride;
  pVBuffer->m_size = bufferSize;

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return pVBuffer;
}

SPtr<IndexBuffer>
OGLGraphicsManager::createIndexBuffer(const Vector<uint32>& indices, const uint32 usage)
{
  auto pIBuffer = sh_makeShared<OGLIndexBuffer>();

  GLuint ibo;
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

  GLenum glUsage = GL_STATIC_DRAW;

  switch (usage) {
  case USAGE::kImmutable:
    glUsage = GL_STATIC_DRAW;
    break;
  case USAGE::kDynamic:
    glUsage = GL_DYNAMIC_DRAW;
    break;
  case USAGE::kStaging:
    glUsage = GL_STREAM_DRAW;
    break;
  default:
    glUsage = GL_STATIC_DRAW;
    break;
  }

  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               indices.size() * sizeof(uint32),
               indices.data(),
               glUsage);

  pIBuffer->m_bufferID = ibo;
  pIBuffer->m_count = static_cast<uint32>(indices.size());

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  return pIBuffer;
}

SPtr<ConstantBuffer>
OGLGraphicsManager::createConstantBuffer(const uint32 bufferSize,
                                         const uint32 usage,
                                         const void* pData)
{
  auto pCBuffer = sh_makeShared<OGLConstantBuffer>();
  
  GLuint ubo = 0;
  glGenBuffers(1, &ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, ubo);

  GLenum glUsage = GL_STATIC_DRAW;

  switch (usage) {
  case USAGE::kImmutable:
    glUsage = GL_STATIC_DRAW;
    break;

  case USAGE::kDynamic:
    glUsage = GL_DYNAMIC_DRAW;
    break;

  case USAGE::kStaging:
    glUsage = GL_STREAM_DRAW;
    break;

  default:
    glUsage = GL_STATIC_DRAW;
    break;
  }

  glBufferData(GL_UNIFORM_BUFFER, bufferSize, pData, glUsage);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  pCBuffer->m_bufferID = ubo;
  pCBuffer->m_size = bufferSize;

  return pCBuffer;
}

SPtr<SamplerState>
OGLGraphicsManager::createSamplerState(const uint32 filter, const uint32 textAddress)
{
  auto pSamplerState = sh_makeShared<OGLSamplerState>();

  GLuint sampler = 0;
  glGenSamplers(1, &sampler);

  GLenum minFilter = GL_LINEAR;
  GLenum magFilter = GL_LINEAR;

  switch (filter) {
  case SAMPLER_FILTER::kFilterMinMagMipPoint:
    minFilter = GL_NEAREST_MIPMAP_NEAREST;
    magFilter = GL_NEAREST;
    break;
  case SAMPLER_FILTER::kFilterMinMagMipLinear:
    minFilter = GL_LINEAR_MIPMAP_LINEAR;
    magFilter = GL_LINEAR;
    break;
  case SAMPLER_FILTER::kFilterAnisotropic:
    minFilter = GL_LINEAR_MIPMAP_LINEAR;
    magFilter = GL_LINEAR;
    break;
  default:
    minFilter = GL_LINEAR;
    magFilter = GL_LINEAR;
    break;
  }

  glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, minFilter);
  glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, magFilter);

  GLenum wrapMode = GL_REPEAT;

  switch (textAddress) {
  case TEXTURE_ADDRESS_MODE::kWrap:
    wrapMode = GL_REPEAT;
    break;
  case TEXTURE_ADDRESS_MODE::kMirror:
    wrapMode = GL_MIRRORED_REPEAT;
    break;
  case TEXTURE_ADDRESS_MODE::kClamp:
    wrapMode = GL_CLAMP_TO_EDGE;
    break;
  case TEXTURE_ADDRESS_MODE::kBorder:
    wrapMode = GL_CLAMP_TO_BORDER;
    break;
  default:
    wrapMode = GL_REPEAT;
    break;
  }

  glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, wrapMode);
  glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, wrapMode);
  glSamplerParameteri(sampler, GL_TEXTURE_WRAP_R, wrapMode);

  glSamplerParameterf(sampler, GL_TEXTURE_MIN_LOD, 0.0f);
  glSamplerParameterf(sampler, GL_TEXTURE_MAX_LOD, FLT_MAX);

  glSamplerParameteri(sampler, GL_TEXTURE_COMPARE_MODE, GL_NONE);
  glSamplerParameteri(sampler, GL_TEXTURE_COMPARE_FUNC, GL_NEVER);

  pSamplerState->m_samplerID = sampler;

  return pSamplerState;
}

SPtr<Texture2D>
OGLGraphicsManager::createTextureFromFile(const String& fileName,
                                          const void* pData,
                                          const uint32 width,
                                          const uint32 height,
                                          const uint32 bpp)
{
  if(!pData) {
    SH_ASSERT(false && "Texture data is null!");
    return SPtr<Texture2D>();
  }

  SystemPath path(fileName);
  uint32 format = 0;
  uint32 pitch = width * bpp;

  Vector<uint8> textureData;
  const void* finalData = pData;

  if(path.extension() == ".hdr") {
    format = TEXTURE_FORMAT::kR32G32B32A32_FLOAT;
    pitch = width * 4 * sizeof(float);
  }
  else {
    if (bpp == 3) {
      textureData.resize(width * height * 4);

      const uint8* src = reinterpret_cast<const uint8*>(pData);

      for (uint32 i = 0; i < width * height; ++i)
      {
        textureData[i * 4 + 0] = src[i * 3 + 0];
        textureData[i * 4 + 1] = src[i * 3 + 1];
        textureData[i * 4 + 2] = src[i * 3 + 2];
        textureData[i * 4 + 3] = 255;
      }

      finalData = textureData.data();
      pitch = width * 4;
    }

    format = TEXTURE_FORMAT::kR8G8B8A8_UNORM;
  }

  auto pTexture = cast::st_ptr<OGLTexture2D>(createTexture2D(width, height, format,
                                                             0, 0, 1, 1));

  glBindTexture(pTexture->m_target, pTexture->m_textureID);

  GLenum dataFormat = translateTextureFormat(format);
  GLenum dataType = translateTextureType(format);

  glTexSubImage2D(pTexture->m_target, 0, 0, 0, width, height,
                  dataFormat, dataType, finalData);

  glBindTexture(pTexture->m_target, 0);

  return pTexture;
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
  auto pTexture = sh_makeShared<OGLTexture2D>();

  GLuint textureID = 0;
  glGenTextures(1, &textureID);

  GLenum target = (arraySize > 1) ? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D;

  glBindTexture(target, textureID);

  GLenum internalFormat = translateInternalTextureFormat(format);
  GLenum dataFormat = translateTextureFormat(format);
  GLenum dataType = translateTextureType(format);

  uint32 levels = mipLevels;

  if (levels == 0) {
    levels = cast::st<uint32>(Math::floor(Math::log2(Math::max(width, height)))) + 1;
  }

  if(arraySize > 1) {
    glTexStorage3D(target, levels, internalFormat, width, height, arraySize);
  } else {
    glTexStorage2D(target, levels, internalFormat, width, height);
  }

  glTexParameteri(target, GL_TEXTURE_MIN_FILTER,
                  levels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);

  pTexture->m_textureID = textureID;
  pTexture->m_target = target;
  pTexture->m_width = width;
  pTexture->m_height = height;
  pTexture->m_mipLevels = levels;
  pTexture->m_arraySize = arraySize;
  pTexture->m_format = format;

  glBindTexture(target, 0);

  if (mipLevels > 1) {
    generateMips(pTexture);
  }

  return pTexture;
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
  auto pTexture = sh_makeShared<OGLTexture2D>();
  
  const uint32 errorSize = 128;
  Vector<uint32> pixels;
  pixels.resize(errorSize * errorSize);

  for (uint32 y = 0; y < errorSize; ++y) {
    for(uint32 x = 0; x < errorSize; ++x) {
      bool isPink = ((x / 16) % 2) == ((y / 16) % 2);
      pixels[y * errorSize + x] = isPink ? 0xFFFF00FF : 0xFF000000;
    }
  }

  GLuint textureID = 0;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, errorSize, errorSize, 0,
               GL_BGRA, GL_UNSIGNED_BYTE, pixels.data());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glBindTexture(GL_TEXTURE_2D, 0);

  pTexture->m_textureID = textureID;
  pTexture->m_target = GL_TEXTURE_2D;
  pTexture->m_width = errorSize;
  pTexture->m_height = errorSize;
  pTexture->m_mipLevels = 1;
  pTexture->m_arraySize = 1;
  pTexture->m_format = TEXTURE_FORMAT::kR8G8B8A8_UNORM;

  return pTexture;
}

SPtr<Texture2D>
OGLGraphicsManager::createDefaultNormalTexture()
{
  auto pTexture = sh_makeShared<OGLTexture2D>();

  const uint32 normalSize = 128;
  Vector<uint32> pixels;
  pixels.resize(normalSize * normalSize);
  for (uint32 y = 0; y < normalSize; ++y) {
    for (uint32 x = 0; x < normalSize; ++x) {
      pixels[y * normalSize + x] = 0xFFFF8080;
    }
  }

  GLuint textureID = 0;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, normalSize, normalSize, 0,
               GL_BGRA, GL_UNSIGNED_BYTE, pixels.data());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glBindTexture(GL_TEXTURE_2D, 0);

  pTexture->m_textureID = textureID;
  pTexture->m_target = GL_TEXTURE_2D;
  pTexture->m_width = normalSize;
  pTexture->m_height = normalSize;
  pTexture->m_mipLevels = 1;
  pTexture->m_arraySize = 1;
  pTexture->m_format = TEXTURE_FORMAT::kR8G8B8A8_UNORM;

  return pTexture;
}

SPtr<Texture2D>
OGLGraphicsManager::createBlackTexture()
{
  auto pTexture = sh_makeShared<OGLTexture2D>();

  const uint32 size = 128;
  Vector<uint32> pixels;
  pixels.resize(size * size);
  for (uint32 y = 0; y < size; ++y) {
    for (uint32 x = 0; x < size; ++x) {
      pixels[y * size + x] = 0xFF000000;
    }
  }

  GLuint textureID = 0;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size, size, 0,
               GL_BGRA, GL_UNSIGNED_BYTE, pixels.data());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glBindTexture(GL_TEXTURE_2D, 0);

  pTexture->m_textureID = textureID;
  pTexture->m_target = GL_TEXTURE_2D;
  pTexture->m_width = size;
  pTexture->m_height = size;
  pTexture->m_mipLevels = 1;
  pTexture->m_arraySize = 1;
  pTexture->m_format = TEXTURE_FORMAT::kR8G8B8A8_UNORM;

  return pTexture;
}

SPtr<BlendState>
OGLGraphicsManager::createBlendState(const BlendDesc& blendDesc,
                                     const LinearColor& blendFactor)
{
  auto pBlendState = std::make_shared<OGLBlendState>();

  const auto& rt = blendDesc.renderTarget[0];

  pBlendState->m_blendEnable = rt.blendEnable;

  pBlendState->m_srcRGB = translateBlend(rt.srcBlend);
  pBlendState->m_dstRGB = translateBlend(rt.destBlend);
  pBlendState->m_opRGB = translateBlendOp(rt.blendOp);

  pBlendState->m_srcAlpha = translateBlend(rt.srcBlendAlpha);
  pBlendState->m_dstAlpha = translateBlend(rt.destBlendAlpha);
  pBlendState->m_opAlpha = translateBlendOp(rt.blendOpAlpha);

  pBlendState->m_writeMask = rt.renderTargetWriteMask;

  pBlendState->m_blendFactor = blendFactor;

  return pBlendState;
}

SPtr<RasterizerState>
OGLGraphicsManager::createRasterizerState(const RasterizerDesc& rasterizerDesc)
{
  auto pRState = std::make_shared<OGLRasterizerState>();

  pRState->m_fillMode = translateFillMode(rasterizerDesc.fillMode);
  pRState->m_cullMode = translateCullMode(rasterizerDesc.cullMode);
  pRState->m_bFrontCounterClockwise = rasterizerDesc.frontCounterClockwise;

  pRState->m_depthBias = rasterizerDesc.depthBias;
  pRState->m_depthBiasClamp = rasterizerDesc.depthBiasClamp;
  pRState->m_slopeScaledDepthBias = rasterizerDesc.slopeScaledDepthBias;

  pRState->m_bDepthClipEnable = !rasterizerDesc.depthClipEnable;
  pRState->m_bScissorEnable = rasterizerDesc.scissorEnable;
  pRState->m_bMultisampleEnable = rasterizerDesc.multisampleEnable;
  pRState->m_bAntialiasedLineEnable = rasterizerDesc.antialiasedLineEnable;

  return pRState;
}

SPtr<DepthStencilState>
OGLGraphicsManager::createDepthStencilState(const DepthStencilDesc& depthSDesc)
{
  auto pDSState = std::make_shared<OGLDepthStencilState>();

  // Depth state
  pDSState->m_depthEnable = depthSDesc.depthEnable;
  pDSState->m_depthWriteMask = (depthSDesc.depthWriteMask ==
                                          DEPTH_WRITE_MASK::kAll);
  pDSState->m_depthFunc = translateComparison(depthSDesc.depthFunc);

  // Stencil state
  pDSState->m_stencilEnable = depthSDesc.stencilEnable;
  pDSState->m_stencilReadMask = depthSDesc.stencilReadMask;
  pDSState->m_stencilWriteMask = depthSDesc.stencilWriteMask;

  // Front face stencil operations
  pDSState->m_frontFace.func = translateComparison(depthSDesc.frontFace.stencilFunc);
  pDSState->m_frontFace.stencilFailOp =
            translateStencilOp(depthSDesc.frontFace.stencilFailOp);
  pDSState->m_frontFace.depthFailOp =
            translateStencilOp(depthSDesc.frontFace.stencilDepthFailOp);
  pDSState->m_frontFace.passOp =
            translateStencilOp(depthSDesc.frontFace.stencilPassOp);

  // Back face stencil operations
  pDSState->m_backFace.func = translateComparison(depthSDesc.backFace.stencilFunc);
  pDSState->m_backFace.stencilFailOp =
            translateStencilOp(depthSDesc.backFace.stencilFailOp);
  pDSState->m_backFace.depthFailOp =
            translateStencilOp(depthSDesc.backFace.stencilDepthFailOp);
  pDSState->m_backFace.passOp =
            translateStencilOp(depthSDesc.backFace.stencilPassOp);

  return pDSState;
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
  if (pTexture.expired()) {
    SH_ASSERT(false && "Texture expired!");
    return;
  }

  auto pOGLTexture = cast::re_ptr<OGLTexture2D>(pTexture.lock());
  glBindTexture(pOGLTexture->m_target, pOGLTexture->m_textureID);
  glGenerateMipmap(pOGLTexture->m_target);
  glBindTexture(pOGLTexture->m_target, 0);
}

void
OGLGraphicsManager::updateConstantBuffer(const WPtr<ConstantBuffer> pCBuffer,
                                         const void* pData,
                                         const uint32 dataSize)
{
  if (pCBuffer.expired()) {
    SH_ASSERT(false && "Constant buffer expired!");
    return;
  }

  auto pOGLCBuffer = cast::re_ptr<OGLConstantBuffer>(pCBuffer.lock());
  glBindBuffer(GL_UNIFORM_BUFFER, pOGLCBuffer->m_bufferID);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, dataSize, pData);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void
OGLGraphicsManager::updateTexture2D(WPtr<Texture2D> pTexture,
                                    uint8* pData,
                                    uint32 width,
                                    uint32 bpp)
{
  if (pTexture.expired()) {
    SH_ASSERT(false && "Texture expired!");
    return;
  }

  auto pOGLTexture = cast::re_ptr<OGLTexture2D>(pTexture.lock());

  glBindTexture(pOGLTexture->m_target, pOGLTexture->m_textureID);
  GLenum dataFormat = translateTextureFormat(pOGLTexture->m_format);
  GLenum dataType = translateTextureType(pOGLTexture->m_format);
  glTexSubImage2D(pOGLTexture->m_target, 0, 0, 0, width, pOGLTexture->m_height,
                  dataFormat, dataType, pData);
  glBindTexture(pOGLTexture->m_target, 0);
}

void
OGLGraphicsManager::updateScreenSize(const Vector2& size)
{
  uint32 newWidth = cast::st<uint32>(size.x);
  uint32 newHeight = cast::st<uint32>(size.y);

  if(m_pBackBuffer) {
    m_pBackBuffer.reset();
  }

  if (m_pDepthStencil) {
    m_pDepthStencil.reset();
  }

  auto pBackBuffer = createTexture2D(newWidth, newHeight, TEXTURE_FORMAT::kR8G8B8A8_UNORM,
                                     0, BIND_FLAGS::kRenderTarget, 1, 1);

  auto pDepthStencil = createTexture2D(newWidth, newHeight, TEXTURE_FORMAT::kD24_UNORM_S8_UINT,
                                      0, BIND_FLAGS::kDepthStencil, 1, 1);

  m_pBackBuffer = pBackBuffer;
  m_pDepthStencil = pDepthStencil;

  Viewport vp;
  vp.topLeftX = m_currentViewport.topLeftX;
  vp.topLeftY = m_currentViewport.topLeftY;
  vp.width = newWidth;
  vp.height = newHeight;
  vp.minDepth = m_currentViewport.minDepth;
  vp.maxDepth = m_currentViewport.maxDepth;
  setViewport(vp);
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
  SH_UNREFERENCED_PARAMETER(pTexture);
  SH_UNREFERENCED_PARAMETER(filePath);
}

void
OGLGraphicsManager::setViewport(const Viewport& vp)
{
  int32 x = cast::st<int32>(vp.topLeftX);
  int32 y = m_frameBufferHeight - cast::st<int32>(vp.topLeftY + vp.height);

  glViewport(x, y, cast::st<int32>(vp.width), cast::st<int32>(vp.height));
  glDepthRange(cast::st<GLclampd>(vp.minDepth), cast::st<GLclampd>(vp.maxDepth));

  m_currentViewport = vp;
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
  SH_UNREFERENCED_PARAMETER(numBuffers);
  setUniformBuffers(pCBuffer, startSlot);
}

void
OGLGraphicsManager::psSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                                         const uint32 startSlot,
                                         const uint32 numBuffers)
{
  SH_UNREFERENCED_PARAMETER(numBuffers);
  setUniformBuffers(pCBuffer, startSlot);
}

void
OGLGraphicsManager::gsSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                                         const uint32 startSlot,
                                         const uint32 numBuffers)
{
  SH_UNREFERENCED_PARAMETER(numBuffers);
  setUniformBuffers(pCBuffer, startSlot);
}

void
OGLGraphicsManager::csSetConstantBuffers(const WPtr<ConstantBuffer> pCBuffer,
                                         const uint32 startSlot,
                                         const uint32 numBuffers)
{
  SH_UNREFERENCED_PARAMETER(numBuffers);
  setUniformBuffers(pCBuffer, startSlot);
}

void
OGLGraphicsManager::setPrimitiveTopology(const uint32 primitive)
{
  switch (primitive) {
  case PRIMITIVE_TOPOLOGY::kPointList:
    m_currentTopology = GL_POINTS;
    break;
  case PRIMITIVE_TOPOLOGY::kLineList:
    m_currentTopology = GL_LINES;
    break;
  case PRIMITIVE_TOPOLOGY::kLineStrip:
    m_currentTopology = GL_LINE_STRIP;
    break;
  case PRIMITIVE_TOPOLOGY::kTrianglelist:
    m_currentTopology = GL_TRIANGLES;
    break;
  default:
    m_currentTopology = GL_TRIANGLES;
    break;
  }
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
  if (pBlendState.expired()) {
    SH_ASSERT(false && "Blend state expired!");
    return;
  }

  auto pOGLBlendState = cast::re_ptr<OGLBlendState>(pBlendState.lock());

  if (pOGLBlendState->m_blendEnable) {
    glEnable(GL_BLEND);
  }
  else {
    glDisable(GL_BLEND);
  }

  glBlendFuncSeparate(pOGLBlendState->m_srcRGB,
                      pOGLBlendState->m_dstRGB,
                      pOGLBlendState->m_srcAlpha,
                      pOGLBlendState->m_dstAlpha);

  glBlendEquationSeparate(pOGLBlendState->m_opRGB,
                          pOGLBlendState->m_opAlpha);

  glBlendColor(pOGLBlendState->m_blendFactor.r,
               pOGLBlendState->m_blendFactor.g,
               pOGLBlendState->m_blendFactor.b,
               pOGLBlendState->m_blendFactor.a);

  glColorMask((pOGLBlendState->m_writeMask & 0x1) != 0,
              (pOGLBlendState->m_writeMask & 0x2) != 0,
              (pOGLBlendState->m_writeMask & 0x4) != 0,
              (pOGLBlendState->m_writeMask & 0x8) != 0);
}

void
OGLGraphicsManager::setRasterizerState(const WPtr<RasterizerState> pRasterizerState)
{
  if (pRasterizerState.expired()) {
    SH_ASSERT(false && "Rasterizer state expired!");
    return;
  }

  auto pRState = cast::re_ptr<OGLRasterizerState>(pRasterizerState.lock());

  // Fill mode
  glPolygonMode(GL_FRONT_AND_BACK, pRState->m_fillMode);

  // Culling mode
  if (pRState->m_cullMode != GL_NONE) {
    glEnable(GL_CULL_FACE);
    glCullFace(pRState->m_cullMode);
  }
  else {
    glDisable(GL_CULL_FACE);
  }

  // Front face winding order
  glFrontFace(pRState->m_bFrontCounterClockwise ? GL_CCW : GL_CW);

  // Depth bias (polygon offset)
  if (pRState->m_depthBias != 0.0f || pRState->m_slopeScaledDepthBias != 0.0f) {
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(pRState->m_slopeScaledDepthBias, cast::st<float>(pRState->m_depthBias));
  }
  else {
    glDisable(GL_POLYGON_OFFSET_FILL);
  }

  // Depth clipping
  if (pRState->m_bDepthClipEnable) {
    glEnable(GL_DEPTH_CLAMP);
  }
  else {
    glDisable(GL_DEPTH_CLAMP);
  }

  // Scissor test
  if(pRState->m_bScissorEnable) {
    glEnable(GL_SCISSOR_TEST);
  }
  else {
    glDisable(GL_SCISSOR_TEST);
  }

  // Multisampling
  if (pRState->m_bMultisampleEnable) {
    glEnable(GL_MULTISAMPLE);
  }
  else {
    glDisable(GL_MULTISAMPLE);
  }

  // Antialiased lines
  if (pRState->m_bAntialiasedLineEnable) {
    glEnable(GL_LINE_SMOOTH);
  }
  else {
    glDisable(GL_LINE_SMOOTH);
  }
}

void
OGLGraphicsManager::setDepthStencilState(const WPtr<DepthStencilState> pDepthSState,
                                         const uint8 stencilRef)
{
  if (pDepthSState.expired()) {
    SH_ASSERT(false && "Depth stencil state expired!");
    return;
  }

  auto pDSState = cast::re_ptr<OGLDepthStencilState>(pDepthSState.lock());

  // Depth state
  if (pDSState->m_depthEnable) {
    glEnable(GL_DEPTH_TEST);
  }
  else {
    glDisable(GL_DEPTH_TEST);
  }

  glDepthFunc(pDSState->m_depthFunc);
  glDepthMask(pDSState->m_depthWriteMask ? GL_TRUE : GL_FALSE);

  // Stencil state
  if (pDSState->m_stencilEnable) {
    glEnable(GL_STENCIL_TEST);
  }
  else {
    glDisable(GL_STENCIL_TEST);
  }

  glStencilMask(pDSState->m_stencilWriteMask);

  // Front face stencil operations
  glStencilOpSeparate(GL_FRONT,
                      pDSState->m_frontFace.stencilFailOp,
                      pDSState->m_frontFace.depthFailOp,
                      pDSState->m_frontFace.passOp);

  glStencilFuncSeparate(GL_FRONT,
                        pDSState->m_frontFace.func,
                        stencilRef,
                        pDSState->m_stencilReadMask);

  // Back face stencil operations
  glStencilOpSeparate(GL_BACK,
                      pDSState->m_backFace.stencilFailOp,
                      pDSState->m_backFace.depthFailOp,
                      pDSState->m_backFace.passOp);
}

void
OGLGraphicsManager::setScissorRects(const Rect& scissorClip)
{
  Viewport& currentVP = m_currentViewport;

  int32 x = cast::st<int32>(currentVP.topLeftX + scissorClip.min.x);
  int32 y = m_frameBufferHeight - cast::st<int32>(currentVP.topLeftY + scissorClip.max.y);

  int32 width = cast::st<int32>(scissorClip.max.x - scissorClip.min.x);
  int32 height = cast::st<int32>(scissorClip.max.y - scissorClip.min.y);

  glScissor(x, y, width, height);
}

void
OGLGraphicsManager::useProgram(uint32 programID)
{
  glUseProgram(programID);
}

void
OGLGraphicsManager::draw(const uint32 vertexCount, const uint32 startVertexLocation)
{
  glDrawArrays(cast::st<GLenum>(m_currentTopology), startVertexLocation, vertexCount);
}

void
OGLGraphicsManager::drawIndexed(const uint32 indexCount,
                                const uint32 startIndexLocation,
                                const uint32 baseVertexLocation)
{
  uint32 ibo = m_currentIBO;
  GLenum indexType = GL_UNSIGNED_INT;

  const void* offset = cast::re<void*>(startIndexLocation * sizeof(uint32));

  glDrawElementsBaseVertex(cast::st<GLenum>(m_currentTopology),
                           indexCount,
                           indexType,
                           offset,
                           baseVertexLocation);
}

void
OGLGraphicsManager::dispatch(const uint32 threadGroupCountX,
                             const uint32 threadGroupCountY,
                             const uint32 threadGroupCountZ)
{
  glDispatchCompute(threadGroupCountX, threadGroupCountY, threadGroupCountZ);

  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT |
                  GL_SHADER_STORAGE_BARRIER_BIT |
                  GL_UNIFORM_BARRIER_BIT);
}

void
OGLGraphicsManager::setUniformBuffers(WPtr<ConstantBuffer> pCBuffer, uint32 startSlot)
{
  if (pCBuffer.expired()) {
    SH_ASSERT(false && "Constant buffer expired!");
    return;
  }
  auto pUBO = cast::re_ptr<OGLConstantBuffer>(pCBuffer.lock());
  glBindBufferBase(GL_UNIFORM_BUFFER, startSlot, pUBO->m_bufferID);
}
}
