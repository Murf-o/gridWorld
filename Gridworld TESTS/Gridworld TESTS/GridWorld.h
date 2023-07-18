
#ifndef _GRID_WORLD_H
#define _GRID_WORLD_H

#include <vector>
#include <iostream>
#include "List.h"

using std::vector;


class GridWorld {

  private:
    // private stuff goes here!
    //   typedefs
    
    //   data members
    int rowSize;
    int colSize;
    vector<vector<List*>> grid; 
    List* recycledIDs;  //IDS that were previously held
    int nextID; //next unused ID
    int pop;  //population

    vector<int> IDRow;  //these two vectors tell us the row and col of the district the ID is located in. Both vectors are the same size
    vector<int> IDCol;  //ID doesn't exist value is -1 -- "IDCol[200] = -1" ID 200 doesn't exist. 
    
    //   private helper functions

    //ID to return is put inside reference variable ID
    bool getRecycledID(int& ID){
      
      if(recycledIDs->isEmpty())
        return false;
      ID = recycledIDs->popFront();
      return true;
    }
    //   etc.

    //returns whether or not ID exists in grid
    bool validID(int id){

      //if id is out of bounds in the vectors, ID doesn't exist, return false
      if(id >= IDRow.size()){
        return false;
      }

      //if id doesn exist(Maybe dead), inside the vectors, return false
      if(IDRow[id] == -1){
        return false;
      }

      //return true otherwise
      return true;
    }

    //METHOD OVERLOAD
    //returns whether or not ID exists in grid
    //returns location of ID in reference parameters "row" and "col"
    bool validID(int id, int& row, int& col) const{

        //if id is out of bounds in the vectors, ID doesn't exist, return false
      if(id > IDRow.size()){
        return false;
      }

      //if id doesn exist(Maybe dead), inside the vectors, return false
      if(IDRow[id] == -1){
        return false;
      }

      //otherwise, update reference variables and return true 
      row = IDRow[id];
      col = IDCol[id];
      return true;
    }

    void resize(int id){
      int size = IDRow.size();
      int multiplier = (int)(id/size)+1;
      IDRow.resize(size*multiplier, -1);
      IDCol.resize(size*multiplier, -1);
    }


  public:
    /**
    * constructor:  initializes a "world" with nrows and
    *    ncols (nrows*ncols districtcs) in which all 
    *    districtricts are empty (a wasteland!).
    */
    GridWorld(unsigned nrows, unsigned ncols) :  grid(nrows, vector<List*>(ncols)), IDRow(nrows), IDCol(ncols) {
      // your constructor code here!
        for(int i = 0; i < nrows; i++){
          for(int j = 0; j < ncols; j++){
            grid[i][j] = new List;
          }
        }
        recycledIDs = new List;
        rowSize = nrows;
        colSize = ncols;
        nextID = 0;
        pop = 0;
        
    }

    ~GridWorld(){
      // your destructor code here.
      delete recycledIDs;
      for(int i = 0; i < rowSize; i++){
        for(int j = 0; j < colSize; j++){
          delete grid[i][j];
        }
      }
    }

    /*
     * function: birth
     * description:  if row/col is valid, a new person is created
     *   with an ID according to rules in handout.  New person is
     *   placed in district (row, col)
     *
     * return:  indicates success/failure
     */
    bool birth(int row, int col, int &id){
      if(row >= rowSize || col >= colSize)
        return false;
      
      
      //decide which ID
      int IDNum;
      //case 1 - recycledID exists, use that
      if(getRecycledID(IDNum)){
        id = IDNum;
        grid[row][col]->addNode(id);

        //update vectors containing district location of ID
        IDRow[id] = row;
        IDCol[id] = col;
      }
      else{ //case 2 - Assign smallest unused integer
        id = nextID;
        nextID++; //update nextID by 1
        grid[row][col]->addNode(id);

        //update vectors containing district location of ID
        if(id >= IDRow.size())
          resize(id);

        IDRow[id] = row;                                              //ERROR --- NEED TO RESIZE VECTORS IF ID IS TOO BIG
        IDCol[id] = col;
      }

      pop++;
      return true;
    }

    /*
     * function: death 
     * description:  if given person is alive, person is killed and
     *   data structures updated to reflect this change.
     *
     * return:  indicates success/failure
     */
    bool death(int personID){

        //if ID doesn't exist(maybe dead), return false
      if(!validID(personID))
        return false;

      //get district location of personID
      int row = IDRow[personID];
      int col = IDCol[personID];


      //kill ID in that district
      grid[row][col]->killID(personID);

      //update the two location vectors to show that ID is not held -- value is -1
      IDRow[personID] = -1;
      IDCol[personID] = -1;

      //update recycledIDs list
      pop--;
      recycledIDs->addNode(personID);

      return true;
    }

    /*
     * function: whereis
     * description:  if given person is alive, his/her current residence
     *   is reported via reference parameters row and col.
     *
     * return:  indicates success/failure
     */
    bool whereis(int id, int &row, int &col)const{

      //check if validID, if it is, function will update variables "row" and "col" with location of ID
      if(validID(id, row, col))
        return true;
      
      return false;
    }

    /*
     * function: move
     * description:  if given person is alive, and specified target-row
     *   and column are valid, person is moved to specified district and
     *   data structures updated accordingly.
     *
     * return:  indicates success/failure
     *
     * comment/note:  the specified person becomes the 'newest' member
     *   of target district (least seniority) --  see requirements of members().
     */
    bool move(int id, int targetRow, int targetCol){
      int row, col;

      //if is not valid ID, return false, otherwise, put location values into row and col
      if(!validID(id, row, col))
        return false;
      
      //if district doesn't exist in grid, return false
      if(targetCol >= colSize || targetRow >= rowSize)
        return false;

      //remove ID from original district, and add them into the target District -- KEEP THE SAME ID
      grid[row][col]->killID(id);
      grid[targetRow][targetCol]->addNode(id);
      IDRow[id] = targetRow;
      IDCol[id] = targetCol;

      return true;
    }

    std::vector<int> * members(int row, int col)const{

      if(row >= rowSize || col >= colSize || grid[row][col] == nullptr)
        return new vector<int>;
      
      int size = grid[row][col]->getPopulation();

      vector<int>* memb = new vector<int>(size);
      
      grid[row][col]->memberList(*memb);

      return memb;
    }

    /*
     * function: population
     * description:  returns the current (living) population of the world.
     */
    int population()const{
      return pop;
    }
    
    /*
     * function: population(int,int)
     * description:  returns the current (living) population of specified
     *   district.  If district does not exist, zero is returned
     */
    int population(int row, int col)const{
      
      //if paramters out of bounds, return 0
      if(row >= rowSize || col >= colSize)
        return 0;

      return grid[row][col]->getPopulation();
    }

    /*
     * function: num_rows
     * description:  returns number of rows in world
     */
    int num_rows()const {
      return rowSize;
    }

    /*
     * function: num_cols
     * description:  returns number of columns in world
     */
    int num_cols()const {
      return colSize;
    }



};

#endif
