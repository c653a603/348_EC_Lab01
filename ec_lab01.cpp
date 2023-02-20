/*
Programmer: Connor Anderson
Date Modified: 2/20/2023
Description/steps of the algorithm:
  1. retrieve the first choices of each department
  2. check for duplicates of the first valid choice
  3. consult programmer's preferences if there are duplicates
    - highest ranked department looking to hire is chosen
  4. remove the department that got the programmer
    - mark choices as '0'
  5. update remaining departments (remove the programmer last assigned)
    - update lists to not include the programmer that was just hired
*/

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// used to read a line according to the form of the input file
void readLine(const string &line_data, char* store_data, int start) {
  for (int index = 0; index < 9; index += 2) {
    store_data[start + (index/2)] = line_data[index];
  }
}

// used to mark a department once it has hired a programmer
void remove_dept(char* dept_choices){
  for(int i = 0; i < 5; i++){
    dept_choices[i] = '0';
  }
}

// used to remove a programmer from a department's lists once they have been hired
void remove_prgmr(int prgmr_index, char* dept_choices){
  prgmr_index += 1;
  char prgmr = prgmr_index + '0';
  for(int i = 0; i < 5; i++){
    if(dept_choices[i] == prgmr && i == 4){
      dept_choices[i] = '0';
    } else if(dept_choices[i] == prgmr) {
      for(int j = i; j < 4; j++){
      dept_choices[j] = move(dept_choices[j+1]);
      }
      dept_choices[4] = '0';
    }
  }
}

int main() {

  // array to store the final decsions
  int decisions[5];

  // process to open the input file
  string fileName;
  cout << "Enter the name of the input file: ";
  cin >> fileName;

  ifstream myfile;
  myfile.open(fileName);
  string line;

  // arrays to store the input data
  char deptArray[25];
  char prgmrArray[25];
  // read the file
  int lineNum = 0;
  while (getline(myfile, line)) {
    if (lineNum < 5)
      readLine(line, deptArray, lineNum*5);
    else
      readLine(line, prgmrArray, (lineNum-5)*5);

    lineNum++;
  }
  // close the file
  myfile.close();

  
  //convert to 2d arrays
  char depts[5][5];
  char prgmrs[5][5];
  for (int index = 0; index < 25; index++) {
    
    depts[index%5][index/5] = deptArray[index];
    // [department][the programmer at that rank]
    
    prgmrs[index%5][index/5] = prgmrArray[index];
    // [programmer][the departmentment at that rank]
  }


  // begin the algorithm to find a department for each of the 5 programmers
  // iterate this 5 times -- once to assign each programmer
  for (int i = 0; i < 5; i++){
    
    // retrieve the first choices of each department
    int first_choices[5];
    for(int temp = 0; temp < 5; temp++)
    {
      first_choices[temp] = depts[temp][0] - '0';
    }

    // get the department(s) that want to hire the selected programmer (the first, valid first choice)
    int hiring_depts[5];
    int num_of_depts = 0;
    for(int j = 0; j < 5; j++)
    {
      if(first_choices[j] != 0)
      {
        // add the index of the first department with a vaild first choice
        hiring_depts[num_of_depts] = j;
        num_of_depts++;
        // add the other departments looking at the same programmer
        for(int k = j+1; k < 5; k++)
        {
          if(first_choices[j] == first_choices[k])
          {
            hiring_depts[num_of_depts] = k;
            num_of_depts++;
          }
        }
        break;
      }
    }
    
    // now hiring_depts should have all of the departments that want the same programmer
    
    // first check if there is only one departmentment that wants said programmer
    if(num_of_depts == 1){
      
      //cout << "Department #" << hiring_depts[0]+1 << " gets Programmer #" << first_choices[hiring_depts[0]] << endl;
      decisions[hiring_depts[0]] = first_choices[hiring_depts[0]];
      
      // remove the dept that hired the programmer
      remove_dept(depts[hiring_depts[0]]);
      // remove the programmer from the other departments lists
      for(int department = 0; department < 5; department++){
        remove_prgmr(first_choices[hiring_depts[0]]-1, depts[department]);
      }
    } 
    // if there are multiple departments trying to hire the programmer
    else {
      // consult the programmers preferences
      int chosen_dept_index = 0;
      int chosen_prgmr_index = (first_choices[hiring_depts[0]]) - 1;
      // iterates through the programmers preferences
      for(int rank = 0; rank < 5; rank++){
        int found_match = 0;
        // iterates through the departments looking to hire the programmer
        for(int dept = 0; dept < num_of_depts; dept++){
          if(prgmrs[chosen_prgmr_index][rank] == (hiring_depts[dept]+1) + '0'){
            //cout << "Department #" << (hiring_depts[dept]+1) << " gets Programmer #" << (first_choices[hiring_depts[0]]) << endl;
            decisions[hiring_depts[dept]] = first_choices[hiring_depts[0]];
            chosen_dept_index = hiring_depts[dept];
            found_match = 1;
          }
        }
        // once the programmer matched with its highest ranked option -- break
        if (found_match == 1)
          break;
        
      }
      // remove the department
      remove_dept(depts[chosen_dept_index]); 
      // remove the programmer
      for(int department = 0; department < 5; department++){
        remove_prgmr(chosen_prgmr_index, depts[department]);
      }
    }
    
  } 

  for(int dept = 0; dept < 5; dept++){
    cout << "Department #" << dept+1 << " gets Programmer #" << decisions[dept] << endl;
  }
  
  return 0;
}