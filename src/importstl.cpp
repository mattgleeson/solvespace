#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include "solvespace.h"

namespace SolveSpace {

static Vector _v(aiVector3D *v) {
    return Vector::From(v->x, v->y, v->z);
}

static void addLine(Vector start, Vector end) {
    hRequest hr = SS.GW.AddRequest(Request::Type::LINE_SEGMENT, /*rememberForUndo=*/false);
    SK.GetEntity(hr.entity(1))->PointForceTo(start);
    SK.GetEntity(hr.entity(2))->PointForceTo(end);
}

static void addTriangle(Vector v1, Vector v2, Vector v3, Vector normal) {
    addLine(v1, v2);
    addLine(v2, v3);
    addLine(v3, v1);
}

static void LoadMesh(const aiMesh *mesh) {
    // process each face in the mesh
    for (unsigned int i=0; i<mesh->mNumFaces; i++) {
        aiFace *face = &(mesh->mFaces[i]);
        aiVector3D *normal = &(mesh->mNormals[i]);
        ssassert(face->mNumIndices == 3, "triangles only");
        addTriangle(_v(&(mesh->mVertices[face->mIndices[0]])),
                    _v(&(mesh->mVertices[face->mIndices[1]])),
                    _v(&(mesh->mVertices[face->mIndices[2]])),
                    _v(normal));
    }
}

void ImportStl(const Platform::Path &filename) {
    // Create an instance of the Importer class
    Assimp::Importer importer;
    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll
    // propably to request more postprocessing than we do in this example.
    const aiScene* scene = importer.ReadFile(filename.raw.c_str(),
                                             aiProcess_CalcTangentSpace       |
                                             aiProcess_Triangulate            |
                                             aiProcess_JoinIdenticalVertices  |
                                             aiProcess_SortByPType);

    // If the import failed, report it
    if(!scene) {
        Message(importer.GetErrorString());
        return;
    }
    // Now we can access the file's contents.
    LoadMesh(scene->mMeshes[0]);
    // We're done. Everything will be cleaned up by the importer destructor
}

}
