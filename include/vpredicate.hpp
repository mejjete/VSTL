#ifndef VSTL_PREDICATE
    #define VSTL_PREDICATE
namespace vstl
{
    namespace pred
    {
        template <typename T>
        class LessThanComparable {};            //A class that has a less-than operator<.
        
        template <typename T>
        class Assignable {};                    //A class that has an operator() for its own type

        template <typename T>
        class EqualityComparable {};            //A class that has an equalance operator== for its own type
    };
}
#endif