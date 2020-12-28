#include "tarlib.h"
#include "microtar.h"

#include <iostream>
#include <string.h>
#include <sstream>
#include <stdlib.h>

using namespace std;

int main() {
   Archive tar("test.tar","r");  //Создать архив для чтения
   Archive tar1("example.tar","w"); //Создать архив для записи
   //Добавить файлы в архив  
   tar1.addElement("dir/abc/ewgwe/hewh/rej/file.txt");
   tar1.addElement("dir/abc/ewgwe/file.txt");
   //Создать потоки вывода, связанные с файлами
   FIStream in1(dynamic_cast<File*>(tar.findElement("dir/abc/ewgwe/file.txt")));
   FIStream in2(dynamic_cast<File*>(tar.findElement("dir/abc/ewgwe/hewh/rej/file.txt")));
   FOStream out1(dynamic_cast<File*>(tar1.findElement("dir/abc/ewgwe/file.txt"))); 
   FOStream out2(dynamic_cast<File*>(tar1.findElement("dir/abc/ewgwe/hewh/rej/file.txt")));
   //Чтение
   string str1,str2;
   in1 >> str1;
   in2 >> str2;
   cout<<"str1: "<<str1<<"\n";
   cout<<"str2: "<<str2<<"\n";

   //Запись
   out1 << "Example 1";
   out2 << "Example 2";
   
   return 0;
}