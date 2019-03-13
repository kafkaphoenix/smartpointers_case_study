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

    //RAII

    /*The principle of RAII is simple: wrap a resource (a pointer for instance) into an object, and
    dispose of the resource in its destructor. And this is exactly what smart pointers do*/

    //Smart Pointers

    /*First, a smart pointer syntactically behaves like a pointer in many way: it can be dereferenced
    with operator* or operator-> , that is to say you can call *sp or sp->member on it. And it
    is also convertible to bool, so that it can be used in an if statement like a pointer. Finally,
    the underlying pointer itself is accessible with a .get() method.*/

    /*it doesn't deal with copy! Indeed, as is, a SmartPointer copied also copies the underlying
    pointer, so the below code has a bug:*/
    {
    //SmartPointer<int> sp1(new int(42));
    //SmartPointer<int> sp2 = sp1; // now both sp1 and sp2 point to
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

    //std::unique_ptr<const House> buildAHouse();
    // for some reason, I don't want you
    // to modify the house you're being passed

    /*To ensure that there is only one unique_ptr that owns a memory resource, std::unique_ptr
    cannot be copied. The ownership can however be transferred from one unique_ptr to another
    (which is how you can pass them or return them from a function) by moving a unique_ptr
    into another one.
    A move can be achieved by returning an std::unique_ptr by value from a function, or explicitly
    in code:*/

    /*std::unique_ptr<int> p1 = std::make_unique(42);
    std::unique_ptr<int> p2 = std::move(p1); // now p2 hold the resource and p1 no longer hold anything*/

    //Raw pointers

    /*raw pointers and references represent access to an object, but not ownership. In fact,
    this is the default way of passing objects to functions and methods:*/

    /*This is particularly relevant to note when you hold an object with a unique_ptr and want to
    pass it to an interface. You don’t pass the unique_ptr , nor a reference to it, but rather a
    reference to the pointed to object:*/

     /*std::unique_ptr<House> house = buildAHouse();
     renderHouse(*house);*/

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

    /*void useMyWeakPointer(std::weak_ptr<int> wp){
        if (std::shared_ptr<int> sp = wp.lock()){
            // the resource is still here and can be used
        }else{
            // the resource is no longer here
        }
    }*/
    //A typical use case for this is about breaking shared_ptr circular references

    /*struct House
    {
        std::shared_ptr<House> neighbour;
    };
    std::shared_ptr<House> house1 = std::make_shared<House>();
    std::shared_ptr<House> house2 = std::make_shared<House>();;
    house1->neighbour = house2;
    house2->neighbour = house1;*/
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
    //std::auto_ptr<int> p1(new int(42));
    //std::auto_ptr<int> p2 = p1; // it looks like p2 == p1, but no!
    // p1 is now empty and p2 uses the resource

    //PIMPL IDIOM by using unique_ptr (See Fridge.h and Fridge.cpp)

    //How to Transfer unique_ptr from a set to another set

    /*Given a base class and a derived one. If we want a collection of several objects implementing Base, but that could be of any
    derived classes and we want to prevent our collection to have duplicates, we could use std::set
    std::set<std::unique_ptr<Base>>*/

    /*operator== is not used by std::set. Elements a and b are considered equal iff(if and only if) !(a < b) && !(b < a)
    therefore the comparison between elements of the set will call the operator< of std::unique_ptr ,
    which compares the memory addresses of the pointers inside them.
    To implement no logical duplicates, we need to call the operator< on Base (provided that it
    exists, maybe using an id provided by Base for instance) to compare elements and determines
    whether they are duplicates. And to make the set use this operator, we need to customize the
    comparator of the set.
    std::set supports specifying a comparison function. The default is less which will use operator <
    to check equality. You can define a custom function to check equality and use that one instead:*/

    /*
    struct ComparePointee
    {
    template<typename T>
    bool operator()(std::unique_ptr<T> const& up1, std::unique_ptr<T>const& up2)
    {
    return *up1 < *up2;
    }
    };
    std::set<std::unique_ptr<int>, ComparePointee> mySet;
    To avoid writing this type every time we instantiate such a set in code, we can hide its technical
    aspects behind an alias:
    template<typename T>
    using UniquePointerSet = std::set<std::unique_ptr<T>,ComparePointee>;

    UniquePointerSet<Base> source;
    source.insert(std::make_unique<Derived>());

    To transfer elements efficiently , we use the insert method:
    destination.insert(begin(source), end(source));
    But this leads to a compilation error!
    Indeed, the insert methods attemps to make a copy of the unique_ptr elements.
    */
    //C++17's new method on set: merge
    /*
    destination.merge(source);
    This makes destination take over the nodes of the tree inside of source. It's like performing a
    splicing on lists. So after executing this line, destination has the elements that source had, and
    source is empty.
    */
    //Pre C++17
    /*
    We can't use std::move
    The standard algorithm to move elements from a collection to another collection is std::move .
    Here is how it works with std::vector :
    std::vector<std::unique_ptr<Base>> source;
    source.push_back(std::make_unique<Derived>());
    std::vector<std::unique_ptr<Base>> destination;
    std::move(begin(source),end(source),std::back_inserter(destination));
    after the execution of this line, destination has the elements that source had and source is
    not empty, but has empty unique_ptr .But if we try this approach with our example we get the
    same compilation error as in the beginning, some unique_ptr are getting copied
    So before C++17, moving elements from a set doesn't seem to be possible. Something has to
    give: either moving, or the sets. This leads us to two possible aspects to give up on.

    */
    //Keeping the set but paying up for the copies
    /*To give up on the move and accepting to copy the elements from a set to another, we need to
    make a copy of the contents pointed by the unique_ptrs. For this, let's assume that Base has is a
    polymorphic clone method overridden in Derived :*/

//    class Base
//    {
//        public:
//        virtual std::unique_ptr<Base> cloneBase() const = 0;
//        // rest of Base...
//    };
//    class Derived : public Base
//    {
//        public:
//        std::unique_ptr<Base> cloneBase() const override
//        {
//        return std::make_unique<Derived>(*this);
//        }
//        // rest of Derived...
//    };

//    At call site, we can make copies of the unique_ptrs from a set over to the other one, for instance
//    this way:
//    auto clone = [](std::unique_ptr<Base> const& pointer){ return
//    pointer->cloneBase(); };
//    std::transform(begin(source), end(source), std::inserter(destination,
//    end(destination)), clone);
//    std::transform applies the given function to a range and stores the result in another range
//    A std::insert_iterator which can be used to insert elements into the container c at the position indicated by i.
//    Or, with a for loop:
//    for (auto const& pointer : source)
//    {
//    destination.insert(pointer->cloneBase());
//    }

    //Keeping the move and throwing away the set
    /*The set that doesn't let the move happen is the source set. If you only need the destination to
    have unique elements, you can replace the source set by a std::vector .
    Indeed, std::vector does not add a const to the value returned by its iterator*/


    //Custom deleters
    /*Let's take the example of a House class, that carries its building Instructions with it, which are
    polymorphic and can be either a Sketch or a full-fledged Blueprint. One way to deal with the life cycle of the Instructions is to store them as a unique_ptr in
    the House . And say that a copy of the house makes a deep copy of the instructions. We can't bind a unique_ptr to a stack-allocated object, because calling delete on it
    would cause undefined behaviour. One solution would be to make a copy of the blueprint and allocating it on the heap. This may be
    OK, or it may be costly. But passing objects allocated on the stack is a legitimate need. We then don't want the House to
    destroy the Instructions in its destructor when the object comes from the stack. std::unique_ptr can help here*/

    //Seeing the real face of std::unique_ptr

    /*Most of the time, the C++ unique pointer is used as std::unique_ptr<T> . But its complete
    type has a second template parameter, its deleter:
    This opens the possibility to use unique pointers for types that have a specific code for disposing
    of their resources. This happens in legacy code coming from C where a function typically takes
    care of deallocating an object along with its contents:
    struct GizmoDeleter
    {
    void operator()(Gizmo* p)
    {
        oldFunctionThatDeallocatesAGizmo(p);
    }
    };
    using GizmoUniquePtr = std::unique_ptr<Gizmo, GizmoDeleter>;
    */

    //Using several deleters

    /*Our initial problem was that we wanted the unique_ptr to delete the Instructions, except when
    they came from the stack in which case we wanted it to leave them alone.
    The deleter can be customized to delete or not delete, given the situation. For this we can use
    several deleting functions, all of the same function type (being
    void(*)(Instructions*) ):
    The deleting functions are then:

    void deleteInstructions(Instructions* instructions){ delete
    instructions;}
    void doNotDeleteInstructions(Instructions* instructions){}

    To use them, the occurrences of std::unique_ptr<Instructions> needs to be replaced
    with InstructionUniquePtr , and the unique pointers can be constructed this way:

    if (method == BuildingMethod::fromSketch)
        return House(InstructionsUniquePtr(new Sketch,
        deleteInstructions));
    if (method == BuildingMethod::fromBlueprint)
        return House(InstructionsUniquePtr(new Blueprint,
        deleteInstructions));

    Except when the parameter comes from the stack, in which case the no-op deleter can be used:

    Blueprint blueprint;

    House house(InstructionsUniquePtr(&blueprint, doNotDeleteInstructions));
    */
    /*we should note that if the unique_ptr is moved out of the
    scope of the stack object, it would point to a resource that doesn't exist any more. Using the
    unique_ptr after this point causes a memory corruption.*/

    /*if the constructor of House were to take
    more than one argument then we should declare the construction of the unique_ptr in a
    separate statement, like this:

    InstructionsUniquePtr instructions(new Sketch, deleteInstructions);
    return House(move(instructions), getHouseNumber());

    Indeed there could be a memory leak if an exception was thrown (cf Item 17 of Effective C++).
    And also that when we don't use custom deleters, we shouldn't use new directly, but prefer
    std::make_unique that lets you pass the arguments for the construction of the pointed-to
    object.*/

    /*In general, holding an std::unique_ptr means being its owner. And this means that it is OK
    to modify the pointed-to object. But in the case where the object comes from the stack (or from
    wherever else when it is passed with the no-op deleter), the unique pointer is just holding a
    reference to an externally owned object . In this case, you don't want the unique pointer to
    modify the object, because it would have side effects on the caller, and this would makes the
    code much more harder to reason about.
    Therefore, when using this technique make sure to work on pointer to const objects :
    using InstructionsUniquePtr =
    std::unique_ptr< const Instructions, void(*)( const
    Instructions*)>;
    and the deleters become:
    void deleteInstructions( const Instructions* instructions){ delete
    instructions;}
    void doNotDeleteInstructions( const Instructions* instructions){}
    This way, the unique pointer can't cause trouble outside of the class. This will save you a sizable
    amount of debugging.*/

    //A Unique Interface to simplify custom deleters and their ugliness

    /*
    Use the same interface for all custom deleters on all types .
    This can be defined in another namespace, a technical one. Let's call this namespace util for
    the example.
    Then in this namespace, we write all the common code that creates the custom unique_ptr .
    Let's call this helper MakeConstUnique for instance. Here is its code:
    namespace util
    {
    template<typename T>
    void doDelete(const T* p)
    {
    delete p;
    }
    template<typename T>
    void doNotDelete(const T* x)
    {
    }
    template<typename T>
    using CustomUniquePtr = std::unique_ptr<const T, void(*)(const
    T*)>;
    template<typename T>
    auto MakeConstUnique(T* pointer)
    {
    return CustomUniquePtr<T>(pointer, doDelete<T>);
    }
    template<typename T>
    auto MakeConstUniqueNoDelete(T* pointer)
    {
    return CustomUniquePtr<T>(pointer, doNotDelete<T>);
    }
    }
    With this code, no need to define anything else to start using a unique_ptr on a particular
    type with custom deleters. For instance, to create an instance of a unique_ptr that does a
    delete of its resource when it gets out of scope, we write:
    auto myComputer = util::MakeConstUnique(new
    store::electronics::gaming::Computer);
    And to create one that does not delete its resource:
    auto myComputer = util::MakeConstUniqueNoDelete(new
    store::electronics::gaming::Computer);
    What is interesting about this interface is that:
    there is no longer any mention of delete in the common case,
    we can now use auto , thanks to the return type of MakeConstUnique .
    Note that all this made us go down to one occurrence of the namespace of Computer ,
    instead of three.*/

    //Specific deleters

    /*Now what if, for some reason, we didn't want to call delete on the class Computer, but a
    particular dedicated function?*/

    /*To keep using MakeConstUnique with this type, we can make a total specialization of this
    template function for the type Computer . We could do this in the module defining Computer,
    by reopening the util namespace:
    namespace util
    {
    template<>
    auto MakeConstUnique(store::electronics::gaming::Computer*
    pointer)
    {
    return
    CustomUniquePtr<store::electronics::gaming::Computer>(pointer,
    specificFunctionThatFreesAComputer);
    }
    }*/

    //Changes of deleter during the life of an unique_ptr

    /*Here is a toy example we use an unique_ptr on int , with a customisable deleter:
    using IntDeleter = void(*)(int*);
    using IntUniquePtr = std::unique_ptr<int, IntDeleter>;
    One deleter is to be used for even numbers, and another one for odd numbers*/

    /*Consider the following code:
    IntUniquePtr p1(new int(42), deleteEvenNumber);
    IntUniquePtr p2(new int(43), deleteOddNumber);
    p1 = move(p2);*/
    /*Each resource is deleted with the correct deleter, which means that the assignment did bring
    over the deleter. This makes sense because the resources would not be disposed of with the
    correct deleter otherwise.*/

    /*Resetting the pointer
    Another way to change the resource contained in an std::unique_ptr is to call its reset
    method, like in the following simple example:
    std::unique_ptr<int> p1(new int(42));
    p1.reset(new int(43));
    The reset method calls the deleter on the current resource (42), and then takes on the new one
    (43).
    But the reset method only takes one argument , which is the new resource. It cannot be passed a
    deleter along with this new resource. For that reason, it can no longer be used directly in our
    example with even and odd numbers. Indeed, the following code:
    IntUniquePtr p1(new int(42), deleteEvenNumber);
    p1.reset(new int(43)); // can't pass deleteOddNumber*/

    /*In fact we could manually change the deleter in a separate statement, by exploiting the fact that
    the get_deleter method of unique_ptr returns the deleter by non-const reference:
    p1.get_deleter() = deleteOddNumber;
    But why doesn't reset have a deleter argument? And how to hand over a new resource to an
    std::unique_ptr along with its appropriate deleter in a single statement?
    Howard Hinnant, who is amongst many other things lead designer and author of the
    std::unique_ptr component, answers this question on Stack Overflow:
    Here is how to use his answer in our initial example:
    IntUniquePtr p1(new int(42), deleteEvenNumber);
    p1 = IntUniquePtr(new int(43), deleteOddNumber);
    which gives the following desired output*/


    //How to Return a Smart Pointer AND Use Covariance

    return 0;
}
