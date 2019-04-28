#include <algorithm>
#include "determinization.h"

static State*
determinizationProcess_Sync(vector<State*> &presentStates, const vector<char> &alphabet,
                            const vector<State*> &sameStates,
                            const vector<State*> &init, const vector<State*> &fin) {
    // Naming the new state
    string newID = concatenateID(sameStates);
    // Checking if the state isn't already there
    auto search = State::searchById(presentStates, newID);
    if (search != nullptr) {
        return search;
    }

    // Allocation of the state, and giving it its properties
    State* actual = initStateDet(presentStates, sameStates, init, newID);

    // Merging transitions with the same character
    for (auto c: alphabet) {
        vector<State*> sameFinalState;
        locatingSameTransition(sameFinalState, sameStates, c);
        // Creating the transition, with the character and the address of the terminal state
        if (!sameFinalState.empty()) {
            Transition* tr = new Transition;
            tr->trans = c;
            tr->dest = determinizationProcess_Sync(presentStates, alphabet, sameFinalState, init, fin);
            actual->exits.push_back(tr);
        }
    }
    return actual;
}

FA* FA::determinization_Sync() {
    if (_determinized) {
        return this;
    }

    vector<State*> initials, finals, states;

    // Recovering final and initial states
    State::recoverSpecials(_states, initials, finals);

    determinizationProcess_Sync(states, _alphabet, initials, initials, finals);

    // Creating the FA in itself
    FA* determinized = new FA(states, _alphabet);
    return determinized;
}

static State*
determinizationProcess_Async(vector<State*> &presentStates, const vector<char> &alphabet,
                             const vector<State*> &sameStates,
                             const vector<State*> &init, const vector<State*> &fin) {
    // Recovering every state accessible with empty transition
    vector<State*> groupState = sameStates;
    for (int i = 0; i < groupState.size(); i++) {
        // Note that we are using a classic for loop, because otherwise it wouldn't take into account the update made to groupState
        for (Transition* tr: groupState[i]->exits) {
            if (tr->trans == EMPTY) {
                if (State::searchById(groupState, tr->dest->id) == nullptr) {
                    groupState.push_back(tr->dest);
                }
            }
        }
    }

    // Naming the new state
    string newID = concatenateID(groupState);
    // Checking if the state isn't already there
    auto search = State::searchById(presentStates, newID);
    if (search != nullptr) {
        return search;
    }

    // Allocation of the state, and giving it its properties
    State* actual = initStateDet(presentStates, groupState, init, newID);

    // Merging transitions with the same character
    for (auto c: alphabet) {
        vector<State*> sameFinalState;
        locatingSameTransition(sameFinalState, groupState, c);
        // Creating the transition, with the character and the address of the terminal state
        if (!sameFinalState.empty()) {
            Transition* tr = new Transition;
            tr->trans = c;
            tr->dest = determinizationProcess_Async(presentStates, alphabet, sameFinalState, init, fin);
            actual->exits.push_back(tr);
        }
    }
    return actual;
}

FA* FA::determinization_Async() {
    vector<State*> initials, finals, states;

    // Recovering final and initial states
    State::recoverSpecials(_states, initials, finals);

    determinizationProcess_Async(states, _alphabet, initials, initials, finals);

    // Creating the FA in itself
    FA* determinized = new FA(states, _alphabet);
    return determinized;
}

FA* FA::determinize() {
    if (_synchronous) {
        return determinization_Sync();
    } else {
        return determinization_Async();
    }
}

static State*
initStateDet(vector<State*> &presentStates, const vector<State*> &sameStates, const vector<State*> &init,
             const string &newID) {
    // Creating the state
    State* actual = new State;
    actual->id = newID;
    actual->initial = sameStates == init;
    actual->final = State::isAnyFinal(sameStates);
    presentStates.push_back(actual);
    return actual;
}

static void locatingSameTransition(vector<State*> &sameFinalState, const vector<State*> &sameStates, const char c) {
    for (State* st: sameStates) {
        for (Transition* tr: st->exits) {
            bool isNotPresent = find_if(sameFinalState.begin(), sameFinalState.end(), [tr](const State* s) -> bool {
                return s == tr->dest;
            }) == sameFinalState.end();
            if (tr->trans == c && isNotPresent) {
                // We add the element if it is not already on the list and if the transition character correspond
                sameFinalState.push_back(tr->dest);
            }
        }
    }
}