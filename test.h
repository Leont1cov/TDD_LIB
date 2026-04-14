#ifndef MERETDD_TEST_H
#define MERETDD_TEST_H

#include <ostream>
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

    inline int runTests(std::ostream& output)
    {
        output << "Запущено тестов: " << getTests().size() << '\n';

        int numPassed{};
        int numFailed{};

        for(MereTDD::TestBase* test: MereTDD::getTests())
        {
            output << "---------------\n" << test->name() << '\n';
            try
            {
                test->run();
            }
            catch(...)
            {
                test->setFailed("Выброшено неожиданное исключение.");
            }
            if (test->passed())
            {
                numPassed++;
                output << "Пройден\n";
            }
            else
            {
                numFailed++;
                output << "Провален\n" << test->reason() << '\n';
            }
        }

        output << "---------------\n";
        if (numFailed == 0)
        {
            output << "Все тесты успешно пройдены.\n";
        }
        else
        {
            output << "Тестов успешно пройдено: " << numPassed
                      << "\nТестов провалено: " << numFailed << '\n';
        }

        return numFailed;
    }
}

#define MERETDD_CLASS_FINAL(line) Test##line
#define MERETDD_CLASS_RELAY(line) MERETDD_CLASS_FINAL(line)
#define MERETDD_CLASS MERETDD_CLASS_RELAY( __LINE__ )

#define MERETDD_INSTANCE_FINAL(line) test##line
#define MERETDD_INSTANCE_RELAY(line) MERETDD_INSTANCE_FINAL(line)
#define MERETDD_INSTANCE MERETDD_INSTANCE_RELAY( __LINE__ )

// начало определения макроса теста >>>
#define TEST(testName) \
class MERETDD_CLASS: public MereTDD::TestBase\
{\
public:\
    MERETDD_CLASS(std::string_view name): TestBase(name)\
    {\
        MereTDD::getTests().push_back(this);\
    }\
\
    void run() override;\
};\
\
MERETDD_CLASS MERETDD_INSTANCE(testName);\
\
void MERETDD_CLASS::run()
// >>> окончание определения макроса теста

#endif // MERETDD_TEST_H