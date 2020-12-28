#include "tarlib.h"

Archive::Archive(std::string fileName,std::string _mode){
   mode = _mode;
   mtar_open(&tar,fileName.c_str(),mode.c_str());
   root = new Directory(this);
   mtar_header_t h;
   if(mode == "r"){
      while ((mtar_read_header(&tar, &h)) != MTAR_ENULLRECORD) {
         addElement(h.name);
         mtar_next(&tar);
      }
   }
}

Archive::~Archive() {
   mtar_finalize(&tar);
   mtar_close(&tar);
   delete root;
}

void Archive:: dfsDelete(Element* curElement){
   if(dynamic_cast<File*>(curElement)){
      delete curElement;
      return;
   }
   else{
      Directory* d = dynamic_cast<Directory*>(curElement);
      for(int i = 0; i < d->getSize(); ++i){
         dfsDelete(d->getChild(i));
      }
   }
   delete curElement;
}

void Archive:: removeElement(std::string name){
   Element* curElement = findElement(name);
   dfsDelete(curElement);
}

void Archive::addElement(std::string fullName) {
   dfsAdd(fullName,root);
}

Element* Archive::findElement(std::string fullPath){
   Element* result;
   for(int i = 0; i < root->getSize(); i++){
      dfsFind(fullPath,root->getChild(i),result);
   }
   return result;
}

void Archive::dfsAdd(std::string curPath,Directory *curDir) {
   if (curPath == "")
      return;
   std::string curName = getCurNameInPath(curPath);
   Element *curElement = curDir->findElement(curName);
   //В текущей папке нет данного элемента
   if (!curElement) {
      //Текущий элемент является файлом
      if (curName.find('/') == std::string::npos) {
         Element* file = new File(this,curName,curDir);
         curDir->addElement(file);
      }
      //Текущий элемент является папкой
      else {
         Element * dir = new Directory(this,curName,curDir);
         curDir->addElement(dir);
         removeFirstDirInPath(curPath);
         dfsAdd(curPath,dynamic_cast<Directory*>(dir));
      }
   }
   //Если текущий элемент найден в текущей директории
   else {
      //Если данный элемент - файл, то добавляем его в текущую папку и выходим из рекурсии
      if (curName.find('/') == std::string::npos){
         Element* file = new File(this,curName,curDir);
         curDir->addElement(file);
         return;
      }
      //Иначе просто идем дальше, делая текущей папкой найденный элемент
      else{
         removeFirstDirInPath(curPath);
         dfsAdd(curPath,dynamic_cast<Directory *>(curElement));
      }
   }
}
                             
void Archive::dfsFind(std::string curPath,Element* curElement,Element*& result){
   std::string curName = getCurNameInPath(curPath);
   if(curName == "" || 
      curName != curElement->getName()){
      return;
   }
   
   else{
      removeFirstDirInPath(curPath);
      Directory* dir = dynamic_cast<Directory*>(curElement);
      if(dir){
         for(int i = 0; i < dir->getSize(); i++){
            dfsFind(curPath,dir->getChild(i),result);
         }
      }
      else{
         result = curElement;
         return;
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

std::string Element:: getFullName()const{
   Directory* ptr = parent;
   std::string result = "";
   while(ptr){
      result.insert(0,ptr->getName());
      ptr = ptr->getParent();
   }
   result += getName();
   return result;
}

Directory::~Directory(){
   for(int i = 0; i < children.size(); ++i)
      delete children[i];
}

FIStream::FIStream(File* file):file(file){
   if(file->getArchive()->getMode() != "r")
      throw mtar_bad_mode();
   mtar_t* tar = file->getArchive()->getTar();
   mtar_header_t h;
   char* p;
   mtar_find(tar, file->getFullName().c_str(), &h);
   p = (char*)calloc(1, h.size + 1);
   mtar_read_data(tar, p, h.size);
   str(p);
   free(p);
}

FOStream::FOStream(File *file):file(file){
   if(file->getArchive()->getMode() != "w")
      throw mtar_bad_mode();
}

FOStream:: ~FOStream(){
   mtar_t* tar = file->getArchive()->getTar();
   mtar_write_file_header(tar, file->getFullName().c_str(),this->str().size());
   mtar_write_data(tar, this->str().c_str(), this->str().size());
}
