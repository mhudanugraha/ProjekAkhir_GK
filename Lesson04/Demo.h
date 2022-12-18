#pragma once

#include "RenderEngine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <SOIL/SOIL.h>

class Demo :
	public RenderEngine
{
public:
	Demo();
	~Demo();
private:
	GLuint depthmapShader, shadowmapShader, bottomLayerVBO, bottomLayerVAO, bottomLayerEBO, bottomLayer_texture, borderLayerVBO, borderLayerVAO, borderLayerEBO, borderLayer_texture, layerVBO, layerVAO, layerEBO, layer_texture, tiangVBO, tiangVAO, tiangEBO, tiang_texture, planeVBO, planeVAO, planeEBO, plane_texture, stexture, stexture2, gerobakVBO, gerobakVAO, gerobakEBO, gerobak_texture, rodaVBO, rodaVAO, rodaEBO, roda_texture, containerVBO, containerVAO, containerEBO, container_texture, depthMapFBO, depthMap;
	float viewCamX, viewCamY, viewCamZ, upCamX, upCamY, upCamZ, posCamX, posCamY, posCamZ, CAMERA_SPEED, fovy;
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	virtual void Init();
	virtual void DeInit();
	virtual void Update(double deltaTime);
	virtual void Render();
	virtual void ProcessInput(GLFWwindow* window);
	void BuildBottomLayer();
	void BuildLayer();
	void BuildLayerBorder();
	void BuildTiang();
	void BuildTexturedPlane();
	void BuildGerobak();
	void BuildRoda();
	void BuildContainer();
	void DrawBottomLayer(GLuint shader);
	void DrawLayerA(GLuint shader);
	void DrawLayerB(GLuint shader);
	void DrawLayerC(GLuint shader);
	void DrawLayerBorderA(GLuint shader);
	void DrawLayerBorderB(GLuint shader);
	void DrawLayerBorderC(GLuint shader);
	void DrawTiangA(GLuint shader);
	void DrawTiangB(GLuint shader);
	void DrawTiangC(GLuint shader);
	void DrawTiangD(GLuint shader);
	void DrawTiangE(GLuint shader);
	void DrawTiangF(GLuint shader);
	void DrawTiangG(GLuint shader);
	void DrawTiangH(GLuint shader);
	void DrawTiangI(GLuint shader);
	void DrawTiangJ(GLuint shader);
	void DrawTiangK(GLuint shader);
	void DrawTiangL(GLuint shader);
	void DrawTiangM(GLuint shader);
	void DrawTiangN(GLuint shader);
	void DrawTiangO(GLuint shader);
	void DrawTiangP(GLuint shader);
	void DrawTiangQ(GLuint shader);
	void DrawTiangR(GLuint shader);
	void DrawTiangS(GLuint shader);
	void DrawTiangT(GLuint shader);
	void DrawTiangU(GLuint shader);
	void DrawTiangV(GLuint shader);
	void DrawTiangW(GLuint shader);
	void DrawTiangX(GLuint shader);
	void DrawTexturedPlane(GLuint shader);
	void DrawGerobak(GLuint shader);
	void DrawRodaA(GLuint shader);
	void DrawRodaB(GLuint shader);
	void DrawRodaC(GLuint shader);
	void DrawContainerA(GLuint shader);
	void DrawContainerB(GLuint shader);
	void MoveCamera(float speed);
	void StrafeCamera(float speed);
	void RotateCamera(float speed);
	void UpDownCamera(float speed);
	void InitCamera();
	void BuildDepthMap();
	void BuildShaders();
};

