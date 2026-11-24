// include/beman/optional/optional.hpp                                -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BEMAN_OPTIONAL_OPTIONAL_HPP
#define BEMAN_OPTIONAL_OPTIONAL_HPP

#include <version>
#include <compare>
#include <concepts>
#if defined(__cpp_lib_format_ranges)
#include <format>
#endif
#include <functional>
#include <ranges>
#include <type_traits>
#include <utility>

#include <beman/optional/detail/iterator.hpp>

namespace beman::optional {

namespace detail {
template <typename T, typename U>
concept optional_eq_rel = requires(const T& t, const U& u) {
    { t == u } -> std::convertible_to<bool>;
};

template <typename T, typename U>
concept optional_ne_rel = requires(const T& t, const U& u) {
    { t != u } -> std::convertible_to<bool>;
};

template <typename T, typename U>
concept optional_lt_rel = requires(const T& t, const U& u) {
    { t < u } -> std::convertible_to<bool>;
};

template <typename T, typename U>
concept optional_gt_rel = requires(const T& t, const U& u) {
    { t > u } -> std::convertible_to<bool>;
};

template <typename T, typename U>
concept optional_le_rel = requires(const T& t, const U& u) {
    { t <= u } -> std::convertible_to<bool>;
};

template <typename T, typename U>
concept optional_ge_rel = requires(const T& t, const U& u) {
    { t >= u } -> std::convertible_to<bool>;
};

//! \omit
struct from_function_t {
    explicit from_function_t() = default;
};

//! \omit
inline constexpr from_function_t from_function{};
} // namespace detail

/**
 *
 * @brief Tag type for in-place construction
 */

//! \elsewhere
struct in_place_t {
    explicit in_place_t() = default;
};

/**
 * @brief Tag for in-place construction
 */

//! \elsewhere
inline constexpr in_place_t in_place{};

} // namespace beman::optional

namespace beman::optional {
// \rSec1[optional]{Optional objects}

// \rSec2[optional.general]{General}

//! Subclause \ref{optional} describes class template `optional` that represents
//! optional objects.
//! An \defn{optional object} is an
//! object that contains the storage for another object and manages the lifetime of
//! this contained object, if any. The contained object may be initialized after
//! the optional object has been initialized, and may be destroyed before the
//! optional object has been destroyed. The initialization state of the contained
//! object is tracked by the optional object.

// \rSec2[optional.syn]{Header \tcode{<optional>} synopsis}

// \ref{optional.optional}, class template \tcode{optional}
template <class T>
class optional; // partially freestanding

// \ref{optional.optional.ref}, partial specialization of \tcode{optional} for lvalue reference types
template <class T>
class optional<T&>; // partially freestanding
} // namespace beman::optional

// Since P3168R2: Give std::optional Range Support.
template <class T>
inline constexpr bool std::ranges::enable_view<beman::optional::optional<T>> = true;

// Iterators for optional<T&> have lifetimes that are not tied to the optional.
template <class T>
inline constexpr bool std::ranges::enable_borrowed_range<beman::optional::optional<T&>> = true;

// Since P3168R2: Give std::optional Range Support.
#if defined(__cpp_lib_format_ranges)
template <class T>
inline constexpr auto std::format_kind<beman::optional::optional<T>> = range_format::disabled;
#endif

namespace beman::optional {
namespace detail {
//! \omit
template <typename T>
inline constexpr bool is_optional = false;
//! \omit
template <typename T>
inline constexpr bool is_optional<optional<T>> = true;
} // namespace detail
/**
 * @brief Concept for checking if derived from optional.
 */
//! \expos
template <class T>
concept is_derived_from_optional = requires(const T& t) { []<class U>(const optional<U>&) {}(t); };

// \ref{optional.nullopt}, no-value state indicator
/**
 * @brief Tag type for empty optional construction
 */

//! \verbatim-synopsis
//! struct nullopt_t{@\seebelow@};
struct nullopt_t {
    /** @enum Tag
     * @brief Tag enum for nullopt_t construction
     */

    //! \omit
    enum class Tag { tag /**< the tag value for nullopt_t::Tag */ };

    /**
     * @brief Construct a new nullopt_t object
     * @arg Tag
     * @details constexpr for nullopt_t to be literal.
     */

    //! \elsewhere
    explicit constexpr nullopt_t(Tag) noexcept {}

  private:
    //! \omit
    friend constexpr bool operator==(nullopt_t, nullopt_t) noexcept { return true; }

    //! \omit
    friend constexpr std::strong_ordering operator<=>(nullopt_t, nullopt_t) noexcept {
        return std::strong_ordering::equivalent;
    }
};

/// Tag to disengage optional objects.

//! \at optional.nullopt
//! The struct `nullopt_t` is an empty class type used as a unique type to
//! indicate the state of not containing a value for `optional` objects.
//! In particular, `optional<T>` has a constructor with `nullopt_t` as a single
//! argument; this indicates that an optional object not containing a value
//! shall be constructed.
//!
//! Type `nullopt_t` does not have
//! a default constructor or an initializer-list constructor, and
//! is not an aggregate.
//! `nullopt_t` models \libconcept{copyable} and
//! `three_way_comparable<strong_ordering>`.
//! \verbatim-itemdecl
//! struct nullopt_t{@\seebelow@};
//! inline constexpr nullopt_t nullopt(@\unspec@);
inline constexpr nullopt_t nullopt{nullopt_t::Tag::tag};

// \ref{optional.bad.access}, class \tcode{bad_optional_access}
class bad_optional_access;

// \ref{optional.relops}, relational operators
template <class T, class U>
constexpr bool operator==(const optional<T>& x, const optional<U>& y)
    requires detail::optional_eq_rel<T, U>;
template <class T, class U>
constexpr bool operator!=(const optional<T>& x, const optional<U>& y)
    requires detail::optional_ne_rel<T, U>;
template <class T, class U>
constexpr bool operator<(const optional<T>& x, const optional<U>& y)
    requires detail::optional_lt_rel<T, U>;
template <class T, class U>
constexpr bool operator>(const optional<T>& x, const optional<U>& y)
    requires detail::optional_gt_rel<T, U>;
template <class T, class U>
constexpr bool operator<=(const optional<T>& x, const optional<U>& y)
    requires detail::optional_le_rel<T, U>;
template <class T, class U>
constexpr bool operator>=(const optional<T>& x, const optional<U>& y)
    requires detail::optional_ge_rel<T, U>;
template <class T, std::three_way_comparable_with<T> U>
constexpr std::compare_three_way_result_t<T, U> operator<=>(const optional<T>& x, const optional<U>& y);

// \ref{optional.nullops}, comparison with \tcode{nullopt}
template <class T>
constexpr bool operator==(const optional<T>& x, nullopt_t) noexcept;
template <class T>
constexpr std::strong_ordering operator<=>(const optional<T>& x, nullopt_t) noexcept;

// \ref{optional.comp.with.t}, comparison with \tcode{T}
template <class T, class U>
constexpr bool operator==(const optional<T>& x, const U& v)
    requires (!detail::is_optional<U>) && detail::optional_eq_rel<T, U>;

template <class T, class U>
constexpr bool operator==(const T& v, const optional<U>& x)
    requires (!detail::is_optional<T>) && detail::optional_eq_rel<T, U>;

template <class T, class U>
constexpr bool operator!=(const optional<T>& x, const U& v)
    requires (!detail::is_optional<U>) && detail::optional_ne_rel<T, U>;

template <class T, class U>
constexpr bool operator!=(const T& v, const optional<U>& x)
    requires (!detail::is_optional<T>) && detail::optional_ne_rel<T, U>;

template <class T, class U>
constexpr bool operator<(const optional<T>& x, const U& v)
    requires (!detail::is_optional<U>) && detail::optional_lt_rel<T, U>;

template <class T, class U>
constexpr bool operator<(const T& v, const optional<U>& x)
    requires (!detail::is_optional<T>) && detail::optional_lt_rel<T, U>;

template <class T, class U>
constexpr bool operator>(const optional<T>& x, const U& v)
    requires (!detail::is_optional<U>) && detail::optional_gt_rel<T, U>;

template <class T, class U>
constexpr bool operator>(const T& v, const optional<U>& x)
    requires (!detail::is_optional<T>) && detail::optional_gt_rel<T, U>;

template <class T, class U>
constexpr bool operator<=(const optional<T>& x, const U& v)
    requires (!detail::is_optional<U>) && detail::optional_le_rel<T, U>;

template <class T, class U>
constexpr bool operator<=(const T& v, const optional<U>& x)
    requires (!detail::is_optional<T>) && detail::optional_le_rel<T, U>;

template <class T, class U>
constexpr bool operator>=(const optional<T>& x, const U& v)
    requires (!detail::is_optional<U>) && detail::optional_ge_rel<T, U>;

template <class T, class U>
constexpr bool operator>=(const T& v, const optional<U>& x)
    requires (!detail::is_optional<T>) && detail::optional_ge_rel<T, U>;

template <class T, class U>
    requires (!is_derived_from_optional<U>) && std::three_way_comparable_with<T, U>
constexpr std::compare_three_way_result_t<T, U> operator<=>(const optional<T>& x, const U& v);

// \ref{optional.specalg}, specialized algorithms
/**
 * @brief Swap
 */
template <class T>
constexpr void swap(optional<T>& x, optional<T>& y) noexcept(noexcept(x.swap(y)))
    requires std::is_move_constructible_v<T> && std::is_swappable_v<T>;

/**
 * @brief Make an optional
 */
template <int = 0, class T>
constexpr optional<std::decay_t<T>>
make_optional(T&& v) noexcept(std::is_nothrow_constructible_v<optional<std::decay_t<T>>, T>)
    requires std::is_constructible_v<std::decay_t<T>, T>;

template <class T, class... Args>
constexpr optional<T> make_optional(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
    requires std::is_constructible_v<T, Args...>;

template <class T, class U, class... Args>
constexpr optional<T>
make_optional(std::initializer_list<U> il,
              Args&&... args) noexcept(std::is_nothrow_constructible_v<T, std::initializer_list<U>&, Args...>)
    requires std::is_constructible_v<T, std::initializer_list<U>&, Args...>;

// \ref{optional.hash}, hash support
/* We are not actually in ::std, don't declare beman::optional::hash.
template <class T>
struct hash;
template <class T>
struct hash<optional<T>>;
*/
/// END [optional.syn]

// \rSec2[optional.bad.access]{Class \tcode{bad_optional_access}}

/**
 * @brief Exception thrown when trying to access the value of an empty optional
 */

//! The class `bad_optional_access` defines the type of objects thrown as
//! exceptions to report the situation where an attempt is made to access the
//! value of an optional object that does not contain a value.
class bad_optional_access : public std::exception {
  public:
    /**
     * @brief Construct a new bad optional access object
     *
     */
    // \syn see \ref{exception} for the specification of the special member functions
    bad_optional_access() = default;

    /**
     * @brief Get the error message for bad optional access
     *
     * @return const char*
     */

    //! \at optional.bad.access
    //! \returns An \impldef{return value of `bad_optional_access::what`} \ntbs{},
    //! which during constant evaluation is encoded with
    //! the ordinary literal encoding\iref{lex.ccon}.
    const char* what() const noexcept { return "Optional has no value"; }
};

// \rSec2[optional.optional]{Class template \tcode{optional}}

namespace detail {
template <class T, class U>
concept enable_forward_value = !std::is_same_v<std::decay_t<U>, optional<T>> &&
                               !std::is_same_v<std::decay_t<U>, in_place_t> && std::is_constructible_v<T, U&&>;

//! \expos
//! The exposition-only variable template `converts-from-any-cvref`
//! is used by some constructors for `optional`.
template <class T, class W>
constexpr bool converts_from_any_cvref = std::disjunction_v<std::is_constructible<T, W&>,
                                                            std::is_convertible<W&, T>,
                                                            std::is_constructible<T, W>,
                                                            std::is_convertible<W, T>,
                                                            std::is_constructible<T, const W&>,
                                                            std::is_convertible<const W&, T>,
                                                            std::is_constructible<T, const W>,
                                                            std::is_convertible<const W, T>>;

template <class T, class U, class Other>
concept enable_from_other =
    !std::is_same_v<T, U> && std::is_constructible_v<T, Other> && !converts_from_any_cvref<T, optional<U>>;

template <class T, class U>
concept enable_assign_forward = !std::is_same_v<optional<T>, std::decay_t<U>> &&
                                !std::conjunction_v<std::is_scalar<T>, std::is_same<T, std::decay_t<U>>> &&
                                std::is_constructible_v<T, U> && std::is_assignable_v<T&, U>;

template <class T, class U, class Other>
concept enable_assign_from_other =
    std::is_constructible_v<T, Other> && std::is_assignable_v<T&, Other> &&
    !converts_from_any_cvref<T, optional<U>> && //
    !std::is_assignable_v<T&, optional<U>&> && !std::is_assignable_v<T&, optional<U>&&> &&
    !std::is_assignable_v<T&, const optional<U>&> && !std::is_assignable_v<T&, const optional<U>&&>;
} // namespace detail

// \rSec3[optional.optional.general]{General}

/**
 *
 * @brief Optional Objects
 */

//! An instance of `optional<T>` is said to
//! \defn{contain a value}
//! when and only when
//! its member `val` is active\iref{class.union.general};
//! `val` is referred to as its
//! \defn{contained value}.
//! An optional object's contained value
//! is nested within\iref{intro.object} the optional object.
//!
//! A type `X` is a
//! \defn{valid contained type} for `optional`
//! if `X` is an lvalue reference type or a complete non-array object type,
//! and `remove_cvref_t<X>` is a type other than `in_place_t` or `nullopt_t`.
//! If a specialization of `optional` is instantiated with a type `T`
//! that is not a valid contained type for `optional`, the program is ill-formed.
//! If `T` is an object type,
//! `T` shall meet the \oldconcept{Destructible} requirements (\tref{cpp17.destructible}).
//! \covers mandates !is_same_v<T, remove_cv_t<in_place_t>>
//! \covers mandates !is_same_v<remove_cv_t<T>, nullopt_t>
//! \covers mandates is_object_v<T>
//! \covers mandates !is_array_v<T>
template <class T>
class optional {
    static_assert((!std::is_same_v<T, std::remove_cv_t<in_place_t>>) &&
                  (!std::is_same_v<std::remove_cv_t<T>, nullopt_t>));
    static_assert(std::is_object_v<T> && !std::is_array_v<T>);

  public:
    /**
     * @brief Type alias for the value type contained in the optional.
     *
     */
    using value_type = T;

    /**
     * @brief Type alias for the iterator type of the optional.
     *
     * @details Since P3168R2: Give std::optional Range Support.
     */

    //! \impdef
    //! \at optional.iterators
    //! These types
    //! model \libconcept{contiguous_iterator}\iref{iterator.concept.contiguous},
    //! meet the \oldconcept{RandomAccessIterator} requirements\iref{random.access.iterators}, and
    //! meet the requirements for constexpr iterators\iref{iterator.requirements.general},
    //! with value type `remove_cv_t<T>`.
    //! The reference type is `T&` for `iterator` and
    //! `const T&` for `const_iterator`.
    //!
    //! All requirements on container iterators\iref{container.reqmts} apply to
    //! `optional::iterator` and `optional::const_iterator` as well.
    //!
    //! Any operation that initializes or destroys the contained value of an optional object invalidates all iterators
    //! into that object.
    using iterator = detail::contiguous_iterator<T, optional>; // \syn see~\ref{optional.iterators}

    /**
     * @brief Type alias for the const iterator type of the optional.
     *
     * @details Since P3168R2: Give std::optional Range Support.
     */

    //! \impdef
    //! \at optional.iterators
    //! \also
    using const_iterator = detail::contiguous_iterator<const T, optional>; // \syn see~\ref{optional.iterators}

    // \ref{optional.ctor}, constructors
    /**
     * @brief Default constructs an empty optional.
     *
     */
    constexpr optional() noexcept;

    /**
     * @brief Constructs an empty optional.
     *
     */
    constexpr optional(nullopt_t) noexcept;

    /**
     * @brief   Copy constructs the value from \p rhs if it has one.
     *
     */
    constexpr optional(const optional& rhs)
        requires std::is_copy_constructible_v<T> && (!std::is_trivially_copy_constructible_v<T>);
    /**
     * @brief Copy constructs the value from \p rhs if it has one.
     * @details Defaulted if T is trivially copy constructible.
     */

    //! \merge
    constexpr optional(const optional&)
        requires std::is_copy_constructible_v<T> && std::is_trivially_copy_constructible_v<T>
    = default;
    /**
     *  @brief Move constructs the value from \p rhs if it has one.
     */
    constexpr optional(optional&& rhs) noexcept(std::is_nothrow_move_constructible_v<T>)
        requires std::is_move_constructible_v<T> && (!std::is_trivially_move_constructible_v<T>);
    /**
     * @brief Move constructs the value from \p rhs if it has one.
     * @details Defaulted if T is trivially move constructible.
     */

    //! \merge
    constexpr optional(optional&&)
        requires std::is_move_constructible_v<T> && std::is_trivially_move_constructible_v<T>
    = default;

    /**
     *  @brief   Constructs the value in-place using the given arguments.
     *
     * @param args The arguments to use for in-place construction.
     */
    template <class... Args>
    constexpr explicit optional(in_place_t, Args&&... args)
        requires std::is_constructible_v<T, Args...>;

    /**
     *  @brief   Constructs the value in-place using the given arguments.
     *
     * @param il The initializer list to use for in-place construction.
     * @param args The arguments to use for in-place construction.
     */
    template <class U, class... Args>
    constexpr explicit optional(in_place_t, std::initializer_list<U> il, Args&&... args)
        requires std::is_constructible_v<T, std::initializer_list<U>&, Args&&...>;

    /**
     * @brief   Constructs the value from \p u, forwarding it if necessary.
     *
     * If \p u is convertible to \p T, this is an explicit constructor.
     */
    template <class U = T>
    constexpr explicit(!std::is_convertible_v<U, T>) optional(U&& v)
        requires detail::enable_forward_value<T, U>;

    /**
     * @brief   Constructs the value from \p rhs if it has one.
     *
     * @tparam U
     */
    template <class U>
    constexpr explicit(!std::is_convertible_v<U, T>) optional(const optional<U>& rhs)
        requires (detail::enable_from_other<T, U, const U&>);

    /**
     * @brief   Constructs the value from \p rhs if it has one.
     */
    template <class U>
    constexpr explicit(!std::is_convertible_v<U, T>) optional(optional<U>&& rhs)
        requires (detail::enable_from_other<T, U, U&&>);

    // \ref{optional.dtor}, destructor
    /**
     * @brief   Destructs the optional.
     *
     */

    //! \merge
    constexpr ~optional()
        requires std::is_trivially_destructible_v<T>
    = default;

    /**
     *  @brief   Destroys the optional and its value if it has one.
     */
    constexpr ~optional()
        requires (!std::is_trivially_destructible_v<T>);

    // \ref{optional.assign}, assignment
    /**
     * @brief   Resets the optional to an empty state.
     */
    constexpr optional& operator=(nullopt_t) noexcept;

    /**
     * @brief   Copy assigns the value from \p rhs if it has one.
     */
    constexpr optional& operator=(const optional& rhs)
        requires std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T> &&
                 (!std::is_trivially_copy_assignable_v<T>);

    /**
     * @brief Copy assigns the value from \p rhs if it has one.
     *
     * @return optional&
     */

    //! \merge
    constexpr optional& operator=(const optional&)
        requires std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T> &&
                     std::is_trivially_copy_constructible_v<T> && std::is_trivially_copy_assignable_v<T>
    = default;

    /**
     *  @brief   Move assigns the value from \p rhs if it has one.
     */
    constexpr optional& operator=(optional&& rhs) noexcept(std::is_nothrow_move_constructible_v<T>)
        requires std::is_move_constructible_v<T> && std::is_move_assignable_v<T> &&
                 (!std::is_trivially_move_assignable_v<T>);

    /**
     * @brief   Move assigns the value from \p rhs if it has one.
     */

    //! \merge
    constexpr optional& operator=(optional&&)
        requires std::is_move_constructible_v<T> && std::is_move_assignable_v<T> &&
                     std::is_trivially_move_constructible_v<T> && std::is_trivially_move_assignable_v<T>
    = default;

    /**
     * @brief  Assigns the contained value from \p u, destroying the old value if there
     *
     * @tparam U The type of the value to assign.
     * @param u The value to assign.
     * @return optional&
     */
    template <class U = T>
    constexpr optional& operator=(U&& v)
        requires detail::enable_assign_forward<T, U>;

    /**
     * @brief   Assigns the contained value from \p rhs if it has one, destroying the old value if there
     *
     */
    template <class U>
    constexpr optional& operator=(const optional<U>& rhs)
        requires (detail::enable_assign_from_other<T, U, const U&>);

    /**
     * @brief   Assigns the contained value from \p rhs if it has one, destroying the old value if there
     */
    template <class U>
    constexpr optional& operator=(optional<U>&& rhs)
        requires (detail::enable_assign_from_other<T, U, U>);
    /**
     * @brief Constructs the value in-place, destroying the current one if there
     *
     * @tparam Args
     * @param args The argument list to use in emplacement construction.
     * @return T&
     */
    template <class... Args>
    constexpr T& emplace(Args&&... args);
    /**
     * @brief Constructs the value in-place using the given arguments, destroying the current one if there
     *
     * @tparam U
     * @tparam Args
     * @param il The initializer list to use in emplacement construction.
     * @param args The argument list to use in emplacement construction.
     * @return T&
     */
    template <class U, class... Args>
    constexpr T& emplace(std::initializer_list<U> il, Args&&... args);

    // \ref{optional.swap}, swap
    /**
     * @brief Swaps this optional with the other.
     *
     * @param rhs The optional to swap with.
     */
    constexpr void swap(optional& rhs) noexcept(std::is_nothrow_move_constructible<T>::value &&
                                                std::is_nothrow_swappable<T>::value);

    // \ref{optional.iterators}, iterator support
    /**
     * @brief   Returns an iterator to the beginning of the optional.
     * @return iterator
     */
    constexpr iterator begin() noexcept;

    /**
     * @brief   Returns a const iterator to the beginning of the optional.
     * @return const_iterator
     */
    constexpr const_iterator begin() const noexcept;

    /**
     * @brief   Returns an iterator to the end of the optional.
     * @return iterator
     */
    constexpr iterator end() noexcept;

    /**
     * @brief   Returns a const iterator to the end of the optional.
     * @return const_iterator
     */
    constexpr const_iterator end() const noexcept;

    // \ref{optional.observe}, observers
    /**
     * @brief   Returns a pointer to the contained value.
     */
    constexpr const T* operator->() const;

    /**
     * @brief   Returns a pointer to the contained value.
     *
     * @return T*
     */
    constexpr T* operator->();
    /**
     * @brief Returns a reference to the contained value.
     *
     * @return T&
     */
    constexpr T& operator*() &;

    /**
     * @brief  Returns a reference to the contained value.
     *
     * @return const T&
     */
    constexpr const T& operator*() const&;

    /**
     * @brief   Returns a reference to the contained value.
     *
     * @return T&&
     */
    constexpr T&& operator*() &&;

    /**
     * @brief Converts the optional to a boolean indicating whether it has a value.
     *
     * @return bool
     */
    constexpr explicit operator bool() const noexcept;

    /**
     * @brief   Returns whether or not the optional has a value.
     *
     * @return bool
     */
    constexpr bool has_value() const noexcept;

    /**
     * @brief Returns a reference to the contained value.
     *
     * @return T&
     */
    constexpr T& value() &;

    /**
     * @brief Returns a reference to the contained value.
     *
     * @return const T&
     */
    constexpr const T& value() const&;

    /**
     * @brief Returns a reference to the contained value.
     *
     * @return T&&
     */
    constexpr T&& value() &&;

    /**
     * @brief Returns the contained value if there is one, otherwise returns `u`.
     *
     * @tparam U The type of the alternate value
     * @param u The value to return in the empty case
     * @return T
     */
    template <class U = std::remove_cv_t<T>>
    constexpr std::remove_cv_t<T> value_or(U&& v) const&;
    /**
     * @brief Returns the contained value if there is one, otherwise returns `u`.
     *
     * @tparam U The type of the alternate value
     * @param u The value to return in the empty case
     * @return T
     */
    template <class U = std::remove_cv_t<T>>
    constexpr std::remove_cv_t<T> value_or(U&& v) &&;

    // \ref{optional.monadic}, monadic operations

    /**
     * @brief Applies a function to the contained value if there is one.
     *
     * @tparam F The type of the invocable
     * @param f The invocable to apply to the contained value
     * @return auto
     *
     * @details
     * The return type is the same as `std::invoke_result_t<F, T&>`,
     * but wrapped in an optional. The function \p f must return an optional type.
     */
    template <class F>
    constexpr auto and_then(F&& f) &;

    /**
     * @brief Applies a function to the contained value if there is one.
     *
     * @tparam F The type of the invocable
     * @param f The invocable to apply to the contained value
     * @return auto
     *
     * @details
     * The return type is the same as `std::invoke_result_t<F, T&>`,
     * but wrapped in an optional. The function \p f must return an optional type.
     */
    template <class F>
    constexpr auto and_then(F&& f) &&;

    /**
     * @brief Applies a function to the contained value if there is one.
     *
     * @tparam F The type of the invocable
     * @param f The invocable to apply to the contained value
     * @return auto
     *
     * @details
     * The return type is the same as `std::invoke_result_t<F, T&>`,
     * but wrapped in an optional. The function \p f must return an optional type.
     */
    template <class F>
    constexpr auto and_then(F&& f) const&;

    /**
     * @brief Applies a function to the contained value if there is one.
     *
     * @tparam F The type of the invocable
     * @param f The invocable to apply to the contained value
     * @return auto
     *
     * @details
     * The return type is the same as `std::invoke_result_t<F, T&>`,
     * but wrapped in an optional. The function \p f must return an optional type.
     */
    template <class F>
    constexpr auto and_then(F&& f) const&&;

    /**
     * @brief Returns an optional containing the result of applying \p f to the
     * contained value, or an empty optional if there is no contained value.
     *
     * @tparam F
     * @param f An invocable to apply to the contained value.
     * @return optional
     */
    template <class F>
    constexpr auto transform(F&& f) &;

    /**
     * @brief Returns an optional containing the result of applying \p f to the
     * contained value, or an empty optional if there is no contained value.
     *
     * @tparam F
     * @param f An invocable to apply to the contained value.
     * @return auto
     */
    template <class F>
    constexpr auto transform(F&& f) &&;

    /**
     * @brief Returns an optional containing the result of applying \p f to the
     * contained value, or an empty optional if there is no contained value.
     *
     * @tparam F
     * @param f An invocable to apply to the contained value.
     * @return auto
     */
    template <class F>
    constexpr auto transform(F&& f) const&;

    /**
     * @brief Returns an optional containing the result of applying \p f to the
     * contained value, or an empty optional if there is no contained value.
     *
     * @tparam F
     * @param f An invocable to apply to the contained value.
     * @return auto
     */
    template <class F>
    constexpr auto transform(F&& f) const&&;

    /**
     * @brief Returns an optional containing the contained value if it has one, or the result of applying \p f to the
     * optional if it does not.
     *
     * @tparam F An invocable type returning an optional.
     * @param f The invocable to call if the optional is disengaged.
     * @return optional
     */
    template <class F>
    constexpr optional or_else(F&& f) const&
        requires (std::invocable<F> && std::copy_constructible<T>);

    /**
     * @brief Returns an optional containing the contained value if it has one, or
     * the result of calling \p f if it does not.
     *
     * @tparam F An invocable type returning an optional.
     * @param f The invocable to call if the optional is disengaged.
     * @return auto
     */
    template <class F>
    constexpr optional or_else(F&& f) &&
        requires (std::invocable<F> && std::move_constructible<T>);

    // \ref{optional.mod}, modifiers
    /**
     * @brief Resets the optional to an empty state, destroying the contained value if there is one.
     *
     */
    constexpr void reset() noexcept;

  private:
    //! \omit
    struct empty {};
    union {
        /**
         * @brief The empty state of the optional.
         *
         */

        //! \omit
        empty _{};

        /**
         * @brief The contained value of the optional.
         *
         */

        //! \expos(val)
        T value_;
    };
    //! \omit
    bool engaged_ = false;

    template <class... Args>
    constexpr void construct(Args&&... args) {
        std::construct_at(std::addressof(value_), std::forward<Args>(args)...);
        engaged_ = true;
    }

    constexpr void hard_reset() noexcept {
        std::destroy_at(std::addressof(value_));
        engaged_ = false;
    }

    //! \omit
    template <class U>
    friend class optional;

    //! \omit
    template <class F, class Arg>
    constexpr optional(detail::from_function_t, F&& f, Arg&& arg)
        : value_(std::invoke(std::forward<F>(f), std::forward<Arg>(arg))), engaged_(true) {}
};

// \rSec3[optional.ctor]{Constructors}

//! \ensures `*this` does not contain a value.
//! \remarks No contained value is initialized.
//! For every object type `T` these constructors are constexpr constructors\iref{dcl.constexpr}.
template <class T>
inline constexpr optional<T>::optional() noexcept : _(), engaged_(false) {}

//! \also
template <class T>
inline constexpr optional<T>::optional(nullopt_t) noexcept {}

//! \covers constraints is_copy_constructible_v<T>
//! \effects If `rhs` contains a value, direct-non-list-initializes `val`
//! with `rhs.val`.
//! \ensures `rhs.has_value() == this->has_value()`.
//! \throws Any exception thrown by the selected constructor of `T`.
//! \remarks This constructor is defined as deleted unless
//! `is_copy_constructible_v<T>` is `true`.
//! If `is_trivially_copy_constructible_v<T>` is `true`,
//! this constructor is trivial.
template <class T>
inline constexpr optional<T>::optional(const optional& rhs)
    requires std::is_copy_constructible_v<T> && (!std::is_trivially_copy_constructible_v<T>)
{
    if (rhs.has_value()) {
        construct(rhs.value_);
    }
}

//! \seebelow noexcept
//! \effects If `rhs` contains a value, direct-non-list-initializes `val`
//! with `std::move(rhs.val)`.
//! `rhs.has_value()` is unchanged.
//! \ensures `rhs.has_value() == this->has_value()`.
//! \throws Any exception thrown by the selected constructor of `T`.
//! \remarks The exception specification is equivalent to
//! `is_nothrow_move_constructible_v<T>`.
//! If `is_trivially_move_constructible_v<T>` is `true`,
//! this constructor is trivial.
template <class T>
inline constexpr optional<T>::optional(optional&& rhs) noexcept(std::is_nothrow_move_constructible_v<T>)
    requires std::is_move_constructible_v<T> && (!std::is_trivially_move_constructible_v<T>)
{
    if (rhs.has_value()) {
        construct(std::move(rhs.value_));
    }
}

//! \effects Direct-non-list-initializes `val`
//! with `std::forward<Args>(args)...`.
//! \ensures `*this` contains a value.
//! \throws Any exception thrown by the selected constructor of `T`.
//! \remarks If `T`'s constructor selected for the initialization is a constexpr constructor, this constructor is a
//! constexpr constructor.
template <class T>
template <class... Args>
inline constexpr optional<T>::optional(in_place_t, Args&&... args)
    requires std::is_constructible_v<T, Args...>
    : value_(std::forward<Args>(args)...), engaged_(true) {}

//! \effects Direct-non-list-initializes `val`
//! with `il, std::forward<Args>(args)...`.
//! \ensures `*this` contains a value.
//! \throws Any exception thrown by the selected constructor of `T`.
//! \remarks If `T`'s constructor selected for the initialization is a constexpr constructor, this constructor is a
//! constexpr constructor.
template <class T>
template <class U, class... Args>
inline constexpr optional<T>::optional(in_place_t, std::initializer_list<U> il, Args&&... args)
    requires std::is_constructible_v<T, std::initializer_list<U>&, Args&&...>
    : value_(il, std::forward<Args>(args)...), engaged_(true) {}

//! \seebelow explicit
//! \constraints
//! \item `is_constructible_v<T, U>` is `true`,
//! \item `is_same_v<remove_cvref_t<U>, in_place_t>` is `false`, and
//! \item `is_same_v<remove_cvref_t<U>, optional>` is `false`.
//! \effects Direct-non-list-initializes `val`
//! with `std::forward<U>(v)`.
//! \ensures `*this` contains a value.
//! \throws Any exception thrown by the selected constructor of `T`.
//! \remarks If `T`'s selected constructor is a constexpr constructor,
//! this constructor is a constexpr constructor.
//! The expression inside `explicit` is equivalent to:
//! \codeblock
//! !is_convertible_v<U, T>
//! \endcodeblock
template <class T>
template <class U>
inline constexpr optional<T>::optional(U&& v)
    requires detail::enable_forward_value<T, U>
    : optional(in_place, std::forward<U>(v)) {}

//! \seebelow explicit
//! \constraints
//! \item `is_same_v<T, U>` is `false`,
//! \item `is_constructible_v<T, const U&>` is `true`, and
//! \item `converts-from-any-cvref<T, optional<U>>` is `false`.
//! \effects If `rhs` contains a value,
//! direct-non-list-initializes `val`
//! with `rhs.operator*()`.
//! \ensures `rhs.has_value() == this->has_value()`.
//! \throws Any exception thrown by the selected constructor of `T`.
//! \remarks The expression inside `explicit` is equivalent to:
//! \codeblock
//! !is_convertible_v<U, T>
//! \endcodeblock
template <class T>
template <class U>
inline constexpr optional<T>::optional(const optional<U>& rhs)
    requires (detail::enable_from_other<T, U, const U&>)
{
    if (rhs.has_value()) {
        construct(*rhs);
    }
}

//! \seebelow explicit
//! \constraints
//! \item `is_same_v<T, U>` is `false`,
//! \item `is_constructible_v<T, U>` is `true`, and
//! \item `converts-from-any-cvref<T, optional<U>>` is `false`.
//! \effects If `rhs` contains a value,
//! direct-non-list-initializes `val`
//! with `std::move(rhs).operator*()`.
//! `rhs.has_value()` is unchanged.
//! \ensures `rhs.has_value() == this->has_value()`.
//! \throws Any exception thrown by the selected constructor of `T`.
//! \remarks The expression inside `explicit` is equivalent to:
//! \codeblock
//! !is_convertible_v<U, T>
//! \endcodeblock
template <class T>
template <class U>
inline constexpr optional<T>::optional(optional<U>&& rhs)
    requires (detail::enable_from_other<T, U, U&&>)
{
    if (rhs.has_value()) {
        construct(*std::move(rhs));
    }
}

// \rSec3[optional.dtor]{Destructor}

//! \effects If `is_trivially_destructible_v<T>` is `false`
//! and `*this` contains a value,
//! calls `val.T::~T()`.
//! \remarks If `is_trivially_destructible_v<T>` is `true`, then this destructor is trivial.
template <class T>
inline constexpr optional<T>::~optional()
    requires (!std::is_trivially_destructible_v<T>)
{
    if (has_value())
        std::destroy_at(std::addressof(value_));
}

// \rSec3[optional.assign]{Assignment}

//! \effects If `*this` contains a value, calls `val.T::~T()` to destroy the contained value; otherwise no effect.
//! \ensures `*this` does not contain a value.
//! \returns `*this`.
template <class T>
inline constexpr optional<T>& optional<T>::operator=(nullopt_t) noexcept {
    reset();
    return *this;
}

//! \covers constraints is_copy_constructible_v<T>
//! \covers constraints is_copy_assignable_v<T>
//! \effects See \tref{optional.assign.copy}.
//! \lib2dtab2[optional.assign.copy]{`optional::operator=(const optional&)` effects}
//! \column `*this` contains a value
//! \column `*this` does not contain a value
//! \row `rhs` contains a value
//! \cell assigns `rhs.val` to `val`
//! \cell direct-non-list-initializes `val` with `rhs.val`
//! \row `rhs` does not contain a value
//! \cell destroys the contained value by calling `val.T::~T()`
//! \cell no effect
//! \endlib2dtab2
//! \ensures `rhs.has_value() == this->has_value()`.
//! \returns `*this`.
//! \remarks If any exception is thrown, the result of the expression `this->has_value()` remains unchanged.
//! If an exception is thrown during the call to `T`'s copy constructor, no effect.
//! If an exception is thrown during the call to `T`'s copy assignment,
//! the state of its contained value is as defined by the exception safety guarantee of `T`'s copy assignment.
//! This operator is defined as deleted unless
//! `is_copy_constructible_v<T>` is `true` and
//! `is_copy_assignable_v<T>` is `true`.
//! If `is_trivially_copy_constructible_v<T> &&`
//! `is_trivially_copy_assignable_v<T> &&`
//! `is_trivially_destructible_v<T>` is `true`,
//! this assignment operator is trivial.
template <class T>
inline constexpr optional<T>& optional<T>::operator=(const optional<T>& rhs)
    requires std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T> &&
             (!std::is_trivially_copy_assignable_v<T>)
{
    if (!rhs.has_value())
        reset();
    else if (has_value())
        value_ = rhs.value_;
    else
        construct(rhs.value_);
    return *this;
}

//! \seebelow noexcept
//! \effects See \tref{optional.assign.move}.
//! The result of the expression `rhs.has_value()` remains unchanged.
//! \lib2dtab2[optional.assign.move]{`optional::operator=(optional&&)` effects}
//! \column `*this` contains a value
//! \column `*this` does not contain a value
//! \row `rhs` contains a value
//! \cell assigns `std::move(rhs.val)` to `val`
//! \cell direct-non-list-initializes `val` with `std::move(rhs.val)`
//! \row `rhs` does not contain a value
//! \cell destroys the contained value by calling `val.T::~T()`
//! \cell no effect
//! \endlib2dtab2
//! \ensures `rhs.has_value() == this->has_value()`.
//! \returns `*this`.
//! \remarks The exception specification is equivalent to:
//! \codeblock
//! is_nothrow_move_constructible_v<T>
//! \endcodeblock
//!
//!
//! If any exception is thrown, the result of the expression `this->has_value()` remains unchanged.
//! If an exception is thrown during the call to `T`'s move constructor,
//! the state of `rhs.val` is determined by the exception safety guarantee of `T`'s move constructor.
//! If an exception is thrown during the call to `T`'s move assignment,
//! the states of `val` and `rhs.val` are determined by the exception safety guarantee of `T`'s move assignment.
//! If `is_trivially_move_constructible_v<T> &&`
//! `is_trivially_move_assignable_v<T> &&`
//! `is_trivially_destructible_v<T>` is `true`,
//! this assignment operator is trivial.
template <class T>
inline constexpr optional<T>&
optional<T>::operator=(optional<T>&& rhs) noexcept(std::is_nothrow_move_constructible_v<T>)
    requires std::is_move_constructible_v<T> && std::is_move_assignable_v<T> &&
             (!std::is_trivially_move_assignable_v<T>)
{
    if (!rhs.has_value())
        reset();
    else if (has_value())
        value_ = std::move(rhs.value_);
    else
        construct(std::move(rhs.value_));
    return *this;
}

//! \constraints
//! \item `is_same_v<remove_cvref_t<U>, optional>` is `false`,
//! \item `conjunction_v<is_scalar<T>, is_same<T, decay_t<U>>>` is `false`,
//! \item `is_constructible_v<T, U>` is `true`, and
//! \item `is_assignable_v<T&, U>` is `true`.
//! \effects If `*this` contains a value,
//! assigns `std::forward<U>(v)` to `val`;
//! otherwise direct-non-list-initializes `val`
//! with `std::forward<U>(v)`.
//! \ensures `*this` contains a value.
//! \returns `*this`.
//! \remarks If any exception is thrown, the result of the expression `this->has_value()` remains unchanged. If an
//! exception is thrown during the call to `T`'s constructor, the state of `v` is determined by the exception safety
//! guarantee of `T`'s constructor. If an exception is thrown during the call to `T`'s assignment, the states of `val`
//! and `v` are determined by the exception safety guarantee of `T`'s assignment.
template <class T>
template <class U>
inline constexpr optional<T>& optional<T>::operator=(U&& v)
    requires detail::enable_assign_forward<T, U>
{
    if (has_value()) {
        value_ = std::forward<U>(v);
    } else {
        construct(std::forward<U>(v));
    }

    return *this;
}

//! \constraints
//! \item `is_constructible_v<T, const U&>` is `true`,
//! \item `is_assignable_v<T&, const U&>` is `true`,
//! \item `converts-from-any-cvref<T, optional<U>>` is `false`,
//! \item `is_assignable_v<T&, optional<U>&>` is `false`,
//! \item `is_assignable_v<T&, optional<U>&&>` is `false`,
//! \item `is_assignable_v<T&, const optional<U>&>` is `false`, and
//! \item `is_assignable_v<T&, const optional<U>&&>` is `false`.
//! \effects See \tref{optional.assign.copy.templ}.
//! \lib2dtab2[optional.assign.copy.templ]{`optional::operator=(const optional<U>&)` effects}
//! \column `*this` contains a value
//! \column `*this` does not contain a value
//! \row `rhs` contains a value
//! \cell assigns `rhs.operator*()` to `val`
//! \cell direct-non-list-initializes `val` with `rhs.operator*()`
//! \row `rhs` does not contain a value
//! \cell destroys the contained value by calling `val.T::~T()`
//! \cell no effect
//! \endlib2dtab2
//! \ensures `rhs.has_value() == this->has_value()`.
//! \returns `*this`.
//! \remarks If any exception is thrown,
//! the result of the expression `this->has_value()` remains unchanged.
//! If an exception is thrown during the call to `T`'s constructor,
//! the state of `rhs.val` is determined by
//! the exception safety guarantee of `T`'s constructor.
//! If an exception is thrown during the call to `T`'s assignment,
//! the states of `val` and `rhs.val` are determined by
//! the exception safety guarantee of `T`'s assignment.
template <class T>
template <class U>
inline constexpr optional<T>& optional<T>::operator=(const optional<U>& rhs)
    requires (detail::enable_assign_from_other<T, U, const U&>)
{
    if (has_value()) {
        if (rhs.has_value()) {
            value_ = *rhs;
        } else {
            hard_reset();
        }
    }

    else if (rhs.has_value()) {
        construct(*rhs);
    }

    return *this;
}

//! \constraints
//! \item `is_constructible_v<T, U>` is `true`,
//! \item `is_assignable_v<T&, U>` is `true`,
//! \item `converts-from-any-cvref<T, optional<U>>` is `false`,
//! \item `is_assignable_v<T&, optional<U>&>` is `false`,
//! \item `is_assignable_v<T&, optional<U>&&>` is `false`,
//! \item `is_assignable_v<T&, const optional<U>&>` is `false`, and
//! \item `is_assignable_v<T&, const optional<U>&&>` is `false`.
//! \effects See \tref{optional.assign.move.templ}.
//! The result of the expression `rhs.has_value()` remains unchanged.
//! \lib2dtab2[optional.assign.move.templ]{`optional::operator=(optional<U>&&)` effects}
//! \column `*this` contains a value
//! \column `*this` does not contain a value
//! \row `rhs` contains a value
//! \cell assigns `std::move(rhs).operator*()` to `val`
//! \cell direct-non-list-initializes `val` with `std::move(rhs).operator*()`
//! \row `rhs` does not contain a value
//! \cell destroys the contained value by calling `val.T::~T()`
//! \cell no effect
//! \endlib2dtab2
//! \ensures `rhs.has_value() == this->has_value()`.
//! \returns `*this`.
//! \remarks If any exception is thrown,
//! the result of the expression `this->has_value()` remains unchanged.
//! If an exception is thrown during the call to `T`'s constructor,
//! the state of `rhs.val` is determined by
//! the exception safety guarantee of `T`'s constructor.
//! If an exception is thrown during the call to `T`'s assignment,
//! the states of `val` and `rhs.val` are determined by
//! the exception safety guarantee of `T`'s assignment.
template <class T>
template <class U>
inline constexpr optional<T>& optional<T>::operator=(optional<U>&& rhs)
    requires (detail::enable_assign_from_other<T, U, U>)
{
    if (has_value()) {
        if (rhs.has_value()) {
            value_ = *std::move(rhs);
        } else {
            hard_reset();
        }
    }

    else if (rhs.has_value()) {
        construct(*std::move(rhs));
    }

    return *this;
}

//! \effects Calls `*this = nullopt`. Then direct-non-list-initializes `val`
//! with `std::forward<Args>(args)...`.
//! \ensures `*this` contains a value.
//! \returns `val`.
//! \throws Any exception thrown by the selected constructor of `T`.
//! \remarks If an exception is thrown during the call to `T`'s constructor,
//! `*this` does not contain a value, and
//! the previous `val` (if any) has been destroyed.
template <class T>
template <class... Args>
constexpr T& optional<T>::emplace(Args&&... args) {
    static_assert(std::is_constructible_v<T, Args&&...>);
    *this = nullopt;
    construct(std::forward<Args>(args)...);
    return value();
}

//! \effects Calls `*this = nullopt`. Then direct-non-list-initializes `val` with
//! `il, std::forward<Args>(args)...`.
//! \ensures `*this` contains a value.
//! \returns `val`.
//! \throws Any exception thrown by the selected constructor of `T`.
//! \remarks If an exception is thrown during the call to `T`'s constructor,
//! `*this` does not contain a value, and
//! the previous `val` (if any) has been destroyed.
template <class T>
template <class U, class... Args>
constexpr T& optional<T>::emplace(std::initializer_list<U> il, Args&&... args) {
    static_assert(std::is_constructible_v<T, std::initializer_list<U>&, Args&&...>);
    *this = nullopt;
    construct(il, std::forward<Args>(args)...);
    return value();
}

// \rSec3[optional.swap]{Swap}

//! \seebelow noexcept
//! \expects `T` meets the \oldconcept{Swappable} requirements\iref{swappable.requirements}.
//! \effects See \tref{optional.swap}.
//! \lib2dtab2[optional.swap]{`optional::swap(optional&)` effects}
//! \column `*this` contains a value
//! \column `*this` does not contain a value
//! \row `rhs` contains a value
//! \cell calls `swap(val, rhs.val)`
//! \cell direct-non-list-initializes `val` with `std::move(rhs.val)`, followed by `rhs.val.T::~T()`; postcondition is
//! that `*this` contains a value and `rhs` does not contain a value
//! \row `rhs` does not contain a value
//! \cell direct-non-list-initializes `rhs.val` with `std::move(val)`, followed by `val.T::~T()`; postcondition is that
//! `*this` does not contain a value and `rhs` contains a value
//! \cell no effect
//! \endlib2dtab2
//! \throws Any exceptions thrown by the operations in the relevant part of \tref{optional.swap}.
//! \remarks The exception specification is equivalent to:
//! \codeblock
//! is_nothrow_move_constructible_v<T> && is_nothrow_swappable_v<T>
//! \endcodeblock
//!
//!
//! If any exception is thrown, the results of the expressions `this->has_value()` and `rhs.has_value()` remain
//! unchanged. If an exception is thrown during the call to function `swap`, the states of `val` and `rhs.val` are
//! determined by the exception safety guarantee of `swap` for lvalues of `T`. If an exception is thrown during the
//! call to `T`'s move constructor, the states of `val` and `rhs.val` are determined by the exception safety guarantee
//! of `T`'s move constructor.
template <class T>
inline constexpr void optional<T>::swap(optional<T>& rhs) noexcept(std::is_nothrow_move_constructible<T>::value &&
                                                                   std::is_nothrow_swappable<T>::value) {
    static_assert(std::is_move_constructible_v<T>);
    using std::swap;
    if (has_value()) {
        if (rhs.has_value()) {
            swap(value(), *rhs);
        } else {
            std::construct_at(std::addressof(rhs.value_), std::move(value_));
            value_.T::~T();
        }
    } else if (rhs.has_value()) {
        std::construct_at(std::addressof(value_), std::move(rhs.value_));
        rhs.value_.T::~T();
    }
    swap(engaged_, rhs.engaged_);
}

// \rSec3[optional.iterators]{Iterator support}

// Since P3168R2: Give std::optional Range Support.

//! \returns If `has_value()` is `true`,
//! an iterator referring to the contained value.
//! Otherwise, a past-the-end iterator value.
template <class T>
inline constexpr optional<T>::iterator optional<T>::begin() noexcept {
    return iterator(has_value() ? std::addressof(value_) : nullptr);
}

//! \also
template <class T>
inline constexpr optional<T>::const_iterator optional<T>::begin() const noexcept {
    return const_iterator(has_value() ? std::addressof(value_) : nullptr);
}

//! \returns-expr
template <class T>
inline constexpr optional<T>::iterator optional<T>::end() noexcept {
    return begin() + has_value();
}

//! \also
template <class T>
inline constexpr optional<T>::const_iterator optional<T>::end() const noexcept {
    return begin() + has_value();
}

// \rSec3[optional.observe]{Observers}

//! \hardexpects `has_value()` is `true`.
//! \returns `addressof(val)`.
//! \remarks These functions are constexpr functions.
template <class T>
inline constexpr const T* optional<T>::operator->() const {
    return std::addressof(value_);
}

//! \also
template <class T>
inline constexpr T* optional<T>::operator->() {
    return std::addressof(value_);
}

//! \hardexpects `has_value()` is `true`.
//! \returns `val`.
//! \remarks These functions are constexpr functions.
template <class T>
inline constexpr T& optional<T>::operator*() & {
    return value_;
}

//! \also
template <class T>
inline constexpr const T& optional<T>::operator*() const& {
    return value_;
}

//! \hardexpects `has_value()` is `true`.
//! \effects-equiv
template <class T>
inline constexpr T&& optional<T>::operator*() && {
    return std::move(value_);
}

//! \returns `true` if and only if `*this` contains a value.
//! \remarks This function is a constexpr function.
template <class T>
inline constexpr optional<T>::operator bool() const noexcept {
    return engaged_;
}

//! \returns `true` if and only if `*this` contains a value.
//! \remarks This function is a constexpr function.
template <class T>
inline constexpr bool optional<T>::has_value() const noexcept {
    return engaged_;
}

//! \effects-equiv
template <class T>
inline constexpr T& optional<T>::value() & {
    return has_value() ? value_ : throw bad_optional_access();
}

//! \also
template <class T>
inline constexpr const T& optional<T>::value() const& {
    return has_value() ? value_ : throw bad_optional_access();
}

//! \effects-equiv
template <class T>
inline constexpr T&& optional<T>::value() && {
#if defined(_MSC_VER) // MSVC BUG -- ternary creates temporary
    if (has_value()) {
        return std::move(value_);
    }
    throw bad_optional_access();
#else
    return has_value() ? std::move(value_) : throw bad_optional_access();
#endif
}

//! \effects-equiv
template <class T>
template <class U>
inline constexpr std::remove_cv_t<T> optional<T>::value_or(U&& v) const& {
    using X = std::remove_cv_t<T>;
    static_assert(std::is_convertible_v<const T&, X>, "Must be able to convert const T& to remove_cv_t<T>");
    static_assert(std::is_convertible_v<U, X>, "Must be able to convert v to remove_cv_t<T>");
    if (has_value())
        return value_;
    return std::forward<U>(v);
}

//! \effects-equiv
template <class T>
template <class U>
inline constexpr std::remove_cv_t<T> optional<T>::value_or(U&& v) && {
    using X = std::remove_cv_t<T>;
    static_assert(std::is_convertible_v<T, X>, "Must be able to convert T to remove_cv_t<T>");
    static_assert(std::is_convertible_v<U, X>, "Must be able to convert v to remove_cv_t<T>");
    if (has_value()) {
        return std::move(value_);
    }
    return std::forward<U>(v);
}

// \rSec3[optional.monadic]{Monadic operations}

//! \group and-then-lvalue
//! Let `U` be `invoke_result_t<F, decltype((val))>`.
//! \mandates `remove_cvref_t<U>` is a specialization of `optional`.
//! \covers detail::is_optional<remove_cvref_t<U>>
//! \effects Equivalent to:
//! \codeblock
//! if (*this) {
//!   return invoke(std::forward<F>(f), @\exposid{val}@);
//! } else {
//!   return remove_cvref_t<U>();
//! }
//! \endcodeblock
template <class T>
template <class F>
constexpr auto optional<T>::and_then(F&& f) & {
    using U = std::invoke_result_t<F, T&>;
    static_assert(detail::is_optional<std::remove_cvref_t<U>>);
    if (has_value()) {
        return std::invoke(std::forward<F>(f), value_);
    } else {
        return std::remove_cvref_t<U>();
    }
}

//! \group and-then-rvalue
//! Let `U` be `invoke_result_t<F, decltype(std::move(val))>`.
//! \mandates `remove_cvref_t<U>` is a specialization of `optional`.
//! \covers detail::is_optional<remove_cvref_t<U>>
//! \effects Equivalent to:
//! \codeblock
//! if (*this) {
//!   return invoke(std::forward<F>(f), std::move(@\exposid{val}@));
//! } else {
//!   return remove_cvref_t<U>();
//! }
//! \endcodeblock
template <class T>
template <class F>
constexpr auto optional<T>::and_then(F&& f) && {
    using U = std::invoke_result_t<F, T&&>;
    static_assert(detail::is_optional<std::remove_cvref_t<U>>);
    if (has_value()) {
        return std::invoke(std::forward<F>(f), std::move(value_));
    } else {
        return std::remove_cvref_t<U>();
    }
}

//! \also and-then-lvalue
template <class T>
template <class F>
constexpr auto optional<T>::and_then(F&& f) const& {
    using U = std::invoke_result_t<F, const T&>;
    static_assert(detail::is_optional<std::remove_cvref_t<U>>);
    if (has_value()) {
        return std::invoke(std::forward<F>(f), value_);
    } else {
        return std::remove_cvref_t<U>();
    }
}

//! \also and-then-rvalue
template <class T>
template <class F>
constexpr auto optional<T>::and_then(F&& f) const&& {
    using U = std::invoke_result_t<F, const T&&>;
    static_assert(detail::is_optional<std::remove_cvref_t<U>>);
    if (has_value()) {
        return std::invoke(std::forward<F>(f), std::move(value_));
    } else {
        return std::remove_cvref_t<U>();
    }
}

//! \group transform-lvalue
//! Let `U` be `invoke_result_t<F, decltype((val))>`.
//! \mandates `U` is a valid contained type for `optional`.
//! \covers !is_array_v<U>
//! \covers !is_same_v<U, in_place_t>
//! \covers !is_same_v<U, nullopt_t>
//! \covers is_object_v<U> || is_reference_v<U>
//! The declaration
//! \codeblock
//! U u(invoke(std::forward<F>(f), @\exposid{val}@));
//! \endcodeblock
//! is well-formed for some invented variable `u`.
//! \note
//! There is no requirement that `U` is movable\iref{dcl.init.general}.
//! \endnote
//! \returns If `*this` contains a value, an `optional<U>` object
//! whose contained value is direct-non-list-initialized with
//! `invoke(std::forward<F>(f), val)`;
//! otherwise, `optional<U>()`.
template <class T>
template <class F>
constexpr auto optional<T>::transform(F&& f) & {
    using U = std::invoke_result_t<F, T&>;
    static_assert(!std::is_array_v<U>);
    static_assert(!std::is_same_v<U, in_place_t>);
    static_assert(!std::is_same_v<U, nullopt_t>);
    static_assert(std::is_object_v<U> || std::is_reference_v<U>); /// References now allowed
    return (has_value()) ? optional<U>{detail::from_function, std::forward<F>(f), value_} : optional<U>{};
}

//! \group transform-rvalue
//! Let `U` be
//! `invoke_result_t<F, decltype(std::move(val))>`.
//! \mandates `U` is a valid contained type for `optional`.
//! \covers !is_array_v<U>
//! \covers !is_same_v<U, in_place_t>
//! \covers !is_same_v<U, nullopt_t>
//! \covers is_object_v<U> || is_reference_v<U>
//! The declaration
//! \codeblock
//! U u(invoke(std::forward<F>(f), std::move(@\exposid{val}@)));
//! \endcodeblock
//! is well-formed for some invented variable `u`.
//! \note
//! There is no requirement that `U` is movable\iref{dcl.init.general}.
//! \endnote
//! \returns If `*this` contains a value, an `optional<U>` object
//! whose contained value is direct-non-list-initialized with
//! `invoke(std::forward<F>(f), std::move(val))`;
//! otherwise, `optional<U>()`.
template <class T>
template <class F>
constexpr auto optional<T>::transform(F&& f) && {
    using U = std::invoke_result_t<F, T&&>;
    static_assert(!std::is_array_v<U>);
    static_assert(!std::is_same_v<U, in_place_t>);
    static_assert(!std::is_same_v<U, nullopt_t>);
    static_assert(std::is_object_v<U> || std::is_reference_v<U>); /// References now allowed
    return (has_value()) ? optional<U>{detail::from_function, std::forward<F>(f), std::move(value_)} : optional<U>{};
}

//! \also transform-lvalue
template <class T>
template <class F>
constexpr auto optional<T>::transform(F&& f) const& {
    using U = std::invoke_result_t<F, const T&>;
    static_assert(!std::is_array_v<U>);
    static_assert(!std::is_same_v<U, in_place_t>);
    static_assert(!std::is_same_v<U, nullopt_t>);
    static_assert(std::is_object_v<U> || std::is_reference_v<U>); /// References now allowed
    return (has_value()) ? optional<U>{detail::from_function, std::forward<F>(f), value_} : optional<U>{};
}

//! \also transform-rvalue
template <class T>
template <class F>
constexpr auto optional<T>::transform(F&& f) const&& {
    using U = std::invoke_result_t<F, const T&>;
    static_assert(!std::is_array_v<U>);
    static_assert(!std::is_same_v<U, in_place_t>);
    static_assert(!std::is_same_v<U, nullopt_t>);
    static_assert(std::is_object_v<U> || std::is_reference_v<U>); /// References now allowed
    return (has_value()) ? optional<U>{detail::from_function, std::forward<F>(f), std::move(value_)} : optional<U>{};
}

/**
 * @brief Returns an optional containing the contained value if it has one, or the result of calling
 * \p `f` if it does not.
 *
 * @tparam T
 * @tparam F
 * @param f
 * @return optional<T>
 */

//! \effects-equiv
template <class T>
template <class F>
constexpr optional<T> optional<T>::or_else(F&& f) const&
    requires (std::invocable<F> && std::copy_constructible<T>)
{
    static_assert(std::is_same_v<std::remove_cvref_t<std::invoke_result_t<F>>, optional>);
    if (has_value())
        return value_;

    return std::forward<F>(f)();
}

/**
 * @brief Returns an optional containing the contained value if it has one, or the result of calling
 * \p `f` if it does not.
 *
 * @tparam T
 * @tparam F
 * @param f
 * @return optional<T>
 */

//! \effects-equiv
template <class T>
template <class F>
constexpr optional<T> optional<T>::or_else(F&& f) &&
    requires (std::invocable<F> && std::move_constructible<T>)
{
    static_assert(std::is_same_v<std::remove_cvref_t<std::invoke_result_t<F>>, optional>);
    if (has_value())
        return std::move(value_);

    return std::forward<F>(f)();
}

// \rSec3[optional.mod]{Modifiers}

//! \effects If `*this` contains a value, calls `val.T::~T()` to destroy the contained value;
//! otherwise no effect.
//! \ensures `*this` does not contain a value.
template <class T>
constexpr void optional<T>::reset() noexcept {
    if constexpr (!std::is_trivially_destructible_v<T>) {
        if (has_value())
            value_.~T();
    }
    engaged_ = false;
}

/****************/
/* optional<T&> */
/****************/

namespace detail {

#ifdef __cpp_lib_reference_from_temporary
using std::reference_constructs_from_temporary_v;
using std::reference_converts_from_temporary_v;
#else
template <class To, class From>
concept reference_converts_from_temporary_v =
    std::is_reference_v<To> &&
    (
        // A prvalue of a type similar to To, so that we're binding directly to
        // the materialized prvalue of type From
        (!std::is_reference_v<From> && std::is_convertible_v<std::remove_cvref_t<From>*, std::remove_cvref_t<To>*>) ||
        // A value of an unrelated type, convertible to To, but only by
        // materializing a To and binding a const reference; if we were trying
        // to bind a non-const reference, we'd be unable to. (This is not quite
        // exhaustive of the problem cases, but I think it's fairly close in
        // practice.)
        (std::is_lvalue_reference_v<To> && std::is_const_v<std::remove_reference_t<To>> &&
         std::is_convertible_v<From, const std::remove_cvref_t<To>&&> &&
         !std::is_convertible_v<From, std::remove_cvref_t<To>&>));

template <class To, class From>
concept reference_constructs_from_temporary_v =
    // This is close in practice, because cases where conversion and
    // construction differ in semantics are rare.
    reference_converts_from_temporary_v<To, From>;
#endif

} // namespace detail

// \rSec2[optional.optional.ref]{Partial specialization of \tcode{optional} for reference types}

// \rSec3[optional.optional.ref.general]{General}

/**
 * @brief A specialization of `optional` for references.
 */

//! An object of type `optional<T&>`
//! \defn{contains a value}
//! if and only if `val != nullptr` is `true`.
//! When an `optional<T&>` contains a value,
//! the \defn{contained value}
//! is a reference to `*val`.
//!
//! Each type `optional<T&>` is a trivially copyable class\iref{class.prop}.
template <class T>
class optional<T&> {
  public:
    /**
     * @brief The type of the value contained in the optional.
     *
     */
    using value_type = T;
    /**
     * @brief The type of the iterator for the optional.
     *
     */

    //! \impdef
    //! \at optional.ref.iterators
    //! This type
    //! models \libconcept{contiguous_iterator}\iref{iterator.concept.contiguous},
    //! meets the \oldconcept{RandomAccessIterator} requirements\iref{random.access.iterators},
    //! and meets the requirements for constexpr iterators\iref{iterator.requirements.general},
    //! with value type `remove_cv_t<T>`.
    //! The reference type is `T&` for `iterator`.
    //!
    //! All requirements on container iterators\iref{container.reqmts} apply to
    //! `optional::iterator`.
    using iterator = detail::contiguous_iterator<T, optional>; // \syn see~\ref{optional.ref.iterators}

    // \ref{optional.ref.ctor}, constructors

    /**
     * @brief Default constructor.
     */
    constexpr optional() noexcept = default;

    /**
     * @brief Constructs an empty optional.
     */

    //! \ensures `*this` does not contain a value.
    constexpr optional(nullopt_t) noexcept : optional() {}

    /**
     * @brief Copy constructor.
     *
     * Constructs an empty optional if the rhs is empty, otherwise constructs
     * the contained value from the rhs.
     */
    constexpr optional(const optional& rhs) noexcept = default;

    /**
     * @brief In-place constructor.
     *
     * @tparam Arg
     * @param arg The value to construct in-place from.
     */
    template <class Arg>
        requires (std::is_constructible_v<T&, Arg> && !detail::reference_constructs_from_temporary_v<T&, Arg>)
    constexpr explicit optional(in_place_t, Arg&& arg);

    /**
     * @brief Construct from a U
     * @tparam U
     * @param u The value to construct from.
     * @details
     * Constructs the contained value from `u` if it is convertible to `T&`.
     * If `T&` can be constructed from a temporary, this constructor is
     * deleted to prevent binding a temporary to a reference.
     */

    //! \constraints
    //! \covers !reference_constructs_from_temporary_v<T&, U>
    //! \item `is_same_v<remove_cvref_t<U>, optional>` is `false`,
    //! \item `is_same_v<remove_cvref_t<U>, in_place_t>` is `false`, and
    //! \item `is_constructible_v<T&, U>` is `true`.
    //! \effects Equivalent to: `convert-ref-init-val(std::forward<U>(u))`.
    //! \ensures `*this` contains a value.
    //! \remarks This constructor is defined as deleted if
    //! \codeblock
    //! reference_constructs_from_temporary_v<T&, U>
    //! \endcodeblock
    //! is `true`.
    template <class U>
        requires (std::is_constructible_v<T&, U> && !std::is_same_v<std::remove_cvref_t<U>, in_place_t> &&
                  !std::is_same_v<std::remove_cvref_t<U>, optional> &&
                  !detail::reference_constructs_from_temporary_v<T&, U>)
    constexpr explicit(!std::is_convertible_v<U, T&>)
        optional(U&& u) noexcept(std::is_nothrow_constructible_v<T&, U>) {
        convert_ref_init_val(u);
    }

    /**
     * @brief Constructs an optional from a U, but deletes the constructor if
     *
     * @tparam U
     */

    //! \merge
    template <class U>
        requires (std::is_constructible_v<T&, U> && !std::is_same_v<std::remove_cvref_t<U>, in_place_t> &&
                  !std::is_same_v<std::remove_cvref_t<U>, optional> &&
                  detail::reference_constructs_from_temporary_v<T&, U>)
    constexpr optional(U&& u) = delete;

    // The full set of 4 overloads on optional<U> by value category, doubled to
    // 8 by deleting if reference_constructs_from_temporary_v is true. This
    // allows correct constraints by propagating the value category from the
    // optional to the value within the rhs.

    /**
     * @brief Constructs an optional from another optional of type U.
     *
     * @tparam U
     * @param rhs The optional to construct from.
     * @details
     * Constructs the contained value from the rhs if it has a value, otherwise
     * constructs an empty optional.
     * If `T&` can be constructed from a temporary, this constructor is
     * deleted to prevent binding a temporary to a reference.
     */
    template <class U>
        requires (std::is_constructible_v<T&, U&> && !std::is_same_v<std::remove_cv_t<T>, optional<U>> &&
                  !std::is_same_v<T&, U> && !detail::reference_constructs_from_temporary_v<T&, U&>)
    constexpr explicit(!std::is_convertible_v<U&, T&>)
        optional(optional<U>& rhs) noexcept(std::is_nothrow_constructible_v<T&, U&>);

    /**
     * @brief Constructs an optional from another optional of type U.
     * @param rhs The optional to construct from.
     * @tparam U
     * @details
     * Constructs the contained value from the rhs if it has a value, otherwise
     * constructs an empty optional.
     * If `T&` can be constructed from a temporary, this constructor is
     * deleted to prevent binding a temporary to a reference.
     */
    template <class U>
        requires (std::is_constructible_v<T&, const U&> && !std::is_same_v<std::remove_cv_t<T>, optional<U>> &&
                  !std::is_same_v<T&, U> && !detail::reference_constructs_from_temporary_v<T&, const U&>)
    constexpr explicit(!std::is_convertible_v<const U&, T&>)
        optional(const optional<U>& rhs) noexcept(std::is_nothrow_constructible_v<T&, const U&>);

    /**
     * @brief Constructs an optional from another optional of type U.
     * @param rhs The optional to construct from.
     * @tparam U
     * @details
     * Constructs the contained value from the rhs if it has a value, otherwise
     * constructs an empty optional.
     * If `T&` can be constructed from a temporary, this constructor is
     * deleted to prevent binding a temporary to a reference.
     */
    template <class U>
        requires (std::is_constructible_v<T&, U> && !std::is_same_v<std::remove_cv_t<T>, optional<U>> &&
                  !std::is_same_v<T&, U> && !detail::reference_constructs_from_temporary_v<T&, U>)
    constexpr explicit(!std::is_convertible_v<U, T&>)
        optional(optional<U>&& rhs) noexcept(noexcept(std::is_nothrow_constructible_v<T&, U>));

    /**
     * @brief Constructs an optional from another optional of type U.
     *
     * @tparam U The type contained the right hand side optional.
     * @param rhs The optional to construct from.
     * @details
     * Constructs the contained value from the rhs if it has a value, otherwise
     * constructs an empty optional.
     * If `T&` can be constructed from a temporary, this constructor is
     * deleted to prevent binding a temporary to a reference.
     */
    template <class U>
        requires (std::is_constructible_v<T&, const U> && !std::is_same_v<std::remove_cv_t<T>, optional<U>> &&
                  !std::is_same_v<T&, U> && !detail::reference_constructs_from_temporary_v<T&, const U>)
    constexpr explicit(!std::is_convertible_v<const U, T&>)
        optional(const optional<U>&& rhs) noexcept(noexcept(std::is_nothrow_constructible_v<T&, const U>));

    /**
     * @brief Constructs an optional from another optional of type U
     *
     * @tparam U
     * @param rhs
     * @details
     * Constructs the contained value from the rhs if it has a value, otherwise
     * constructs an empty optional.
     * If `T&` can be constructed from a temporary, this constructor is
     * deleted to prevent binding a temporary to a reference.
     */

    //! \merge
    template <class U>
        requires (std::is_constructible_v<T&, U&> && !std::is_same_v<std::remove_cv_t<T>, optional<U>> &&
                  !std::is_same_v<T&, U> && detail::reference_constructs_from_temporary_v<T&, U&>)
    constexpr optional(optional<U>& rhs) = delete;

    /**
     * @brief Constructs an optional from another optional of type U
     *
     * @tparam U
     * @param rhs
     * @details
     * Constructs the contained value from the rhs if it has a value, otherwise
     * constructs an empty optional.
     * If `T&` can be constructed from a temporary, this constructor is
     * deleted to prevent binding a temporary to a reference.
     */

    //! \merge
    template <class U>
        requires (std::is_constructible_v<T&, const U&> && !std::is_same_v<std::remove_cv_t<T>, optional<U>> &&
                  !std::is_same_v<T&, U> && detail::reference_constructs_from_temporary_v<T&, const U&>)
    constexpr optional(const optional<U>& rhs) = delete;

    /**
     * @brief Constructs an optional from another optional of type U
     *
     * @tparam U
     * @param rhs
     * @details
     * Constructs the contained value from the rhs if it has a value, otherwise
     * constructs an empty optional.
     * If `T&` can be constructed from a temporary, this constructor is
     * deleted to prevent binding a temporary to a reference.
     */

    //! \merge
    template <class U>
        requires (std::is_constructible_v<T&, U> && !std::is_same_v<std::remove_cv_t<T>, optional<U>> &&
                  !std::is_same_v<T&, U> && detail::reference_constructs_from_temporary_v<T&, U>)
    constexpr optional(optional<U>&& rhs) = delete;

    /**
     * @brief Constructs an optional from another optional of type U
     *
     * @tparam U
     * @param rhs
     * @details
     * Constructs the contained value from the rhs if it has a value, otherwise
     * constructs an empty optional.
     * If `T&` can be constructed from a temporary, this constructor is
     * deleted to prevent binding a temporary to a reference.
     */

    //! \merge
    template <class U>
        requires (std::is_constructible_v<T&, const U> && !std::is_same_v<std::remove_cv_t<T>, optional<U>> &&
                  !std::is_same_v<T&, U> && detail::reference_constructs_from_temporary_v<T&, const U>)
    constexpr optional(const optional<U>&& rhs) = delete;

    /**
     * @brief Destructor.
     *
     * @details
     * Does not destroy the contained value, as it is a reference.
     */
    constexpr ~optional() = default;

    // \ref{optional.ref.assign}, assignment
    /**
     * @brief Nullopt assignment operator.
     *
     * @return optional&
     *
     * @details
     * Destroys the current value if there is one, and leaves the optional
     * in an empty state.
     */
    constexpr optional& operator=(nullopt_t) noexcept;

    /**
     * @brief Assignment operator.
     *
     * @param rhs The value to assign.
     * @return optional&
     * @details
     * If `rhs` has a value, assigns it to the contained value. Otherwise resets
     * the contained value in `*this`.
     */
    constexpr optional& operator=(const optional& rhs) noexcept = default;

    /**
     * @brief Emplaces a new value in the optional, destroying the current one
     * if the optional is engaged.
     *
     * @tparam U The type of the value to emplace.
     * @param u The value to emplace.
     * @return T&
     * @details
     * Constructs the contained value from `u` if it is convertible to `T&`.
     */
    template <class U>
        requires (std::is_constructible_v<T&, U> && !detail::reference_constructs_from_temporary_v<T&, U>)
    constexpr T& emplace(U&& u) noexcept(std::is_nothrow_constructible_v<T&, U>);

    // \ref{optional.ref.swap}, swap
    /**
     * @brief Swaps the contents of this optional with another.
     *
     * @param rhs The value to swap with.
     */
    constexpr void swap(optional& rhs) noexcept;

    // \ref{optional.ref.iterators}, iterator support
    /**
     * @brief Returns an iterator to the beginning of the optional.
     *
     * @return iterator
     */
    constexpr iterator begin() const noexcept;

    /**
     * @brief Returns an iterator to the end of the optional.
     *
     * @return iterator
     */
    constexpr iterator end() const noexcept;

    // \ref{optional.ref.observe}, observers
    /**
     * @brief Returns a pointer to the contained value.
     *
     * @return T*
     */
    constexpr T* operator->() const noexcept;

    /**
     * @brief Returns a reference to the contained value.
     *
     * @return T&
     */
    constexpr T& operator*() const noexcept;

    /**
     * @brief Converts the optional to a boolean value.
     *
     * @return bool
     */
    constexpr explicit operator bool() const noexcept;

    /**
     * @brief Checks if the optional has a value.
     *
     * @return bool
     */
    constexpr bool has_value() const noexcept;

    /**
     * @brief Returns the contained value if there is one, otherwise throws
     *
     * @return T&
     */
    constexpr T& value() const;

    // LWG4304. std::optional<NonReturnable&> is ill-formed due to value_o
    // Resolution:
    // -?- Constraints: T is a non-array object type.
    // -?- Remarks: The return type is unspecified if T is an array type or a
    //     non-object type. [Note ?: This is to avoid the declaration being
    //     ill-formed. — end note]
    //
    // Implementer Note: Using decay_t as a detail as it is remove_cv_t for
    // non-array objects, and produces a valid type for arrays and functions,
    // which are otherwise `required` out.
    /**
     * @brief Returns the contained value if there is one, otherwise returns `u`.
     *
     * @tparam U The type of the alternate value
     * @param u The value to return in the empty case
     * @return std::remove_cv_t<T>
     */
    template <class U = std::remove_cv_t<T>>
        requires (std::is_object_v<T> && !std::is_array_v<T>)
    constexpr std::decay_t<T> value_or(U&& u) const;

    // \ref{optional.ref.monadic}, monadic operations
    /**
     * @brief Applies a function to the contained value if there is one.
     *
     * @tparam F The type of the invocable
     * @param f The invocable to apply to the contained value
     * @return auto
     *
     * @details
     * The return type is the same as `std::invoke_result_t<F, T&>`,
     * but wrapped in an optional. The function \p f must return an optional type.
     */
    template <class F>
    constexpr auto and_then(F&& f) const;

    /**
     * @brief Applies a function to the contained value if there is one.
     *
     * @tparam F The type of the invocable
     * @param f The invocable to apply to the contained value
     * @return optional<std::invoke_result_t<F, T&>>
     *
     * @details
     * The return type is the same as `std::invoke_result_t<F, T&>`,
     * but wrapped in an optional.
     */
    template <class F>
    constexpr optional<std::invoke_result_t<F, T&>> transform(F&& f) const;

    /**
     * @brief Calls a function if the optional is empty.
     *
     * @tparam F The type of the invocable
     * @param f The invocable to apply to the contained value
     * @return optional
     * @details
     * The return type is the same as the return type of \p f, which must
     * return an optional type.
     */
    template <class F>
    constexpr optional or_else(F&& f) const
        requires (std::invocable<F>);

    // \ref{optional.ref.mod}, modifiers
    /**
     * @brief Resets the optional to an empty state.
     *
     */
    constexpr void reset() noexcept;

  private:
    //! \expos(val)
    T* value_ = nullptr; // exposition only

    // \ref{optional.ref.expos}, exposition only helper functions
    //! \expos
    //! \effects Creates a variable `r` as if by `T& r(std::forward<U>(u));`
    //! and then initializes `val` with `addressof(r)`.
    template <class U>
    constexpr void convert_ref_init_val(U&& u) {
        T& r(std::forward<U>(u));
        value_ = std::addressof(r);
    }

    //! \omit
    template <class U>
    friend class optional;

    //! \omit
    template <class F, class Arg>
    constexpr optional(detail::from_function_t, F&& f, Arg&& arg) {
        convert_ref_init_val(std::invoke(std::forward<F>(f), std::forward<Arg>(arg)));
    }
};

// \rSec3[optional.ref.ctor]{Constructors}

//! \constraints
//! \item `is_constructible_v<T&, Arg>` is `true`, and
//! \item `reference_constructs_from_temporary_v<T&, Arg>` is `false`.
//! \effects Equivalent to: `convert-ref-init-val(std::forward<Arg>(arg))`.
//! \ensures `*this` contains a value.
template <class T>
template <class Arg>
    requires (std::is_constructible_v<T&, Arg> && !detail::reference_constructs_from_temporary_v<T&, Arg>)
constexpr optional<T&>::optional(in_place_t, Arg&& arg) {
    convert_ref_init_val(std::forward<Arg>(arg));
}

// Clang is unhappy with the out-of-line definition
//
// template <class T>
// template <class U>
//     requires(std::is_constructible_v<T&, U> && !(is_same_v<remove_cvref_t<U>, in_place_t>) &&
//              !(is_same_v<remove_cvref_t<U>, optional<T&>>) && !detail::reference_constructs_from_temporary_v<T&, U>)
// constexpr optional<T&>::optional(U&& u) noexcept(is_nothrow_constructible_v<T&, U>)
//     : value_(std::addressof(static_cast<T&>(std::forward<U>(u)))) {}

//! \constraints
//! \covers !reference_constructs_from_temporary_v<T&, U&>
//! \item `is_same_v<remove_cv_t<T>, optional<U>>` is `false`,
//! \item `is_same_v<T&, U>` is `false`, and
//! \item `is_constructible_v<T&, U&>` is `true`.
//! \effects Equivalent to:
//! \codeblock
//! if (rhs.has_value()) @\exposid{convert-ref-init-val}@(rhs.operator*());
//! \endcodeblock
//! \remarks This constructor is defined as deleted if
//! \codeblock
//! reference_constructs_from_temporary_v<T&, U&>
//! \endcodeblock
//! is `true`.
template <class T>
template <class U>
    requires (std::is_constructible_v<T&, U&> && !std::is_same_v<std::remove_cv_t<T>, optional<U>> &&
              !std::is_same_v<T&, U> && !detail::reference_constructs_from_temporary_v<T&, U&>)
constexpr optional<T&>::optional(optional<U>& rhs) noexcept(std::is_nothrow_constructible_v<T&, U&>) {
    if (rhs.has_value()) {
        convert_ref_init_val(*rhs);
    }
}

//! \constraints
//! \covers !reference_constructs_from_temporary_v<T&, const U&>
//! \item `is_same_v<remove_cv_t<T>, optional<U>>` is `false`,
//! \item `is_same_v<T&, U>` is `false`, and
//! \item `is_constructible_v<T&, const U&>` is `true`.
//! \effects Equivalent to:
//! \codeblock
//! if (rhs.has_value()) @\exposid{convert-ref-init-val}@(rhs.operator*());
//! \endcodeblock
//! \remarks This constructor is defined as deleted if
//! \codeblock
//! reference_constructs_from_temporary_v<T&, const U&>
//! \endcodeblock
//! is `true`.
template <class T>
template <class U>
    requires (std::is_constructible_v<T&, const U&> && !std::is_same_v<std::remove_cv_t<T>, optional<U>> &&
              !std::is_same_v<T&, U> && !detail::reference_constructs_from_temporary_v<T&, const U&>)
constexpr optional<T&>::optional(const optional<U>& rhs) noexcept(std::is_nothrow_constructible_v<T&, const U&>) {
    if (rhs.has_value()) {
        convert_ref_init_val(*rhs);
    }
}

//! \constraints
//! \covers !reference_constructs_from_temporary_v<T&, U>
//! \item `is_same_v<remove_cv_t<T>, optional<U>>` is `false`,
//! \item `is_same_v<T&, U>` is `false`, and
//! \item `is_constructible_v<T&, U>` is `true`.
//! \effects Equivalent to:
//! \codeblock
//! if (rhs.has_value()) @\exposid{convert-ref-init-val}@(std::move(rhs).operator*());
//! \endcodeblock
//! \remarks This constructor is defined as deleted if
//! \codeblock
//! reference_constructs_from_temporary_v<T&, U>
//! \endcodeblock
//! is `true`.
template <class T>
template <class U>
    requires (std::is_constructible_v<T&, U> && !std::is_same_v<std::remove_cv_t<T>, optional<U>> &&
              !std::is_same_v<T&, U> && !detail::reference_constructs_from_temporary_v<T&, U>)
constexpr optional<T&>::optional(optional<U>&& rhs) noexcept(noexcept(std::is_nothrow_constructible_v<T&, U>)) {
    if (rhs.has_value()) {
        convert_ref_init_val(*std::move(rhs));
    }
}

//! \constraints
//! \covers !reference_constructs_from_temporary_v<T&, const U>
//! \item `is_same_v<remove_cv_t<T>, optional<U>>` is `false`,
//! \item `is_same_v<T&, U>` is `false`, and
//! \item `is_constructible_v<T&, const U>` is `true`.
//! \effects Equivalent to:
//! \codeblock
//! if (rhs.has_value()) @\exposid{convert-ref-init-val}@(std::move(rhs).operator*());
//! \endcodeblock
//! \remarks This constructor is defined as deleted if
//! \codeblock
//! reference_constructs_from_temporary_v<T&, const U>
//! \endcodeblock
//! is `true`.
template <class T>
template <class U>
    requires (std::is_constructible_v<T&, const U> && !std::is_same_v<std::remove_cv_t<T>, optional<U>> &&
              !std::is_same_v<T&, U> && !detail::reference_constructs_from_temporary_v<T&, const U>)
constexpr optional<T&>::optional(const optional<U>&& rhs) noexcept(
    noexcept(std::is_nothrow_constructible_v<T&, const U>)) {
    if (rhs.has_value()) {
        convert_ref_init_val(*std::move(rhs));
    }
}

// \rSec3[optional.ref.assign]{Assignment}

//! \effects Assigns `nullptr` to `val`.
//! \ensures `*this` does not contain a value.
//! \returns `*this`.
template <class T>
constexpr optional<T&>& optional<T&>::operator=(nullopt_t) noexcept {
    value_ = nullptr;
    return *this;
}

//! \constraints
//! \item `is_constructible_v<T&, U>` is `true`, and
//! \item `reference_constructs_from_temporary_v<T&, U>` is `false`.
//! \effects Equivalent to: `convert-ref-init-val(std::forward<U>(u))`.
//! \returns `*val`.
template <class T>
template <class U>
    requires (std::is_constructible_v<T&, U> && !detail::reference_constructs_from_temporary_v<T&, U>)
constexpr T& optional<T&>::emplace(U&& u) noexcept(std::is_nothrow_constructible_v<T&, U>) {
    convert_ref_init_val(std::forward<U>(u));
    return *value_;
}

// \rSec3[optional.ref.swap]{Swap}

//! \effects Equivalent to: `std::swap(val, rhs.val)`.
template <class T>
constexpr void optional<T&>::swap(optional<T&>& rhs) noexcept {
    std::swap(value_, rhs.value_);
}

// \rSec3[optional.ref.iterators]{Iterator support}

//! \returns An object `i` of type `iterator`, such that
//! `i` is an iterator referring to `*val`
//! if `has_value()` is `true`, and
//! a past-the-end iterator value otherwise.
template <class T>
constexpr optional<T&>::iterator optional<T&>::begin() const noexcept {
    return iterator(has_value() ? value_ : nullptr);
};

//! \returns `begin() + has_value()`.
template <class T>
constexpr optional<T&>::iterator optional<T&>::end() const noexcept {
    return begin() + has_value();
}

// \rSec3[optional.ref.observe]{Observers}

//! \hardexpects `has_value()` is `true`.
//! \returns `val`.
template <class T>
constexpr T* optional<T&>::operator->() const noexcept {
    return value_;
}

//! \hardexpects `has_value()` is `true`.
//! \returns `*val`.
template <class T>
constexpr T& optional<T&>::operator*() const noexcept {
    return *value_;
}

//! \returns `val != nullptr`.
template <class T>
constexpr optional<T&>::operator bool() const noexcept {
    return value_ != nullptr;
}

//! \returns `val != nullptr`.
template <class T>
constexpr bool optional<T&>::has_value() const noexcept {
    return value_ != nullptr;
}

//! \effects-equiv
template <class T>
constexpr T& optional<T&>::value() const {
    if (has_value()) {
        return *value_;
    }
    throw bad_optional_access();
}

//! \constraints `T` is a non-array object type.
//! \covers is_object_v<T>
//! \covers !is_array_v<T>
//!
//! \remarks The return type is unspecified if
//! `T` is an array type or a non-object type.
//! \note
//! This is to avoid the declaration being ill-formed.
//! \endnote
//! \effects-equiv
template <class T>
template <class U>
    requires (std::is_object_v<T> && !std::is_array_v<T>)
constexpr std::decay_t<T> optional<T&>::value_or(U&& u) const {
    using X = std::remove_cv_t<T>;
    static_assert(std::is_convertible_v<T&, X>, "remove_cv_t<T> must be constructible from a T&");
    static_assert(std::is_convertible_v<U, X>, "Must be able to convert u to remove_cv_t<T>");
    if (has_value()) {
        return *value_;
    }
    return std::forward<U>(u);
}

// \rSec3[optional.ref.monadic]{Monadic operations}

//! Let `U` be `invoke_result_t<F, T&>`.
//! \mandates `U` is a specialization of `optional`.
//! \covers detail::is_optional<U>
//! \effects-equiv
template <class T>
template <class F>
constexpr auto optional<T&>::and_then(F&& f) const {
    using U = std::invoke_result_t<F, T&>;
    static_assert(detail::is_optional<U>, "F must return an optional");
    if (has_value()) {
        return std::invoke(std::forward<F>(f), *value_);
    } else {
        return std::remove_cvref_t<U>();
    }
}

/**
 * @brief Transforms the value contained in the optional, if it exists.
 *
 * @return optional<std::invoke_result_t<F, T&>>
 */

//! Let `U` be `invoke_result_t<F, T&>`.
//! \mandates `U` is a valid contained type for `optional`.
//! \covers !is_same_v<remove_cvref_t<U>, in_place_t>
//! \covers !is_same_v<remove_cvref_t<U>, nullopt_t>
//! \covers (is_object_v<U> && !is_array_v<U>) || is_lvalue_reference_v<U>
//! The declaration
//! \codeblock
//! U u(invoke(std::forward<F>(f), *@\exposid{val}@));
//! \endcodeblock
//! is well-formed for some invented variable `u`.
//! \note There is no requirement that `U` is movable\iref{dcl.init.general}.
//! \endnote
//! \returns If `*this` contains a value, an `optional<U>` object
//! whose contained value is direct-non-list-initialized with
//! `invoke(std::forward<F>(f), *val)`;
//! otherwise, `optional<U>()`.
template <class T>
template <class F>
constexpr optional<std::invoke_result_t<F, T&>> optional<T&>::transform(F&& f) const {
    using U = std::invoke_result_t<F, T&>;
    static_assert(!std::is_same_v<std::remove_cvref_t<U>, in_place_t>, "Result must not be in_place_t");
    static_assert(!std::is_same_v<std::remove_cvref_t<U>, nullopt_t>, "Result must not be nullopt_t");
    static_assert((std::is_object_v<U> && !std::is_array_v<U>) || std::is_lvalue_reference_v<U>,
                  "Result must be an non-array object or an lvalue reference");
    if (has_value()) {
        return optional<U>{detail::from_function, std::forward<F>(f), *value_};
    } else {
        return optional<U>{};
    }
}

/**
 * @brief Returns an optional containing the contained value if it has one, or the result of calling
 * \p `f` if it does not.
 *
 * @tparam T
 * @tparam F
 * @param f
 * @return optional<T&>
 */

//! \effects-equiv
template <class T>
template <class F>
constexpr optional<T&> optional<T&>::or_else(F&& f) const

    requires (std::invocable<F>)
{
    static_assert(std::is_same_v<std::remove_cvref_t<std::invoke_result_t<F>>, optional>,
                  "Result must be an optional");
    if (has_value()) {
        return *this;
    } else {
        return std::forward<F>(f)();
    }
}

// \rSec3[optional.ref.mod]{Modifiers}

//! \effects Assigns `nullptr` to `val`.
//! \ensures `*this` does not contain a value.
template <class T>
constexpr void optional<T&>::reset() noexcept {
    value_ = nullptr;
}

// \rSec3[optional.ref.expos]{Exposition only helper functions}

// \rSec2[optional.nullopt]{No-value state indicator}

// \rSec2[optional.relops]{Relational operators}

//! \constraints The expression `*x == *y` is well-formed and
//! its result is convertible to `bool`.
//! \note
//! `T` need not be \oldconcept{EqualityComparable}.
//! \endnote
//! \returns If `x.has_value() != y.has_value()`, `false`; otherwise if `x.has_value() == false`, `true`; otherwise `*x
//! == *y`.
//! \remarks Specializations of this function template
//! for which `*x == *y` is a core constant expression
//! are constexpr functions.
template <class T, class U>
constexpr bool operator==(const optional<T>& x, const optional<U>& y)
    requires detail::optional_eq_rel<T, U>
{
    return static_cast<bool>(x) == static_cast<bool>(y) && (!x || *x == *y);
}

//! \constraints The expression `*x != *y` is well-formed and
//! its result is convertible to `bool`.
//! \returns If `x.has_value() != y.has_value()`, `true`;
//! otherwise, if `x.has_value() == false`, `false`;
//! otherwise `*x != *y`.
//! \remarks Specializations of this function template
//! for which `*x != *y` is a core constant expression
//! are constexpr functions.
template <class T, class U>
constexpr bool operator!=(const optional<T>& x, const optional<U>& y)
    requires detail::optional_ne_rel<T, U>
{
    return static_cast<bool>(x) != static_cast<bool>(y) || (static_cast<bool>(x) && *x != *y);
}

//! \constraints `*x < *y` is well-formed
//! and its result is convertible to `bool`.
//! \returns If `!y`, `false`;
//! otherwise, if `!x`, `true`;
//! otherwise `*x < *y`.
//! \remarks Specializations of this function template
//! for which `*x < *y` is a core constant expression
//! are constexpr functions.
template <class T, class U>
constexpr bool operator<(const optional<T>& x, const optional<U>& y)
    requires detail::optional_lt_rel<T, U>
{
    return static_cast<bool>(y) && (!x || *x < *y);
}

//! \constraints The expression `*x > *y` is well-formed and
//! its result is convertible to `bool`.
//! \returns If `!x`, `false`;
//! otherwise, if `!y`, `true`;
//! otherwise `*x > *y`.
//! \remarks Specializations of this function template
//! for which `*x > *y` is a core constant expression
//! are constexpr functions.
template <class T, class U>
constexpr bool operator>(const optional<T>& x, const optional<U>& y)
    requires detail::optional_gt_rel<T, U>
{
    return static_cast<bool>(x) && (!y || *x > *y);
}

//! \constraints The expression `*x <= *y` is well-formed and
//! its result is convertible to `bool`.
//! \returns If `!x`, `true`;
//! otherwise, if `!y`, `false`;
//! otherwise `*x <= *y`.
//! \remarks Specializations of this function template
//! for which `*x <= *y` is a core constant expression
//! are constexpr functions.
template <class T, class U>
constexpr bool operator<=(const optional<T>& x, const optional<U>& y)
    requires detail::optional_le_rel<T, U>
{
    return !x || (static_cast<bool>(y) && *x <= *y);
}

//! \constraints The expression `*x >= *y` is well-formed and
//! its result is convertible to `bool`.
//! \returns If `!y`, `true`;
//! otherwise, if `!x`, `false`;
//! otherwise `*x >= *y`.
//! \remarks Specializations of this function template
//! for which `*x >= *y` is a core constant expression
//! are constexpr functions.
template <class T, class U>
constexpr bool operator>=(const optional<T>& x, const optional<U>& y)
    requires detail::optional_ge_rel<T, U>
{
    return !y || (static_cast<bool>(x) && *x >= *y);
}

//! \returns If `x && y`, `*x <=> *y`; otherwise `x.has_value() <=> y.has_value()`.
//! \remarks Specializations of this function template
//! for which `*x <=> *y` is a core constant expression
//! are constexpr functions.
template <class T, std::three_way_comparable_with<T> U>
constexpr std::compare_three_way_result_t<T, U> operator<=>(const optional<T>& x, const optional<U>& y) {
    return x && y ? *x <=> *y : bool(x) <=> bool(y);
}

// \rSec2[optional.nullops]{Comparison with \tcode{nullopt}}

//! \returns `!x`.
template <class T>
constexpr bool operator==(const optional<T>& x, nullopt_t) noexcept {
    return !x;
}

//! \returns `x.has_value() <=> false`.
template <class T>
constexpr std::strong_ordering operator<=>(const optional<T>& x, nullopt_t) noexcept {
    return bool(x) <=> false;
}

// \rSec2[optional.comp.with.t]{Comparison with \tcode{T}}

//! \constraints `U` is not a specialization of `optional`.
//! \covers !detail::is_optional<U>
//! The expression `*x == v` is well-formed and
//! its result is convertible to `bool`.
//! \note
//! `T` need not be \oldconcept{EqualityComparable}.
//! \endnote
//! \effects Equivalent to:
//! \codeblock
//! if (x.has_value())
//!   return *x == v;
//! return false;
//! \endcodeblock
template <class T, class U>
constexpr bool operator==(const optional<T>& x, const U& v)
    requires (!detail::is_optional<U>) && detail::optional_eq_rel<T, U>
{
    return x && *x == v;
}

//! \constraints `T` is not a specialization of `optional`.
//! \covers !detail::is_optional<T>
//! The expression `v == *x` is well-formed and
//! its result is convertible to `bool`.
//! \effects Equivalent to:
//! \codeblock
//! if (x.has_value())
//!   return v == *x;
//! return false;
//! \endcodeblock
template <class T, class U>
constexpr bool operator==(const T& v, const optional<U>& x)
    requires (!detail::is_optional<T>) && detail::optional_eq_rel<T, U>
{
    return x && v == *x;
}

//! \constraints `U` is not a specialization of `optional`.
//! \covers !detail::is_optional<U>
//! The expression `*x != v` is well-formed and
//! its result is convertible to `bool`.
//! \effects Equivalent to:
//! \codeblock
//! if (x.has_value())
//!   return *x != v;
//! return true;
//! \endcodeblock
template <class T, class U>
constexpr bool operator!=(const optional<T>& x, const U& v)
    requires (!detail::is_optional<U>) && detail::optional_ne_rel<T, U>
{
    return !x || *x != v;
}

//! \constraints `T` is not a specialization of `optional`.
//! \covers !detail::is_optional<T>
//! The expression `v != *x` is well-formed and
//! its result is convertible to `bool`.
//! \effects Equivalent to:
//! \codeblock
//! if (x.has_value())
//!   return v != *x;
//! return true;
//! \endcodeblock
template <class T, class U>
constexpr bool operator!=(const T& v, const optional<U>& x)
    requires (!detail::is_optional<T>) && detail::optional_ne_rel<T, U>
{
    return !x || v != *x;
}

//! \constraints `U` is not a specialization of `optional`.
//! \covers !detail::is_optional<U>
//! The expression `*x < v` is well-formed and
//! its result is convertible to `bool`.
//! \effects Equivalent to:
//! \codeblock
//! if (x.has_value())
//!   return *x < v;
//! return true;
//! \endcodeblock
template <class T, class U>
constexpr bool operator<(const optional<T>& x, const U& v)
    requires (!detail::is_optional<U>) && detail::optional_lt_rel<T, U>
{
    return !x || *x < v;
}

//! \constraints `T` is not a specialization of `optional`.
//! \covers !detail::is_optional<T>
//! The expression `v < *x` is well-formed and
//! its result is convertible to `bool`.
//! \effects Equivalent to:
//! \codeblock
//! if (x.has_value())
//!   return v < *x;
//! return false;
//! \endcodeblock
template <class T, class U>
constexpr bool operator<(const T& v, const optional<U>& x)
    requires (!detail::is_optional<T>) && detail::optional_lt_rel<T, U>
{
    return x && v < *x;
}

//! \constraints `U` is not a specialization of `optional`.
//! \covers !detail::is_optional<U>
//! The expression `*x > v` is well-formed and
//! its result is convertible to `bool`.
//! \effects Equivalent to:
//! \codeblock
//! if (x.has_value())
//!   return *x > v;
//! return false;
//! \endcodeblock
template <class T, class U>
constexpr bool operator>(const optional<T>& x, const U& v)
    requires (!detail::is_optional<U>) && detail::optional_gt_rel<T, U>
{
    return x && *x > v;
}

//! \constraints `T` is not a specialization of `optional`.
//! \covers !detail::is_optional<T>
//! The expression `v > *x` is well-formed and
//! its result is convertible to `bool`.
//! \effects Equivalent to:
//! \codeblock
//! if (x.has_value())
//!   return v > *x;
//! return true;
//! \endcodeblock
template <class T, class U>
constexpr bool operator>(const T& v, const optional<U>& x)
    requires (!detail::is_optional<T>) && detail::optional_gt_rel<T, U>
{
    return !x || v > *x;
}

//! \constraints `U` is not a specialization of `optional`.
//! \covers !detail::is_optional<U>
//! The expression `*x <= v` is well-formed and
//! its result is convertible to `bool`.
//! \effects Equivalent to:
//! \codeblock
//! if (x.has_value())
//!   return *x <= v;
//! return true;
//! \endcodeblock
template <class T, class U>
constexpr bool operator<=(const optional<T>& x, const U& v)
    requires (!detail::is_optional<U>) && detail::optional_le_rel<T, U>
{
    return !x || *x <= v;
}

//! \constraints `T` is not a specialization of `optional`.
//! \covers !detail::is_optional<T>
//! The expression `v <= *x` is well-formed and
//! its result is convertible to `bool`.
//! \effects Equivalent to:
//! \codeblock
//! if (x.has_value())
//!   return v <= *x;
//! return false;
//! \endcodeblock
template <class T, class U>
constexpr bool operator<=(const T& v, const optional<U>& x)
    requires (!detail::is_optional<T>) && detail::optional_le_rel<T, U>
{
    return x && v <= *x;
}

//! \constraints `U` is not a specialization of `optional`.
//! \covers !detail::is_optional<U>
//! The expression `*x >= v` is well-formed and
//! its result is convertible to `bool`.
//! \effects Equivalent to:
//! \codeblock
//! if (x.has_value())
//!   return *x >= v;
//! return false;
//! \endcodeblock
template <class T, class U>
constexpr bool operator>=(const optional<T>& x, const U& v)
    requires (!detail::is_optional<U>) && detail::optional_ge_rel<T, U>
{
    return x && *x >= v;
}

//! \constraints `T` is not a specialization of `optional`.
//! \covers !detail::is_optional<T>
//! The expression `v >= *x` is well-formed and
//! its result is convertible to `bool`.
//! \effects Equivalent to:
//! \codeblock
//! if (x.has_value())
//!   return v >= *x;
//! return true;
//! \endcodeblock
template <class T, class U>
constexpr bool operator>=(const T& v, const optional<U>& x)
    requires (!detail::is_optional<T>) && detail::optional_ge_rel<T, U>
{
    return !x || v >= *x;
}

//! \constraints-in-decl
//! \effects Equivalent to: `return x.has_value() ? *x <=> v : strong_ordering::less;`
template <class T, class U>
    requires (!is_derived_from_optional<U>) && std::three_way_comparable_with<T, U>
constexpr std::compare_three_way_result_t<T, U> operator<=>(const optional<T>& x, const U& v) {
    return bool(x) ? *x <=> v : std::strong_ordering::less;
}

// \rSec2[optional.specalg]{Specialized algorithms}

//! \effects Calls `x.swap(y)`.
template <class T>
constexpr void swap(optional<T>& x, optional<T>& y) noexcept(noexcept(x.swap(y)))
    requires std::is_move_constructible_v<T> && std::is_swappable_v<T>
{
    return x.swap(y);
}

//! \constraints The call to `make_optional` does not use
//! an explicit \grammarterm{template-argument-list} that
//! begins with a type \grammarterm{template-argument}, and
//! `is_constructible_v<decay_t<T>, T>` is `true`.
//! \effects-equiv
template <int, class T>
constexpr optional<std::decay_t<T>>
make_optional(T&& v) noexcept(std::is_nothrow_constructible_v<optional<std::decay_t<T>>, T>)
    requires std::is_constructible_v<std::decay_t<T>, T>
{
    return optional<std::decay_t<T>>{std::forward<T>(v)};
}

//! \effects-equiv
template <class T, class... Args>
constexpr optional<T> make_optional(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
    requires std::is_constructible_v<T, Args...>
{
    static_assert(!std::is_reference_v<T>, "May not make an optional containing a reference");
    return optional<T>{in_place, std::forward<Args>(args)...};
}

//! \effects-equiv
template <class T, class U, class... Args>
constexpr optional<T>
make_optional(std::initializer_list<U> il,
              Args&&... args) noexcept(std::is_nothrow_constructible_v<T, std::initializer_list<U>&, Args...>)
    requires std::is_constructible_v<T, std::initializer_list<U>&, Args...>
{
    static_assert(!std::is_reference_v<U>, "Can not make optional from initializer_list of references.");
    return optional<T>{in_place, il, std::forward<Args>(args)...};
}

} // namespace beman::optional

namespace std {
// \rSec2[optional.hash]{Hash support}

//! The specialization `hash<optional<T>>` is enabled\iref{unord.hash}
//! if and only if `hash<remove_const_t<T>>` is enabled.
//! When enabled, for an object `o` of type `optional<T>`,
//! if `o.has_value() == true`, then `hash<optional<T>>()(o)`
//! evaluates to the same value as `hash<remove_const_t<T>>()(*o)`;
//! otherwise it evaluates to an unspecified value.
//! The member functions are not guaranteed to be `noexcept`.
//! \verbatim-itemdecl
//! template<class T> struct hash<optional<T>>;

//! \merge
template <typename T>
    requires requires(T a) {
        { std::hash<remove_const_t<T>>{}(a) } -> std::convertible_to<std::size_t>;
    }
struct hash<beman::optional::optional<T>> {
    static_assert(!is_reference_v<T>, "hash is not enabled for reference types");
    /**
     * @brief Hashes the optional value.
     *
     * @param o
     * @return size_t
     */
    size_t operator()(const beman::optional::optional<T>& o) const noexcept(noexcept(hash<remove_const_t<T>>{}(*o))) {
        if (o) {
            return std::hash<std::remove_const_t<T>>{}(*o);
        } else {
            return 0;
        }
    }
};
} // namespace std

#endif // BEMAN_OPTIONAL_OPTIONAL_HPP
