#version 330 core
// source from: http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/billboards/
// with individual changes made

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 squareVertices;
layout(location = 1) in vec2 UV;


// Output data ; will be interpolated for each fragment.
out vec2 UVcoords;

// Values that stay constant for the whole mesh.
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VP; // Model-View-Projection matrix, but without the Model (the position is in BillboardPos; the orientation depends on the camera)
uniform vec3 BillboardPos; // Position of the center of the billboard
uniform vec2 BillboardSize; // Size of the billboard, in world units (probably meters)

void main()
{
	vec3 particleCenter_wordspace = BillboardPos;
	
	vec3 vertexPosition_worldspace = 
		particleCenter_wordspace
		+ CameraRight_worldspace * squareVertices.x * BillboardSize.x
		+ CameraUp_worldspace * squareVertices.y * BillboardSize.y;

	gl_Position = VP * vec4(vertexPosition_worldspace, 1.0f);

	// Or, if BillboardSize is in percentage of the screen size (1,1 for fullscreen) :
	//vertexPosition_worldspace = particleCenter_wordspace;
	//gl_Position = VP * vec4(vertexPosition_worldspace, 1.0f); // Get the screen-space position of the particle's center
	//gl_Position /= gl_Position.w; // Here we have to do the perspective division ourselves.
	//gl_Position.xy += squareVertices.xy * vec2(0.2, 0.05); // Move the vertex in directly screen space. No need for CameraUp/Right_worlspace here.
	
	// Or, if BillboardSize is in pixels : 
	// Same thing, just use (ScreenSizeInPixels / BillboardSizeInPixels) instead of BillboardSizeInScreenPercentage.


	UVcoords = UV;
}
