/*
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#version 150

#define M_PI 3.1415926535897932384626433832795

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 Texcoord;
in vec3 Normal;
in vec3 FragPos_World;
out vec4 outColor;


uniform mat4 modelToWorld;
uniform sampler2D tex;
uniform float ambientFactor;
uniform vec3 viewPos;
uniform float specularStrength;
uniform Material material;
uniform Light light;

void main()
{
    vec3 tNormal = vec3(-cos(M_PI * Texcoord.x), 0, sin(M_PI * Texcoord.x));
    vec4 t = texture(tex, Texcoord);
    if (t.r == 0 && t.g == 0 && t.b == 0) {
        t = vec4(0.1, 0.1, 0.1, 1);
    }

    // ambient
    vec3 ambient = light.ambient * t.xyz;

    vec3 norm = normalize(tNormal);
    vec3 lightDir = normalize(light.position - FragPos_World);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * t.xyz);

    vec3 viewDir = normalize(viewPos - FragPos_World);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * light.specular;

    vec3 result = ambient + diffuse + specular;
    outColor = vec4(result, t.a);
}
