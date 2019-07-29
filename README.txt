Documentation Author: Niko Procopi 2019

This tutorial was designed for Visual Studio 2017 / 2019
If the solution does not compile, retarget the solution
to a different version of the Windows SDK. If you do not
have any version of the Windows SDK, it can be installed
from the Visual Studio Installer Tool

Welcome to the Smart Pointers Tutorial!
Prerequesites: Pointers

Smart pointers will automatically delete the
allocated data that they point to, as soon as
the pointer itself is erased (from the stack or heap, doesn't matter),
which is guarranteed to eventually happen when 
"int main()" reaches the end. Smart pointers can prevent memory leaks,
there is really no disadvantage to using them, its just a matter of 
preference
