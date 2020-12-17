#include "tarlib.h"

Archive::Archive(std::string fileName,const char *mode) {
   mtar_open(&tar,fileName.c_str(),mode);
   root = new Directory;
   mtar_header_t h;
   while ((mtar_read_header(&tar, &h)) != MTAR_ENULLRECORD) {
      dfs(h.name,root);
      mtar_next(&tar);
   }
}

Archive::~Archive() {
   mtar_close(&tar);
   /*Нужно еще удалить все элементы из дерева */
}

void Archive::add(std::string name) {

}

void Archive::dfs(std::string curPath,Directory *curDir) {
   if (curPath == "")
      return;

   std::string curName = getCurNameInPath(curPath);
   Element *curElement = curDir->findElement(curName);

   std::cout<<curDir->getName()<<" "<<curName<<"\n";

   //В текущей папке нет данного элемента
   if (!curElement) {
      //Текущий элемент является файлом
      if (curName.find('/') == std::string::npos) {
         Element* file = new File(curName,curDir);
         curDir->addElement(file);
      }
      //Текущий элемент является папкой
      else {
         Element * dir = new Directory(curName,curDir);
         curDir->addElement(dir);
         removeFirstDirInPath(curPath);
         dfs(curPath,dynamic_cast<Directory*>(dir));
      }
   }
   //Если текущий элемент найден в текущей директории
   else {
      //Если данный элемент - файл, то добавляем его в текущую папку и выходим из рекурсии
      if (curName.find('/') == std::string::npos){
         Element* file = new File(curName,curDir);
         curDir->addElement(file);
         return;
      }
      //Иначе просто идем дальше, делая текущей папкой найденный элемент
      else{
         removeFirstDirInPath(curPath);
         dfs(curPath,dynamic_cast<Directory *>(curElement));
      }
   }
}

void Archive::removeFirstDirInPath(std::string &name) {
   size_t pos = name.find('/');
   if(pos != std::string:: npos)
      name.erase(name.begin(),name.begin()+pos+1);
   else
      name = "";
}

std::string Archive::getCurNameInPath(std::string name)const {
   std::string res="";
   for (int i = 0; i < name.size(); i++) {
      if (name[i] == '/'){
         res += '/';
         break;
      }
      res += name[i];
   }
   return res;
}

Element *Directory:: findElement(std::string name) {
   //Если элемент является папкой и совпадают имена - папка найдена
   for (int i = 0; i < children.size(); i++) {
      if (children[i]->getName() == name)
         return children[i];
   }
   return nullptr;
}

bool Archive::isEmpty() {
   return root->isEmpty();
}

