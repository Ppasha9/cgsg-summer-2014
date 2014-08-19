/* FILE NAME   : default.vert
 * PURPOSE     : Simple vertex shader.
 * PROGRAMMER  : RK2.
 * LAST UPDATE : 15.08.2014
 */

#version 420

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;

uniform vec4 UseColor;

uniform mat4 MatrWorld;
uniform mat4 MatrView;
uniform mat4 MatrWVP;
uniform mat4 MatrWorldInverseTranspose;

uniform float Time;
uniform float Trans;

out vec4 FragColor;
out vec3 FragPos;
out vec2 FragTexCoord;
out vec3 FragNormal;
out vec4 CameraPos;
out vec3 CameraPosIn;

/* Main function */
void main( void )
{
  CameraPos = MatrWVP * vec4(InPosition.xyz, 1);
  CameraPosIn = InPosition;

  gl_Position = MatrWVP * vec4(InPosition.xyz, 1);
  FragNormal = mat3(MatrWorldInverseTranspose) * InNormal;
  FragColor = vec4(InColor.xyz, 0.0);
  FragPos = mat4x3(MatrWorld) * vec4(InPosition, 1);

  FragTexCoord = InTexCoord.st;
} /* End of 'main' function */

/* End of 'default.vert' file */
