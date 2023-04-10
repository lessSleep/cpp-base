#include <iostream>
#include <ranges>
#include <type_traits>
namespace tag_invoke_test {
  template <auto& CPO>  using tag_t = std::remove_cvref_t<decltype(CPO)>;
    void tag_invoke();
  template <typename CPO, typename... Args>  using tag_invoke_result_t = decltype(tag_invoke(std::declval<CPO&&>(), std::declval<Args&&>()...));
  template<typename CPO, typename... Args>  concept nothrow_tag_invocable = noexcept(tag_invoke(std::declval<CPO&&>(), std::declval<Args&&>()...));   
  
  
     struct example_cpo {        // An optional default implementation        template<typename T>        friend bool tag_invoke(example_cpo, const T& x) noexcept {            return false;        }
        template<typename T>        auto operator()(const T& x) const          noexcept(nothrow_tag_invocable<example_cpo, const T&>)             -> tag_invoke_result_t<example_cpo, const T&>        {            return tag_invoke(example_cpo{}, x);        }              };    inline constexpr example_cpo example{};
    struct my_type {        friend bool tag_invoke(tag_t<example> , const my_type& t) noexcept {            return t.is_example_;        }
        bool is_example_;    };} 
        //namespace tag_invoke_test
int main(){    
    auto val = tag_invoke_test::example(3);   
     val = tag_invoke_test::example(tag_invoke_test::my_type{ true });
  return 0;}