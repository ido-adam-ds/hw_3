#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include "library2.h"

namespace DS{

    class Exception{
    private:
        StatusType error;
    public:
        Exception(StatusType error): error(error) {}
        const StatusType& getError() const{
            return this -> error;
        }
    };

    class Failure: public Exception{
    public:
        Failure(): Exception(StatusType::FAILURE) {}
    };

    /*class InvalidInput: public Exception{
    public:
        InvalidInput(): Exception(StatusType::INVALID_INPUT) {}
    };
    */
   
    class AllocationError: public Exception{
    public:
        AllocationError(): Exception(StatusType::ALLOCATION_ERROR) {}
    };

}


#endif /* EXCEPTIONS_H */