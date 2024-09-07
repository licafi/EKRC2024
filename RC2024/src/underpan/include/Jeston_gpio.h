#ifndef Jeston_gpio
#define Jeston_gpio

#include <string>

class JetsonGPIO {
public:
    JetsonGPIO(std::string pin);
    ~JetsonGPIO();

    void setDirection(std::string direction);
    void setValue(std::string value);

private:
    std::string pinNumber;

    void exportGPIO();
    void unexportGPIO();
};

void getball();
void putball();

#endif // Jeston_gpio