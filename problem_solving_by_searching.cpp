#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include <queue>

using namespace std;

//object class that stores state number, its parent, action number to get here from parent, and cost.
class Obj {
    int state_number;
    Obj* parent;
    int action;
    int cost;
    public:
    //default constructor initializing data with defaul values...
        Obj() {
            this->state_number = -1;
            this->parent = nullptr;
            this->action = -1;
            this->cost = -1;
        }
        //parameteried constructor initializing members with given values...
        Obj(int state_number, Obj* parent, int action, int cost) {
            this->state_number = state_number;
            this->parent = parent;
            this->action = action;
            this->cost = cost;
        }

        //setters for all the data members.
        void setStateNumber(int state_number) {
            this->state_number = state_number;
        }
        void setParent(Obj* Parent) {
            this->parent = Parent;
        }
        void setAction(int action) {
            this->action = action;
        }
        void setCost(int cost) {
            this->cost = cost;
        }

        //getters of all the data members.
        int getStateNumber() const {
            return state_number;
        }
        Obj* getParent() const {
            return parent;
        }
        int getAction() const {
            return action;
        }
        int getCost() const {
            return cost;
        }
};


//Taking global variables to store data from file.
//using pointer arrays for dynamic allocation....
int m, n, t;
string *states;
string *actions;
int **transitions_table;
string **test_cases;

//this function will read data from file and store it in their respective variables and arrays.
void populate()
{
    //ifstream -> input from file.
    ifstream data_from_file;

    //opening file that contains data.
    data_from_file.open("sample_input_file.txt");

    //if file fails to open!! Show ERROR
    if (data_from_file.fail())
    {
        cout << "[+] No such file found!!!" << endl;
        exit(0);
    }
    else
    {

        string temp;
        
        //reading m,n,t
        data_from_file >> m;
        data_from_file >> n;
        data_from_file >> t;

        //skip empty line
        getline(data_from_file, temp);

        states = new string[m];
        for (int i = 0; i < m; i++)
        {
            getline(data_from_file, states[i]);
            //if it is an empty line then still skip.
            if(states[i].size() == 0) i--;
        }

        actions = new string[n];
        for (int i = 0; i < n; i++)
        {
            getline(data_from_file,actions[i]);

            if(actions[i].size() == 0) i--;
        }

        transitions_table = new int *[m];
        for (int i = 0; i < m; i++)
        {
            transitions_table[i] = new int[n];
            
            data_from_file >> transitions_table[i][0];
            data_from_file >> transitions_table[i][1];
            data_from_file >> transitions_table[i][2];
        }

        //store data in test cases....
        test_cases = new string*[t];

        for(int i = 0; i < t; i++) {
            test_cases[i] = new string[2];

            getline(data_from_file, temp);

            if(temp.size() == 0) {
                i--;
                continue;
            }

            int starti, endi;
            for(int i = 0; i < temp.size()-3; i++) {
                if(temp[i] == ' ' && temp[i+1] == ' ' && temp[i+2] == ' ' && temp[i+3] == ' '){
                    starti = i;
                    endi = i+3;
                    break;
                }
            }

            test_cases[i][0] = temp.substr(0, starti);
            test_cases[i][1] = temp.substr(endi+1, temp.size());
        }
        //closing the file...
        data_from_file.close();
    }
}

//deleting all the data that we store in pointers array.
void deleteData() {
    delete[] states;
    delete[] actions;

    for(int i = 0; i < m; i ++) {
        delete[] transitions_table[i];
    }
    delete[] transitions_table;

    for(int i = 0; i < t; i++) {
        delete[] test_cases[i];
    }
    delete[] test_cases;
}

//useless function to check if data is stored properly in files or not!
void displayFile() {
    cout << m << " " << n << " " << t << endl << endl;

    for(int i = 0; i < m; i++) {
        cout << states[i] << endl;
    }
    cout << endl;
    for(int i = 0; i < n; i++) {
        cout << actions[i] << endl;
    }
    cout << endl;
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            cout << transitions_table[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    for(int i = 0; i < t; i++) {
        cout << test_cases[i][0] << " " << test_cases[i][1] << endl;
    }
}

//function takes a string and returns its index from the total states combintaion
int stateNumber(string checkState) {
    for(int i = 0; i < m; i++) {
        if(states[i] == checkState) return i;
    }
    return -1;
}

//function that takes stateNumber and return if it is already processed or not!
//helps in adding new Obj to queue
bool alreadyProcessed(int* processedArray,int state_number) {
    for(int i = 0; i < m; i++) {
        if (processedArray[i] == state_number) return true;
    }
    return false;
}


int main()
{
    //populate data from file to states,actions and transitions...
    populate();

    //display data to check proper input.
    //displayFile();

    //for total test_cases, display results...
    for(int total_test_cases = 0; total_test_cases < t; total_test_cases++) {
        //queue to store Obj* 
        queue<Obj*> myque;

        //initializing array that store states and use to check if state is already checked or not!!!
        int* processed = new int[m];
        int p_index = 0;
        for(int i = 0; i < m; i++) processed[i] = -1;

        Obj* toDelete[m];

        //Stack that will store actions performed on states
        stack<int> actions_stack;

        //store the initial state and final state index according to combination in file.
        int initial_state_number = stateNumber(test_cases[total_test_cases][0]);
        int final_state_number = stateNumber(test_cases[total_test_cases][1]);

        //initializing initial state Obj....
        Obj* initial_state = new Obj(initial_state_number, nullptr, -1, 0);

        // initial_state->state_number = initial_state_number;
        // initial_state->parent = nullptr;
        // initial_state->action = -1;
        // initial_state->cost = 0;

        //inserting Obj0 in queue
        myque.push(initial_state);

        //bool checks if final state has been reached or not.
        bool found = false;


        while(!myque.empty()) {
            //get the Obj* from the queue and process it
            Obj* newState = myque.front();
            myque.pop();

            int current_state_number = newState->getStateNumber();

            //apply all the actions on current state and store all the new states Obj in the queue.
            for(int i = 0; i < n; i++){
                int new_state_after_action = transitions_table[current_state_number][i];

                //check if new state is final state or not.
                if(new_state_after_action == final_state_number) {
                    //cout << "GOAL HAS BEEN REACHED!!!" << endl;

                    //now our objective is to go back until parent is null.
                    //storing actions in stacks
                    Obj* parent = newState;
                    //this state action is current (i).
                    actions_stack.push(i);

                    //while parent is not null, go to its parent and store its action.
                    while(parent != nullptr) {
                        if(parent->getAction() != -1)
                            actions_stack.push(parent->getAction());

                        parent = parent->getParent();
                        //break;
                    }

                    //as solution is found. break the loop.

                    delete newState;

                    found = true;
                    break;
                }

                //if the new state is not the current state and is not even processed. then add it in the queue.
                if(new_state_after_action != current_state_number && !alreadyProcessed(processed, new_state_after_action)) {
                    Obj* newObj = new Obj(new_state_after_action, newState, i, newState->getCost() + 1);
                    // newObj->state_number = new_state_after_action;
                    // newObj->action = i;
                    // newObj->parent = newState;
                    // newObj->cost = newState->cost+1;

                    myque.push(newObj);
                }
            }

            if(found) break;

            //add every current state to processed after every loop
            processed[p_index] = current_state_number;
            toDelete[p_index] = newState;
            p_index++;
        }

        //if solution does not found then display!!!
        if(!found) {
            cout << "[+]SOLUTION DOES NOT EXIST!!!" << endl;
        }
        else {
            //fetch the action number, take the string from actions and display the string
            while(!actions_stack.empty()) {
                int act = actions_stack.top();
                actions_stack.pop();
                cout << actions[act] << "->";
            }
            cout << "\b\b  ";
            cout << endl;
        }

        delete [] processed;
        processed = nullptr;
        
        for(int i = 0; i < p_index; i++) {
            Obj* temp = toDelete[i];

            delete temp;
        }

        delete initial_state;
    }
    
    deleteData();

    return 0;
}

// <3 SHUKAR ALLAH. ALHAMDULLILAH <3