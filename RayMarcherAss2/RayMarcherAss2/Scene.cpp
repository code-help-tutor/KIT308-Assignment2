WeChat: cstutorcs
QQ: 749389476
Email: tutorcs@163.com
/*  The following code is a VERY heavily modified from code originally sourced from:
	Ray tracing tutorial of http://www.codermind.com/articles/Raytracer-in-C++-Introduction-What-is-ray-tracing.html
	It is free to use for educational purpose and cannot be redistributed outside of the tutorial pages. */

// YOU SHOULD _NOT_ NEED TO MODIFY THIS FILE (FOR ASSIGNMENT 1, 2, OR 3)

#include <iostream>
#include <cmath>

#include "Scene.h"
#include "Config.h"
#include "SceneObjects.h"

#include "ImageIO.h"

#define SCENE_VERSION_MAJOR 1
#define SCENE_VERSION_MINOR 5

static const Vector NullVector = { 0.0f,0.0f,0.0f };
static const Point Origin = { 0.0f,0.0f,0.0f };
static const SimpleString emptyString("");

bool GetMaterial(const Config &sceneFile, Material &currentMat)
{
    SimpleString materialType = sceneFile.GetByNameAsString("Type", emptyString);

	if (materialType.compare("checkerboard") == 0)
	{
        currentMat.type = TextureType::CHECKERBOARD;
	} 
	else if (materialType.compare("wood") == 0)
    {
        currentMat.type = TextureType::WOOD;
	}
	else if (materialType.compare("circles") == 0)
	{
		currentMat.type = TextureType::CIRCLES;
	}
	else
    { 
        // default
        currentMat.type = TextureType::GOURAUD;
    }

	currentMat.size = float(sceneFile.GetByNameAsFloat("Size", 0.0f));
	currentMat.offset = sceneFile.GetByNameAsVector("Offset", NullVector);
	currentMat.diffuse = sceneFile.GetByNameAsFloatOrColour("Diffuse", 0.0f);
	currentMat.diffuse2 = sceneFile.GetByNameAsFloatOrColour("Diffuse2", 0.0f);
	currentMat.reflection = float(sceneFile.GetByNameAsFloat("Reflection", 0.0f));
	if (currentMat.reflection == 0)
	{
		currentMat.reflection = float(sceneFile.GetByNameAsFloat("Refraction", 0.0f));
	}
	currentMat.specular = sceneFile.GetByNameAsFloatOrColour("Specular", 0.0f);
	currentMat.power = float(sceneFile.GetByNameAsFloat("Power", 0.0f)); 

	return true;
}

bool GetSphere(const Config &sceneFile, const Scene& scene, Sphere &currentSph)
{
    currentSph.pos = sceneFile.GetByNameAsPoint("Center", Origin); 
    currentSph.size =  float(sceneFile.GetByNameAsFloat("Size", 0.0f)); 

	return true;
}

bool GetPlane(const Config& sceneFile, const Scene& scene, Plane& currentPlane)
{
	currentPlane.pos = sceneFile.GetByNameAsPoint("Center", Origin);
	currentPlane.normal = normalise(sceneFile.GetByNameAsVector("Normal", NullVector));

	return true;
}

bool GetBox(const Config& sceneFile, const Scene& scene, Box& currentBox)
{
	currentBox.pos = sceneFile.GetByNameAsPoint("Center", Origin);
	currentBox.halfSize = sceneFile.GetByNameAsVector("HalfSize", NullVector);

	return true;
}

bool GetObject(const Config& sceneFile, const Scene& scene, SceneObject& currentObj)
{
	SimpleString objectType = sceneFile.GetByNameAsString("Type", emptyString);

	SimpleString opType = sceneFile.GetByNameAsString("Operator", emptyString);
	if (opType.compare("Intersection") == 0)
	{
		currentObj.op = Operator::INTERSECTION;
	}
	else if (opType.compare("Subtraction") == 0)
	{
		currentObj.op = Operator::SUBTRACTION;
	} 
	else if (opType.compare("Smooth") == 0)
	{
		currentObj.op = Operator::SMOOTH;
	}
	else
	{
		currentObj.op = Operator::UNION;
	}
	
	currentObj.materialId = sceneFile.GetByNameAsInteger("Material.Id", 0);

	if (currentObj.materialId >= scene.numMaterials)
	{
		fprintf(stderr, "Malformed Scene file: Object Material Id not valid.\n");
		return false;
	}

	if (objectType.compare("Sphere") == 0)
	{
		currentObj.type = PrimitiveType::SPHERE;
		return GetSphere(sceneFile, scene, currentObj.sphere);
	}
	else if (objectType.compare("Plane") == 0)
	{
		currentObj.type = PrimitiveType::PLANE;
		return GetPlane(sceneFile, scene, currentObj.plane);
	}
	else if (objectType.compare("Box") == 0)
	{
		currentObj.type = PrimitiveType::BOX;
		return GetBox(sceneFile, scene, currentObj.box);
	}

	return true;
}

void GetLight(const Config &sceneFile, Light &currentLight)
{
	currentLight.pos = sceneFile.GetByNameAsPoint("Position", Origin); 
	currentLight.intensity = sceneFile.GetByNameAsFloatOrColour("Intensity", 0.0f);
}

bool init(const char* inputName, Scene& scene)
{
//	int nbMats, nbSpheres, nbBlobs, nbLights, 
	unsigned int versionMajor, versionMinor;
	Config sceneFile(inputName);
    if (sceneFile.SetSection("Scene") == -1)
    {
		fprintf(stderr, "Malformed Scene file: No Scene section.\n");
		return false;
    }

	versionMajor = sceneFile.GetByNameAsInteger("Version.Major", 0);
	versionMinor = sceneFile.GetByNameAsInteger("Version.Minor", 0);

	if (versionMajor != SCENE_VERSION_MAJOR || versionMinor != SCENE_VERSION_MINOR)
	{
        fprintf(stderr, "Malformed Scene file: Wrong scene file version.\n");
		return false;
	}

	scene.skyboxMaterialId = sceneFile.GetByNameAsInteger("Skybox.Material.Id", 0);

	// camera details
	scene.cameraPosition = sceneFile.GetByNameAsPoint("Camera.Position", Origin);
    scene.cameraRotation = -float(sceneFile.GetByNameAsFloat("Camera.Rotation", 45.0f)) * PIOVER180;

    scene.cameraFieldOfView = float(sceneFile.GetByNameAsFloat("Camera.FieldOfView", 45.0f));
    if (scene.cameraFieldOfView <= 0.0f || scene.cameraFieldOfView >= 189.0f)
    {
	    fprintf(stderr, "Malformed Scene file: Out of range FOV.\n");
        return false;
    }

	scene.exposure = float(sceneFile.GetByNameAsFloat("Exposure", 1.0f));

	scene.numMaterials = sceneFile.GetByNameAsInteger("NumberOfMaterials", 0);
	scene.numLights = sceneFile.GetByNameAsInteger("NumberOfLights", 0);

	scene.numObjects = sceneFile.GetByNameAsInteger("NumberOfObjects", 0);

	scene.materialContainer = new Material[scene.numMaterials];
	scene.lightContainer = new Light[scene.numLights];
	scene.objectContainer = new SceneObject[scene.numObjects];

	// have to read the materials section before the material ids (used for the triangles, 
	// spheres, and planes) can be turned into pointers to actual materials
	for (unsigned int i = 0; i < scene.numMaterials; ++i)
    {   
        Material &currentMat = scene.materialContainer[i];
        SimpleString sectionName("Material");
        sectionName.append((unsigned long) i);
        if (sceneFile.SetSection( sectionName ) == -1)
        {
			fprintf(stderr, "Malformed Scene file: Missing Material section.\n");
		    return false;
        }
        if (!GetMaterial(sceneFile, currentMat))
		{
			fprintf(stderr, "Malformed Scene file: Malformed Material section.\n");
		    return false;
		}
    }

	for (unsigned int i = 0; i < scene.numLights; ++i)
	{
		Light &currentLight = scene.lightContainer[i];
		SimpleString sectionName("Light");
		sectionName.append((unsigned long)i);
		if (sceneFile.SetSection(sectionName) == -1)
		{
			fprintf(stderr, "Malformed Scene file: Missing Light section.\n");
			return false;
		}
		GetLight(sceneFile, currentLight);
	}

	for (unsigned int i = 0; i < scene.numObjects; ++i)
    {   
        SceneObject &currentObj = scene.objectContainer[i];
        SimpleString sectionName("Object");
        sectionName.append((unsigned long) i);
        if (sceneFile.SetSection( sectionName ) == -1)
        {
			fprintf(stderr, "Malformed Scene file: Missing Object section %d.\n", i);
		    return false;
        }
		if (!GetObject(sceneFile, scene, currentObj))
		{
			fprintf(stderr, "Malformed Scene file: Object %d section.\n", i);
		    return false;
		}
    }

	return true;
}

