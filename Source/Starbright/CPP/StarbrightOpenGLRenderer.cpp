#include "SuperCPP.h"
#include "StarbrightOpenGLRenderer.h"
using namespace SuperCPP;

#include <stdio.h>
#include <string.h>

namespace PROJECT_WORKSPACE
{
namespace Starbright
{

//-----------------------------------------------------------------------------
//  OpenGLRenderer
//-----------------------------------------------------------------------------
OpenGLRenderer::OpenGLRenderer()
  : color_shader(0),
    texture_shader(0),
    texture_shader_with_color_multiply(0),
    premultiplied_texture_shader_with_color_add(0),
    premultiplied_texture_shader_with_color_fill(0)
{
  color_shader = define_shader(
    // Vertex Shader
    SB_PRECISION_MEDIUMP_FLOAT
    "uniform mat4   transform;                 \n"
    "attribute vec4 position;                  \n"
    "attribute" SB_LOWP "vec4 color;           \n"
    "varying  " SB_LOWP "vec4 vertex_color;    \n"
    "void main()                               \n"
    "{                                         \n"
    "  gl_Position = transform * position;     \n"
    "  vertex_color = color / 255.0;           \n"
    "}                                         \n",

    // Pixel Shader
    SB_PRECISION_MEDIUMP_FLOAT
    "varying" SB_LOWP "vec4 vertex_color;    \n"
    "void main()                             \n"
    "{                                       \n"
    "  gl_FragColor = vertex_color;          \n"
    "}                                       \n"
  );

  texture_shader = define_shader(
    // Vertex Shader
    SB_PRECISION_MEDIUMP_FLOAT
    "uniform mat4   transform;             \n"
    "attribute vec4 position;              \n"
    "attribute      vec2 uv;               \n"
    "varying        vec2 vertex_uv;        \n"
    "void main()                           \n"
    "{                                     \n"
    "  gl_Position = transform * position; \n"
    "  vertex_uv = uv;                     \n" 
    "}                                     \n",

    // Pixel Shader
    SB_PRECISION_MEDIUMP_FLOAT
    "uniform              sampler2D texture_0;        \n"
    "varying              vec2      vertex_uv;        \n"
    "void main()                                      \n"
    "{                                                \n"
    "  gl_FragColor = texture2D(texture_0,vertex_uv); \n"
    "}                                                \n"
  );

  texture_shader_with_color_multiply = define_shader(
    // Vertex Shader
    SB_PRECISION_MEDIUMP_FLOAT
    "uniform mat4   transform;                  \n"
    "attribute vec4 position;                   \n"
    "attribute      vec2 uv;                    \n"
    "varying        vec2 vertex_uv;             \n"
    "attribute" SB_LOWP "vec4 color;         \n"
    "varying  " SB_LOWP "vec4 vertex_color;  \n"
    "void main()                                \n"
    "{                                          \n"
    "  gl_Position = transform * position;      \n"
    "  vertex_uv = uv;                          \n" 
    "  vertex_color = color / 255.0;            \n"
    "}                                          \n",

    // Pixel Shader
    SB_PRECISION_MEDIUMP_FLOAT
    "uniform              sampler2D texture_0;                       \n"
    "varying              vec2      vertex_uv;                     \n"
    "varying" SB_LOWP "vec4 vertex_color;                       \n"
    "void main()                                                   \n"
    "{                                                             \n"
    "  gl_FragColor = texture2D(texture_0,vertex_uv) * vertex_color; \n"
    "}                                                             \n"
  );

  premultiplied_texture_shader_with_color_add = define_shader(
    // Vertex Shader
    SB_PRECISION_MEDIUMP_FLOAT
    "uniform mat4   transform;                 \n"
    "attribute vec4 position;                  \n"
    "attribute      vec2 uv;                   \n"
    "varying        vec2 vertex_uv;            \n"
    "attribute" SB_LOWP "vec4 color;        \n"
    "varying  " SB_LOWP "vec4 vertex_color; \n"
    "void main()                               \n"
    "{                                         \n"
    "  gl_Position = transform * position;     \n"
    "  vertex_uv = uv;                         \n" 
    "  vertex_color = color / 255.0;           \n"
    "}                                         \n",

    // Pixel Shader
    SB_PRECISION_MEDIUMP_FLOAT
    "uniform              sampler2D texture_0;                       \n"
    "varying              vec2      vertex_uv;                     \n"
    "varying" SB_LOWP "vec4 vertex_color;                       \n"
    "void main()                                                   \n"
    "{                                                             \n"
    "  vec4 texture_color = texture2D(texture_0,vertex_uv);          \n"
    "  gl_FragColor = vec4( texture_color.xyz + (vertex_color.xyz * texture_color.a), texture_color.a ); \n"
    "}                                                             \n"
  );

  premultiplied_texture_shader_with_color_fill = define_shader(
    // Vertex Shader
    SB_PRECISION_MEDIUMP_FLOAT
    "uniform mat4   transform;                            \n"
    "attribute vec4 position;                             \n"
    "attribute      vec2 uv;                              \n"
    "varying        vec2 vertex_uv;                       \n"
    "attribute" SB_LOWP "vec4  color;                  \n"
    "varying  " SB_LOWP "vec4  vertex_color;           \n"
    "varying  " SB_LOWP "float vertex_inverse_a;       \n"
    "void main()                                          \n"
    "{                                                    \n"
    "  gl_Position = transform * position;                \n"
    "  vertex_uv = uv;                                    \n" 
    "  vertex_color = color / 255.0;                      \n"
    "  vertex_inverse_a = 1.0 - vertex_color.a;           \n"
    "}                                                    \n",

    // Pixel Shader
    SB_PRECISION_MEDIUMP_FLOAT
    "uniform                sampler2D texture_0;            \n"
    "varying                vec2      vertex_uv;          \n"
    "varying" SB_LOWP   "vec4      vertex_color;       \n"
    "varying  " SB_LOWP "float vertex_inverse_a;       \n"
    "void main()                                          \n"
    "{                                                    \n"
    "  vec4 texture_color = texture2D(texture_0,vertex_uv); \n"
    "  gl_FragColor = vec4( ((texture_color.xyz*vertex_inverse_a)+vertex_color.xyz) * texture_color.a, texture_color.a );  \n" 
    "}                                                   \n"
  );

  projection_transform.set_identity();
}

OpenGLRenderer::~OpenGLRenderer()
{
  // Delete shaders
  while (shaders.count())
  {
    delete_shader( shaders.locate_first() );
  }

  while (textures.count())
  {
    delete_texture( textures.locate_first() );
  }
}

void OpenGLRenderer::begin_draw( int _display_width, int _display_height )
{
  display_width  = _display_width;
  display_height = _display_height;
}

void OpenGLRenderer::end_draw()
{
  flush();
}

void OpenGLRenderer::clear( int flags )
{
  int gl_flags = 0;
  if (flags & Renderer::CLEAR_COLOR)
  {
    int color = clear_color;
    gl_flags |= GL_COLOR_BUFFER_BIT;
    glClearColor( ((color>>16)&255)/255.0f, ((color>>8)&255)/255.0f, (color&255)/255.0f, ((color>>24)&255)/255.0f );
  }

  if (flags & Renderer::CLEAR_DEPTH)
  {
    double depth = clear_depth;
    gl_flags |= GL_DEPTH_BUFFER_BIT;
    glClearDepth( (float) depth );
  }

  if (flags & Renderer::CLEAR_STENCIL)
  {
    int stencil = clear_stencil;
    gl_flags |= GL_STENCIL_BUFFER_BIT;
    glClearStencil( stencil );
  }

  if (gl_flags) glClear( gl_flags );
}

// COLOR|DEPTH|STENCIL
int  OpenGLRenderer::define_shader( const char* vertex_src, const char* pixel_src )
{
  OpenGLShader* shader = new OpenGLShader();

  shader->vertex_shader = glCreateShader( GL_VERTEX_SHADER );
  shader->pixel_shader = glCreateShader( GL_FRAGMENT_SHADER );

  glShaderSource( shader->vertex_shader, 1, (const char**) &vertex_src, 0 );
  glCompileShader( shader->vertex_shader );
  //SB_check_for_shader_compile_errors( shader->vertex_shader );

  glShaderSource( shader->pixel_shader, 1, (const char**) &pixel_src, 0 );
  glCompileShader( shader->pixel_shader );
  //SB_check_for_shader_compile_errors( shader->pixel_shader );

  shader->program = glCreateProgram();
  glAttachShader( shader->program, shader->vertex_shader );
  glAttachShader( shader->program, shader->pixel_shader );

  glLinkProgram( shader->program );

  shader->transform_setting  = glGetUniformLocation( shader->program, "transform" );
  shader->position_attribute = glGetAttribLocation( shader->program, "position" );
  shader->color_attribute    = glGetAttribLocation( shader->program, "color" );
  shader->uv_attribute       = glGetAttribLocation( shader->program,  "uv" );

  {
    int i;
    for (i=0; i<Renderer::MAX_CONCURRENT_TEXTURES; ++i)
    {
      char name[80];
      sprintf( name, "texture_%d", i );
      shader->texture_settings[i] = glGetUniformLocation( shader->program, name );
    }
  }

  return shaders.add( shader );
}

int  OpenGLRenderer::define_texture( void* pixels, int width, int height, int options )
{
  OpenGLTexture* texture = new OpenGLTexture();
  int bpp = options & (32|16|8);

  texture->width  = width;
  texture->height = height;
  texture->options = options;

  glGenTextures( 1, &texture->gl_id );
  glBindTexture( GL_TEXTURE_2D, texture->gl_id );

  switch (bpp)
  {
    case 32:
      glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
      break;

    case 16:
      {
        // Convert 16-bit ARGB to 16-bit RGBA
        int count = width*height;
        UInt16* cursor = ((UInt16*) pixels) - 1;
        while (--count >= 0)
        {
          UInt16 pixel = *(++cursor);
          *cursor = (UInt16)
                  ( ((pixel<<4) & 0x0f00)
                  | ((pixel>>4)&0xf0) 
                  | ((pixel<<12)&0xf000)
                  | ((pixel>>12)&15) );
        }
      }
      glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, pixels );
      break;
  }

  return textures.add( texture );
}

void OpenGLRenderer::delete_shader( int shader_id )
{
  OpenGLShader* shader = (OpenGLShader*) shaders.remove( shader_id );
  if ( !shader ) return;

  glDeleteShader( shader->pixel_shader );
  glDeleteShader( shader->pixel_shader );
  glDeleteProgram( shader->program );

  delete shader;
}

void OpenGLRenderer::delete_texture( int texture_id )
{
  OpenGLTexture* texture = (OpenGLTexture*) textures.remove( texture_id );
  if ( !texture ) return;

  glDeleteTextures( 1, &texture->gl_id );

  delete texture;
}

void OpenGLRenderer::flush()
{
  render();
  glFlush();
}

void OpenGLRenderer::render()
{
  GLfloat m[16];
  OpenGLShader* shader;

  if ( !vertex_count ) return;

  //glDisable( GL_CULL_FACE );

  RenderMode render_mode = this->render_mode;
  if ( !active_shader_id )
  {
    // Choose a built-in shader based on the render mode.
    active_shader_id = color_shader;

    if (render_mode.textures_enabled())
    {
      switch (render_mode.value & RenderMode::VERTEX_COLOR_MODE_MASK)
      {
        case RenderMode::VERTEX_COLOR_MULTIPLY:
          active_shader_id = texture_shader_with_color_multiply;
          break;
        case RenderMode::VERTEX_COLOR_ADD:
          active_shader_id = premultiplied_texture_shader_with_color_add;
          break;
        case RenderMode::VERTEX_COLOR_FILL:
          active_shader_id = premultiplied_texture_shader_with_color_fill;
          break;
        default:
          active_shader_id = texture_shader;
      }
    }
  }

  shader = (OpenGLShader*) shaders.get( active_shader_id );
  if ( !shader ) return;

  glViewport( 0, 0, display_width, display_height );

  projection_transform.to_float( m );

  if (render_mode.blend_enabled())
  {
    int gl_src_blend, gl_dest_blend;

    switch (render_mode.src_blend())
    {
      case RenderMode::BLEND_ZERO:              gl_src_blend = GL_ZERO;                break;
      case RenderMode::BLEND_ONE:               gl_src_blend = GL_ONE;                 break;
      case RenderMode::BLEND_SRC_ALPHA:         gl_src_blend = GL_SRC_ALPHA;           break;
      case RenderMode::BLEND_INVERSE_SRC_ALPHA: gl_src_blend = GL_ONE_MINUS_SRC_ALPHA; break;
      default: 
        printf( "Superbright OpenGL Renderer: Invalid source blend (%d), defaulting to BLEND_ONE.\n", render_mode.src_blend() );
        gl_src_blend = GL_ONE;
    }

    switch (render_mode.dest_blend())
    {
      case RenderMode::BLEND_ZERO:              gl_dest_blend = GL_ZERO;                break;
      case RenderMode::BLEND_ONE:               gl_dest_blend = GL_ONE;                 break;
      case RenderMode::BLEND_SRC_ALPHA:         gl_dest_blend = GL_SRC_ALPHA;           break;
      case RenderMode::BLEND_INVERSE_SRC_ALPHA: gl_dest_blend = GL_ONE_MINUS_SRC_ALPHA; break;
      default: 
        printf( "Superbright OpenGL Renderer: Invalid destination blend (%d), defaulting to BLEND_ZERO.\n", render_mode.dest_blend() );
        gl_dest_blend = GL_ONE;
    }

    glEnable( GL_BLEND );
    glBlendFunc( gl_src_blend, gl_dest_blend );
  }
  else
  {
    glDisable( GL_BLEND );
  }

  if (active_texture_count && render_mode.textures_enabled())
  {
    int i;
    for (i=active_texture_count; --i>=0; )
    {
      int texture_id = active_texture_ids[i];
      OpenGLTexture* texture = (OpenGLTexture*) textures.get( texture_id );

      if (texture)
      {
        glActiveTexture( GL_TEXTURE0+i );
        glBindTexture( GL_TEXTURE_2D, texture->gl_id );
        glUniform1i( shader->texture_settings[i], i );
      }

      if (render_mode.wrap_h_enabled())
      {
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
      }
      else
      {
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
      }

      if (render_mode.wrap_v_enabled())
      {
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
      }
      else
      {
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
      }

      if (render_mode.point_filter_enabled())
      {
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
      }
      else
      {
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
      }
    }
  }

  // ---- Set Vertex Color Mode -----------------------------------------------
  // Copy vertex colors and set up vertex color mode
  Vertex* src_vertex = vertices - 1;
  GLint*    dest_color = color_buffer - 1;
  int count = vertex_count;

  // Swap red and blue AND multiply R,G,B by A.
  while (--count >= 0)
  {
    int color = (++src_vertex)->color;
    int a = (color >> 24) & 255;
    int r = (((color >> 16) & 255) * a) / 255;
    int g = (((color >> 8) & 255) * a) / 255;
    int b = ((color & 255) * a) / 255;
    *(++dest_color) = (a << 16) | (b << 16) | (g << 8) | r;
  }

  // Swap red and blue without premultiplying vertex color
  //while (--count >= 0)
  //{
  //  int color = (++src_vertex)->color;
  //  *(++dest_color) = (color & 0xff00ff00) | ((color >> 16) & 0xff) | ((color << 16) & 0xff0000);
  //}

  if (shader->uv_attribute >= 0)
  {
    // Copy position and data at the same time
    Vertex*         src_vertex = vertices - 1;
    OpenGLVertexXY* dest_xy   = vertex_buffer_xy - 1;
    OpenGLVertexUV* dest_uv   = vertex_buffer_uv - 1;
    int count = vertex_count;
    while (--count >= 0)
    {
      (++dest_xy)->x = (++src_vertex)->x;
      dest_xy->y     = src_vertex->y;
      (++dest_uv)->u = src_vertex->u;
      dest_uv->v     = src_vertex->v;
    }
  }
  else
  {
    // Copy position data only
    Vertex*         src_vertex = vertices - 1;
    OpenGLVertexXY* dest_xy   = vertex_buffer_xy - 1;
    int count = vertex_count;
    while (--count >= 0)
    {
      (++dest_xy)->x = (++src_vertex)->x;
      dest_xy->y     = src_vertex->y;
    }
  }

  // Configure shader
  glUseProgram( shader->program );

  glUniformMatrix4fv( shader->transform_setting, 1, GL_FALSE, m );
  glVertexAttribPointer( shader->position_attribute, 2, GL_FLOAT, GL_FALSE, 0, vertex_buffer_xy );
  glEnableVertexAttribArray( shader->position_attribute );

  if (shader->color_attribute >= 0)
  {
    glVertexAttribPointer( shader->color_attribute, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, color_buffer );
    glEnableVertexAttribArray( shader->color_attribute );
  }

  if (shader->uv_attribute >= 0)
  {
    glVertexAttribPointer( shader->uv_attribute, 2, GL_FLOAT, GL_FALSE, 0, vertex_buffer_uv );
    glEnableVertexAttribArray( shader->uv_attribute );
  }

  switch (primitive_type)
  {
    case Renderer::TRIANGLES:
      glDrawArrays( GL_TRIANGLES, 0, vertex_count );
      break;

    case Renderer::LINES:
      glDrawArrays( GL_LINES, 0, vertex_count );
      break;

    case Renderer::POINTS:
      glDrawArrays( GL_POINTS, 0, vertex_count );
      break;
  }

  // Cleanup
  if (active_texture_count && render_mode.textures_enabled())
  {
    int i;
    for (i=active_texture_count; --i>=0; )
    {
      glActiveTexture( GL_TEXTURE0+i );
    }
  }

  vertex_count = 0;
  memset( vertices, 0, sizeof(Vertex) * Renderer::VERTEX_BUFFER_COUNT );
}

}; // namespace Starbright
}; // namespace PROJECT_WORKSPACE
