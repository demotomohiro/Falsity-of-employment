/* File generated with Shader Minifier 1.1.4
 * http://www.ctrl-alt-test.fr
 */
#ifndef SOUND_VS_HPP_
# define SOUND_VS_HPP_

const char * const sound_vs =
  "#version 330\n"
  "float x(vec3 f)"
  "{"
    "vec3 v=f+vec3(2.),o=fract(v*v.yzx*222.)+vec3(2.);"
    "v.y*=v.z*o.z;"
    "v.x*=v.y*o.y;"
    "return fract(v.x*v.x);"
  "}"
  "vec3 f(float f)"
  "{"
    "float m=f+2.;"
    "vec3 v=vec3(m,m*m,m*m*m),o=fract(v*222.)+vec3(2.);"
    "return fract(v*o.yzx*o.zxy);"
  "}"
  "float f(float f,float m)"
  "{"
    "return floor(f*m)/m;"
  "}"
  "float f(float f,float m,out float o)"
  "{"
    "float v=f*m,h=floor(v);"
    "o=v-h;"
    "return h/m;"
  "}"
  "float m(float m)"
  "{"
    "return sin(6.28319*m)*.5+.5;"
  "}"
  "float m(vec3 f,float m)"
  "{"
    "vec3 v=f*m,e=floor(v);"
    "float s=x((e+vec3(0.,0.,0.))/m),o=x((e+vec3(1.,0.,0.))/m),h=x((e+vec3(0.,1.,0.))/m),z=x((e+vec3(1.,1.,0.))/m),I=x((e+vec3(0.,0.,1.))/m),t=x((e+vec3(1.,0.,1.))/m),g=x((e+vec3(0.,1.,1.))/m),n=x((e+vec3(1.,1.,1.))/m);"
    "vec3 p=smoothstep(0.,1.,fract(v));"
    "return mix(mix(mix(s,o,p.x),mix(h,z,p.x),p.y),mix(mix(I,t,p.x),mix(g,n,p.x),p.y),p.z);"
  "}"
  "float t(float m)"
  "{"
    "return m/44100;"
  "}"
  "float n(float f)"
  "{"
    "return max(sin(6.28319*(sin(f*.25)+2.)*f),0.);"
  "}"
  "float v(float f)"
  "{"
    "return max(fract(f*2.)*8.-7.,0.);"
  "}"
  "float r(float f)"
  "{"
    "return max(fract(f*2.),0.);"
  "}"
  "float p(float m)"
  "{"
    "return max(sin(6.28319*m*2.)*2.-1.,0.);"
  "}"
  "float s(float m)"
  "{"
    "return max(sin(6.28319*m*2.+3.)*2.-1.,0.);"
  "}"
  "float o(float f)"
  "{"
    "return p(f)+s(f);"
  "}"
  "float w(float m)"
  "{"
    "return max(1.-m*m*.0625,0.);"
  "}"
  "float e(float m)"
  "{"
    "return floor(f(floor(m*12.)/12./60.).x*12.)/12.;"
  "}"
  "float a(float f)"
  "{"
    "return max(fract(f/60.*2.+.75)*4.-3.,0.);"
  "}"
  "float l(float f)"
  "{"
    "return max(fract(f/60.*2.+.25)*4.-3.,0.);"
  "}"
  "float c(float f)"
  "{"
    "return max(fract(f/60.*2.)*4.-3.,0.);"
  "}"
  "float u(float m)"
  "{"
    "return max(sin(12.5664*m/60.)*2.-1.,0.);"
  "}"
  "float g(float m)"
  "{"
    "return max(sin(12.5664*m/60.-1.5708)*2.-1.,0.);"
  "}"
  "float i(float m)"
  "{"
    "return max(sin(12.5664*m/60.-1.5708-1.5708)*2.-1.,0.);"
  "}"
  "float y(float m)"
  "{"
    "return max(sin(12.5664*m/60.-1.5708-1.5708-1.5708)*2.-1.,0.);"
  "}"
  "float z(float m)"
  "{"
    "float f=m-60.;"
    "return max(1.-f*f*.0625,0.);"
  "}"
  "float a(vec3 f,float m)"
  "{"
    "vec3 v=pow(abs(f),vec3(m));"
    "return pow(v.x+v.y+v.z,1/m);"
  "}"
  "float a(vec3 f,float m,float v)"
  "{"
    "return a(f,m)-v;"
  "}"
  "float h(vec3 m)"
  "{"
    "vec3 f=m-vec3(.05);"
    "f=fract(f*12.)-vec3(.5);"
    "return-a(f,4.,.55);"
  "}"
  "vec3 V(vec3 f)"
  "{"
    "float m=.0001;"
    "return normalize(vec3(h(f+vec3(m,0,0))-h(f+vec3(-m,0,0)),h(f+vec3(0,m,0))-h(f+vec3(0,-m,0)),h(f+vec3(0,0,m))-h(f+vec3(0,0,-m))));"
  "}"
  "out vec2 out_sample;"
  "float V(float m,float v)"
  "{"
    "float x=f(v/1000.).x,e=floor(m*v)+x*.1,o=m*v-e,s=f(e/v/60.).x,p=f((e+1.)/v/60.).x;"
    "return mix(s,p,smoothstep(0.,1.,o));"
  "}"
  "float c(float f,float m)"
  "{"
    "float e=0;"
    "e+=V(f,m)*.5;"
    "e+=V(f,m*2.)*.25;"
    "e+=V(f,m*4.)*.125;"
    "e+=V(f,m*8.)*.0625;"
    "return e;"
  "}"
  "float I(float f)"
  "{"
    "return sign(fract(f)-.5);"
  "}"
  "float D(float f)"
  "{"
    "float m=fract(f)*2.;"
    "return m<1.?m:2-m;"
  "}"
  "float q(float f)"
  "{"
    "return-4.*f+4.*sqrt(f);"
  "}"
  "void main()"
  "{"
    "float x=t(gl_VertexID);"
    "const float s=6.28319;"
    "float p=x*s,h=floor(x*8.),r=exp(floor(f(h/8./60.).x*8.)*.25)+160.;"
    "out_sample=vec2(0.);"
    "const float w=.15;"
    "if(sin(6.28319*x/60.*8.)>0.)"
      "out_sample+=vec2(I(p*8.)+fract(p*7.95)+f(p)*.5)/2.5*w*v(x),out_sample+=vec2(I(p*32.)+fract(p*16.95)+f(p)*.5)/2.5*w*(v(x-.5)+v(x-.75));"
    "else"
      " out_sample+=vec2(I(p*32.)+fract(p*7.95)+f(p)*.5)/2.5*w*v(x*2.),out_sample+=vec2(I(p*32.)+fract(p*7.95)+f(p)*.5)/2.5*w*v(x*3.),out_sample+=vec2(I(p*16.)+fract(p*16.95)+f(p)*.5)/2.5*w*(v(x-.25)+v(x-.75));"
    "const float a=.5,l=a*2.;"
    "float V=x/60.,d=f(x/8.,16.)*.5,b=f(V,60./a),Z=f(V,60./l),Y,X=f(m(x/l),4.,Y),W=exp(floor(f(X+d).x*5.)*.5)+70.,U=exp(floor(f(Z).x*3.)*.5)+40.;"
    "out_sample+=vec2(m(p*W)*.25*D(Y));"
    "out_sample+=vec2(D(p*U)*.24*q(Y)+f(p*W).x*.01);"
    "float T=e(x),S=1.-v(x);"
    "out_sample+=c(x,1000.+exp(T*8.*.25)*2222.)*.25*S*S*u(x);"
    "out_sample+=m(x*(g(x)*222.+222.))*.2*o(x)*g(x);"
    "float R=sin(p*(x+1.))*2;"
    "out_sample+=vec2(sign(m(x*r+R)-.5))*.25*n(x)*i(x);"
    "out_sample+=vec2(m(x*r))*.25*y(x);"
    "out_sample*=1.-z(x);"
  "}";

#endif // SOUND_VS_HPP_
