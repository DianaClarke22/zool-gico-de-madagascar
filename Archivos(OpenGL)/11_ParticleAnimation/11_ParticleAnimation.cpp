/*
* Laboratorio de Computación Gráfica e Interacción Humano-Computadora
* 11 - Animación de Partículas
*/

#include <iostream>
#include <stdlib.h>

// GLAD: Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator
// https://glad.dav1d.de/
#include <glad/glad.h>

// GLFW: https://www.glfw.org/
#include <GLFW/glfw3.h>

// GLM: OpenGL Math library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Model loading classes
#include <shader_m.h>
#include <camera.h>
#include <model.h>
#include <material.h>
#include <light.h>
#include <cubemap.h>
#include <particles.h>

#include <irrKlang.h>
using namespace irrklang;

// Max number of bones
#define MAX_RIGGING_BONES 100

// Functions
bool Start();
bool Update();

// Definición de callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// Gobals
GLFWwindow* window;

// Tamaño en pixeles de la ventana
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

// Definición de cámara (posición en XYZ)
Camera camera(glm::vec3(0.0f, 41.1818f, 10.0f));//permite tener perspectiva libre 1era per
//Camera camera3rd(glm::vec3(0.0f, 0.0f, 0.0f));//3era persona
Camera camera3rd(glm::vec3(0.0f, 0.0f, 0.0f));//3era persona

// Controladores para el movimiento del mouse
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
//float gira puertas
float Pizq = 0.0f, Pder = 0.0f;
// Variables para la velocidad de reproducción
// de la animación
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float elapsedTime = 0.0f;

float     rotateCharacter = 0.0f;
glm::vec3 position(5.0f, 58.0f, -544.0f);//posicion de frutas en cabeza
glm::vec3 forwardView(0.0f, 0.0f, 1.0f);

glm::vec3 BoundingboxMin(0.0f, 0.0f, 1.0f);//editar con parametros limmites del campo para el mono
glm::vec3 BoundingboxMax(0.0f, 0.0f, 1.0f);
// Shaders
Shader *staticShader;
Shader *cubemapShader;
Shader *particlesShader;
Shader* ourShader;
Shader* mLightsShader;
Shader* proceduralShader;
Shader* wavesShader;

// Partículas
Particles particlesSystem(400); // creamos 200 partículas

// Carga la información del modelo
Model   *particleModel,*AguayLunaFarol,*Carrito,*EdificacionMarmol,*HojasyPasto,*Ladrillos,*Madera_EscalerasArbolSillasPajaPastoSecoBambu,
*PuertaDer,*PuertaIzq,*RejasyFarolVerde,*TierraCanguro,*Volcan,*CosasLemur,*Tachos,*Piso,*agua,*fruta;
Model* character;//*notas;
Model* rico, * skipper, * cabo, * marlene, * kanguro, * leona, * mort, * rey,*sol,*kowaski,* KanguroAnimado,
*LeonaAnimado,*MarleneAnimado,*SkipperAnimado,*CaboAnimado,*MortAnimado,*ReyAnimado;
Model* p1, * p2, * p3, * p4, * p5;
Model* bicicleta, * estatuaLeon, * fuente, * osoEstatua, * sombrillas,*periodicos,*carro;

float tradius = 10.0f;
float theta = 0.0f;
float alpha = 0.0f;

// Cubemap
CubeMap *mainCubeMap;
// Materiales
Material materialRejasyFarolVerde, materialHojasPasto, materialLadrillos, materialAguaLunaFarol, materialEdiMarmol
, materialMaderaEscaArbolSillPajaPastoS, materialVolcan, materialCarritoPuerta, materialTierraCanguro, materialTachos
, materialPisoGeneral,materialCosasLemur,materialsol;

// Light gLight; // used to be this in previous article
std::vector<Light> gLights;
glm::vec3 lpos = glm::vec3(0.0f);//variable q controlamos con el teclado
// Pose inicial del modelo
glm::mat4 gBones[MAX_RIGGING_BONES];
glm::mat4 gBonesBar[MAX_RIGGING_BONES];
glm::mat4 gBonesKanguro[MAX_RIGGING_BONES];
glm::mat4 gBonesLeona[MAX_RIGGING_BONES];
glm::mat4 gBonesMarlene[MAX_RIGGING_BONES];
glm::mat4 gBonesSkipper[MAX_RIGGING_BONES];
glm::mat4 gBonesCabo[MAX_RIGGING_BONES];
glm::mat4 gBonesMort[MAX_RIGGING_BONES];
glm::mat4 gBonesRey[MAX_RIGGING_BONES];

float	fps = 30.0f, fpsRico = 30.0f, fpsKanguro = 30.0f, fpsLeona = 30.0f,fpsMarlene=30.0f, 
fpsCabo = 30.0f,fpsSkp=30.0f,fpsMort=30.0f,fpsRey=30.0f;
int		keys = 0, keyRico = 0, keyKanguro = 0, keyLeona = 0,keyMarlene=0,keyCabo=0,keySkp=0,keyMort=0,keyRey=0;
int		animationCount = 0, animationCountRico = 0, animationCountKanguro = 0, animationCountLeona = 0,
animationCountMarlene=0, animationCountSkp = 0, animationCountCabo = 0, animationCountMort= 0, animationCountRey = 0;

float proceduralTime = 0.0f,proceduralKowaski=0.0f;
float wavesTime = 0.0f;
//acciones
float desaparece = 1.0f,avanzax=0.0f, avanzay = 0.0f, avanzaz = 0.0f,crece=0.0f,rotar=0.0f,orbitar=0.0f
, avanzapinguino = 0.0f, volteaPinguino = 0.0, avanzanutria = 0.0f;
bool active = true, flagPinguino=true,flagnutria=true;
// Audio
ISoundEngine *SoundEngine = createIrrKlangDevice();

// selección de cámara
bool    activeCamera = 1; // activamos la primera cámara

// Entrada a función principal
int main()
{
	if (!Start())
		return -1;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		if (!Update())
			break;
	}

	glfwTerminate();
	return 0;

}

bool Start() {
	// Inicialización de GLFW

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creación de la ventana con GLFW
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "FBX Animation with OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Ocultar el cursor mientras se rota la escena
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: Cargar todos los apuntadores
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	// Activación de buffer de profundidad
	glEnable(GL_DEPTH_TEST);

	// Compilación y enlace de shaders
	ourShader = new Shader("shaders/10_vertex_skinning-IT.vs", "shaders/10_fragment_skinning-IT.fs");
	cubemapShader = new Shader("shaders/10_vertex_cubemap.vs", "shaders/10_fragment_cubemap.fs");
	particlesShader = new Shader("shaders/13_particles.vs", "shaders/13_particles.fs");
	mLightsShader = new Shader("shaders/11_PhongShaderMultLights.vs", "shaders/11_PhongShaderMultLights.fs");
	proceduralShader = new Shader("shaders/12_ProceduralAnimation.vs", "shaders/12_ProceduralAnimation.fs");
	wavesShader = new Shader("shaders/13_wavesAnimation.vs", "shaders/13_wavesAnimation.fs");
	ourShader->setBonesIDs(MAX_RIGGING_BONES);
	//Modelos
	particleModel = new Model("models/snow/snow.fbx");//lluvia
	AguayLunaFarol = new Model("models/zoo/AguayLunaFarol.fbx");
	Carrito= new Model("models/zoo/Carrito.fbx");
	EdificacionMarmol = new Model("models/zoo/EdificacionMarmol.fbx");
	HojasyPasto = new Model("models/zoo/HojasyPasto.fbx");
	CosasLemur = new Model("models/zoo/CosasLemurfbx.fbx");
	Ladrillos = new Model("models/zoo/Ladrillos.fbx");
	//Madera_EscalerasArbolSillasPajaPastoSecoBambu = new Model("models/zoo/Madera_EscalerasArbolSillasPajaPastoSecoBambu.fbx");
	Madera_EscalerasArbolSillasPajaPastoSecoBambu = new Model("models/zoo/madera1.fbx");
	PuertaDer = new Model("models/zoo/PuertaDer1.fbx");
	PuertaIzq = new Model("models/zoo/PuertaIzq1.fbx");
	RejasyFarolVerde = new Model("models/zoo/RejasyFarolVerde.fbx");
	TierraCanguro = new Model("models/zoo/TierraCanguro.fbx");
	Volcan = new Model("models/zoo/Volcan.fbx");
	Tachos = new Model("models/zoo/Tachos.fbx");
	Piso = new Model("models/zoo/Piso.fbx");
	agua = new Model("models/zoo/Agua.fbx");
	fruta = new Model("models/zoo/frutas.fbx");
	//Modelos Nuevos
	bicicleta = new Model("models/zoo/nuevosModelos/bicicletas.fbx");
	estatuaLeon = new Model("models/zoo/nuevosModelos/estatuaLeon.fbx");
	fuente = new Model("models/zoo/nuevosModelos/Fuente.fbx");
	osoEstatua = new Model("models/zoo/nuevosModelos/osoEstatua.fbx");
	sombrillas = new Model("models/zoo/nuevosModelos/Sombrillas.fbx");
	carro= new Model("models/zoo/nuevosModelos/carro.fbx");
	//periodicos= new Model("models/zoo/nuevosModelos/periodicos.fbx");
	//notas= new Model("models/zoo/notas.fbx");

	//Personas
	//character = new Model("models/zoo/character1.fbx");
	character = new Model("models/zoo/character1.fbx");
	p1= new Model("models/zoo/personajes/p1.fbx");
	p2= new Model("models/zoo/personajes/p2.fbx");
	p3= new Model("models/zoo/personajes/p3.fbx");
	p4= new Model("models/zoo/personajes/p4.fbx");
	p5= new Model("models/zoo/personajes/p5.fbx");

	sol = new Model("models/IllumModels/moon.fbx");
	//Modelos-Animales
	//skipper = new Model("models/zoo/Animales/Skipper.obj");
	//skipper = new Model("models/zoo/Animales/skipper.fbx");
	//cabo = new Model("models/zoo/Animales/Cabo.obj");
	//rico = new Model("models/zoo/Animales/Rico.obj");
	
	rey = new Model("models/zoo/Animales/Rey.obj");
	//mort = new Model("models/zoo/Animales/Mort.obj");
	kowaski = new Model("models/zoo/Animales/kowaski.fbx");
	//marlene = new Model("models/zoo/Animales/Marlene.obj");
	//kanguro = new Model("models/zoo/Animales/kanguro1.obj");
	//leona = new Model("models/zoo/Animales/Leona.obj");

	//Animales keyframe
	KanguroAnimado = new Model("models/zoo/Animales/KanguroA1.fbx");
	LeonaAnimado = new Model("models/zoo/Animales/leonaA.fbx");
	MarleneAnimado = new Model("models/zoo/Animales/nutriaA.fbx");
	//MarleneAnimado = new Model("models/zoo/Animales/MarleneA1.fbx");
	SkipperAnimado = new Model("models/zoo/Animales/SkipperA.fbx");
	rico = new Model("models/zoo/Animales/ricoA.fbx");
	CaboAnimado = new Model("models/zoo/Animales/CaboA.fbx");
	MortAnimado = new Model("models/zoo/Animales/MortA.fbx");
	ReyAnimado = new Model("models/zoo/Animales/reyA.fbx");
		//
	// Cubemap
	vector<std::string> faces
	{
		"textures/cubemap/01/posx.png",
		"textures/cubemap/01/negx.png",
		"textures/cubemap/01/posy.png",
		"textures/cubemap/01/negy.png",
		"textures/cubemap/01/posz.png",
		"textures/cubemap/01/negz.png"
	};
	mainCubeMap = new CubeMap();
	mainCubeMap->loadCubemap(faces);
	//time, arrays -- Componentes de modelos moviles
	character->SetPose(0.0f, gBones);
	fps = (float)character->getFramerate();
	keys = (int)character->getNumFrames();
	
	rico->SetPose(0.0f, gBonesBar);
	fpsRico = (float)rico->getFramerate();
	keyRico = (int)rico->getNumFrames();

	KanguroAnimado->SetPose(0.0f, gBonesKanguro);
	fpsKanguro = (float)KanguroAnimado->getFramerate();
	keyKanguro = (int)KanguroAnimado->getNumFrames();
	
	LeonaAnimado->SetPose(0.0f, gBonesLeona);
	fpsLeona= (float)LeonaAnimado->getFramerate();
	keyLeona = (int)LeonaAnimado->getNumFrames();

	MarleneAnimado->SetPose(0.0f, gBonesMarlene);
	fpsMarlene = (float)MarleneAnimado->getFramerate();
	keyMarlene = (int)MarleneAnimado->getNumFrames();

	SkipperAnimado->SetPose(0.0f, gBonesSkipper);
	fpsSkp = (float)SkipperAnimado->getFramerate();
	keySkp= (int)SkipperAnimado->getNumFrames();

	CaboAnimado->SetPose(0.0f, gBonesCabo);
	fpsCabo= (float)CaboAnimado->getFramerate();
	keyCabo = (int)CaboAnimado->getNumFrames();

	MortAnimado->SetPose(0.0f, gBonesMort);
	fpsMort= (float)MortAnimado->getFramerate();
	keyMort= (int)MortAnimado->getNumFrames();

	ReyAnimado->SetPose(0.0f, gBonesRey);
	fpsRey = (float)ReyAnimado->getFramerate();
	keyRey = (int)ReyAnimado->getNumFrames();
	
	camera3rd.Position = glm::vec3(position.x, position.y-18.0f, position.z+30.0f);//pos de camara 3era persona con efecto 1era persona
	//camera3rd.Position.y += 1.7f;
	camera3rd.Position -= forwardView;
	camera3rd.Front = forwardView;

	// Lights configuration

	/*Light light01;
	light01.Position = glm::vec3(5.0f, 2.0f, 5.0f);
	light01.Color = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	gLights.push_back(light01);

	Light light02;
	light02.Position = glm::vec3(-5.0f, 2.0f, 5.0f);
	light02.Color = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	gLights.push_back(light02);

	Light light03;
	light03.Position = glm::vec3(5.0f, 2.0f, -5.0f);
	light03.Color = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	gLights.push_back(light03);

	Light light04;
	light04.Position = glm::vec3(-5.0f, 2.0f, -5.0f);
	light04.Color = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	gLights.push_back(light04);*/

	Light light01, light02, light03, light04, light05, light06, light07, light08, light09, light10, light11, light12
		,light13;
	light01.Position = glm::vec3(20.6936f, 68.4982f, -289.845f);
	light01.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	/*light02.Position = glm::vec3(199.34f, 139.283f, 66.4318f);
	light02.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	light03.Position = glm::vec3(-144.156f, -27.3953f, 66.4318f);
	light03.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	light04.Position = glm::vec3(-246.661f, -140.184f, 58.6923f);
	light04.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	light05.Position = glm::vec3(-66.9243f, -182.765f, 57.8161f);
	light05.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	light06.Position = glm::vec3(82.8553f, -186.351f, 57.8161f);
	light06.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	light07.Position = glm::vec3(263.481f, -173.782f, 57.8161f);
	light07.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	light08.Position = glm::vec3(263.481f, -496.018f, 57.8161f);
	light08.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	light09.Position = glm::vec3(81.4648f, -442.051f, 57.8161f);
	light09.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	light10.Position = glm::vec3(-73.8452f, -435.699f, 57.8161f);
	light10.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	light11.Position = glm::vec3(-253.559f, -505.615f, 57.8161f);
	light11.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	light12.Position = glm::vec3(-184.164f, -569.088f, 57.8161f);
	light12.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	light13.Position = glm::vec3(209.103f, -576.234f, 57.8161f);
	light13.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);*/
	
	gLights.push_back(light01);
	/*gLights.push_back(light02);
	gLights.push_back(light03);
	gLights.push_back(light04);
	gLights.push_back(light05);
	gLights.push_back(light06);
	gLights.push_back(light07);
	gLights.push_back(light08);
	gLights.push_back(light09);
	gLights.push_back(light10);
	gLights.push_back(light11);
	gLights.push_back(light12);
	gLights.push_back(light13);*/

	
	//MATERIALES
	materialAguaLunaFarol.transparency = 0.9f;

	materialsol.ambient = glm::vec4(0.24725f, 0.1995f, 0.0745f, 1.0f);// green rubber
	materialsol.diffuse = glm::vec4(0.75164f, 0.60648f, 0.22648f, 1.0f);
	materialsol.specular = glm::vec4(0.628281f, 0.555802f, 0.366065f, 1.0f);
	materialsol.transparency = 1.0f;

	materialRejasyFarolVerde.ambient = glm::vec4(0.0f, 0.05f, 0.0f, 1.0f);// green rubber
	materialRejasyFarolVerde.diffuse = glm::vec4(0.4f, 0.5f, 0.4f, 1.0f);
	materialRejasyFarolVerde.specular = glm::vec4(0.04f, 0.7f, 0.04f, 1.0f);
	materialRejasyFarolVerde.transparency = 1.0f;
	
	materialHojasPasto.ambient = glm::vec4(0.0f,	0.05f,	0.0f, 1.0f);//green rubber
	materialHojasPasto.diffuse = glm::vec4(0.4f,	0.5f,	0.4f, 1.0f);
	materialHojasPasto.specular = glm::vec4(0.04f,	0.7f,	0.04f, 1.0f);
	materialHojasPasto.transparency = 1.0f;

	materialLadrillos.ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);//red opaco
	materialLadrillos.diffuse = glm::vec4(0.5f,	0.0f,	0.0f, 1.0f);
	materialLadrillos.specular = glm::vec4(0.7f,	0.6f,	0.6f, 1.0f);
	materialLadrillos.transparency = 1.0f;

	materialEdiMarmol.ambient = glm::vec4(0.25f,	0.2072f,	0.20725f, 1.0f);//mármol=pearl
	materialEdiMarmol.diffuse = glm::vec4(1.0f,	0.829f,	0.829f, 1.0f);
	materialEdiMarmol.specular = glm::vec4(0.296f,	0.296f,	0.296f, 1.0f);
	materialEdiMarmol.transparency = 1.0f;

	materialMaderaEscaArbolSillPajaPastoS.ambient = glm::vec4(0.25f, 0.148f, 0.06475f,1.0f);//bronze
	materialMaderaEscaArbolSillPajaPastoS.diffuse = glm::vec4(0.4f, 0.2368f, 0.1036f,1.0f);
	materialMaderaEscaArbolSillPajaPastoS.specular = glm::vec4(0.774597f, 0.458561f, 0.200621f, 1.0f);
	materialMaderaEscaArbolSillPajaPastoS.transparency = 1.0f;

	materialVolcan.ambient = glm::vec4(0.05375f,	0.05f,	0.066f, 1.0f);//obsidean
	materialVolcan.diffuse = glm::vec4(0.18275f,	0.17f,	0.225f, 1.0f);
	materialVolcan.specular = glm::vec4(0.3327f,	0.3286f,	0.3464f, 1.0f);
	materialVolcan.transparency = 1.0f;

	materialCarritoPuerta.ambient = glm::vec4(0.192f,	0.1922f,	0.1922f, 1.0f);//silver
	materialCarritoPuerta.diffuse = glm::vec4(0.507f,	0.5075f,	0.5075f, 1.0f);
	materialCarritoPuerta.specular = glm::vec4(0.5082f,	0.5082f,	0.5082f, 1.0f);
	materialCarritoPuerta.transparency = 1.0f;

	materialTierraCanguro.ambient = glm::vec4(0.25f, 0.148f, 0.0647f, 1.0f);//Polished bronze
	materialTierraCanguro.diffuse = glm::vec4(0.4f, 0.2368f, 0.1036f, 1.0f);
	materialTierraCanguro.specular = glm::vec4(0.7745f, 0.4585f, 0.2006f, 1.0f);
	materialTierraCanguro.transparency = 1.0f;

	materialTachos.ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);//black plastic
	materialTachos.diffuse = glm::vec4(0.01f, 0.01f, 0.01f, 1.0f);
	materialTachos.specular = glm::vec4(0.50f, 0.50f, 0.50f, 1.0f);
	materialTachos.transparency = 1.0f;

	materialPisoGeneral.ambient = glm::vec4(0.05f, 0.05f, 0.05f, 1.0f);//white rubber
	materialPisoGeneral.diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	materialPisoGeneral.specular = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
	materialPisoGeneral.transparency = 1.0f;
	materialCosasLemur.transparency = 1.0f;
	// SoundEngine->play2D("sound/EternalGarden.mp3", true);
	
	return true;
}
void SetLightUniformInt(Shader* shader, const char* propertyName, size_t lightIndex, int value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setInt(uniformName.c_str(), value);
}
void SetLightUniformFloat(Shader* shader, const char* propertyName, size_t lightIndex, float value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setFloat(uniformName.c_str(), value);
}
void SetLightUniformVec4(Shader* shader, const char* propertyName, size_t lightIndex, glm::vec4 value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setVec4(uniformName.c_str(), value);
}
void SetLightUniformVec3(Shader* shader, const char* propertyName, size_t lightIndex, glm::vec3 value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setVec3(uniformName.c_str(), value);
}
bool Update() {
	// Cálculo del framerate
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	elapsedTime += deltaTime;
	if (elapsedTime > 1.0f / fps) {
		animationCount++;
		if (animationCount > keys - 1)
			animationCount = 0;
		animationCountRico++;
		if (animationCountRico > keyRico - 1)
			animationCountRico = 0;
		animationCountKanguro++;
		if (animationCountKanguro > keyKanguro - 1)
			animationCountKanguro = 0;
		animationCountLeona++;
		if (animationCountLeona > keyLeona - 1)
			animationCountLeona = 0;
		animationCountMarlene++;
		if (animationCountMarlene > keyMarlene - 1)
			animationCountMarlene = 0;
		animationCountSkp++;
		if (animationCountSkp> keySkp- 1)
			animationCountSkp= 0;
		animationCountCabo++;
		if (animationCountCabo> keyCabo- 1)
			animationCountCabo= 0;
		animationCountMort++;
		if (animationCountMort> keyMort- 1)
			animationCountMort= 0;
		animationCountRey++;
		if (animationCountRey> keyRey- 1)
			animationCountRey = 0;
		ReyAnimado->SetPose((float)animationCountRey, gBonesRey);
		MortAnimado->SetPose((float)animationCountMort, gBonesMort);
		CaboAnimado->SetPose((float)animationCountCabo, gBonesCabo);
		SkipperAnimado->SetPose((float)animationCountSkp, gBonesSkipper);
		MarleneAnimado->SetPose((float)animationCountMarlene, gBonesMarlene);
		LeonaAnimado->SetPose((float)animationCountLeona, gBonesLeona);
		KanguroAnimado->SetPose((float)animationCountKanguro, gBonesKanguro);
		rico->SetPose((float)animationCountRico, gBonesBar);
		character->SetPose((float)animationCount, gBones);
		elapsedTime = 0.0f;
		particlesSystem.UpdatePhysics(deltaTime);
	}

	// Procesa la entrada del teclado o mouse
	processInput(window);
	gLights[0].Position.x = avanzax;//Estamos actualizando la posicion de la luz
	gLights[0].Position.y = avanzay;
	gLights[0].Position.z = avanzaz;
	// Renderizado R - G - B - A
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Cubemap (fondo)
	{
		glm::mat4 projection;
		glm::mat4 view;

		if (activeCamera) {
			projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera.GetViewMatrix();
		}
		else {
			projection = glm::perspective(glm::radians(camera3rd.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera3rd.GetViewMatrix();
		}
		
		mainCubeMap->drawCubeMap(*cubemapShader, projection, view);
		//mainCubeMap->drawCubeMap(*cubemapShader);
	}
	glUseProgram(0);
	{// agua con wave
		wavesShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		glm::mat4 projection;
		glm::mat4 view;
		if (activeCamera) {//F2,si te mueves en los 4 lados
			projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera.GetViewMatrix();
		}
		else {//F1, miras un punto fijo donde puedes rotar y mueves en 2 lados up down
			projection = glm::perspective(glm::radians(camera3rd.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera3rd.GetViewMatrix();
		}
		wavesShader->setMat4("projection", projection);
		wavesShader->setMat4("view", view);

		//                         AGUA
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f,-3.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		wavesShader->setMat4("model", model);

		wavesShader->setFloat("time", wavesTime);
		wavesShader->setFloat("radius", 1.0f);
		wavesShader->setFloat("height", 1.0f);

		agua->Draw(*wavesShader);
		wavesTime -= 0.015f;//velocidad
	}
	glUseProgram(0);
	{
		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 model = glm::mat4(1.0f);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		//carga los modelos con la camara
		if (activeCamera) {//F2,si te mueves en los 4 lados
			projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera.GetViewMatrix();
		}
		else {//F1, miras un punto fijo donde puedes rotar y mueves en 2 lados up down
			projection = glm::perspective(glm::radians(camera3rd.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera3rd.GetViewMatrix();
		}
		ourShader->use();
		/*projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		view = camera.GetViewMatrix();*/
		ourShader->setMat4("projection", projection);
		ourShader->setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position.x, -1.0f, position.z)); // posicion personaje
		model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.16f, 0.16f, 0.16f));	// it's a bit too big for our scene, so scale it down
		ourShader->setFloat("transparency", materialEdiMarmol.transparency);
		ourShader->setMat4("model", model);
		ourShader->setMat4("gBones", MAX_RIGGING_BONES, gBones);
		character->Draw(*ourShader);
		//ANIMACION ANIMALES
//rico
		model = glm::mat4(1.0f);
		if (flagPinguino)
			if (avanzapinguino < 85)
				avanzapinguino += 1.0f;
			else { flagPinguino = false; //volteaPinguino = 90.0f;
					}
		else
			if(avanzapinguino>=0)
				avanzapinguino -= 1.0f;
			else { flagPinguino = true; //volteaPinguino = 0.0f; 
					}

		model = glm::translate(model, glm::vec3(6.31704f, 31.327f, 329.141f-avanzapinguino));
		model = glm::rotate(model, glm::radians(volteaPinguino), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));	// it's a bit too big for our scene, so scale it down
		ourShader->setFloat("transparency", materialEdiMarmol.transparency);
		ourShader->setMat4("model", model);
		ourShader->setMat4("gBones", MAX_RIGGING_BONES, gBonesBar);
		rico->Draw(*ourShader);
		//kanguro
		model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(2.60325f, 40.8535f, -341.38f)); // translate it down so it's at the center of the scene
		model = glm::translate(model, glm::vec3(-368.998f, 18.4383f, 266.077f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));	// it's a bit too big for our scene, so scale it down
		ourShader->setFloat("transparency", materialEdiMarmol.transparency);
		ourShader->setMat4("model", model);
		ourShader->setMat4("gBones", MAX_RIGGING_BONES, gBonesKanguro);
		KanguroAnimado->Draw(*ourShader);
		//leon 
		model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(2.60325f, 40.8535f, -341.38f)); // translate it down so it's at the center of the scene
		model = glm::translate(model, glm::vec3(419.06f, 66.3232f, 314.759f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));	// it's a bit too big for our scene, so scale it down
		ourShader->setFloat("transparency", materialEdiMarmol.transparency);
		ourShader->setMat4("model", model);
		ourShader->setMat4("gBones", MAX_RIGGING_BONES, gBonesLeona);
		LeonaAnimado->Draw(*ourShader);
		//nutria
		model = glm::mat4(1.0f);
		if (flagnutria)
			if (avanzanutria < 32)
				avanzanutria += 0.5f;
			else {
				flagnutria = false; //volteaPinguino = 90.0f;
			}
		else
			if (avanzanutria >= 0)
				avanzanutria -= 0.5f;
			else {
				flagnutria = true; //volteaPinguino = 0.0f; 
			}
	
		model = glm::translate(model, glm::vec3(25.3104f, 11.9323f, 685.578f-avanzanutria));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));	// it's a bit too big for our scene, so scale it down
		ourShader->setFloat("transparency", materialEdiMarmol.transparency);
		ourShader->setMat4("model", model);
		ourShader->setMat4("gBones", MAX_RIGGING_BONES, gBonesMarlene);
		MarleneAnimado->Draw(*ourShader);

		//Skipper
		model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(2.60325f, 40.8535f, -341.38f)); // translate it down so it's at the center of the scene
		model = glm::translate(model, glm::vec3(4.14627f, 36.9869f, 344.478f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));	// it's a bit too big for our scene, so scale it down
		ourShader->setFloat("transparency", materialEdiMarmol.transparency);
		ourShader->setMat4("model", model);
		ourShader->setMat4("gBones", MAX_RIGGING_BONES, gBonesSkipper);
		SkipperAnimado->Draw(*ourShader);

		//Cabo
		model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(2.60325f, 40.8535f, -341.38f)); // translate it down so it's at the center of the scene
		model = glm::translate(model, glm::vec3(5.85065f, 36.2667f, 313.65f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));	// it's a bit too big for our scene, so scale it down
		ourShader->setFloat("transparency", materialEdiMarmol.transparency);
		ourShader->setMat4("model", model);
		ourShader->setMat4("gBones", MAX_RIGGING_BONES, gBonesCabo);
		CaboAnimado->Draw(*ourShader);

		//Mort
		model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(2.60325f, 40.8535f, -341.38f)); // translate it down so it's at the center of the scene
		model = glm::translate(model, glm::vec3(-88.1979f, 10.3127f, -33.4693f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));	// it's a bit too big for our scene, so scale it down
		ourShader->setFloat("transparency", materialEdiMarmol.transparency);
		ourShader->setMat4("model", model);
		ourShader->setMat4("gBones", MAX_RIGGING_BONES, gBonesMort);
		MortAnimado->Draw(*ourShader);

		//Rey
		model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(2.60325f, 40.8535f, -341.38f)); // translate it down so it's at the center of the scene
		model = glm::translate(model, glm::vec3(44.4284f, 49.2624f, -100.229f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));	// it's a bit too big for our scene, so scale it down
		ourShader->setFloat("transparency", materialEdiMarmol.transparency);
		ourShader->setMat4("model", model);
		ourShader->setMat4("gBones", MAX_RIGGING_BONES, gBonesRey);
		ReyAnimado->Draw(*ourShader);
	}
	glUseProgram(0);
	{

		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 model = glm::mat4(1.0f);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		//carga los modelos con la camara
		if (activeCamera) {//F2,si te mueves en los 4 lados
			projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera.GetViewMatrix();
		}
		else {//F1, miras un punto fijo donde puedes rotar y mueves en 2 lados up down
			projection = glm::perspective(glm::radians(camera3rd.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera3rd.GetViewMatrix();
		}
		//Activamos para objetos transparentes
		mLightsShader->use();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);
		// Aplicamos transformaciones del modelo

		/*                         notas               */
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		//model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0f, 135.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(31.16f, 31.16f, 31.16f));	// it's a bit too big for our scene, so scale it down
		//mLightsShader->setMat4("model", model);
		//mLightsShader->setFloat("transparency", materialEdiMarmol.transparency);
		//notas->Draw(*mLightsShader);

		/*                         BICICLETAS  Estatuas fuente osoEstatua sombrillas carro               */
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);
		mLightsShader->setFloat("transparency", materialEdiMarmol.transparency);
		p1->Draw(*mLightsShader); p2->Draw(*mLightsShader); p3->Draw(*mLightsShader); p4->Draw(*mLightsShader); p5->Draw(*mLightsShader);
		bicicleta->Draw(*mLightsShader); estatuaLeon->Draw(*mLightsShader);//periodicos->Draw(*mLightsShader);
		fuente->Draw(*mLightsShader); osoEstatua->Draw(*mLightsShader); sombrillas->Draw(*mLightsShader);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-144.039f, 74.6692f, 2.0456f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.1f + crece/10, 0.1f + crece/10, 0.1f + crece/10));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);
		mLightsShader->setFloat("transparency", materialEdiMarmol.transparency);
		carro->Draw(*mLightsShader);
		/*                              EDIFICACIONMARMOL                                */
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		// Aplicamos propiedades materiales
		mLightsShader->setVec4("MaterialAmbientColor", materialEdiMarmol.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", materialEdiMarmol.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", materialEdiMarmol.specular);
		mLightsShader->setFloat("transparency", materialEdiMarmol.transparency);
		EdificacionMarmol->Draw(*mLightsShader);
		//

		//                  ANIMALES                //
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		//mLightsShader->setMat4("model", model);
		//mLightsShader->setFloat("transparency", materialRejasyFarolVerde.transparency);
		//rico->Draw(*mLightsShader);
		//marlene->Draw(*mLightsShader);
		//skipper->Draw(*mLightsShader);
		//cabo->Draw(*mLightsShader);
		//leona->Draw(*mLightsShader);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f+avanza, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		//mLightsShader->setMat4("model", model);
		//kanguro->Draw(*mLightsShader);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f+0.5, 3.0f, 0.0f)); // translate it down so it's at the center of the scene
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		//mLightsShader->setMat4("model", model);
		//rey->Draw(*mLightsShader);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, -0.7f, 0.0f)); // translate it down so it's at the center of the scene
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		//mLightsShader->setMat4("model", model);
		//mort->Draw(*mLightsShader);
		//					FRUTA
		model = glm::mat4(1.0f);
		if (active) {
			model = glm::translate(model, glm::vec3(30.1761f, 41.1818f, -123.526f)); // translate it down so it's at the center of the scene
			model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 1.0f));
		}
		else {
			model = glm::translate(model, glm::vec3(position.x, 58.0f, position.z));//agarra objeto
			model = glm::rotate(model, glm::radians(orbitar), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		if (!active)  orbitar += 10.0f;
		// translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		mLightsShader->setMat4("model", model);
		fruta->Draw(*mLightsShader);

		/*                              REJASYFAROLVERDE                                  */
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(camera.Position));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		// Aplicamos propiedades materiales
		mLightsShader->setVec4("MaterialAmbientColor", materialRejasyFarolVerde.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", materialRejasyFarolVerde.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", materialRejasyFarolVerde.specular);
		mLightsShader->setFloat("transparency", materialRejasyFarolVerde.transparency);
		RejasyFarolVerde->Draw(*mLightsShader);

		/*                              HOJASYPASTO                                 */
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);
		// Configuramos propiedades de fuentes de luz

		// Aplicamos propiedades materiales
		mLightsShader->setVec4("MaterialAmbientColor", materialHojasPasto.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", materialHojasPasto.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", materialHojasPasto.specular);
		mLightsShader->setFloat("transparency", materialHojasPasto.transparency);
		HojasyPasto->Draw(*mLightsShader);

		/*                              LADRILLOS                                */
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);
		// Configuramos propiedades de fuentes de luz
		mLightsShader->setInt("numLights", (int)gLights.size());
		for (size_t i = 0; i < gLights.size(); ++i) {//atravez del ciclo le pasamos los valores al shader
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			mLightsShader->setVec3("eye", camera.Position);//vector E es el brillo del vertor Reflectado, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}

		// Aplicamos propiedades materiales
		mLightsShader->setVec4("MaterialAmbientColor", materialLadrillos.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", materialLadrillos.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", materialLadrillos.specular);
		mLightsShader->setFloat("transparency", materialLadrillos.transparency);
		Ladrillos->Draw(*mLightsShader);
		//SetLightUniformVec4(mLightsShader, "Color", 0, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));


		/*                              Madera_EscalerasArbolSillasPajaPastoSecoBambu                               */
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		// Aplicamos propiedades materiales
		mLightsShader->setVec4("MaterialAmbientColor", materialMaderaEscaArbolSillPajaPastoS.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", materialMaderaEscaArbolSillPajaPastoS.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", materialMaderaEscaArbolSillPajaPastoS.specular);
		mLightsShader->setFloat("transparency", materialMaderaEscaArbolSillPajaPastoS.transparency);
		Madera_EscalerasArbolSillasPajaPastoSecoBambu->Draw(*mLightsShader);
		/*                              VOLCAN                              */
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		// Aplicamos propiedades materiales
		mLightsShader->setVec4("MaterialAmbientColor", materialVolcan.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", materialVolcan.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", materialVolcan.specular);
		mLightsShader->setFloat("transparency", materialVolcan.transparency);
		Volcan->Draw(*mLightsShader);

		/*                              carrito                             */
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(0.0f + rotar), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f , 1.0f , 1.0f ));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		// Aplicamos propiedades materiales
		mLightsShader->setVec4("MaterialAmbientColor", materialCarritoPuerta.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", materialCarritoPuerta.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", materialCarritoPuerta.specular);
		mLightsShader->setFloat("transparency", materialCarritoPuerta.transparency);
		Carrito->Draw(*mLightsShader);

		/*                              PuertaDer                             */
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-75.0f, 55.0f, -554.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(0.0f + Pder), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f , 1.0f , 1.0f ));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		// Aplicamos propiedades materiales
		mLightsShader->setVec4("MaterialAmbientColor", materialCarritoPuerta.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", materialCarritoPuerta.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", materialCarritoPuerta.specular);
		mLightsShader->setFloat("transparency", materialCarritoPuerta.transparency);
		PuertaDer->Draw(*mLightsShader);

		/*                              PuertaIzq                            */
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(75.0f, 53.0f, -554.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(45.0f - Pizq), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f , 1.0f , 1.0f ));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		// Aplicamos propiedades materiales
		mLightsShader->setVec4("MaterialAmbientColor", materialCarritoPuerta.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", materialCarritoPuerta.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", materialCarritoPuerta.specular);
		mLightsShader->setFloat("transparency", materialCarritoPuerta.transparency);
		PuertaIzq->Draw(*mLightsShader);

		/*                              Tierra                              */
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		// Aplicamos propiedades materiales
		mLightsShader->setVec4("MaterialAmbientColor", materialTierraCanguro.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", materialTierraCanguro.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", materialTierraCanguro.specular);
		mLightsShader->setFloat("transparency", materialTierraCanguro.transparency);
		TierraCanguro->Draw(*mLightsShader);

		/*                              Tachos                              */
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(desaparece, desaparece, desaparece));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		// Aplicamos propiedades materiales
		mLightsShader->setVec4("MaterialAmbientColor", materialTachos.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", materialTachos.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", materialTachos.specular);
		mLightsShader->setFloat("transparency", materialTachos.transparency);
		Tachos->Draw(*mLightsShader);

		/*                              PisoGeneral                             */
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		// Aplicamos propiedades materiales
		mLightsShader->setVec4("MaterialAmbientColor", materialPisoGeneral.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", materialPisoGeneral.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", materialPisoGeneral.specular);
		mLightsShader->setFloat("transparency", materialPisoGeneral.transparency);
		Piso->Draw(*mLightsShader);

		/*                              saltarin, frutas y sillitas(JuguetesLemur)                             */
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);


		//Aplicamos propiedades materiales
		mLightsShader->setVec4("MaterialAmbientColor", materialCosasLemur.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", materialCosasLemur.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", materialCosasLemur.specular);
		mLightsShader->setFloat("transparency", materialCosasLemur.transparency);
		CosasLemur->Draw(*mLightsShader);
	}
	glUseProgram(0);
	{
		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 model = glm::mat4(1.0f);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		//carga los modelos con la camara
		if (activeCamera) {//F2,si te mueves en los 4 lados
			projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera.GetViewMatrix();
		}
		else {//F1, miras un punto fijo donde puedes rotar y mueves en 2 lados up down
			projection = glm::perspective(glm::radians(camera3rd.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera3rd.GetViewMatrix();
		}
		mLightsShader->use();
		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);
			
		/*                              LUNA DE FAROL                                   */
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);
		// Configuramos propiedades de fuentes de 
		mLightsShader->setFloat("transparency", materialAguaLunaFarol.transparency);
		AguayLunaFarol->Draw(*mLightsShader);
	}
	glUseProgram(0);
	{
		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 model = glm::mat4(1.0f);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		//carga los modelos con la camara
		if (activeCamera) {//F2,si te mueves en los 4 lados
			projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera.GetViewMatrix();
		}
		else {//F1, miras un punto fijo donde puedes rotar y mueves en 2 lados up down
			projection = glm::perspective(glm::radians(camera3rd.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera3rd.GetViewMatrix();
		}

		//   PROCEDURAL - Sol y kowaski
		proceduralShader->use();
		proceduralShader->setMat4("projection", projection);
		proceduralShader->setMat4("view", view);

		//               KOWASKI
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 10.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		proceduralShader->setMat4("model", model);

		proceduralShader->setFloat("time", proceduralKowaski);
		proceduralShader->setFloat("radius", 30.0f);//movimiento en 'y' y 'x'
		proceduralShader->setFloat("height", 5.0f);//alto

		kowaski->Draw(*proceduralShader);
		proceduralKowaski += 0.02;//mas valor, gira mas rapido

		//                SOL
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));
		proceduralShader->setMat4("model", model);

		proceduralShader->setFloat("time", proceduralTime);
		proceduralShader->setFloat("radius", 40.0f);//movimiento en 'y' y 'x'
		proceduralShader->setFloat("height", -5.0f);//alto

		sol->Draw(*proceduralShader);
		proceduralTime += 1.11;//mas valor, gira mas rapido
	}
	glUseProgram(0);
	{
		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 model = glm::mat4(1.0f);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		//carga los modelos con la camara
		if (activeCamera) {//F2,si te mueves en los 4 lados
			projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera.GetViewMatrix();
		}
		else {//F1, miras un punto fijo donde puedes rotar y mueves en 2 lados up down
			projection = glm::perspective(glm::radians(camera3rd.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
			view = camera3rd.GetViewMatrix();
		}
		//lluvia
		for (int psc = 0; psc < particlesSystem.particles.size(); psc++) {
			Particle p_i = particlesSystem.particles.at(psc);

			// Activación del shader de las partículas
			particlesShader->use();
			particlesShader->setMat4("projection", projection);
			particlesShader->setMat4("view", view);

			// Aplicamos transformaciones del modelo
			glm::mat4 model = glm::mat4(1.0f);
			if(activeCamera==1)
			model = glm::translate(model, camera.Position);
			else model = glm::translate(model, camera3rd.Position);
			model = glm::translate(model, p_i.position); // translate it down so it's at the center of the scene
			model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down

			particlesShader->setMat4("model", model);

			// Dibujamos el modelo
			particleModel->Draw(*particlesShader);
		}
		
	}
	glUseProgram(0);

	// glfw: swap buffers 
	glfwSwapBuffers(window);
	glfwPollEvents();

	return true;
}

// Procesamos entradas del teclado
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime*15.0f);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime * 15.0f);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime *15.0f);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime *15.0f);
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	//entrada cierra o abre
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		if(Pizq+Pder<=180)
		{
			Pizq += +5.0f; Pder += +5.0f;
		}
		
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		if (Pizq + Pder >= 0)
		{
			Pizq -= 5.0f; Pder -= 5.0f;
		}
	//tacho desaparece
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		{
			desaparece = 1.0f; 
		}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		{
			desaparece =0.0f;
		}
	//canguro avanza
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		{
			avanzax += 1.0f; 
		}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		{
			avanzax -= 1.0f; 
		}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		avanzay += 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		avanzay -= 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		avanzaz += 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
	{
		avanzaz -= 1.0f;
	}
	//puertas crecen
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	{
		crece += 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	{
		if(crece>=1.0f)crece -= 1.0f;
	}
	//carrito rota
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	{
		rotar += 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
	{
		rotar -= 1.0f;
	}
	//frutas en tu cabeza orbitando
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
	{
		if (active)active = false;
		else { active = true; }
	}
	// camera movement
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		//character = new Model("models/zoo/character1.fbx");
		position = position + 0.1F * forwardView * 17.0f;
		camera3rd.Front = forwardView;
		camera3rd.ProcessKeyboard(FORWARD, deltaTime);
		//camera3rd.Position = position;
		camera3rd.Position = glm::vec3(position.x, position.y - 18.0f, position.z + 30.0f);
		camera3rd.Position.y += 1.7f;
		camera3rd.Position -= forwardView;

	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		//character = new Model("models/zoo/character1.fbx");
		position = position - 0.1F * forwardView * 17.0f;
		camera3rd.Front = forwardView;
		camera3rd.ProcessKeyboard(BACKWARD, deltaTime );
		//camera3rd.Position = position;
		camera3rd.Position = glm::vec3(position.x, position.y - 18.0f, position.z + 30.0f);
		camera3rd.Position.y += 1.7f;
		camera3rd.Position -= forwardView;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		//character = new Model("models/zoo/character1.fbx");
		rotateCharacter += 0.5f;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec4 viewVector = model * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		forwardView = glm::vec3(viewVector);
		forwardView = glm::normalize(forwardView);

		camera3rd.Front = forwardView;
		//camera3rd.Position = position;
		camera3rd.Position = glm::vec3(position.x, position.y - 18.0f, position.z + 30.0f);
		camera3rd.Position.y += 1.7f;
		camera3rd.Position -= forwardView;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		//character = new Model("models/zoo/character1.fbx");
		rotateCharacter -= 0.5f;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec4 viewVector = model * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		forwardView = glm::vec3(viewVector);
		forwardView = glm::normalize(forwardView);

		camera3rd.Front = forwardView;
		//camera3rd.Position = position;
		camera3rd.Position = glm::vec3(position.x, position.y - 18.0f, position.z + 30.0f);
		camera3rd.Position.y += 1.7f;
		camera3rd.Position -= forwardView;
	}
	
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
	{
		activeCamera = 0; //camera3rd.Position = position;
	}
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
		activeCamera = 1;
	
}

// glfw: Actualizamos el puerto de vista si hay cambios del tamaño
// de la ventana
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// glfw: Callback del movimiento y eventos del mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos; 

	lastX = (float)xpos;
	lastY = (float)ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: Complemento para el movimiento y eventos del mouse
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}
