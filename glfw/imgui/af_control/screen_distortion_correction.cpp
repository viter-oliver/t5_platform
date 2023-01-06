#include "screen_distortion_correction.h"
#include "af_shader.h"
#include "af_primitive_object.h"
#include "common_functions.h"
#include "inverse-matrix.hpp"

namespace screen_distortion_correction {
const char *vs_code = R"glsl(#version 300 es
	      precision mediump float;
	      in vec2 position;
        uniform mat2 customMtx;
        uniform vec2 customDelta;
        out vec2 outTxtCd;
        void main()
        {
             outTxtCd=(position+vec2(1,1))*0.5;
             gl_Position=vec4(position.xy,0.0,1.0);
        }
		)glsl";
const char *fs_code = R"glsl(#version 300 es
	      precision mediump float;
        in vec2 outTxtCd;
        uniform sampler2D text;
	      out vec4 o_clr;

        uniform float px[15];
        uniform float py[15];
		    uniform int distortion_file_valid; 
        void main()
        {
          vec2 tmpPos= outTxtCd;
          vec2 Texcoord;
          Texcoord.x = px[0]
          +px[1] * tmpPos.x
          +px[2] * tmpPos.x  * tmpPos.x 
          +px[3] * tmpPos.x  * tmpPos.x  * tmpPos.x 
          +px[4] * tmpPos.x  * tmpPos.x  * tmpPos.x  * tmpPos.x 
          +px[5] * tmpPos.y 
          +px[6] * tmpPos.x  * tmpPos.y 
          +px[7] * tmpPos.x  * tmpPos.x  * tmpPos.y
          +px[8] * tmpPos.x  * tmpPos.x  * tmpPos.x  * tmpPos.y 
          +px[9] * tmpPos.y  * tmpPos.y 
          +px[10] * tmpPos.x  * tmpPos.y  * tmpPos.y 
          +px[11]  * tmpPos.x  * tmpPos.x  * tmpPos.y  * tmpPos.y 
          +px[12] * tmpPos.y  * tmpPos.y  * tmpPos.y 
          +px[13] * tmpPos.x  * tmpPos.y  * tmpPos.y  * tmpPos.y 
          +px[14] * tmpPos.y  * tmpPos.y  * tmpPos.y  * tmpPos.y ;
          Texcoord.y = py[0]
          +py[1] * tmpPos.x
          +py[2] * tmpPos.x  * tmpPos.x 
          +py[3] * tmpPos.x  * tmpPos.x  * tmpPos.x 
          +py[4] * tmpPos.x  * tmpPos.x  * tmpPos.x  * tmpPos.x 
          +py[5] * tmpPos.y 
          +py[6] * tmpPos.x  * tmpPos.y 
          +py[7] * tmpPos.x  * tmpPos.x  * tmpPos.y
          +py[8] * tmpPos.x  * tmpPos.x  * tmpPos.x  * tmpPos.y 
          +py[9] * tmpPos.y  * tmpPos.y 
          +py[10] * tmpPos.x  * tmpPos.y  * tmpPos.y 
          +py[11] * tmpPos.x  * tmpPos.x  * tmpPos.y  * tmpPos.y 
          +py[12] * tmpPos.y  * tmpPos.y  * tmpPos.y 
          +py[13] * tmpPos.x  * tmpPos.y  * tmpPos.y  * tmpPos.y 
          +py[14] * tmpPos.y  * tmpPos.y  * tmpPos.y  * tmpPos.y ;
          o_clr = texture(text, Texcoord);
        }
		)glsl";
float _width, _height;
ps_shader _pshader;
ps_primrive_object _ps_prm; 
std::vector<float> px(15);
std::vector<float> py(15);
GLuint _VboHandle;
GLuint _fboId;
GLint _prev_fbo;
GLuint _colorTextId;
GLuint _depthStencilTextId;
GLfloat _plain_vertices[] = {
    -1.f, -1.f, 
    1.f, -1.f, 
    -1.f, 1.f, 
    1.f, 1.0f,
};

void init(float width, float height) {
  _width = width;
  _height = height;
  _pshader = make_shared<af_shader>(vs_code, fs_code);
  
  _ps_prm = make_shared<primitive_object>();
  _ps_prm->set_ele_format({ 2 });
  _ps_prm->load_vertex_data(_plain_vertices, 
  sizeof(_plain_vertices) / sizeof(float));
  prepareFBO1(_colorTextId, _depthStencilTextId, _fboId, _width, _height);
}
void uninit() {
  glDeleteBuffers(1, &_VboHandle);
  glDeleteTextures(1, &_colorTextId);
  glDeleteTextures(1, &_depthStencilTextId);
  glDeleteFramebuffers(1, &_fboId);
}
void bind() {
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &_prev_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
}
void disbind() { glBindFramebuffer(GL_FRAMEBUFFER, _prev_fbo); }
void draw() {
  glViewport(0, 0, _width, _height);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  _pshader->use();
  _pshader->uniform("px[0]", &px[0]);
  _pshader->uniform("py[0]", &py[0]);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _colorTextId);
  _pshader->uniform("text",0);
  glBindVertexArray(_ps_prm->_vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void calculate_params(float input_point[15][2],float output_point[15][2]) {
  using efmatrix=extendable_matrix::Matrix<float>;
  efmatrix matrix_cal(15),matrix_inverse(15);
  float(* ipt)[2]=input_point;
  float(* opt)[2]=output_point;
  for(int ix=0;ix<15;++ix){
    matrix_cal[ix]={
    1,
    ipt[ix][0],
    pow(ipt[ix][0],2),
    pow(ipt[ix][0],3),
    pow(ipt[ix][0],4),
    ipt[ix][1],
    ipt[ix][0]*ipt[ix][1],
    pow(ipt[ix][0],2)*ipt[ix][1],
    pow(ipt[ix][0],3)*ipt[ix][1],
    pow(ipt[ix][1],2),
    ipt[ix][0]*pow(ipt[ix][1],2),
    pow(ipt[ix][0],2)*pow(ipt[ix][1],2),
    pow(ipt[ix][1],3),
    ipt[ix][0]*pow(ipt[ix][1],3),
    pow(ipt[ix][1],4),
    };
  }
  matrix_cal.Inverse(matrix_inverse);
  std::vector<float> xvout={
    opt[0][0],opt[1][0],opt[2][0],opt[3][0],opt[4][0],
    opt[5][0],opt[6][0],opt[7][0],opt[8][0],opt[9][0],
    opt[10][0],opt[11][0],opt[12][0],opt[13][0],opt[14][0],
  };
  matrix_inverse.multiply(xvout,px);
  std::vector<float> yvout={
    opt[0][1],opt[1][1],opt[2][1],opt[3][1],opt[4][1],
    opt[5][1],opt[6][1],opt[7][1],opt[8][1],opt[9][1],
    opt[10][1],opt[11][1],opt[12][1],opt[13][1],opt[14][1],
  }; 
  matrix_inverse.multiply(yvout,py);
}
} // namespace screen_distortion_correction