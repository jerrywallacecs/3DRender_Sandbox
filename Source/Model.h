#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include "Textures/Texture.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
	Model(const char* path);

	void Draw(Shader& shader);
	void Draw(Shader& shader, Shader& outlineShader, glm::mat4& modelMatrix);

private:
	// model data
	std::vector<Mesh> meshes;
	std::vector<Texture> loadedTextures;
	std::string directory;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};


#endif