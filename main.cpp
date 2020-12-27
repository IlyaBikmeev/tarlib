#include "tarlib.h"
#include "microtar.h"

#include <iostream>
#include <string.h>
#include <sstream>
#include <stdlib.h>

using namespace std;

int main() {
   Archive tar("test.tar","w");

   tar.addElement("dir/abc/ewgwe/hewh/rej/file.txt");
   tar.addElement("dir/abc/ewgwe/file.txt");
   FOStream out1(dynamic_cast<File*>(tar.findElement("dir/abc/ewgwe/file.txt")));
   FOStream out2(dynamic_cast<File*>(tar.findElement("dir/abc/ewgwe/hewh/rej/file.txt")));
   out1 << "hello!";
   out2 << "Good evening!";
   
   return 0;
}