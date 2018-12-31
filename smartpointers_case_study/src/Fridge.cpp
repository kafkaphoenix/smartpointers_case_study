#include "Engine.h"
#include "Fridge.h"

class Fridge::FridgeImpl
{
    public:
        void coolDown(){
            //
        }
    private:
        Engine engine_;
};

Fridge::Fridge() = default;
Fridge::~Fridge() = default;

void Fridge::coolDown()
{
    impl_->coolDown();
}
/*
The class now delegates its functionalities and members to FridgeImpl , and Fridge only has to
forward the calls and manage the life cycle of the impl_ pointer.
*/
/*
What makes it work is that pointers only need a forward declaration to compile . For
this reason, the header file of the Fridge class doesn't need to see the full definition of
FridgeImpl , and therefore neither do Fridge 's clients.
*/
