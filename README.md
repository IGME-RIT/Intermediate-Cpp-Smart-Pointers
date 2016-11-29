# Intermediate C++: Smart Pointers

Management of dynamically allocated memory (new/delete) is a common problem in C++.
Programmers frequently run into situations where a dynamically allocated object exists in multiple places, or doesn't get deleted when it should.
Many programmers have solved this problem, and prefer to do it in different ways, none of which are perfect. Smart pointers are one of these ways.
This example shows some basic situations where you may want to use smart pointers, and gives examples of unique_ptr, shared_ptr, and weak_ptr.

# Setup

You will need to have CMake installed on your computer, and properly added to your path.
In order to setup, run the following in a shell, then open the project in your preferred editor.
Windows setup has been configured for use with Visual Studio.

Windows:
```
cd path/to/folder
setup.cmd
```
Linux:
```
cd path/to/folder
./setup
```