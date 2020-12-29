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

class mtar_bad_mode : public std::exception{
public:
   const char * what()const throw(){return "Wrong mode !";}
};

class mtar_not_found : public std::exception{

public:
   const char* what()const throw(){return "Not found!";}
};

class Archive;
class Element;
class Directory;
class File;

//Класс архив
class Archive {
   mtar_t tar;
   Directory *root;
   std::string mode;                                       

   void removeFirstDirInPath(std::string& name);               //Удалить первое имя из пути
   std::string getCurNameInPath(std::string name)const;        //Получить первое имя в пути
   void dfsAdd(std::string curName,Directory* curRoot);        //Обход в глубину
   void dfsFind(std::string name,Element* curElement,Element*&); //Поиск в глубину 
   void dfsDelete(Element* curElement);

public:
   Archive(std::string fileName,std::string _mode);
   virtual ~Archive(); 
   Directory *getRoot() const {return root;}
   void addElement(std::string fullName);                //Добавить элемент в архив              
   void removeElement(std::string name);                 //Удалить элемент из архива
   Element* findElement(std::string fullPath);           //Поиск элемента в архиве
   mtar_t* getTar(){return &tar;}
   std::string getMode()const{return mode;}
};

//Базовый класс для папки и файла
class Element{
protected:
   std::string name;
   Archive* archive;    //Архив, в котором лежит элемент
   Directory *parent;   //Родительская папка

public:
   Element(Archive* _archive,
           std::string name="",Directory*parent=nullptr):
           archive(_archive),name(name),parent(parent){}
   virtual ~Element(){}
   Directory* getParent()const{return parent;} 
   std::string getName() const {return name;}         
   std::string getFullName()const;
   Archive* getArchive()const{return archive;}
   virtual void remove(Element*){}                 
};

//Класс папка
class Directory : public Element {
   std::vector<Element*> children;   

public:
   Directory(Archive* _archive,
             std::string name="",Directory*parent = nullptr):
             Element(_archive,name,parent),children(0){}
   Element* getChild(int i)const{return children[i];}
   ~Directory();
   void addElement(Element* cur){children.push_back(cur);}
   void remove(Element* c){children.erase(find(children.begin(),children.end(),c));}
   Element* findElement(std::string name);
   size_t getSize()const{return children.size();}
   bool isEmpty(){return children.size() == 0;}
};

//Класс файл
class File : public Element {

public:
   File(Archive* _archive,std::string name,Directory* parent):
        Element(_archive,name,parent){}
   ~File(){}  
};
//Классы для потоков
class FIStream : public std::istringstream {
   File *file;
public:
   FIStream(File *file);
};

class FOStream : public std::ostringstream {
   File *file;
public:
   FOStream(File *file);
   ~FOStream();
};

#endif