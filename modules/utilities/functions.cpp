#include "utilities.h"


void errorLog(std::string s) {
    std::cout << "Error: " <<s<< "\n";
}

void warningLog(std::string s) {
    std::cout << "Warning: " << s << "\n";

}

void myAssert(bool stmt,std::string failDescription) {

    if (!stmt) {
        std::cout << "Assertion error: ";
        std::cout << failDescription << std::endl;
        throw ASSERTION_ERROR;
    }
    else {
        return;
    }


}

std::pair<int,int> stringToIntPair(std::string s) {

    int middlePos = s.find(",");

    std::string x = s.substr(0, middlePos);
    std::string y = s.substr(middlePos + 1, s.length() - x.length());




    return std::pair(std::stoi(x), std::stoi(y));

}


