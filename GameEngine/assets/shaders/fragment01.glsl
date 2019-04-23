#version 420 
//fragment01.glsl

// In from a previous stage (vertex shader)
in vec4 color;		// in from the vertex shader
in vec4 vertPosWorld;
in vec4 vertNormal;	// "Model space" (only rotation)
in vec4 vertUV_x2;		// Texture coordinates

in vec4 vertTanXYZ;			// Tangent to the surface
in vec4 vertBiNormXYZ;		// bi-normal (or bi-tangent) to the surface

uniform bool isWater1;
uniform bool isWater2;

uniform vec4 objectDiffuse;
uniform vec4 objectSpecular;

uniform vec3 eyeLocation;
//uniform vec3 objectColour;
// Set this to true (1), and the vertex colour is used
uniform bool useVertexColour;
uniform bool bDontUseLighting;	

uniform vec4 waterOffset;

//vec4 gl_FragColor
out vec4 finalOutputColour;		// Any name, but must be vec4
out vec4 finalOutputNormal;			// GL_COLOR_ATTACHMENT1
out vec4 finalOutputVertWorldPos;	// GL_COLOR_ATTACHMENT2

struct sLight
{
	vec4 position;			
	vec4 diffuse;		
	vec4 specular;	// rgb = highlight colour, w = power
	vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	vec4 direction;	// Spot, directional lights
	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
	vec4 param2;	// x = 0 for off, 1 for on
};

const int POINT_LIGHT_TYPE = 0;
const int SPOT_LIGHT_TYPE = 1;
const int DIRECTIONAL_LIGHT_TYPE = 2;

const int NUMBEROFLIGHTS = 20;
uniform sLight theLights[NUMBEROFLIGHTS];  	// 80 uniforms

// CAN'T put texture samplers into an array (sadly)
//uniform sampler textures[10];

uniform sampler2D texture00;
uniform sampler2D texture01;
uniform sampler2D texture02;
uniform sampler2D texture03;
uniform sampler2D texture04;
uniform sampler2D texture05;
uniform sampler2D texture06;
uniform sampler2D texture07;

// This is 4 x 2 floats or 8 floats
uniform vec4 texBlendWeights[2];	// x is 0, y is 1, z is 2

// Cube map texture (NOT a sampler3D)
uniform samplerCube textureSkyBox;
uniform samplerCube textureSpace;
uniform float dayMix;

uniform bool useSkyBoxTexture;

//alpha transperancy
uniform float wholeObjectAlphaTransparency;

// For the 2 pass rendering
uniform float renderPassNumber;	// 1 = 1st pass, 2nd for offscreen to quad
uniform sampler2D texPass1OutputTexture;
uniform bool isQuad;

//uniform float screenWidth;
//uniform float screenHeight;

uniform bool isReflective;
uniform bool isRefractive;

uniform bool blur;
uniform bool useReticle;

uniform bool bIsParticleImposter;
// Used to fade the particle during "death" (set to 1.0f if not being used)
uniform float ParticleImposterBlackThreshold;
uniform float alphaOverride;	
uniform bool toon;

void main()
{
	// output black to all layers
	finalOutputColour = vec4( 0.0f, 0.0f, 0.0f, 1.0f );
	finalOutputNormal = vec4( 0.0f, 0.0f, 0.0f, 1.0f );
	finalOutputVertWorldPos = vertPosWorld;

//	if ( bIsParticleImposter == true ) 
//	{
//		// Discard anything that's "black enough"
//		vec4 tex0Col = texture( texture00, vertUV_x2.st ).rgba;	// Imposter texture (smoke, fire)
//		
//		// Out shader was only testing for black and white, 
//		// but now we have coloured textures, so use all the colours to 
//		// test for "black".
//		// Alternatively, you can use a 2nd texture as a "mask" (black and white) texture
//		
//		// Note "greyscale", that humans see, isn't even over the colours. 
//		// (google "RGB to greyscale" to get this equation:)
//		float pixelBlackAndWhite = (0.3f * tex0Col.r) + (0.59f * tex0Col.g) + (0.11f * tex0Col.b);
//		
//		// We had a threshold of 0.25, but how it's passed in
//		if ( pixelBlackAndWhite < ParticleImposterBlackThreshold )
//		{
//			// Don't draw it
//			// Literally Doesn't Draw the pixel
//			discard;
//		}	
//		// OK, I'm drawing the particle imposter...
//		// The darker it is, the more transparent it should be
//		// Colour is from 0 to 1  (white = 1)
//		// Alpha goes from 0 to 1, too (oh my goodness)
//		finalOutputColour.a = pixelBlackAndWhite;
//		finalOutputColour.rgb += tex0Col.rgb;
//		
////		finalOutputColour.rgb = vec3(1.0f, 0.0f, 0.0f);
//		// Override the "fade at death" transparency, too
////		finalOutputColour.a *= alphaOverride;
////		finalOutputColour.a = 1.0f;
//
//		finalOutputColour = vec4(1.0f, 0.0f, 0.0f, 1.0f);
////		finalOutputColour.r = 1.0f - ParticleImposterAlphaOverride;
//		finalOutputNormal.rgb = vertNormal.xyz;
//		finalOutputNormal.r = 1.0f;
//		finalOutputNormal.a = 1.0f;
//		return;
//	}//if(bIsParticleImposter)

	if ( int(renderPassNumber) == 2 )
	{ 
		if(useReticle == true)
		{
			vec4 tex1stPassCol = texture( texPass1OutputTexture, vertUV_x2.st ).rgba;
//			finalOutputColour.rgb = texture( texPass1OutputTexture, vertUV_x2.st ).rgb;
//			finalOutputColour.a = 1.0f;
			
			vec4 tex2Col = texture( texture02, vertUV_x2.st ).rgba;

			finalOutputColour.rgba = (tex1stPassCol * 0.8f) 	 // 0
		                  + (tex2Col * texBlendWeights[0].z);

//			return;
		}else
		{
			// 2nd pass (very simple)
	//		vec2 textCoords = vec2( vertUV_x2.s / screenWidth, vertUV_x2.t / screenHeight );
			finalOutputColour.rgb = texture( texPass1OutputTexture, vertUV_x2.st ).rgb;
			finalOutputColour.a = 1.0f;

		}
		if(blur)
		{
			float step = 0.001f;
			float size = 0.003f;
	//		
			vec3 vecSample = vec3(0.0f, 0.0f, 0.0f );
			int count = 0;
		
			for ( float x = -size; x <= size; x += step )
			{
				for ( float y = -size; y <= size; y += step )
				{
					count++;
					vecSample += texture( texPass1OutputTexture, vertUV_x2.st + vec2(x, y)).rgb;
				}
			}
			finalOutputColour.rgb = (vecSample.rgb / float(count));
		}
		return;
//		if(isQuad){
//			// 2nd pass (very simple)
//			vec4 tex1stPassCol = texture( texPass1OutputTexture, vertUV_x2.st ).rgba;
////			finalOutputColour.rgb = texture( texPass1OutputTexture, vertUV_x2.st ).rgb;
////			finalOutputColour.a = 1.0f;
//			
//			vec4 tex2Col = texture( texture02, vertUV_x2.st ).rgba;
//
//			finalOutputColour.rgba = (tex1stPassCol * 0.8f) 	 // 0
//		                  + (tex2Col * texBlendWeights[0].z);
//
//			return;
//		}
	}

	vec4 materialDiffuse = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec4 materialSpecular = objectSpecular;

	// is this the skybox texture?
	if (useSkyBoxTexture == true)
	{
		//finalOutputColour.rgb = vec3(1.0f, 0.0f, 0.0f);

		// Note for cube maps, the texture coordinates are 3D
		// (so here we are using the normal on the surface 
		//  of the sphere, like a "ray cast" really)
		vec3 skyPixelColour = texture( textureSkyBox, vertNormal.xyz ).rgb;
		vec3 skyBoxSpace = texture(textureSpace, vertNormal.xyz).rgb;

		vec3 skyBoxColor = (skyPixelColour * dayMix) + (skyBoxSpace * (1.0f - dayMix));
		
		finalOutputColour.rgb = skyBoxColor;
		finalOutputColour.a = 1.0f;
		return;
	}

	if(isReflective == true || isRefractive == true)
	{
		float ratio = 1.00 / 1.52;
		vec3 I = normalize(vertPosWorld.xyz - eyeLocation);
		vec3 R = vec3(0.0f, 0.0f, 0.0f);
		if(isReflective == true){
			R = reflect(I, normalize(vertNormal.xyz)); 
		}else{
			R = refract(I, normalize(vertNormal.xyz), ratio);
		}
		finalOutputColour = vec4(texture( textureSkyBox, R ).rgb, 1.0f);
		return;
	}

	
	if ( useVertexColour )
	{
		//gl_FragColor = vec4(color, 1.0);
		materialDiffuse = color;
	}
	else
	{
		vec4 vertUV = vertUV_x2;
		if(isWater1){
			vertUV.s += waterOffset.x;
			vertUV.t += waterOffset.y;
		}
		if(isWater2){
			vertUV.s += waterOffset.z;
			vertUV.t += waterOffset.w;
		}
		vec4 tex0Col = texture( texture00, vertUV.st ).rgba;
		vec4 tex1Col = texture( texture01, vertUV.st ).rgba;
		vec4 tex2Col = texture( texture02, vertUV.st ).rgba;
		vec4 tex3Col = texture( texture03, vertUV.st ).rgba;
		vec4 tex4Col = texture( texture04, vertUV.st ).rgba;
		vec4 tex5Col = texture( texture05, vertUV.st ).rgba;
		vec4 tex6Col = texture( texture06, vertUV.st ).rgba;
		vec4 tex7Col = texture( texture07, vertUV.st ).rgba;
		
		materialDiffuse =  objectDiffuse
						  + (tex0Col * texBlendWeights[0].x) 	 // 0
		                  + (tex1Col * texBlendWeights[0].y)     // 1
						  + (tex2Col * texBlendWeights[0].z)     // 2
		                  + (tex3Col * texBlendWeights[0].w)     // 3
		                  + (tex4Col * texBlendWeights[1].x)     // 4
		                  + (tex5Col * texBlendWeights[1].y)     // 5
		                  + (tex6Col * texBlendWeights[1].z)     // 6
		                  + (tex7Col * texBlendWeights[1].w);    // 7
		//gl_FragColor = vec4(objectColour, 1.0);
//		materialDiffuse = objectDiffuse;
//		return;
	}

	// Is this being lit? 
	if ( bDontUseLighting )
	{
		// Just exit early
		finalOutputColour = objectDiffuse;

		finalOutputNormal.rgb = vertNormal.xyz;
		finalOutputNormal.a = 1.0f;

		if(toon){
			vec3 viewVector = eyeLocation.xyz - vertPosWorld.xyz;
			viewVector = normalize(viewVector);

			vec3 pointNormal = normalize(vertNormal.xyz);
			float angleDot = dot(viewVector, pointNormal);

		//	if(angle >= -0.174533f && angle <= 0.174533f){
			if(angleDot > 0.95){
				finalOutputColour.rgb *= 0.95f; 
			}
			else if(angleDot > 0.5){
				finalOutputColour.rgb *= 0.6f; 
			}
			else if(angleDot > 0.25){
				finalOutputColour.rgb *= 0.15f; 
			}
			else {
				finalOutputColour.rgb = vec3(0.0f, 0.0f, 0.0f);
			}
		}

		return;
	}

	vec3 norm = normalize(vertNormal.xyz);
	vec4 finalObjectColour = vec4( 0.0f, 0.0f, 0.0f, 1.0f );

	for ( int index = 0; index < NUMBEROFLIGHTS; index++ )
	{	
		// ********************************************************
		// is light "on"
		if ( theLights[index].param2.x == 0.0f )
		{	// it's off
			continue;
		}

		int intLightType = int(theLights[index].param1.x);

		// We will do the directional light here... 
		// (BEFORE the attenuation, since sunlight has no attenuation, really)
		if ( intLightType == DIRECTIONAL_LIGHT_TYPE )		// = 2
		{
			// This is supposed to simulate sunlight. 
			// SO: 
			// -- There's ONLY direction, no position
			// -- Almost always, there's only 1 of these in a scene
			// Cheapest light to calculate. 

			vec3 lightContrib = theLights[index].diffuse.rgb;
			
			// Get the dot product of the light and normalize
			float dotProduct = dot( -theLights[index].direction.xyz,  
									   normalize(norm.xyz) );	// -1 to 1

			dotProduct = max( 0.0f, dotProduct );		// 0 to 1
			
			lightContrib *= dotProduct;		
			
			finalObjectColour.rgb += (materialDiffuse.rgb * theLights[index].diffuse.rgb * lightContrib); 
									 //+ (materialSpecular.rgb * lightSpecularContrib.rgb);
			// NOTE: There isn't any attenuation, like with sunlight.
			// (This is part of the reason directional lights are fast to calculate)

			// TODO: Still need to do specular, but this gives you an idea
			finalOutputColour.rgb = finalObjectColour.rgb;
			finalOutputColour.a = wholeObjectAlphaTransparency;

			finalOutputNormal.rgb = vertNormal.xyz;
			finalOutputNormal.r = 1.0f;
			finalOutputNormal.a = 1.0f;

			return;		
		}
		
		// Contribution for this light
		vec3 vLightToVertex = theLights[index].position.xyz - vertPosWorld.xyz;
		float distanceToLight = length(vLightToVertex);	
		vec3 lightVector = normalize(vLightToVertex);
		float dotProduct = dot(lightVector, norm);

		dotProduct = max( 0.0f, dotProduct );	
		
		vec3 lightDiffuseContrib = dotProduct * theLights[index].diffuse.rgb;

		//specular
		vec3 lightSpecularContrib = vec3(0.0f);

		vec3 reflectVector = reflect( -lightVector, normalize(norm) );

		// Get eye or view vector
		// The location of the vertex in the world to your eye
		vec3 eyeVector = normalize(eyeLocation.xyz - vertPosWorld.xyz);
		
		// To simplify, we are NOT using the light specular value, just the object’s.
		float objectSpecularPower = objectSpecular.w; 

		lightSpecularContrib = pow( max(0.0f, dot( eyeVector, reflectVector) ), objectSpecularPower )
			                   * objectSpecular.rgb;	//* theLights[lightIndex].Specular.rgb

		float attenuation = 1.0f / 
				( theLights[index].atten.x + 										
				  theLights[index].atten.y * distanceToLight +						
				  theLights[index].atten.z * distanceToLight*distanceToLight );  	
				  
		lightDiffuseContrib *= attenuation;
		lightSpecularContrib *= attenuation;

		// But is it a spot light
		if ( intLightType == SPOT_LIGHT_TYPE )		// = 1
		{	
			// Yes, it's a spotlight
			// Calcualate light vector (light to vertex, in world)
			vec3 vertexToLight = vertPosWorld.xyz - theLights[index].position.xyz;

			vertexToLight = normalize(vertexToLight);

			float currentLightRayAngle
					= dot( vertexToLight.xyz, theLights[index].direction.xyz );
					
			currentLightRayAngle = max(0.0f, currentLightRayAngle);

			//vec4 param1;	
			// x = lightType, y = inner angle, z = outer angle, w = TBD

			// Is this inside the cone? 
			float outerConeAngleCos = cos(radians(theLights[index].param1.z));
			float innerConeAngleCos = cos(radians(theLights[index].param1.y));
							
			// Is it completely outside of the spot?
			if ( currentLightRayAngle < outerConeAngleCos )
			{
				// Nope. so it's in the dark
				lightDiffuseContrib = vec3(0.0f, 0.0f, 0.0f);
				lightSpecularContrib = vec3(0.0f, 0.0f, 0.0f);
			}
			else if ( currentLightRayAngle < innerConeAngleCos )
			{
				// Angle is between the inner and outer cone
				// (this is called the penumbra of the spot light, by the way)
				// 
				// This blends the brightness from full brightness, near the inner cone
				//	to black, near the outter cone
				float penumbraRatio = (currentLightRayAngle - outerConeAngleCos) / 
									  (innerConeAngleCos - outerConeAngleCos);
									  
				lightDiffuseContrib *= penumbraRatio;
				lightSpecularContrib *= penumbraRatio;
			}
						
		}// if ( intLightType == 1 )
			
		finalObjectColour.rgb += (materialDiffuse.rgb * lightDiffuseContrib.rgb) 
		                         + (materialSpecular.rgb * lightSpecularContrib.rgb);
		
//		if(dotProduct > 0.95){
//			finalObjectColour.rgb = vec3(1.0f, 0.5f, 0.5f);
//		}
//		else if(dotProduct > 0.5){
//			finalObjectColour.rgb = vec3(0.6f, 0.3f, 0.3f);
//		}
//		else if(dotProduct > 0.25){
//			finalObjectColour.rgb = vec3(0.4f, 0.2f, 0.2f);
//		}
//		else {
//			finalObjectColour.rgb = vec3(0.2f, 0.1f, 0.1f);
//		}
		// ********************************************************
	}//for(intindex=0...

//	if(vertPosWorld.y > 0.2f){
//		finalOutputColour.rgb = finalObjectColour.rgb;
//	}else{
//		finalObjectColour.r = 0.0f;
//		finalOutputColour.g = 0.0f;
//		finalOutputColour.b = 1.0f;
//	}
//	finalOutputColour.a = 1.0f;
// Particle imposter (smoke, fire, water, etc.)

	finalOutputColour.rgb = finalObjectColour.rgb;
	finalOutputColour.a = wholeObjectAlphaTransparency;

	// Also output the normal to colour #1
	finalOutputNormal.rgb = vertNormal.xyz;
//	finalOutputNormal.r = 1.0f;
	finalOutputNormal.a = 1.0f;


	if ( bIsParticleImposter == true ) 
	{
		// Discard anything that's "black enough"
		vec4 tex0Col = texture( texture00, vertUV_x2.st ).rgba;	// Imposter texture (smoke, fire)
		
		// Out shader was only testing for black and white, 
		// but now we have coloured textures, so use all the colours to 
		// test for "black".
		// Alternatively, you can use a 2nd texture as a "mask" (black and white) texture
		
		// Note "greyscale", that humans see, isn't even over the colours. 
		// (google "RGB to greyscale" to get this equation:)
		float pixelBlackAndWhite = (0.3f * tex0Col.r) + (0.59f * tex0Col.g) + (0.11f * tex0Col.b);
//		
//		// We had a threshold of 0.25, but how it's passed in
		if ( pixelBlackAndWhite < ParticleImposterBlackThreshold )
		{
			// Don't draw it
			// Literally Doesn't Draw the pixel
			discard;
		}	
//		// OK, I'm drawing the particle imposter...
//		// The darker it is, the more transparent it should be
//		// Colour is from 0 to 1  (white = 1)
//		// Alpha goes from 0 to 1, too (oh my goodness)
		finalOutputColour.a = pixelBlackAndWhite;
		finalOutputColour.rgb += tex0Col.rgb;
		
//		finalOutputColour.rgb = vec3(1.0f, 0.0f, 0.0f);
		// Override the "fade at death" transparency, too
//		finalOutputColour.a *= alphaOverride;
//		finalOutputColour.a = 1.0f;

		finalOutputColour.a *= alphaOverride;
//		finalOutputColour.r = 1.0f - ParticleImposterAlphaOverride;
//		finalOutputNormal.rgb = vertNormal.xyz;
//		finalOutputNormal.r = 1.0f;
//		finalOutputNormal.a = 1.0f;
//		return;
	}//if(bIsParticleImposter)
	
	if(toon){
		vec3 viewVector = eyeLocation.xyz - vertPosWorld.xyz;
		viewVector = normalize(viewVector);

		vec3 pointNormal = normalize(vertNormal.xyz);
		float angleDot = dot(viewVector, pointNormal);

	//	if(angle >= -0.174533f && angle <= 0.174533f){
		if(angleDot < 0.3f){
			finalOutputColour.rgb = vec3(0.0f, 0.0f, 0.0f);
		}
	}
}
