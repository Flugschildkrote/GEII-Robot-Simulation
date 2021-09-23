#ifndef SINGLETON_H_INCLUDED
#define SINGLETON_H_INCLUDED

#include <cstdlib>
#include <iostream>



namespace gwe
{
    namespace singleton
    {
        template <typename T>
        class gwSingleton
        {
        protected:
            gwSingleton(void) {}
            virtual ~gwSingleton(void) { }
            static T* mInstance;
        public:

            static T* getInstance(void)
            {
                if(mInstance == nullptr)
                {
                    mInstance = new T;
                }
                return (static_cast<T*>(gwSingleton::mInstance));
            }

            static void deleteInstance(void)
            {
                if(mInstance != nullptr)
                {
                    delete mInstance;
                    mInstance = nullptr;
                }
            }
        };

        template <typename T>
        T* gwSingleton<T>::mInstance = nullptr;
    }
}


#endif // SINGLETON_H_INCLUDED
