#include <iostream>
#include <type_traits>
// g++ --std=c++17 -o tag tag1_invoke.cpp
// 参考 https://zhuanlan.zhihu.com/p/431032074

//假如你写了一个库，namespace叫xxLib


namespace xxLib {

   namespace detail {
      struct do_something_t {
         template<typename T>
         void operator()(T &t) noexcept {
            tag1_invoke(do_something_t{}, t);
         }
      };

      // 注意函数定义不再是do_something，而是tag1_invoke，tag就是 detail::do_something_t
      template<typename T>
      void tag1_invoke(do_something_t, T &t) noexcept {
         std::cout << "xxLib do something" << std::endl;
      }
   }
   // do_something{} 是你的xxLib的接口，是定制点（CPO），以 inline detail::do_something_t 类型去库里面找，
   inline detail::do_something_t do_something{};  //注意这里是do_something{}，不是do_something(),表示do_something是一个object,就是CPO里面的O，
   //第一步先找到上面的do_something，而 do_something这个object的operator()里，会调用do_something函数，这里强制ADL执行，总会正确找到第三方定制。所以这里就不会再有歧义了。
   //C++17前后的定制点调用效率没有任何损失
}

// -------------------------------------------------------------------------------

namespace users {
  struct complicated_structure {
    
    //1 下面的friend，就是能够让 你写的xxLib库访问用户的 complicated_structure ，可以理解为回调
    //2 tag1_invoke 第一个参数是你的xxLib的定制点，第二个参数是 struct，识别类型 就是上面的 complicated_structure
    //xxLib::tag_t<xxLib::do_something> 说明是要被 xxLib::detail::do_something_t 调用，理解为实现了定制点  
    friend void tag1_invoke(std::tag_t<xxLib::do_something>, complicated_structure &t) noexcept 
    {
       //函数体是自己定制的代码，例如
       std::cout << "user customized do something" << std::endl;
    }
   // 如果不使用 tag1_invoke
   //  friend void do_something(complicated_structure &t) noexcept {
   //     std::cout << "customized do something" << std::endl;
   //  }

  };

  struct simple_structure {
  };
}


int main() 
{
   //tag1_invoke的好处是：下面两种方法都能正确调用
   //使用方法1
   users::simple_structure s;
   xxLib::do_something(s); //类似构造函数的方法使用接口

   users::complicated_structure c; //复杂
   xxLib::do_something(c);

   //使用方法2 
   using namespace xxLib;
   do_something(s);
   do_something(c);

   return 0;
}
//输出
// xxLib do something
// user customized do something
// xxLib do something
// user customized do something