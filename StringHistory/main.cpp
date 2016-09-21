#include <string>
#include <stack>
#include <memory>
#include <mutex>
#include <iostream>
#include <algorithm>

#define TEST_MODE 1

#ifdef TEST_MODE
#define PRINT_VALUE(VALUE) \
    std::cout << VALUE << std::endl;
#else
#define PRINT_VALUE(VALUE) (void) VALUE;
#endif

// Class provides a simple restoring by saving all values
// Pro: fast implmentation, CPU performance
// Cons: RAM consaption is maximum
template<typename T>
class FastRestorer {
public:
    static void set_last_changed_string(T* pointer)
    {
       std::lock_guard<std::mutex> lock(
          history_lock);
       access_history.push_back(pointer);
    }
    static T* get_last_changed_string()
    {
       std::lock_guard<std::mutex> lock(
          history_lock);
       if (access_history.empty()) {
          return nullptr;
       }
       T* last = access_history.back();
       access_history.pop_back();
       return last;
    }

    static void clear_history(T* pointer)
    {
       std::lock_guard<std::mutex> lck(
          history_lock);
       access_history.erase(
          std::remove(
             access_history.begin(),
             access_history.end(),
             pointer),
          access_history.end());
    }

    static std::mutex history_lock;
    static std::deque<T*> access_history;
};
template<typename T>
std::mutex FastRestorer<T>::history_lock;
template<typename T>
std::deque<T*> FastRestorer<T>::access_history;


// Class provides a simple interface for manipulating
// * adding new char to the end of string
// * remove last char of the string
// * reject (UnDo) last change with any string of this type
// TODO(EZamakhov) : add template for string types
// TODO(EZamakhov) : add logging error states
class CustomString {
    typedef FastRestorer<CustomString> RestorerType;
public:
    // Fabric method
    // Provides own deletion method for correct deletion in shared objects
    static std::shared_ptr<CustomString>
       Create(const std::string& str = std::string())
    {
       return std::shared_ptr<CustomString>(
          new CustomString(str),
          [](CustomString*p) {
            delete p;
            },
          std::allocator<CustomString>());
    }

   // Add char to the end of string
   void append(char ch)
   {
      save_curent_value();
      _str.push_back(ch);
      PRINT_VALUE(get_value());
   }
   // remove last char in the string
   void erase()
   {
      save_curent_value();
      if (_str.empty()) {
         return;
      }
      _str.pop_back();
      PRINT_VALUE(get_value());
   }
   // return current string value
   const std::string& get_value() const
   {
      return _str;
   }
   // restore last changed element state
   static void UnDo()
   {
       restore_last_global_value();
   }
private:
   explicit CustomString(const std::string& str) : _str(str)
   {
       PRINT_VALUE(get_value());
   }
   ~CustomString()
   {
      RestorerType::clear_history(this);
   }
   void save_curent_value()
   {
      RestorerType::set_last_changed_string(this);
      _states.push(_str);
   }
   // Restore last global change
   static void restore_last_global_value()
   {
      CustomString* last = RestorerType::get_last_changed_string();
      if (last == nullptr) {
         return;
      }
      last->restore_last_current_value();
      PRINT_VALUE(last->get_value());
   }
   // Restore last current object change
   void restore_last_current_value()
   {
       if (_states.empty()) {
          return;
       }
       _str = _states.top();
       _states.pop();
   }

   std::string _str;
   std::stack<std::string> _states;
};

void TEST(std::shared_ptr<CustomString> cstr, const std::string& expected) {
    const std::string& actual = cstr->get_value();
    if(actual != expected) {
        std::cout << "TEST FAILED!" << std::endl
                  << "Expected: \"" << expected << '"' << std::endl
                  << "Actual:   \"" << actual << '"' << std::endl;
    }
    else
    {
        std::cout << "TEST OK!" << std::endl;
    }
}



int main(int argc, char* argv[])
{
   std::shared_ptr<CustomString> cstr =
      CustomString::Create("Start Value");
   // Test initalization
   TEST(cstr, "Start Value");

   // Test adding char
   cstr->append('!');
   TEST(cstr, "Start Value!");

   //test removing char
   cstr->erase();
   TEST(cstr, "Start Value");

   // Test restore removing char
   cstr->UnDo();
   TEST(cstr, "Start Value!");

   // test restore adding char
   cstr->UnDo();
   TEST(cstr, "Start Value");

   // test restore in case of no states
   cstr->UnDo();
   TEST(cstr, "Start Value");
   // twice
   cstr->UnDo();
   TEST(cstr, "Start Value");

   cstr.reset();
   cstr = CustomString::Create();

   // Test adding chars
   cstr->append('1');
   TEST(cstr, "1");
   cstr->append('2');
   TEST(cstr, "12");
   cstr->append('3');
   TEST(cstr, "123");
   cstr->append('4');
   TEST(cstr, "1234");

   // test restore adding char
   cstr->UnDo();
   TEST(cstr, "123");
   cstr->UnDo();
   TEST(cstr, "12");
   cstr->UnDo();
   TEST(cstr, "1");
   cstr->UnDo();
   TEST(cstr, "");
   cstr->UnDo();
   TEST(cstr, "");


   // Test for cheching clear history
   std::shared_ptr<CustomString> cstr1 = CustomString::Create("1");
   std::shared_ptr<CustomString> cstr2 = CustomString::Create("2");
   cstr1->append('A');
   TEST(cstr1, "1A");
   cstr2->append('B');
   TEST(cstr2, "2B");

   // Remove last changed
   cstr2.reset();
   cstr1->UnDo();
   TEST(cstr1, "1");


   return 0;
}
