#ifndef FRIDGE_H
#define FRIDGE_H
#include <memory>

/*If Fridge.h would #include Engine.h, any client of the Fridge class would
indirectly #include the Engine class. So when the Engine class is modified, all the clients of
Fridge have to recompile, even if they don't use Engine directly.*/

/*The pimpl idiom aims at solving this issue by adding a level of indirection, FridgeImpl , that
takes on the Engine .*/

class Fridge
{
    public:

        Fridge();
        ~Fridge(); //We declare the destructor and thus prevent the compiler from doing it for us
        void coolDown();
    private:
        class FridgeImpl;
        /*std::unique_ptr happens to check in its destructor if the definition of the type is visible
        before calling delete. So it refuses to compile and to call delete if the type is only forward
        declared.*/
        std::unique_ptr<FridgeImpl> impl_ = std::make_unique<FridgeImpl>();
};

    /*We can still use the default implementation for the destructor that the compiler would have
    generated. But we need to put it in the implementation file, after the definition of
    FridgeImpl*/

    //For more info visit //https://herbsutter.com/gotw/_100/

#endif // FRIDGE_H
