#include <vector>
#pragma once
using namespace::std;

class List{

  private: 
  struct NODE{
    int ID;
    NODE* prev;
    NODE* next;
  };
  vector<NODE*> vecList;
  NODE* head;
  NODE* tail;
  int population;

  //removes specific node from list, 4 cases total
  bool removeNODE(NODE* cur){
    
    //case 0 - cur is nullptr
    if(cur == nullptr)
      return false;

    //case 0.5 -  singleton
    else if(population == 1){
      head = nullptr;
      tail == nullptr;
      delete cur;
    }

    //case 1 - cur is the head of the list
    else if(head == cur){
      head = head->next;
      head->prev = nullptr;
      delete cur;
    }
    //case 2 - cur is in the tail of the list
    else if(tail == cur){
      tail = tail->prev;
      tail->next = nullptr;
      delete cur;
    }
    //case 3 - cur is in the middle of the list
    else{
      NODE* prev = cur->prev;
      NODE* nxt = cur->next;
      prev->next = nxt;
      nxt->prev = prev;
      delete cur;
    } 
    population--;
    return true;
  }

  //resize vecList to proper size
  void resize(int id){
      int size = vecList.size();
      int multiplier =  (int)(id/size)+1;
      vecList.resize(size*multiplier);
    }


  public:

  //Constructor
  List() : vecList(300){
    head = nullptr;
    tail = nullptr;
    for(int i = 0; i < 300; i++){
      vecList[i] = nullptr;
    }
    population = 0;
  }

  //Destructor
  ~List(){
    NODE* cur = head;
    NODE* nxt;
    while(cur != nullptr){
      nxt = cur->next;
      delete cur;
      cur = nxt;
    }
  }

  //returns true if list is empty, false otherwise
  bool isEmpty(){
    if(head == nullptr){
      return true;
    }
    return false;
  }

  //adds node to back of the list
  void addNode(int id){
  
    NODE* person = new NODE;
    person->ID = id;
    population++;
    if(id >= vecList.size())
      resize(id);

    vecList[id] = person;  //add node to vector 

    if(isEmpty()){
        //connect to head and tail to it
       person->next = nullptr;
       person->prev = nullptr;
       head = person;
       tail = person;
       return;
    }
    
    //add node to back of the list
    person->next = nullptr;
    person->prev = tail;
    tail->next = person;
    tail = person;
  }

  //removes first node in list -- head, if empty -- return -1
  int popFront(){
    if(isEmpty()){
      return -1;
    }

    NODE* cur = head;
    int ret;
    vecList[cur->ID] = nullptr;

    if(population == 1){  //SINGLETON
      population--;
      tail == nullptr;
      head = nullptr;
      ret = cur->ID;
      delete cur;
      return ret;
    }

    population--;
    head = head->next;  //update head
    head->prev = nullptr; //disconnect prev node
   
    ret = cur->ID;
    delete cur;
    return ret;
  }

  //return number of nodes in list
  int getPopulation(){
    return population;
  }

  //removes specific node with "ID" 'id', using vecList
  bool killID(int id){

    //if not in LIST, return false
    if(id >= vecList.size() || vecList[id] == nullptr)
      return false; 

    //otherwise, remove cur from list and set vector value to nullptr -- return true;
    NODE* cur = vecList[id];
    removeNODE(cur);
    vecList[id] = nullptr;
    return true;
  }

  //put IDS from the list into a vector, from descending order -- head to tail, vector already initialized with length
  void memberList(vector<int>& v){
    NODE* cur = head;
    
    for(int i = 0; i < population; i++){
      v[i] = cur->ID;
      cur = cur->next;
    }
  }

  


};
