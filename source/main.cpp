/*
Smart Pointers
(c) 2016
Author: David Erbelding
Written under the supervision of David I. Schwartz, Ph.D., and
supported by a professional development seed grant from the B. Thomas
Golisano College of Computing & Information Sciences
(https://www.rit.edu/gccis) at the Rochester Institute of Technology.
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.
This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <string>
#include <memory>


int main() 
{
    // Management of dynamically allocated memory (new/delete) is one of the most common problems in C++.
    // Programmers frequently run into situations where a dynamically allocated object exists in multiple places, or doesn't get deleted when it should.
    // Many programmers have solved this problem, and prefer to do it in different ways, none of which are perfect. It boils down to personal preference.

    // Now that we've got that out of the way:
    // Smart pointers are a commonly used strategy that help manage memory.
    // In order to understand why someone might want to use smart pointers, we have an example that doesn't use them.

    {
        // (Note: this class is defined in local scope, it doesn't exist outside of this scope)
        class Person
        {
        public:                         // Our person class is simple.
            std::string name;           // Each person has a string for their name.
            Person* parent = nullptr;   // and a pointer to one parent nullptr by default (in this universe, humans reproduce by budding or something).

            Person(std::string name) : name(name)   // Our constructor creates a person with a name.
            {
                std::cout << name << " constructed" << std::endl;
            }

            ~Person()
            {
                std::cout << name << " destructed" << std::endl;
                if (parent != nullptr)
                {
                    delete parent;                  // The destructor also deletes our "parent" if we have one.
                }                                   // This will continue up the chain until there is no parent to delete.
            }
        };

        // Here we go:
        std::cout << "Creating objects with new as raw pointers: " << std::endl;
        Person* tim = new Person("tim");
        tim->parent = new Person("timothy");

        delete tim;     // What happens when we delete jack?
                        // jacky should also be deleted. If jacky is deleted, then any parent
                        // (in this case none) would also be deleted, and so on.

        std::cin.get();

        // What if we do this though:
        Person* jim = new Person("jim");
        jim->parent = new Person("jimothy");

        delete jim->parent;

        //      This line causes an error. Why?
        // delete jim;
        //              jim still has a pointer to jimothy (and tries to delete it), but jimothy has already been deleted. oops.

        // We can fix it like this, but it's not a great fix. (we should have just deleted jim to start with)
        jim->parent = nullptr;
        delete jim;

        // The problem here arises because we have access to the pointer in two places.
        // Jimothy should really belong to jim, and in this scope, and can be deleted from either place, which is a bit awkward to deal with.

        // Obviously to us it's clear that we should not delete it ourselves, but it can become hard to tell with more complicated code.
        // It's also possible that someone might accidentally put a return in this function before jim or jimothy are deleted, and neither would get deleted.
        // All of these bugs are bugs, and shouldn't happen, but it's usually a good idea to avoid potential bugs before they happen (especially ones so hard to track down later on).
    }
    std::cin.get();







    {
        // Now let's try an alternate solution using a smart pointer namely unique_ptr, from the standard library.

        class Person
        {
        public:
            std::string name;               // The variables here are the same, but
            std::unique_ptr<Person> parent; // We use a unique_ptr template class instead of a raw pointer.

            Person(std::string name) : name(name)
            {
                std::cout << name << " constructed" << std::endl;
            }
            ~Person()
            {                                                       // In the destructor, we don't have to call delete on our parent pointer, because it isn't a pointer.
                std::cout << name << " destructed" << std::endl;    // It's an object, which means, its destructor will get called after this objects destructor is called.
            }                                                       // This means that the unique_ptr can delete our pointer for us. Awesome!
        };                                                          // Also notice that this avoids the stupid if statement as well.

        // Let's create some unique pointers now.
        std::cout << "Creating objects with new as unique_ptr: " << std::endl;
        std::unique_ptr<Person> joe(new Person("Joe the third"));               // A unique pointer constructor takes in a pointer.
                                                                                // It will be deleted when unique_ptr goes out of scope.
                                                                                // Note: It would be simpler to just use a regular object in this case.
                                                                                // It would do the same thing, but this is for demo purposes.


        joe->parent = std::unique_ptr<Person>(new Person("Joe the second"));    // Here we create a new unique pointer and assign it as a parent.
                                                                                // Wait, what? joe isn't the actual object. How does it have ->parent?
                                                                                // Black Magic (operator overloading of ->)
                                                                                // Because we are forced to create the object as a unique_ptr,
                                                                                // We know that joe is responsible for joe2, and thus we can't delete joe2 on accident.
                                                                                // (we actually can as you'll see later, but it's very clearly intentional)

        joe->parent->parent = std::unique_ptr<Person>(new Person("Joe the first"));    // Finally we do another nested one.


        // Now, I know that I don't have to delete anything, because it's automatically handled for me when it goes out of scope.
        // You can also force it to be deleted with release(), although it may not work as expected:

        Person* p = joe.release();  // Release doesn't delete the pointer itself, it actually does one better.
                                    // Release removes control of the pointer from unique_ptr, and gives you back the raw pointer.

        delete p;                   // Now I can delete it myself.
        std::cin.get();




        // Other things that happen with unique pointers:
        std::unique_ptr<Person> who(new Person("who"));
        std::unique_ptr<Person> what(new Person("what"));

        who.swap(what); // Swap will swap the pointers of two unique pointers without changing the variables.
        std::cout << "who->name: " << who->name << std::endl;
        std::cout << "what->name: " << what->name << std::endl;
        std::cin.get();

        //      This is not allowed:
        // who = what;

        //      But this is allowed.
        who = std::unique_ptr<Person>(new Person("who"));   // This actually deletes the old one, and makes a new one.
        // Equivalently:
        delete what.release();
        what = std::unique_ptr<Person>(new Person("what"));

        std::cin.get();

    }
    std::cin.get();





    {
        // shared_ptr and weak_ptr

        // With the previous example, we have a clearcut strategy for handling a pointer that we know should belong to a single object.
        // What happens when our pointer needs to be shared between multiple objects?
        // a unique pointer would just delete the object when the first owner is deleted, and all the other owners would be left hanging.

        // The solution to this problem is reference counting.
        // We make our object keep track of how many things have pointers to it.
        // If we ever reach 0, we just delete it.
        // That's great, but then we have to remember to add and subtract all over the place, and I probably won't.
        // Instead, use a shared_ptr.

        // This person class is exactly the same as the previous, but with a shared_ptr instead.
        class Person
        {
        public:
            std::string name;
            std::shared_ptr<Person> parent; // shared_ptr

            Person(std::string name) : name(name)
            {
                std::cout << name << " constructed" << std::endl;
            }
            ~Person()
            {                                                       // The destructor doesn't even have to change!
                std::cout << name << " destructed" << std::endl;
            }
        };

        // And now for the test!
        // Create some people.
        Person* blossom = new Person("Blossom");    // These will be regular pointers so I can manage them myself.
        Person* bubbles = new Person("Bubbles");
        Person* buttercup = new Person("Buttercup");


        blossom->parent = std::shared_ptr<Person>(new Person("Professor"));
        bubbles->parent = blossom->parent;      // Unlike with unique_ptr, we can assign a shared_ptr to eachother.
        buttercup->parent = bubbles->parent;    // This increments the reference counter from each one that has it. (at this point it will be 3.)

        std::cout << "blossom->parent.use_count(): " << blossom->parent.use_count() << std::endl;
        // Now, me deleting blossom won't delete the professor, he will only be deleted when all 3 of them are also deleted.
        
        delete blossom;
        // Professor still exists at this point. (ref count 2)
        std::cout << "bubbles->parent.use_count(): " << bubbles->parent.use_count() << std::endl;


        delete bubbles;
        // Professor still exists now too. (ref count 1)
        std::cout << "buttercup->parent.use_count(): " << buttercup->parent.use_count() << std::endl;


        delete buttercup;
        // Professor is gone now
        // If we had kept a pointer to the professor here, we would have kept him from being deleted, because the counter would still be 1.

        // shared_ptr can also swap like unique_ptr.
        std::cin.get();



        // You may run into a problem like this though:
        Person* fred = new Person("fred");

        fred->parent = std::shared_ptr<Person>(fred);   // This will never be deleted, because it is counting itself!
                                                        // This can happen with any sized loop of self referencing objects.
                                                        // Once a loop like this happens, nothing that they reference will ever be cleaned up.
                                                        // (This problem can also happen with unique_ptr too)

        // Fixing this requires manual memory management, and then we are exactly back where we started:
        fred->parent.reset();

        // Solving this problem isn't always possible, but there exists weak_ptr to slightly fix the problem.

        // Here is an easy way to demo it:
        std::weak_ptr<Person> weakPtr;


        {
            std::shared_ptr<Person> fredzilla = std::shared_ptr<Person>(new Person("Fredzilla"));
            weakPtr = fredzilla;

            std::cout << "weakPtr.use_count(): " << weakPtr.use_count() << std::endl;   // We can access fredzilla using our weakpointer here, because it still exists.

            // weak pointers are slightly harder to use than a shared pointer, it requires you to convert to a shared pointer to read the data.
            // Like so:

            std::shared_ptr<Person> temp = weakPtr.lock();  // Lock gives a shared pointer if the pointer exists, otherwise the shared pointer will be null.
            if (temp != nullptr)
            {
                std::cout << "temp->name: " << temp->name << std::endl;
            }
        }

        std::cout << "weakPtr.use_count(): " << weakPtr.use_count() << std::endl;   // We can't access it here, because it's gone. There's no pointer.

        if (weakPtr.lock() == nullptr)
        {
            std::cout << "weakPtr.lock() == nullptr" << std::endl;  // The lock now gives a nullptr, meaning the object has been deleted already.
        }
    }
    std::cin.get();


    // Hopefully this example has been helpful to you in understanding the basics of smart pointers.
    // There are other kinds of smart pointers, and even these can be used in much more complicated ways, but these are the core three you should know.
    // You may also hear about auto_ptr online. Don't use it. It was the original, and has been replaced with these better more specific smart pointers.

    return 0;   // End Program.
}