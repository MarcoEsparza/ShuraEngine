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

using std::reinterpret_pointer_cast;

#if SH_PLATFORM == SH_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#endif

// TODO: Delete this, make a wrapper.
#include "fstream"
#include "sstream"


namespace shEngineSDK {
using std::fstream;
using std::ios;
using std::stringstream;

void
compileShader(uint32& shader, uint32& ID, const String& shaderData)
{
  GLint Result = GL_FALSE;
  int InfoLogLength;

  // Compile shader
  char const* pShaderSource = shaderData.c_str();
  glShaderSource(shader, 1, &pShaderSource, NULL);
  glCompileShader(shader);

  // Check shader
  glGetShaderiv(shader, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &InfoLogLength);

  if (InfoLogLength > 0)
  {
    Vector<char> vsErrorMessage(InfoLogLength + 1);
    glGetShaderInfoLog(shader,
                       InfoLogLength,
                       NULL,
                       &vsErrorMessage[0]);
  }

  // Link the program
  uint32 programID = glCreateProgram();
  glAttachShader(programID, shader);
  glLinkProgram(programID);

  // Check the program
  glGetShaderiv(programID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);

  if (InfoLogLength > 0)
  {
    Vector<char> vsErrorMessage(InfoLogLength + 1);
    glGetShaderInfoLog(programID,
                       InfoLogLength,
                       NULL,
                       &vsErrorMessage[0]);
  }

  glDetachShader(programID, shader);
  //glDeleteShader(shader);

  ID = programID;
}

#if SH_PLATFORM == SH_PLATFORM_WIN32
void
enableOpenGL(const HWND& hwnd, HandleDC& hdc, RenderingContext& hrc)
{
  hdc = GetDC(hwnd);

  PIXELFORMATDESCRIPTOR pfd = { 0 };
  pfd.nSize = sizeof(pfd);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 24;
  pfd.cStencilBits = 8;
  pfd.iLayerType = PFD_MAIN_PLANE;

  int format = ChoosePixelFormat(hdc, &pfd);
  SetPixelFormat(hdc, format, &pfd);

  hrc = wglCreateContext(hdc);
  wglMakeCurrent(hdc, hrc);
}
#endif

void
OGLGraphicsManager::internalInit(const Screen& screen,
                                 const bool,
                                 const SAMPLE_DESC&)
{
  m_device = make_shared<OGLDevice>();
  m_rContext = make_shared<OGLRenderContext>();

  m_width = screen.getWidth();
  m_height = screen.getHeight();

#if SH_PLATFORM == SH_PLATFORM_WIN32
  enableOpenGL(screen.getPlatformHandler(),
               m_device->m_device,
               m_rContext->m_rContext);
#endif

  glewExperimental = true;

  GLenum err = glewInit();
  if (err != GLEW_OK) {
    SH_ASSERT("Error initializing glew");
  }

  glViewport(0, 0, m_width, m_height);

  m_framebuffer = make_shared<OGLFrameBuffer>();

  glGenFramebuffers(1, &m_framebuffer->m_frameObject);
  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer->m_frameObject);

  glGenTextures(GL_TEXTURE_2D, &m_framebuffer->m_texture);
  glBindTexture(GL_TEXTURE_2D, m_framebuffer->m_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
  //glBindTexture(GL_TEXTURE_2D, 0);

  glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER,
                         GL_COLOR_ATTACHMENT0,
                         GL_TEXTURE_2D,
                         m_framebuffer->m_texture,
                         0);
  
  m_depthRender = make_shared<OGLDepthRender>();
  m_depthRender->m_color = LinearColor(0.1f, 0.1f, 0.1f);

  glGenRenderbuffers(1, &m_depthRender->m_depthBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, m_depthRender->m_depthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                            GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER,
                            m_depthRender->m_depthBuffer);

  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    SH_ASSERT("Framebuffer incomplete");
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glEnable(GL_DEPTH_TEST);
}

void
OGLGraphicsManager::internalClearRenderTarget(const SPtr<RenderTargetView>& pTarget,
                                              LinearColor& color)
{
  auto pFbo = reinterpret_pointer_cast<OGLFrameBuffer>(pTarget);

  glBindBuffer(GL_FRAMEBUFFER, pFbo->m_frameObject);
  glViewport(0, 0, m_width, m_height);
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void
OGLGraphicsManager::internalClearDepthStencil(const SPtr<Texture2D>& pDepthSV)
{
  auto pDepth = reinterpret_pointer_cast<OGLDepthRender>(pDepthSV);

  glViewport(0, 0, m_width, m_height);
  glClearColor(pDepth->m_color.r, pDepth->m_color.g, pDepth->m_color.b, pDepth->m_color.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void
OGLGraphicsManager::internalPresent()
{
#if SH_PLATFORM == SH_PLATFORM_WIN32
  SwapBuffers(m_device->m_device);
#endif
}

SPtr<RenderTargetView>
OGLGraphicsManager::internalGetMainRenderTargetView() const
{
  return m_framebuffer;
}

SPtr<Texture2D>
OGLGraphicsManager::internalGetMainDepthStencil() const
{
  return m_depthRender;
}

SPtr<InputLayout>
OGLGraphicsManager::internalCreateInputLayout(const Vector<shINPUT_LAYOUT_TYPES::E>& types,
                                              const SPtr<ProgramShader>& pPShader)
{
  auto pProgramShader = reinterpret_pointer_cast<OGLProgramShader>(pPShader);

  pProgramShader->m_programID = glCreateProgram();

  glAttachShader(pProgramShader->m_programID, pProgramShader->m_vertexShader);
  glAttachShader(pProgramShader->m_programID, pProgramShader->m_fragShader);

  for (uint8 i = 0; i < types.size(); ++i) {
    if (types[i] == shINPUT_LAYOUT_TYPES::kPosition) {
      glBindAttribLocation(pProgramShader->m_programID, i, "vertexPosition");
    }
    else if (types[i] == shINPUT_LAYOUT_TYPES::kNormal) {
      glBindAttribLocation(pProgramShader->m_programID, i, "vertexNormal");
    }
    else if (types[i] == shINPUT_LAYOUT_TYPES::kTexcoord) {
      glBindAttribLocation(pProgramShader->m_programID, i, "vertexUV");
    }
    else if (types[i] == shINPUT_LAYOUT_TYPES::kBoneIndices) {
      glBindAttribLocation(pProgramShader->m_programID, i, "boneID");
    }
    else if (types[i] == shINPUT_LAYOUT_TYPES::kBoneWieghts) {
      glBindAttribLocation(pProgramShader->m_programID, i, "boneWeight");
    }
  }

  glLinkProgram(pProgramShader->m_programID);

  return nullptr;
}

SPtr<ProgramShader>
OGLGraphicsManager::internalCreateProgramShader(const String& vertexFileName,
                                                const String& fragFileName,
                                                const String&,
                                                const String&,
                                                const String&)
{
  auto pProgramShader = make_shared<OGLProgramShader>();

  pProgramShader->m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
  pProgramShader->m_fragShader = glCreateShader(GL_FRAGMENT_SHADER);

  String vsCode;
  String fsCode;
  fstream vsFile(vertexFileName, ios::in);
  fstream fsFile(fragFileName, ios::in);

  if (!vsFile.is_open() || !fsFile.is_open()) {
    return nullptr;
  }

  stringstream vsSStream;
  vsSStream << vsFile.rdbuf();
  vsCode = vsSStream.str();
  vsFile.close();

  stringstream fsSStream;
  fsSStream << fsFile.rdbuf();
  fsCode = fsSStream.str();
  fsFile.close();

  compileShader(pProgramShader->m_vertexShader,
                pProgramShader->m_programID,
                vsCode);

  compileShader(pProgramShader->m_fragShader,
                pProgramShader->m_programID,
                vsCode);

  return pProgramShader;
}

SPtr<VertexBuffer>
OGLGraphicsManager::internalCreateVertexBuffer(const Vector<VertexData>& vertices,
                                               const uint32)
{
  auto pVBuffer = make_shared<OGLVertexBuffer>();

  glGenBuffers(1, &pVBuffer->m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, pVBuffer->m_vbo);
  glBufferData(GL_ARRAY_BUFFER,
               vertices.size() * sizeof(VertexData),
               vertices.data(),
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return pVBuffer;
}

SPtr<IndexBuffer>
OGLGraphicsManager::internalCreateIndexBuffer(const Vector<uint32>& indices,
                                              const uint32)
{
  auto pIBuffer = make_shared<OGLIndexBuffer>();

  glGenBuffers(1, &pIBuffer->m_ibo);
  glBindBuffer(GL_ARRAY_BUFFER, pIBuffer->m_ibo);
  glBufferData(GL_ARRAY_BUFFER,
               indices.size() * sizeof(VertexData),
               indices.data(),
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return pIBuffer;
}

SPtr<ConstantBuffer>
OGLGraphicsManager::internalCreateConstantBuffer(const uint32 bufferSize,
                                                 const uint32 bindingPoint,
                                                 const void* pData)
{
  auto pCBuffer = make_shared<OGLConstantBuffer>();

  glGenBuffers(1, &pCBuffer->m_ubo);
  glBindBuffer(GL_ARRAY_BUFFER, pCBuffer->m_ubo);
  glBufferData(GL_ARRAY_BUFFER,
               bufferSize,
               pData,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  pCBuffer->m_binding = bindingPoint;
  glBindBufferBase(GL_UNIFORM_BUFFER, pCBuffer->m_binding, pCBuffer->m_ubo);

  return pCBuffer;
}

SPtr<SamplerState>
OGLGraphicsManager::internalCreateSamplerState(const uint32 filter,
                                               const uint32 textAddress)
{
  auto pSamplerLinear = make_shared<OGLSamplerState>();

  glGenSamplers(1, &pSamplerLinear->m_samplerID);

  // TODO: Values are temporal, it should be replaced with a translator.
  if (textAddress == 1) {
    glSamplerParameteri(pSamplerLinear->m_samplerID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(pSamplerLinear->m_samplerID, GL_TEXTURE_WRAP_T, GL_REPEAT);
  }

  // TODO: Values are temporal, it should be replaced with a translator.
  if (filter == 21) {
    glSamplerParameteri(pSamplerLinear->m_samplerID,
                        GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(pSamplerLinear->m_samplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }

  return pSamplerLinear;
}

#define STB_IMAGE_IMPLEMENTATION
#include "externals/stb_image.h"

SPtr<Texture2D>
OGLGraphicsManager::internalCreateTextureFromFile(const String& fileName)
{
  int32 width, height, bpp;

  unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &bpp, STBI_rgb_alpha);

  //int32 pitch = width * bpp;

  auto pTexture = reinterpret_pointer_cast<OGLTexture2D>(internalCreateTexture2D(width,
                                                         height,
                                                         GL_RGBA8,
                                                         GL_RGBA8,
                                                         GL_UNSIGNED_BYTE));

  glBindTexture(GL_TEXTURE_2D, pTexture->m_texture);
  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGBA8,
               width,
               height,
               0,
               GL_RGBA8,
               GL_UNSIGNED_BYTE,
               data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(data);

  return pTexture;
}

SPtr<Texture2D>
OGLGraphicsManager::internalCreateTexture2D(const uint32 width,
                                            const uint32 height,
                                            const uint32 format,
                                            const uint32 usage,
                                            const uint32 bindFlags)
{
  auto pTexture2D = make_shared<OGLTexture2D>();

  glGenTextures(GL_TEXTURE_2D, &pTexture2D->m_texture);
  glBindTexture(GL_TEXTURE_2D, pTexture2D->m_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, usage, bindFlags, 0);
  glBindTexture(GL_TEXTURE_2D, 0);

  return pTexture2D;
}

void
OGLGraphicsManager::internalUpdateConstantBuffer(const SPtr<ConstantBuffer>& pCBuffer,
                                                 const void* pData,
                                                 const uint32 dataSize)
{
  auto pConstantBuffer = reinterpret_pointer_cast<OGLConstantBuffer>(pCBuffer);

  glBindBuffer(GL_UNIFORM_BUFFER, pConstantBuffer->m_ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, dataSize, pData);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void
OGLGraphicsManager::internalSetRenderTargets(const SPtr<RenderTargetView>&,
                                             const SPtr<Texture2D>&,
                                             const uint32)
{

}

void
OGLGraphicsManager::internalSetInputLayout(const SPtr<InputLayout>&)
{

}

void
OGLGraphicsManager::internalSetVertexBuffers(const SPtr<VertexBuffer>&,
                                             const uint32,
                                             const uint32,
                                             const uint32)
{
  //auto pVertexBuffer = reinterpret_pointer_cast<OGLVertexBuffer>(pVBuffer);

  //glBindBuffer(GL_ARRAY_BUFFER, pVertexBuffer->m_vbo);
}

void
OGLGraphicsManager::internalSetIndexBuffers(const SPtr<IndexBuffer>&,
                                            const uint32)
{

}

void
OGLGraphicsManager::internalVSSetConstantBuffers(const SPtr<ConstantBuffer>&,
                                                 const uint32,
                                                 const uint32)
{

}

void
OGLGraphicsManager::internalPSSetConstantBuffers(const SPtr<ConstantBuffer>&,
                                                 const uint32,
                                                 const uint32)
{

}

void
OGLGraphicsManager::internalSetPrimitiveTopology(const uint32 primitive)
{
  if (primitive == 4) {
    m_topology = GL_TRIANGLES;
  }
}

void
OGLGraphicsManager::internalSetProgramShader(const SPtr<ProgramShader>& pPShader,
                                             const void*,
                                             const uint32)
{
  auto pProgramShader = reinterpret_pointer_cast<OGLProgramShader>(pPShader);

  glUseProgram(pProgramShader->m_programID);
}

void
OGLGraphicsManager::internalSetShaderResourceView(const SPtr<Texture2D>& pShaderRV,
                                                  const uint32,
                                                  const uint32)
{
  auto pTexture = reinterpret_pointer_cast<OGLTexture2D>(pShaderRV);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, pTexture->m_texture);
}

void
OGLGraphicsManager::internalSetSamplerState(const SPtr<SamplerState>& pSamplerLinear,
                                            const uint32 startSlot,
                                            const uint32)
{
  auto pSampler = reinterpret_pointer_cast<OGLSamplerState>(pSamplerLinear);

  glActiveTexture(GL_TEXTURE0);
  glBindSampler(startSlot, pSampler->m_samplerID);
}

void
OGLGraphicsManager::internalDraw(const uint32, const uint32)
{

}

void
OGLGraphicsManager::internalDrawIndexed(const uint32 indexCount,
                                        const uint32 startIndexLocation,
                                        const uint32)
{
  glDrawElements(m_topology, indexCount, GL_UNSIGNED_INT, &startIndexLocation);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glUseProgram(0);
}
}
