// https://learnopengl.com/Model-Loading/Model
#include <assimp/version.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <string>

using namespace std;

// https://stackoverflow.com/questions/1135841/c-multiline-string-literal
const string objContent = R"(
# Blender 3.3.1
# www.blender.org
o Cube
v 1.000000 1.000000 -1.000000
v 1.000000 -1.000000 -1.000000
v 1.000000 1.000000 1.000000
v 1.000000 -1.000000 1.000000
v -1.000000 1.000000 -1.000000
v -1.000000 -1.000000 -1.000000
v -1.000000 1.000000 1.000000
v -1.000000 -1.000000 1.000000
vn -0.0000 1.0000 -0.0000
vn -0.0000 -0.0000 1.0000
vn -1.0000 -0.0000 -0.0000
vn -0.0000 -1.0000 -0.0000
vn 1.0000 -0.0000 -0.0000
vn -0.0000 -0.0000 -1.0000
vt 0.625000 0.500000
vt 0.375000 0.500000
vt 0.625000 0.750000
vt 0.375000 0.750000
vt 0.875000 0.500000
vt 0.625000 0.250000
vt 0.125000 0.500000
vt 0.375000 0.250000
vt 0.875000 0.750000
vt 0.625000 1.000000
vt 0.625000 0.000000
vt 0.375000 1.000000
vt 0.375000 0.000000
vt 0.125000 0.750000
s 0
f 1/1/1 5/5/1 7/9/1 3/3/1
f 4/4/2 3/3/2 7/10/2 8/12/2
f 8/13/3 7/11/3 5/6/3 6/8/3
f 6/7/4 2/2/4 4/4/4 8/14/4
f 2/2/5 1/1/5 3/3/5 4/4/5
f 6/8/6 5/6/6 1/1/6 2/2/6
)";

void processMesh(aiMesh *mesh, const aiScene *scene)
{
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
		cout << "Vertex: " << mesh->mVertices[i].x << " " << mesh->mVertices[i].y << " " << mesh->mVertices[i].z << endl;
}

void processNode(aiNode *node, const aiScene *scene)
{
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        processMesh(mesh, scene);			
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene);
}  

int main()
{
    Assimp::Importer importer;
	const aiScene *scene = importer.ReadFileFromMemory(objContent.c_str(), objContent.size(), aiProcess_Triangulate | aiProcess_FlipUVs); 
	
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
        cout << "Assimp Error: " << importer.GetErrorString() << endl;
		return 0;
	}
	
	cout << "Scene loaded!!" << endl;
	
    processNode(scene->mRootNode, scene);
	
	return 0;
}
