#version 420 
// vertex01.glsl

//uniform mat4 MVP;		
uniform mat4 matModel;		// M
uniform mat4 matModelInvTrans; // inverse(transpose(matModel))
uniform mat4 matView;		// V
uniform mat4 matProj;		// P

in vec4 vColour;		// rgb   	was "attribute"
in vec4 vPosition;		// xyz		was "attribute"
in vec4 vNormal;        // normal xyz
in vec4 vUV_x2;			// Texture coordinates (2 of them)
in vec4 vTanXYZ;			// Tangent to the surface
in vec4 vBiNormXYZ;		// bi-normal (or bi-tangent) to the surface

in vec4 vBoneID;		// really passing it as 4 values
in vec4 vBoneWeight;	

out vec4 color;			// exit to fragment
out vec4 vertPosWorld;  // "World space"
out vec4 vertNormal;    // "Model space"
out vec4 vertUV_x2;		// To the next shader stage
out vec4 vertTanXYZ;	// Tangent to the surface
out vec4 vertBiNormXYZ;	// bi-normal (or bi-tangent) to the surface

// For skinned mesh
const int MAXNUMBEROFBONES = 100;
uniform mat4 bones[MAXNUMBEROFBONES];
// Pass the acutal number you are using to control how often this loops
uniform int numBonesUsed;			
uniform bool bIsASkinnedMesh;	// True to do the skinned mesh

uniform bool instanceDraw;

void main()
{
	vec3 posTemp = vPosition.xyz;

	color = vColour;
	
	// Pass the texture coordinates out, unchanged.
	vertUV_x2 = vUV_x2;
	
	// Also pass the bi-tangent (bi-normal) and tangent to fragment
	vertTanXYZ = vTanXYZ;		// Tangent to the surface
	vertBiNormXYZ = vBiNormXYZ;

	// Skinned mesh or not?
	if ( bIsASkinnedMesh )
	{	
		//*****************************************************************
		// Before I do the full MVP (screen space) calculation, 
		// I'm going to adjust where the vertex is based on the 
		// the bone locations
		
		mat4 BoneTransform 
		               = bones[ int(vBoneID[0]) ] * vBoneWeight[0];
		BoneTransform += bones[ int(vBoneID[1]) ] * vBoneWeight[1];
		BoneTransform += bones[ int(vBoneID[2]) ] * vBoneWeight[2];
		BoneTransform += bones[ int(vBoneID[3]) ] * vBoneWeight[3];
		// You could also do bBoneID.x, .y, etc.
				
	//	matrixWorld = BoneTransform * matrixWorld;
		
		vec4 vertPositionFromBones = BoneTransform * vec4(posTemp.xyz, 1.0);
		//*****************************************************************
	
		mat4 MVP = matProj * matView * matModel;
		gl_Position = MVP * vertPositionFromBones;			// ON SCREEN
		
		vertPosWorld = matModel * vec4(posTemp, 1.0);
		
		
		
		// Transforms the normal into "world space"
		// Remove all scaling and transformation from model
		// Leaving ONLY rotation... 
		
		// This only leaves rotation (translation and scale are removed)
		mat4 matBoneTransform_InTrans = inverse(transpose(BoneTransform));
		
		vec4 vNormBone = matBoneTransform_InTrans * vec4(normalize(vNormal.xyz),1.0f);
		
		vertNormal = matModelInvTrans * vNormBone;
		
		
		// And then you do the same for the binormal and bitangent
		vec4 vTanXYZ_Bone = matBoneTransform_InTrans * vTanXYZ;
		vertTanXYZ = matModelInvTrans * vTanXYZ_Bone;
		
		vec4 vBiNormXYZ_Bone = matBoneTransform_InTrans * vBiNormXYZ;
		vertBiNormXYZ = matModelInvTrans * vBiNormXYZ_Bone;
		
		
		// Debug: make green if this flag is set
		color.rgb = vec3(0.0f, 1.0f, 0.0f);
	}
	else
	{
		// Is a regular mesh
		// Note these are 'backwards'
		mat4 MVP = matProj * matView * matModel;
		gl_Position = MVP * vec4(posTemp, 1.0f);			// ON SCREEN
		
		vertPosWorld = matModel * vec4(posTemp, 1.0f);
		
		// Transforms the normal into "world space"
		// Remove all scaling and transformation from model
		// Leaving ONLY rotation... 
		vertNormal = matModelInvTrans * vec4(normalize(vNormal.xyz),1.0f);
	
	}//if ( bIsASkinnedMesh )
	
	/***************old code****************/
//	// Note these are 'backwards'
//	mat4 MVP = matProj * matView * matModel;
//    gl_Position = MVP * vec4(posTemp, 1.0); // ON SCREEN
//	
//	vertPosWorld = matModel * vec4(posTemp, 1.0);
//	
//	// Transforms the normal into "world space"
//	// Remove all scaling and transformation from model
//	// Leaving ONLY rotation... 
//	vertNormal = vec3( matModelInvTrans * vec4(normalize(vNormal),1.0f) );
//
//    color = vColour;
//
//	// Pass the texture coordinates out, unchanged.
//	vertUV_x2.xy = vUV_x2.xy;
}



