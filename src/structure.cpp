#include <algorithm>
#include "structure.h"

using namespace std;


FA::FA(std::string FAname) {
    _name = FAname;
}

FA::FA() {
}

FA::FA(vector<State*> &states, vector<char> &alphabet) {
    _states = states;
    _alphabet = alphabet;
    _name = "Finite Automata (from file)";
}

FA::~FA() {
    for (State* st: _states) {
        for (Transition* t: st->exits) {
            delete (t);
        }
        delete (st);
    }
}

void FA::display() const {
    int a_size = _alphabet.size(), st_size = _states.size(), t_size, i, c, j;

    State* curSt;

    cout << endl << " - - - - - - - - - - - - - - - - - - - - - " << endl;
    cout << "          FA: " << _name << endl << endl;
    for (i = 0; i < st_size; i++) {
        //Displays each state ID and if it is I/F
        curSt = _states[i];

        cout << "State " << curSt->id << " :";


        t_size = curSt->exits.size();

        if (curSt->initial) {
            cout << " Initial";
        }
        if (curSt->final) {
            cout << " Final";
        }
        cout << endl;

        //Displays the transitions of a state from each letter in the alphabet
        for (c = 0; c < a_size; c++) {
            cout << "    " << _alphabet[c] << ": ";

            for (j = 0; j < t_size; j++) {
                if (curSt->exits[j]->trans == _alphabet[c]) {
                    cout << curSt->exits[j]->dest->id << " ";

                }

            }
            cout << endl;
        }


        cout << endl;
    }
    cout << " - - - - - - - - - - - - - - - - - - - - - " << endl << endl;
}


void FA::addState(string ID) {
    State* St = new State;
    string
    x;
    char c, letter;
    bool exists;
    Transition* newT = nullptr;



    //Asks for state ID if it wasn't already given
    if (ID == "-1") {
        cout << "State ID: ";
        cin >> ID;
    }
    St->id = ID;

    //sets if the FA is initial or final
    cout << "Is state " << St->id << " initial (y/n): ";
    cin >> c;
    St->initial = c == 'y';


    cout << "Is state " << St->id << " final (y/n): ";
    cin >> c;
    St->final = c == 'y';

    //adds generated state to full list
    _states.push_back(St);

    do {
        //display automata
        display();

        cout << "Add another transition from state " << St->id << " ? (y/n)";
        cin >> c;
        if (c == 'y') {
            cout << "Transition Character: ";
            cin >> letter;

            //tries to find the transition character in the alphabet, if it does not exist, generate it
            if (find(_alphabet.begin(), _alphabet.end(), letter) == _alphabet.end()) {
                _alphabet.push_back(letter);
            }

            cout << "Transition destination state: ";
            cin >> x;

            //Tries to find the destination state, if it does not exist, creates it
            if (State::searchById(&_states, x) == nullptr) {
                cout << endl << "    This is a new transition, making a new state... " << endl << endl;
                //recursion <3<3<3<3
                addState(x);
            }

            //creates a Transition and adds it to the list
            newT = new Transition;

            newT->trans = letter;
            newT->dest = State::searchById(&_states, x);

            //if the transition already exists, it does not add it
            exists = false;
            for (Transition* exitStates : St->exits) {
                if (exitStates->trans == newT->trans && exitStates->dest == newT->dest) {
                    exists = true;
                    break;
                }
            }

            if (!exists) {
                St->exits.push_back(newT);
            }
        }
    } while (c == 'y');

}

void FA::changeName(string name) {
    _name = name;
}


State* State::searchById(vector<State*>* list, string id) {
    vector<State*>::iterator it;
    it = find_if(list->begin(), list->end(), [&id](State* st) -> bool {
        return st->id == id;
    });
    if (it != list->end()) {
        return *it;
    }
    return nullptr;
}