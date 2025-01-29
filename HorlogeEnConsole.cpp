#include <iostream>
#include <ctime>

int main()
{
 std::time_t t=std::time(nullptr);
 std::tm*time_info=std::localtime(&t);
    return 0;
}