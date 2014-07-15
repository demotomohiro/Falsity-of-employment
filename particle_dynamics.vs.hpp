/* File generated with Shader Minifier 1.1.4
 * http://www.ctrl-alt-test.fr
 */
#ifndef PARTICLE_DYNAMICS_VS_HPP_
# define PARTICLE_DYNAMICS_VS_HPP_

const char * const particle_dynamics_vs =
  "#version 330\n"
  "float x(vec3 m)"
  "{"
    "vec3 f=m+vec3(2.),s=fract(f*f.yzx*222.)+vec3(2.);"
    "f.y*=f.z*s.z;"
    "f.x*=f.y*s.y;"
    "return fract(f.x*f.x);"
  "}"
  "vec3 f(float m)"
  "{"
    "float f=m+2.;"
    "vec3 s=vec3(f,f*f,f*f*f),g=fract(s*222.)+vec3(2.);"
    "return fract(s*g.yzx*g.zxy);"
  "}"
  "float f(float f,float m)"
  "{"
    "return floor(f*m)/m;"
  "}"
  "float f(float f,float m,out float v)"
  "{"
    "float s=f*m,g=floor(s);"
    "v=s-g;"
    "return g/m;"
  "}"
  "float t(float m)"
  "{"
    "return sin(6.28319*m)*.5+.5;"
  "}"
  "float t(vec3 f,float m)"
  "{"
    "vec3 s=f*m,v=floor(s);"
    "float g=x((v+vec3(0.,0.,0.))/m),n=x((v+vec3(1.,0.,0.))/m),l=x((v+vec3(0.,1.,0.))/m),i=x((v+vec3(1.,1.,0.))/m),c=x((v+vec3(0.,0.,1.))/m),z=x((v+vec3(1.,0.,1.))/m),o=x((v+vec3(0.,1.,1.))/m),y=x((v+vec3(1.,1.,1.))/m);"
    "vec3 p=smoothstep(0.,1.,fract(s));"
    "return mix(mix(mix(g,n,p.x),mix(l,i,p.x),p.y),mix(mix(c,z,p.x),mix(o,y,p.x),p.y),p.z);"
  "}"
  "float n(float m)"
  "{"
    "return m/44100;"
  "}"
  "float m(float f)"
  "{"
    "return max(sin(6.28319*(sin(f*.25)+2.)*f),0.);"
  "}"
  "float s(float f)"
  "{"
    "return max(fract(f*2.)*8.-7.,0.);"
  "}"
  "float p(float f)"
  "{"
    "return max(fract(f*2.),0.);"
  "}"
  "float r(float m)"
  "{"
    "return max(sin(6.28319*m*2.)*2.-1.,0.);"
  "}"
  "float v(float m)"
  "{"
    "return max(sin(6.28319*m*2.+3.)*2.-1.,0.);"
  "}"
  "float e(float f)"
  "{"
    "return r(f)+v(f);"
  "}"
  "float l(float m)"
  "{"
    "return max(1.-m*m*.0625,0.);"
  "}"
  "float c(float m)"
  "{"
    "return floor(f(floor(m*12.)/12./60.).x*12.)/12.;"
  "}"
  "float i(float f)"
  "{"
    "return max(fract(f/60.*2.+.75)*4.-3.,0.);"
  "}"
  "float w(float f)"
  "{"
    "return max(fract(f/60.*2.+.25)*4.-3.,0.);"
  "}"
  "float o(float f)"
  "{"
    "return max(fract(f/60.*2.)*4.-3.,0.);"
  "}"
  "float d(float m)"
  "{"
    "return max(sin(12.5664*m/60.)*2.-1.,0.);"
  "}"
  "float a(float m)"
  "{"
    "return max(sin(12.5664*m/60.-1.5708)*2.-1.,0.);"
  "}"
  "float u(float m)"
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
  "float a(vec3 f,float m,float s)"
  "{"
    "return a(f,m)-s;"
  "}"
  "float g(vec3 m)"
  "{"
    "vec3 f=m-vec3(.05);"
    "f=fract(f*12.)-vec3(.5);"
    "return-a(f,4.,.55);"
  "}"
  "vec3 V(vec3 f)"
  "{"
    "float m=.0001;"
    "return normalize(vec3(g(f+vec3(m,0,0))-g(f+vec3(-m,0,0)),g(f+vec3(0,m,0))-g(f+vec3(0,-m,0)),g(f+vec3(0,0,m))-g(f+vec3(0,0,-m))));"
  "}"
  "uniform float sample_count;"
  "layout(location=0)in vec3 in_prev_pos;"
  "layout(location=1)in vec3 in_crnt_pos;"
  "out vec3 out_positions;"
  "void main()"
  "{"
    "float f=n(sample_count),m=float(gl_VertexID)/400000.;"
    "const float s=8.,p=.13;"
    "float v=m*3.14159*2.*s;"
    "vec2 z=vec2(cos(v*8.+m),sin(v*8.))*p*.3,x=vec2(cos(v),sin(v))*.13+vec2(.5)+z;"
    "vec3 t=vec3(x,m);"
    "if(l(f)>0.)"
      "{"
        "out_positions=t;"
        "return;"
      "}"
    "vec3 r=vec3(step(1.,in_crnt_pos)-step(0.,-in_crnt_pos)),e=vec3(step(1.,in_prev_pos)-step(0.,-in_prev_pos)),D=in_crnt_pos-r,u=in_prev_pos-r-e;"
    "out_positions=D;"
    "vec3 I=D-u;"
    "const float h=.005;"
    "if(dot(I,I)>h*h)"
      "I=normalize(I)*h;"
    "out_positions+=I*.98;"
    "vec3 b=vec3(0.);"
    "float Z=d(f),Y=i(f)*2.+Z*2.;"
    "b.x=sin(Y)*.02;"
    "b.z=cos(Y);"
    "b.y=(c(f)-.5)*2.*(1.-Z);"
    "out_positions+=b*.0132*d(f);"
    "out_positions+=-V(D)*.004*g(D)*a(f);"
    "float X=w(f);"
    "if(D.y<X&&X>.02)"
      "out_positions.y-=.0001;"
    "vec3 W=t-D;"
    "out_positions+=W*.002*y(f);"
    "if(o(f)<.5&&o(f)>0.)"
      "{"
        "vec3 U=D-vec3(.5);"
        "float T=dot(U,U);"
        "vec3 S=normalize(U);"
        "out_positions+=S/T*(o(f)-.2)*.0002;"
      "}"
  "}";

#endif // PARTICLE_DYNAMICS_VS_HPP_