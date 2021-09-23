#include "gwMeshRenderer.h"
#include "gwMesh.h"

using namespace gwe;

gwe::gwMeshRenderer::gwMeshRenderer(gwMesh* mesh) : gwComponent(GW_COMPONENT_MESH_RENDERER), mMesh(mesh)
{
    //ctor
}

gwe::gwMeshRenderer::~gwMeshRenderer(void)
{
    //dtor
}


gwMesh* gwe::gwMeshRenderer::getMesh(void) const { return mMesh; }

void gwe::gwMeshRenderer::render(void)
{
    if(mMesh == nullptr)
        return;

   // std::cout << "RENDERING" << std::endl;
    glBindVertexArray(mMesh->getVao());
            glDrawArrays(mMesh->getDrawMode(), 0, mMesh->getVerticesCount());
    glBindVertexArray(0);
}

void gwe::gwMeshRenderer::setMesh(gwMesh* mesh) { mMesh = mesh; }
