#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include "solvespace.h"

namespace SolveSpace {

static void DoTheSceneProcessing(const aiScene *scene) {
}

void ImportStl(const Platform::Path &filename) {
    // Create an instance of the Importer class
    Assimp::Importer importer;
    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll
    // propably to request more postprocessing than we do in this example.
    const aiScene* scene = importer.ReadFile( filename.raw.c_str(),
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
    DoTheSceneProcessing(scene);
    // We're done. Everything will be cleaned up by the importer destructor
}

}
