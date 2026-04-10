#ifndef MERETDD_TEST_H
#define MERETDD_TEST_H

#include <string_view>
#include <vector> // динамический массив

namespace MereTDD
{
    class TestBase
    {
        std::string mName;   // название теста
        bool mPassed;        // результат теста
        std::string mReason; // причина провала теста
    public:
        TestBase(std::string_view name): mName{ name }, mPassed{ true }
        {}
        virtual ~TestBase() = default;
        virtual void run() = 0;
        // геттеры
        std::string_view name() const
        {
            return mName;
        }
        bool passed() const
        {
            return mPassed;
        }
        std::string_view reason() const
        {
            return mReason;
        }
        // 
        void setFailed(std::string_view reason)
        {
            mPassed = false;
            mReason = reason;
        }
    };

    inline std::vector<TestBase*>& getTests()
    {
        static std::vector<TestBase*> tests;
        return tests;
    }

    inline void runTests()
    {
        for(MereTDD::TestBase* test: MereTDD::getTests())
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
class Test: public MereTDD::TestBase\
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