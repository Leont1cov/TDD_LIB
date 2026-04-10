#ifndef MERETDD_TEST_H
#define MERETDD_TEST_H

#include <string_view>
#include <vector> // динамический массив

namespace MereTDD
{
    class TestInterface
    {
    public:
        virtual ~TestInterface() = default;
        virtual void run() = 0;
    };

    inline std::vector<TestInterface*>& getTests()
    {
        static std::vector<TestInterface*> tests;
        return tests;
    }

    inline void runTests()
    {
        for(MereTDD::TestInterface* test: MereTDD::getTests())
        {
            try
            {
                test->run();
            }
            catch(...)
            {
                test->setFailed("Выброшено неожиданное исключение.");
            }
        }
    }
}

// начало определения макроса теста >>>
#define TEST \
class Test: public MereTDD::TestInterface\
{\
    std::string mName;\
    bool mResult;\
public:\
    Test(std::string_view name): mName{ name }, mResult{ true }\
    {\
        MereTDD::getTests().push_back(this);\
    }\
\
    void run() override;\
};\
\
Test test("названиеСоздаваемогоТеста");\
\
void Test::run()
// >>> окончание определения макроса теста

#endif // MERETDD_TEST_H