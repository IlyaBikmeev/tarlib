#ifndef _TARLIB_H_
#define _TARLIB_H_

#include "microtar.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

/*
В данной работе в классах element,directory,file будем хранить имена элементов,
а в классе archive будем использовать структуры и функции из файла "microtar.h"
для доступа к тому или иному элементу по имени.
*/

class Archive;
class Element;
class Directory;
class File;

//Класс архив
class Archive {
   mtar_t tar;
   Directory *root;                                        

   void removeFirstDirInPath(std::string& name);         //Удалить первое имя из пути
   std::string getCurNameInPath(std::string name)const;  //Получить первое имя в пути
   void add(std::string name);                           //Добавление элемента в архив
   void dfs(std::string curName,Directory* curRoot);     //Обход в глубину

public:
   Archive(std::string fileName = "",const char* mode ="");
   ~Archive(); 
   Directory *getRoot() const {return root;}                
   void remove(std::string name);
   bool isEmpty();
};

//Базовый класс для папки и файла
class Element{
protected:
   std::string name;
   Directory *parent;   //Родительская папка

public:
   Element(std::string name="",Directory*parent=nullptr):name(name),parent(parent){}   
   Directory* getParent()const{return parent;} 
   std::string getName() const {return name;}         
   std::string getFullName()const;
   virtual void remove(Element*){}                 
};

//Класс папка
class Directory : public Element {
   std::vector<Element*> children;   

public:
   Directory(std::string name="",Directory*parent = nullptr):Element(name,parent){}
   Element* getChild(int i)const{return children[i];}
   void addElement(Element* cur){children.push_back(cur);}
   void remove(Element* c){children.erase(find(children.begin(),children.end(),c));}
   Element* findElement(std::string name);
   bool isEmpty(){return children.size() == 0;}
};

//Класс файл
class File : public Element {

public:
   File(std::string name,Directory* parent):Element(name,parent){}
   ~File();  
};

//Классы для потоков

class FIStream : public std::istringstream {
   File *file;
public:
   FIStream(File *file = nullptr);
   ~FIStream();
};

class FOStream : public std::ostringstream {
   File *file;
public:
   FOStream(File *file);
   ~FOStream();
};


#endif