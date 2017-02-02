#include "glob.h"

Glob::Glob()
{

}

Glob& glob() {
    static Glob gl;
    return gl;
}
