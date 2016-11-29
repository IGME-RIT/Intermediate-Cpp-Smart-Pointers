/*
Class Templates
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

#pragma once

// Creating a class template is simple-ish
template<class TypeA, class TypeB>  // Start by declaring the template like this. We have a class here that has two templated types.
class Pair                          // This example is a simple pair class that stores two objects together.
{

public:                             // The rest is simple, we define the class here using the types above.
                                    // Note that we define the constructor here:
                                    // This is the most important part of class templates: EVERYTHING MUST BE DEFINED IN THE HEADER!!!
                                    // class templates don't have cpp files, because the code they have isn't "code" it's a template for code.
    Pair(TypeA a, TypeB b) : first(a), second(b) {}

    TypeA first;
    TypeB second;
};


// Here I overload the << operator for our ostream object so that we can print out our pair:
template<class TypeA, class TypeB>
std::ostream& operator<<(std::ostream& output, const Pair<TypeA, TypeB>& pair)
{
    return output << pair.first << ", " << pair.second;
}