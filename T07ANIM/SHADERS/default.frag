/* FILE NAME   : default.frag
 * PURPOSE     : Simple fragment shader.
 * PROGRAMMER  : RK2.
 * LAST UPDATE : 15.08.2014
 */

#version 420

/* Result color */
out vec4 OutColor;

/* Global program data */
uniform float Time;
uniform vec4 LightPosition;
uniform vec4 LightColor;
uniform vec3 ViewDir;

/* Drawing material */
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Phong;
uniform float Trans;

uniform vec4 UseColor;

uniform mat4 MatrWVP;
uniform mat4 MatrWorldInverseTranspose;
uniform mat4 MatrWorld;

uniform float WaterLevel;

/* Texture */
uniform sampler2D DrawTexture;

/* Input data */
in vec4 FragColor;
in vec3 FragPos;
in vec2 FragTexCoord;
in vec3 FragNormal;

in vec4 CameraPos;
in vec3 CameraPosIn;

vec3 Illum( vec3 N )
{
  vec4 texc = texture2D(DrawTexture, FragTexCoord.xy);

  vec3 color = Ka;
  vec3 Dir = mat3(MatrWorld) * ViewDir; 

  vec3 lPos = vec3(0, 100, 0);
  vec3 l = normalize(lPos - FragPos);

  N = faceforward(N, ViewDir, N);
  float nl = dot(N, l);
  
  color = (texc.xyz + Kd) * abs(nl);
  
  vec3 R = reflect(Dir, N);
  R = Dir - N * (2 * dot(Dir, N));
  float rl = dot(R, l);
  if (rl > 0)
    color += Ks * pow(dot(R, l), 14);
  
  return color;
}

/* Main function */
void main( void )
{
  
  float start = 300, end = 800;
  // vec4 pos = vec4(DrawPos.xyz, 1) * MatrWorld;
  //float start = 0, end = 10;
  
  float dist = CameraPos.z;
  float t = 0.5;
  
  if (dist < start)
    t = 1;
  else if (dist > end)
    t = 0;
  else
    t = 1 - (dist - start) / (end - start);
  // OutColor = vec4((vec3(dist, dist, dist) / 2000).xyz, 1);
  // OutColor = vec4(DrawNormal.xyz, Trans);
  // OutColor = vec4(Illum(DrawNormal).xyz, Trans);
  OutColor = vec4(Illum(FragNormal).xyz, Trans) * t + vec4(0.3, 0.3, 0.3, 1) * (1 - t);
  // OutColor = vec4(Illum(FragNormal).xyz, Trans);
  if (CameraPosIn.y < WaterLevel)
    OutColor = vec4((OutColor.xyz + vec3(0, 0.1, 0.3)).xyz, Trans);
} /* End of 'main' function */

/* End of 'default.frag' file */
