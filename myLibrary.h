#pragma once

#include <bits/stdc++.h>

#ifdef __GNUG__
#include <ext/pb_ds/assoc_container.hpp>
#include <cstdlib>
#include <memory>
#include <cxxabi.h>
#endif


namespace myLib {
    // Check if a type is stream writable, i.e., std::cout << foo;
    template<typename S, typename T, typename = void>
    struct is_to_stream_writable : std::false_type {
    };

    template<typename S, typename T>
    struct is_to_stream_writable<S, T,
            std::void_t<decltype(std::declval<S &>() << std::declval<T>()) >>
            : std::true_type {
    };


// Printing std::tuple
// The indices trick: http://loungecpp.wikidot.com/tips-and-tricks:indices
    namespace pprint {

        template<std::size_t...>
        struct seq {
        };

        template<std::size_t N, std::size_t... Is>
        struct gen_seq : gen_seq<N - 1, N - 1, Is...> {
        };

        template<std::size_t... Is>
        struct gen_seq<0, Is...> : seq<Is...> {
        };

        template<typename T>
        inline T to_string(T value) {
          return value;
        }

        inline std::string to_string(char value) {
          return "'" + std::string(1, value) + "'";
        }

        inline std::string to_string(const char *value) {
          return "\"" + std::string(value) + "\"";
        }

        inline std::string to_string(const std::string &value) {
          return "\"" + value + "\"";
        }

        template<class Ch, class Tr, class Tuple, std::size_t... Is>
        void print_tuple(std::basic_ostream<Ch, Tr> &os, Tuple const &t, seq<Is...>) {
          using swallow = int[];
          (void) swallow{0, (void(os << (Is == 0 ? "" : ", ") << to_string(std::get<Is>(t))), 0)...};
        }

    }

    template<class Ch, class Tr, class... Args>
    auto operator<<(std::basic_ostream<Ch, Tr> &os, std::tuple<Args...> const &t)
    -> std::basic_ostream<Ch, Tr> & {
      os << "(";
      pprint::print_tuple(os, t, pprint::gen_seq<sizeof...(Args)>());
      return os << ")";
    }

// Enum value must be greater or equals than MAGIC_ENUM_RANGE_MIN. By default MAGIC_ENUM_RANGE_MIN = -128.
// If need another min range for all enum types by default, redefine the macro MAGIC_ENUM_RANGE_MIN.
#if !defined(MAGIC_ENUM_RANGE_MIN)
#  define MAGIC_ENUM_RANGE_MIN -128
#endif

// Enum value must be less or equals than MAGIC_ENUM_RANGE_MAX. By default MAGIC_ENUM_RANGE_MAX = 128.
// If need another max range for all enum types by default, redefine the macro MAGIC_ENUM_RANGE_MAX.
#if !defined(MAGIC_ENUM_RANGE_MAX)
#  define MAGIC_ENUM_RANGE_MAX 128
#endif

    namespace magic_enum {

        // Enum value must be in range [-MAGIC_ENUM_RANGE_MAX, MAGIC_ENUM_RANGE_MIN]. By default  MAGIC_ENUM_RANGE_MIN = -128, MAGIC_ENUM_RANGE_MAX = 128.
        // If need another range for all enum types by default, redefine the macro MAGIC_ENUM_RANGE_MAX and MAGIC_ENUM_RANGE_MIN.
        // If need another range for specific enum type, add specialization enum_range for necessary enum type.
        template<typename E>
        struct enum_range final {
            static_assert(std::is_enum_v<E>, "magic_enum::enum_range requires enum type.");
            static constexpr int min = std::is_signed_v<std::underlying_type_t<E>> ? MAGIC_ENUM_RANGE_MIN : 0;
            static constexpr int max = MAGIC_ENUM_RANGE_MAX;
        };

        static_assert(MAGIC_ENUM_RANGE_MAX > 0,
                      "MAGIC_ENUM_RANGE_MAX must be greater than 0.");
        static_assert(MAGIC_ENUM_RANGE_MAX < std::numeric_limits<int>::max(),
                      "MAGIC_ENUM_RANGE_MAX must be less than INT_MAX.");

        static_assert(MAGIC_ENUM_RANGE_MIN <= 0,
                      "MAGIC_ENUM_RANGE_MIN must be less or equals than 0.");
        static_assert(MAGIC_ENUM_RANGE_MIN > std::numeric_limits<int>::min(),
                      "MAGIC_ENUM_RANGE_MIN must be greater than INT_MIN.");

        namespace detail {

            template<typename E, typename U = std::underlying_type_t<E>>
            [[nodiscard]] constexpr int min_impl() {
              static_assert(std::is_enum_v<E>, "magic_enum::detail::min_impl requires enum type.");
              constexpr int min = enum_range<E>::min > (std::numeric_limits<U>::min)() ? enum_range<E>::min
                                                                                       : (std::numeric_limits<U>::min)();

              return min;
            }

            template<typename E, typename U = std::underlying_type_t<E>>
            [[nodiscard]] constexpr decltype(auto) range_impl() {
              static_assert(std::is_enum_v<E>, "magic_enum::detail::range_impl requires enum type.");
              static_assert(enum_range<E>::max > enum_range<E>::min, "magic_enum::enum_range requires max > min.");
              constexpr int max = enum_range<E>::max < (std::numeric_limits<U>::max)() ? enum_range<E>::max
                                                                                       : (std::numeric_limits<U>::max)();
              constexpr auto range = std::make_integer_sequence<int, max - min_impl<E>() + 1>{};

              return range;
            }


            [[nodiscard]] constexpr bool is_name_char(char c, bool front) noexcept {
              return (!front && c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
            }

            template<typename E, E V>
            [[nodiscard]] constexpr std::string_view name_impl() noexcept {
              static_assert(std::is_enum_v<E>, "magic_enum::detail::name_impl requires enum type.");
#if defined(__clang__)
              std::string_view name{__PRETTY_FUNCTION__};
      constexpr auto suffix = sizeof("]") - 1;
#elif defined(__GNUC__) && __GNUC__ >= 9
              std::string_view name{__PRETTY_FUNCTION__};
              constexpr auto suffix = sizeof("; std::string_view = std::basic_string_view<char>]") - 1;
#elif defined(_MSC_VER)
              std::string_view name{__FUNCSIG__};
      constexpr auto suffix = sizeof(">(void) noexcept") - 1;
#else
      return {}; // Unsupported compiler.
#endif

#if defined(__clang__) || (defined(__GNUC__) && __GNUC__ >= 9) || defined(_MSC_VER)
              name.remove_suffix(suffix);
              for (std::size_t i = name.size(); i > 0; --i) {
                if (!is_name_char(name[i - 1], false)) {
                  name.remove_prefix(i);
                  break;
                }
              }

              if (name.length() > 0 && is_name_char(name.front(), true)) {
                return name;
              }
              else {
                return {}; // Value does not have name.
              }
#endif
            }

            template<typename E, int... I>
            [[nodiscard]] constexpr decltype(auto) strings_impl(std::integer_sequence<int, I...>) noexcept {
              static_assert(std::is_enum_v<E>, "magic_enum::detail::strings_impl requires enum type.");
              constexpr std::array<std::string_view, sizeof...(I)> names{{name_impl<E, static_cast<E>(I +
                                                                                                      min_impl<E>())>()...}};

              return names;
            }

            template<typename E>
            [[nodiscard]] constexpr std::string_view name_impl(int value) noexcept {
              static_assert(std::is_enum_v<E>, "magic_enum::detail::name_impl requires enum type.");
              constexpr auto names = strings_impl<E>(range_impl<E>());
              const int i = value - min_impl<E>();

              if (i >= 0 && static_cast<std::size_t>(i) < names.size()) {
                return names[i];
              }
              else {
                return {}; // Value out of range.
              }
            }

            template<typename E, int... I>
            [[nodiscard]] constexpr decltype(auto) values_impl(std::integer_sequence<int, I...>) noexcept {
              static_assert(std::is_enum_v<E>, "magic_enum::detail::values_impl requires enum type.");
              constexpr int n = sizeof...(I);
              constexpr std::array<bool, n> valid{{!name_impl<E, static_cast<E>(I + min_impl<E>())>().empty()...}};
              constexpr int num_valid = ((valid[I] ? 1 : 0) + ...);

              std::array<E, num_valid> enums{};
              for (int i = 0, v = 0; i < n && v < num_valid; ++i) {
                if (valid[i]) {
                  enums[v++] = static_cast<E>(i + min_impl<E>());
                }
              }

              return enums;
            }

            template<typename E, std::size_t... I>
            [[nodiscard]] constexpr decltype(auto) names_impl(std::integer_sequence<std::size_t, I...>) noexcept {
              static_assert(std::is_enum_v<E>, "magic_enum::detail::names_impl requires enum type.");
              constexpr auto enums = values_impl<E>(range_impl<E>());
              constexpr std::array<std::string_view, sizeof...(I)> names{{name_impl<E, enums[I]>()...}};

              return names;
            }

            template<typename E>
            [[nodiscard]] constexpr std::optional<E> enum_cast_impl(std::string_view value) noexcept {
              static_assert(std::is_enum_v<E>, "magic_enum::detail::enum_cast_impl requires enum type.");
              constexpr auto values = values_impl<E>(range_impl<E>());
              constexpr auto count = values.size();
              constexpr auto names = names_impl<E>(std::make_index_sequence<count>{});

              for (std::size_t i = 0; i < count; ++i) {
                if (names[i] == value) {
                  return values[i];
                }
              }

              return std::nullopt; // Invalid value or out of range.
            }

            template<typename T>
            using enable_if_enum_t = typename std::enable_if<std::is_enum_v<T>>::type;

            template<typename T, bool = std::is_enum_v<T>>
            struct is_scoped_enum_impl : std::false_type {
            };

            template<typename T>
            struct is_scoped_enum_impl<T, true> : std::bool_constant<!std::is_convertible_v<T, std::underlying_type_t<T>>> {
            };

            template<typename T, bool = std::is_enum_v<T>>
            struct is_unscoped_enum_impl : std::false_type {
            };

            template<typename T>
            struct is_unscoped_enum_impl<T, true>
                    : std::bool_constant<std::is_convertible_v<T, std::underlying_type_t<T>>> {
            };

        } // namespace magic_enum::detail

        // Checks whether T is an Unscoped enumeration type.
        // Provides the member constant value which is equal to true, if T is an [Unscoped enumeration](https://en.cppreference.com/w/cpp/language/enum#Unscoped_enumeration) type.
        // Otherwise, value is equal to false.
        template<typename T>
        struct is_unscoped_enum : detail::is_unscoped_enum_impl<T> {
        };

        template<typename T>
        inline constexpr bool is_unscoped_enum_v = is_unscoped_enum<T>::value;

        // Checks whether T is an Scoped enumeration type.
        // Provides the member constant value which is equal to true, if T is an [Scoped enumeration](https://en.cppreference.com/w/cpp/language/enum#Scoped_enumerations) type.
        // Otherwise, value is equal to false.
        template<typename T>
        struct is_scoped_enum : detail::is_scoped_enum_impl<T> {
        };

        template<typename T>
        inline constexpr bool is_scoped_enum_v = is_scoped_enum<T>::value;

        // Obtains enum value from enum string name.
        template<typename E, typename = detail::enable_if_enum_t<E>>
        [[nodiscard]] constexpr std::optional<E> enum_cast(std::string_view value) noexcept {
          static_assert(std::is_enum_v<E>, "magic_enum::enum_cast requires enum type.");

          return detail::enum_cast_impl<E>(value);
        }

        // Obtains enum value from integer value.
        template<typename E, typename = detail::enable_if_enum_t<E>>
        [[nodiscard]] constexpr std::optional<E> enum_cast(std::underlying_type_t<E> value) noexcept {
          static_assert(std::is_enum_v<E>, "magic_enum::enum_cast requires enum type.");

          if (detail::name_impl<E>(static_cast<int>(value)).empty()) {
            return std::nullopt; // Invalid value or out of range.
          }
          else {
            return static_cast<E>(value);
          }
        }

        // Returns enum value at specified index.
        // No bounds checking is performed: the behavior is undefined if index >= number of enum values.
        template<typename E, typename = detail::enable_if_enum_t<E>>
        [[nodiscard]] constexpr E enum_value(std::size_t index) {
          static_assert(std::is_enum_v<E>, "magic_enum::enum_value requires enum type.");
          constexpr auto values = detail::values_impl<E>(detail::range_impl<E>());

          return assert(index < values.size()), values[index];
        }

        // Obtains value enum sequence.
        template<typename E, typename = detail::enable_if_enum_t<E>>
        [[nodiscard]] constexpr decltype(auto) enum_values() noexcept {
          static_assert(std::is_enum_v<E>, "magic_enum::enum_values requires enum type.");
          constexpr auto values = detail::values_impl<E>(detail::range_impl<E>());

          return values;
        }

        // Returns number of enum values.
        template<typename E, typename = detail::enable_if_enum_t<E>>
        [[nodiscard]] constexpr std::size_t enum_count() noexcept {
          static_assert(std::is_enum_v<E>, "magic_enum::enum_count requires enum type.");
          constexpr auto count = detail::values_impl<E>(detail::range_impl<E>()).size();

          return count;
        }

        // Obtains string enum name from enum value.
        template<typename E, typename D = std::decay_t<E>, typename = detail::enable_if_enum_t<D>>
        [[nodiscard]] constexpr std::optional<std::string_view> enum_name(E value) noexcept {
          static_assert(std::is_enum_v<D>, "magic_enum::enum_name requires enum type.");
          const auto name = detail::name_impl<D>(static_cast<int>(value));

          if (name.empty()) {
            return std::nullopt; // Invalid value or out of range.
          }
          else {
            return name;
          }
        }

        // Obtains string enum name sequence.
        template<typename E, typename = detail::enable_if_enum_t<E>>
        [[nodiscard]] constexpr decltype(auto) enum_names() noexcept {
          static_assert(std::is_enum_v<E>, "magic_enum::enum_names requires enum type.");
          constexpr auto count = detail::values_impl<E>(detail::range_impl<E>()).size();
          constexpr auto names = detail::names_impl<E>(std::make_index_sequence<count>{});

          return names;
        }

        namespace ops {

            template<typename E, typename D = std::decay_t<E>, typename = detail::enable_if_enum_t<E>>
            std::ostream &operator<<(std::ostream &os, E value) {
              static_assert(std::is_enum_v<D>, "magic_enum::ops::operator<< requires enum type.");
              const auto name = detail::name_impl<D>(static_cast<int>(value));

              if (!name.empty()) {
                os << name;
              }

              return os;
            }

            template<typename E, typename = detail::enable_if_enum_t<E>>
            std::ostream &operator<<(std::ostream &os, std::optional<E> value) {
              static_assert(std::is_enum_v<E>, "magic_enum::ops::operator<< requires enum type.");

              if (value.has_value()) {
                const auto name = detail::name_impl<E>(static_cast<int>(value.value()));
                if (!name.empty()) {
                  os << name;
                }
              }

              return os;
            }

        } // namespace magic_enum::ops

    } // namespace magic_enum

    namespace pprint {

        // Some utility structs to check template specialization
        template<typename Test, template<typename...> class Ref>
        struct is_specialization : std::false_type {
        };

        template<template<typename...> class Ref, typename... Args>
        struct is_specialization<Ref<Args...>, Ref> : std::true_type {
        };

        template<typename ...>
        using to_void = void;

        template<typename T, typename = void>
        struct is_container : std::false_type {
        };

        template<typename T>
        struct is_container<T,
                to_void<decltype(std::declval<T>().begin()),
                        decltype(std::declval<T>().end()),
                        typename T::value_type
                >> : std::true_type // will  be enabled for iterable objects
        {
        };

        class PrettyPrinter {
        private:
            std::ostream &stream_;
            std::string line_terminator_;
            size_t indent_;
            bool quotes_;
            bool compact_;

        public:

            PrettyPrinter(std::ostream &stream = std::cout) :
                    stream_(stream),
                    line_terminator_("\n"),
                    indent_(2),
                    quotes_(false),
                    compact_(false) {}

            PrettyPrinter &line_terminator(const std::string &value) {
              line_terminator_ = value;
              return *this;
            }

            PrettyPrinter &indent(size_t indent) {
              indent_ = indent;
              return *this;
            }

            PrettyPrinter &compact(bool value) {
              compact_ = value;
              return *this;
            }

            PrettyPrinter &quotes(bool value) {
              quotes_ = value;
              return *this;
            }

            template<typename T>
            void print(T value) {
              print_internal(value, 0, line_terminator_, 0);
            }

            template<typename T>
            void print(std::initializer_list<T> value) {
              print_internal(value, 0, line_terminator_, 0);
            }

            template<typename T, typename... Targs>
            void print(T value, Targs... Fargs) {
              print_internal(value, 0, "", 0);
              auto current_quotes = quotes_;
              quotes_ = false;
              print_internal(" ", 0, "", 0);
              quotes_ = current_quotes;
              print(Fargs...);
            }

            template<typename T>
            void print_inline(T value) {
              print_internal(value, indent_, "", 0);
            }

            template<typename T>
            void print_inline(std::initializer_list<T> value) {
              print_internal(value, indent_, "", 0);
            }

            template<typename T, typename... Targs>
            void print_inline(T value, Targs... Fargs) {
              print_internal(value, indent_, "", 0);
              auto current_quotes = quotes_;
              quotes_ = false;
              print_internal(" ", 0, "", 0);
              quotes_ = current_quotes;
              print_inline(Fargs...);
            }

        private:

            template<typename T>
            typename std::enable_if<std::is_integral<T>::value == true, void>::type
            print_internal(T value, size_t indent = 0, const std::string &line_terminator = "\n", size_t level = 0) {
              stream_ << std::string(indent, ' ') << value << line_terminator;
            }

            template<typename T>
            typename std::enable_if<std::is_null_pointer<T>::value == true, void>::type
            print_internal(T value, size_t indent = 0, const std::string &line_terminator = "\n", size_t level = 0) {
              stream_ << std::string(indent, ' ') << "nullptr" << line_terminator;
            }

            void
            print_internal(float value, size_t indent = 0, const std::string &line_terminator = "\n", size_t level = 0) {
              stream_ << std::string(indent, ' ') << value << 'f' << line_terminator;
            }

            void
            print_internal(double value, size_t indent = 0, const std::string &line_terminator = "\n", size_t level = 0) {
              stream_ << std::string(indent, ' ') << value << line_terminator;
            }

            void print_internal(const std::string &value, size_t indent = 0, const std::string &line_terminator = "\n",
                                size_t level = 0) {
              if (!quotes_)
                print_internal_without_quotes(value, indent, line_terminator, level);
              else
                stream_ << std::string(indent, ' ') << "\"" << value << "\"" << line_terminator;
            }

            void print_internal(const char *value, size_t indent = 0, const std::string &line_terminator = "\n",
                                size_t level = 0) {
              if (!quotes_)
                print_internal_without_quotes(value, indent, line_terminator, level);
              else
                stream_ << std::string(indent, ' ') << "\"" << value << "\"" << line_terminator;
            }

            void
            print_internal(char value, size_t indent = 0, const std::string &line_terminator = "\n", size_t level = 0) {
              if (!quotes_)
                print_internal_without_quotes(value, indent, line_terminator, level);
              else
                stream_ << std::string(indent, ' ') << "'" << value << "'" << line_terminator;
            }

            void print_internal_without_quotes(const std::string &value, size_t indent = 0,
                                               const std::string &line_terminator = "\n", size_t level = 0) {
              stream_ << std::string(indent, ' ') << value << line_terminator;
            }

            void print_internal_without_quotes(const char *value, size_t indent = 0,
                                               const std::string &line_terminator = "\n", size_t level = 0) {
              stream_ << std::string(indent, ' ') << value << line_terminator;
            }

            void print_internal_without_quotes(char value, size_t indent = 0, const std::string &line_terminator = "\n",
                                               size_t level = 0) {
              stream_ << std::string(indent, ' ') << value << line_terminator;
            }

            void
            print_internal(bool value, size_t indent = 0, const std::string &line_terminator = "\n", size_t level = 0) {
              stream_ << std::string(indent, ' ') << (value ? "true" : "false") << line_terminator;
            }

            template<typename T>
            typename std::enable_if<std::is_pointer<T>::value == true, void>::type
            print_internal(T value, size_t indent = 0, const std::string &line_terminator = "\n", size_t level = 0) {
              if (value == nullptr) {
                return print_internal(nullptr, indent, line_terminator, level);
              }
              stream_ << std::string(indent, ' ') << "<" << type(value) << " at "
                      << value << ">" << line_terminator;
            }

            std::string demangle(const char *name) {
#ifdef __GNUG__
              int status = -4;
              std::unique_ptr<char, void (*)(void *)> res{
                      abi::__cxa_demangle(name, NULL, NULL, &status),
                      std::free
              };
              return (status == 0) ? res.get() : name;
#else
              return name;
#endif
            }

            template<class T>
            std::string type(const T &t) {
              return demangle(typeid(t).name());
            }

            template<typename T>
            typename std::enable_if<std::is_enum<T>::value == true, void>::type
            print_internal(T value, size_t indent = 0, const std::string &line_terminator = "\n", size_t level = 0) {
              auto enum_string = magic_enum::enum_name(value);
              if (enum_string.has_value()) {
                stream_ << std::string(indent, ' ') << enum_string.value()
                        << line_terminator;
              }
              else {
                stream_ << std::string(indent, ' ') << static_cast<std::underlying_type_t<T>>(value)
                        << line_terminator;
              }
            }

            template<typename T>
            typename std::enable_if<std::is_class<T>::value == true &&
                                    is_to_stream_writable<std::ostream, T>::value == true &&
                                    std::is_enum<T>::value == false &&
                                    is_specialization<T, std::unique_ptr>::value == false &&
                                    is_specialization<T, std::shared_ptr>::value == false &&
                                    is_specialization<T, std::weak_ptr>::value == false &&
                                    is_specialization<T, std::tuple>::value == false &&
                                    is_specialization<T, std::variant>::value == false &&
                                    is_specialization<T, std::vector>::value == false &&
                                    is_specialization<T, std::list>::value == false &&
                                    is_specialization<T, std::deque>::value == false &&
                                    is_specialization<T, std::queue>::value == false &&
                                    is_specialization<T, std::priority_queue>::value == false &&
                                    is_specialization<T, std::stack>::value == false &&
                                    is_specialization<T, std::set>::value == false &&
                                    is_specialization<T, std::multiset>::value == false &&
                                    is_specialization<T, std::unordered_set>::value == false &&
                                    is_specialization<T, std::unordered_multiset>::value == false &&
                                    is_specialization<T, std::map>::value == false &&
                                    is_specialization<T, std::multimap>::value == false &&
                                    is_specialization<T, std::unordered_map>::value == false &&
                                    is_specialization<T, std::unordered_multimap>::value == false, void>::type
            print_internal(T value, size_t indent = 0, const std::string &line_terminator = "\n", size_t level = 0) {
              stream_ << std::string(indent, ' ') << value << line_terminator;
            }

            template<typename T>
            typename std::enable_if<std::is_class<T>::value == true &&
                                    is_to_stream_writable<std::ostream, T>::value == false &&
                                    std::is_enum<T>::value == false &&
                                    is_specialization<T, std::unique_ptr>::value == false &&
                                    is_specialization<T, std::shared_ptr>::value == false &&
                                    is_specialization<T, std::weak_ptr>::value == false &&
                                    is_specialization<T, std::tuple>::value == false &&
                                    is_specialization<T, std::variant>::value == false &&
                                    is_specialization<T, std::vector>::value == false &&
                                    is_specialization<T, std::list>::value == false &&
                                    is_specialization<T, std::deque>::value == false &&
                                    is_specialization<T, std::queue>::value == false &&
                                    is_specialization<T, std::priority_queue>::value == false &&
                                    is_specialization<T, std::stack>::value == false &&
                                    is_specialization<T, std::set>::value == false &&
                                    is_specialization<T, std::multiset>::value == false &&
                                    is_specialization<T, std::unordered_set>::value == false &&
                                    is_specialization<T, std::unordered_multiset>::value == false &&
                                    is_specialization<T, std::map>::value == false &&
                                    is_specialization<T, std::multimap>::value == false &&
                                    is_specialization<T, std::unordered_map>::value == false &&
                                    is_specialization<T, std::unordered_multimap>::value == false, void>::type
            print_internal(T value, size_t indent = 0, const std::string &line_terminator = "\n", size_t level = 0) {
              stream_ << std::string(indent, ' ') << "<Object " << type(value) << ">"
                      << line_terminator;
            }

            template<typename T>
            typename std::enable_if<std::is_member_function_pointer<T>::value == true, void>::type
            print_internal(T value, size_t indent = 0, const std::string &line_terminator = "\n", size_t level = 0) {
              stream_ << std::string(indent, ' ') << "<Object.method " << type(value)
                      << " at " << &value << ">"
                      << line_terminator;
            }

            template<typename Container>
            typename std::enable_if<is_specialization<Container, std::vector>::value, void>::type
            print_internal(const Container &value, size_t indent = 0, const std::string &line_terminator = "\n",
                           size_t level = 0) {
              typedef typename Container::value_type T;
              if (level == 0 && !compact_) {
                if (value.size() == 0) {
                  print_internal_without_quotes("[", 0, "");
                }
                else if (value.size() == 1) {
                  print_internal_without_quotes("[", 0, "");
                  print_internal(value.front(), 0, "", level + 1);
                }
                else if (value.size() > 0) {
                  print_internal_without_quotes("[", 0, "\n");
                  print_internal(value.front(), indent + indent_, "", level + 1);
                  if (value.size() > 1 && is_container<T>::value == false)
                    print_internal_without_quotes(", ", 0, "\n");
                  else if (is_container<T>::value)
                    print_internal_without_quotes(", ", 0, "\n");
                  for (size_t i = 1; i < value.size() - 1; i++) {
                    print_internal(value[i], indent + indent_, "", level + 1);
                    if (is_container<T>::value == false)
                      print_internal_without_quotes(", ", 0, "\n");
                    else
                      print_internal_without_quotes(", ", 0, "\n");
                  }
                  if (value.size() > 1) {
                    print_internal(value.back(), indent + indent_, "\n", level + 1);
                  }
                }
                if (value.size() == 0)
                  print_internal_without_quotes("]", indent, "");
                else if (is_container<T>::value == false)
                  print_internal_without_quotes("]", indent, "");
                else
                  print_internal_without_quotes(line_terminator_ + "]", indent, "");
                print_internal_without_quotes(line_terminator_, 0, "");
              }
              else {
                if (value.size() == 0) {
                  print_internal_without_quotes("[", indent, "");
                }
                else if (value.size() == 1) {
                  print_internal_without_quotes("[", indent, "");
                  print_internal(value.front(), 0, "", level + 1);
                }
                else if (value.size() > 0) {
                  print_internal_without_quotes("[", indent, "");
                  print_internal(value.front(), 0, "", level + 1);
                  if (value.size() > 1)
                    print_internal_without_quotes(", ", 0, "");
                  for (size_t i = 1; i < value.size() - 1; i++) {
                    print_internal(value[i], 0, "", level + 1);
                    print_internal_without_quotes(", ", 0, "");
                  }
                  if (value.size() > 1) {
                    print_internal(value.back(), 0, "", level + 1);
                  }
                }
                print_internal_without_quotes("]", 0, "");
                if (level == 0 && compact_)
                  print_internal_without_quotes(line_terminator_, 0, "");
              }

            }

            template<typename T, unsigned long int S>
            void print_internal(const std::array<T, S> &value, size_t indent = 0, const std::string &line_terminator = "\n",
                                size_t level = 0) {
              if (level == 0 && !compact_) {
                if (value.size() == 0) {
                  print_internal_without_quotes("[", 0, "");
                }
                else if (value.size() == 1) {
                  print_internal_without_quotes("[", 0, "");
                  print_internal(value.front(), 0, "", level + 1);
                }
                else if (value.size() > 0) {
                  print_internal_without_quotes("[", 0, "\n");
                  print_internal(value.front(), indent + indent_, "", level + 1);
                  if (value.size() > 1 && is_container<T>::value == false)
                    print_internal_without_quotes(", ", 0, "\n");
                  else if (is_container<T>::value)
                    print_internal_without_quotes(", ", 0, "\n");
                  for (size_t i = 1; i < value.size() - 1; i++) {
                    print_internal(value[i], indent + indent_, "", level + 1);
                    if (is_container<T>::value == false)
                      print_internal_without_quotes(", ", 0, "\n");
                    else
                      print_internal_without_quotes(", ", 0, "\n");
                  }
                  if (value.size() > 1) {
                    print_internal(value.back(), indent + indent_, "\n", level + 1);
                  }
                }
                if (value.size() == 0)
                  print_internal_without_quotes("]", indent, "");
                else if (is_container<T>::value == false)
                  print_internal_without_quotes("]", indent, "");
                else
                  print_internal_without_quotes(line_terminator_ + "]", indent, "");
                print_internal_without_quotes(line_terminator_, 0, "");
              }
              else {
                if (value.size() == 0) {
                  print_internal_without_quotes("[", indent, "");
                }
                else if (value.size() == 1) {
                  print_internal_without_quotes("[", indent, "");
                  print_internal(value.front(), 0, "", level + 1);
                }
                else if (value.size() > 0) {
                  print_internal_without_quotes("[", indent, "");
                  print_internal(value.front(), 0, "", level + 1);
                  if (value.size() > 1)
                    print_internal_without_quotes(", ", 0, "");
                  for (size_t i = 1; i < value.size() - 1; i++) {
                    print_internal(value[i], 0, "", level + 1);
                    print_internal_without_quotes(", ", 0, "");
                  }
                  if (value.size() > 1) {
                    print_internal(value.back(), 0, "", level + 1);
                  }
                }
                print_internal_without_quotes("]", 0, "");
                if (level == 0 && compact_)
                  print_internal_without_quotes(line_terminator_, 0, "");
              }

            }

            template<typename Container>
            typename std::enable_if<is_specialization<Container, std::list>::value ||
                                    is_specialization<Container, std::deque>::value,
                    void>::type print_internal(const Container &value, size_t indent = 0,
                                               const std::string &line_terminator = "\n",
                                               size_t level = 0) {
              typedef typename Container::value_type T;
              if (level == 0 && !compact_) {
                if (value.size() == 0) {
                  print_internal_without_quotes("[", 0, "");
                }
                else if (value.size() == 1) {
                  print_internal_without_quotes("[", 0, "");
                  print_internal(value.front(), 0, "", level + 1);
                }
                else if (value.size() > 0) {
                  print_internal_without_quotes("[", 0, "\n");
                  print_internal(value.front(), indent + indent_, "", level + 1);
                  if (value.size() > 1 && is_container<T>::value == false)
                    print_internal_without_quotes(", ", 0, "\n");
                  else if (is_container<T>::value)
                    print_internal_without_quotes(", ", 0, "\n");

                  typename Container::const_iterator iterator;
                  for (iterator = std::next(value.begin()); iterator != std::prev(value.end()); ++iterator) {
                    print_internal(*iterator, indent + indent_, "", level + 1);
                    if (is_container<T>::value == false)
                      print_internal_without_quotes(", ", 0, "\n");
                    else
                      print_internal_without_quotes(", ", 0, "\n");
                  }

                  if (value.size() > 1) {
                    print_internal(value.back(), indent + indent_, "\n", level + 1);
                  }
                }
                if (value.size() == 0)
                  print_internal_without_quotes("]", indent, "");
                else if (is_container<T>::value == false)
                  print_internal_without_quotes("]", indent, "");
                else
                  print_internal_without_quotes(line_terminator_ + "]", indent, "");
                print_internal_without_quotes(line_terminator_, 0, "");
              }
              else {
                if (value.size() == 0) {
                  print_internal_without_quotes("[", indent, "");
                }
                else if (value.size() == 1) {
                  print_internal_without_quotes("[", indent, "");
                  print_internal(value.front(), 0, "", level + 1);
                }
                else if (value.size() > 0) {
                  print_internal_without_quotes("[", indent, "");
                  print_internal(value.front(), 0, "", level + 1);
                  if (value.size() > 1)
                    print_internal_without_quotes(", ", 0, "");

                  typename Container::const_iterator iterator;
                  for (iterator = std::next(value.begin()); iterator != std::prev(value.end()); ++iterator) {
                    print_internal(*iterator, 0, "", level + 1);
                    print_internal_without_quotes(", ", 0, "");
                  }

                  if (value.size() > 1) {
                    print_internal(value.back(), 0, "", level + 1);
                  }
                }
                print_internal_without_quotes("]", 0, "");
                if (level == 0 && compact_)
                  print_internal_without_quotes(line_terminator_, 0, "");
              }

            }

            template<typename Container>
            typename std::enable_if<is_specialization<Container, std::set>::value ||
                                    is_specialization<Container, std::multiset>::value ||
                                    is_specialization<Container, std::unordered_set>::value ||
                                    is_specialization<Container, std::unordered_multiset>::value, void>::type
            print_internal(const Container &value, size_t indent = 0, const std::string &line_terminator = "\n",
                           size_t level = 0) {
              typedef typename Container::value_type T;
              if (level == 0 && !compact_) {
                if (value.size() == 0) {
                  print_internal_without_quotes("{", 0, "");
                }
                else if (value.size() == 1) {
                  print_internal_without_quotes("{", 0, "");
                  print_internal(*(value.begin()), 0, "", level + 1);
                }
                else {
                  print_internal_without_quotes("{", 0, "\n");
                  print_internal(*(value.begin()), indent + indent_, "", level + 1);
                  if (value.size() > 1 && is_container<T>::value == false)
                    print_internal_without_quotes(", ", 0, "\n");
                  else if (is_container<T>::value)
                    print_internal_without_quotes(", ", 0, "\n");

                  typename Container::const_iterator iterator;
                  for (iterator = std::next(value.begin());
                       (iterator != value.end()) && (std::next(iterator) != value.end()); ++iterator) {
                    print_internal(*iterator, indent + indent_, "", level + 1);
                    if (is_container<T>::value == false)
                      print_internal_without_quotes(", ", 0, "\n");
                    else
                      print_internal_without_quotes(", ", 0, "\n");
                  }

                  if (value.size() > 1) {
                    print_internal(*iterator, indent + indent_, "\n", level + 1);
                  }
                }
                if (value.size() == 0)
                  print_internal_without_quotes("}", indent, "");
                else if (is_container<T>::value == false)
                  print_internal_without_quotes("}", indent, "");
                else
                  print_internal_without_quotes(line_terminator_ + "}", indent, "");
                print_internal_without_quotes(line_terminator_, 0, "");
              }
              else {
                if (value.size() == 0) {
                  print_internal_without_quotes("{", indent, "");
                }
                else if (value.size() == 1) {
                  print_internal_without_quotes("{", indent, "");
                  print_internal(*(value.begin()), 0, "", level + 1);
                }
                else {
                  print_internal_without_quotes("{", indent, "");
                  print_internal(*(value.begin()), 0, "", level + 1);
                  if (value.size() > 1)
                    print_internal_without_quotes(", ", 0, "");

                  typename Container::const_iterator iterator;
                  for (iterator = std::next(value.begin());
                       (iterator != value.end()) && (std::next(iterator) != value.end()); ++iterator) {
                    print_internal(*iterator, 0, "", level + 1);
                    print_internal_without_quotes(", ", 0, "");
                  }

                  if (value.size() > 1) {
                    print_internal(*iterator, 0, "", level + 1);
                  }
                }
                print_internal_without_quotes("}", 0, "");
                if (level == 0 && compact_)
                  print_internal_without_quotes(line_terminator_, 0, "");
              }

            }

            template<typename T>
            typename std::enable_if<is_specialization<T, std::map>::value == true ||
                                    is_specialization<T, std::multimap>::value == true ||
                                    is_specialization<T, std::unordered_map>::value == true ||
                                    is_specialization<T, std::unordered_multimap>::value == true, void>::type
            print_internal(const T &value, size_t indent = 0, const std::string &line_terminator = "\n", size_t level = 0) {
              typedef typename T::mapped_type Value;
              if (level == 0 && !compact_) {
                if (value.size() == 0) {
                  print_internal_without_quotes("{", 0, "");
                }
                else if (value.size() == 1) {
                  print_internal_without_quotes("{", 0, "");
                  for (auto &kvpair : value) {
                    print_internal(kvpair.first, 0, "", level + 1);
                    print_internal_without_quotes(" : ", 0, "");
                    print_internal(kvpair.second, 0, "", level + 1);
                  }
                }
                else if (value.size() > 0) {
                  size_t count = 0;
                  for (auto &kvpair : value) {
                    if (count == 0) {
                      print_internal_without_quotes("{", 0, "\n");
                      print_internal(kvpair.first, indent + indent_, "", level + 1);
                      print_internal_without_quotes(" : ", 0, "");
                      print_internal(kvpair.second, 0, "", level + 1);
                      if (value.size() > 1 && is_container<Value>::value == false)
                        print_internal_without_quotes(", ", 0, "\n");
                      else if (is_container<Value>::value)
                        print_internal_without_quotes(", ", 0, "\n");
                    }
                    else if (count + 1 < value.size()) {
                      print_internal(kvpair.first, indent + indent_, "", level + 1);
                      print_internal_without_quotes(" : ", 0, "");
                      print_internal(kvpair.second, 0, "", level + 1);
                      if (is_container<Value>::value == false)
                        print_internal_without_quotes(", ", 0, "\n");
                      else
                        print_internal_without_quotes(", ", 0, "\n");
                    }
                    else {
                      print_internal(kvpair.first, indent + indent_, "", level + 1);
                      print_internal_without_quotes(" : ", 0, "");
                      print_internal(kvpair.second, 0, "\n", level + 1);
                    }
                    count += 1;
                  }
                }
                if (value.size() == 0)
                  print_internal_without_quotes("}", indent, "");
                else if (is_container<Value>::value == false)
                  print_internal_without_quotes("}", indent, "");
                else
                  print_internal_without_quotes(line_terminator_ + "}", indent, "");
                print_internal_without_quotes(line_terminator_, 0, "");
              }

              else {
                if (value.size() == 0) {
                  print_internal_without_quotes("{", indent, "");
                }
                else if (value.size() == 1) {
                  print_internal_without_quotes("{", indent, "");
                  for (auto &kvpair : value) {
                    print_internal(kvpair.first, 0, "", level + 1);
                    print_internal_without_quotes(" : ", 0, "");
                    print_internal(kvpair.second, 0, "", level + 1);
                  }
                }
                else if (value.size() > 0) {
                  size_t count = 0;
                  for (auto &kvpair : value) {
                    if (count == 0) {
                      print_internal_without_quotes("{", indent, "");
                      print_internal(kvpair.first, 0, "", level + 1);
                      print_internal_without_quotes(" : ", 0, "");
                      print_internal(kvpair.second, 0, "", level + 1);
                      print_internal_without_quotes(", ", 0, "");
                    }
                    else if (count + 1 < value.size()) {
                      print_internal(kvpair.first, indent + indent_, "", level + 1);
                      print_internal_without_quotes(" : ", 0, "");
                      print_internal(kvpair.second, 0, "", level + 1);
                      print_internal_without_quotes(", ", 0, "");
                    }
                    else {
                      print_internal(kvpair.first, 0, "", level + 1);
                      print_internal_without_quotes(" : ", 0, "");
                      print_internal(kvpair.second, 0, "", level + 1);
                    }
                    count += 1;
                  }
                }
                print_internal_without_quotes("}", 0, "");
                if (level == 0 && compact_)
                  print_internal_without_quotes(line_terminator_, 0, "");
              }
            }

            template<typename Key, typename Value>
            void print_internal(std::pair<Key, Value> value, size_t indent = 0, const std::string &line_terminator = "\n",
                                size_t level = 0) {
              print_internal_without_quotes("(", indent, "");
              print_internal(value.first, 0, "");
              print_internal_without_quotes(", ", 0, "");
              print_internal(value.second, 0, "");
              print_internal_without_quotes(")", 0, line_terminator, level);
            }

            template<class ...Ts>
            void print_internal(std::variant<Ts...> value, size_t indent = 0,
                                const std::string &line_terminator = "\n", size_t level = 0) {
              std::visit([=](const auto &value) { print_internal(value, indent, line_terminator, level); }, value);
            }

            template<typename T>
            void print_internal(std::optional<T> value, size_t indent = 0,
                                const std::string &line_terminator = "\n", size_t level = 0) {
              if (value) {
                print_internal(value.value(), indent, line_terminator, level);
              }
              else {
                print_internal_without_quotes("nullopt", indent, line_terminator, level);
              }
            }

            template<typename Container>
            typename std::enable_if<is_specialization<Container, std::queue>::value, void>::type
            print_internal(const Container &value, size_t indent = 0, const std::string &line_terminator = "\n",
                           size_t level = 0) {
              auto current_compact = compact_;
              compact_ = true;
              typedef typename Container::value_type T;
              auto local = value;
              std::vector<T> local_vector;
              while (!local.empty()) {
                local_vector.push_back(local.front());
                local.pop();
              }
              print_internal(local_vector, indent, line_terminator, level);
              compact_ = current_compact;
            }

            template<typename Container>
            typename std::enable_if<is_specialization<Container, std::priority_queue>::value, void>::type
            print_internal(const Container &value, size_t indent = 0, const std::string &line_terminator = "\n",
                           size_t level = 0) {
              auto current_compact = compact_;
              compact_ = true;
              typedef typename Container::value_type T;
              auto local = value;
              std::vector<T> local_vector;
              while (!local.empty()) {
                local_vector.push_back(local.top());
                local.pop();
              }
              print_internal(local_vector, indent, line_terminator, level);
              compact_ = current_compact;
            }

            template<typename T>
            void print_internal(std::initializer_list<T> value, size_t indent = 0,
                                const std::string &line_terminator = "\n", size_t level = 0) {
              std::multiset<T> local;
              for (const T &x : value) {
                local.insert(x);
              }
              print_internal(local, indent, line_terminator_, level);
            }

            template<typename Container>
            typename std::enable_if<is_specialization<Container, std::stack>::value, void>::type
            print_internal(const Container &value, size_t indent = 0, const std::string &line_terminator = "\n",
                           size_t level = 0) {
              bool current_compact = compact_;
              compact_ = false; // Need to print a stack like its a stack, i.e., vertical
              typedef typename Container::value_type T;
              auto local = value;
              std::vector<T> local_vector;
              while (!local.empty()) {
                local_vector.push_back(local.top());
                local.pop();
              }
              print_internal(local_vector, indent, line_terminator, level);
              compact_ = current_compact;
            }

            template<class... Args>
            void
            print_internal(const std::tuple<Args...> &value, size_t indent = 0, const std::string &line_terminator = "\n",
                           size_t level = 0) {
              stream_ << std::string(indent, ' ') << value
                      << line_terminator;
            }

            template<typename T>
            void print_internal(const std::complex<T> &value, size_t indent = 0, const std::string &line_terminator = "\n",
                                size_t level = 0) {
              stream_ << std::string(indent, ' ') << "(" <<
                      value.real() << " + " << value.imag() << "i)"
                      << line_terminator;
            }

            template<typename Pointer>
            typename std::enable_if<is_specialization<Pointer, std::unique_ptr>::value ||
                                    is_specialization<Pointer, std::shared_ptr>::value ||
                                    is_specialization<Pointer, std::weak_ptr>::value, void>::type
            print_internal(const Pointer &value, size_t indent = 0, const std::string &line_terminator = "\n",
                           size_t level = 0) {
              stream_ << std::string(indent, ' ') << "<" <<
                      type(value) << " at " << &value << ">"
                      << line_terminator;
            }

        };
    }

/*/---------------------------macros----------------------/*/
    using namespace __gnu_pbds;
    using namespace std;
    using ll = long long;
    using lf = long double;

    template<typename T = int>
    using v = vector<T>;

    template<typename T = int, typename fn = greater<>>
    using min_heap = priority_queue<T, v<T>, fn>;

    template<typename T = int, typename V = int, typename... Rest>
    using umap = unordered_map<T, V, Rest...>;

    template<typename T = int, typename... Rest>
    using uset = unordered_set<T, Rest...>;

    template<typename T = int, typename V = int, typename... Rest>
    using tup = tuple<T, V, Rest...>;

    const ll MOD = 1000000007;

#define endl '\n'
#define mt make_tuple
#define isOdd(x) ((x)&1)
#define isEven(x) (!((x)&1))
#define all(x) (x).begin(), (x).end()
#define allr(x) (x).rbegin(), (x).rend()

#define read(...)   \
    ll __VA_ARGS__; \
    __read(__VA_ARGS__)
#ifndef ONLINE_JUDGE
#define deb(...) cerr << "L(" << __LINE__ << ")*[" << #__VA_ARGS__ << "]: [", __write(__VA_ARGS__)
#else
#define deb(...)
#endif
#define __timeStart auto __start_time = chrono::high_resolution_clock::now()
#define __timeEnd                                                                             \
    auto __stop_time = chrono::high_resolution_clock::now();                                  \
    auto __duration = chrono::duration_cast<chrono::nanoseconds>(__stop_time - __start_time); \
    cerr << "Time taken : " << ((long double)__duration.count()) / ((long double)1e9) << "s " << endl
#define execTime(x)  \
    {                \
        __timeStart; \
        x;           \
        __timeEnd;   \
    }
/*/---------------------------macros----------------------/*/

/*/---------------------------STL overloaded I/O----------------------/*/
    template<class T>
    istream &operator>>(istream &is, v<T> &V);

    template<class L, class R>
    istream &operator>>(istream &is, pair<L, R> &P);

    template<class T, size_t S>
    istream &operator>>(istream &is, array<T, S> &A);

    template<class Head, class... Tail>
    tuple<Head, Tail...> tuple_read_impl(istream &is);

    template<class... Types>
    istream &operator>>(istream &is, tuple<Types...> &tup);

    template<class T>
    ostream &operator<<(ostream &os, const v<T> &V);

    template<class T>
    ostream &operator<<(ostream &os, const unordered_set<T> &S);

    template<class T>
    ostream &operator<<(ostream &os, const multiset<T> &S);

    template<class T>
    ostream &operator<<(ostream &os, const set<T> &S);

    template<class L, class R>
    ostream &operator<<(ostream &os, const pair<L, R> &P);

    template<class L, class R>
    ostream &operator<<(ostream &os, const map<L, R> &M);

    template<class L, class R>
    ostream &operator<<(ostream &os, const unordered_map<L, R> &M);

    template<class T>
    ostream &operator<<(ostream &os, const queue<T> &Q);

    template<class T>
    ostream &operator<<(ostream &os, const stack<T> &S);

    template<class T>
    ostream &operator<<(ostream &os, const forward_list<T> &A);

    template<class T>
    ostream &operator<<(ostream &os, const list<T> &A);

    template<class T, size_t S>
    ostream &operator<<(ostream &os, const array<T, S> &A);

    template<class... T>
    ostream &operator<<(ostream &os, const tuple<T...> &_tup);

    template<class T>
    istream &operator>>(istream &is, v<T> &V) {
      for (auto &e : V)
        is >> e;
      return is;
    }

    template<class L, class R>
    istream &operator>>(istream &is, pair<L, R> &P) {
      cin >> P.first >> P.second;
      return is;
    }

    template<class T, size_t S>
    istream &operator>>(istream &is, array<T, S> &A) {
      for (auto &a : A)
        is >> a;
      return is;
    }

    template<class T>
    ostream &operator<<(ostream &os, const v<T> &V) {
      pprint::PrettyPrinter print(os);
      print.print(V);
      return os;
    }

    template<class T>
    ostream &operator<<(ostream &os, const unordered_set<T> &S) {
      pprint::PrettyPrinter print(os);
      print.print(S);
      return os;
    }

    template<class T>
    ostream &operator<<(ostream &os, const multiset<T> &S) {
      pprint::PrettyPrinter print(os);
      print.print(S);
      return os;
    }

    template<class T>
    ostream &operator<<(ostream &os, const set<T> &S) {
      pprint::PrettyPrinter print(os);
      print.print(S);
      return os;
    }

    template<class L, class R>
    ostream &operator<<(ostream &os, const pair<L, R> &P) {
      pprint::PrettyPrinter print(os);
      print.print(P);
      return os;
    }

    template<class L, class R>
    ostream &operator<<(ostream &os, const map<L, R> &M) {
      pprint::PrettyPrinter print(os);
      print.print(M);
      return os;
    }

    template<class L, class R>
    ostream &operator<<(ostream &os, const unordered_map<L, R> &M) {
      pprint::PrettyPrinter print(os);
      print.print(M);
      return os;
    }

    template<class T, size_t S>
    ostream &operator<<(ostream &os, const array<T, S> &A) {
      pprint::PrettyPrinter print(os);
      print.print(A);
      return os;
    }

    template<class T>
    ostream &operator<<(ostream &os, const queue<T> &Q) {
      pprint::PrettyPrinter print(os);
      print.print(Q);
      return os;
    }

    template<class T>
    ostream &operator<<(ostream &os, const stack<T> &S) {
      pprint::PrettyPrinter print(os);
      print.print(S);
      return os;
    }

    template<class T>
    ostream &operator<<(ostream &os, const forward_list<T> &A) {
      pprint::PrettyPrinter print(os);
      print.print(A);
      return os;
    }

    template<class T>
    ostream &operator<<(ostream &os, const list<T> &A) {
      pprint::PrettyPrinter print(os);
      print.print(A);
      return os;
    }

    template<class... T>
    ostream &operator<<(ostream &os, const tuple<T...> &_tup) {
      pprint::PrettyPrinter print(os);
      print.print(_tup);
      return os;
    }

    template<class Head, class... Tail>
    tuple<Head, Tail...> tuple_read_impl(istream &is) {
      Head val;
      is >> val;
      if constexpr (sizeof...(Tail) == 0)  // this was the last tuple value
        return tuple{val};
      else
        return tuple_cat(tuple{val}, tuple_read_impl<Tail...>(is));
    }

    template<class... Types>
    istream &operator>>(istream &is, tuple<Types...> &tup) {
      tup = tuple_read_impl<Types...>(is);
      return is;
    }

/*/---------------------------STL overloaded I/O----------------------/*/

/*/---------------------------I/O Ports----------------------/*/


    void __write() {
      cerr << "]\n";
    }

    template<class T, class... V>
    void __write(const T &t, const V &...other) {
      cerr<<t;
      if (sizeof...(other))
        cerr << ", ";
      __write(other...);
    }

    template<class T>
    void __read(T &x) {
      cin >> x;
    }

    template<class T, class... V>
    void __read(T &t, V &...other) {
      __read(t);
      __read(other...);
    }
/*/---------------------------I/O Ports----------------------/*/

/*/---------------------------myFunctions----------------------/*/
    struct pair_hash {
        static int betterHash(uint64_t x) {
          x += 0x9e3779b97f4a7c15;
          x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
          x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
          return x ^ (x >> 31);
        }

        template<class T1, class T2>
        size_t operator()(const pair<T1, T2> &p) const {
          auto h1 = hash<T1>{}(p.first);
          auto h2 = hash<T2>{}(p.second);
          return betterHash(h1 * 1111111 + h2);
        }
    };

    ll power(ll x, ll y, ll mod = LLONG_MAX) {
      ll res = 1;
      x = x % mod;
      while (y > 0) {
        if (y & 1)
          res = (res * x) % mod;
        y = y >> 1;
        x = (x * x) % mod;
      }
      return res;
    }

    mt19937_64 __MT19337GEN__(chrono::steady_clock::now().time_since_epoch().count());

    inline int64_t random(ll l, ll r) {
      uniform_int_distribution<ll> generator(l, r);
      return generator(__MT19337GEN__);
    }

    inline int64_t random() {
      uniform_int_distribution<ll> generator(LLONG_MIN, LLONG_MAX);
      return generator(__MT19337GEN__);
    }

    void fastIO() {
      ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
      cout << fixed << setprecision(25), cerr << fixed << setprecision(10);
    }

    template<typename T, typename K>
    bool itHas(const T &container, const K &key) {
      return container.find(key) != container.end();
    }

/*/---------------------------myFunctions----------------------/*/
}  // namespace myLib

namespace std {
    namespace {
        template<class T>
        inline void hash_combine(std::size_t &seed, T const &v) {
          seed ^= hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }

        template<class Tuple, size_t Index = std::tuple_size<Tuple>::value - 1>
        struct HashValueImpl {
            static void apply(size_t &seed, Tuple const &tuple) {
              HashValueImpl<Tuple, Index - 1>::apply(seed, tuple);
              hash_combine(seed, get<Index>(tuple));
            }
        };

        template<class Tuple>
        struct HashValueImpl<Tuple, 0> {
            static void apply(size_t &seed, Tuple const &tuple) {
              hash_combine(seed, get<0>(tuple));
            }
        };
    }  // namespace

    template<typename... TT>
    struct hash<std::tuple<TT...>> {
        size_t
        operator()(std::tuple<TT...> const &tt) const {
          size_t seed = 0;
          HashValueImpl<std::tuple<TT...>>::apply(seed, tt);
          return seed;
        }
    };
}  // namespace std

using namespace myLib;
using namespace std;
