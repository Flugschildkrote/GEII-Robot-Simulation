#ifndef PIXY_H
#define PIXY_H

#include "Physics.h"
#include "gwGlobalHeader.h"
#include "gwRenderer.h"

#ifndef RESOURCE_NAME
#define RESOURCE_NAME 0
#endif // RESOURCE_NAME

#ifndef RESOURCE_FILE
#define RESOURCE_FILE 1
#endif // RESOURCE_FILE

#ifndef PIXY_WIDTH
#define PIXY_WIDTH 320
#endif // PIXY_WIDTH

#ifndef PIXY_CENTER_X
#define PIXY_CENTER_X 150
#endif // PIXY_CENTER_X

#ifndef PIXY_HEIGHT
#define PIXY_HEIGHT 200
#endif // PIXY_HEIGHT

#ifndef PIXY_CENTER_Y
#define PIXY_CENTER_Y 100
#endif // PIXY_CENTER_Y


#ifndef PIXY_COMPONENT
#define PIXY_COMPONENT 4
#endif // PIXY_COMPONENT

#ifndef PIXY_BUFFER_SIZE
#define PIXY_BUFFER_SIZE sizeof(uint8_t)
#endif // PIXY_BUFFER_SIZE


const std::string PIXY_MESH[2] = {"PIXY_MESH", "Data/Meshs/Pixy.obj"};
const std::string PIXY_TEXTURE[2] = {"PIXY_TEXTURE", "Data/Textures/CNY_70.jpg"};

const std::string PIXY_SHADER[3] = {"PIXY_SHADER", "Data/Shaders/Default/3D/Textured_3D.vert", "Data/Shaders/Default/3D/Textured_3D.frag"};



namespace simu{

struct Pixel{
    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t A;
};

struct Square{
    int x1;
    int x2;
    int y1;
    int y2;
};

#ifndef LINE
#define LINE
struct Line{
    int y;
    int start;
    int end;

    friend std::ostream& operator<<( std::ostream& outs, const Line& line ){
        return outs << "Line " << line.y << "->[" << line.start << "," << line.end << "]";
    }

    bool line_cross(const Line &other){
        if((y-other.y) == 1 || (y-other.y == -1)){
            if(end < other.start || start > other.end){
                return false;
            }
            return true;
        }
        return false;
    }

    bool less_than(const Line &line2) const{
        if(y < line2.y) {return true;}
        return start < line2.start;
    }

    bool equal_to(const Line &other) const{
        return (y == other.y ) && (start == other.start) && (end == other.end);
    }
};

inline bool operator< (Line const &line1, Line const &line2) {
    return line1.less_than(line2);
}

inline bool operator==(Line const &line1, Line const &line2) {
    return line1.equal_to(line2);
}

struct Block{
    std::vector<Line> lines;
    int x1 = -1;
    int y1 = -1;
    int x2 = -1;
    int y2 = -1;
    void compute(void){
        if(lines.size() == 0)
            return;
        x1 = lines[0].start;
        x2 = lines[0].end;
        y1 = lines[0].y;
        y2 = lines[0].y;
        for(int i(1); i< lines.size(); i++){
            if(lines[i].start < x1){x1 = lines[i].start; }
            if(lines[i].end > x2){x2 = lines[i].end; }
            if(lines[i].y < y1){y1 = lines[i].y; }
            if(lines[i].y > y2){y2 = lines[i].y; }
        }
    }
};

struct Pixy_Block{
    int xCenter,yCenter,width, height;
    int getArea(void) {return width*height; }
};

inline bool operator<(const Pixy_Block &obj1, const Pixy_Block &obj2){
    int obj1_Area = obj1.width*obj1.height;
    int obj2_Area = obj2.width*obj2.height;
    return obj1_Area<obj2_Area;
}

inline bool isGreater(const Pixy_Block &obj1, const Pixy_Block &obj2){
    return (obj2<obj1);
}

#endif // LINE



class Pixy
{
public:
    Pixy(gwe::gwScene* scene,gwe::gwShader* shader,const std::string &name, gwe::gwGameObject* object = nullptr);
    virtual ~Pixy();

    void Run(void);
    void updateTexture(void);
    gwe::gwGameObject* getGameObject(void) const;
    std::vector<Pixy_Block> getBlocks(void) const;

private:
    void InitGameObject(gwe::gwScene* scene, gwe::gwShader* shader);
    void InitResources(void);
    void Setup(gwe::gwScene* scene, gwe::gwShader* shader);

    gwe::gwGameObject* mGameObject;
    gwe::gwGameObject* mParent;

    gwe::gwOffScreenRenderer* mRenderer;
    //uint8_t mPixelsBuffer[PIXY_HEIGHT*PIXY_WIDTH*PIXY_BUFFER_SIZE*PIXY_COMPONENT];
    Pixel mPixels[PIXY_HEIGHT*PIXY_WIDTH];
    gwe::gwShader *mShader;
    int mValue = 0;
    std::vector<simu::Block> mBlockList;
    std::vector<simu::Pixy_Block> mObjectBlocks;
    std::string mName;
};

}

#endif // PIXY_H
