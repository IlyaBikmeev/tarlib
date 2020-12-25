#include "tarlib.h"
#include "microtar.h"

#include <iostream>
#include <string.h>
#include <sstream>
#include <stdlib.h>

using namespace std;


int main() {
   Archive tar("example.tar","r");
   //tar.addElement("dir/abc/file2.txt");
   //tar.addElement("dir/abc/qwerty/file3.txt");

   File* f1 = dynamic_cast<File*>(tar.findElement("dir/abc/file2.txt"));
   File* f2 = dynamic_cast<File*>(tar.findElement("dir/abc/qwerty/file3.txt"));

   FIStream in(f1);
   string str;
   in >> str;
   cout<<str<<"\n";

   return 0;
}