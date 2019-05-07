#ifndef FINITE_AUTOMATA_MATH_CS_MINIMIZATION_H
#define FINITE_AUTOMATA_MATH_CS_MINIMIZATION_H

#include "FA.h"


typedef struct PatternGroup
{
    vector<State*> group;
    vector<int> pattern;
} PatternGroup;

typedef vector<PatternGroup>* Partition;

/// Keeps creating new Partitions according to the Minimization algortihm until there is no increase in partition number
/// \param P initial partition
/// \param alphabet Initial FA alphabet
/// \return Final Partition
static Partition partitioning(Partition P, vector<char> alphabet);

/// Turns a partition into a list of states
/// \param P Partition
/// \param alphabet initial Fa alphabet
/// \return List of States
static vector<State*>* Partition2States(Partition P, vector<char> &alphabet);

/// Get the pattern of a State in a Partition
/// \param source The previous Partition
/// \param exits Exits of a State
/// \param alphabet FA alphabet
/// \return Pattern of
static vector<int>* getPattern(Partition source, vector<Transition*> &exits, vector<char> alphabet);


/// Checks if 2 patterns are the same
/// \param p1 Pattern 1
/// \param p2 Pattern 2
/// \return True if they are the same
static bool isSamePattern(vector<int> &p1, vector<int> &p2);


#endif //FINITE_AUTOMATA_MATH_CS_MINIMIZATION_H