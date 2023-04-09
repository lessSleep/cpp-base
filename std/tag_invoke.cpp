#include <iostream>
#include <type_traits>
// g++ --std=c++17 -o tag tag_invoke.cpp
// 参考 https://zhuanlan.zhihu.com/p/431032074

//假如你写了一个库，namespace叫xxLib
namespace xxLib {
   namespace detail
   {
      void tag_invoke();
      struct tag_invoke_t 
      {
         template<typename Tag, typename... Args>
         constexpr auto operator() (Tag tag, Args &&... args) const
            noexcept(noexcept(tag_invoke(static_cast<Tag &&>(tag), static_cast<Args &&>(args)...)))
            -> decltype(tag_invoke(static_cast<Tag &&>(tag), static_cast<Args &&>(args)...))
         {
            return tag_invoke(static_cast<Tag &&>(tag), static_cast<Args &&>(args)...);
         }
      };
   }

   inline constexpr detail::tag_invoke_t tag_invoke{};

   template<auto& Tag> 
   using tag_t = std::decay_t<decltype(Tag)>; //namespace users 里面的 tag_t，也就是给用户用的
}

namespace xxLib {

   namespace detail {
      struct do_something_t {
         template<typename T>
         void operator()(T &t) noexcept {
            tag_invoke(do_something_t{}, t);
         }
      };

      // 注意函数定义不再是do_something，而是tag_invoke，tag就是 detail::do_something_t
      template<typename T>
      void tag_invoke(do_something_t, T &t) noexcept {
         std::cout << "xxLib do something" << std::endl;
      }
   }
   // do_something{} 是你的xxLib的接口，是定制点（CPO），以 inline detail::do_something_t 类型去库里面找，
   inline detail::do_something_t do_something{};  //注意这里是do_something{}，不是do_something(),表示do_something是一个object,就是CPO里面的O

}

// -------------------------------------------------------------------------------

namespace users {
  struct complicated_structure {
    
    //1 下面的friend，就是能够让 你写的xxLib库访问用户的 complicated_structure ，可以理解为回调
    //2 tag_invoke 第一个参数是你的xxLib的定制点，第二个参数是 struct，识别类型 就是上面的 complicated_structure
    //xxLib::tag_t<xxLib::do_something> 说明是要被 xxLib::detail::do_something_t 调用，理解为实现了定制点  
    friend void tag_invoke(xxLib::tag_t<xxLib::do_something>, complicated_structure &t) noexcept 
    {
       //函数体是自己定制的代码，例如
       std::cout << "user customized do something" << std::endl;
    }
    // 如果不使用 tag_invoke,就需要用户自己写这么一个函数。但是，如果用户的库里面原来有这么一个函数，且不是用来适配你的xxLib中的do_something，
    // 就会造成用户需要修改用户库的代码，例如把 do_something 改成 do_something_1,这是库作者不愿意见到的（库作者更难起名字，命名本身就是编程中最难的问题）。
    //  friend void do_something(complicated_structure &t) noexcept {
    //     std::cout << "customized do something" << std::endl;
    //  }

  };

  struct simple_structure {
  };
}


int main() 
{
   //tag_invoke的好处是：下面两种方法都能正确调用
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