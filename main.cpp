#include <iostream>
#include "src/FA.h"
#include "src/files.h"

int main() {
    string lmao = "automata/test5.txt";
    FA fa(lmao);
    fa.display();
    fa.isSynchronous(true);
    FA* fx = fa.determinize();
    fx->display();
    fx->isSynchronous(true);

    return 0;
}