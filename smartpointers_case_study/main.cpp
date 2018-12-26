#include <iostream>
using std::cout;
using std::endl;
using std::cin;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <algorithm>
using std::find;
#include <list>
using std::list;

int main()
{

    //Objects allocated on Stack vs Heap

    /*Objects allocated on the stack are automatically destroyed when they go out of scope*/
    /*The heap is where dynamically allocated objects are stored, that is to say objects that are
    allocated with a call to new , which returns a pointer. To destroy an object allocated by new,
    we have to do it manually by calling delete*/
    /*This offers the advantages of keeping them longer than the end of a scope, and without incurring any
    copy except those of pointers which are very cheap. Also, pointers allow to manipulate objects
    polymorphically: a pointer to a base class can in fact point to objects of any derived class*/

    //RAI

    /*The principle of RAII is simple: wrap a resource (a pointer for instance) into an object, and
    dispose of the resource in its destructor. And this is exactly what smart pointers do*/

    //Smart Pointers

    /*First, a smart pointer syntactically behaves like a pointer in many way: it can be dereferenced
    with operator* or operator-> , that is to say you can call *sp or sp->member on it. And it
    is also convertible to bool, so that it can be used in an if statement like a pointer. Finally,
    the underlying pointer itself is accessible with a .get() method.*/

    /*it
    doesn't deal with copy! Indeed, as is, a SmartPointer copied also copies the underlying
    pointer, so the below code has a bug:*/
    {
    SmartPointer<int> sp1(new int(42));
    SmartPointer<int> sp2 = sp1; // now both sp1 and sp2 point to
    // the same object
    } // sp1 and sp2 are both destroyed, the pointer is deleted twice!

    //Smart Pointers Types
    /*
     std::unique_ptr
     raw pointer
     std::shared_ptr
     std::weak_ptr
     boost::scoped_ptr
     std::auto_ptr
    */

    //std::unique_ptr

    //The semantics of std::unique_ptr is that it is the sole owner of a memory resource.
    /*Note that std::unique_ptr is the preferred pointer to return from a factory function.
    std::unique_ptr<House> buildAHouse();
    But this works the other way around too, by passing
    an std::unique_ptr as a parameter
    class House
    {
    public:
    House(std::unique_ptr<PileOfWood> wood);
    …
    In this case, the house takes ownership of the PileOfWood*/

    /*Note though that even when you receive a unique_ptr, you're not guaranteed that no one else
    has access to this pointer. Indeed, if another context keeps a copy of the pointer inside your
    unique_ptr , then modifying the pointed to object through the unique_ptr object will of
    course impact this other context. If you don't want this to happen, the way to express it is by
    using a unique_ptr to const :*/

    std::unique_ptr<const House> buildAHouse();
    // for some reason, I don't want you
    // to modify the house you're being passed

    /*To ensure that there is only one unique_ptr that owns a memory resource, std::unique_ptr
    cannot be copied. The ownership can however be transferred from one unique_ptr to another
    (which is how you can pass them or return them from a function) by moving a unique_ptr
    into another one.
    A move can be achieved by returning an std::unique_ptr by value from a function, or explicitly
    in code:*/

    std::unique_ptr<int> p1 = std::make_unique(42);
    std::unique_ptr<int> p2 = move(p1); // now p2 hold the resource and p1 no longer hold anything

    //Raw pointers

    /*raw pointers and references represent access to an object, but not ownership . In fact,
    this is the default way of passing objects to functions and methods:*/

    /*This is particularly relevant to note when you hold an object with a unique_ptr and want to
    pass it to an interface. You don’t pass the unique_ptr , nor a reference to it, but rather a
    reference to the pointed to object:*/

     std::unique_ptr<House> house = buildAHouse();
     renderHouse(*house);

     //std::shared_ptr

     /*A single memory resource can be held by several std::shared_ptr s at the same
    time . The shared_ptr s internally maintain a count of how many of them there are holding
    the same resource, and when the last one is destroyed, it deletes the memory resource.*/

     /*Therefore std::shared_ptr allows copies, but with a reference-counting mechanism to make
    sure that every resource is deleted once and only once.*/
    /*One good case for using std::shared_ptr though is when objects are shared in the
    domain . Using shared pointers then reflects it in an expressive way. Typically, the nodes of a
    graphs are well represented as shared pointers, because several nodes can hold a reference to
    one other node.*/

    //std::weak_ptr

    /*std::weak_ptr s can hold a reference to a shared object along with other
    std::shared_ptr s, but they don't increment the reference count. This means that if no more
    std::shared_ptr are holding an object, this object will be deleted even if some weak pointers
    still point to it. For this reason, a weak pointer needs to check if the object it points to
    is still alive. To do this, it has to be copied into to a std::shared_ptr*/

    void useMyWeakPointer(std::weak_ptr<int> wp){
        if (std::shared_ptr<int> sp = wp.lock()){
            // the resource is still here and can be used
        }else{
            // the resource is no longer here
        }
    }
    //A typical use case for this is about breaking shared_ptr circular references

    struct House
    {
        std::shared_ptr<House> neighbour;
    };
    std::shared_ptr<House> house1 = std::make_shared<House>();
    std::shared_ptr<House> house2 = std::make_shared<House>();;
    house1->neighbour = house2;
    house2->neighbour = house1;
    /*None of the houses ends up being destroyed at the end of this code, because the shared_ptr s
    points into one another. But if one is a weak_ptr instead, there is no longer a circular
    reference.
    Another use case pointed out by this answer https://stackoverflow.com/questions/12030650/when-is-stdweak-ptr-useful
    on Stack Overflow is that weak_ptr can be used to maintain a cache . The data may or may not have been cleared from the cache, and the
    weak_ptr references this data.*/

    //boost::scoped_ptr
    /*It simply disables the copy and even the move construction. So it is the sole owner of a resource,
    and its ownership cannot be transferred. Therefore, a scoped_ptr can only live inside... a
    scope. Or as a data member of an object.*/

    //std::auto_ptr
    /*auto_ptr was present in C++98, has been deprecated in C++11 and removed from the
    language in C++17.
    It aimed at filling the same need as unique_ptr , but back when move semantics didn't exist in
    C++. It essentially does in its copy constructor what unique_ptr does in its move
    constructor. But auto_ptr is inferior to unique_ptr and you shouldn't use it if you have
    access to unique_ptr , because it can lead to erroneous code
    */
    std::auto_ptr<int> p1(new int(42));
    std::auto_ptr<int> p2 = p1; // it looks like p2 == p1, but no!
    // p1 is now empty and p2 uses the resource



    return 0;
}
