#include "Demo.h"

Demo::Demo() {

}

Demo::~Demo() {
}

void Demo::Init() {
	BuildShaders();
	BuildDepthMap();
	BuildBottomLayer();
	BuildLayer();
	BuildLayerBorder();
	BuildTiang();
	BuildGerobak();
	BuildRoda();
	BuildContainer();
	BuildTexturedPlane();

	InitCamera();
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &layerVAO);
	glDeleteBuffers(1, &layerVBO);
	glDeleteBuffers(1, &layerEBO);
	glDeleteVertexArrays(1, &tiangVAO);
	glDeleteBuffers(1, &tiangVBO);
	glDeleteBuffers(1, &tiangEBO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &planeVBO);
	glDeleteBuffers(1, &planeEBO);
	glDeleteBuffers(1, &depthMapFBO);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	// update camera movement 
	// -------------
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		MoveCamera(CAMERA_SPEED);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		MoveCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		StrafeCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		StrafeCamera(CAMERA_SPEED);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		UpDownCamera(CAMERA_SPEED);
	}

	// update camera rotation
	// ----------------------
	double mouseX, mouseY;
	double midX = screenWidth / 2;
	double midY = screenHeight / 2;
	float angleY = 0.0f;
	float angleZ = 0.0f;

	// Get mouse position
	glfwGetCursorPos(window, &mouseX, &mouseY);
	if ((mouseX == midX) && (mouseY == midY)) {
		return;
	}

	// Set mouse position
	glfwSetCursorPos(window, midX, midY);

	// Get the direction from the mouse cursor, set a resonable maneuvering speed
	angleY = (float)((midX - mouseX)) / 1000;
	angleZ = (float)((midY - mouseY)) / 1000;

	// The higher the value is the faster the camera looks around.
	viewCamY += angleZ * 2;

	// limit the rotation around the x-axis
	if ((viewCamY - posCamY) > 8) {
		viewCamY = posCamY + 8;
	}
	if ((viewCamY - posCamY) < -8) {
		viewCamY = posCamY - 8;
	}
	RotateCamera(-angleY);
}

void Demo::Update(double deltaTime) {
}

void Demo::Render() {

	glEnable(GL_DEPTH_TEST);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// Step 1 Render depth of scene to texture
	// ----------------------------------------
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.0f, far_plane = 7.5f;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	// render scene from light's point of view
	UseShader(this->depthmapShader);
	glUniformMatrix4fv(glGetUniformLocation(this->depthmapShader, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	glViewport(0, 0, this->SHADOW_WIDTH, this->SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawBottomLayer(this->depthmapShader);
	DrawLayerA(this->depthmapShader);
	DrawLayerB(this->depthmapShader);
	DrawLayerC(this->depthmapShader);
	DrawLayerBorderA(this->depthmapShader);
	DrawLayerBorderB(this->depthmapShader);
	DrawLayerBorderC(this->depthmapShader);
	DrawTiangA(this->depthmapShader);
	DrawTiangB(this->depthmapShader);
	DrawTiangC(this->depthmapShader);
	DrawTiangD(this->depthmapShader);
	DrawTiangE(this->depthmapShader);
	DrawTiangF(this->depthmapShader);
	DrawTiangG(this->depthmapShader);
	DrawTiangH(this->depthmapShader);
	DrawTiangI(this->depthmapShader);
	DrawTiangJ(this->depthmapShader);
	DrawTiangK(this->depthmapShader);
	DrawTiangL(this->depthmapShader);
	DrawTiangM(this->depthmapShader);
	DrawTiangN(this->depthmapShader);
	DrawTiangO(this->depthmapShader);
	DrawTiangP(this->depthmapShader);
	DrawTiangQ(this->depthmapShader);
	DrawTiangR(this->depthmapShader);
	DrawTiangS(this->depthmapShader);
	DrawTiangT(this->depthmapShader);
	DrawTiangU(this->depthmapShader);
	DrawTiangV(this->depthmapShader);
	DrawTiangW(this->depthmapShader);
	DrawTiangX(this->depthmapShader);
	DrawGerobak(this->depthmapShader);
	DrawRodaA(this->depthmapShader);
	DrawRodaB(this->depthmapShader);
	DrawRodaC(this->depthmapShader);
	DrawContainerA(this->depthmapShader);
	DrawContainerB(this->depthmapShader);
	DrawTexturedPlane(this->depthmapShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Step 2 Render scene normally using generated depth map
	// ------------------------------------------------------
	glViewport(0, 0, this->screenWidth, this->screenHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Pass perspective projection matrix
	UseShader(this->shadowmapShader);
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(this->shadowmapShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	// LookAt camera (position, target/direction, up)
	glm::vec3 cameraPos = glm::vec3(posCamX, posCamY, posCamZ);
	glm::vec3 cameraFront = glm::vec3(viewCamX, viewCamY, viewCamZ);
	glm::mat4 view = glm::lookAt(cameraPos, cameraFront, glm::vec3(upCamX, upCamY, upCamZ));
	glUniformMatrix4fv(glGetUniformLocation(this->shadowmapShader, "view"), 1, GL_FALSE, glm::value_ptr(view));

	// Setting Light Attributes
	glUniformMatrix4fv(glGetUniformLocation(this->shadowmapShader, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "viewPos"), cameraPos.x, cameraPos.y, cameraPos.z);
	glUniform3f(glGetUniformLocation(this->shadowmapShader, "lightPos"), -2.0f, 4.0f, -1.0f);

	// Configure Shaders
	glUniform1i(glGetUniformLocation(this->shadowmapShader, "diffuseTexture"), 0);
	glUniform1i(glGetUniformLocation(this->shadowmapShader, "shadowMap"), 1);

	// Render floor
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, plane_texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	DrawTexturedPlane(this->shadowmapShader);

	// Render bottom layer
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bottomLayer_texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	DrawBottomLayer(this->shadowmapShader);

	// Render layer
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, layer_texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	DrawLayerA(this->shadowmapShader);
	DrawLayerB(this->shadowmapShader);
	DrawLayerC(this->shadowmapShader);

	// Render border layer
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, borderLayer_texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	DrawLayerBorderA(this->shadowmapShader);
	DrawLayerBorderB(this->shadowmapShader);
	DrawLayerBorderC(this->shadowmapShader);

	// Render tiang
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tiang_texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	DrawTiangA(this->shadowmapShader);
	DrawTiangB(this->shadowmapShader);
	DrawTiangC(this->shadowmapShader);
	DrawTiangD(this->shadowmapShader);
	DrawTiangE(this->shadowmapShader);
	DrawTiangF(this->shadowmapShader);
	DrawTiangG(this->shadowmapShader);
	DrawTiangH(this->shadowmapShader);
	DrawTiangI(this->shadowmapShader);
	DrawTiangJ(this->shadowmapShader);
	DrawTiangK(this->shadowmapShader);
	DrawTiangL(this->shadowmapShader);
	DrawTiangM(this->shadowmapShader);
	DrawTiangN(this->shadowmapShader);
	DrawTiangO(this->shadowmapShader);
	DrawTiangP(this->shadowmapShader);
	DrawTiangQ(this->shadowmapShader);
	DrawTiangR(this->shadowmapShader);
	DrawTiangS(this->shadowmapShader);
	DrawTiangT(this->shadowmapShader);
	DrawTiangU(this->shadowmapShader);
	DrawTiangV(this->shadowmapShader);
	DrawTiangW(this->shadowmapShader);
	DrawTiangX(this->shadowmapShader);

	// Render gerobak
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gerobak_texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	DrawGerobak(this->shadowmapShader);

	// Render roda
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, roda_texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	DrawRodaA(this->shadowmapShader);
	DrawRodaB(this->shadowmapShader);
	DrawRodaC(this->shadowmapShader);

	// Render container
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, container_texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	DrawContainerA(this->shadowmapShader);
	DrawContainerB(this->shadowmapShader);

	glDisable(GL_DEPTH_TEST);
}

void Demo::BuildBottomLayer()
{
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &bottomLayer_texture);
	glBindTexture(GL_TEXTURE_2D, bottomLayer_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("construction.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords, normal
		// upper
		 2.5, 0.2,  3.1, 0, 0,   0.0f,  1.0f,  0.0f, // 0
		-2.5, 0.2,  3.1, 1, 0,   0.0f,  1.0f,  0.0f, // 1
		-2.5, 0.2, -3.1, 1, 1,   0.0f,  1.0f,  0.0f, // 2
		 2.5, 0.2, -3.1, 0, 1,   0.0f,  1.0f,  0.0f, // 3

		// right
		2.5,  0.2,  3.1, 0, 0, 1.0f,  0.0f,  0.0f, // 4
		2.5,  0.2, -3.1, 1, 0, 1.0f,  0.0f,  0.0f, // 5
		2.5, -0.2, -3.1, 1, 1, 1.0f,  0.0f,  0.0f, // 6
		2.5, -0.2,  3.1, 0, 1, 1.0f,  0.0f,  0.0f, // 7

		// back
		-2.5, -0.2, -3.1, 0, 0, 0.0f,  0.0f,  -1.0f, // 8 
		 2.5, -0.2, -3.1, 1, 0, 0.0f,  0.0f,  -1.0f, // 9
		 2.5,  0.2, -3.1, 1, 1, 0.0f,  0.0f,  -1.0f, // 10
		-2.5,  0.2, -3.1, 0, 1, 0.0f,  0.0f,  -1.0f, // 11

		// left
		-2.5, -0.2, -3.1, 0, 0, -1.0f,  0.0f,  0.0f, // 12
		-2.5, -0.2,  3.1, 1, 0, -1.0f,  0.0f,  0.0f, // 13
		-2.5,  0.2,  3.1, 1, 1, -1.0f,  0.0f,  0.0f, // 14
		-2.5,  0.2, -3.1, 0, 1, -1.0f,  0.0f,  0.0f, // 15

		// front
		-2.5, -0.2, 3.1, 0, 0, 0.0f,  0.0f,  1.0f, // 16
		 2.5, -0.2, 3.1, 1, 0, 0.0f,  0.0f,  1.0f, // 17
		 2.5,  0.2, 3.1, 1, 1, 0.0f,  0.0f,  1.0f, // 18
		-2.5,  0.2, 3.1, 0, 1, 0.0f,  0.0f,  1.0f, // 19
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // upper
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 17, 18, 16, 18, 19,  // front
	};

	glGenVertexArrays(1, &bottomLayerVAO);
	glGenBuffers(1, &bottomLayerVBO);
	glGenBuffers(1, &bottomLayerEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(bottomLayerVAO);

	glBindBuffer(GL_ARRAY_BUFFER, bottomLayerVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bottomLayerEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Demo::BuildLayer()
{
	
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &layer_texture);
	glBindTexture(GL_TEXTURE_2D, layer_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("construction.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords, normal
		-2.5, 0.1, 2.9, 0, 0, 0.0f,  1.0f,  0.0f, // 0
		-2.3, 0.1, 2.9, 1, 0, 0.0f,  1.0f,  0.0f, // 1
		-2.5, 0.1, 1.9, 0, 1, 0.0f,  1.0f,  0.0f, // 2
		-2.3, 0.1, 1.9, 1, 1, 0.0f,  1.0f,  0.0f, // 3

		-2.5, 0.1, 1.7, 0, 0, 0.0f,  1.0f,  0.0f, // 4
		-2.3, 0.1, 1.7, 1, 0, 0.0f,  1.0f,  0.0f, // 5
		-2.5, 0.1, 0.7, 0, 1, 0.0f,  1.0f,  0.0f, // 6
		-2.3, 0.1, 0.7, 1, 1, 0.0f,  1.0f,  0.0f, // 7

		-2.5, 0.1,  0.5, 0, 0, 0.0f,  1.0f,  0.0f, // 8
		-2.3, 0.1,  0.5, 1, 0, 0.0f,  1.0f,  0.0f, // 9
		-2.5, 0.1, -0.5, 0, 1, 0.0f,  1.0f,  0.0f, // 10
		-2.3, 0.1, -0.5, 1, 1, 0.0f,  1.0f,  0.0f, // 11

		-2.5, 0.1, -0.7, 0, 0, 0.0f,  1.0f,  0.0f, // 12
		-2.3, 0.1, -0.7, 1, 0, 0.0f,  1.0f,  0.0f, // 13
		-2.5, 0.1, -1.7, 0, 1, 0.0f,  1.0f,  0.0f, // 14
		-2.3, 0.1, -1.7, 1, 1, 0.0f,  1.0f,  0.0f, // 15

		-2.5, 0.1, -1.9, 0, 0, 0.0f,  1.0f,  0.0f, // 16
		-2.3, 0.1, -1.9, 1, 0, 0.0f,  1.0f,  0.0f, // 17
		-2.5, 0.1, -2.9, 0, 1, 0.0f,  1.0f,  0.0f, // 18
		-2.3, 0.1, -2.9, 1, 1, 0.0f,  1.0f,  0.0f, // 19

		-2.3, 0.1,  3.1, 0, 0, 0.0f,  1.0f,  0.0f, // 20
		-1.3, 0.1,  3.1, 1, 0, 0.0f,  1.0f,  0.0f, // 21
		-2.3, 0.1, -3.1, 0, 1, 0.0f,  1.0f,  0.0f, // 22
		-1.3, 0.1, -3.1, 1, 1, 0.0f,  1.0f,  0.0f, // 23

		-1.3, 0.1, 2.9, 0, 0, 0.0f,  1.0f,  0.0f, // 24
		-1.1, 0.1, 2.9, 1, 0, 0.0f,  1.0f,  0.0f, // 25
		-1.3, 0.1, 1.9, 0, 1, 0.0f,  1.0f,  0.0f, // 26
		-1.1, 0.1, 1.9, 1, 1, 0.0f,  1.0f,  0.0f, // 27

		-1.3, 0.1, 1.7, 0, 0, 0.0f,  1.0f,  0.0f, // 28
		-1.1, 0.1, 1.7, 1, 0, 0.0f,  1.0f,  0.0f, // 29
		-1.3, 0.1, 0.7, 0, 1, 0.0f,  1.0f,  0.0f, // 30
		-1.1, 0.1, 0.7, 1, 1, 0.0f,  1.0f,  0.0f, // 31

		-1.3, 0.1,  0.5, 0, 0, 0.0f,  1.0f,  0.0f, // 32
		-1.1, 0.1,  0.5, 1, 0, 0.0f,  1.0f,  0.0f, // 33
		-1.3, 0.1, -1.7, 0, 1, 0.0f,  1.0f,  0.0f, // 34
		-1.1, 0.1, -1.7, 1, 1, 0.0f,  1.0f,  0.0f, // 35

		-1.3, 0.1, -1.9, 0, 0, 0.0f,  1.0f,  0.0f, // 36
		-1.1, 0.1, -1.9, 1, 0, 0.0f,  1.0f,  0.0f, // 37
		-1.3, 0.1, -2.9, 0, 1, 0.0f,  1.0f,  0.0f, // 38
		-1.1, 0.1, -2.9, 1, 1, 0.0f,  1.0f,  0.0f, // 39

		-1.1, 0.1, -0.5, 0, 0, 0.0f,  1.0f,  0.0f, // 40
		-0.1, 0.1, -0.5, 1, 0, 0.0f,  1.0f,  0.0f, // 41
		-1.1, 0.1, -3.1, 0, 1, 0.0f,  1.0f,  0.0f, // 42
		-0.1, 0.1, -3.1, 1, 1, 0.0f,  1.0f,  0.0f, // 43

		-1.1, 0.1, 1.9, 0, 0, 0.0f,  1.0f,  0.0f, // 44
		-0.1, 0.1, 1.9, 1, 0, 0.0f,  1.0f,  0.0f, // 45
		-1.1, 0.1, 0.7, 0, 1, 0.0f,  1.0f,  0.0f, // 46
		-0.1, 0.1, 0.7, 1, 1, 0.0f,  1.0f,  0.0f, // 47

		-0.1, 0.1,  1.9, 0, 0, 0.0f,  1.0f,  0.0f, // 48
		 0.1, 0.1,  1.9, 1, 0, 0.0f,  1.0f,  0.0f, // 49
		-0.1, 0.1, -2.9, 0, 1, 0.0f,  1.0f,  0.0f, // 50
		 0.1, 0.1, -2.9, 1, 1, 0.0f,  1.0f,  0.0f, // 51

		0.1, 0.1,  1.9, 0, 0, 0.0f,  1.0f,  0.0f, // 52
		1.1, 0.1,  1.9, 1, 0, 0.0f,  1.0f,  0.0f, // 53
		0.1, 0.1, -3.1, 0, 1, 0.0f,  1.0f,  0.0f, // 54
		1.1, 0.1, -3.1, 1, 1, 0.0f,  1.0f,  0.0f, // 55

		1.1, 0.1, 2.9, 0, 0, 0.0f,  1.0f,  0.0f, // 56
		1.3, 0.1, 2.9, 1, 0, 0.0f,  1.0f,  0.0f, // 57
		1.1, 0.1, 1.9, 0, 1, 0.0f,  1.0f,  0.0f, // 58
		1.3, 0.1, 1.9, 1, 1, 0.0f,  1.0f,  0.0f, // 59

		1.1, 0.1, 1.7, 1, 0, 0.0f,  1.0f,  0.0f, // 60
		1.3, 0.1, 1.7, 0, 0, 0.0f,  1.0f,  0.0f, // 61
		1.1, 0.1, 0.7, 1, 1, 0.0f,  1.0f,  0.0f, // 62
		1.3, 0.1, 0.7, 0, 1, 0.0f,  1.0f,  0.0f, // 63

		1.1, 0.1,  0.5, 1, 0, 0.0f,  1.0f,  0.0f, // 64
		1.3, 0.1,  0.5, 0, 0, 0.0f,  1.0f,  0.0f, // 65
		1.1, 0.1, -0.5, 1, 1, 0.0f,  1.0f,  0.0f, // 66
		1.3, 0.1, -0.5, 0, 1, 0.0f,  1.0f,  0.0f, // 67

		1.1, 0.1, -0.5, 1, 0, 0.0f,  1.0f,  0.0f, // 68
		1.3, 0.1, -0.5, 0, 0, 0.0f,  1.0f,  0.0f, // 69
		1.1, 0.1, -1.7, 1, 1, 0.0f,  1.0f,  0.0f, // 70
		1.3, 0.1, -1.7, 0, 1, 0.0f,  1.0f,  0.0f, // 71

		1.1, 0.1, -1.9, 1, 0, 0.0f,  1.0f,  0.0f, // 72
		1.3, 0.1, -1.9, 0, 0, 0.0f,  1.0f,  0.0f, // 73
		1.1, 0.1, -2.9, 1, 1, 0.0f,  1.0f,  0.0f, // 74
		1.3, 0.1, -2.9, 0, 1, 0.0f,  1.0f,  0.0f, // 75

		2.3, 0.1,  3.1, 0, 0, 0.0f,  1.0f,  0.0f, // 76
		1.3, 0.1,  3.1, 1, 0, 0.0f,  1.0f,  0.0f, // 77
		2.3, 0.1, -3.1, 0, 1, 0.0f,  1.0f,  0.0f, // 78
		1.3, 0.1, -3.1, 1, 1, 0.0f,  1.0f,  0.0f, // 79

		2.3, 0.1, 2.9, 1, 0, 0.0f, 1.0f, 0.0f, // 80
		2.5, 0.1, 2.9, 0, 0, 0.0f, 1.0f, 0.0f, // 81
		2.3, 0.1, 1.9, 1, 1, 0.0f, 1.0f, 0.0f, // 82
		2.5, 0.1, 1.9, 0, 1, 0.0f, 1.0f, 0.0f, // 83

		2.3, 0.1, 1.7, 1, 0, 0.0f, 1.0f, 0.0f, // 84
		2.5, 0.1, 1.7, 0, 0, 0.0f, 1.0f, 0.0f, // 85
		2.3, 0.1, 0.7, 1, 1, 0.0f, 1.0f, 0.0f, // 86
		2.5, 0.1, 0.7, 0, 1, 0.0f, 1.0f, 0.0f, // 87

		2.3, 0.1,  0.5, 1, 0, 0.0f, 1.0f, 0.0f, // 88
		2.5, 0.1,  0.5, 0, 0, 0.0f, 1.0f, 0.0f, // 89
		2.3, 0.1, -0.5, 1, 1, 0.0f, 1.0f, 0.0f, // 90
		2.5, 0.1, -0.5, 0, 1, 0.0f, 1.0f, 0.0f, // 91

		2.3, 0.1, -0.7, 1, 0, 0.0f, 1.0f, 0.0f, // 92
		2.5, 0.1, -0.7, 0, 0, 0.0f, 1.0f, 0.0f, // 93
		2.3, 0.1, -1.7, 1, 1, 0.0f, 1.0f, 0.0f, // 94
		2.5, 0.1, -1.7, 0, 1, 0.0f, 1.0f, 0.0f, // 95

		2.3, 0.1, -1.9, 1, 0, 0.0f, 1.0f, 0.0f, // 96
		2.5, 0.1, -1.9, 0, 0, 0.0f, 1.0f, 0.0f, // 97
		2.3, 0.1, -2.9, 1, 1, 0.0f, 1.0f, 0.0f, // 98
		2.5, 0.1, -2.9, 0, 1, 0.0f, 1.0f, 0.0f, // 99

		-2.5, -0.1, 2.9, 0, 0, 0.0f,  -1.0f,  0.0f, // 100
		-2.3, -0.1, 2.9, 1, 0, 0.0f,  -1.0f,  0.0f, // 101
		-2.5, -0.1, 1.9, 0, 1, 0.0f,  -1.0f,  0.0f, // 102
		-2.3, -0.1, 1.9, 1, 1, 0.0f,  -1.0f,  0.0f, // 103

		-2.5, -0.1, 1.7, 0, 0, 0.0f,  -1.0f,  0.0f, // 104
		-2.3, -0.1, 1.7, 1, 0, 0.0f,  -1.0f,  0.0f, // 105
		-2.5, -0.1, 0.7, 0, 1, 0.0f,  -1.0f,  0.0f, // 106
		-2.3, -0.1, 0.7, 1, 1, 0.0f,  -1.0f,  0.0f, // 107

		-2.5, -0.1,  0.5, 0, 0, 0.0f,  -1.0f,  0.0f, // 108
		-2.3, -0.1,  0.5, 1, 0, 0.0f,  -1.0f,  0.0f, // 109
		-2.5, -0.1, -0.5, 0, 1, 0.0f,  -1.0f,  0.0f, // 110
		-2.3, -0.1, -0.5, 1, 1, 0.0f,  -1.0f,  0.0f, // 111

		-2.5, -0.1, -0.7, 0, 0, 0.0f,  -1.0f,  0.0f, // 112
		-2.3, -0.1, -0.7, 1, 0, 0.0f,  -1.0f,  0.0f, // 113
		-2.5, -0.1, -1.7, 0, 1, 0.0f,  -1.0f,  0.0f, // 114
		-2.3, -0.1, -1.7, 1, 1, 0.0f,  -1.0f,  0.0f, // 115

		-2.5, -0.1, -1.9, 0, 0, 0.0f,  -1.0f,  0.0f, // 116
		-2.3, -0.1, -1.9, 1, 0, 0.0f,  -1.0f,  0.0f, // 117
		-2.5, -0.1, -2.9, 0, 1, 0.0f,  -1.0f,  0.0f, // 118
		-2.3, -0.1, -2.9, 1, 1, 0.0f,  -1.0f,  0.0f, // 119

		-2.3, -0.1,  3.1, 0, 0, 0.0f,  -1.0f,  0.0f, // 120
		-1.3, -0.1,  3.1, 1, 0, 0.0f,  -1.0f,  0.0f, // 121
		-2.3, -0.1, -3.1, 0, 1, 0.0f,  -1.0f,  0.0f, // 122
		-1.3, -0.1, -3.1, 1, 1, 0.0f,  -1.0f,  0.0f, // 123

		-1.3, -0.1, 2.9, 0, 0, 0.0f,  -1.0f,  0.0f, // 124
		-1.1, -0.1, 2.9, 1, 0, 0.0f,  -1.0f,  0.0f, // 125
		-1.3, -0.1, 1.9, 0, 1, 0.0f,  -1.0f,  0.0f, // 126
		-1.1, -0.1, 1.9, 1, 1, 0.0f,  -1.0f,  0.0f, // 127

		-1.3, -0.1, 1.7, 0, 0, 0.0f,  -1.0f,  0.0f, // 128
		-1.1, -0.1, 1.7, 1, 0, 0.0f,  -1.0f,  0.0f, // 129
		-1.3, -0.1, 0.7, 0, 1, 0.0f,  -1.0f,  0.0f, // 130
		-1.1, -0.1, 0.7, 1, 1, 0.0f,  -1.0f,  0.0f, // 131

		-1.3, -0.1,  0.5, 0, 0, 0.0f,  -1.0f,  0.0f, // 132
		-1.1, -0.1,  0.5, 1, 0, 0.0f,  -1.0f,  0.0f, // 133
		-1.3, -0.1, -1.7, 0, 1, 0.0f,  -1.0f,  0.0f, // 134
		-1.1, -0.1, -1.7, 1, 1, 0.0f,  -1.0f,  0.0f, // 135

		-1.3, -0.1, -1.9, 0, 0, 0.0f,  -1.0f,  0.0f, // 136
		-1.1, -0.1, -1.9, 1, 0, 0.0f,  -1.0f,  0.0f, // 137
		-1.3, -0.1, -2.9, 0, 1, 0.0f,  -1.0f,  0.0f, // 138
		-1.1, -0.1, -2.9, 1, 1, 0.0f,  -1.0f,  0.0f, // 139

		-1.1, -0.1, -0.5, 0, 0, 0.0f,  -1.0f,  0.0f, // 140
		-0.1, -0.1, -0.5, 1, 0, 0.0f,  -1.0f,  0.0f, // 141
		-1.1, -0.1, -3.1, 0, 1, 0.0f,  -1.0f,  0.0f, // 142
		-0.1, -0.1, -3.1, 1, 1, 0.0f,  -1.0f,  0.0f, // 143

		-1.1, -0.1, 1.9, 0, 0, 0.0f,  -1.0f,  0.0f, // 144
		-0.1, -0.1, 1.9, 1, 0, 0.0f,  -1.0f,  0.0f, // 145
		-1.1, -0.1, 0.7, 0, 1, 0.0f,  -1.0f,  0.0f, // 146
		-0.1, -0.1, 0.7, 1, 1, 0.0f,  -1.0f,  0.0f, // 147

		-0.1, -0.1,  1.9, 0, 0, 0.0f,  -1.0f,  0.0f, // 148
		 0.1, -0.1,  1.9, 1, 0, 0.0f,  -1.0f,  0.0f, // 149
		-0.1, -0.1, -2.9, 0, 1, 0.0f,  -1.0f,  0.0f, // 150
		 0.1, -0.1, -2.9, 1, 1, 0.0f,  -1.0f,  0.0f, // 151

		0.1, -0.1,  1.9, 0, 0, 0.0f,  -1.0f,  0.0f, // 152
		1.1, -0.1,  1.9, 1, 0, 0.0f,  -1.0f,  0.0f, // 153
		0.1, -0.1, -3.1, 0, 1, 0.0f,  -1.0f,  0.0f, // 154
		1.1, -0.1, -3.1, 1, 1, 0.0f,  -1.0f,  0.0f, // 155

		1.1, -0.1, 2.9, 0, 0, 0.0f,  -1.0f,  0.0f, // 156
		1.3, -0.1, 2.9, 1, 0, 0.0f,  -1.0f,  0.0f, // 157
		1.1, -0.1, 1.9, 0, 1, 0.0f,  -1.0f,  0.0f, // 158
		1.3, -0.1, 1.9, 1, 1, 0.0f,  -1.0f,  0.0f, // 159

		1.1, -0.1, 1.7, 1, 0, 0.0f,  -1.0f,  0.0f, // 160
		1.3, -0.1, 1.7, 0, 0, 0.0f,  -1.0f,  0.0f, // 161
		1.1, -0.1, 0.7, 1, 1, 0.0f,  -1.0f,  0.0f, // 162
		1.3, -0.1, 0.7, 0, 1, 0.0f,  -1.0f,  0.0f, // 163

		1.1, -0.1,  0.5, 1, 0, 0.0f,  -1.0f,  0.0f, // 164
		1.3, -0.1,  0.5, 0, 0, 0.0f,  -1.0f,  0.0f, // 165
		1.1, -0.1, -0.5, 1, 1, 0.0f,  -1.0f,  0.0f, // 166
		1.3, -0.1, -0.5, 0, 1, 0.0f,  -1.0f,  0.0f, // 167

		1.1, -0.1, -0.5, 1, 0, 0.0f,  -1.0f,  0.0f, // 168
		1.3, -0.1, -0.5, 0, 0, 0.0f,  -1.0f,  0.0f, // 169
		1.1, -0.1, -1.7, 1, 1, 0.0f,  -1.0f,  0.0f, // 170
		1.3, -0.1, -1.7, 0, 1, 0.0f,  -1.0f,  0.0f, // 171

		1.1, -0.1, -1.9, 1, 0, 0.0f,  -1.0f,  0.0f, // 172
		1.3, -0.1, -1.9, 0, 0, 0.0f,  -1.0f,  0.0f, // 173
		1.1, -0.1, -2.9, 1, 1, 0.0f,  -1.0f,  0.0f, // 174
		1.3, -0.1, -2.9, 0, 1, 0.0f,  -1.0f,  0.0f, // 175

		2.3, -0.1,  3.1, 0, 0, 0.0f,  -1.0f,  0.0f, // 176
		1.3, -0.1,  3.1, 1, 0, 0.0f,  -1.0f,  0.0f, // 177
		2.3, -0.1, -3.1, 0, 1, 0.0f,  -1.0f,  0.0f, // 178
		1.3, -0.1, -3.1, 1, 1, 0.0f,  -1.0f,  0.0f, // 179

		2.3, -0.1, 2.9, 1, 0, 0.0f, -1.0f, 0.0f, // 180
		2.5, -0.1, 2.9, 0, 0, 0.0f, -1.0f, 0.0f, // 181
		2.3, -0.1, 1.9, 1, 1, 0.0f, -1.0f, 0.0f, // 182
		2.5, -0.1, 1.9, 0, 1, 0.0f, -1.0f, 0.0f, // 183

		2.3, -0.1, 1.7, 1, 0, 0.0f, -1.0f, 0.0f, // 184
		2.5, -0.1, 1.7, 0, 0, 0.0f, -1.0f, 0.0f, // 185
		2.3, -0.1, 0.7, 1, 1, 0.0f, -1.0f, 0.0f, // 186
		2.5, -0.1, 0.7, 0, 1, 0.0f, -1.0f, 0.0f, // 187

		2.3, -0.1,  0.5, 1, 0, 0.0f, -1.0f, 0.0f, // 188
		2.5, -0.1,  0.5, 0, 0, 0.0f, -1.0f, 0.0f, // 189
		2.3, -0.1, -0.5, 1, 1, 0.0f, -1.0f, 0.0f, // 190
		2.5, -0.1, -0.5, 0, 1, 0.0f, -1.0f, 0.0f, // 191

		2.3, -0.1, -0.7, 1, 0, 0.0f, -1.0f, 0.0f, // 192
		2.5, -0.1, -0.7, 0, 0, 0.0f, -1.0f, 0.0f, // 193
		2.3, -0.1, -1.7, 1, 1, 0.0f, -1.0f, 0.0f, // 194
		2.5, -0.1, -1.7, 0, 1, 0.0f, -1.0f, 0.0f, // 195

		2.3, -0.1, -1.9, 1, 0, 0.0f, -1.0f, 0.0f, // 196
		2.5, -0.1, -1.9, 0, 0, 0.0f, -1.0f, 0.0f, // 197
		2.3, -0.1, -2.9, 1, 1, 0.0f, -1.0f, 0.0f, // 198
		2.5, -0.1, -2.9, 0, 1, 0.0f, -1.0f, 0.0f, // 199
		
	};

	unsigned int indices[] = {
		 0,  1,  3,  0,  2,  3,
		 4,  5,  7,  4,  6,  7,
		 8,  9, 11,  8, 10, 11,
		12, 13, 15, 12, 14, 15,
		16, 17, 19, 16, 18, 19,
		20, 21, 23, 20, 22, 23,
		24, 25, 27, 24, 26, 27,
		28, 29, 31, 28, 30, 31,
		32, 33, 35, 32, 34, 35,
		36, 37, 39, 36, 38, 39,
		40, 41, 43, 40, 42, 43,
		44, 45, 47, 44, 46, 47,
		48, 49, 51, 48, 50, 51,
		52, 53, 55, 52, 54, 55,
		56, 57, 59, 56, 58, 59,
		60, 61, 63, 60, 62, 63,
		64, 65, 67, 64, 66, 67,
		68, 69, 71, 68, 70, 71,
		72, 73, 75, 72, 74, 75,
		76, 77, 79, 76, 78, 79,
		80, 81, 83, 80, 82, 83,
		84, 85, 87, 84, 86, 87,
		88, 89, 91, 88, 90, 91,
		92, 93, 95, 92, 94, 95,
		96, 97, 99, 96, 98, 99,
		100, 101, 103, 100, 102, 103,
		104, 105, 107, 104, 106, 107,
		108, 109, 111, 108, 110, 111,
		112, 113, 115, 112, 114, 115,
		116, 117, 119, 116, 118, 119,
		120, 121, 123, 120, 122, 123,
		124, 125, 127, 124, 126, 127,
		128, 129, 131, 128, 130, 131,
		132, 133, 135, 132, 134, 135,
		136, 137, 139, 136, 138, 139,
		140, 141, 143, 140, 142, 143,
		144, 145, 147, 144, 146, 147,
		148, 149, 151, 148, 150, 151,
		152, 153, 155, 152, 154, 155,
		156, 157, 159, 156, 158, 159,
		160, 161, 163, 160, 162, 163,
		164, 165, 167, 164, 166, 167,
		168, 169, 171, 168, 170, 171,
		172, 173, 175, 172, 174, 175,
		176, 177, 179, 176, 178, 179,
		180, 181, 183, 180, 182, 183,
		184, 185, 187, 184, 186, 187,
		188, 189, 191, 188, 190, 191,
		192, 193, 195, 192, 194, 195,
		196, 197, 199, 196, 198, 199,
	};

	glGenVertexArrays(1, &layerVAO);
	glGenBuffers(1, &layerVBO);
	glGenBuffers(1, &layerEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(layerVAO);

	glBindBuffer(GL_ARRAY_BUFFER, layerVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, layerEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
}

void Demo::BuildLayerBorder()
{
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &borderLayer_texture);
	glBindTexture(GL_TEXTURE_2D, borderLayer_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("construction.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords, normal
		// left
		-2.5, -0.1, 2.9, 0, 0, -1.0f, 0.0f, 0.0f,  // 0
		-2.5, -0.1, 1.9, 1, 0, -1.0f, 0.0f, 0.0f,  // 1
		-2.5,  0.1, 2.9, 0, 1, -1.0f, 0.0f, 0.0f,  // 2
		-2.5,  0.1, 1.9, 1, 1, -1.0f, 0.0f, 0.0f,  // 3

		-2.5, -0.1, 1.7, 0, 0, -1.0f, 0.0f, 0.0f,  // 4
		-2.5, -0.1, 0.7, 1, 0, -1.0f, 0.0f, 0.0f,  // 5
		-2.5,  0.1, 1.7, 0, 1, -1.0f, 0.0f, 0.0f,  // 6
		-2.5,  0.1, 0.7, 1, 1, -1.0f, 0.0f, 0.0f,  // 7

		-2.5, -0.1,  0.5, 0, 0, -1.0f, 0.0f, 0.0f,  // 8
		-2.5, -0.1, -0.5, 1, 0, -1.0f, 0.0f, 0.0f,  // 9
		-2.5,  0.1,  0.5, 0, 1, -1.0f, 0.0f, 0.0f,  // 10
		-2.5,  0.1, -0.5, 1, 1, -1.0f, 0.0f, 0.0f,  // 11

		-2.5, -0.1, -0.7, 0, 0, -1.0f, 0.0f, 0.0f,  // 12
		-2.5, -0.1, -1.7, 1, 0, -1.0f, 0.0f, 0.0f,  // 13
		-2.5,  0.1, -0.7, 0, 1, -1.0f, 0.0f, 0.0f,  // 14
		-2.5,  0.1, -1.7, 1, 1, -1.0f, 0.0f, 0.0f,  // 15

		-2.5, -0.1, -1.9, 0, 0, -1.0f, 0.0f, 0.0f,  // 16
		-2.5, -0.1, -2.9, 1, 0, -1.0f, 0.0f, 0.0f,  // 17
		-2.5,  0.1, -1.9, 0, 1, -1.0f, 0.0f, 0.0f,  // 18
		-2.5,  0.1, -2.9, 1, 1, -1.0f, 0.0f, 0.0f,  // 19

		// right
		2.5, -0.1, 2.9, 0, 0, 1.0f, 0.0f, 0.0f,  // 20
		2.5, -0.1, 1.9, 1, 0, 1.0f, 0.0f, 0.0f,  // 21
		2.5,  0.1, 2.9, 0, 1, 1.0f, 0.0f, 0.0f,  // 22
		2.5,  0.1, 1.9, 1, 1, 1.0f, 0.0f, 0.0f,  // 23

		2.5, -0.1, 1.7, 0, 0, 1.0f, 0.0f, 0.0f,  // 24
		2.5, -0.1, 0.7, 1, 0, 1.0f, 0.0f, 0.0f,  // 25
		2.5,  0.1, 1.7, 0, 1, 1.0f, 0.0f, 0.0f,  // 26
		2.5,  0.1, 0.7, 1, 1, 1.0f, 0.0f, 0.0f,  // 27

		2.5, -0.1,  0.5, 0, 0, 1.0f, 0.0f, 0.0f,  // 28
		2.5, -0.1, -0.5, 1, 0, 1.0f, 0.0f, 0.0f,  // 29
		2.5,  0.1,  0.5, 0, 1, 1.0f, 0.0f, 0.0f,  // 30
		2.5,  0.1, -0.5, 1, 1, 1.0f, 0.0f, 0.0f,  // 31

		2.5, -0.1, -0.7, 0, 0, 1.0f, 0.0f, 0.0f,  // 32
		2.5, -0.1, -1.7, 1, 0, 1.0f, 0.0f, 0.0f,  // 33
		2.5,  0.1, -0.7, 0, 1, 1.0f, 0.0f, 0.0f,  // 34
		2.5,  0.1, -1.7, 1, 1, 1.0f, 0.0f, 0.0f,  // 35

		2.5, -0.1, -1.9, 0, 0, 1.0f, 0.0f, 0.0f,  // 36
		2.5, -0.1, -2.9, 1, 0, 1.0f, 0.0f, 0.0f,  // 37
		2.5,  0.1, -1.9, 0, 1, 1.0f, 0.0f, 0.0f,  // 38
		2.5,  0.1, -2.9, 1, 1, 1.0f, 0.0f, 0.0f,  // 39

		// back
		-2.3, -0.1, -3.1, 0, 0, 0.0f, 0.0f, -1.0f,  // 40
		-1.3, -0.1, -3.1, 1, 0, 0.0f, 0.0f, -1.0f,  // 41
		-2.3,  0.1, -3.1, 0, 1, 0.0f, 0.0f, -1.0f,  // 42
		-1.3,  0.1, -3.1, 1, 1, 0.0f, 0.0f, -1.0f,  // 43

		-1.1, -0.1, -3.1, 0, 0, 0.0f, 0.0f, -1.0f,  // 44
		-0.1, -0.1, -3.1, 1, 0, 0.0f, 0.0f, -1.0f,  // 45
		-1.1,  0.1, -3.1, 0, 1, 0.0f, 0.0f, -1.0f,  // 46
		-0.1,  0.1, -3.1, 1, 1, 0.0f, 0.0f, -1.0f,  // 47

		0.1, -0.1, -3.1, 0, 0, 0.0f, 0.0f, -1.0f,  // 48
		1.1, -0.1, -3.1, 1, 0, 0.0f, 0.0f, -1.0f,  // 49
		0.1,  0.1, -3.1, 0, 1, 0.0f, 0.0f, -1.0f,  // 50
		1.1,  0.1, -3.1, 1, 1, 0.0f, 0.0f, -1.0f,  // 51

		1.3, -0.1, -3.1, 0, 0, 0.0f, 0.0f, -1.0f,  // 52
		2.3, -0.1, -3.1, 1, 0, 0.0f, 0.0f, -1.0f,  // 53
		1.3,  0.1, -3.1, 0, 1, 0.0f, 0.0f, -1.0f,  // 54
		2.3,  0.1, -3.1, 1, 1, 0.0f, 0.0f, -1.0f,  // 55

		// front
		-2.3, -0.1, 3.1, 0, 0, 0.0f, 0.0f, 1.0f,  // 56
		-1.3, -0.1, 3.1, 1, 0, 0.0f, 0.0f, 1.0f,  // 57
		-2.3,  0.1, 3.1, 0, 1, 0.0f, 0.0f, 1.0f,  // 58
		-1.3,  0.1, 3.1, 1, 1, 0.0f, 0.0f, 1.0f,  // 59

		-1.1, -0.1, 1.9, 0, 0, 0.0f, 0.0f, 1.0f,  // 60
		 1.1, -0.1, 1.9, 1, 0, 0.0f, 0.0f, 1.0f,  // 61
		-1.1,  0.1, 1.9, 0, 1, 0.0f, 0.0f, 1.0f,  // 62
		 1.1,  0.1, 1.9, 1, 1, 0.0f, 0.0f, 1.0f,  // 63

		1.3, -0.1, 3.1, 0, 0, 0.0f, 0.0f, 1.0f,  // 64
		2.3, -0.1, 3.1, 1, 0, 0.0f, 0.0f, 1.0f,  // 65
		1.3,  0.1, 3.1, 0, 1, 0.0f, 0.0f, 1.0f,  // 66
		2.3,  0.1, 3.1, 1, 1, 0.0f, 0.0f, 1.0f,  // 67

		-1.1, -0.1, 2.9, 0, 0, -1.0f, 0.0f, 0.0f,  // 68
		-1.1, -0.1, 1.9, 1, 0, -1.0f, 0.0f, 0.0f,  // 69
		-1.1,  0.1, 2.9, 0, 1, -1.0f, 0.0f, 0.0f,  // 70
		-1.1,  0.1, 1.9, 1, 1, -1.0f, 0.0f, 0.0f,  // 71

		1.1, -0.1, 2.9, 0, 0, 1.0f, 0.0f, 0.0f,  // 72
		1.1, -0.1, 1.9, 1, 0, 1.0f, 0.0f, 0.0f,  // 73
		1.1,  0.1, 2.9, 0, 1, 1.0f, 0.0f, 0.0f,  // 74
		1.1,  0.1, 1.9, 1, 1, 1.0f, 0.0f, 0.0f,  // 75

		// center
		-1.1, -0.1, 0.7, 0, 0, 0.0f, 0.0f, -1.0f,  // 76
		-0.1, -0.1, 0.7, 1, 0, 0.0f, 0.0f, -1.0f,  // 77
		-1.1,  0.1, 0.7, 0, 1, 0.0f, 0.0f, -1.0f,  // 78
		-0.1,  0.1, 0.7, 1, 1, 0.0f, 0.0f, -1.0f,  // 79

		-1.1, -0.1, -0.5, 0, 0, 0.0f, 0.0f, 1.0f,  // 80
		-0.1, -0.1, -0.5, 1, 0, 0.0f, 0.0f, 1.0f,  // 81
		-1.1,  0.1, -0.5, 0, 1, 0.0f, 0.0f, 1.0f,  // 82
		-0.1,  0.1, -0.5, 1, 1, 0.0f, 0.0f, 1.0f,  // 83

		-0.1, -0.1,  0.7, 0, 0, -1.0f, 0.0f, 0.0f,  // 84
		-0.1, -0.1, -0.5, 1, 0, -1.0f, 0.0f, 0.0f,  // 85
		-0.1,  0.1,  0.7, 0, 1, -1.0f, 0.0f, 0.0f,  // 86
		-0.1,  0.1, -0.5, 1, 1, -1.0f, 0.0f, 0.0f,  // 87

		-1.1, -0.1,  0.5, 0, 0, -1.0f, 0.0f, 0.0f,  // 88
		-1.1, -0.1, -0.5, 1, 0, -1.0f, 0.0f, 0.0f,  // 89
		-1.1,  0.1,  0.5, 0, 1, -1.0f, 0.0f, 0.0f,  // 90
		-1.1,  0.1, -0.5, 1, 1, -1.0f, 0.0f, 0.0f,  // 91
	};

	unsigned int indices[] = {
		 0,  1,  3,  0,  2,  3,
		 4,  5,  7,  4,  6,  7,
		 8,  9, 11,  8, 10, 11,
		12, 13, 15, 12, 14, 15,
		16, 17, 19, 16, 18, 19,
		20, 21, 23, 20, 22, 23,
		24, 25, 27, 24, 26, 27,
		28, 29, 31, 28, 30, 31,
		32, 33, 35, 32, 34, 35,
		36, 37, 39, 36, 38, 39,
		40, 41, 43, 40, 42, 43,
		44, 45, 47, 44, 46, 47,
		48, 49, 51, 48, 50, 51,
		52, 53, 55, 52, 54, 55,
		56, 57, 59, 56, 58, 59,
		60, 61, 63, 60, 62, 63,
		64, 65, 67, 64, 66, 67,
		68, 69, 71, 68, 70, 71,
		72, 73, 75, 72, 74, 75,
		76, 77, 79, 76, 78, 79,
		80, 81, 83, 80, 82, 83,
		84, 85, 87, 84, 86, 87,
		88, 89, 91, 88, 90, 91
	};

	glGenVertexArrays(1, &borderLayerVAO);
	glGenBuffers(1, &borderLayerVBO);
	glGenBuffers(1, &borderLayerEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(borderLayerVAO);

	glBindBuffer(GL_ARRAY_BUFFER, borderLayerVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, borderLayerEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Demo::BuildTiang()
{
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &tiang_texture);
	glBindTexture(GL_TEXTURE_2D, tiang_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("construction.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords, normal
		// front
		-0.1, 0.2, 0.1, 0, 0, 0.0f,  0.0f,  1.0f, // 20
		 0.1, 0.2, 0.1, 1, 0, 0.0f,  0.0f,  1.0f, // 21
		 0.1, 4.5, 0.1, 1, 1, 0.0f,  0.0f,  1.0f, // 22
		-0.1, 4.5, 0.1, 0, 1, 0.0f,  0.0f,  1.0f, // 23

		// right
		0.1, 4.5,  0.1, 0, 0, 1.0f,  0.0f,  0.0f, // 24
		0.1, 4.5, -0.1, 1, 0, 1.0f,  0.0f,  0.0f, // 25
		0.1, 0.2, -0.1, 1, 1, 1.0f,  0.0f,  0.0f, // 26
		0.1, 0.2,  0.1, 0, 1, 1.0f,  0.0f,  0.0f, // 27

		// back
		-0.1, 0.2, -0.1, 0, 0, 0.0f,  0.0f,  -1.0f, // 28 
		 0.1, 0.2, -0.1, 1, 0, 0.0f,  0.0f,  -1.0f, // 29
		 0.1, 4.5, -0.1, 1, 1, 0.0f,  0.0f,  -1.0f, // 30
		-0.1, 4.5, -0.1, 0, 1, 0.0f,  0.0f,  -1.0f, // 31

		// left
		-0.1, 0.2, -0.1, 0, 0, -1.0f,  0.0f,  0.0f, // 32
		-0.1, 0.2,  0.1, 1, 0, -1.0f,  0.0f,  0.0f, // 33
		-0.1, 4.5,  0.1, 1, 1, -1.0f,  0.0f,  0.0f, // 34
		-0.1, 4.5, -0.1, 0, 1, -1.0f,  0.0f,  0.0f, // 35

		// upper
		 0.1, 4.5,  0.1, 0, 0,   0.0f,  1.0f,  0.0f, // 36
		-0.1, 4.5,  0.1, 1, 0,   0.0f,  1.0f,  0.0f, // 37
		-0.1, 4.5, -0.1, 1, 1,   0.0f,  1.0f,  0.0f, // 38
		 0.1, 4.5, -0.1, 0, 1,   0.0f,  1.0f,  0.0f, // 39

	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
	};

	glGenVertexArrays(1, &tiangVAO);
	glGenBuffers(1, &tiangVBO);
	glGenBuffers(1, &tiangEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(tiangVAO);

	glBindBuffer(GL_ARRAY_BUFFER, tiangVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tiangEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Demo::BuildGerobak()
{
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &gerobak_texture);
	glBindTexture(GL_TEXTURE_2D, gerobak_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("crate.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords, normal
		-0.5, -0.2, 0.25, 0, 0, 0.0f, 0.0f, 1.0f,  // 0
		 0.5, -0.2, 0.25, 1, 0, 0.0f, 0.0f, 1.0f,  // 1
		-0.5,  0.2, 0.25, 0, 1, 0.0f, 0.0f, 1.0f,  // 2
		 0.5,  0.2, 0.25, 1, 1, 0.0f, 0.0f, 1.0f,  // 3

		-0.5, -0.2, -0.25, 0, 0, 0.0f, 0.0f, -1.0f,  // 4
		 0.5, -0.2, -0.25, 1, 0, 0.0f, 0.0f, -1.0f,  // 5
		-0.5,  0.2, -0.25, 0, 1, 0.0f, 0.0f, -1.0f,  // 6
		 0.5,  0.2, -0.25, 1, 1, 0.0f, 0.0f, -1.0f,  // 7

		-0.5, -0.2,  0.25, 0, 0, -1.0f, 0.0f, 0.0f,  // 8
		-0.5, -0.2, -0.25, 1, 0, -1.0f, 0.0f, 0.0f,  // 9
		-0.5,  0.2,  0.25, 0, 1, -1.0f, 0.0f, 0.0f,  // 10
		-0.5,  0.2, -0.25, 1, 1, -1.0f, 0.0f, 0.0f,  // 11

		0.5, -0.2,  0.25, 0, 0, 1.0f, 0.0f, 0.0f,  // 12
		0.5, -0.2, -0.25, 1, 0, 1.0f, 0.0f, 0.0f,  // 13
		0.5,  0.2,  0.25, 0, 1, 1.0f, 0.0f, 0.0f,  // 14
		0.5,  0.2, -0.25, 1, 1, 1.0f, 0.0f, 0.0f,  // 15

		-0.5, -0.2,  0.25, 0, 0, 0.0f, -1.0f, 0.0f,  // 16
		 0.5, -0.2,  0.25, 1, 0, 0.0f, -1.0f, 0.0f,  // 17
		-0.5, -0.2, -0.25, 0, 1, 0.0f, -1.0f, 0.0f,  // 18
		 0.5, -0.2, -0.25, 1, 1, 0.0f, -1.0f, 0.0f,  // 19

		-0.45, -0.15, 0.2, 0, 0, 0.0f, 0.0f, -1.0f,  // 20
		 0.45, -0.15, 0.2, 1, 0, 0.0f, 0.0f, -1.0f,  // 21
		-0.45,   0.2, 0.2, 0, 1, 0.0f, 0.0f, -1.0f,  // 22
		 0.45,   0.2, 0.2, 1, 1, 0.0f, 0.0f, -1.0f,  // 23

		-0.45, -0.15, -0.2, 0, 0, 0.0f, 0.0f, 1.0f,  // 24
		 0.45, -0.15, -0.2, 1, 0, 0.0f, 0.0f, 1.0f,  // 25
		-0.45,   0.2, -0.2, 0, 1, 0.0f, 0.0f, 1.0f,  // 26
		 0.45,   0.2, -0.2, 1, 1, 0.0f, 0.0f, 1.0f,  // 27

		-0.45, -0.15,  0.2, 0, 0, 1.0f, 0.0f, 0.0f,  // 28
		-0.45, -0.15, -0.2, 1, 0, 1.0f, 0.0f, 0.0f,  // 29
		-0.45,   0.2,  0.2, 0, 1, 1.0f, 0.0f, 0.0f,  // 30
		-0.45,   0.2, -0.2, 1, 1, 1.0f, 0.0f, 0.0f,  // 31

		0.45, -0.15,  0.2, 0, 0, -1.0f, 0.0f, 0.0f,  // 32
		0.45, -0.15, -0.2, 1, 0, -1.0f, 0.0f, 0.0f,  // 33
		0.45,   0.2,  0.2, 0, 1, -1.0f, 0.0f, 0.0f,  // 34
		0.45,   0.2, -0.2, 1, 1, -1.0f, 0.0f, 0.0f,  // 35

		-0.45, -0.15,  0.2, 0, 0, 0.0f, 1.0f, 0.0f,  // 36
		 0.45, -0.15,  0.2, 1, 0, 0.0f, 1.0f, 0.0f,  // 37
		-0.45, -0.15, -0.2, 0, 1, 0.0f, 1.0f, 0.0f,  // 38
		 0.45, -0.15, -0.2, 1, 1, 0.0f, 1.0f, 0.0f,  // 39

		 -0.5, 0.2,  0.25, 0, 0, 0.0f, 1.0f, 0.0f,  // 40
		-0.45, 0.2,  0.25, 1, 0, 0.0f, 1.0f, 0.0f,  // 41
		 -0.5, 0.2, -0.25, 0, 1, 0.0f, 1.0f, 0.0f,  // 42
		-0.45, 0.2, -0.25, 1, 1, 0.0f, 1.0f, 0.0f,  // 43

		0.45, 0.2,  0.25, 0, 0, 0.0f, 1.0f, 0.0f,  // 44
		 0.5, 0.2,  0.25, 1, 0, 0.0f, 1.0f, 0.0f,  // 45
		0.45, 0.2, -0.25, 0, 1, 0.0f, 1.0f, 0.0f,  // 46
		 0.5, 0.2, -0.25, 1, 1, 0.0f, 1.0f, 0.0f,  // 47

		-0.45, 0.2, 0.25, 0, 0, 0.0f, 1.0f, 0.0f,  // 48
		 0.45, 0.2, 0.25, 1, 0, 0.0f, 1.0f, 0.0f,  // 49
		-0.45, 0.2,  0.2, 0, 1, 0.0f, 1.0f, 0.0f,  // 50
		 0.45, 0.2,  0.2, 1, 1, 0.0f, 1.0f, 0.0f,  // 51

		-0.45, 0.2,  -0.2, 0, 0, 0.0f, 1.0f, 0.0f,  // 52
		 0.45, 0.2,  -0.2, 1, 0, 0.0f, 1.0f, 0.0f,  // 53
		-0.45, 0.2, -0.25, 0, 1, 0.0f, 1.0f, 0.0f,  // 54
		 0.45, 0.2, -0.25, 1, 1, 0.0f, 1.0f, 0.0f,  // 55

		0.5, 0.1, 0.25, 0, 0, 0.0f, 0.0f, 1.0f,  // 56
		0.8, 0.1, 0.25, 1, 0, 0.0f, 0.0f, 1.0f,  // 57
		0.5, 0.2, 0.25, 0, 1, 0.0f, 0.0f, 1.0f,  // 58
		0.8, 0.2, 0.25, 1, 1, 0.0f, 0.0f, 1.0f,  // 59

		0.5, 0.1, 0.2, 0, 0, 0.0f, 0.0f, -1.0f,  // 60
		0.8, 0.1, 0.2, 1, 0, 0.0f, 0.0f, -1.0f,  // 61
		0.5, 0.2, 0.2, 0, 1, 0.0f, 0.0f, -1.0f,  // 62
		0.8, 0.2, 0.2, 1, 1, 0.0f, 0.0f, -1.0f,  // 63

		0.5, 0.1, 0.25, 0, 0, 0.0f, -1.0f, 0.0f,  // 64
		0.8, 0.1, 0.25, 1, 0, 0.0f, -1.0f, 0.0f,  // 65
		0.5, 0.1,  0.2, 0, 1, 0.0f, -1.0f, 0.0f,  // 66
		0.8, 0.1,  0.2, 1, 1, 0.0f, -1.0f, 0.0f,  // 67

		0.5, 0.2, 0.25, 0, 0, 0.0f, 1.0f, 0.0f,  // 68
		0.8, 0.2, 0.25, 1, 0, 0.0f, 1.0f, 0.0f,  // 69
		0.5, 0.2,  0.2, 0, 1, 0.0f, 1.0f, 0.0f,  // 70
		0.8, 0.2,  0.2, 1, 1, 0.0f, 1.0f, 0.0f,  // 71

		0.8, 0.1, 0.25, 0, 0, 1.0f, 0.0f, 0.0f,  // 72
		0.8, 0.1,  0.2, 1, 0, 1.0f, 0.0f, 0.0f,  // 73
		0.8, 0.2, 0.25, 0, 1, 1.0f, 0.0f, 0.0f,  // 74
		0.8, 0.2,  0.2, 1, 1, 1.0f, 0.0f, 0.0f,  // 75

		0.5, 0.1, -0.25, 0, 0, 0.0f, 0.0f, -1.0f,  // 76
		0.8, 0.1, -0.25, 1, 0, 0.0f, 0.0f, -1.0f,  // 77
		0.5, 0.2, -0.25, 0, 1, 0.0f, 0.0f, -1.0f,  // 78
		0.8, 0.2, -0.25, 1, 1, 0.0f, 0.0f, -1.0f,  // 79

		0.5, 0.1, -0.2, 0, 0, 0.0f, 0.0f, 1.0f,  // 80
		0.8, 0.1, -0.2, 1, 0, 0.0f, 0.0f, 1.0f,  // 81
		0.5, 0.2, -0.2, 0, 1, 0.0f, 0.0f, 1.0f,  // 82
		0.8, 0.2, -0.2, 1, 1, 0.0f, 0.0f, 1.0f,  // 83

		0.5, 0.1, -0.25, 0, 0, 0.0f, -1.0f, 0.0f,  // 84
		0.8, 0.1, -0.25, 1, 0, 0.0f, -1.0f, 0.0f,  // 85
		0.5, 0.1,  -0.2, 0, 1, 0.0f, -1.0f, 0.0f,  // 86
		0.8, 0.1,  -0.2, 1, 1, 0.0f, -1.0f, 0.0f,  // 87

		0.5, 0.2, -0.25, 0, 0, 0.0f, 1.0f, 0.0f,  // 88
		0.8, 0.2, -0.25, 1, 0, 0.0f, 1.0f, 0.0f,  // 89
		0.5, 0.2,  -0.2, 0, 1, 0.0f, 1.0f, 0.0f,  // 90
		0.8, 0.2,  -0.2, 1, 1, 0.0f, 1.0f, 0.0f,  // 91

		0.8, 0.1, -0.25, 0, 0, 1.0f, 0.0f, 0.0f,  // 92
		0.8, 0.1,  -0.2, 1, 0, 1.0f, 0.0f, 0.0f,  // 93
		0.8, 0.2, -0.25, 0, 1, 1.0f, 0.0f, 0.0f,  // 94
		0.8, 0.2,  -0.2, 1, 1, 1.0f, 0.0f, 0.0f,  // 95

	};

	unsigned int indices[] = {
		 0,  1,  3,  0,  2,  3,
		 4,  5,  7,  4,  6,  7,
		 8,  9, 11,  8, 10, 11,
		12, 13, 15, 12, 14, 15,
		16, 17, 19, 16, 18, 19,
		20, 21, 23, 20, 22, 23,
		24, 25, 27, 24, 26, 27,
		28, 29, 31, 28, 30, 31,
		32, 33, 35, 32, 34, 35,
		36, 37, 39, 36, 38, 39,
		40, 41, 43, 40, 42, 43,
		44, 45, 47, 44, 46, 47,
		48, 49, 51, 48, 50, 51,
		52, 53, 55, 52, 54, 55,
		56, 57, 59, 56, 58, 59,
		60, 61, 63, 60, 62, 63,
		64, 65, 67, 64, 66, 67,
		68, 69, 71, 68, 70, 71,
		72, 73, 75, 72, 74, 75,
		76, 77, 79, 76, 78, 79,
		80, 81, 83, 80, 82, 83,
		84, 85, 87, 84, 86, 87,
		88, 89, 91, 88, 90, 91,
		92, 93, 95, 92, 94, 95,
	};

	glGenVertexArrays(1, &gerobakVAO);
	glGenBuffers(1, &gerobakVBO);
	glGenBuffers(1, &gerobakEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(gerobakVAO);

	glBindBuffer(GL_ARRAY_BUFFER, gerobakVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gerobakEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Demo::BuildRoda()
{
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &roda_texture);
	glBindTexture(GL_TEXTURE_2D, roda_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("roda.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords, normal
		-0.05, -0.05, 0.05, 0, 0, 0.0f, 0.0f, 1.0f,  // 0
		 0.05, -0.05, 0.05, 1, 0, 0.0f, 0.0f, 1.0f,  // 1
		-0.05,  0.05, 0.05, 0, 1, 0.0f, 0.0f, 1.0f,  // 2
		 0.05,  0.05, 0.05, 1, 1, 0.0f, 0.0f, 1.0f,  // 3

		-0.05, -0.05, -0.05, 0, 0, 0.0f, 0.0f, -1.0f,  // 4
		 0.05, -0.05, -0.05, 1, 0, 0.0f, 0.0f, -1.0f,  // 5
		-0.05,  0.05, -0.05, 0, 1, 0.0f, 0.0f, -1.0f,  // 6
		 0.05,  0.05, -0.05, 1, 1, 0.0f, 0.0f, -1.0f,  // 7

		-0.05, -0.05,  0.05, 0, 0, -1.0f, 0.0f, 0.0f,  // 8
		-0.05, -0.05, -0.05, 1, 0, -1.0f, 0.0f, 0.0f,  // 9
		-0.05,  0.05,  0.05, 0, 1, -1.0f, 0.0f, 0.0f,  // 10
		-0.05,  0.05, -0.05, 1, 1, -1.0f, 0.0f, 0.0f,  // 11

		0.05, -0.05,  0.05, 0, 0, 1.0f, 0.0f, 0.0f,  // 12
		0.05, -0.05, -0.05, 1, 0, 1.0f, 0.0f, 0.0f,  // 13
		0.05,  0.05,  0.05, 0, 1, 1.0f, 0.0f, 0.0f,  // 14
		0.05,  0.05, -0.05, 1, 1, 1.0f, 0.0f, 0.0f,  // 15

		-0.05, 0.05,   0.0, 0, 0, 0.0f, 1.0f, 0.0f,  // 16
		 0.05, 0.05,   0.0, 1, 0, 0.0f, 1.0f, 0.0f,  // 17
		-0.05, 0.05, -0.05, 0, 1, 0.0f, 1.0f, 0.0f,  // 18
		 0.05, 0.05, -0.05, 1, 1, 0.0f, 1.0f, 0.0f,  // 19
	};

	unsigned int indices[] = {
		 0,  1,  3,  0,  2,  3,
		 4,  5,  7,  4,  6,  7,
		 8,  9, 11,  8, 10, 11,
		12, 13, 15, 12, 14, 15,
		16, 17, 19, 16, 18, 19,
	};

	glGenVertexArrays(1, &rodaVAO);
	glGenBuffers(1, &rodaVBO);
	glGenBuffers(1, &rodaEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(rodaVAO);

	glBindBuffer(GL_ARRAY_BUFFER, rodaVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rodaEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Demo::BuildContainer()
{
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &container_texture);
	glBindTexture(GL_TEXTURE_2D, container_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("container.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords, normal
		 0.5, 0.5,  1.0, 0, 0, 0.0f, -1.0f, 0.0f,  // 0
		 0.5, 0.5, -1.0, 1, 0, 0.0f, -1.0f, 0.0f,  // 1
		-0.5, 0.5,  1.0, 0, 1, 0.0f, -1.0f, 0.0f,  // 2
		-0.5, 0.5, -1.0, 1, 1, 0.0f, -1.0f, 0.0f,  // 3

		-0.5, -0.5,  1.0, 0, 0, -1.0f, 0.0f, 0.0f,  // 4
		-0.5, -0.5, -1.0, 1, 0, -1.0f, 0.0f, 0.0f,  // 5
		-0.5,  0.5,  1.0, 0, 1, -1.0f, 0.0f, 0.0f,  // 6
		-0.5,  0.5, -1.0, 1, 1, -1.0f, 0.0f, 0.0f,  // 7

		0.5, -0.5,  1.0, 0, 0, 1.0f, 0.0f, 0.0f,  // 8
		0.5, -0.5, -1.0, 1, 0, 1.0f, 0.0f, 0.0f,  // 9
		0.5,  0.5,  1.0, 0, 1, 1.0f, 0.0f, 0.0f,  // 10
		0.5,  0.5, -1.0, 1, 1, 1.0f, 0.0f, 0.0f,  // 11

		-0.5, -0.5, 1.0, 0, 0, 0.0f, 0.0f, 1.0f,  // 12
		 0.5, -0.5, 1.0, 1, 0, 0.0f, 0.0f, 1.0f,  // 13
		-0.5, -0.4, 1.0, 0, 1, 0.0f, 0.0f, 1.0f,  // 14
		 0.5, -0.4, 1.0, 1, 1, 0.0f, 0.0f, 1.0f,  // 15

		-0.5, -0.5, -1.0, 0, 0, 0.0f, 0.0f, -1.0f,  // 16
		 0.5, -0.5, -1.0, 1, 0, 0.0f, 0.0f, -1.0f,  // 17
		-0.5, -0.4, -1.0, 0, 1, 0.0f, 0.0f, -1.0f,  // 18
		 0.5, -0.4, -1.0, 1, 1, 0.0f, 0.0f, -1.0f,  // 19

		-0.5,  0.5, 1.0, 0, 0, 0.0f, 0.0f, 1.0f,  // 20
		 0.5,  0.5, 1.0, 1, 0, 0.0f, 0.0f, 1.0f,  // 21
		-0.5, 0.45, 1.0, 0, 1, 0.0f, 0.0f, 1.0f,  // 22
		 0.5, 0.45, 1.0, 1, 1, 0.0f, 0.0f, 1.0f,  // 23

		-0.5,  0.5, -1.0, 0, 0, 0.0f, 0.0f, -1.0f,  // 24
		 0.5,  0.5, -1.0, 1, 0, 0.0f, 0.0f, -1.0f,  // 25
		-0.5, 0.45, -1.0, 0, 1, 0.0f, 0.0f, -1.0f,  // 26
		 0.5, 0.45, -1.0, 1, 1, 0.0f, 0.0f, -1.0f,  // 27

		 -0.5, -0.4, 1.0, 0, 0, 0.0f, 0.0f, 1.0f,  // 28
		-0.45, -0.4, 1.0, 1, 0, 0.0f, 0.0f, 1.0f,  // 29
		 -0.5, 0.45, 1.0, 0, 1, 0.0f, 0.0f, 1.0f,  // 30
		-0.45, 0.45, 1.0, 1, 1, 0.0f, 0.0f, 1.0f,  // 31

		 -0.5, -0.4, -1.0, 0, 0, 0.0f, 0.0f, -1.0f,  // 32
		-0.45, -0.4, -1.0, 1, 0, 0.0f, 0.0f, -1.0f,  // 33
		 -0.5, 0.45, -1.0, 0, 1, 0.0f, 0.0f, -1.0f,  // 34
		-0.45, 0.45, -1.0, 1, 1, 0.0f, 0.0f, -1.0f,  // 35

		 0.5, -0.4, 1.0, 0, 0, 0.0f, 0.0f, 1.0f,  // 36
		0.45, -0.4, 1.0, 1, 0, 0.0f, 0.0f, 1.0f,  // 37
		 0.5, 0.45, 1.0, 0, 1, 0.0f, 0.0f, 1.0f,  // 38
		0.45, 0.45, 1.0, 1, 1, 0.0f, 0.0f, 1.0f,  // 39

		 0.5, -0.4, -1.0, 0, 0, 0.0f, 0.0f, -1.0f,  // 40
		0.45, -0.4, -1.0, 1, 0, 0.0f, 0.0f, -1.0f,  // 41
		 0.5, 0.45, -1.0, 0, 1, 0.0f, 0.0f, -1.0f,  // 42
		0.45, 0.45, -1.0, 1, 1, 0.0f, 0.0f, -1.0f,  // 43

		-0.45, -0.4,  1.0, 0, 0, 1.0f, 0.0f, 0.0f,  // 44
		-0.45, -0.4, -1.0, 1, 0, 1.0f, 0.0f, 0.0f,  // 45
		-0.45, 0.45,  1.0, 0, 1, 1.0f, 0.0f, 0.0f,  // 46
		-0.45, 0.45, -1.0, 1, 1, 1.0f, 0.0f, 0.0f,  // 47

		0.45, -0.4,  1.0, 0, 0, 1.0f, 0.0f, 0.0f,  // 48
		0.45, -0.4, -1.0, 1, 0, 1.0f, 0.0f, 0.0f,  // 49
		0.45, 0.45,  1.0, 0, 1, 1.0f, 0.0f, 0.0f,  // 50
		0.45, 0.45, -1.0, 1, 1, 1.0f, 0.0f, 0.0f,  // 51

		 0.45, -0.4,  1.0, 0, 0, 0.0f, 1.0f, 0.0f,  // 52
		 0.45, -0.4, -1.0, 1, 0, 0.0f, 1.0f, 0.0f,  // 53
		-0.45, -0.4,  1.0, 0, 1, 0.0f, 1.0f, 0.0f,  // 54
		-0.45, -0.4, -1.0, 1, 1, 0.0f, 1.0f, 0.0f,  // 55

		 0.45, 0.45,  1.0, 0, 0, 0.0f, -1.0f, 0.0f,  // 56
		 0.45, 0.45, -1.0, 1, 0, 0.0f, -1.0f, 0.0f,  // 57
		-0.45, 0.45,  1.0, 0, 1, 0.0f, -1.0f, 0.0f,  // 58
		-0.45, 0.45, -1.0, 1, 1, 0.0f, -1.0f, 0.0f,  // 59
	};

	unsigned int indices[] = {
		 0,  1,  3,  0,  2,  3,
		 4,  5,  7,  4,  6,  7,
		 8,  9, 11,  8, 10, 11,
		12, 13, 15, 12, 14, 15,
		16, 17, 19, 16, 18, 19,
		20, 21, 23, 20, 22, 23,
		24, 25, 27, 24, 26, 27,
		28, 29, 31, 28, 30, 31,
		32, 33, 35, 32, 34, 35,
		36, 37, 39, 36, 38, 39,
		40, 41, 43, 40, 42, 43,
		44, 45, 47, 44, 46, 47,
		48, 49, 51, 48, 50, 51,
		52, 53, 55, 52, 54, 55,
		56, 57, 59, 56, 58, 59,
	};

	glGenVertexArrays(1, &containerVAO);
	glGenBuffers(1, &containerVBO);
	glGenBuffers(1, &containerEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(containerVAO);

	glBindBuffer(GL_ARRAY_BUFFER, containerVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, containerEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// define normal pointer layout 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Demo::BuildTexturedPlane()
{
	// Load and create a texture 
	glGenTextures(1, &plane_texture);
	glBindTexture(GL_TEXTURE_2D, plane_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("plane.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-25.0f,	-0.2f, -25.0f, 0, 0, 0.0f,  1.0f,  0.0f,
		 25.0f,	-0.2f, -25.0f, 1, 0, 0.0f,  1.0f,  0.0f,
		 25.0f,	-0.2f,  25.0f, 1, 1, 0.0f,  1.0f,  0.0f,
		-25.0f,	-0.2f,  25.0f, 0, 1, 0.0f,  1.0f,  0.0f,
	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glGenBuffers(1, &planeEBO);

	glBindVertexArray(planeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Normal attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawBottomLayer(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(bottomLayerVAO);
	glm::mat4 model;
	//model = glm::translate(model, glm::vec3(0, -0.75f, 0));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawLayerA(GLuint shader)
{
	
	UseShader(shader);
	glBindVertexArray(layerVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 1.5, 0));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 300, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	
}

void Demo::DrawLayerB(GLuint shader)
{
	
	UseShader(shader);
	glBindVertexArray(layerVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 2.7, 0));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 300, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	
}

void Demo::DrawLayerC(GLuint shader)
{

	UseShader(shader);
	glBindVertexArray(layerVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 3.9, 0));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 300, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

}

void Demo::DrawLayerBorderA(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(borderLayerVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 1.5f, 0));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 138, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawLayerBorderB(GLuint shader)
{
	
	UseShader(shader);
	glBindVertexArray(borderLayerVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 2.7, 0));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 138, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	
}

void Demo::DrawLayerBorderC(GLuint shader)
{

	UseShader(shader);
	glBindVertexArray(borderLayerVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 3.9, 0));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 138, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

}

void Demo::DrawTiangA(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(tiangVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(2.4, 0, 3.0));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawTiangB(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(tiangVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(1.2, 0, 3.0));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawTiangC(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(tiangVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(-1.2, 0, 3.0));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawTiangD(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(tiangVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(-2.4, 0, 3.0));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawTiangE(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(tiangVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(2.4, 0, 1.8));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawTiangF(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(tiangVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(1.2, 0, 1.8));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawTiangG(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(tiangVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(-1.2, 0, 1.8));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawTiangH(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(tiangVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(-2.4, 0, 1.8));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawTiangI(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(tiangVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(2.4, 0, 0.6));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawTiangJ(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(tiangVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(1.2, 0, 0.6));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawTiangK(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(tiangVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(-1.2, 0, 0.6));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawTiangL(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(tiangVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(-2.4, 0, 0.6));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawTiangM(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(tiangVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(2.4, 0, -0.6));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawTiangN(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(tiangVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(1.2, 0, -0.6));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawTiangO(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(tiangVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(-2.4, 0, -0.6));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawTiangP(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(tiangVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(2.4, 0, -1.8));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawTiangQ(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(tiangVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(1.2, 0, -1.8));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawTiangR(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(tiangVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(-1.2, 0, -1.8));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawTiangS(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(tiangVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(-2.4, 0, -1.8));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawTiangT(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(tiangVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(2.4, 0, -3.0));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawTiangU(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(tiangVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(1.2, 0, -3.0));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawTiangV(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(tiangVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, -3.0));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawTiangW(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(tiangVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(-1.2, 0, -3.0));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawTiangX(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(tiangVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(-2.4, 0, -3.0));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawGerobak(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(gerobakVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0.1, 7));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 144, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawRodaA(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(rodaVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.25, -0.15, 6.75));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawRodaB(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(rodaVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.25, -0.15, 7.25));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawRodaC(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(rodaVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(-0.5, -0.15, 7));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawContainerA(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(containerVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(-4.0, 0.3, 0));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawContainerB(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(containerVAO);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(-5.5, 0.3, 0));

	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawTexturedPlane(GLuint shader)
{
	UseShader(shader);
	glBindVertexArray(planeVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::InitCamera()
{
	posCamX = 0.0f;
	posCamY = 7.0f;
	posCamZ = 10.0f;
	viewCamX = 0.0f;
	viewCamY = 0.0f;
	viewCamZ = -1.0f;
	upCamX = 0.0f;
	upCamY = 1.0f;
	upCamZ = 0.0f;
	CAMERA_SPEED = 0.001f;
	fovy = 45.0f;
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Demo::MoveCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	// forward positive cameraspeed and backward negative -cameraspeed.
	posCamX = posCamX + x * speed;
	posCamZ = posCamZ + z * speed;
	viewCamX = viewCamX + x * speed;
	viewCamZ = viewCamZ + z * speed;
}

void Demo::StrafeCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	float orthoX = -z;
	float orthoZ = x;

	// left positive cameraspeed and right negative -cameraspeed.
	posCamX = posCamX + orthoX * speed;
	posCamZ = posCamZ + orthoZ * speed;
	viewCamX = viewCamX + orthoX * speed;
	viewCamZ = viewCamZ + orthoZ * speed;
}

void Demo::RotateCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	viewCamZ = (float)(posCamZ + glm::sin(speed) * x + glm::cos(speed) * z);
	viewCamX = (float)(posCamX + glm::cos(speed) * x - glm::sin(speed) * z);
}

void Demo::UpDownCamera(float speed)
{
	float y = viewCamY - posCamY;
	// forward positive cameraspeed and backward negative -cameraspeed.
	posCamY = posCamY + y * speed;
	viewCamY = viewCamY + y * speed;
}

void Demo::BuildDepthMap() {
	// configure depth map FBO
	// -----------------------
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, this->SHADOW_WIDTH, this->SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Demo::BuildShaders()
{
	// build and compile our shader program
	// ------------------------------------
	shadowmapShader = BuildShader("shadowMapping.vert", "shadowMapping.frag", nullptr);
	depthmapShader = BuildShader("depthMap.vert", "depthMap.frag", nullptr);
}

int main(int argc, char** argv) {
	RenderEngine& app = Demo();
	app.Start("Final Project", 1920, 1080, false, true);
}