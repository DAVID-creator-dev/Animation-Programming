
/////////////////////
// INPUT VARIABLES //
/////////////////////
in lowp vec3 inputPosition;
in lowp vec3 normal;
in lowp vec4 boneIndices;
in lowp vec4 boneWeights;

//////////////////////
// OUTPUT VARIABLES //
//////////////////////
smooth out vec2 texCoord;
smooth out vec3 outNormal;

uniform SceneMatrices
{
	uniform mat4 projectionMatrix;
} sm;

uniform mat4 modelViewMatrix;

uniform SkinningMatrices
{
	uniform mat4 mat[64];
} skin;

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////

void main(void)
{
    vec4 pos = vec4(inputPosition, 1.0);  
    vec4 norm = vec4(normal, 0.0f);  

    vec4 posSkinned = vec4(0.0f);  
    vec4 normSkinned = vec4(0.0f);

    for (int i = 0; i < 4; i++) {
        int boneIndex = int(boneIndices[i]);  
        if (boneIndex >= 0) {
            mat4 bone = skin.mat[boneIndex];  
            float weight = boneWeights[i];  

            posSkinned += (bone * pos) * weight;  
            normSkinned += (bone * norm) * weight;
        }
    }
    posSkinned = vec4(posSkinned.xyz, 1.0);  

    gl_Position = sm.projectionMatrix * (modelViewMatrix * posSkinned);

    mat3 normalMatrix = transpose(inverse(mat3(modelViewMatrix)));  
    outNormal = normalize(normalMatrix * normSkinned.xyz);
}

