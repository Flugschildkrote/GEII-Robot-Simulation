#ifndef GWCAMERA_H
#define GWCAMERA_H

#include "gwGlobalHeader.h"
#include "gwComponent.h"

namespace gwe {
class gwCamera : public gwComponent
{
public:
    gwCamera(void);
    virtual ~gwCamera(void);

    glm::mat4 getView(void) const;
    glm::mat4* getView_p(void);

    void update(void);
    void render(void);

protected:
    glm::mat4 mView;
private:


};
}
#endif // GWCAMERA_H
