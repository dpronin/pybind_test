#include <pybind11/cast.h>
#include <pybind11/pybind11.h>

#include <type_traits>
#include <utility>

namespace {

class Example {
public:
  explicit Example(std::string msg) : msg_(std::move(msg)) {}
  ~Example() = default;

  Example(Example const &) = default;
  Example &operator=(Example const &) = default;

  Example(Example &&) = default;
  Example &operator=(Example &&) = default;

  void set(std::string msg) noexcept(
      std::is_nothrow_move_assignable_v<std::string>) {
    msg_ = std::move_if_noexcept(msg);
  }
  auto const &get() const noexcept { return msg_; }

private:
  std::string msg_;
};

int add(int i = 1, int j = 2) { return i + j; }

} // namespace

PYBIND11_MODULE(pybind_test, m) {
  namespace py = pybind11;
  using namespace py::literals;

  m.def("add", &add, "A function that adds two numbers", "i"_a = 1, "j"_a = 2);
  m.attr("answer") = 42;
  m.attr("what") = py::cast("World");

  py::class_<Example>(m, "Example")
      .def(py::init<std::string>())
      .def_property("msg", &Example::get, &Example::set);
}
