#include "Pixy.h"
#include "gwResourceManager.h"
#include "gwStaticMesh.h"
#include "gwTexture.h"
#include "gwScene.h"
#include "gwGameObject.h"
#include "gwMeshRenderer.h"
#include "gwMaterial.h"
#include "gwTransform.h"
#include "gwCamera.h"
#include "gwShader.h"
#include <algorithm>
#include <list>

using namespace gwe;

simu::Pixy::Pixy( gwe::gwScene* scene,gwe::gwShader* shader,const std::string &name, gwe::gwGameObject* parent) : mParent(parent), mName(name)
{
    Setup(scene, shader);
}

simu::Pixy::~Pixy()
{
    //dtor
}

void simu::Pixy::Run(void){
    mObjectBlocks.clear();
    mObjectBlocks.reserve(mBlockList.size());


    int largest = 0;
    int lastArea = 0;
    for(unsigned int i(0); i < mBlockList.size(); i++){
        simu::Pixy_Block tmpBlock;
        tmpBlock.width = (mBlockList[i].x2-mBlockList[i].x1);
        tmpBlock.height = (mBlockList[i].y2-mBlockList[i].y1);

        tmpBlock.xCenter = tmpBlock.width/2+mBlockList[i].x1;
        tmpBlock.yCenter = tmpBlock.height/2+mBlockList[i].y1;
        mObjectBlocks.push_back(tmpBlock);
        if(tmpBlock.getArea() > lastArea){
            largest = i;
            lastArea = tmpBlock.getArea();
        }
    }

    std::sort(mObjectBlocks.begin(), mObjectBlocks.end(), simu::isGreater);
  /*  if(lastArea != 0){
        std::cout << mObjectBlocks[0].yCenter << std::endl;
    }*/
}

void simu::Pixy::updateTexture(void){
    gwTexture* texture = mRenderer->getColorTexture();
    glBindTexture(GL_TEXTURE_2D, texture->getID());

    int pixelsCount = PIXY_WIDTH*PIXY_HEIGHT;
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, mPixels);

    /*for(int i(0); i < PIXY_WIDTH*PIXY_HEIGHT; i++){
        int index = i*4;
        mPixels[i].R = &mPixelsBuffer[index];
        mPixels[i].G = &mPixelsBuffer[index+1];
        mPixels[i].B = &mPixelsBuffer[index+2];
        mPixels[i].A = &mPixelsBuffer[index+3];
    }*/

 /*   bool hit = false;
    int xPos = 0;
    int yPos = 0;
    int minX = -1;
    int maxX = -1;
    int minY = -1;
    int maxY = -1;*/
    bool pixels_b[PIXY_WIDTH][PIXY_HEIGHT];
    bool lineDetected = false;

    /** PROCESS LOOP */
    std::list<simu::Line> mLinesStack;
    simu::Line tmpLine;
    for(int y(0); y < PIXY_HEIGHT; y++){
        tmpLine.y = y;
        for(int x(0); x < PIXY_WIDTH; x++){
            int index = PIXY_WIDTH*y+x;
            if((mPixels[index].R == 255 &&  mPixels[index].G == 201 &&  mPixels[index].B == 13) ||
               (mPixels[index].R == 254 &&  mPixels[index].G == 254 &&  mPixels[index].B == 254))
            {
              //  mPixels[index].R = 255;
              //  mPixels[index].G = 255;
              //  mPixels[index].B = 255;
                if(!lineDetected){
                    tmpLine.start = x;
                    lineDetected = true;
                }
                tmpLine.end = x;

                if(x == PIXY_WIDTH-1){
                    mLinesStack.push_back(tmpLine);
                    lineDetected = false;
                }
            }else{
                if(lineDetected){
                    lineDetected = false;
                    mLinesStack.push_back(tmpLine);
                }
            }
        }
    }

    std::list<simu::Line> mProcessingStack;
    mBlockList.clear();
    while(!mLinesStack.empty()){
        /** On cherche la ligne le plus en bas à gauche*/
        simu::Line startLine = *mLinesStack.begin();
        for(std::list<simu::Line>::iterator it(mLinesStack.begin()); it != mLinesStack.end(); it++){
            if(*it < startLine){
                startLine = *it;
            }
        }


        /** Après avoir trouvé la ligne de départ pour le block on la retire du block principal et on l'ajoute au la liste d'analyse*/
        mLinesStack.remove(startLine);
        mProcessingStack.push_back(startLine);
        /** Ensuite on cherche toutes les lignes en contact avec la ligne actuelle, on la retire de la liste d'analyse et on l'ajoute a la liste de block*/
        simu::Block mBlockStack;
        while(!mProcessingStack.empty()){
            simu::Line processingLine = *mProcessingStack.begin();
            /** Pour chaque ligne trouvée, on la retire du block principal et on l'ajoute au block d'analyse;*/
            std::vector<simu::Line> foundLines;
            for(std::list<simu::Line>::iterator it(mLinesStack.begin()); it != mLinesStack.end(); it++){
                if(processingLine.line_cross(*it)){
                    foundLines.push_back(*it);
                }
            }
            for(int i(0); i < foundLines.size(); i++){
                mLinesStack.remove(foundLines[i]);
                mProcessingStack.push_back(foundLines[i]);
            }

            mProcessingStack.remove(processingLine);
            mBlockStack.lines.push_back(processingLine);

        }
        mBlockStack.compute();
        mBlockList.push_back(mBlockStack);
           // for(int )

    }

   /** std::cout << "Blocks : " << mBlockList.size() << std::endl;*/

    for(int i(0); i < mBlockList.size(); i++){
        int x1 = mBlockList[i].x1;
        int x2 = mBlockList[i].x2;
        int y1 = mBlockList[i].y1;
        int y2 = mBlockList[i].y2;

        int width = (mBlockList[i].x2-mBlockList[i].x1);
        int height = (mBlockList[i].y2-mBlockList[i].y1);

        int xCenter = width/2+mBlockList[i].x1;
        int yCenter = height/2+mBlockList[i].y1;

        for(int x(x1); x <= x2; x++){
            for(int y(y1); y <= y2; y++){
                if(x == x1 || x == x2 || y == y1 || y == y2){
                    int index = x+y*PIXY_WIDTH;
                    mPixels[index].R = 255;
                    mPixels[index].G = 0;
                    mPixels[index].B = 255;
                    mPixels[index].A = 255;
                }else if(x > xCenter-1 && x < xCenter+1 && y > yCenter-1 && y < yCenter+1 ){
                    int index = x+y*PIXY_WIDTH;
                    mPixels[index].R = 255;
                    mPixels[index].G = 0;
                    mPixels[index].B = 0;
                    mPixels[index].A = 255;
                }
            }
        }
    }
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, PIXY_WIDTH, PIXY_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, mPixels);
    glBindTexture(GL_TEXTURE_2D, 0);
}

gwe::gwGameObject* simu::Pixy::getGameObject(void) const { return mGameObject; }

void simu::Pixy::InitGameObject(gwe::gwScene* scene, gwe::gwShader* shader){
    std::string prefix = "PIXY_";
    int index = 0;
    while(scene->getGameObject(Physics::make_string(prefix, index))){
        index++;
    }

    mGameObject = new gwGameObject(Physics::make_string(prefix, index), mParent);
    scene->addGameObject(mGameObject);
   // mGameObject->getTransform()->rotate(30, glm::vec3(1,0,0), GW_WORLD_SPACE);

    gwMeshRenderer* renderer = new gwMeshRenderer(dynamic_cast<gwStaticMesh*>(gwe::singleton::gwResourceManager::getInstance()->getResource(PIXY_MESH[RESOURCE_NAME]+mName)));
    mGameObject->addComponent(renderer);
    gwMaterial* material = new gwMaterial;
    material->setTexture(dynamic_cast<gwTexture*>(gwe::singleton::gwResourceManager::getInstance()->getResource(PIXY_TEXTURE[RESOURCE_NAME]+mName)));
    mGameObject->addComponent(material);
   // mGameObject->addComponent(camera);

    gwCamera* camera = new gwCamera();
    std::cout << "[***************************************************]" << std::endl;
    mRenderer = new gwOffScreenRenderer("Renderer1"+mName, PIXY_WIDTH, PIXY_HEIGHT, scene);
    mRenderer->setCamera(camera);
    mRenderer->setShader(mShader);
    mGameObject->addComponent(camera);
}

void simu::Pixy::InitResources(void){
    singleton::gwResourceManager* mgr = gwe::singleton::gwResourceManager::getInstance();

    if(mgr->getResource(PIXY_MESH[RESOURCE_NAME]+mName) == nullptr){
        gwStaticMesh* mesh = new gwStaticMesh(PIXY_MESH[RESOURCE_NAME]+mName, PIXY_MESH[RESOURCE_FILE]);
        mesh->load();
    }

    if(mgr->getResource(PIXY_TEXTURE[RESOURCE_NAME]+mName) == nullptr){
        gwTexture* texture = new gwTexture(PIXY_TEXTURE[RESOURCE_NAME]+mName, PIXY_TEXTURE[RESOURCE_FILE]);
        texture->load();
    }

    mShader = dynamic_cast<gwShader*>(mgr->getResource(PIXY_SHADER[RESOURCE_NAME]+mName));
    if(mShader == nullptr){
        mShader = new gwShader(PIXY_SHADER[RESOURCE_NAME]+mName, PIXY_SHADER[1],PIXY_SHADER[2]);
        mShader->load();
    }
}

void simu::Pixy::Setup(gwe::gwScene* scene, gwe::gwShader* shader){
    InitResources();
    InitGameObject(scene, shader);
}

std::vector<simu::Pixy_Block> simu::Pixy::getBlocks(void) const { return mObjectBlocks; }
